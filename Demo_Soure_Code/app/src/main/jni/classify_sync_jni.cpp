/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: classify sync jni
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <jni.h>
#include <string>
#include <memory.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <climits>

#include "HiAiModelManagerService.h"
#include "cloud_service.h"

#define LOG_TAG "SYNC_DDK_MSG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define IF_BOOL_EXEC(expr, exec_expr) \
    { \
        if (expr) { \
            exec_expr; \
        } \
    }

using namespace std;
using namespace hiai;

static shared_ptr<AiModelMngerClient> g_clientSync = nullptr;
static vector<vector<TensorDimension>> inputDimension;
static vector<vector<TensorDimension>> outputDimension;
static vector<vector<shared_ptr<AiTensor>>> input_tensor;
static vector<vector<shared_ptr<AiTensor>>> output_tensor;

static map<string, int> g_syncNameToIndex;
static long time_use_sync = 0;

static const int FAILED = -1;

bool CheckMulOveflow(const int& number, const int& channel, const int& h, const int& w)
{
    if (number > 0 && channel > 0 && h > 0 && w > 0) {
        if (number > (INT_MAX / channel / h / w)) {
            LOGE("multiply Integer can result in overflow!");
            return false;
        }
        return true;
    } else {
        LOGE("Input should be positive!");
        return false;
    }
}

void ResourceDestroy(shared_ptr<AiModelBuilder>& modelBuilder, vector<MemBuffer*>& memBuffers)
{
    if (modelBuilder == nullptr) {
        LOGE("[AI_DEMO_SYNC] modelBuilder is null.");
        return;
    }

    for (auto tmpBuffer : memBuffers) {
        modelBuilder->MemBufferDestroy(tmpBuffer);
    }
    return;
}

int LoadSync(vector<string>& names, vector<string>& modelPaths, shared_ptr<AiModelMngerClient>& client)
{
    int ret;
    vector<shared_ptr<AiModelDescription>> modelDescs;
    vector<MemBuffer*> memBuffers;
    shared_ptr<AiModelBuilder> modelBuilder = make_shared<AiModelBuilder>(client);
    if (modelBuilder == nullptr) {
        LOGI("[AI_DEMO_SYNC] creat modelBuilder failed.");
        return FAILED;
    }

    for (size_t i = 0; i < modelPaths.size(); ++i) {
        string modelPath = modelPaths[i];
        string modelName = names[i];
        g_syncNameToIndex[modelName] = i;

        // We can achieve the optimization by loading model from OM file.
        LOGI("[AI_DEMO_SYNC] modelpath is %s\n.", modelPath.c_str());
        MemBuffer* buffer = modelBuilder->InputMemBufferCreate(modelPath);
        if (buffer == nullptr) {
            LOGE("[AI_DEMO_SYNC] cannot find the model file.");
            return FAILED;
        }
        memBuffers.push_back(buffer);

        string modelNameFull = string(modelName) + string(".om");
        shared_ptr<AiModelDescription> desc =
            make_shared<AiModelDescription>(modelNameFull, 3, 0,
                0, 0);
        if (desc == nullptr) {
            LOGE("[AI_DEMO_SYNC] LoadModelSync: desc make_shared error.");
            ResourceDestroy(modelBuilder, memBuffers);
            return FAILED;
        }
        desc->SetModelBuffer(buffer->GetMemBufferData(), buffer->GetMemBufferSize());

        LOGE("[AI_DEMO_SYNC] loadModel %s IO Tensor.", desc->GetName().c_str());
        modelDescs.push_back(desc);
    }

    ret = client->Load(modelDescs);
    ResourceDestroy(modelBuilder, memBuffers);
    if (ret != 0) {
        LOGE("[AI_DEMO_SYNC] Model Load Failed.");
        return FAILED;
    }
    return SUCCESS;
}

