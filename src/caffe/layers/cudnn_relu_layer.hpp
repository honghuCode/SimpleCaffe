//
// Created by yipeng on 2020/3/25.
//
#ifndef SIMPLE_CAFFE_CUDNN_RELU_LAYER_HPP_
#define SIMPLE_CAFFE_CUDNN_RELU_LAYER_HPP_

#include <vector>

#include "caffe/tensor.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/activation_layer.hpp"
#include "caffe/layers/relu_layer.hpp"

namespace caffe {

#ifdef USE_CUDNN

template <typename Dtype>
class CuDNNReLULayer : public ReLULayer<Dtype> {
 public:
	explicit CuDNNReLULayer(const LayerParameter& param)
			: ReLULayer<Dtype>(param), handles_setup_(false) {}

	//重写 层初始化接口
	virtual void LayerSetUp(const vector<Tensor<Dtype>*>& bottom,
													const vector<Tensor<Dtype>*>& top) override;
	//重写 reshape接口
	virtual void Reshape(const vector<Tensor<Dtype>*>& bottom,
	                     const vector<Tensor<Dtype>*>& top) override;
	virtual ~CuDNNReLULayer();

 protected:
	/*
	 * 内部函数： 层的前向计算
	 * 输入tensor vector大小是1个tensor
	 * 输出tensor vector大小也是1个tensor
	 * 计算y = max(0, x) 如果有negative_slope 会计算是负数的情况
	 */
	virtual void Forward_gpu(const vector<Tensor<Dtype>*>& bottom,
	                         const vector<Tensor<Dtype>*>& top) override;
	/*
	 * 内部函数： 层的反向计算
	 * 输出tensor vector大小是1个tensor
	 * bool vector 表明下标对应值是否反向传播
	 * 输入tensor vector大小也是1个tensor
	 */
	virtual void Backward_gpu(const vector<Tensor<Dtype>*>& top,
	                          const vector<bool>& propagate_down,
	                          const vector<Tensor<Dtype>*>& bottom) override;

	bool handles_setup_;
	cudnnHandle_t cudnn_handle_;                  //cudnn句柄
	cudnnTensorDescriptor_t bottom_desc_;         //输入张量的描述符
	cudnnTensorDescriptor_t top_desc_;            //输出张量的描述符
	cudnnActivationDescriptor_t activation_desc_; //激活函数操作的描述符
};
#endif //USE_CUDNN

}      //namespace caffe

#endif //SIMPLE_CAFFE_CUDNN_RELU_LAYER_HPP_
