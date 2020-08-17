#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <cmath>
#include <string>
using namespace std;

#define DEFAULT_LEARNING_RATE 0.5

typedef vector<vector<vector<double> > > Layers;
typedef vector<vector<double> > Config;

enum ActivationFunction {LINEAR = 0, SIGMOID = 1, RELU = 2};
enum LossFunction {MSE = 0, CE = 1, SCE = 2};

void setup(); // put it once in main function for random (MUST DO !!!)
double sigmoid(double x);
double sigmoid_derivative(double x);
double relu(double x);
double relu_derivative(double x);
double activation_func(double x, int F = SIGMOID, bool derivative = false);
string activation_func_name(int F);
double loss_func(const vector<double>& x, const vector<double>& y, int F);
double loss_func_partial(double xj, double yj, int F);
string loss_func_name(int F);
vector<string> split(const string s, const string delim);
string& trim(string &s);
int argmax(vector<double> v);
Layers reshape(vector<double> v, int height, int width, int depth);
Layers init_layers(int l, int h, int w, double value = 0);
vector<vector<double> > init_matrix(int h, int w, double value = 0);
vector<double> init_vector(int n, double value = 0);

#endif