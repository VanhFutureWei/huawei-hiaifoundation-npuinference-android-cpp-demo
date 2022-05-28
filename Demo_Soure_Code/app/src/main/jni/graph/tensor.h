/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: tensor
 */

#ifndef GE_TENSOR_H
#define GE_TENSOR_H

#include <memory>
#include <vector>
#include <atomic>
#include "buffer.h"
#include "shape.h"
#include "types.h"
#include "debug/ge_error_codes.h"
#include "./detail/attributes_holder.h"

namespace ge {
class TensorDescImpl;

using TensorDescImplPtr = std::shared_ptr<TensorDescImpl>;

class GRAPH_API_EXPORT TensorDesc : public AttrHolder {
    friend class TensorUtils;

    friend class AttrValue;

    friend class ModelSerialize;

public:
    TensorDesc();

    explicit TensorDesc(Shape shape, Format format = FORMAT_NCHW, DataType dt = DT_FLOAT);
    TensorDesc(Shape shape, DataType dt);
    TensorDesc(const TensorDesc& desc);
    TensorDesc(TensorDesc&& desc);

    ~TensorDesc() = default;
    string GetName() const;
    void SetName(const string& name);
    TensorDesc& operator=(const TensorDesc& desc);
    TensorDesc& operator=(TensorDesc&& desc);

    const Shape& GetShape() const;
    Shape& MutableShape();
    void SetShape(Shape shape);

    Format GetFormat() const;
    void SetFormat(Format format);

    DataType GetDataType() const;
    void SetDataType(DataType dt);

private:
    using AttrHolder::DelAttr;
    using AttrHolder::GetAllAttrs;
    using AttrHolder::GetAttr;
    using AttrHolder::HasAttr;
    using AttrHolder::SetAttr;

private:
    void Init();

    ProtoAttrMapHelper MutableAttrMap() override;

    ConstProtoAttrMapHelper GetAttrMap() const override;

    TensorDesc(const ProtoMsgOwner& protoOnwer, proto::TensorDescriptor* protoMsg);

    friend class Tensor;

    friend class AttrValueImp;

    friend class ModelSerializeImp;

    GeIrProtoHelper<proto::TensorDescriptor> tensorDescriptor_;
    mutable Shape __shape_;

    void RefTo(const TensorDesc& tensorDesc)
    {
        tensorDescriptor_ = tensorDesc.tensorDescriptor_;
    }

    Shape& ShapeReference() const;
};

class GRAPH_API_EXPORT Tensor {
public:
    Tensor();
    explicit Tensor(const TensorDesc& tensorDesc);
    Tensor(const TensorDesc& tensorDesc, const Buffer& data);
    Tensor(const TensorDesc& tensorDesc, const uint8_t* data, size_t size);

    ~Tensor() = default;

    Tensor(const Tensor& other);
    Tensor& operator=(const Tensor& other);

    const TensorDesc& GetTensorDesc() const;
    TensorDesc& MutableTensorDesc();
    GraphErrCodeStatus SetTensorDesc(const TensorDesc& tensorDesc);

    const Buffer GetData() const;
    Buffer MutableData();
    GraphErrCodeStatus SetData(const Buffer& data);
    GraphErrCodeStatus SetData(const uint8_t* data, size_t size);

private:
    friend class AttrValueImp;

    Tensor(const ProtoMsgOwner& protoOnwer, proto::TensorDef* protoMsg);

    GeIrProtoHelper<proto::TensorDef> tensorDef_;
    mutable TensorDesc __desc_;
    TensorDesc& DescReference() const;
};
} // namespace ge

#endif // GE_TENSOR_H
