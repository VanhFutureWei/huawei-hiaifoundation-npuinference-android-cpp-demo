/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: model builder types
 */
#ifndef INC_MODEL_MANAGER_MODEL_BUILDER_TYPES_H
#define INC_MODEL_MANAGER_MODEL_BUILDER_TYPES_H
#include <string>
#include <vector>
#include <map>

#include "tensor/nd_tensor_desc.h"

namespace hiai {
enum class FormatMode {
    USE_NCHW = 0,
    USE_ORIGIN = 1
};

enum PrecisionMode {
    PRECISION_MODE_FP32 = 0,
    PRECISION_MODE_FP16
};

enum CacheMode {
    CACHE_BUILDED_MODEL,
    CACHE_LOADED_MODEL
};

enum class DeviceConfigMode {
    AUTO,
    MODEL_LEVEL,
    OP_LEVEL
};

enum class FallBackMode {
    ENABLE,
    DISABLE
};

enum class ExecuteDevice {
    NPU = 0,
    CPU = 1
};

/*
 *    OFF : 不进行模型优化
 *    ON_DEVICE_TUNING : 在process阶段动态shape优化模型
 *    ON_DEVICE_PREPROCESS_TUNING : 在加载编译阶段优化模型
 *    ON_CLOUD_TUNING : 预留,云端优化模型
 */
enum class TuningStrategy {
    OFF = 0,
    ON_DEVICE_TUNING,
    ON_DEVICE_PREPROCESS_TUNING,
    ON_CLOUD_TUNING
};

static const uint32_t MIN_DYNAMIC_SHAPE_CACHE_NUM = 1;
static const uint32_t MAX_DYNAMIC_SHAPE_CACHE_NUM = 10;

struct DynamicShapeConfig {
    bool enable = false; // mark whether dynamic shape enable.
    uint32_t maxCachedNum = 0; // max cache model
    CacheMode cacheMode = CacheMode::CACHE_BUILDED_MODEL;
};

struct ModelDeviceConfig {
    DeviceConfigMode deviceConfigMode = DeviceConfigMode::AUTO;
    FallBackMode fallBackMode = FallBackMode::ENABLE;
    std::vector<ExecuteDevice> modelDeviceOrder;
    std::map<std::string, std::vector<ExecuteDevice>> opDeviceOrder;
};

struct ModelBuildOptions {
    std::vector<NDTensorDesc> inputTensorDescs;

    FormatMode formatMode = FormatMode::USE_NCHW;

    PrecisionMode precisionMode = PrecisionMode::PRECISION_MODE_FP32;

    DynamicShapeConfig dynamicShapeConfig;

    ModelDeviceConfig modelDeviceConfig;

    TuningStrategy tuningStrategy = TuningStrategy::OFF;

    size_t estimatedOutputSize = 0;
};
} // namespace hiai
#endif
