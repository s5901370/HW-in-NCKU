#include <cstdio>
#include <cmath>
#include <vector>
#include "Matrix.hpp"
#define dim 21
#define step 0.001
#define T 2000
using namespace std;
void read(char * filename,vector<vector<double>>& data,vector<double>& y){
	FILE *f ;
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
			y.push_back(t);
			data.push_back(a);
		}	
	}
}
double sigmoid(double a){
	return (1/(1+exp(-a)));
}
double sgn(double a){
	if(a>0)
		return 1;
	else
		return -1;
}
double mul(const vector<double> a,const double b[dim]){
	double ans=0;
	for(int i=0;i<dim;++i)
		ans += a[i]*b[i];
	return ans;
}
void Logistic(const vector<vector<double>>& t,const vector<double>& y ,double* w){
	//Stochastic
	// int size = t.size();
	// for(int i=0;i<T;++i){
	// 	double sig = sigmoid((-1)*y[i%size]*mul(t[i%size],w));
	// 	for(int k=0;k<dim;++k){
	// 		w[k] += step*sig*y[i%size]*t[i%size][k];
	// 	}
	// }

	//Batch
	double temp[dim];
	

	int size = t.size();
	for(int i=0;i<T;++i){
		for(int k=0;k<dim;++k)
			temp[k] = w[k];
		for(int k=0;k<size;++k){
			double sig = sigmoid((-1)*y[k]*mul(t[k],temp));
			for(int j=0;j<dim;++j){
				temp[j] -= sig*y[k]*t[k][j];
			}
		}
		for(int k=0;k<dim;++k)
			w[k] -= step*temp[k]/size;
	}
	

}
double test(const vector<vector<double>>& t,const vector<double>& y,const double* w){
	double count=0;
	int time = t.size();
	for(int i=0;i<time;++i){
		if(sgn(mul(t[i],w)) != y[i])
			++count;
	}
	return count/time;
}


int main(){
	vector<vector<double>> trainData,testData; 
	vector<double> trainY,testY;
	char s[] = "train.txt";
	read(s,trainData,trainY);
	char c[] = "test.txt";
	read(c,testData,testY);
	double weight[dim];
	for(int i=0;i<dim;++i)
		weight[i]=0;
	Logistic(trainData,trainY,weight);
	double ans = test(testData,testY,weight);
	printf("%lf\n",ans);
	for(int i=0;i<dim;++i)
		printf("%lf ",weight[i] );
}