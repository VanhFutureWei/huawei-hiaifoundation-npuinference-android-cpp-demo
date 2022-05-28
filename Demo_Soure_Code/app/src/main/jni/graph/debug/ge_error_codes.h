/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: ge_error_codes
 */

#ifndef GE_ERROR_CODES_H
#define GE_ERROR_CODES_H

#include <stdint.h>
namespace ge {
using GraphErrCodeStatus = uint32_t;
const GraphErrCodeStatus GRAPH_FAILED = 0xFFFFFFFF;
const GraphErrCodeStatus GRAPH_SUCCESS = 0;
const GraphErrCodeStatus GRAPH_PARAM_INVALID = 50331649;
} // namespace ge

#endif // GE_ERROR_CODES_H
