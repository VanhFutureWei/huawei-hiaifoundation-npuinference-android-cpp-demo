/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: built model
 */
#ifndef FRAMEWORK_MODEL_MANAGER_BUILT_MODEL_H
#define FRAMEWORK_MODEL_MANAGER_BUILT_MODEL_H

#include "tensor/nd_tensor_buffer.h"
#include "model_manager_api_export.h"
#include "error_types.h"

namespace hiai {
class IBuiltModel {
public:
    virtual ~IBuiltModel() = default;

    virtual Status SaveToExternalBuffer(std::shared_ptr<IBuffer>& buffer, size_t& realSize) const = 0;
    virtual Status SaveToBuffer(std::shared_ptr<IBuffer>& buffer) const = 0;

    virtual Status RestoreFromBuffer(const std::shared_ptr<IBuffer>& buffer) = 0;

    virtual Status SaveToFile(const char* file) const = 0;

    virtual Status RestoreFromFile(const char* file) = 0;

    virtual Status CheckCompatibility(bool& compatible) const = 0;

    virtual std::string GetName() const = 0;
    virtual void SetName(const std::string& name) = 0;

    virtual std::vector<NDTensorDesc> GetInputTensorDescs() const = 0;
    virtual std::vector<NDTensorDesc> GetOutputTensorDescs() const = 0;
};

HIAI_MM_API_EXPORT std::shared_ptr<IBuiltModel> CreateBuiltModel();

} // namespace hiai
#endif // FRAMEWORK_MODEL_MANAGER_BUILT_MODEL_H
