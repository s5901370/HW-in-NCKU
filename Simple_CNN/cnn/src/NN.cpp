#include "NN.hpp"
#include "BinaryStream.hpp"
using namespace std;

NN::NN(const vector<int> shape, double eps, int N, double learning_rate, int activation_function, int loss_function): eps(eps), N(N), loss_function(loss_function) {
	// hidden and output layer, input layer is not used here
	for (int i = 1; i < shape.size(); ++i) {
		vector<Neuron> layer;
		for (int j = 0; j < shape[i]; ++j)
			layer.push_back(Neuron(shape[i-1], learning_rate, activation_function)); // number of prev layer neurons
		w.push_back(layer);
	}
}

NN::NN(const vector<vector<double> > shape_learning_rate, double eps, int N, int loss_function): eps(eps), N(N), loss_function(loss_function) {
	// hidden and output layer, input layer is not used here
	for (int i = 1; i < shape_learning_rate.size(); ++i) {
		vector<Neuron> layer;
		int shape = (shape_learning_rate[i-1].size() > 0)? shape_learning_rate[i-1][0]: 0;
		for (int j = 0; j < (int)shape_learning_rate[i][0]; ++j) {
			// number of prev layer neurons
			double learning_rate = (shape_learning_rate[i].size() > 1)? shape_learning_rate[i][1]: DEFAULT_LEARNING_RATE;
			int activation_function = (shape_learning_rate[i].size() > 2)? shape_learning_rate[i][2]: SIGMOID;
			layer.push_back(Neuron(shape, learning_rate, activation_function));
		}
		w.push_back(layer);
	}
}

NN::NN(Layers weights, double eps, int N, double learning_rate, int activation_function, int loss_function): eps(eps), N(N), loss_function(loss_function) {
	// input, hidden and output layer
	// input layer should only have a bias for each neuron
	for (int i = 0; i < weights.size(); ++i) {
		vector<Neuron> layer;
		for (int j = 0; j < weights[i].size(); ++j)
			layer.push_back(Neuron(weights[i][j], learning_rate, activation_function));
		w.push_back(layer);
	}
}

vector<Neuron>& NN::operator[](int i) {
	if (i >= w.size()) throw "Layer index out of range";
	return w[i];
}

vector<Neuron> NN::operator[](int i) const {
	if (i >= w.size()) throw "Layer index out of range";
	return w[i];
}

vector<double> NN::getOutput(int l) {
	vector<double> output;
	for (int j = 0; j < w[l].size(); ++j) {
		output.push_back(w[l][j].getOutput());
	}
	return output;
}

void NN::forward(const vector<double>& input) {
	if (w.size() > 0 && w[0].size() > 0 && input.size() != w[0][0].size() - 1)
		throw "NN input size error";
	// assign input value
	for (int j = 0; j < w[0].size(); ++j)
		w[0][j].cal(input);
	// feed dorward
	for (int l = 1; l < w.size(); ++l) {
		for (int j = 0; j < w[l].size(); ++j) {
			w[l][j].cal(getOutput(l - 1));
		}
	}
	last_input = input;
}

vector<double> NN::backProp(const vector<double>& expect_output) {
	// calculate output layer delta
	vector<Neuron> &output_layer = w[w.size() - 1];
	if (expect_output.size() != output_layer.size()) throw "NN expect output size error";
	// cout << "delta " << w.size() - 1 << " = ";
	for (int j = 0; j < output_layer.size(); ++j) {
		output_layer[j].calOutputDelta(expect_output[j]);
		// cout << output_layer[j].getDelta() << " ";
	}
	// cout << endl;

	// calculate hidden layer delta
	for (int l = w.size() - 1; l > 0; --l) {
		// cout << "delta " << l - 1 << " = ";
		for (int j = 0; j < w[l-1].size(); ++j) {
			w[l-1][j].calHiddenDelta(w[l], j);
			// cout << w[l-1][j].getDelta() << " ";
		}
		// cout << endl;
	}

	// update all weights
	for (int l = w.size() - 1; l > 0; --l) { // hidden layers
		for (int j = 0; j < w[l].size(); ++j) {
			w[l][j].update(getOutput(l - 1));
		}
	}
	for (int j = 0; j < w[0].size(); ++j) {
		w[0][j].update(last_input);
	}
	
	vector<double> delta;
	for (int i = 0; i < w[0].size(); i++) {
		delta.push_back(w[0][i].getDelta());
	}
	return delta;
}

