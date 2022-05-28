/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: build model
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <jni.h>
#include "string.h"
#include <sys/system_properties.h>
#include <string>
#include <dlfcn.h>
#include <stdlib.h>
#include <android/log.h>
#include "model_manager/model_builder.h"
#include "util/version_util.h"

static const char* LOG_TAG = "buildmodel";
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
using namespace std;
using namespace hiai;
typedef enum {
    CHECK_OFFLINEMODEL_COMPATIBILITY_SUCCESS = 0,
    BUILD_OFFLINEMODEL_SUCCESS,
    BUILD_OFFLINEMODEL_FAILED,
    GENERATE_OFFLINE_MODEL_FAILED,
    INVALID_OFFLINE_MODEL,
    INVALID_ONLINE_MODEL,
    NO_NPU
} RESULT_CODE;

bool FileExist(const char* path)
{
    if (path == nullptr) {
        return false;
    }
    FILE* fp = fopen(path, "r+");
    if (fp == nullptr) {
        return false;
    }
    fclose(fp);
    return true;
}

bool ModelCompatibilityProcessFromBuffeOutFile(const char* offlinemodel)
{
    bool rslt = FileExist(offlinemodel);
    if (!rslt) {
        ALOGE("[AI_DEMO_CHECKMODEL_COPM] offlinemodel is not in directory\n");
        return false;
    }

    std::string path(offlinemodel);
    int pos = path.rfind("/");
    std::string name = path.substr(pos + 1);
    ALOGE("[AI_DEMO_CHECKMODEL_COPM] Model name : %s\n", name.c_str());

    std::shared_ptr<IBuiltModel> builtModel = CreateBuiltModel();
    if (builtModel == nullptr) {
        ALOGE("[AI_DEMO_CHECKMODEL_COPM] CreateBuiltModel Failed.");
        return false;
    }
    auto ret = builtModel->RestoreFromFile(offlinemodel);
    if (ret != hiai::SUCCESS) {
        ALOGE("[AI_DEMO_CHECKMODEL_COPM] RestoreFromFile Failed.");
        return false;
    }

    bool compatible = false;
    ret = builtModel->CheckCompatibility(compatible);
    if (ret != hiai::SUCCESS) {
        ALOGE("[AI_DEMO_CHECKMODEL_COPM] CheckCompatibility Failed.");
        return false;
    }
    ALOGE("[AI_DEMO_CHECKMODEL_COPM] CheckModelCompatibility compatible is : %d", compatible);
    return compatible;
}

RESULT_CODE BuildModel(const char* offlinemodel)
{
    std::shared_ptr<IModelBuilder> modelBuilder = CreateModelBuilder();
    if (modelBuilder == nullptr) {
        ALOGE("[AI_DEMO_BUILDMODEL] CreateModelBuilder failed\n");
        return BUILD_OFFLINEMODEL_FAILED;
    }

    std::string path(offlinemodel);
    int pos = path.rfind("/");
    std::string name = path.substr(pos + 1);
    ALOGE("[AI_DEMO_BUILDMODEL] Model name : %s\n", name.c_str());

    ModelBuildOptions options;
    std::shared_ptr<IBuiltModel> builtModel = nullptr;
    auto ret = modelBuilder->Build(options, name, string(offlinemodel), builtModel);
    if (ret != hiai::SUCCESS || builtModel == nullptr) {
        ALOGE("[AI_DEMO_BUILDMODEL] Build model Failed.");
        return BUILD_OFFLINEMODEL_FAILED;
    }

    ret = builtModel->SaveToFile(offlinemodel);
    if (ret != hiai::SUCCESS) {
        ALOGE("[AI_DEMO_BUILDMODEL] SaveToFile Failed.");
        return GENERATE_OFFLINE_MODEL_FAILED;
    }

    return BUILD_OFFLINEMODEL_SUCCESS;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_huawei_hiaidemo_utils_ModelManager_modelCompatibilityProcessFromFile(
    JNIEnv* env, jclass type, jstring offlinemodel_)
{
    if (env == NULL) {
        ALOGI("[AI_DEMO_COMPATIBILITY_CHECK] env is null");
        return false;
    }
    const char* offlinemodel = env->GetStringUTFChars(offlinemodel_, 0);
    if (offlinemodel == NULL) {
        ALOGI("[AI_DEMO_COMPATIBILITY_CHECK] offlinemodel path is null");
        return false;
    }
    ALOGI("[AI_DEMO_COMPATIBILITY_CHECK] offlinemodel : %s", offlinemodel);

    const char* currentversion = VersionUtil::GetVersion();
    ALOGI("[AI_DEMO_COMPATIBILITY_CHECK] ddk currentversion : %s", currentversion);
    RESULT_CODE result_code;
    if (currentversion != nullptr && string(currentversion) < "100.320.011.000") {
        result_code = NO_NPU;
    } else {
        bool checkret = ModelCompatibilityProcessFromBuffeOutFile(offlinemodel);
        ALOGI("[AI_DEMO_COMPATIBILITY_CHECK] check result : %d", checkret);
        if (checkret) {
            result_code = CHECK_OFFLINEMODEL_COMPATIBILITY_SUCCESS;
        } else {
            int res = BuildModel(offlinemodel);
            ALOGI("[AI_DEMO_COMPATIBILITY_CHECK] build offlinemodel result_code : %d", res);
            if (res != 1) {
                result_code = BUILD_OFFLINEMODEL_FAILED;
            } else {
                result_code = BUILD_OFFLINEMODEL_SUCCESS;
            }
        }
    }

    env->ReleaseStringUTFChars(offlinemodel_, offlinemodel);

    bool res = false;
    if (result_code == CHECK_OFFLINEMODEL_COMPATIBILITY_SUCCESS || result_code == BUILD_OFFLINEMODEL_SUCCESS) {
        res = true;
    }
    ALOGI("[AI_DEMO_COMPATIBILITY_CHECK] result_code value : %d", result_code);
    return res;
}
