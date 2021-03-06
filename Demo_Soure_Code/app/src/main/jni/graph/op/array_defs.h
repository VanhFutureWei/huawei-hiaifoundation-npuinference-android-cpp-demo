/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: array_defs
 */

#ifndef _HIAI_GRAPH_OP_GE_OP_ARRAY_DEFS_H
#define _HIAI_GRAPH_OP_GE_OP_ARRAY_DEFS_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#ifdef _MSC_VER
#undef __out_z
#endif

#include "../operator_hiai_reg.h"

namespace hiai {
/*
 * Data tensor
 * <Input>
 *      x : Input tensor
 * <Output>
 *      y : Output tensor
 * <Attr>
 *      index : Reserved. The index of input data in network, only support 0 currently.
 * <Added in HiAI version>
 *    100.300.010.011
 */
const char* const DATA = "Data";
const char* const DATA_ATTR_NAME_INDEX = "index";

REG_OP(Data)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT8, DT_UINT8, DT_INT32, DT_BOOL, DT_INT64, DT_DOUBLE }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT8, DT_UINT8, DT_INT32, DT_BOOL, DT_INT64, DT_DOUBLE }))
.ATTR(index, AttrValue::INT { 0 })
.OP_END()

/*
 * Concatenates a list of tensors into a single tensor along one dimension.
 * The number of dimensions of the input tensors must match, and all dimensions except axis must be equal.
 * <Input>
 *      x : List of tensors for concatenation
 * <Output>
 *      y : Concatenated tensor
 * <Attr>
 *      concat_dim : Dimension along which to concatenate
 *      N : Number of inputs
 * <Added in HiAI version>
 *    100.300.010.011
 * <Examples>
 *    TensorDesc xDesc1(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x1 = hiai::op::Data("x1");
 *    x1.update_input_desc_x(xDesc1);
 *
 *    TensorDesc xDesc2(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x2 = hiai::op::Data("x2");
 *    x2.update_input_desc_x(xDesc2);
 *
 *    auto concatD = hiai::op::ConcatD("concatD")
 *                   .create_dynamic_input_x(2)
 *                   .set_dynamic_input_x(1, x1)
 *                   .set_dynamic_input_x(2, x2)
 *                   .set_attr_concat_dim(1)
 *                   .set_attr_N(2);
 */
const char* const CONCAT_ATTR_NAME_AXIS = "concat_dim";
const char* const CONCAT_ATTR_NAME_NUM = "N";

REG_OP(ConcatD)
.DYNAMIC_INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_INT8, DT_BOOL }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_INT8, DT_BOOL }))
.REQUIRED_ATTR(concat_dim, AttrValue::INT)
.ATTR(N, AttrValue::INT { 1 })
.OP_END()

/*
 * Fake-quantize the inputs tensor of type float via global float scalars.
 * <Input>
 *      x : A Tensor of type float32.
 *      min : 0D (scalar), A Tensor of type float32.
 *      max : 0D (scalar), A Tensor of type float32.
 * <Output>
 *      y : A Tensor of type float32.
 * <Attr>
 *      num_bits : An optional int. Defaults to 8.
 *      narrow_range: An optional bool. Defaults to False.
 * <Added in HiAI version>
 *    100.500.010.011
 */
const char* const FAKEQUANTWITHMINMAXVARS_ATTR_NUMBITS = "num_bits";
const char* const FAKEQUANTWITHMINMAXVARS_ATTR_NARROWRANGE = "narrow_range";

REG_OP(FakeQuantWithMinMaxVars)
.INPUT(x, TensorType({ DT_FLOAT }))
.INPUT(min, TensorType({ DT_FLOAT }))
.INPUT(max, TensorType({ DT_FLOAT }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.ATTR(num_bits, AttrValue::INT { 8 })
.ATTR(narrow_range, AttrValue::BOOL { false })
.OP_END()


/*
 * Reshape the input tensor.
 * <Input>
 *    x : Input tensor, of the same type as Data
 *    shape : The shape to be resized, must be const op
 * <Output>
 *    y : Reshaped tensor that has the same values as Attr 'shape'
 * <Attr>
 *    axis     : Dimension along which to reshape
 *    num_axes : Used to calculate the output shape
 *               When 'num_axes' is -1, output.size() = shape.size() + axis.
 *               When 'num_axes' is not -1, output.size() = shape.size() + tensor.size() - num_axes.
 * <Added in HiAI version>
 *    100.300.010.011
 */
const char* const RESHAPE_ATTR_AXIS = "axis";
const char* const RESHAPE_ATTR_NUM_AXES = "num_axes";
const char* const RESHAPE_ATTR_SHAPE = "shape";

REG_OP(Reshape)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_INT64, DT_BOOL }))
.INPUT(shape, TensorType({ DT_INT32, DT_INT64 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_INT64, DT_BOOL }))
.ATTR(axis, AttrValue::INT { 0 })
.ATTR(num_axes, AttrValue::INT { -1 })
.OP_END()

/*
 * Splits a tensor into a list of tensors along the split_dim.
 * <Input>
 *      x : Input tensor.
 * <Output>
 *      y : (Mandatory) splited tensors, whose number is specified by num_split
 * <Attr>
 *      split_dim : Which axis to split on, the value should be in range of [-RANK(X), RANK(X)).
 *      num_split : Number of outputs, x.shape[split_dim] % num_split should be 0.
 * <Added in HiAI version>
 *    100.300.010.011
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    auto splitD = hiai::op::SplitD("splitD")
 *                  .set_input_x(x)
 *                  .create_dynamic_output_y(3)
 *                  .set_attr_split_dim(2)
 *                  .set_attr_num_split(3);
 */
const char* const SPLIT_ATTR_NAME_SPLIT_DIM = "split_dim";
const char* const SPLIT_ATTR_NAME_NUM_SPLIT = "num_split";

