#include<cstdio>
#include<cstdlib>
#include<vector>
#include<iostream>
#include<ctime>
using namespace std;
#define datasize 400
void random(vector<int> &);
vector<vector<float>>list;
int main(){
	float a,b,c,d,e;
	srand(time(NULL));
	FILE *f ;
	if((f = fopen("ttt.txt","r"))!=NULL){
		while(!feof(f)){
			fscanf(f,"%f %f %f %f %f",&a,&b,&c,&d,&e);
			vector<float> v;
			v.resize(6);
			v[0]=1;v[1]=a;v[2]=b;v[3]=c;v[4]=d;v[5]=e;
			list.push_back(v);
			int i = list.size()-1;
		}	
	}
	int min;
	int count = 0;
	vector<int> order;
	order.resize(datasize);
	for(int k=0;k<2000;k++){
		float weight[5]={0};
		while(1){
			int wrong = 0;
			count++;
			random(order);
			for(int i=0;i<list.size();++i){
				float sum=0;
				int ii = order[i];
				for(int j=0;j<5;++j)
					sum+= weight[j]*list[ii][j];
				if((sum>0 && list[ii][5]==1)||(sum<=0 && list[ii][5]==-1))
					continue;
				else{
					for(int j=0;j<5;++j)
						weight[j]+= list[ii][5]*list[ii][j];
						wrong++;
						count++;
				}
			}
			if(wrong==0)
				break;
			else{
				// for(int j=0;j<4;++j)
				// 	cout<<weight[j]<<" ";
				// cout<<wrong<<endl;
			}
		}
	}
	cout<<count/2000;
}
void random(vector<int> &a){
	bool is[datasize]={false};
	int count=0,index=rand()%datasize;
	while(count!=datasize){
		while(is[index])
			index = rand()%datasize;
		a[count++]=index;
		is[index]=true;
	}
}