int UpdateSyncInputTensorVec(vector<TensorDimension>& inputDims, bool isUseAipp, string& modelName)
{
    input_tensor.clear();
    vector<shared_ptr<AiTensor>> inputTensors;
    int ret = FAILED;
    for (auto inDim : inputDims) {
        shared_ptr<AiTensor> input = make_shared<AiTensor>();
        if (isUseAipp) {
            ret = input->Init(inDim.GetNumber(), inDim.GetHeight(), inDim.GetWidth(), AiTensorImage_YUV420SP_U8);
            LOGI("[AI_DEMO_SYNC] model %s uses AIPP(input).", modelName.c_str());
        } else {
            ret = input->Init(&inDim);
            LOGI("[AI_DEMO_SYNC] model %s does not use AIPP(input).", modelName.c_str());
        }
        IF_BOOL_EXEC(ret != SUCCESS, LOGE("[AI_DEMO_SYNC] model %s AiTensor Init failed(input).", modelName.c_str());
                     return FAILED);
        inputTensors.push_back(input);
    }
    input_tensor.push_back(inputTensors);
    IF_BOOL_EXEC(input_tensor.size() == 0, LOGE("[AI_DEMO_SYNC] input_tensor.size() == 0"); return FAILED);

    return SUCCESS;
}

int UpdateSyncOutputTensorVec(vector<TensorDimension>& outputDims, string& modelName)
{
    output_tensor.clear();
    vector<shared_ptr<AiTensor>> outputTensors;
    int ret = FAILED;
    for (auto outDim : outputDims) {
        shared_ptr<AiTensor> output = make_shared<AiTensor>();
        ret = output->Init(&outDim);
        IF_BOOL_EXEC(ret != SUCCESS, LOGE("[AI_DEMO_SYNC] model %s AiTensor Init failed(output).", modelName.c_str());
                     return FAILED);
        outputTensors.push_back(output);
    }
    output_tensor.push_back(outputTensors);
    IF_BOOL_EXEC(output_tensor.size() == 0, LOGE("[AI_DEMO_SYNC] output_tensor.size() == 0"); return FAILED);

    return SUCCESS;
}

shared_ptr<AiModelMngerClient> LoadModelSync(JNIEnv* env, jobject jcontext,
        vector<string> names, vector<string> modelPaths, vector<bool> Aipps)
{
    shared_ptr<AiModelMngerClient> clientSync = make_shared<AiModelMngerClient>();
    IF_BOOL_EXEC(clientSync == nullptr, LOGE("[AI_DEMO_SYNC] Model Manager Client make_shared error.");
                 return nullptr);
    jobject context = env->NewGlobalRef(jcontext);
    bool ret = CloudService::Enable(env, context);
    if (!ret) {
        LOGW("[AI_DEMO_SYNC] Cloud service enable failed. Will run as normal mode.");
    }
    ret = clientSync->Init(nullptr);
    IF_BOOL_EXEC(ret != SUCCESS, LOGE("[AI_DEMO_SYNC] Model Manager Init Failed."); return nullptr);
    ret = LoadSync(names, modelPaths, clientSync);
    IF_BOOL_EXEC(ret != SUCCESS, LOGE("[AI_DEMO_ASYNC] LoadSync Failed."); return nullptr);

    inputDimension.clear();
    outputDimension.clear();
    for (size_t i = 0; i < names.size(); ++i) {
        string modelName = names[i];
        bool isUseAipp = Aipps[i];
        LOGI("[AI_DEMO_SYNC] Get model %s IO Tensor. Use AIPP %d", modelName.c_str(), isUseAipp);
        vector<TensorDimension> inputDims, outputDims;
        ret = clientSync->GetModelIOTensorDim(string(modelName) + string(".om"), inputDims, outputDims);
        IF_BOOL_EXEC(ret != SUCCESS, LOGE("[AI_DEMO_SYNC] Get Model IO Tensor Dimension failed,ret is %d.", ret);
                     return nullptr);
        IF_BOOL_EXEC(inputDims.size() == 0, LOGE("[AI_DEMO_SYNC] inputDims.size() == 0"); return nullptr);
        inputDimension.push_back(inputDims);
        outputDimension.push_back(outputDims);
        IF_BOOL_EXEC(UpdateSyncInputTensorVec(inputDims, isUseAipp, modelName) != SUCCESS, return nullptr);
        IF_BOOL_EXEC(UpdateSyncOutputTensorVec(outputDims, modelName) != SUCCESS, return nullptr);
    }
    env->DeleteGlobalRef(context);
    return clientSync;
}