vector<double> NN::getResult(const vector<double>& input) {
	forward(input);
	return getOutput(w.size() - 1);
}

double NN::calStandardError() {
	double error = 0;
	double count = 0;
	for (int l = 0; l < w.size(); ++l) {
		for (int j = 0; j < w[l].size(); ++j) {
			count += w[l][j].size();
			error += w[l][j].calSquareError();
		}
	}
	return sqrt(error / count);
}

double NN::loss_error(const Records& data) {
	double err_sum = 0;
	for (int i = 0; i < data.size(); ++i) {
		vector<double> res = getResult(data[i].data);
		err_sum += loss_func(res, data[i].output, loss_function);
	}
	return err_sum / data.size();
}

double NN::sample_error(const Records& data) {
	double err_num = 0;
	for (int i = 0; i < data.size(); ++i) {
		vector<double> res = getResult(data[i].data);
		int ans = argmax(res);
		if (ans != data[i].id) err_num++;
	}
	return err_num / data.size();
}

double NN::train(Records train_data, bool show) {
	// eps == 0: just depend on N
	// N == 0: just depend on eps
	int count = 0;
	int iteration = 0;
	double loss_err = 0;
	while (iteration < N || N == 0) {
		forward(train_data[count].data);
		backProp(train_data[count].output);
		count++;
		if (count % train_data.size() == 0) {
			double loss_err_now = loss_error(train_data);
			if (0 < loss_err - loss_err_now && loss_err - loss_err_now < eps) break;
			count %= train_data.size();
			iteration++;
			if (show) cout << "iteration = " << iteration << ", delta loss = " << loss_err - loss_err_now << ", Ein = " << sample_error(train_data) << endl;
			train_data.shuffle();
			loss_err = loss_err_now;
		}
	}

	return sample_error(train_data);
}

double NN::test(Records test_data) {
	return sample_error(test_data);
}

ostream& operator<<(ostream& os, const NN& nn) {
	// { double eps, int N, int loss_function, int layer_num, { int neuron_num, Neuron[neuron_num] }[layer_num] }
	BinaryStream bs;
	// write obj to stream
	bs.writeDouble(os, nn.eps);
	bs.writeInt(os, nn.N);
	bs.writeInt(os, nn.loss_function);
	bs.writeInt(os, nn.w.size());
	for (int i = 0; i < nn.w.size(); i++) {
		bs.writeInt(os, nn.w[i].size());
		for (int j = 0; j < nn.w[i].size(); j++)
			os << nn.w[i][j];
	}
	return os;
}

istream& operator>>(istream& is, NN& nn) {
	// { double eps, int N, int loss_function, int layer_num, { int neuron_num, Neuron[neuron_num] }[layer_num] }
	BinaryStream bs;
	// read obj from stream
	nn.eps = bs.readDouble(is);
	nn.N = bs.readInt(is);
	nn.loss_function = bs.readInt(is);
	int layer_num = bs.readInt(is);
	for (int i = 0; i < layer_num; i++) {
		vector<Neuron> layer;
		int neuron_num = bs.readInt(is);
		for (int j = 0; j < neuron_num; j++) {
			Neuron n;
			is >> n;
			layer.push_back(n);
		}
		nn.w.push_back(layer);
	}
	return is;
}

void NN::print() {
	cout << "NN:" << endl;
	cout << "eps: " << eps << endl;
	cout << "N: " << N << endl;
	cout << "Loss function: " << loss_func_name(loss_function) << endl;
	for (int i = 0; i < w.size(); ++i) {
		cout << "Layer " << i + 1 << ": " << endl;
		for (int j = 0; j < w[i].size(); ++j)
			w[i][j].print();
		cout << endl;
	}
}