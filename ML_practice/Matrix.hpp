#ifndef Matrix_h
#define Matrix_h

#include "Vector.hpp"
#include <iostream>
using namespace std;

template<class T>
class Matrix {
	template<class S>
	friend Matrix<S> operator+(const Matrix<S>& a, const Matrix<S>& b);
	template<class S>
	friend Matrix<S> operator-(const Matrix<S>& a, const Matrix<S>& b);
	template<class S>
	friend Matrix<S> operator*(const Matrix<S>& a, S b);
	template<class S>
	friend Matrix<S> operator*(S a, const Matrix<S>& b);
	template<class S>
	friend Matrix<S> operator*(const Matrix<S>& a, const Matrix<S>& b);
	template<class S>
	friend Vector<S> operator*(const Matrix<S>& m, const Vector<S>& v);
	template<class S>
	friend Vector<S> operator*(const Vector<S>& v, const Matrix<S>& m);
	template<class S>
	friend Matrix<S> operator/(const Matrix<S>& a, S b);
	template<class S>
	friend Matrix<S> operator/(S k, const Matrix<S>& m);
	template<class S>
	friend Matrix<S> operator/(const Matrix<S>& a, const Matrix<S>& b);
	template<class S>
	friend bool operator==(const Matrix<S>& a, const Matrix<S>& b);
	template<class S>
	friend bool operator!=(const Matrix<S>& a, const Matrix<S>& b) { return !(a == b); }

	protected:
		int Row;
		int Col;
		T** M;

	public:
		Matrix<T>(): Row(0), Col(0) {}
		Matrix<T>(int row, int col) {
			create(row, col);
			for (int i = 0; i < Row; i++)
				for (int j = 0; j < Col; j++)
					M[i][j] = T(0);
		}
		Matrix<T>(const vector<vector<T>>& m) {
			create(m.size(), (m.size()? m[0].size(): 0));
			for (int i = 0; i < Row; i++)
				for (int j = 0; j < Col; j++)
					M[i][j] = m[i][j];
		}
		Matrix<T>(const Vector<T>& v, bool transpose = false) {
			if (transpose) {
				create(1, v.size());
				for (int j = 0; j < Col; j++)
					M[0][j] = v[j];
			} else {
				create(v.size(), 1);
				for (int i = 0; i < Row; i++)
					M[i][0] = v[i];
			}
		}
		Matrix<T>(const vector<T>& v, bool transpose = false) {
			if (transpose) {
				create(1, v.size());
				for (int j = 0; j < Col; j++)
					M[0][j] = v[j];
			} else {
				create(v.size(), 1);
				for (int i = 0; i < Row; i++)
					M[i][0] = v[i];
			}
		}
		Matrix<T>(const vector<Vector<T>>& m, bool transpose = false) {
			if (transpose) {
				create(m.size(), (m.size()? m[0].size(): 0));
				for (int i = 0; i < Row; i++)
					for (int j = 0; j < Col; j++)
						M[i][j] = m[i][j];
			} else {
				create((m.size()? m[0].size(): 0), m.size());
				for (int i = 0; i < Row; i++)
					for (int j = 0; j < Col; j++)
						M[i][j] = m[j][i];
			}
		}
		~Matrix<T>() { 
			if (M && M[0]) delete [] M[0];
			if (M) delete [] M;
		}
		Matrix<T>(const Matrix<T>& m) {
			create(m.Row, m.Col);
			for (int i = 0; i < Row; i++)
				for (int j = 0; j < Col; j++)
					M[i][j] = m[i][j];
		}
		Matrix<T>& operator=(const Matrix<T>& m) {
			if (M && M[0]) delete [] M[0];
			if (M) delete [] M;
			create(m.Row, m.Col);
			for (int i = 0; i < Row; i++)
				for (int j = 0; j < Col; j++)
					M[i][j] = m[i][j];
		}
		Matrix<T>& operator=(const vector<vector<T>>& m) {
			if (M && M[0]) delete [] M[0];
			if (M) delete [] M;
			create(m.size(), (m.size()? m[0].size(): 0));
			for (int i = 0; i < Row; i++)
				for (int j = 0; j < Col; j++)
					M[i][j] = m[i][j];
		}

		void create(int Row, int Col) {
			this->Row = Row;
			this->Col = Col;
			M = new T*[Row];
			M[0] = new T[Row * Col];
			for (int i = 1; i < Row; i++) M[i] = M[i-1] + Col;
		}

		Matrix<T> operator-() const { Matrix<T> Zero(Row, Col); return Zero - *this; }

		template<class S>
		Matrix<S>& operator+=(S b) { *this = *this + b; return *this; }
		template<class S>
		Matrix<S>& operator-=(S b) { *this = *this - b; return *this; }
		template<class S>
		Matrix<S>& operator*=(S b) { *this = *this * b; return *this; }
		template<class S>
		Matrix<S>& operator/=(S b) { *this = *this / b; return *this; }

