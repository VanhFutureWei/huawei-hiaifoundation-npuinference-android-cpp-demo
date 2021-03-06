/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: AiTensor
 */
#ifndef FRAMEWORK_TENSOR_COMPATIBLE_AITENSOR_H
#define FRAMEWORK_TENSOR_COMPATIBLE_AITENSOR_H
#include <memory>

#include "tensor/tensor_api_export.h"
#include "util/base_types.h"
#include "tensor/nd_tensor_buffer.h"
#include "compatible/hiai_base_types_cpt.h"

namespace hiai {
class HIAI_TENSOR_API_EXPORT TensorDimension {
public:
    TensorDimension();
    virtual ~TensorDimension();

    /*
     * @brief Tensor初始化接口
     * @param [in] number 输入的Tensor的数量
     * @param [in] channel 输入的Tensor的通道数
     * @param [in] height 输入的Tensor的高度
     * @param [in] width 输入的Tensor的宽度
     * @return 无
     */
    TensorDimension(uint32_t number, uint32_t channel, uint32_t height, uint32_t width);

    void SetNumber(const uint32_t number);

    uint32_t GetNumber() const;

    void SetChannel(const uint32_t channel);

    uint32_t GetChannel() const;

    void SetHeight(const uint32_t height);

    uint32_t GetHeight() const;

    void SetWidth(const uint32_t width);

    uint32_t GetWidth() const;

    bool IsEqual(const TensorDimension& dim);

private:
    uint32_t n{0};
    uint32_t c{0};
    uint32_t h{0};
    uint32_t w{0};
};

class HIAI_TENSOR_API_EXPORT AiTensor {
public:
    AiTensor();
    virtual ~AiTensor();

    /*
     * @brief Tensor初始化接口
     * @param [in] dim 输入tensor的尺寸结构信息
     * @return AIStatus::AI_SUCCESS 成功
     * @return Others 失败
     */
    AIStatus Init(const TensorDimension* dim);
    /*
     * @brief Tensor初始化接口
     * @param [in] dim 输入tensor的尺寸结构信息
     * @param [in] DataType数据类型
     * @return AIStatus::AI_SUCCESS 成功
     * @return Others 失败
     */
    AIStatus Init(const TensorDimension* dim, HIAI_DataType pdataType);
    /*
     * @brief Tensor初始化接口
     * @param [in] handle ION内存信息
     * @param [in] dim 输入tensor的尺寸结构信息
     * @param [in] DataType数据类型
     * @return AIStatus::AI_SUCCESS 成功
     * @return Others 失败
     */
    AIStatus Init(const NativeHandle& handle, const TensorDimension* dim, HIAI_DataType pdataType);
    /*
     * @brief Tensor初始化接口
     * @param [in] number 输入的Tensor的数量
     * @param [in] height 输入的Tensor的高度
     * @param [in] width 输入的Tensor的宽度
     * @param [in] format 输入图片的格式AiTensorImage_Format类型
     * @return AIStatus::AI_SUCCESS 成功
     * @return Others 失败
     */
    AIStatus Init(uint32_t number, uint32_t height, uint32_t width, AiTensorImage_Format format);

    /*
     * @brief 获取Tensor buffer地址接口
     * @return void* tensor buffer地址
     */
    virtual void* GetBuffer() const;

    /*
     * @brief 获取Tensor buffer内存大小
     * @return void* tensor buffer内存大小
     */
    virtual uint32_t GetSize() const;

    AIStatus SetTensorDimension(const TensorDimension* dim);

    TensorDimension GetTensorDimension() const;
    void* GetTensorBuffer() const;

private:
    AiTensor(const AiTensor&) = delete;
    AiTensor& operator=(const AiTensor&) = delete;

private:
    friend class AiModelMngerClient;
    std::shared_ptr<INDTensorBuffer> tensor_;
    NDTensorDesc desc_;
};
} // namespace hiai
#endif
