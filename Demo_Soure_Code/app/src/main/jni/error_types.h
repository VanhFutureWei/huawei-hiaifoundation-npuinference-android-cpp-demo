/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: error types
 */
#ifndef FRAMEWORK_ERROR_TYPES_H
#define FRAMEWORK_ERROR_TYPES_H


namespace hiai {
using Status = uint32_t;

static constexpr Status SUCCESS = 0;
static constexpr Status FAILURE = 1;
static constexpr Status UNINITIALIZED = 2;
static constexpr Status INVALID_PARAM = 3;
static constexpr Status TIMEOUT = 4;
static constexpr Status UNSUPPORTED = 5;
static constexpr Status MEMORY_EXCEPTION = 6;
static constexpr Status INVALID_API = 7;
static constexpr Status INVALID_POINTER = 8;
static constexpr Status CALC_EXCEPTION = 9;
static constexpr Status FILE_NOT_EXIST = 10;
static constexpr Status COMM_EXCEPTION = 11;

} // namespace hiai
#endif // FRAMEWORK_ERROR_TYPES_H
