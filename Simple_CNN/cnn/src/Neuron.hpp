#ifndef NEURON_H
#define NEURON_H

#include "util.hpp"
#include <iostream>
using namespace std;

class Neuron {
public:
	Neuron(): learning_rate(DEFAULT_LEARNING_RATE), f(SIGMOID) {}
	Neuron(int input_num, double learning_rate = DEFAULT_LEARNING_RATE, int f = SIGMOID);
	Neuron(vector<double> w, double learning_rate = DEFAULT_LEARNING_RATE, int f = SIGMOID):
		weight(w), delta_weight(vector<double>(w.size(), 0)), learning_rate(learning_rate), f(f) {}
	double getOutput() const { return activation_func(s, f); }
	double getDelta() const { return delta; }
	double& operator[](int i);
	double operator[](int i) const;
	int size() { return weight.size(); }
	void cal(const vector<double>& input);
	void calOutputDelta(const double expected_output, int loss_function = MSE);
	void calHiddenDelta(const vector<Neuron>& next, int i);
	void update(const vector<double>& input);
	double calSquareError();
	friend ostream& operator<<(ostream& os, const Neuron& n);
	friend istream& operator>>(istream& is, Neuron& n);
	void print();
private:
	vector<double> weight; // [ weight_of_bias, weights... ], where bias is +1
	double learning_rate;
	int f; // activation function
	vector<double> delta_weight;
	double s; // the product of weight and input
	double delta; // partial E(w) / partial s
};

#endif