extern "C" JNIEXPORT jlong JNICALL Java_com_huawei_hiaidemo_utils_ModelManager_GetTimeUseSync(JNIEnv* env, jclass type)
{
    return time_use_sync;
}

int SetField(JNIEnv* env, int len, jobject& modelInfo, jmethodID listGet)
{
    for (int i = 0; i < len; i++) {
        jobject modelInfoObj = env->CallObjectMethod(modelInfo, listGet, i);
        jclass modelInfoClass = env->GetObjectClass(modelInfoObj);
        jfieldID inputIdN = env->GetFieldID(modelInfoClass, "input_N", "I");
        jfieldID inputIdC = env->GetFieldID(modelInfoClass, "input_C", "I");
        jfieldID inputIdH = env->GetFieldID(modelInfoClass, "input_H", "I");
        jfieldID inputIdW = env->GetFieldID(modelInfoClass, "input_W", "I");
        jfieldID inputNumber = env->GetFieldID(modelInfoClass, "input_Number", "I");
        env->SetIntField(modelInfoObj, inputIdN, inputDimension[i][0].GetNumber());
        env->SetIntField(modelInfoObj, inputIdC, inputDimension[i][0].GetChannel());
        env->SetIntField(modelInfoObj, inputIdH, inputDimension[i][0].GetHeight());
        env->SetIntField(modelInfoObj, inputIdW, inputDimension[i][0].GetWidth());
        env->SetIntField(modelInfoObj, inputNumber, inputDimension[i].size());

        jfieldID outputIdN = env->GetFieldID(modelInfoClass, "output_N", "I");
        jfieldID outputIdC = env->GetFieldID(modelInfoClass, "output_C", "I");
        jfieldID outputIdH = env->GetFieldID(modelInfoClass, "output_H", "I");
        jfieldID outputIdW = env->GetFieldID(modelInfoClass, "output_W", "I");
        jfieldID outputNumber = env->GetFieldID(modelInfoClass, "output_Number", "I");
        env->SetIntField(modelInfoObj, outputIdN, outputDimension[i][0].GetNumber());
        env->SetIntField(modelInfoObj, outputIdC, outputDimension[i][0].GetChannel());
        env->SetIntField(modelInfoObj, outputIdH, outputDimension[i][0].GetHeight());
        env->SetIntField(modelInfoObj, outputIdW, outputDimension[i][0].GetWidth());
        env->SetIntField(modelInfoObj, outputNumber, outputDimension[i].size());
    }

    return SUCCESS;
}

