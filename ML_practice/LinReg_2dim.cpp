#include <iostream>
#include <cstdio>
#include "Matrix.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
#define Xmax 1
#define Xmin -1
#define Ymax 1
#define Ymin -1
#define N 1000
void print(const Vector<double>& t){
	for(int i=0;i<t.size();++i)
		printf("%lf ",t[i]);
	printf("\n");

}
void print(const Matrix<double>& t){
	for (int i = 0; i < t.row(); ++i){
		for(int k=0;k<t.col();++k){
			printf("%lf ",t[i][k]);
		}
		printf("\n");
	}
	printf("\n");
}
int sgn(double a){
	if(a>0)
		return 1;
	else
		return -1;
}
void setData(Matrix<double>& t){
	for(int i=0;i<N;++i){
		t[i][0]=1;
		t[i][1]=(double)rand()/RAND_MAX*(Xmax-Xmin)-(Xmax-Xmin)/2;
		t[i][2]=(double)rand()/RAND_MAX*(Ymax-Ymin)-(Ymax-Ymin)/2;
	}
}
void setAns(const Matrix<double>& t,Matrix<double>& y){
	for(int i=0;i<1000;++i){
		double a = t[i][1],b=t[i][2];
		double ans = a*a+b*b-0.6;
		if(((double)rand()/RAND_MAX) < 0.1){
			y[i][0]=-sgn(ans);
		}
		else
			y[i][0]=sgn(ans);
	}
}
void Linear(const Matrix<double>& t, const Matrix<double>& y, Matrix<double>& weight){
	weight = t.pinv()*y;
}
double test(const Matrix<double>& t, const Matrix<double>& y,const Matrix<double>& weight){
	double count=0;
	Matrix<double> ans(N,1);
	ans = t*weight;
	for(int i=0;i<N;++i){
		if(sgn(ans[i][0]) != y[i][0])
			++count;
	}
	return count/1000;
}
int main(){
	srand(time(NULL));

	Matrix<double> Data(N,3);
	Matrix<double> Y(N,1);
	Matrix<double> weight(3,1);
	double count=0;
	for(int time=0;time<N;++time){
		setData(Data);
		setAns(Data,Y);
		Linear(Data,Y,weight);
		count += test(Data,Y,weight);
		// print(Data.pinv());
	}
	// print(Data);
	printf("%lf",count/1000);
	// print(weight);
}