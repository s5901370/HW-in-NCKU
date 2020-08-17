#include "BinaryStream.hpp"
using namespace std;

void BinaryStream::read(istream& is) {
	if (isBigEndian()) {
		is.read(getBuf(), 8);
	} else {
		for (int i = 7; i >=0; i--) {
			is.read(getBuf() + i, 1);
		}
	}
}

void BinaryStream::write(ostream& os) {
	if (isBigEndian()) {
		os.write(getBuf(), 8);
	} else {
		for (int i = 7; i >=0; i--) {
			os.write(getBuf() + i, 1);
		}
	}
}

int BinaryStream::readInt(istream& is) {
	read(is);
	return getInt();
}

void BinaryStream::writeInt(ostream& os, int n) {
	setInt(n);
	write(os);
}

double BinaryStream::readDouble(istream& is) {
	read(is);
	return getDouble();
}

void BinaryStream::writeDouble(ostream& os, double r) {
	setDouble(r);
	write(os);
}

bool BinaryStream::isBigEndian() {
	BinaryStream bs;
	bs.setInt(1);
	if (bs.getBuf()[0] > 0) return false;
	else return true;
}