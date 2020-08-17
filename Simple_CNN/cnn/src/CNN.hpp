#ifndef CNN_H
#define CNN_H

#include "Convolution.hpp"
#include "NN.hpp"

class CNN {
public:
	CNN() {}
	CNN(Layers config, vector<string> labels);
	void setConvolution(Config config);
	void setNN(Config config);
	vector<string> getlabels() { return labels; }
	vector<double> conv_forward(Layers& input);
	void conv_backProp(vector<double>& nn_delta);
	void forward(const vector<double>& input);
	void backProp(const vector<double>& expect_output);
	vector<double> getOutput(const vector<double>& input);
	string getResult(const vector<double>& input);
	double calStandardError();
	double loss_error(const Records& data, bool show_per_record = false);
	double sample_error(const Records& data, bool show_per_record = false);
	double train(Records& train_data, bool show = false, bool show_per_record = false, bool show_detail = false);
	double test(Records& test_data, bool show = false);
	friend ostream& operator<<(ostream& os, const CNN& cnn);
	friend istream& operator>>(istream& is, CNN& cnn);
	void print();
private:
	vector<Convolution> conv_layers;
	vector<string> labels;
	NN nn;
	int map_height;
	int map_width;
	int map_depth;
};

#endif