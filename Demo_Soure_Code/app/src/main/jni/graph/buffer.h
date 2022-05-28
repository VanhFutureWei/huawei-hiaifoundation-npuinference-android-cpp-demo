/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: buffer
 */

#ifndef GE_BUFFER_H
#define GE_BUFFER_H

#include <memory>
#include <vector>
#include <string>
#include "detail/attributes_holder.h"
/*lint -e1540*/
namespace ge {
using std::shared_ptr;

class GRAPH_API_EXPORT Buffer {
public:
    Buffer();

    Buffer(const Buffer& other);

    explicit Buffer(std::size_t size, std::uint8_t defaultVal = 0);

    ~Buffer() = default;
    bool Init(size_t size, uint8_t defaultVal = 0);

    Buffer& operator=(const Buffer& other);
    static Buffer CopyFrom(std::uint8_t* data, std::size_t bufferSize);

    const std::uint8_t* GetData() const;
    std::uint8_t* MutableData();
    std::size_t GetSize() const;
    void Clear();

    inline const std::uint8_t* data() const
    {
        return GetData();
    }
    inline std::uint8_t* data()
    {
        return MutableData();
    } // lint !e659
    inline std::size_t size() const
    {
        return GetSize();
    }

private:
    GeIrProtoHelper<proto::AttrDef> data_;
    std::string* buffer_ = nullptr;

    Buffer(const ProtoMsgOwner& protoOnwer, proto::AttrDef* buffer);

    Buffer(const ProtoMsgOwner& protoOnwer, std::string* buffer);

    friend class AttrValueImp;

    friend class Tensor;
};
} // namespace ge

#endif // GE_BUFFER_H
