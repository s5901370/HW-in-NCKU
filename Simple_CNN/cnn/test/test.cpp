#include "../src/Kernel.hpp"
#include "../src/Convolution.hpp"
#include "../src/BinaryStream.hpp"
#include "../src/NN.hpp"
#include "../src/Records.hpp"
#include "../src/CNN.hpp"
#include <fstream>
#include <iomanip>
using namespace std;

void test_kernel() {
	Kernel k(3, 3);
	k.print();
	
	fstream fout("tmp/c_k_output.txt", ios::out);
	fout << k;
	fout.close();
	fstream fin("tmp/c_k_output.txt", ios::in);
	Kernel kk;
	fin >> kk;
	fin.close();
	kk.print();
}

void test_conv() {
	Layers map = {
		{
			{1,2,3,4,5,6,7},
			{8,9,10,11,12,13,14},
			{15,16,17,18,19,10,21},
			{22,23,24,25,26,27,28},
			{29,30,31,32,33,34,35},
			{36,37,38,39,40,41,42},
			{43,44,45,46,47,48,49}
		},
		{
			{-1,2,3,4,5,6,7},
			{8,9,10,11,12,13,14},
			{15,16,17,18,19,10,21},
			{22,23,24,25,26,27,28},
			{29,30,31,32,33,34,35},
			{36,37,38,39,40,41,42},
			{43,44,45,46,47,48,0}
		}
	};
	Convolution convolution(3, 3, 3, 1, 1, false, 2, 2);

	cout << "input map:" << endl;
	convolution.feed(map);

	cout << "kernels:" << endl;
	convolution.print_kernels();

	convolution.conv(); // with relu
	// convolution.conv(false); // not with relu
	cout << "convolution map:" << endl;
	convolution.print_conv();

	convolution.max_pooling();
	cout << "pooling map:" << endl;
	convolution.print_pooling();

	convolution.flatten();
	cout << "flatten vector:" << endl;
	convolution.print_flatten();
}

void test_bs_1() {
	BinaryStream bs;
	bs.setDouble(cin);
	cout << setprecision(100) << bs.getDouble() << endl;
	fstream fout("tmp/bs_test1_output.txt", ios::out);
	bs.write(fout);
	fout.close();
	fstream fin("tmp/bs_test1_output.txt", ios::in);
	double rr = bs.readDouble(fin);
	cout << setprecision(100) << rr << endl;
}

void test_bs_2() {
	BinaryStream bs;
	int n = 8;
	double d = n;
	bs.setDouble(d);
	cout << setprecision(100) << bs.getDouble() << endl;
	fstream fout("tmp/bs_test2_output.txt", ios::out);
	bs.write(fout);
	fout.close();
	fstream fin("tmp/bs_test2_output.txt", ios::in);
	double rr = bs.readDouble(fin);
	cout << setprecision(100) << rr << endl;
}

void test_bs_endian() {
	cout << "Big-Endian = " << BinaryStream::isBigEndian() << endl;
}

void test_records() {
	Records records("test/data/records.csv", ",", "b");
	for (int i = 0; i < records.size(); i++) {
		for (int j = 0; j < records[i].data.size(); j++) {
			cout << records[i].data[j] << " ";
		}
		cout << endl;
	}
}

void test_neuron_1() {
	Neuron n_out(10, 0.1, RELU);
	n_out.print();
	
	fstream fout("tmp/test_neuron_1_output.txt", ios::out);
	fout << n_out;
	fout.close();
	fstream fin("tmp/test_neuron_1_output.txt", ios::in);
	Neuron n_in;
	fin >> n_in;
	fin.close();

	n_in.print();
}

void test_neuron_2() {
	Neuron n_out({2, 3, 4, 5, 6}, 0.8);
	n_out.print();
	
	fstream fout("tmp/test_neuron_2_output.txt", ios::out);
	fout << n_out;
	fout.close();
	fstream fin("tmp/test_neuron_2_output.txt", ios::in);
	Neuron n_in;
	fin >> n_in;
	fin.close();

	n_in.print();
}

void test_nn_1() {
	// simple test
	vector<double> b({0.15, 0.2, 0.35}), b1({0.25, 0.3, 0.35}), c({0.4, 0.45, 0.6}), c1({0.5, 0.55, 0.6});
	Layers weights;
	vector<vector<double> > w, w2;
	w.push_back(b); w.push_back(b1);
	w2.push_back(c); w2.push_back(c1);
	weights.push_back(w); weights.push_back(w2);
	NN nn(weights, 5e-4);
	vector<double> input({0.05, 0.1}), output({0.01, 0.99});
	nn.forward(input);
	nn.backProp(output);
	cout << nn.calStandardError() << endl;
}

void test_nn_2() {
	// test weights
	Layers weights = {
		{ {0.15, 0.2, 0.35}, {0.25, 0.3, 0.35} },
		{ {0.4, 0.45, 0.6}, {0.5, 0.55, 0.6} }
	};
	NN nn(weights, 5e-4);
	vector<double> input({0.05, 0.1}), output({0.01, 0.99});
	nn.forward(input);
	nn.backProp(output);
	cout << nn.calStandardError() << endl;
}

