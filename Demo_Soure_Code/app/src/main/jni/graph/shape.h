/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: shape
 */

#ifndef GE_SHAPE_H
#define GE_SHAPE_H

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include "types.h"
#include "debug/ge_error_codes.h"
#include "graph_api_export.h"
#include "./detail/attributes_holder.h"

namespace ge {
class GRAPH_API_EXPORT Shape {
public:
    Shape();
    explicit Shape(std::vector<int64_t> s);
    Shape(const Shape& other);
    Shape(Shape&& other);

    ~Shape() = default;

    size_t GetDimNum() const;

    int64_t GetDim(size_t idx) const;
    GraphErrCodeStatus SetDim(size_t idx, int64_t value);
    const std::vector<int64_t> GetDims() const;
    int64_t GetTotalDimNum() const;

    Shape& operator=(const Shape& other); //lint !e1539
    Shape& operator=(Shape&& other); //lint !e1539
    bool operator==(const Shape& other);
    bool operator==(const Shape& other) const;

private:
    GeIrProtoHelper<proto::ShapeDef> shapeDef_;

    friend class TensorDesc;

    Shape(const ProtoMsgOwner& protoOnwer, proto::ShapeDef* protoMsg);

    void RefTo(const Shape& shape)
    {
        shapeDef_ = shape.shapeDef_;
    }
};
} // namespace ge

#endif // GE_SHAPE_H