REG_OP(SplitD)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT8, DT_INT32, DT_BOOL, DT_UINT8 }))
.DYNAMIC_OUTPUT(y, TensorType({ DT_FLOAT, DT_INT8, DT_INT32, DT_BOOL, DT_UINT8 }))
.REQUIRED_ATTR(split_dim, AttrValue::INT)
.REQUIRED_ATTR(num_split, AttrValue::INT)
.OP_END()

/*
 * Splits a tensor into 'num_split' tensors along one dimension.
 * <Input>
 *    x           : Input tensor.
 *    size_splits : Tensor of type int32, must be const op.
 *                  Optional length of each output.
 *                  Sum of the values must be equal to the dim value at 'split_dim' specified.
 *    split_dim   : Tensor of type int32, must be a scalar const op.
 *                  Which axis to split on. A negative value means counting dimensions from the back.
 *                  Accepted range is [-rank(x), rank(x)).
 * <Output>
 *    y          : (Mandatory) spited tensors, whose number is specified by 'num_split'
 * <Attr>
 *   num_split   : int (>= 1), number of outputs
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc sizeSplitsTensorDesc(Shape({3}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr sizeSplitsTensor = std::make_shared<hiai::Tensor>(sizeSplitsTensorDesc);
 *    vector<int32_t> sizeSplitsDataValue = {1, 2, 3};
 *    sizeSplitsTensor->SetData((uint8_t*)sizeSplitsDataValue.data(), 3 * sizeof(int32_t));
 *    auto sizeSplits = hiai::op::Const("sizeSplits").set_attr_value(sizeSplitsTensor);
 *
 *    TensorDesc splitDimTensorDesc(Shape(), FORMAT_NCHW, DT_INT32);
 *    TensorPtr splitDimTensor = std::make_shared<hiai::Tensor>(splitDimTensorDesc);
 *    vector<int32_t> splitDimDataValue = {2};
 *    splitDimTensor->SetData((uint8_t*)splitDimDataValue.data(), 1 * sizeof(int32_t));
 *    auto splitDim = hiai::op::Const("splitDim").set_attr_value(splitDimTensor);
 *
 *    auto splitV = hiai::op::SplitV("splitV")
 *                  .set_input_x(x)
 *                  .set_input_size_splits(sizeSplits)
 *                  .set_input_split_dim(splitDim)
 *                  .set_attr_num_split(3)
 *                  .create_dynamic_output_y(3);
 */
REG_OP(SplitV)
.INPUT(x, TensorType({ DT_FLOAT }))
.INPUT(size_splits, TensorType({ DT_INT32 }))
.INPUT(split_dim, TensorType({ DT_INT32 }))
.REQUIRED_ATTR(num_split, AttrValue::INT)
.DYNAMIC_OUTPUT(y, TensorType({ DT_FLOAT }))
.OP_END()

/*
 * Unpacks the given dimension of a rank-R tensor into rank-(R-1) tensors.
 * <Input>
 *    x    : Input tensor
 * <Output>
 *    y    : List of Tensor objects unstacked from x
 * <Attr>
 *    num  : Output number of tensor after split.
 *            The value of num must be equal to x[axis].
 *    axis : Axis to unpack along
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    auto unpack = hiai::op::Unpack("unpack")
 *                  .set_input_x(x)
 *                  .set_attr_axis(2)
 *                  .set_attr_num(6)
 *                  .create_dynamic_output_y(6);
 */
const char* const UNPACK_ATTR_NAME_NUM = "num";
const char* const UNPACK_ATTR_NAME_AXIS = "axis";

REG_OP(Unpack)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32 }))
.DYNAMIC_OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32 }))
.REQUIRED_ATTR(num, AttrValue::INT)
.ATTR(axis, AttrValue::INT { 0 })
.OP_END()

/*
 * Flattens the input tensor into a 2D matrix.
 * <Input>
 *    x : Tensor of rank
 * <Output>
 *    y : 2D tensor with the contents of the input tensor, with input dimensions up to axis flattened to
 *        the outer dimension of the output and remaining input dimensions flattened into the inner dimension
 *        of the output.
 * <Added in HiAI version>
 *    100.300.010.011
 */
