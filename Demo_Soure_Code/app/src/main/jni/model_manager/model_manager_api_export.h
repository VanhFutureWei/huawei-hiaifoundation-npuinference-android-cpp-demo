/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description:
 */
#ifndef FRAMEWORK_MODEL_MANAGER_MODEL_MANAGER_API_EXPORT_H
#define FRAMEWORK_MODEL_MANAGER_MODEL_MANAGER_API_EXPORT_H

#ifdef HIAI_MM_API_VISIABLE
#ifdef _MSC_VER
#define HIAI_MM_API_EXPORT __declspec(dllexport)
#else
#define HIAI_MM_API_EXPORT __attribute__((__visibility__("default")))
#endif
#else
#define HIAI_MM_API_EXPORT
#endif

#endif // FRAMEWORK_MODEL_MANAGER_MODEL_MANAGER_API_EXPORT_H