extern "C" JNIEXPORT jobject JNICALL Java_com_huawei_hiaidemo_utils_ModelManager_loadModelSync(
    JNIEnv* env, jclass type, jobject modelInfo,  jobject context)
{
    jclass classList = env->GetObjectClass(modelInfo);
    IF_BOOL_EXEC(classList == nullptr, LOGE("[AI_DEMO_SYNC] can not find List class."); return nullptr);
    jmethodID listGet = env->GetMethodID(classList, "get", "(I)Ljava/lang/Object;");
    jmethodID listSize = env->GetMethodID(classList, "size", "()I");
    int len = static_cast<int>(env->CallIntMethod(modelInfo, listSize));
    vector<string> names, modelPaths;
    vector<bool> aipps;
    for (int i = 0; i < len; i++) {
        jobject modelInfoObj = env->CallObjectMethod(modelInfo, listGet, i);
        jclass modelInfoClass = env->GetObjectClass(modelInfoObj);
        jmethodID getOfflineModelName = env->GetMethodID(modelInfoClass, "getOfflineModelName", "()Ljava/lang/String;");
        jmethodID getModelPath = env->GetMethodID(modelInfoClass, "getModelPath", "()Ljava/lang/String;");
        jmethodID getUseAIPP = env->GetMethodID(modelInfoClass, "getUseAIPP", "()Z");
        IF_BOOL_EXEC(getOfflineModelName == nullptr, LOGE("[AI_DEMO_SYNC] can not find getOfflineModelName method.");
                     return nullptr);
        IF_BOOL_EXEC(getModelPath == nullptr, LOGE("[AI_DEMO_SYNC] can not find getModelPath method.");
                     return nullptr);
        IF_BOOL_EXEC(getUseAIPP == nullptr, LOGE("[AI_DEMO_SYNC] can not find getUseAIPP method."); return nullptr);

        jboolean useaipp = (jboolean)env->CallBooleanMethod(modelInfoObj, getUseAIPP);
        jstring modelname = (jstring)env->CallObjectMethod(modelInfoObj, getOfflineModelName);
        jstring modelpath = (jstring)env->CallObjectMethod(modelInfoObj, getModelPath);
        const char* modelName = env->GetStringUTFChars(modelname, 0);
        LOGI("[AI_DEMO_SYNC] modelName is %s .", modelName);
        IF_BOOL_EXEC(modelName == nullptr, LOGE("[AI_DEMO_SYNC] modelName is invalid."); return nullptr);
        const char* modelPath = env->GetStringUTFChars(modelpath, 0);
        IF_BOOL_EXEC(modelPath == nullptr, LOGE("[AI_DEMO_SYNC] modelPath is invalid."); return nullptr);
        LOGE("[AI_DEMO_SYNC] useaipp is %d.", bool(useaipp == JNI_TRUE));
        aipps.push_back(bool(useaipp == JNI_TRUE));
        names.push_back(string(modelName));
        modelPaths.push_back(string(modelPath));
    }
    // load
    IF_BOOL_EXEC(!g_clientSync, g_clientSync = LoadModelSync(env, context,
            names, modelPaths, aipps); IF_BOOL_EXEC(
        g_clientSync == nullptr, LOGE("[AI_DEMO_SYNC] g_clientSync loadModel is nullptr."); return nullptr));
    LOGI("[AI_DEMO_SYNC] sync load model INPUT NCHW : %d %d %d %d.", inputDimension[0][0].GetNumber(),
        inputDimension[0][0].GetChannel(), inputDimension[0][0].GetHeight(), inputDimension[0][0].GetWidth());
    LOGI("[AI_DEMO_SYNC] sync load model OUTPUT NCHW : %d %d %d %d.", outputDimension[0][0].GetNumber(),
        outputDimension[0][0].GetChannel(), outputDimension[0][0].GetHeight(), outputDimension[0][0].GetWidth());
    SetField(env, len, modelInfo, listGet);

    return modelInfo;
}