REG_OP(Flatten)
.INPUT(x, TensorType({ DT_FLOAT }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.OP_END()

/*
 * Flattens the input tensor into a 2D matrix.
 * <Input>
 *    x : Tensor of rank
 * <Output>
 *    y : 2D tensor with the contents of the input tensor, with input dimensions up to axis flattened to
 *        the outer dimension of the output and remaining input dimensions flattened into the inner dimension
 *        of the output.
 * <Attr>
 *    axis : An optional int32, specifying the first axis to flatten.
 *           All preceding axes are retained in the output. Defaults to "1".
 *    end_axis : An optional int32, specifying the last axis to flatten.
 *               All following axes are retained in the output. Defaults to "-1".
 * <Added in HiAI version>
 *    100.500.010.010
 */
const char* const FLATTEN_ATTR_AXIS = "axis";
const char* const FLATTEN_ATTR_ENDAXIS = "end_axis";

REG_OP(FlattenV2)
.INPUT(x, TensorType({ DT_FLOAT }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.ATTR(axis, AttrValue::INT { 1 })
.ATTR(end_axis, AttrValue::INT { -1 })
.OP_END()
/*
 * Extracts a slice of size 'size' from a tensor input starting at the location specified by 'offsets '.
 * The slice 'size' is represented as a tensor shape,
 * where size[i] is the number of elements of the ith dimension to slice.
 * The starting location (offsets) for the slice is represented as an offset in each dimension of input.
 * In other words, offsets[i] is the offset into the ith dimension of input to slice.
 * <Input>
 *      x : Input tensor
 *      offsets : Const, starting location for each dimension of input
 *      size :  Const, number of elements for each dimension of input
 * <Output>
 *      y : Output tensor
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc offsetsTensorDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr offsetsTensor = std::make_shared<hiai::Tensor>(offsetsTensorDesc);
 *    vector<int32_t>offsetsDataValue = {0, 1, 0, 1};
 *    offsetsTensor->SetData((uint8_t*)offsetsDataValue.data(), 4 * sizeof(int32_t));
 *    auto offsets = hiai::op::Const("offsets").set_attr_value(offsetsTensor);
 *
 *    TensorDesc sizeTensorDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr sizeTensor = std::make_shared<hiai::Tensor>(sizeTensorDesc);
 *    vector<int32_t> sizeDataValue = {1, 3, 2, -1};
 *    sizeTensor->SetData((uint8_t*)sizeDataValue.data(), 4 * sizeof(int32_t));
 *    auto size = hiai::op::Const("size").set_attr_value(sizeTensor);
 *
 *    auto slice = hiai::op::Slice("slice")
 *                 .set_input_x(x)
 *                 .set_input_offsets(offsets)
 *                 .set_input_size(size);
 */
REG_OP(Slice)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.INPUT(offsets, TensorType({ DT_INT32 }))
.INPUT(size, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.OP_END()

/*
 * Inserts a dimension of 1 into a tensor's shape.
 * This op support max realdims is 4 and support max padding dims is 3.
 * <Input>
 *    x   : Input tensor
 *   axis : 0D (scalar), dimension index at which to expand the shape of input
 * <Output>
 *    y : Output tensor. The dimension of y not support bigger than 4.
 * <Added in HiAI version>
 *    100.310.010.013
 */
REG_OP(ExpandDims)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.INPUT(axis, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.OP_END()


/*
 * Gathers slices from 'x' axis according to 'indices'.
 * <Input>
 *      x : Tensor from which to gather values
 *      indices : Input tensor
 * <Output>
 *      y : Output tensor
 * <Attr>
 *      axis : which axis to gather values. Accepted range is [-rank(x), rank(x)).
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc indicesDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    hiai::op::Data indices = hiai::op::Data("indices");
 *    indices.update_input_desc_x(indicesDesc);
 *
 *    auto gatherV2D = hiai::op::GatherV2D("gatherV2D")
 *                     .set_input_x(x)
 *                     .set_input_indices(indices)
 *                     .set_attr_axis(1);
 */
const char* const GATHERV2D_ATTR_NAME_AXIS = "axis";

REG_OP(GatherV2D)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32 }))
.INPUT(indices, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32 }))
.REQUIRED_ATTR(axis, AttrValue::INT)
.OP_END()

/*
 * Gathers slices from 'x' into a Tensor with shape specified by 'indices'.
 * <Input>
 *    x       : Tensor from which to gather values.
 *    indices : Input tensor.
 * <Output>
 *    y : Output tensor
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc indicesDesc(Shape({3}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data indices = hiai::op::Data("indices");
 *    indices.update_input_desc_x(indicesDesc);
 *
 *    auto gatherNd = hiai::op::GatherNd("gatherNd")
 *                    .set_input_x(x)
 *                    .set_input_indices(indices);
 */
REG_OP(GatherNd)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32 }))
.INPUT(indices, TensorType({ DT_FLOAT, DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32 }))
.OP_END()

/*
 * Stacks a list of rank-R tensors into one rank-(R+1) tensor.
 * <Input>
 *    x : List of Tensor objects with the same shape and type
 * <Output>
 *    y : Output tensor. The dimension of y not support bigger than 4.
 * <Attr>
 *    axis : Axis to stack along
 *    N    : Number of values
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc1(Shape({4, 5, 6}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x1 = hiai::op::Data("x1");
 *    x1.update_input_desc_x(xDesc1);
 *
 *    TensorDesc xDesc2(Shape({4, 5, 6}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x2 = hiai::op::Data("x2");
 *    x2.update_input_desc_x(xDesc2);
 *
 *    auto pack = hiai::op::Pack("pack")
 *                .create_dynamic_input_x(2)
 *                .set_dynamic_input_x(1, x1)
 *                .set_dynamic_input_x(2, x2)
 *                .set_attr_N(2)
 *                .set_attr_axis(1);
 */
const char* const PACK_ATTR_NAME_NUM = "N";
const char* const PACK_ATTR_NAME_AXIS = "axis";

REG_OP(Pack)
.DYNAMIC_INPUT(x, TensorType({ DT_FLOAT, DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32 }))
.ATTR(axis, AttrValue::INT { 0 })
.REQUIRED_ATTR(N, AttrValue::INT)
.OP_END()

/*
 * Rearranges blocks of spatial data, into depth. More specifically, this op outputs a copy of
 * the input tensor where values from the height and width dimensions are moved to the depth dimension.
 * The attr block_size indicates the input block size.
 * <Input>
 *    x : Input tensor with shape [n, c, h, w] or [n, h, w, c], where h and w must be divisible by 'block_size'
 * <Output>
 *    y : Output tensor
 * <Attr>
 *    block_size  : int (>= 1), size of the spatial block
 *    data_format : Format of input, 'NCHW' or 'NHWC'. Default is 'NHWC'
 * <Added in HiAI version>
 *    100.310.010.013
 */
const char* const SPACETODEPTH_ATTR_NAME_BLOCK_SIZE = "block_size";
const char* const SPACETODEPTH_ATTR_NAME_DATA_FORMAT = "data_format";

REG_OP(SpaceToDepth)
.INPUT(x, TensorType({ DT_FLOAT, DT_UINT8, DT_INT8 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_UINT8, DT_INT8 }))
.REQUIRED_ATTR(block_size, AttrValue::INT)
.ATTR(data_format, AttrValue::STR { "NHWC" })
.OP_END()

/*
 * Rearranges data from depth into blocks of spatial data.This is the reverse transformation of SpaceToDepth.
 * More specifically, this op outputs a copy of the input tensor where values from the depth dimension are moved
 * in spatial blocks to the height and width dimensions.
 * <Input>
 *    x : The input tensor with shape [ batch, height, width, channels ], channels
 *        must be divided by block_size*block_size.
 * <Output>
 *    y: The output tensor
 * <Attr>
 *    block_size : The size of the spatial block, must be greater than 0.
 *    mode : DCR (default) for depth-column-row order re-arrangement.
 *           CRD for column-row-depth order.
 *    data_format : The data format of input, now only support : NHWC.
 * <Added in HiAI version>
 *    100.320.010.010
 */
