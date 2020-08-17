#include "util.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

void setup() { // put it once in main function for random (MUST DO !!!)
	srand((unsigned)time(NULL));
}

double sigmoid(double x) {
	return (1 / (1 + exp(-x)));
}

double sigmoid_derivative(double x) {
	return sigmoid(x) * (1 - sigmoid(x));
}

double relu(double x) {
	return (x > 0)? x: 0;
}

double relu_derivative(double x) {
	return (x > 0)? 1: 0;
}

double activation_func(double x, int F, bool derivative) {
	switch (F) {
		case LINEAR:
			if (!derivative) return x;
			else return 1;
			break;
		case SIGMOID:
			if (!derivative) return sigmoid(x);
			else return sigmoid_derivative(x);
			break;
		case RELU:
			if (!derivative) return relu(x);
			else return relu_derivative(x);
			break;
		default:
			return 0;
	}
}

string activation_func_name(int F) {
	switch (F) {
		case LINEAR: return "Linear"; break;
		case SIGMOID: return "Sigmoid"; break;
		case RELU: return "Relu"; break;
		default: return "Not a function";
	}
}

double loss_func(const vector<double>& x, const vector<double>& y, int F) {
	double sum;
	switch (F) {
		case MSE:
			if (x.size() != y.size()) throw "Loss function size error";
			sum = 0;
			for (int j = 0; j < x.size(); j++) {
				double d = x[j] - y[j];
				sum += d * d;
			}
			return sum / 2;
			break;
		case CE:
			if (x.size() != y.size()) throw "Loss function size error";
			sum = 0;
			for (int j = 0; j < x.size(); j++) {
				sum += log(1 + exp(- y[j] * x[j]));
			}
			return sum;
			break;
		case SCE:
			if (x.size() != y.size()) throw "Loss function size error";
			sum = 0;
			for (int j = 0; j < x.size(); j++) {
				sum += log2(1 + exp(- y[j] * x[j]));
			}
			return sum;
			break;
		default:
			return 0;
	}
}

double loss_func_partial(double xj, double yj, int F) {
	switch (F) {
		case MSE:
			return xj - yj;
			break;
		case CE:
			return - yj / (1 + exp(yj * xj));
			break;
		case SCE:
			return - yj / (1 + exp(yj * xj)) / log(2);
			break;
		default:
			return 0;
	}
}

string loss_func_name(int F) {
	switch (F) {
		case MSE: return "Mean-Square Error"; break;
		case CE: return "Corss-Entropy Error"; break;
		case SCE: return "Scaled Corss-Entropy Error"; break;
		default: return "Not a function";
	}
}

vector<string> split(const string s, const string delim) {
	vector<string> v;
  string::size_type pos1, pos2;
  pos2 = s.find(delim);
  pos1 = 0;
  while (string::npos != pos2) {
    v.push_back(s.substr(pos1, pos2 - pos1));
    pos1 = pos2 + delim.size();
    pos2 = s.find(delim, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
	return v;
}

string& trim(string &s) {
	if (s.empty()) return s;
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

int argmax(vector<double> v) {
	double max = (v.size() > 0)? v[v.size()-1]: 0;
	int id = v.size();
	for (int i = v.size() - 1; i >= 0; i--) {
		if (max <= v[i]) {
			max = v[i];
			id = i;
		}
	}
	return id;
}

Layers reshape(vector<double> v, int height, int width, int depth) {
	Layers maps;
	for (int l = 0; l < depth; l++) {
		vector<vector<double> > layer;
		for (int i = 0; i < height; i++) {
			vector<double> row;
			for (int j = 0; j < width; j++) {
				row.push_back(v[(l * depth + i) * height + j]);
			}
			layer.push_back(row);
		}
		maps.push_back(layer);
	}
	return maps;
}

Layers init_layers(int l, int h, int w, double value) {
	Layers layers;
	for (int i = 0; i < l; i++) {
		vector<vector<double> > layer;
		for (int j = 0; j < h; j++) {
			vector<double> row;
			for (int k = 0; k < w; k++) {
				row.push_back(value);
			}
			layer.push_back(row);
		}
		layers.push_back(layer);
	}
	return layers;
}

vector<vector<double> > init_matrix(int h, int w, double value) {
	vector<vector<double> > matrix;
	for (int i = 0; i < h; i++) {
		vector<double> row;
		for (int j = 0; j < w; j++) {
			row.push_back(value);
		}
		matrix.push_back(row);
	}
	return matrix;
}

vector<double> init_vector(int n, double value) {
	vector<double> v;
	for (int i = 0; i < n; i++) {
		v.push_back(value);
	}
	return v;
}