void test_nn_3() {
	vector<int> shape {2, 2, 2};
	NN nn(shape, 0, 1e4, 0.5, SIGMOID, MSE);

	Records train_data("test/data/hw4/hw4_train.dat", " ", 2);
	train_data.setLabel({"1", "-1"});
	Records test_data("test/data/hw4/hw4_test.dat", " ", 2);
	test_data.setLabelMap(train_data);

	double Ein = nn.train(train_data, true);
	double Eout = nn.test(test_data);

	nn.print();
	cout << "Ein = " << Ein << endl;
	cout << "Eout = " << Eout << endl;
}

void test_nn_4() {
	vector<vector<double> > shape {{20}, {5, 0.6, SIGMOID}, {2, 0.4, SIGMOID}};
	NN nn(shape, 0, 10000, MSE);

	Records train_data("test/data/nn/tttrain.txt", " ", 20);
	train_data.setLabel({"1", "-1"});
	Records test_data("test/data/nn/ttest.txt", " ", 20);
	test_data.setLabelMap(train_data);

	double Ein = nn.train(train_data, true);
	double Eout = nn.test(test_data);

	nn.print();
	cout << "Ein = " << Ein << endl;
	cout << "Eout = " << Eout << endl;
}

void test_nn_5() {
	// train number only use NN
	vector<vector<double> > config {{784}, {10, 0.5, SIGMOID}};
	NN nn(config, 0, 20, SCE);

	vector<string> labels = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	Records train_data("test/data/digit-recognizer/train.csv", ",", "label");
	train_data.setLabel(labels);
	Records test_data("test/data/digit-recognizer/test.csv", ",", "label");
	test_data.setLabel(labels);

	train_data.normalization(0, 255);
	test_data.normalization(0, 255);

	double Ein = nn.train(train_data, true);
	double Eout = nn.test(test_data);

	nn.print();
	cout << "Ein = " << Ein << endl;
	cout << "Eout = " << Eout << endl;
}

void test_nn_6() {
	// train number only use NN
	vector<vector<double> > config {{784}, {20, 0.5, SIGMOID}, {10, 0.5, SIGMOID}};
	NN nn(config, 0, 10, SCE);

	vector<string> labels = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	Records train_data("test/data/digit-recognizer/train.csv", ",", "label");
	train_data.setLabel(labels);
	Records test_data("test/data/digit-recognizer/test.csv", ",", "label");
	test_data.setLabel(labels);

	train_data.normalization(0, 255);
	test_data.normalization(0, 255);

	double Ein = nn.train(train_data, true);
	double Eout = nn.test(test_data);

	fstream fout("data/digit_784_20_10.nn", ios::out);
	fout << nn;
	fout.close();

	nn.print();
	cout << "Ein = " << Ein << endl;
	cout << "Eout = " << Eout << endl;
}

void test_cnn_1() {
	Layers config = {
		{
			{28, 28, 1}, // map_height, map_width, map_depth
			{32, 3, 3, 2, 0, RELU, 2, 2, 0.5}, // kernel_num, kernek_height, kernel_weight, stride, padding, activation_function, pooling_height, pooling_width, learning rate
			{64, 3, 3, 1, 0, RELU, 2, 2, 0.5}
		},
		{
			{0, 10, SCE}, // eps, N, loss_function
			{20, 0.5, RELU}, // hidden_layer, learning_rate, activation_function
			{10, 0.5, SIGMOID} // output_layer, learning_rate, activation_function
		}
	};
	vector<string> labels = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

	CNN cnn(config, labels);

	Records train_data("test/data/digit-recognizer/train.csv", ",", "label");
	train_data.setLabel(labels);
	Records test_data("test/data/digit-recognizer/test.csv", ",", "label");
	test_data.setLabelMap(train_data);

	train_data.normalization(0, 255);
	test_data.normalization(0, 255);

	double Ein = cnn.train(train_data, true, true);
	double Eout = cnn.test(test_data, true);

	fstream fout("tmp/test_cnn_1.cnn", ios::out);
	fout << cnn;
	fout.close();

	cnn.print();
	cout << "Ein = " << Ein << endl;
	cout << "Eout = " << Eout << endl;
}

void test_cnn_2() {
	// using saved CNN model
	CNN cnn;
	fstream fin("tmp/test_cnn_1.cnn", ios::in);
	fin >> cnn;
	fin.close();

	vector<string> labels = cnn.getlabels();
	Records train_data("test/data/digit-recognizer/train.csv", ",", "label");
	train_data.setLabel(labels);
	Records test_data("test/data/digit-recognizer/test.csv", ",", "label");
	test_data.setLabel(labels);

	train_data.normalization(0, 255);
	test_data.normalization(0, 255);

	double Ein = cnn.train(train_data, true, true, true);
	double Eout = cnn.test(test_data, true);

	fstream fout("tmp/test_cnn_1.cnn", ios::out);
	fout << cnn;
	fout.close();

	cnn.print();
	cout << "Ein = " << Ein << endl;
	cout << "Eout = " << Eout << endl;
}

int main() {
	try {

		setup(); // must appear just once in main function
		// test_kernel();
		// test_conv();
		// test_bs_1();
		// test_bs_2();
		// test_bs_endian();
		// test_records();
		// test_neuron_1();
		// test_neuron｀_2();
		// test_nn_1();
		// test_nn_2();
		// test_nn_3();
		// test_nn_4();
		// test_nn_5();
		test_nn_6();
		// test_cnn_1();
		// test_cnn_2();

	} catch (char const * s) {
		cout << s << endl;
	}
	return 0;
}