const char* const DEPTH_SPACE_ATTR_BLOCK_SIZE = "block_size";
const char* const DEPTH_SPACE_ATTR_DATA_FORMAT = "data_format";
const char* const DEPTH_SPACE_ATTR_MODE = "mode";

REG_OP(DepthToSpace)
.INPUT(x, TensorType({ DT_FLOAT }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.REQUIRED_ATTR(block_size, AttrValue::INT)
.ATTR(mode, AttrValue::STR { "DCR" })
.ATTR(data_format, AttrValue::STR { "NHWC" })
.OP_END()

/*
 * Extracts a strided slice of a tensor
 * <Input>
 *    x       : Input tensor
 *    begin   : An int32 tensor, must be const op
 *    end     : An int32 tensor, must be const op
 *    strides : An int32 tensor, must be const op
 * <Output>
 *    y : Output tensor
 * <Attr>
 *    begin_mask       : An int32 mask, defaults to 0.
 *    end_mask         : An int32 mask, defaults to 0.
 *    ellipsis_mask    : An int32 mask, defaults to 0.
 *    new_axis_mask    : An int32 mask, defaults to 0.
 *    shrink_axis_mask : Defaults to 0.
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({5, 6, 7, 8}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc beginTensorDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr beginTensor = std::make_shared<hiai::Tensor>(beginTensorDesc);
 *    vector<int32_t> beginDataValue = {1, 3, 3, 5};
 *    beginTensor->SetData((uint8_t*)beginDataValue.data(), 4 * sizeof(int32_t));
 *    auto begin = hiai::op::Const("begin").set_attr_value(beginTensor);
 *
 *    TensorDesc endTensorDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr endTensor = std::make_shared<hiai::Tensor>(endTensorDesc);
 *    vector<int32_t> endDataValue = {-2, 7, 0, 6};
 *    endTensor->SetData((uint8_t*)endDataValue.data(), 4 * sizeof(int32_t));
 *    auto end = hiai::op::Const("end").set_attr_value(endTensor);
 *
 *    TensorDesc stridesTensorDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr stridesTensor = std::make_shared<hiai::Tensor>(stridesTensorDesc);
 *    vector<int32_t> stridesDataValue = {2, 2, 2, 2};
 *    stridesTensor->SetData((uint8_t*)stridesDataValue.data(), 4 * sizeof(int32_t));
 *    auto strides = hiai::op::Const("strides").set_attr_value(stridesTensor);
 *
 *    auto stridedSlice = hiai::op::StridedSlice("stridedSlice")
 *                        .set_input_x(x)
 *                        .set_input_begin(begin)
 *                        .set_input_end(end)
 *                        .set_input_strides(strides)
 *                        .set_attr_begin_mask(1)
 *                        .set_attr_end_mask(15)
 *                        .set_attr_ellipsis_mask(0)
 *                        .set_attr_new_axis_mask(0)
 *                        .set_attr_shrink_axis_mask(0);
 */
const char* const STRIDE_SLICE_ATTR_BEGIN_MASK = "begin_mask";
const char* const STRIDE_SLICE_ATTR_END_MASK = "end_mask";
const char* const STRIDE_SLICE_ATTR_ELLIPSIS_MASK = "ellipsis_mask";
const char* const STRIDE_SLICE_ATTR_NEW_AXIS_MASK = "new_axis_mask";
const char* const STRIDE_SLICE_ATTR_SHRINK_AXIS_MASK = "shrink_axis_mask";

REG_OP(StridedSlice)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.INPUT(begin, TensorType({ DT_INT32 }))
.INPUT(end, TensorType({ DT_INT32 }))
.INPUT(strides, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.ATTR(begin_mask, AttrValue::INT { 0 })
.ATTR(end_mask, AttrValue::INT { 0 })
.ATTR(ellipsis_mask, AttrValue::INT { 0 })
.ATTR(new_axis_mask, AttrValue::INT { 0 })
.ATTR(shrink_axis_mask, AttrValue::INT { 0 })
.OP_END()

/*
 * Extracts a strided slice of a tensor.
 * <Input>
 *    x       : Tensor of data to extract slices from.
 *    begin   : 1-D tensor of starting indices of corresponding axis in `axes`. Only support Const op.
 *    end     : 1-D tensor of ending indices (exclusive) of corresponding axis in `axes`. Only support Const op.
 *    axes    : 1-D tensor of axes that `starts` and `ends` apply to. Only support Const op.
 *    strides : 1-D tensor of slice step of corresponding axis in `axes`.. Only support Const op.
 * <Output>
 *    y : Output tensor
 * <Attr>
 *    stridedsliceV2 needs to set begin_mask???end_mask???ellipsis_mask???new_axis_mask???shrink_axis_mask to 0;
 *    If stridedsliceV2 attr mask is not 0, the function is reserved.
 * <Added in HiAI version>
 *    100.500.010.010
 * <Examples>
 *
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc beginTensorDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr beginTensor = std::make_shared<hiai::Tensor>(beginTensorDesc);
 *    vector<int32_t>offsetsDataValue = {0, 1, 0, 1};
 *    beginTensor->SetData((uint8_t*)offsetsDataValue.data(), 4 * sizeof(int32_t));
 *    auto offsets = hiai::op::Const("offsets").set_attr_value(beginTensor);
 *
 *    TensorDesc endTensorDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr endTensor = std::make_shared<hiai::Tensor>(endTensorDesc);
 *    vector<int32_t> sizeDataValue = {1, 3, 2, -1};
 *    endTensor->SetData((uint8_t*)sizeDataValue.data(), 4 * sizeof(int32_t));
 *    auto size = hiai::op::Const("size").set_attr_value(endTensor);
 *
 *    auto StridedSliceV2 = hiai::op::StridedSliceV2("StridedSliceV2")
 *                         .set_input_x(x)
 *                         .set_input_offsets(begin)
 *                         .set_input_size(end)
 *                         .set_attr_begin_mask(0)
 *                         .set_attr_end_mask(0)
 *                         .set_attr_ellipsis_mask(0)
 *                         .set_attr_new_axis_mask(0)
 *                         .set_attr_shrink_axis_mask(0);
 */
REG_OP(StridedSliceV2)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.INPUT(begin, TensorType({ DT_INT32 }))
.INPUT(end, TensorType({ DT_INT32 }))
.OPTIONAL_INPUT(axes, TensorType ({ DT_INT32 }))
.OPTIONAL_INPUT(strides, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.ATTR(begin_mask, AttrValue::INT { 0 })
.ATTR(end_mask, AttrValue::INT { 0 })
.ATTR(ellipsis_mask, AttrValue::INT { 0 })
.ATTR(new_axis_mask, AttrValue::INT { 0 })
.ATTR(shrink_axis_mask, AttrValue::INT { 0 })
.OP_END()

/*
 * Zero-pads and then rearranges blocks of spatial data into batch.The operation outputs
 * a copy of the input tensor, in which the values from the height and width dimensions
 * are moved to the batch dimension.
 * <Input>
 *    x : 4-D tensor with shape [batch, depth, height, width]. Both height and width must be divisible
 *        by 'block_shape'.
 *    block_shape : Const OP, 1-D with shape [M], where all values must be >= 1
 *    paddings : Const OP, 2-D with shape [M, 2], where, all values must be >= 0
 *               paddings = [[pad_top, pad_bottom], [pad_left, pad_right]]
 *               The effective spatial dimensions of the zero-padded input tensor will be:
 *               height_pad = pad_top + height + pad_bottom
 *               width_pad = pad_left + width + pad_right
 *               Both 'height_pad' and 'width_pad' must be divisible by 'block_size'.
 * <Output>
 *    y : Tensor of the same type as 'x'
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({5, 6, 7, 8}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc blockShapeTensorDesc(Shape({2}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr blockShapeTensor = std::make_shared<hiai::Tensor>(blockShapeTensorDesc);
 *    vector<int32_t> blockShapeDataValue = {1, 2};
 *    blockShapeTensor->SetData((uint8_t*)blockShapeDataValue.data(), 2 * sizeof(int32_t));
 *    auto blockShape = hiai::op::Const("blockShape").set_attr_value(blockShapeTensor);
 *
 *    TensorDesc paddingsTensorDesc(Shape({2, 2}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr paddingsTensor = std::make_shared<hiai::Tensor>(paddingsTensorDesc);
 *    vector<int32_t> paddingsDataValue = {1, 2, 2, 4};
 *    paddingsTensor->SetData((uint8_t*)paddingsDataValue.data(), 4 * sizeof(int32_t));
 *    auto paddings = hiai::op::Const("paddings").set_attr_value(paddingsTensor);
 *
 *    auto spaceToBatchND = hiai::op::SpaceToBatchND("spaceToBatchND")
 *                          .set_input_x(x)
 *                          .set_input_block_shape(blockShape)
 *                          .set_input_paddings(paddings);
 */
REG_OP(SpaceToBatchND)
.INPUT(x, TensorType({ DT_FLOAT }))
.INPUT(block_shape, TensorType({ DT_INT32 }))
.INPUT(paddings, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.OP_END()

/*
 * This operation reshapes the "batch" dimension 0 into height and width that divided by block.
 * Interleaves these blocks back into the grid defined by the spatial dimensions [batch, depth, height, width],
 * to obtain a result with the same rank as the input. The spatial dimensions of this intermediate result
 * are then optionally cropped according to crops to produce the output.
 * <Input>
 *    x : 4-D tensor with shape [batch, depth, height, width]. It is required that the elements of x.dimension[0]
 *        must be divided by block_shape.dimension[0] * block_shape.dimension[1].
 *    block_shape : Const OP, 1-D with shape [M], where all values must be >= 1
 *    crops : Const OP, 2-D with shape [M, 2], where all values must be >= 0. crops[i] = [crop_start, crop_end]
 *            specifies the amount to crop from input dimension i + 1, which corresponds to spatial dimension i.
 *            It is required that crop_start[i] + crop_end[i] <= block_shape[i] * input_shape[i + 1].
 * <Output>
 *    y : Tensor of the same type as 'x'
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 6, 7, 8}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc blockShapeTensorDesc(Shape({2}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr blockShapeTensor = std::make_shared<hiai::Tensor>(blockShapeTensorDesc);
 *    vector<int32_t> blockShapeDataValue = {1, 2};
 *    blockShapeTensor->SetData((uint8_t*)blockShapeDataValue.data(), 2 * sizeof(int32_t));
 *    auto blockShape = hiai::op::Const("blockShape").set_attr_value(blockShapeTensor);
 *
 *    TensorDesc cropsTensorDesc(Shape({2, 2}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr cropsTensor = std::make_shared<hiai::Tensor>(cropsTensorDesc);
 *    vector<int32_t> cropsDataValue = {1, 2, 2, 4};
 *    cropsTensor->SetData((uint8_t*)cropsDataValue.data(), 4 * sizeof(int32_t));
 *    auto crops = hiai::op::Const("crops").set_attr_value(cropsTensor);
 *
 *    auto batchToSpaceND = hiai::op::BatchToSpaceND("batchToSpaceND")
 *                          .set_input_x(x)
 *                          .set_input_block_shape(blockShape)
 *                          .set_input_crops(crops);
 */
REG_OP(BatchToSpaceND)
.INPUT(x, TensorType({ DT_FLOAT }))
.INPUT(block_shape, TensorType({ DT_INT32 }))
.INPUT(crops, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.OP_END()

/*
 * Constructs a tensor by tiling a given tensor.
 * <Input>
 *    x : Input tensor
 *    multiples : Const OP
 * <Output>
 *    y : Output tensor
 * <Added in HiAI version>
 *    100.310.010.013
 */
REG_OP(Tile)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.INPUT(multiples, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.OP_END()

/*
 * Returns the size of a tensor.
 * <Input>
 *    x : Input tensor
 * <Output>
 *    y : Output tensor
 * <Attr>
 *    dtype : Reserved. data type of the output y.
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({1, 4, 1, 1}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    auto size = hiai::op::Size("size")
 *               .set_input_x(x);
 *
 *    auto cast = hiai::op::CastT("cast")
 *               .set_input_x(size)
 *               .set_attr_src_dtype(3)
 *               .set_attr_dst_dtype(1);
 *
 *    auto add = hiai::op::Add("add")
 *               .set_input_x1(x)
 *               .set_input_x2(cast);
 */
const char* const SIZE_ATTR_DTYPE = "dtype";

REG_OP(Size)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.OUTPUT(y, TensorType({ DT_INT32 }))
.ATTR(dtype, AttrValue::INT { DT_INT32 })
.OP_END()
/*
 * Creates a tensor of shape 'dims' and fills it with 'value'.
 * <Input>
 *    dims : Const OP, 1-D, shape of the output tensor
 *    value : 0-D, value to fill the returned tensor
 * <Output>
 *    y : Output tensor
 * <Added in HiAI version>
 *    100.310.010.013
 */
REG_OP(Fill)
.INPUT(dims, TensorType({ DT_INT32 }))
.INPUT(value, TensorType({ DT_FLOAT, DT_BOOL, DT_INT32, DT_UINT8 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_BOOL, DT_INT32, DT_UINT8 }))
.OP_END()
/*
 * Returns the inverse permutation of a tensor.
 * <Input>
 *    x : Input tensor
 * <Output>
 *    y : Output tensor
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    auto invertPermutation = hiai::op::InvertPermutation("invertPermutation")
 *                             .set_input_x(x);
 */
REG_OP(InvertPermutation)
.INPUT(x, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_INT32 }))
.OP_END()

/*
 * Performs tensor select.
 * <Input>
 *    condition : the condition of select. Only support first dimension broadcast.
 *    x1 : First operand.
 *    x2 : Second operand, has the same shape of x1.
 * <Output>
 *    y : Result, has same element type as two inputs.
 * <Added in HiAI version>
 *    100.320.010.010
 */
REG_OP(Select)
.INPUT(condition, TensorType({ DT_BOOL }))
.INPUT(x1, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.INPUT(x2, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8, DT_BOOL }))
.OP_END()

/*
 * Reverses variable length slices.
 * <Input>
 *    x : A rank R >0 tensor to reverse
 *    seq_lengths : 1-D with length input, seq_lengths.dimension[0] == x.dimension[batch_dim]
 * <Output>
 *    y : Output tensor
 * <Attr>
 *    seq_dim : dimension partially reversed. 'seq_dim' should not equal to 'batch_dim'.
 *              'seq_dim' must be in range [-R, R).
 *    batch_dim : dimension along which reversal is performed, default to 0.
 *                'batch_dim' must not equal to 'seq_dim'.
 *                'batch_dim' must be in range [-R, R).
 * <Added in HiAI version>
 *    100.310.010.013
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc seqLengthsTensorDesc(Shape({5}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr seqLengthsTensor = std::make_shared<hiai::Tensor>(seqLengthsTensorDesc);
 *    vector<int32_t> dataValue(5, 1);
 *    seqLengthsTensor->SetData((uint8_t*)dataValue.data(), 5 * sizeof(int32_t));
 *    auto seqLengths = hiai::op::Const("seqLengths").set_attr_value(seqLengthsTensor);
 *
 *    auto reverseSequence = hiai::op::ReverseSequence("reverseSequence")
 *                           .set_input_x(x)
 *                           .set_input_seq_lengths(seqLengths)
 *                           .set_attr_batch_dim(1)
 *                           .set_attr_seq_dim(2);
 */
const char* const REVERSE_SEQUENCE_ATTR_SEQ_DIM = "seq_dim";
const char* const REVERSE_SEQUENCE_ATTR_BATCH_DIM = "batch_dim";

REG_OP(ReverseSequence)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8 }))
.INPUT(seq_lengths, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_UINT8 }))
.REQUIRED_ATTR(seq_dim, AttrValue::INT)
.ATTR(batch_dim, AttrValue::INT { 0 })
.OP_END()

