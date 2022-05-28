/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: model manager client implementation
 *
 */

#ifndef CLOUD_SERVICE_NEW_H
#define CLOUD_SERVICE_NEW_H
#include <atomic>

#ifndef AICP_API_EXPORT
#if (defined HOST_VISIBILITY) || (defined DEV_VISIBILITY)
#ifdef _MSC_VER
#define AICP_API_EXPORT __declspec(dllexport)
#else
#define AICP_API_EXPORT __attribute__((__visibility__("default")))
#endif
#else
#define AICP_API_EXPORT
#endif
#endif

namespace hiai {
class  AICP_API_EXPORT CloudService {
public:
    static bool Enable(void* env, void* context);

private:
    CloudService() = default;
    ~CloudService() = default;
    static std::atomic<bool> isSoLoaded_;
};
} // end namespace hiai
#endif
