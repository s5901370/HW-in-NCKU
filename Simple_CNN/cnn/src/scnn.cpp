#include "CNN.hpp"

void cnn(char * filepath) {
	// read cnn
	CNN cnn;
	fstream fin(filepath, ios::in);
	fin >> cnn;
	fin.close();

	// predict
	vector<double> input;
	double n;
	while (cin >> n) input.push_back(n);
	cout << cnn.getResult(input);
	cout.flush();
}

void nn(char * filepath) {
	// read nn
	NN nn;
	fstream fin(filepath, ios::in);
	fin >> nn;
	fin.close();

	// predict
	vector<double> input;
	double n;
	for (int i = 0; i < 784; i++) {
		cin >> n;
		input.push_back(n);
	}
	cout << argmax(nn.getResult(input));
	cout.flush();
}

int main(int argc, char * argv[]) {
	// cnn();
	nn(argv[1]);
	return 0;
}