/*
 * The operation pads input according to the paddings and constant_values.
 * <Input>
 *    x : The input tensor.
 *    paddings : The values of paddings, as a role of dimensions to be added on the input tensor x,
 *               must be a Const-OP.
 *    constant_values : A tensor of the same type as x, that indicates the value to use for padding input,
 *                      must be a Const-OP.
 * <Output>
 *    y : The output tensor.
 * <Added in HiAI version>
 *    100.320.010.010
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc biasTensorDesc(Shape({4, 2}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr biasTensor = std::make_shared<hiai::Tensor>(biasTensorDesc);
 *    vector<int32_t> dataValue = {0, 1, 2, 3, 4, 5, 6, 7};
 *    biasTensor->SetData((uint8_t*)dataValue.data(), 8 * sizeof(int32_t));
 *    auto paddings = hiai::op::Const("paddings").set_attr_value(biasTensor);
 *
 *    TensorDesc constantValuesTensorDesc(Shape({1}), FORMAT_NCHW, DT_FLOAT);
 *    TensorPtr constantValuesTensor = std::make_shared<hiai::Tensor>(constantValuesTensorDesc);
 *    vector<float> constantValuesDataValue = {1.0};
 *    constantValuesTensor->SetData((uint8_t*)constantValuesDataValue.data(), 1 * sizeof(float));
 *    auto constantValues = hiai::op::Const("constantValues").set_attr_value(constantValuesTensor);
 *
 *    auto padV2 = hiai::op::PadV2("PadV2")
 *                 .set_input_x(x)
 *                 .set_input_paddings(paddings)
 *                 .set_input_constant_values(constantValues);
 */
