#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "Matrix.hpp"
using namespace std;
#define dim 3 
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
void read(char * filename,Matrix<double>& data,Vector<double>& y){
	FILE *f ;
	vector<vector<double>> temp;
	vector<double> temp2;
	if((f = fopen(filename,"r"))!=NULL){
		while(!feof(f)){
			double t;
			vector<double> a;
			a.push_back(1);//X0
			for(int i = 1;i < dim;++i){
				fscanf(f,"%lf",&t);//X1~X20
				a.push_back(t);
			}
			fscanf(f,"%lf",&t);
			temp2.push_back(t);
			temp.push_back(a);
		}	
	}
	data = temp;
	y = temp2;
}
int sgn(double a){
	if(a>0)
		return 1;
	else
		return -1;
}
void RegularizedLinReg(const Matrix<double>& t,const Vector<double>& y,Matrix<double>& w,const double& lambda){
	Matrix<double> id(w.row(),w.row());
	for(int i=0;i<w.row();++i)
		id[i][i]=1;
	w = (t.trans()*t+lambda*id).inv()*t.trans()*y;
}
double test(const Matrix<double>& t,const Vector<double>& y,const Matrix<double>& w){
	double count =0;
	Matrix<double> ans(dim) ;
	ans = t*w;
	for(int i=0;i<t.row();++i){
		if(sgn(ans[i][0]) != y[i])
			++count;
	}
	return count/t.row();
}
int min(double a[13]){
	int currnt=0;
	double min = 1;
	for(int i=0; i<=12;++i){
		if(a[i]<=min){
			min = a[i];
			currnt = i;
		}
	}
	return currnt;
}
int main(){
	Matrix<double> trainData(dim),testData(dim); 
	Vector<double> trainY(1),testY(1);
	Matrix<double> weight(dim);
	double lambda[13];
	for(int i=0;i<=12;++i){
		lambda[i] = pow(10.0,i-10); // -10~2
	}
	char s[] = "train.txt";
	read(s,trainData,trainY);
	char c[] = "test.txt";
	read(c,testData,testY);
	// printf("B");
	// print(weight);
	double Ein[13];
	double Eout[13];
	for(int i=0;i<=12;++i){
		RegularizedLinReg(trainData,trainY,weight,lambda[i]);
		// print(weight);
		Ein[i] = test(trainData,trainY,weight);
		Eout[i] = test(testData,testY,weight);
	}
	int in = min(Eout);
	// int out = min(Eout);

	printf("Ein = %lf\n lambda = %d ",Ein[in],in-10);
	printf("Eout = %lf\n",Eout[in]);
}