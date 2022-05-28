/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description:util_api_export
 */
#ifndef FRAMEWORK_TENSOR_TENSOR_API_EXPORT_H
#define FRAMEWORK_TENSOR_TENSOR_API_EXPORT_H

#ifdef HIAI_TENSOR_API_VISIABLE
#ifdef _MSC_VER
#define HIAI_TENSOR_API_EXPORT __declspec(dllexport)
#else
#define HIAI_TENSOR_API_EXPORT __attribute__((__visibility__("default")))
#endif
#else
#define HIAI_TENSOR_API_EXPORT
#endif

#endif // FRAMEWORK_TENSOR_TENSOR_API_EXPORT_H