REG_OP(PadV2)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32 }))
.INPUT(paddings, TensorType({ DT_INT32 }))
.INPUT(constant_values, TensorType({ DT_FLOAT, DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32 }))
.OP_END()

/*
 * Given a tensor input, this operation returns a tensor of the same type with all dimensions of size 1 removed.
 * If you do not want to remove all size with 1 dimensions,
 * you can remove specific size 1 dimensions by specifying axis.
 * <Input>
 *    x : The input to be squeezed.
 * <Output>
 *    y : The output tensor.
 * <Attr>
 *    axis : The dimensions which to remove.
 * <Added in HiAI version>
 *    100.320.010.010
 */
const char* const SQUEEZE_ATTR_AXIS = "axis";

REG_OP(Squeeze)
.INPUT(x, TensorType({ DT_FLOAT }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.ATTR(axis, AttrValue::LIST_INT ({}))
.OP_END()

/*
 * Extracts, rearranges, and combines input data.
 * <Input>
 *    x : Input tensor
 *    reindex : Index of input tensor
 * <Output>
 *    y : Output tensor
 * <Added in HiAI version>
 *    100.320.010.010
 */
REG_OP(BatchReindex)
.INPUT(x, TensorType({ DT_FLOAT }))
.INPUT(reindex, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.OP_END()

/*
 * Scatter 'updates' into a new tensor according to 'indices'.
 * <Input>
 *    var     : Input tensor.
 *    indices : An Index tensor.
 *    updates : A tensor.
 * <Output>
 *    var: A Tensor. Has the same type and format as input "var".
 * <Attr>
 *    use_locking : An optional bool. Defaults to "False". If "True", the operation will be protected by a lock.
 *    axis        : Which axis to scatter on. Negative value means counting dimensions from the back.Accepted range is
 *                  [-r, r-1] where r = rank(data).
 * <Added in HiAI version>
 *    100.500.010.010
 */
const char* const SCATTERUPDATE_ATTR_USE_LOCKING = "use_locking";
const char* const SCATTERUPDATE_ATTR_AXIS = "axis";

REG_OP(ScatterUpdate)
.INPUT(var, TensorType({ DT_FLOAT, DT_INT8, DT_UINT8, DT_BOOL}))
.INPUT(indices, TensorType({ DT_INT32, DT_INT64 }))
.INPUT(updates, TensorType({ DT_FLOAT, DT_INT8, DT_UINT8, DT_BOOL }))
.OUTPUT(var, TensorType({ DT_FLOAT, DT_INT8, DT_UINT8, DT_BOOL }))
.ATTR(use_locking, AttrValue::BOOL { false })
.ATTR(axis, AttrValue::INT { 0 })
.OP_END()

/*
 * Pads a tensor.
 * <Input>
 *    x : The input tensor
 *    paddings : The values of padding, as a role of dimensions to be added on the input tensor x,
 *               with shape [D, 2], D is the rank of x.
 * <Output>
 *    y : The output tensor
 * <Added in HiAI version>
 *    100.320.010.010
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc biasTensorDesc(Shape({4, 2}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr biasTensor = std::make_shared<hiai::Tensor>(biasTensorDesc);
 *    vector<int32_t> dataValue = {0, 1, 2, 3, 4, 5, 6, 7};
 *    biasTensor->SetData((uint8_t*)dataValue.data(), 8 * sizeof(int32_t));
 *    auto paddings = hiai::op::Const("paddings").set_attr_value(biasTensor);
 *
 *    auto pad = hiai::op::Pad("Pad")
 *               .set_input_x(x)
 *               .set_input_paddings(paddings);
 */
REG_OP(Pad)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32 }))
.INPUT(paddings, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32 }))
.OP_END()

