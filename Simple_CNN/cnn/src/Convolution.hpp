#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "Kernel.hpp"
#include "util.hpp"
using namespace std;

class Convolution {
public:
	Convolution(): f(LINEAR), learning_rate(DEFAULT_LEARNING_RATE) {}
	Convolution(Layers kernels, int strides = 1, int padding = 0, int f = LINEAR, int ph = 2, int pw = 2, double learning_rate = DEFAULT_LEARNING_RATE);
	Convolution(int kn, int kh = 3, int kw = 3, int strides = 2, int padding = 0, int f = LINEAR, int ph = 2, int pw = 2, double learning_rate = DEFAULT_LEARNING_RATE);
	vector<Kernel> getKernels() { return kernels; }
	int getStrides() const { return strides; }
	int getPadding() const { return padding; }
	Kernel& operator[](int k);
	Kernel operator[](int k) const;
	int size() const { return kernels.size(); }
	int init_alias();
	// forward
	void feed(Layers map) { input_map = map; }
	Layers conv();
	Layers max_pooling();
	vector<double> flatten();
	// back propagation
	Layers getDelta() const { return delta; }
	void calOutputDelta(vector<double>& nn_delta);
	void calHiddenDelta(const Convolution& next);
	void update();
	double calSquareError();
	friend ostream& operator<<(ostream& os, const Convolution& c);
	friend istream& operator>>(istream& is, Convolution& c);
	void print();
	void print_input();
	void print_kernels();
	void print_conv();
	void print_pooling();
	void print_flatten();
private:
	vector<Kernel> kernels;
	int strides;
	int padding;
	int f; // activation function
	int pooling_height;
	int pooling_width;
	double learning_rate;
	// tmp maps
	Layers input_map;
	Layers s_map;
	Layers conv_map;
	Layers pooling_map;
	vector<double> output_vector;
	Layers delta;
	Layers delta_kernel_weight;
	// alias (init_alias)
	int MN; // input map size
	int MH; // input map height
	int MW; // input map width
	int KN; // kernel numbers, conv map size, pooling map size
	int KH; // kernel height
	int KW; // kernel width
	int CH; // conv map height
	int CW; // conv map width
	int ph; // pooling height
	int pw; // pooling width
	int PH; // pooling map height
	int PW; // pooling map width
	int FN; // flatten vector length
	int S; // strides
	int D; // padding
};

#endif