/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: const_defs
 */

#ifndef _HIAI_GRAPH_OP_GE_OP_CONST_DEFS_H
#define _HIAI_GRAPH_OP_GE_OP_CONST_DEFS_H

#include "../operator_hiai_reg.h"

namespace hiai {
/*
 * Constant tensor
 * <Output>
 *    y : Output tensor containing the same value of the provided tensor
 * <Attr>
 *    value : Value for the elements of the output tensor
 * <Added in HiAI version>
 *    100.300.010.011
 * <Examples>
 *    TensorDesc constTensorDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    TensorPtr constTensor = std::make_shared<hiai::Tensor>(constTensorDesc);
 *    vector<float> dataValue(4* 5 * 6 * 7, 0);
 *    constTensor->SetData((uint8_t*)dataValue.data(), 4* 5 * 6 * 7 * sizeof(float));
 *    auto constTemp = hiai::op::Const("constTemp").set_attr_value(constTensor);
 */
REG_OP(Const)
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT8, DT_INT32, DT_BOOL }))
.ATTR(value, AttrValue::TENSOR(new (std::nothrow) Tensor(TensorDesc())))
.OP_END()
} // namespace hiai

#endif // _HIAI_GRAPH_OP_GE_OP_CONST_DEFS_H
