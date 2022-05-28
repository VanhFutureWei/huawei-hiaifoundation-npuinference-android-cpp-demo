/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: attr_value
 */

#ifndef GE_ATTR_VALUE_H
#define GE_ATTR_VALUE_H

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "tensor.h"
#include "debug/ge_error_codes.h"
#include "graph_api_export.h"
#include "buffer.h"
#include "detail/attributes_holder.h"

using std::string;
using std::vector;
namespace ge {
class Tensor;

using TensorPtr = std::shared_ptr<Tensor>;
using ConstTensorPtr = std::shared_ptr<const Tensor>;

class ComputeGraph;

using ComputeGraphPtr = std::shared_ptr<ComputeGraph>;
using ConstComputeGraphPtr = std::shared_ptr<const ComputeGraph>;

class TensorDesc;

class AttrValueImp;

/*lint -e148*/
class GRAPH_API_EXPORT AttrValue {
public:
    class NamedAttrs : public AttrHolder {
    public:
        NamedAttrs();

        virtual ~NamedAttrs() = default;

        void SetName(const std::string& name);

        string GetName() const;

        AttrValue GetItem(const string& key) const;

    protected:
        ProtoAttrMapHelper MutableAttrMap() override;

        ConstProtoAttrMapHelper GetAttrMap() const override;

    private:
        NamedAttrs(const ProtoMsgOwner& owner, proto::NamedAttrs* protoMsg);

        GeIrProtoHelper<proto::NamedAttrs> namedAttrs_;

        friend class AttrValueImp;
    };

public:
    using INT = int64_t;
    using FLOAT = float;
    using BOOL = bool;
    using STR = std::string;
    using TENSOR = TensorPtr;
    using TENSOR_DESC = TensorDesc;
    using GRAPH = ComputeGraphPtr;
    using BYTES = Buffer;
    using NAMED_ATTRS = NamedAttrs;
    using DATA_TYPE = int64_t;
    using LIST_INT = vector<INT>;
    using LIST_FLOAT = vector<FLOAT>;
    using LIST_BOOL = vector<BOOL>;
    using LIST_STR = vector<STR>;
    using LIST_TENSOR = vector<TENSOR>;
    using LIST_TENSOR_DESC = vector<TENSOR_DESC>;
    using LIST_GRAPH = vector<GRAPH>;
    using LIST_BYTES = vector<BYTES>;
    using LIST_NAMED_ATTRS = vector<NAMED_ATTRS>;

    enum ValueType {
        VT_NONE = 0,
        VT_STRING,
        VT_FLOAT,
        VT_BOOL,
        VT_INT,
        VT_TENSOR_DESC,
        VT_TENSOR,
        VT_BYTES,
        VT_GRAPH,
        VT_NAMED_ATTRS,

        VT_LIST_BASE = 1000,
        VT_LIST_STRING = VT_LIST_BASE + VT_STRING,
        VT_LIST_FLOAT = VT_LIST_BASE + VT_FLOAT,
        VT_LIST_BOOL = VT_LIST_BASE + VT_BOOL,
        VT_LIST_INT = VT_LIST_BASE + VT_INT,
        VT_LIST_TENSOR_DESC = VT_LIST_BASE + VT_TENSOR_DESC,
        VT_LIST_TENSOR = VT_LIST_BASE + VT_TENSOR,
        VT_LIST_BYTES = VT_LIST_BASE + VT_BYTES,
        VT_LIST_GRAPH = VT_LIST_BASE + VT_GRAPH,
        VT_LIST_NAMED_ATTRS = VT_LIST_BASE + VT_NAMED_ATTRS,
    };

public:

    AttrValue();

    ~AttrValue() = default;
    bool SetInt(int64_t val);
    int64_t GetInt() const;
    static AttrValue CreateFrom(int64_t val);

    bool SetFloat(float val);
    float GetFloat() const;
    static AttrValue CreateFrom(float val);

    bool SetBool(bool val);
    bool GetBool() const;
    static AttrValue CreateFrom(bool val);

    bool SetString(const std::string& val);
    std::string GetString() const;
    static AttrValue CreateFrom(const std::string& val);

    bool SetTensor(const TensorPtr& val);
    const TensorPtr GetTensor() const;
    static AttrValue CreateFrom(const TensorPtr& val);

    bool SetNamedAttrs(const NamedAttrs& val);
    NamedAttrs GetNamedAttrs() const;
    static AttrValue CreateFrom(const NamedAttrs& val);

    bool SetGraph(const ComputeGraphPtr& val);
    const ComputeGraphPtr GetGraph() const;

    bool SetBuffer(const Buffer& val);
    Buffer GetBuffer() const;
    static AttrValue CreateFrom(const Buffer& val);

    bool SetTensorDesc(const TensorDesc& val);
    TensorDesc GetTensorDesc() const;
    static AttrValue CreateFrom(const TensorDesc& val);

    bool SetIntList(const std::vector<int64_t>& val);
    const std::vector<int64_t> GetIntList() const;
    static AttrValue CreateFrom(const std::vector<int64_t>& val);

    bool SetFloatList(const std::vector<float>& val);
    const std::vector<float> GetFloatList() const;
    static AttrValue CreateFrom(const std::vector<float>& val);

    bool SetBoolList(const std::vector<bool>& val);
    const std::vector<bool> GetBoolList() const;
    static AttrValue CreateFrom(const std::vector<bool>& val);

    bool SetStringList(const std::vector<std::string>& val);
    const std::vector<std::string>  GetStringList() const;
    static AttrValue CreateFrom(const std::vector<std::string>& val);

    bool SetTensorList(const std::vector<TensorPtr>& val);
    const std::vector<TensorPtr> GetTensorList() const;
    static AttrValue CreateFrom(const std::vector<TensorPtr>& val);

    bool SetNamedAttrsList(const std::vector<NamedAttrs>& val);
    const std::vector<NamedAttrs> GetNamedAttrsList() const;

    bool SetGraphList(const std::vector<ComputeGraphPtr>& val);
    const std::vector<ComputeGraphPtr> GetGraphList() const;

    bool SetBufferList(const std::vector<Buffer>& val);
    const std::vector<Buffer> GetBufferList() const;
    static AttrValue CreateFrom(const std::vector<Buffer>& val);

    bool SetTensorDescList(const std::vector<TensorDesc>& val);
    const std::vector<TensorDesc> GetTensorDescList() const;
    static AttrValue CreateFrom(const std::vector<TensorDesc>& val);

    ValueType GetValueType() const;

    bool IsEmpty() const;

    bool operator==(const AttrValue& other) const
    {
        return value_ == other.value_;
    }

public:

    GeIrProtoHelper<proto::AttrDef> value_;

    AttrValue(const ProtoMsgOwner& protoOnwer, ge::proto::AttrDef* val);

    friend class AttrHolder;

    friend class ModelSerializeImp;
};
/*lint +e148*/
} // namespace ge

#endif // GE_ATTR_VALUE_H