/*
 * Pads a tensor with mirrored values.
 * <Input>
 *    x : The input tensor
 *    paddings : The values of paddings, as a role of dimensions to be added on the input tensor x,
 *               must be a Const-OP.
 * <Output>
 *    y : The output tensor
 * <Attr>
 *    mode : REFLECT or SYMMETRIC.
 *           SYMMETRIC  paddings must be no greater than the x dimension size
 *           REFLECT  paddings must be less than the x dimension size
 * <Added in HiAI version>
 *    100.320.010.010
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc biasTensorDesc(Shape({4, 2}), FORMAT_NCHW, DT_INT32);
 *    TensorPtr biasTensor = std::make_shared<hiai::Tensor>(biasTensorDesc);
 *    vector<int32_t> dataValue = {0, 1, 2, 3, 4, 5, 6, 7};
 *    biasTensor->SetData((uint8_t*)dataValue.data(), 8 * sizeof(int32_t));
 *    auto paddings = hiai::op::Const("paddings").set_attr_value(biasTensor);
 *
 *    auto mirrorPad = hiai::op::MirrorPad("mirrorPad")
 *                    .set_input_x(x)
 *                    .set_input_paddings(paddings)
 *                    .set_attr_mode("SYMMETRIC");
 */
const char* const MIRRORPAD_ATTR_MODE = "mode";

REG_OP(MirrorPad)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_BOOL, DT_INT64 }))
.INPUT(paddings, TensorType({ DT_INT32, DT_INT64 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT32, DT_BOOL, DT_INT64 }))
.REQUIRED_ATTR(mode, AttrValue::STR)
.OP_END()

