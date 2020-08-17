#include "Neuron.hpp"
#include "BinaryStream.hpp"

Neuron::Neuron(int input_num, double learning_rate, int f): learning_rate(learning_rate), f(f) {
	for (int i = 0; i <= input_num; ++i) { // bias as weight[0]
		weight.push_back((double) 2 * rand() / RAND_MAX - 1);
		delta_weight.push_back(0);
	}
}

double& Neuron::operator[](int i) {
	if (i >= weight.size()) throw "Neuron index out of range";
	return weight[i];
}

double Neuron::operator[](int i) const {
	if (i >= weight.size()) throw "Neuron index out of range";
	return weight[i];
}

void Neuron::cal(const vector<double>& input) {
	if (input.size() != weight.size() - 1) throw "Neuron size error";
	s = weight[0]; // bias
	for (int i = 0; i < input.size(); ++i) {
		s += input[i] * weight[i+1];
	}
}

void Neuron::calOutputDelta(const double expected_output, int loss_function) {
	delta = loss_func_partial(activation_func(s, f), expected_output, loss_function) * activation_func(s, f, true);
}

void Neuron::calHiddenDelta(const vector<Neuron>& next, int i) {
	double total = 0;
	for(int j = 0; j < next.size(); ++j) {
		total += next[j].getDelta() * next[j][i];
	}
	delta = total * activation_func(s, f, true);
}

void Neuron::update(const vector<double>& input) {
	weight[0] -= learning_rate * delta; // bias, input[bias] = +1
	for (int i = 0; i < input.size(); ++i) {
		weight[i+1] -= learning_rate * delta * input[i];
	}
}

double Neuron::calSquareError() {
	double error = 0;
	for (int i = 0; i < weight.size(); ++i) {
		double c = weight[i] - delta_weight[i];
		error += c * c;
		delta_weight[i] = weight[i];
	}
	return error;
}

ostream& operator<<(ostream& os, const Neuron& n) {
	// { int size, double learning_rate, int f, double weight[size] }
	BinaryStream bs;
	// write obj to stream
	bs.writeInt(os, n.weight.size());
	bs.writeDouble(os, n.learning_rate);
	bs.writeInt(os, n.f);
	for (int i = 0; i < n.weight.size(); i++)
		bs.writeDouble(os, n.weight[i]);
	return os;
}

istream& operator>>(istream& is, Neuron& n) {
	// { int size, double learning_rate, int f, double weight[size] }
	BinaryStream bs;
	// read obj from stream
	int size = bs.readInt(is);
	n.learning_rate = bs.readDouble(is);
	n.f = bs.readInt(is);
	n.weight.clear();
	for (int i = 0; i < size; i++) {
		n.weight.push_back(bs.readDouble(is));
		n.delta_weight.push_back(0);
	}
	return is;
}

void Neuron::print() {
	cout << "Neuron:" << endl;
	cout << "learning rate: " << learning_rate << endl;
	cout << "Activation function: " << activation_func_name(f) << endl;
	cout << "weights:" << endl;
	for (int i = 0; i < weight.size(); ++i)
		cout << weight[i] << " ";
	cout << endl;
}