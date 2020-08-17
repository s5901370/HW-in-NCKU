#ifndef BINARY_STREAM_H
#define BINARY_STREAM_H

#include <iostream>
using namespace std;

class BinaryStream {
public:
	int getInt() { return bs.n; } // get int in the union
	void getInt(ostream& os) { os << bs.n; } // read from stream in the readable format
	void setInt(int n) { clear(); bs.n = n; } // set int in the union
	void setInt(istream& is) { clear(); is >> bs.n; } // write to stream in the readable format
	double getDouble() { return bs.r; } // get int in the union
	void getDouble(ostream& os) { os << bs.r; } // read from stream in the readable format
	void setDouble(double r) { clear(); bs.r = r; } // set double in the union
	void setDouble(istream& is) { clear(); is >> bs.r; } // write to stream in the readable format
	void read(istream& is); // read from the original format, in Big-Endian
	void write(ostream& os); // store in the original format, in Big-Endian
	int readInt(istream& is); // read int from the original format, in Big-Endian
	void writeInt(ostream& os, int n); // store int in the original format, in Big-Endian
	double readDouble(istream& is);  // read double from the original format, in Big-Endian
	void writeDouble(ostream& os, double r); // store int in the original format, in Big-Endian
	static bool isBigEndian(); // check if is Big-Endian
private:
	union {
		int n;
		double r;
		char s[8];
	} bs;
	char * getBuf() { return bs.s; } // get char pointer in the union
	void clear() { for (int i = 0; i < 8; i++) bs.s[i] = 0; } // erase the union
};

#endif