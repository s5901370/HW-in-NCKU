#include "CNN.hpp"
using namespace std;

CNN::CNN(Layers config, vector<string> labels): labels(labels) {
	setConvolution(config[0]);
	setNN(config[1]);
}

void CNN::setConvolution(Config config) {
	// map
	map_height = (config[0].size() > 0)? config[0][0]: 0;
	map_width = (config[0].size() > 1)? config[0][1]: 0;
	map_depth = (config[0].size() > 2)? config[0][2]: 1;
	// convolution Layers
	conv_layers.clear();
	for (int l = 1; l < config.size(); l++) {
		int kernel_num = (config[l].size() > 0)? config[l][0]: 0;
		int kernek_height = (config[l].size() > 1)? config[l][1]: 4;
		int kernel_weight = (config[l].size() > 2)? config[l][2]: 4;
		int stride = (config[l].size() > 3)? config[l][3]: 1;
		int padding = (config[l].size() > 4)? config[l][4]: 0;
		int f = (config[l].size() > 5)? config[l][5]: true;
		int pooling_height = (config[l].size() > 6)? config[l][6]: 2;
		int pooling_width = (config[l].size() > 7)? config[l][7]: 2;
		double learning_rate = (config[l].size() > 8)? config[l][8]: DEFAULT_LEARNING_RATE;
		Convolution layer(kernel_num, kernek_height, kernel_weight, stride, padding, f, pooling_height, pooling_width, learning_rate);
		conv_layers.push_back(layer);
	}
}

void CNN::setNN(Config config) {
	// calculate convolution output number
	Layers input = init_layers(map_depth, map_height, map_width, 0);
	int NN_input_size = conv_forward(input).size();
	// NN
	double eps = (config[0].size() > 0)? config[0][0]: 0;
	int N = (config[0].size() > 1)? config[0][1]: 0;
	int loss_function = (config[0].size() > 2)? config[0][2]: SIGMOID;
	config[0] = vector<double>(1, NN_input_size);
	nn = NN(config, eps, N, loss_function);
}

vector<double> CNN::conv_forward(Layers& input) {
	for (int l = 0; l < conv_layers.size(); l++) {
		conv_layers[l].feed(input);
		conv_layers[l].conv();
		input = conv_layers[l].max_pooling();
		conv_layers[l].print_pooling();
		conv_layers[l].print_kernels();
	}
	return conv_layers[conv_layers.size()-1].flatten();
}

void CNN::conv_backProp(vector<double>& nn_delta) {
	// calculate the delta of last layer of conv
	conv_layers[conv_layers.size() - 1].calOutputDelta(nn_delta);
	// calculate hidden layer delta
	for (int l = conv_layers.size() - 1; l > 0; l--) {
		conv_layers[l-1].calHiddenDelta(conv_layers[l]);
	}
	// update all weights
	for (int l = conv_layers.size() - 1; l >= 0; l--) {
		conv_layers[l].update();
	}
}

void CNN::forward(const vector<double>& input) {
	Layers data = reshape(input, map_height, map_width, map_depth);
	nn.forward(conv_forward(data));
}

void CNN::backProp(const vector<double>& expect_output) {
	vector<double> nn_delta = nn.backProp(expect_output);
	// cout << "nn_delta" << endl;
	// for (int i = 0; i < nn_delta.size(); i++) cout << nn_delta[i] << " ";
	// cout << endl;
	conv_backProp(nn_delta);
}

vector<double> CNN::getOutput(const vector<double>& input) {
	forward(input);
	return nn.getOutput(nn.size() - 1);
}

string CNN::getResult(const vector<double>& input) {
	int ans = argmax(getOutput(input));
	return labels[ans];
}

double CNN::calStandardError() {
	double error = 0;
	double count = 0;
	for (int l = 0; l < nn.size(); ++l) {
		for (int j = 0; j < nn[l].size(); ++j) {
			count += nn[l][j].size();
			error += nn[l][j].calSquareError();
		}
	}
	for (int l = 0; l < conv_layers.size(); l++) {
		for (int k = 0; k < conv_layers[l].size(); k++) {
			count += conv_layers[l][k].getHeight() * conv_layers[l][k].getWidth();
		}
		error += conv_layers[l].calSquareError();
	}
	return sqrt(error / count);
}

