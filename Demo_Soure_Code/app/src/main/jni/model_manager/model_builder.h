/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: model builder
 */
#ifndef INC_MODEL_MANAGER_MODEL_BUILDER_H
#define INC_MODEL_MANAGER_MODEL_BUILDER_H

#include <string>
#include <vector>
#include <map>

#include "tensor/buffer.h"
#include "built_model.h"
#include "model_manager_api_export.h"
#include "model_builder_types.h"
#include "graph/model.h"
namespace hiai {
using ge::Model;
//  change to ModelCompiler
class IModelBuilder {
public:
    virtual ~IModelBuilder() = default;
    virtual Status Build(const ModelBuildOptions& options, const std::string& modelName,
        const std::shared_ptr<ge::Model>& model, std::shared_ptr<IBuiltModel>& builtModel) = 0;
    virtual Status Build(const ModelBuildOptions& options, const std::string& modelName,
        const std::shared_ptr<IBuffer>& modelBuffer, std::shared_ptr<IBuiltModel>& builtModel) = 0;

    virtual Status Build(const ModelBuildOptions& options, const std::string& modelName, const std::string& modelFile,
        std::shared_ptr<IBuiltModel>& builtModel) = 0;
};

HIAI_MM_API_EXPORT std::shared_ptr<IModelBuilder> CreateModelBuilder();
} // namespace hiai
#endif // INC_MODEL_MANAGER_MODEL_BUILDER_H
