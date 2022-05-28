/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: model
 */

#ifndef GE_MODEL_H
#define GE_MODEL_H

#include <string>
#include <vector>
#include <map>

#include "graph.h"
#include "attr_value.h"
#include "detail/attributes_holder.h"

namespace ge {
using std::string;
using std::vector;

/*lint -e148*/
class GRAPH_API_EXPORT Model : public AttrHolder {
public:
    friend class ModelSerializeImp;

    Model();

    ~Model() = default;

    Model(const string& name, const string& customVersion);

    string GetName() const;

    void SetName(const string& name);

    uint32_t GetVersion() const;

    void SetVersion(uint32_t version)
    {
        version_ = version;
    }

    string GetPlatformVersion() const;

    void SetPlatformVersion(string version)
    {
        platform_version_ = version;
    }

    Graph GetGraph() const;

    void SetGraph(const Graph& graph);

    GraphErrCodeStatus Save(Buffer& buffer) const;

    static GraphErrCodeStatus Load(const uint8_t* data, size_t len, Model& model);

    bool IsValid() const;

protected:
    ProtoAttrMapHelper MutableAttrMap() override;

    ConstProtoAttrMapHelper GetAttrMap() const override;

private:
    void Init();

    ProtoAttrMapHelper attrs_;

    string name_;

    uint32_t version_;

    string platform_version_{""};

    Graph graph_;
};

/*lint +e148*/
} // namespace ge

#endif // GE_MODEL_H
