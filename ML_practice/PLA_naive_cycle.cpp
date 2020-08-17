#include<cstdio>
#include<cstdlib>
#include<vector>
#include<iostream>
using namespace std;
vector<vector<float>>list;
int main(){
	float a,b,c,d,e;
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
	float weight[5]={0};
	int min;
	int count = 0;
	while(1){
		int wrong = 0;
		for(int i=0;i<list.size();++i){
			float sum=0;
			for(int j=0;j<5;++j)
				sum+= weight[j]*list[i][j];
			if((sum>0 && list[i][5]==1)||(sum<=0 && list[i][5]==-1))
				continue;
			else{
				for(int j=0;j<5;++j)
					weight[j]+= list[i][5]*list[i][j];
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
	cout<<count;
	// for(int j=0;j<5;++j)
		// cout<<weight[j]<<" ";
}