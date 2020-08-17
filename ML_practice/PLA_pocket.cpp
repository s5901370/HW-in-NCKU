#include<cstdio>
#include<cstdlib>
#include<vector>
#include<iostream>
#include<ctime>
using namespace std;
#define datasize 500
void random(vector<int> &);
vector<vector<float>>list,test;
bool p_test(float a[5],float b[5]){// a is w_pocket
	int wrongA=0,wrongB=0;
	// printf("%f %f %f %f %f\n",b[0],b[1],b[2],b[3],b[4]);
	// printf("%f %f %f %f %f\n",a[0],a[1],a[2],a[3],a[4]);

	for(int i=0;i<datasize;++i){
		float sumA=0,sumB=0;
		for(int j=0;j<5;++j){
			sumA+= a[j]*list[i][j];
			sumB+= b[j]*list[i][j];
		}
		if((sumA>0 && list[i][5]==1)||(sumA<=0 && list[i][5]==-1));
		else
			wrongA++;
		if((sumB>0 && list[i][5]==1)||(sumB<=0 && list[i][5]==-1));
		else
			wrongB++;
	}
	if(wrongA>wrongB){
		// printf("wrongA = %d wrongB = %d \n",wrongA,wrongB);
		return true;
	}
	else{
		return false;
	}
} 
int main(){
	float a,b,c,d,e;
	srand(time(NULL));
	FILE *f ;
	if((f = fopen("raw.txt","r"))!=NULL){
		while(!feof(f)){
			fscanf(f,"%f %f %f %f %f",&a,&b,&c,&d,&e);
			vector<float> v;
			v.resize(6);
			v[0]=1;v[1]=a;v[2]=b;v[3]=c;v[4]=d;v[5]=e;
			list.push_back(v);
		}	
	}
	FILE *fp ;
	if((fp = fopen("test.txt","r"))!=NULL){
		while(!feof(fp)){
			fscanf(fp,"%f %f %f %f %f",&a,&b,&c,&d,&e);
			vector<float> v;
			v.resize(6);
			v[0]=1;v[1]=a;v[2]=b;v[3]=c;v[4]=d;v[5]=e;
			test.push_back(v);
		}	
	}
	int count = 0;
	vector<int> order;
	order.resize(datasize);
	float pocket[5]={0};//pocket[0]=1;
	float error = 0;
	for(int k=0;k<2000;k++){
		float weight[5]={0};
		for(int u=0;u<50;++u){
			random(order);
			for(int i=0;i<list.size();++i){
				float sum=0;
				int ii = order[i];
				for(int j=0;j<5;++j)
					sum+= weight[j]*list[ii][j];
				if((sum>0 && list[ii][5]==1)||(sum<=0 && list[ii][5]==-1))
					continue;
				else{
					for(int j=0;j<5;++j){
						// printf("%f ",list[ii][5]*list[ii][j]);
						weight[j]+= list[ii][5]*list[ii][j];
					}
					// printf("ii = %d %f %f %f %f %f\n",ii,weight[0],weight[1],weight[2],weight[3],weight[4]);

					
				}
			}
			if(p_test(pocket,weight)){
				for(int j=0;j<5;++j){
					pocket[j]=weight[j];
				}
			}
			// printf("%f %f %f %f %f\n",pocket[0],pocket[1],pocket[2],pocket[3],pocket[4]);
			// printf("%f %f %f %f %f\n",weight[0],weight[1],weight[2],weight[3],weight[4]);

		}
		float wrong = 0;	
		for(int i=0;i<list.size();++i){
				float sum=0;
				for(int j=0;j<5;++j)
					sum+= pocket[j]*test[i][j];
				if((sum>0 && test[i][5]==1)||(sum<=0 && test[i][5]==-1));
				else
					wrong++;
		}
		error+=wrong/500;
		// cout<<"k = "<<k<<" "<<error<<" ";
	}
	cout<<error/2000;
}
void random(vector<int> &a){
	bool is[datasize]={false};
	int count=0,index=rand()%datasize;
	while(count!=datasize){
		while(is[index])
			index = rand()%datasize;
		a[count++]=index;
	}
}