int RunProcess(JNIEnv* env, jobject bufList, jmethodID listGet, int vecIndex, int listLength, const char* modelName)
{
    if (static_cast<int64_t>(input_tensor.size()) <= static_cast<int64_t>(vecIndex)) {
        LOGE("input_tensor[%d] is invalid", vecIndex);
        return FAILED;
    }

    for (int i = 0; i < listLength; i++) {
        jbyteArray buf_ = (jbyteArray)(env->CallObjectMethod(bufList, listGet, i));
        jbyte* dataBuff = nullptr;
        int dataBuffSize = 0;
        dataBuff = env->GetByteArrayElements(buf_, nullptr);
        dataBuffSize = env->GetArrayLength(buf_);
        IF_BOOL_EXEC(input_tensor[vecIndex][i]->GetSize() != dataBuffSize,
                     LOGE("[AI_DEMO_SYNC] input->GetSize(%d) != dataBuffSize(%d) ",
                         input_tensor[vecIndex][i]->GetSize(), dataBuffSize);
                     return FAILED);
        memmove(input_tensor[vecIndex][i]->GetBuffer(), dataBuff, (size_t)dataBuffSize);
        env->ReleaseByteArrayElements(buf_, dataBuff, 0);
    }

    AiContext context;
    string key = "model_name";
    string value = modelName;
    value += ".om";
    context.AddPara(key, value);
    LOGI("[AI_DEMO_SYNC] runModel modelname:%s", modelName);
    // before process
    struct timeval tpstart, tpend;
    gettimeofday(&tpstart, nullptr);
    int istamp;
    if (static_cast<int64_t>(output_tensor.size()) <= static_cast<int64_t>(vecIndex)) {
        LOGE("output_tensor[%d] is invalid", vecIndex);
        return FAILED;
    }
    int ret = g_clientSync->Process(context, input_tensor[vecIndex], output_tensor[vecIndex], 1000, istamp);
    IF_BOOL_EXEC(ret, LOGE("[AI_DEMO_SYNC] Runmodel Failed!, ret=%d\n", ret); return FAILED);
    // after process
    gettimeofday(&tpend, nullptr);
    float time_use = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
    time_use_sync = time_use / 1000;
    LOGI("[AI_DEMO_SYNC] inference time %f ms.\n", time_use / 1000);

    return SUCCESS;
}

int HandleOutputList(JNIEnv* env, jobject& outputList, int outputTensorSize, int vecIndex, jmethodID listAdd)
{
    if (vecIndex < 0 || outputTensorSize < 0) {
        LOGE("[AI_DEMO_SYNC] vecIndex or outputTensorSize is negative number.");
        return FAILED;
    }
    if (static_cast<vector<int>::size_type>(vecIndex) >= output_tensor.size()) {
        LOGE("[AI_DEMO_SYNC] vecIndex is greater than output_tensor.size.");
        return FAILED;
    }
    for (long j = 0; j < outputTensorSize; j++) {
        float* outputBuffer = (float*)output_tensor[vecIndex][j]->GetBuffer();
        if (outputBuffer == nullptr) {
            LOGE("[AI_DEMO_SYNC] outputBuffer is null.");
            return FAILED;
        }
        if (!CheckMulOveflow(outputDimension[vecIndex][j].GetNumber(), outputDimension[vecIndex][j].GetChannel(),
            outputDimension[vecIndex][j].GetHeight(), outputDimension[vecIndex][j].GetWidth())) {
            LOGE("input size is invalid!");
            return FAILED;
        }
        int outputsize = outputDimension[vecIndex][j].GetNumber() * outputDimension[vecIndex][j].GetChannel() *
            outputDimension[vecIndex][j].GetHeight() * outputDimension[vecIndex][j].GetWidth();
        jfloatArray result = env->NewFloatArray(outputsize);
        jfloat temp[outputsize];
        for (int i = 0; i < outputsize; i++) {
            temp[i] = outputBuffer[i];
        }
        env->SetFloatArrayRegion(result, 0, outputsize, temp);
        jboolean outputAdd = env->CallBooleanMethod(outputList, listAdd, result);
        LOGI("[AI_DEMO_SYNC] outputAdd result  is %d .", outputAdd);
    }

    return SUCCESS;
}