		T* operator[](int i) { if (i >= Row) throw "index overflow"; return M[i]; }
		T* operator[](int i) const { if (i >= Row) throw "index overflow"; return M[i]; }
		Vector<T> operator()(int i, bool transpose = false) const {
			if (transpose) {
				if (i >= Row) throw "index overflow";
				Vector<T> v(Col);
				for (int j = 0; j < Col; j++) v[j] = M[i][j];
				return v;
			} else {
				if (i >= Col) throw "index overflow";
				Vector<T> v(Row);
				for (int j = 0; j < Col; j++) v[j] = M[j][i];
				return v;
			}
		}
		int row() const { return Row; }
		int col() const { return Col; }

		string toString() const {
			if (Row == 0 || Col == 0) return "[0]";
			string s = "[\n";
			for (int i = 0; i < Row; i++) {
				for (int j = 0; j < Col; j++) {
					if (j != 0) s += ", ";
					else s += " ";
					stringstream ss;
					ss << M[i][j];
					string ts;
					ss >> ts;
					s += ts;
				}
				s += ";\n";
			}
			s += "]";
			return s;
		}
		friend ostream& operator<<(ostream& output, const Matrix<T>& a) { output << a.toString(); return output; }

		Matrix<T> trans() const {
			Matrix<T> t(Col, Row);
			for (int i = 0; i < Row; i++)
			 	for (int j = 0; j < Col; j++)
					t[j][i] = M[i][j];
			return t;
		}
		T cofactor(int i, int j) const {
			if (Row != Col || Row < 1) throw "cannot find cofactor";

			bool s = ((i + j) % 2 == 0)? true: false;
			Matrix<T> m(Row - 1, Col - 1);
			int a = 0;
			for (int p = 0; p < Row; p++) {
				if (p != i) {
					int b = 0;
				 	for (int q = 0; q < Col; q++)
						if (q != j) m[a][b++] = M[p][q];
					a++;
				}
			}
			if(s) return m.det();
			else return -(m.det());
		}
		T det() const {
			if (Row != Col) throw "cannot calculate determinant";

			if(Row == 1) return M[0][0];
			T d = T(0);
			for (int j = 0; j < Col; j++) d += M[0][j] * cofactor(0, j);
			return d;
		}
		Matrix<T> inv() const {
			if (Row != Col) throw "the matrix is invertible";

			Matrix<T> Inv(Row, Col);
			for (int i = 0; i < Row; i++)
			 	for (int j = 0; j < Col; j++)
					Inv[j][i] = cofactor(i, j);
			return Inv / det();
		}
		template<class S>
		Vector<S> chol(Vector<S> b) {
			if (Row != Col) throw "the matrix is not square matrix";
			T** L = new T*[Row];
			for (int n = 0; n < Row; n++) L[n] = new T[n+1];
			// Cholesky decomposition
			for (int i = 0; i < Row; i++) {
				for (int j = 0; j < i; j++) {
					L[i][j] = M[i][j];
					for (int k = 0; k < j; k++) L[i][j] -= L[i][k] * L[j][k] * L[k][k];
					L[i][j] /= L[j][j];
				}
				L[i][i] = M[i][i];
				for (int k = 0; k < i; k++) L[i][i] -= L[i][k] * L[i][k] * L[k][k];
			}
			// forward substitution
			T* y = new T[Row];
			for (int i = 0; i < Row; i++) {
				y[i] = b[i];
				for (int j = 0; j < i; j++) {
					y[i] -= L[i][j] * y[j];
				}
			}
			// backward substitution
			Vector<S> x(Row);
			for (int i = Row - 1; i >= 0; i--) {
				x[i] = y[i] / L[i][i];
				for (int j = Row - 1; j > i; j--) {
					x[i] -= L[j][i] * x[j];
				}
			}
			for (int n = 0; n < Row; n++) delete [] L[n];
			delete [] L;
			delete [] y;
			return x;
		}
		Matrix<T> pinv() const {
			Matrix<T> t = trans();
			return (t * (*this)).inv() * t;
		}
		static Matrix<T> I(int size) {
			Matrix<T> Z(size, size);
			for (int i = 0; i < size; i++) Z[i][i] = 1;
			return Z;
		};
		void appendRow(const vector<T>& v){
			if(Col != v.size()) throw "different Size of Col";
			T temp[Row][Col];
			for(int i=0;i<Row;++i)
				for(int j=0;j<Col;++j){
					temp[i][j] = M[i][j];
				}
			if (M && M[0]) delete [] M[0];
			if (M) delete [] M;
			create(Row+1,Col);
			for(int i=0;i<Row-1;++i)
				for(int j=0;j<Col;++j){
					M[i][j] = temp[i][j];
				}
			for(int j=0;j<Col;++j)
				M[Row-1][j] = v[j];
		}
		void appendRow(const Vector<T>& v){
			if(Col != v.size()) throw "different Size of Col";
			T temp[Row][Col];
			for(int i=0;i<Row;++i)
				for(int j=0;j<Col;++j){
					temp[i][j] = M[i][j];
				}
			if (M && M[0]) delete [] M[0];
			if (M) delete [] M;
			create(Row+1,Col);
			for(int i=0;i<Row-1;++i)
				for(int j=0;j<Col;++j){
					M[i][j] = temp[i][j];
				}
			for(int j=0;j<Col;++j)
				M[Row-1][j] = v[j];
		}
		void appendCol(const vector<T>& v){
			if(Row != v.size()) throw "different Size of Col";
			T temp[Row][Col];
			for(int i=0;i<Row;++i)
				for(int j=0;j<Col;++j){
					temp[i][j] = M[i][j];
				}
			if (M && M[0]) delete [] M[0];
			if (M) delete [] M;
			create(Row,Col+1);
			for(int i=0;i<Row;++i)
				for(int j=0;j<Col-1;++j){
					M[i][j] = temp[i][j];
				}
			for(int j=0;j<Row;++j)
				M[j][Col-1] = v[j];
		}
		void appendCol(const Vector<T>& v){
			if(Row != v.size()) throw "different Size of Col";
			T temp[Row][Col];
			for(int i=0;i<Row;++i)
				for(int j=0;j<Col;++j){
					temp[i][j] = M[i][j];
				}
			if (M && M[0]) delete [] M[0];
			if (M) delete [] M;
			create(Row,Col+1);
			for(int i=0;i<Row;++i)
				for(int j=0;j<Col-1;++j){
					M[i][j] = temp[i][j];
				}
			for(int j=0;j<Row;++j)
				M[j][Col-1] = v[j];
		}
};

