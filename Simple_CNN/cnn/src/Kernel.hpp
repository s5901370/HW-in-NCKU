#ifndef KERNEL_H
#define KERNEL_H

#include "BinaryStream.hpp"
#include <vector>
#include <iostream>
using namespace std;

class Kernel {
public:
	Kernel() {}
	Kernel(int height, int width);
	Kernel(vector<vector<double> > map): map(map) {}
	vector<double>& operator[](int row) { return map[row]; }
	int size() const { return map.size(); }
	int getHeight() const { return map.size(); }
	int getWidth() const { if (map.size() > 0) return map[0].size(); return 0; }
	friend ostream& operator<<(ostream& os, const Kernel& k);
	friend istream& operator>>(istream& is, Kernel& k);
	void print();
private:
	vector<vector<double> > map;
};

#endif