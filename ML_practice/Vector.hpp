#ifndef Vector_h
#define Vector_h

#include <vector>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

template<class T>
class Vector {
	template<class S>
	friend Vector<S> operator+(const Vector<S>& a, const Vector<S>& b);
	template<class S>
	friend Vector<S> operator-(const Vector<S>& a, const Vector<S>& b);
	template<class S>
	friend Vector<S> operator*(const Vector<S>& a, S b);
	template<class S>
	friend Vector<S> operator*(S a, const Vector<S>& b);
	template<class S>
	friend S operator*(const Vector<S>& a, const Vector<S>& b);
	template<class S>
	friend Vector<S> operator/(const Vector<S>& a, S b);
	template<class S>
	friend bool operator==(const Vector<S>& a, const Vector<S>& b);
	template<class S>
	friend bool operator!=(const Vector<S>& a, const Vector<S>& b) { return !(a == b); }

protected:
	T* V;
	int Size;

public:
	Vector<T>(): Size(0) {
		V = new T[0];
	}
	Vector<T>(int Size): Size(Size) {
		V = new T[Size];
		for (int i = 0; i < Size; i++) V[i] = T(0);
	}
	Vector<T>(const vector<T>& v) {
		Size = v.size();
		V = new T[Size];
		for (int i = 0; i < Size; i++) V[i] = v[i];
	}
	~Vector() { if(V) delete [] V; }
	Vector<T>(const Vector<T>& v) {
		Size = v.size();
		V = new T[Size];
		for (int i = 0; i < Size; i++) V[i] = v[i];
	}
	Vector<T>& operator=(const Vector<T>& v) {
		if(V) delete [] V;
		Size = v.size();
		V = new T[Size];
		for (int i = 0; i < Size; i++) V[i] = v[i];
		return *this;
	}
	Vector<T>& operator=(const vector<T>& v) {
		// if(V) delete [] V;
		Size = v.size();
		V = new T[Size];
		for (int i = 0; i < Size; i++) V[i] = v[i];
		return *this;
	}

	Vector<T> operator-() const { Vector<T> Zero(size()); return Zero - *this; }

	template<class S>
	Vector<T>& operator+=(S b) { *this = *this + b; return *this; }
	template<class S>
	Vector<T>& operator-=(S b) { *this = *this - b; return *this; }
	template<class S>
	Vector<T>& operator*=(S b) { *this = *this * b; return *this; }
	template<class S>
	Vector<T>& operator/=(S b) { *this = *this / b; return *this; }

	T& operator[](int i) { if (i >= size()) throw "index overflow"; return V[i]; }
	T operator[](int i) const { if (i >= size()) throw "index overflow"; return V[i]; }
	inline int size() const { return Size; }

	string toString() const {
		if (size() == 0) return "(0)";
		string s = "(";
		for (int i = 0; i < size(); i++) {
			if (i != 0) s += ", ";
			stringstream ss;
			ss << V[i];
			string ts;
			ss >> ts;
			s += ts;
		}
		s += ")";
		return s;
	}
	friend ostream& operator<<(ostream& output, const Vector<T>& a) { output << a.toString(); return output; }

	T norm() {
		return T(sqrt((double)normsqr()));
	}
	T normsqr() {
		T n(0);
		for(int i = 0; i < size(); i++) n += V[i] * V[i];
		return n;
	}
};

//define Vector functions
template<class S>
Vector<S> operator+(const Vector<S>& a, const Vector<S>& b) {
	if (a.size() != b.size()) throw "different size() of two Vectors";

	Vector<S> c(a.size());
	for (int i = 0; i < a.size(); i++) c[i] = a[i] + b[i];
	return c;
}
template<class S>
Vector<S> operator-(const Vector<S>& a, const Vector<S>& b) {
	if (a.size() != b.size()) throw "different size() of two Vectors";

	Vector<S> c(a.size());
	for (int i = 0; i < a.size(); i++) c[i] = a[i] - b[i];
	return c;
}
template<class S>
Vector<S> operator*(S a, const Vector<S>& b) {
	//Scalar multiplication
	Vector<S> c(b.size());
	for (int i = 0; i < b.size(); i++) c[i] = a * b[i];
	return c;
}
template<class S>
Vector<S> operator*(const Vector<S>& a, S b) {
	//Scalar multiplication
	Vector<S> c(a.size());
	for (int i = 0; i < a.size(); i++) c[i] = a[i] * b;
	return c;
}
template<class S>
S operator*(const Vector<S>& a, const Vector<S>& b) {
	//Inner product
	if (a.size() != b.size()) throw "different size() of two Vector<S>s";

	S c(0);
	for (int i = 0; i < a.size(); i++) c += a[i] * b[i];
	return c;
}
template<class S>
Vector<S> operator/(const Vector<S>& a, S b) {
	//Scalar multiplication
	if (b == 0.0) throw "divide by 0";

	Vector<S> c(a.size());
	for (int i = 0; i < a.size(); i++) c[i] = a[i] / b;
	return c;
}
template<class S>
bool operator==(const Vector<S>& a, const Vector<S>& b) {
	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); i++) if (a[i] != b[i]) return false;
	return true;
}

#endif
