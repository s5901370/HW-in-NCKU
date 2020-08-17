#include "Convolution.hpp"

bool In(int index, int size) {
	return 0 <= index && index < size;
}

Convolution::Convolution(Layers kernels, int strides, int padding, int f, int ph, int pw, double learning_rate):
	strides(strides), padding(padding), f(f), pooling_height(ph), pooling_width(pw), learning_rate(learning_rate) {
	for (int k = 0; k < kernels.size(); k++) {
		this->kernels.push_back(Kernel(kernels[k]));
		int kernel_height = kernels[k].size(), kernel_width = 0;
		if (kernel_height > 0) kernel_width = kernels[k][0].size();
		delta_kernel_weight.push_back(init_matrix(kernel_height, kernel_width, 0));
	}
}

Convolution::Convolution(int kn, int kh, int kw, int strides, int padding, int f, int ph, int pw, double learning_rate):
	strides(strides), padding(padding), f(f), pooling_height(ph), pooling_width(pw), learning_rate(learning_rate) {
	for (int i = 0; i < kn; i++) {
		kernels.push_back(Kernel(kh, kw)); // not checking if they are the same
		delta_kernel_weight.push_back(init_matrix(kh, kw, 0));
	}
}

Kernel& Convolution::operator[](int k) {
	if (k >= kernels.size()) throw "Kernel index out of range";
	return kernels[k];
}

Kernel Convolution::operator[](int k) const {
	if (k >= kernels.size()) throw "Kernel index out of range";
	return kernels[k];
}

int Convolution::init_alias() {
	S = strides;
	D = padding;
	MH = 0; MW = 0; KH = 0; KW = 0;
	MN = input_map.size();
	if (MN > 0) {
		MH = input_map[0].size();
		if (MH > 0) MW = input_map[0][0].size();
	}
	KN = kernels.size();
	if (KN > 0) {
		KH = kernels[0].getHeight();
		KW = kernels[0].getWidth();
	}
	CH = (MH + 2 * padding - KH) / strides + 1;
	CW = (MW + 2 * padding - KW) / strides + 1;
	ph = pooling_height; pw = pooling_width;
	PH = (CH % ph == 0)? (CH / ph): (CH / ph + 1);
	PW = (CW % pw == 0)? (CW / pw): (CW / pw + 1);
	FN = PH * PW;
	return FN;
}

Layers Convolution::conv() {
	init_alias();
	// initializing convolution map
	s_map.clear();
	s_map = init_layers(kernels.size(), CH, CW, 0);
	conv_map.clear();
	conv_map = init_layers(kernels.size(), CH, CW, 0);
	// convolution
	for (int k = 0; k < KN; k++)
	for (int a = 0; a < CH; a++)
	for (int b = 0; b < CW; b++)
	for (int h = 0; h < MN; h++)
	for (int p = 0; p < KH; p++)
	for (int q = 0; q < KW; q++) {
		if ( In(S*a+p-D, MH) && In(S*b+q-D, MW) ) {
			s_map[k][a][b] += input_map[h][S*a+p-D][S*b+q-D] * kernels[k][p][q];
		}
	}
	for (int k = 0; k < KN; k++)
	for (int a = 0; a < CH; a++)
	for (int b = 0; b < CW; b++) {
		s_map[k][a][b] /= KN * KH * KW; // *
		conv_map[k][a][b] = activation_func(s_map[k][a][b], f);
	}
	return conv_map;
}

Layers Convolution::max_pooling() {
	init_alias();
	// initialization
	// find the minimum
	double min = 0;
	for (int k = 0; k < KN; k++)
	for (int a = 0; a < CH; a++)
	for (int b = 0; b < CW; b++)
		if (conv_map[k][a][b] < min) min = conv_map[k][a][b];
	pooling_map.clear();
	pooling_map = init_layers(conv_map.size(), PH, PW, min - 1);
	// max pooling
	for (int k = 0; k < KN; k++)
	for (int a = 0; a < CH; a++)
	for (int b = 0; b < CW; b++) {
		if (pooling_map[k][a/ph][b/ph] < conv_map[k][a][b])
			pooling_map[k][a/ph][b/ph] = conv_map[k][a][b];
	}
	return pooling_map;
}

vector<double> Convolution::flatten() {
	init_alias();
	output_vector.clear();
	for (int k = 0; k < KN; k++)
	for (int u = 0; u < PH; u++)
	for (int v = 0; v < PW; v++) {
		output_vector.push_back(pooling_map[k][u][v]);
	}
	return output_vector;
}

void Convolution::calOutputDelta(vector<double>& nn_delta) {
	init_alias();
	Layers dp = reshape(nn_delta, PH, PW, KN); // poolinged size delta
	delta.clear();
	delta = init_layers(KN, CH, CW, 0); // original size delta of next layer
	for (int k = 0; k < KN; k++)
	for (int a = 0; a < CH; a++)
	for (int b = 0; b < CW; b++) {
		delta[k][a][b] = dp[k][a/ph][b/pw] / (ph * pw);
	}
}