/*
 * Return a one-hot tensor
 * <Input>
 *    x : A index tensor.
 *    depth : A scalar defining the depth of the one hot dimension.
 *    on_value : A scalar defining the value to fill in output when x[j] = i. (default: 1).
 *    off_value : A scalar defining the value to fill in output when x[j] != i. (default: 0).
 * <Output>
 *    y : Result, has element type as T. The dimension of y not support bigger than 4.
 * <Attr>
 *    axis : The axis to fill.(default: -1)
 * <added in HiAI version>
 *    100.320.010.010
 * <Examples>
 *    TensorDesc xDesc(Shape({4}), FORMAT_NCHW, DT_INT32);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    TensorDesc depthTensorDesc(Shape(), FORMAT_NCHW, DT_INT32);
 *    TensorPtr depthTensor = std::make_shared<hiai::Tensor>(depthTensorDesc);
 *    vector<int32_t> depthDataValue = {1};
 *    depthTensor->SetData((uint8_t*)depthDataValue.data(), 1 * sizeof(int32_t));
 *    auto depth = hiai::op::Const("depth").set_attr_value(depthTensor);
 *
 *    TensorDesc onValueTensorDesc(Shape(), FORMAT_NCHW, DT_FLOAT);
 *    TensorPtr onValueTensor = std::make_shared<hiai::Tensor>(onValueTensorDesc);
 *    vector<float> onValueDataValue = {1.0};
 *    onValueTensor->SetData((uint8_t*)onValueDataValue.data(), 1 * sizeof(float));
 *    auto onValue = hiai::op::Const("onValue").set_attr_value(onValueTensor);
 *
 *    TensorDesc offValueTensorDesc(Shape(), FORMAT_NCHW, DT_FLOAT);
 *    TensorPtr offValueTensor = std::make_shared<hiai::Tensor>(offValueTensorDesc);
 *    vector<float> offValueDataValue = {1.0};
 *    offValueTensor->SetData((uint8_t*)offValueDataValue.data(), 1 * sizeof(float));
 *    auto offValue = hiai::op::Const("offValue").set_attr_value(offValueTensor);
 *
 *    auto oneHot = hiai::op::OneHot("oneHot")
 *                  .set_input_x(x)
 *                  .set_input_depth(depth)
 *                  .set_input_on_value(onValue)
 *                  .set_input_off_value(offValue)
 *                  .set_attr_axis(0);
 */
const char* const ONEHOT_ATTR_AXIS = "axis";

REG_OP(OneHot)
.INPUT(x, TensorType({ DT_INT32, DT_UINT8 }))
.INPUT(depth, TensorType({ DT_INT32 }))
.INPUT(on_value, TensorType({ DT_UINT8, DT_INT8, DT_FLOAT, DT_BOOL }))
.INPUT(off_value, TensorType({ DT_UINT8, DT_INT8, DT_FLOAT, DT_BOOL }))
.OUTPUT(y, TensorType({ DT_UINT8, DT_INT8, DT_FLOAT, DT_BOOL }))
.ATTR(axis, AttrValue::INT { -1 })
.OP_END()

/*
 * Obtain the shape of input tensor,  expressed in the form of one-dimensional integer tensor.
 * <Input>
 *    x : The input tensor.
 * <Output>
 *    y: The output tensor.
 * <Attr>
 *    dtype : Reserved. data type of the output y.
 * <Added in HiAI version>
 *    100.320.010.010
 * <Examples>
 *    TensorDesc xDesc(Shape({4, 5, 6, 7}), FORMAT_NCHW, DT_FLOAT);
 *    hiai::op::Data x = hiai::op::Data("x");
 *    x.update_input_desc_x(xDesc);
 *
 *    auto shape = hiai::op::Shape("shape")
 *               .set_input_x(x);
 */
const char* const SHAPE_ATTR_DTYPE = "dtype";

REG_OP(Shape)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT32, DT_BOOL, DT_UINT8 }))
.OUTPUT(y, TensorType({ DT_INT32 }))
.ATTR(dtype, AttrValue::INT { DT_INT32 })
.OP_END()

/*
 * Dequantizes the 'input' tensor into a float tensor.
 * <Input>
 *    x : Tensor of type uint8.
 *    min_range : Tensor of type float, defining the minimum scalar value possibly produced for the input.
 *                Min need lessequal 0.
 *    max_range : Tensor of type float, defining the maximum scalar value possibly produced for the input.
 * <Output>
 *    y : Tensor of type float.
 * <Attr>
 *    mode : Reserved.
 * <Added in HiAI version>
 *    100.320.010.010
 */
const char* const DEQUANTIZE_ATTR_MODE = "mode";

REG_OP(Dequantize)
.INPUT(x, TensorType({ DT_UINT8 }))
.INPUT(min_range, TensorType({ DT_FLOAT }))
.INPUT(max_range, TensorType({ DT_FLOAT }))
.OUTPUT(y, TensorType({ DT_FLOAT }))
.ATTR(mode, AttrValue::STR { "MIN_COMBINED" })
.OP_END()

/*
 * Quantizes the 'input' tensor of type float to 'output' tensor of type uint8.
 * <Input>
 *    x          : Tensor of type float.
 *    min_range  : Tensor of type float, defining the minimum scalar value possibly produced for the input.
 *                 Min need lessequal 0.
 *    max_range  : Tensor of type float, defining the maximum scalar value possibly produced for the input.
 * <Output>
 *    y          : Tensor of type uint8.
 * <Attr>
 *    mode : Reserved.
 * <Added in HiAI version>
 *    100.320.010.010
 */
const char* const QUANTIZE_ATTR_MODE = "mode";

REG_OP(Quantize)
.INPUT(x, TensorType({ DT_FLOAT }))
.INPUT(min_range, TensorType({ DT_FLOAT }))
.INPUT(max_range, TensorType({ DT_FLOAT }))
.OUTPUT(y, TensorType({ DT_UINT8 }))
.ATTR(mode, AttrValue::STR { "MIN_COMBINED" })
.OP_END()

/*
 * Broadcasts an array for a compatible shape.
 * <Input>
 *    x : A Tensor to broadcast.
 *    shape : The shape of the desired output, must be const op.
 * <Output>
 *    y : Result, has the same type as x.
 * <Added in HiAI version>
 *    100.500.010.010
 */
REG_OP(BroadcastTo)
.INPUT(x, TensorType({ DT_FLOAT, DT_INT8, DT_UINT8, DT_BOOL }))
.INPUT(shape, TensorType({ DT_INT32 }))
.OUTPUT(y, TensorType({ DT_FLOAT, DT_INT8, DT_UINT8, DT_BOOL }))
.OP_END()
} // namespace hiai
#pragma GCC diagnostic pop

#endif  // _HIAI_GRAPH_OP_GE_OP_ARRAY_DEFS_H
