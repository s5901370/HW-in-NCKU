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
void partition(const Matrix<double>& t,const Vector<double>& y,Matrix<double>& a,Vector<double>& b
	,Matrix<double>& c,Vector<double>& d,int gap,int times){
	int t1 = gap*times,tt = gap*(times+1);

	for(int i=0;i<t1;++i)
		for(int j=0;j<dim;++j)
			a[i][j] = t[i][j];
	for(int i=t1;i<tt;++i)
		for(int j=0;j<dim;++j)
			c[i-t1][j] = t[i][j];	
	for(int i=0;i<t1;++i)
		b[i] = y[i];
	for(int i=t1;i<tt;++i)
		d[i-t1] = y[i];
	for(int i=tt;i<200;++i)
		for(int j=0;j<dim;++j){
			a[i-gap][j] = t[i][j];
		}
	for(int i=tt;i<200;++i)
		b[i-gap] = y[i];
}
int main(){
	Matrix<double> trainData(dim),testData(dim); 
	Vector<double> trainY(1),testY(1);
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
	double Eval[13]={0};
	for(int i=0;i<=12;++i){
		for(int j=0;j<5;++j){
			Matrix<double> weight(dim);
			Matrix<double> EtrainData(160,dim),EvalData(40,dim);
			Vector<double> EtrainY(160),EvalY(40);
			partition(trainData,trainY,EtrainData,EtrainY,EvalData,EvalY,40,j);
			RegularizedLinReg(EtrainData,EtrainY,weight,lambda[i]);
			// print(weight);
			Eval[i] += test(EvalData,EvalY,weight);
			// Eout[i] = test(testData,testY,weight);
		}
		Eval[i] /= 5;
		// printf("Ecv %d = %lf\n",i-10,Eval[i]);
	}
	int in = min(Eval); // best lambda

	Matrix<double> weight(dim);
	RegularizedLinReg(trainData,trainY,weight,lambda[in]);
	double Ein = test(trainData,trainY,weight);
	double Eout = test(testData,testY,weight);
	printf("Ecv = %lf\n lambda = %d ",Eval[in],in-10);
	printf("Ein = %lf Eout = %lf\n",Ein,Eout);
}