void Convolution::calHiddenDelta(const Convolution& next) {
	init_alias();
	Layers dp = next.getDelta(); // poolinged size delta
	// calculate delta
	int NS = next.getStrides();
	int ND = next.getPadding();
	delta.clear();
	delta = init_layers(KN, CH, CW, 0);
	for (int h = 0; h < KN; h++)
	for (int i = 0; i < CH; i++)
	for (int j = 0; j < CW; j++)
	for (int k = 0; k < dp.size(); k++)
	for (int a = 0; a < dp[k].size(); a++)
	for (int b = 0; b < dp[k][a].size(); b++) {
		if ( In(i/ph-NS*a+ND, next[k].getHeight()) && In(j/pw-NS*b+ND, next[k].getWidth()) ) {
			delta[h][i][j] += dp[k][a][b] * next[k][i/ph-NS*a+ND][j/pw-NS*b+ND] * activation_func(s_map[h][i][j], f, true);
			delta[h][i][j] /= next[k].size() * next[k].getHeight() * next[k].getWidth(); // *
		}
	}
}

void Convolution::update() {
	init_alias();
	for (int k = 0; k < KN; k++)
	for (int p = 0; p < KH; p++)
	for (int q = 0; q < KW; q++) {
		double total = 0;
		for (int a = 0; a < CH; a++)
		for (int b = 0; b < CW; b++) {
			double total_m = 0;
			for (int h = 0; h < MN; h++) {
				total_m += input_map[h][S*a+p-D][S*b+q-D];
			}
			total += delta[k][a][b] * total_m / (KN * KH * KW); // *
		}
		kernels[k][p][q] -= learning_rate * total;
	}
}

double Convolution::calSquareError() {
	init_alias();
	double error = 0;
	for (int k = 0; k < KN; k++)
	for (int p = 0; p < KH; p++)
	for (int q = 0; q < KW; q++) {
		double c = kernels[k][p][q] - delta_kernel_weight[k][p][q];
		error += c * c;
		delta_kernel_weight[k][p][q] = kernels[k][p][q];
	}
	return error;
}

ostream& operator<<(ostream& os, const Convolution& c) {
	// { int strides, int padding, int f, int pooling_height, int pooling_width, double learning_rate, int kernel_num, Kernel[kernel_num] }
	BinaryStream bs;
	// write obj to stream
	bs.writeInt(os, c.strides);
	bs.writeInt(os, c.padding);
	bs.writeInt(os, c.f);
	bs.writeInt(os, c.pooling_height);
	bs.writeInt(os, c.pooling_width);
	bs.writeDouble(os, c.learning_rate);
	bs.writeInt(os, c.f);
	bs.writeInt(os, c.kernels.size());
	for (int k = 0; k < c.kernels.size(); k++)
		os << c.kernels[k];
	return os;
}

istream& operator>>(istream& is, Convolution& c) {
	// { int strides, int padding, int f, int pooling_height, int pooling_width, double learning_rate, int kernel_num, Kernel[kernel_num] }
	BinaryStream bs;
	// read obj from stream
	c.strides = bs.readInt(is);
	c.padding = bs.readInt(is);
	c.f = bs.readInt(is);
	c.pooling_height = bs.readInt(is);
	c.pooling_width = bs.readInt(is);
	c.learning_rate = bs.readDouble(is);
	c.f = bs.readInt(is);
	int kernel_num = bs.readInt(is);
	for (int k = 0; k < kernel_num; k++) {
		Kernel kernel;
		is >> kernel;
		c.kernels.push_back(kernel);
		c.delta_kernel_weight.push_back(init_matrix(kernel.getHeight(), kernel.getWidth()));
	}
	return is;
}

void Convolution::print() {
	cout << "strides = " << strides << endl;
	cout << "padding = " << padding << endl;
	cout << "Activation function = " << activation_func_name(f) << endl;
	cout << "pooling height = " << pooling_height << endl;
	cout << "pooling width = " << pooling_width << endl;
	cout << "learning rate = " << learning_rate << endl;
	print_kernels();
}

void Convolution::print_input() {
	init_alias();
	for (int h = 0; h < MN; h++) {
		for (int i = 0; i < MH; i++) {
			for (int j = 0; j < MW; j++) {
				cout << input_map[h][i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void Convolution::print_kernels() {
	init_alias();
	for (int k = 0; k < KN; k++) {
		cout << "Kernel " << k << ":" << endl;
		kernels[k].print();
	}
}

void Convolution::print_conv() {
	init_alias();
	for (int k = 0; k < KN; k++) {
		for (int a = 0; a < CH; a++) {
			for (int b = 0; b < CW; b++) {
				cout << conv_map[k][a][b] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void Convolution::print_pooling() {
	init_alias();
	for (int k = 0; k < KN; k++) {
		for (int u = 0; u < PH; u++) {
			for (int v = 0; v < PW; v++) {
				cout << pooling_map[k][u][v] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void Convolution::print_flatten() {
	init_alias();
	for (int i = 0; i < FN; i++)
		cout << output_vector[i] << " ";
	cout << endl;
}