//define Matrix functions
template<class S>
Matrix<S> operator+(const Matrix<S>& a, const Matrix<S>& b) {
	if(a.Row != b.Row || a.Col != b.Col) throw "different Size of two vectors";

	Matrix<S> c(a.Row, a.Col);
	for (int i = 0; i < a.Row; i++)
	 	for (int j = 0; j < a.Col; j++)
			c[i][j] = a[i][j] + b[i][j];
	return c;
}
template<class S>
Matrix<S> operator-(const Matrix<S>& a, const Matrix<S>& b) {
	if(a.Row != b.Row || a.Col != b.Col) throw "different Size of two vectors";

	Matrix<S> c(a.Row, a.Col);
	for (int i = 0; i < a.Row; i++)
	 	for (int j = 0; j < a.Col; j++)
			c[i][j] = a[i][j] - b[i][j];
	return c;
}
template<class S>
Matrix<S> operator*(S a, const Matrix<S>& b) {
	//Scalar multiplication
	Matrix<S> c(b.Row, b.Col);
	for (int i = 0; i < b.Row; i++)
	 	for (int j = 0; j < b.Col; j++)
			c[i][j] = a * b[i][j];
	return c;
}
template<class S>
Matrix<S> operator*(const Matrix<S>& a, S b) {
	//Scalar multiplication
	Matrix<S> c(a.Row, a.Col);
	for (int i = 0; i < a.Row; i++)
	 	for (int j = 0; j < a.Col; j++)
			c[i][j] = a[i][j] * b;
	return c;
}
template<class S>
Matrix<S> operator*(const Matrix<S>& a, const Matrix<S>& b) {
	//Inner product
	if (a.Col != b.Row) throw "wrong size to multiply";
	Matrix<S> c(a.Row, b.Col);
	for (int i = 0; i < a.Row; i++)
	 	for (int j = 0; j < b.Col; j++)
			for (int k = 0; k < a.Col; k++)
				c[i][j] += a[i][k] * b[k][j];
	return c;
}
template<class S>
Vector<S> operator*(const Matrix<S>& m, const Vector<S>& v) {
	//Inner product
	if (m.Col != v.size()) throw "wrong size to multiply";

	Vector<S> c(m.Row);
	for (int i = 0; i < m.Row; i++)
		for (int k = 0; k < m.Col; k++)
			c[i] += m[i][k] * v[k];
	return c;
}
template<class S>
Vector<S> operator*(const Vector<S>& v, const Matrix<S>& m) {
	//Inner product
	if (v.size() != m.Row) throw "wrong size to multiply";

	Vector<S> c(m.Col);
	for (int j = 0; j < m.Col; j++)
		for (int k = 0; k < m.Row; k++)
			c[j] += v[k] * m[k][j];
	return c;
}
template<class S>
Matrix<S> operator/(const Matrix<S>& a, S b) {
	//Scalar multiplication
	if (b == 0.0) throw "divide by 0";

	Matrix<S> c(a.Row, a.Col);
	for (int i = 0; i < a.Row; i++)
	 	for (int j = 0; j < a.Col; j++)
			c[i][j] = a[i][j] / b;
	return c;
}
template<class S>
Matrix<S> operator/(S k, const Matrix<S>& m) {
	return k * m.inv();
}
template<class S>
Matrix<S> operator/(const Matrix<S>& a, const Matrix<S>& b) {
	return a * b.inv();
}
template<class S>
bool operator==(const Matrix<S>& a, const Matrix<S>& b) {
	if (a.Row != b.Row || a.Col != b.Col) return false;
	for (int i = 0; i < a.Row; i++)
	 	for (int j = 0; j < a.Col; j++)
			if (a[i][j] != b[i][j]) return false;
	return true;
}

#endif