double CNN::loss_error(const Records& data, bool show_per_record) {
	double err_sum = 0;
	for (int i = 0; i < data.size(); ++i) {
		vector<double> res = getOutput(data[i].data);
		err_sum += loss_func(res, data[i].output, nn.getLossFunction());
		if (show_per_record) {
			cout << "record" << i << "loss error = " << err_sum / i << endl;
		}
	}
	return err_sum / data.size();
}

double CNN::sample_error(const Records& data, bool show_per_record) {
	double err_num = 0;
	for (int i = 0; i < data.size(); ++i) {
		string ans = getResult(data[i].data);
		if (ans != data[i].label) err_num++;
		if (show_per_record) {
			cout << "record " << i << ", ans = " << ans << ", expect = " <<  data[i].label << ", sample error = " << err_num / i << endl;
		}
	}
	return err_num / data.size();
}

double CNN::train(Records& train_data, bool show, bool show_per_record, bool show_detail) {
	int N = nn.getN();
	double eps = nn.getEps();
	int count = 0;
	int iteration = 0;
	double loss_err = 0;
	while (iteration < N || N == 0) {
		forward(train_data[count].data);
		backProp(train_data[count].output);
		if (show_per_record) {
			double err = loss_func(getOutput(train_data[count].data), train_data[count].output, nn.getLossFunction());
			if (!show_detail)
				cout << "iteration = " << iteration << ", record = " << count << ", loss error = " << err << endl;
			if (show_detail) {
				double sample_err = sample_error(train_data, show_per_record);
				cout << "iteration = " << iteration << ", record = " << count << ", loss error = " << err << ", Ein = " << sample_err << endl;
			}
		}
		count++;
		if (count % train_data.size() == 0) {
			double loss_err_now = loss_error(train_data, show_per_record);
			if (0 < loss_err - loss_err_now && loss_err - loss_err_now < eps) break;
			count %= train_data.size();
			iteration++;
			if (show) cout << "iteration = " << iteration << ", delta loss = " << loss_err - loss_err_now << ", Ein = " << sample_error(train_data, show_per_record) << endl;
			train_data.shuffle();
			loss_err = loss_err_now;
		}
	}

	return sample_error(train_data);
}

double CNN::test(Records& test_data, bool show) {
	return sample_error(test_data, show);
}

ostream& operator<<(ostream& os, const CNN& cnn) {
	// { int map_height, int map_width, int map_depth, int layer_size, Convolution[layer_size], nn }
	BinaryStream bs;
	// write obj to stream
	bs.writeInt(os, cnn.map_height);
	bs.writeInt(os, cnn.map_width);
	bs.writeInt(os, cnn.map_depth);
	bs.writeInt(os, cnn.conv_layers.size());
	for (int l = 0; l < cnn.conv_layers.size(); l++) {
		os << cnn.conv_layers[l];
	}
	os << cnn.nn;
	bs.writeInt(os, cnn.labels.size());
	for (int i = 0; i < cnn.labels.size(); i++) {
		os << cnn.labels[i] << endl;
	}
	return os;
}

istream& operator>>(istream& is, CNN& cnn) {
	// { int map_height, int map_width, int map_depth, int layer_size, Convolution[layer_size], nn, int label_num, string labels }
	BinaryStream bs;
	// read obj from stream
	cnn.map_height = bs.readInt(is);
	cnn.map_width = bs.readInt(is);
	cnn.map_depth = bs.readInt(is);
	int layer_size = bs.readInt(is);
	for (int l = 0; l < layer_size; l++) {
		Convolution c;
		is >> c;
		cnn.conv_layers.push_back(c);
	}
	is >> cnn.nn;
	int label_num = bs.readInt(is);
	cnn.labels.clear();
	for (int i = 0; i < label_num; i++) {
		string s;
		is >> s;
		cnn.labels.push_back(s);
	}
	return is;
}

void CNN::print() {
	for (int l = 0; l < conv_layers.size(); l++) {
		cout << "Convolution Layer " << l << ":" << endl;
		conv_layers[l].print();
	}
	nn.print();
}