extern "C" JNIEXPORT jobject JNICALL Java_com_huawei_hiaidemo_utils_ModelManager_runModelSync(
    JNIEnv* env, jclass type, jobject modelInfo, jobject bufList)
{
    // check params
    IF_BOOL_EXEC(env == nullptr, LOGE("[AI_DEMO_SYNC] runModelSync env is null"); return nullptr);
    jclass ModelInfo = env->GetObjectClass(modelInfo);
    IF_BOOL_EXEC(ModelInfo == nullptr, LOGE("[AI_DEMO_SYNC] can not find ModelInfo class."); return nullptr);
    IF_BOOL_EXEC(bufList == nullptr, LOGE("[AI_DEMO_SYNC] buf_ is null."); return nullptr);
    jmethodID getOfflineModelName = env->GetMethodID(ModelInfo, "getOfflineModelName", "()Ljava/lang/String;");
    jmethodID getModelPath = env->GetMethodID(ModelInfo, "getModelPath", "()Ljava/lang/String;");
    IF_BOOL_EXEC(getOfflineModelName == nullptr, LOGE("[AI_DEMO_SYNC] can not find getOfflineModelName method.");
                 return nullptr);
    IF_BOOL_EXEC(getModelPath == nullptr, LOGE("[AI_DEMO_SYNC] can not find getModelPath method."); return nullptr);
    jstring modelname = (jstring)env->CallObjectMethod(modelInfo, getOfflineModelName);
    jstring modelpath = (jstring)env->CallObjectMethod(modelInfo, getModelPath);
    const char* modelName = env->GetStringUTFChars(modelname, 0);
    IF_BOOL_EXEC(modelName == nullptr, LOGE("[AI_DEMO_SYNC] modelName is invalid."); return nullptr);
    int vecIndex = g_syncNameToIndex[modelName];
    const char* modelPath = env->GetStringUTFChars(modelpath, 0);
    IF_BOOL_EXEC(modelPath == nullptr, LOGE("[AI_DEMO_SYNC] modelPath is invalid."); return nullptr);
    // buf_list
    jclass classList = env->GetObjectClass(bufList);
    IF_BOOL_EXEC(classList == nullptr, LOGE("[AI_DEMO_SYNC] can not find List class."); return nullptr);
    jmethodID listGet = env->GetMethodID(classList, "get", "(I)Ljava/lang/Object;");
    jmethodID listSize = env->GetMethodID(classList, "size", "()I");
    IF_BOOL_EXEC(listGet == nullptr, LOGE("[AI_DEMO_SYNC] can not find get method."); return nullptr);
    IF_BOOL_EXEC(listSize == nullptr, LOGE("[AI_DEMO_SYNC] can not find size method."); return nullptr);
    int len = static_cast<int>(env->CallIntMethod(bufList, listSize));
    // load
    IF_BOOL_EXEC(!g_clientSync, LOGE("[AI_DEMO_SYNC] Model Manager Client is nullptr."); return nullptr);
    env->ReleaseStringUTFChars(modelpath, modelPath);

    // run
    LOGI("[AI_DEMO_SYNC] INPUT NCHW : %d %d %d %d.", inputDimension[0][0].GetNumber(),
        inputDimension[0][0].GetChannel(), inputDimension[0][0].GetHeight(), inputDimension[0][0].GetWidth());
    LOGI("[AI_DEMO_SYNC] OUTPUT NCHW : %d %d %d %d.", outputDimension[0][0].GetNumber(),
        outputDimension[0][0].GetChannel(), outputDimension[0][0].GetHeight(), outputDimension[0][0].GetWidth());
    IF_BOOL_EXEC(RunProcess(env, bufList, listGet, vecIndex, len, modelName) != SUCCESS, return nullptr);
    // output_tensor
    jclass outputListClass = env->FindClass("java/util/ArrayList");
    jmethodID outputListInit = env->GetMethodID(outputListClass, "<init>", "()V");
    jmethodID listAdd = env->GetMethodID(outputListClass, "add", "(Ljava/lang/Object;)Z");
    jobject outputList = env->NewObject(outputListClass, outputListInit, "");
    long outputTensorSize = output_tensor[vecIndex].size();
    LOGI("[AI_DEMO_SYNC] outputTensorSize is %ld .", outputTensorSize);

    IF_BOOL_EXEC(HandleOutputList(env, outputList, outputTensorSize, vecIndex, listAdd) != SUCCESS, return nullptr);

    env->ReleaseStringUTFChars(modelname, modelName);

    return outputList;
}