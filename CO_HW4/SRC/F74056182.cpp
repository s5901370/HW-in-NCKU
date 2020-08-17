#include<iostream>
#include<cstring>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<algorithm>
using namespace std;
unsigned six(int n){
	unsigned ans=1;
	for(int i=0;i<n;++i)
		ans*=16;
	return ans;
}
int two(int n){
	int ans=0;
	while(n!=1){
		n/=2;
		++ans;
	}
	return ans;

}
int ten(int j){
	int a=1;
	for(int i=0;i<j;++i)
		a*=10;
	return a;
}

int stoi(string s){
	int a=0;
	for(int i=0;i<s.size();++i){
		a += ten(s.size()-1-i)*(s[i]-'0');
	}
	return a;
}
unsigned int hex_to(string l){//2~9
	unsigned ans = 0;
	int a;
	for(int i=0;i<8;++i){
		switch(l[9-i]){
			case '0':a = 0;break;
			case '1':a=1;break;
			case '2':a=2;break;
			case '3':a=3;break;
			case '4':a=4;break;
			case '5':a=5;break;
			case '6':a=6;break;
			case '7':a=7;break;
			case '8':a=8;break;
			case '9':a=9;break;
			case 'a':a=10;break;
			case 'b':a=11;break;
			case 'c':a=12;break;
			case 'd':a=13;break;
			case 'e':a=14;break;
			case 'f':a=15;
		}
		ans+=a*six(i);
	}
	return ans;
}

int main(int argv,char** argc){
	int input,output;
	string output_name="",input_name="";
	int o=strlen(argc[2]);
	while(o!=0){
		if(o=='/')
			input = o;
		--o;
	}
	for(int i=o+1;i<strlen(argc[2]);++i)
		input_name+=argc[2][i];
	o = strlen(argc[4]);
	while(o!=0){
		if(o=='/')
			output = o;
		--o;
	}
	for (int i = o+1; i < strlen(argc[4]); ++i)
		output_name+=argc[4][i];

	fstream file;
	file.open(input_name.c_str(),ios::in);
	if(!file){
		cout<<"could not open "+input_name;
		return 0;
	}
	string l;
	getline(file,l);
	int cache = stoi(l);//cache total size
	getline(file,l);
	int block = stoi(l);//address / block
	getline(file,l);
	int ass = stoi(l);
	getline(file,l);
	int rep = stoi(l);
	//cout<<cache<<" "<<block<<" "<<ass<<" "<<rep<<endl;

	int index = 1024*cache/block;//index,block number
	//int set;
	//unsigned tag = 4294967296/index/block;
	long hit=0;
	vector<long> hit_instr,miss_instr;
	long miss=0;
	long count=0;
	
	//cout<<index<<" "<<tag<<" "<<set<<endl; 

	if(rep==0){//FIFO
		if(ass==0){//direct   set=index; max_size=1
			int mem[index]={0};
			while(getline(file,l)){
				++count;
				unsigned int a = hex_to(l)/block;
				int Set = a%index;//index
				int Tag = a/index;//tag
				if(mem[Set]==Tag){//find it 
					hit_instr.push_back(count);
					++hit;
				}
				else{
					miss_instr.push_back(count);
					++miss;
					mem[Set]=Tag;
				}
			}
		}
		else if(ass==1){//4 way  set=index/4;
			int max_size=4;
			index/=max_size;
			vector<int> mem[index];
			while(getline(file,l)){
				++count;
				unsigned int a = hex_to(l)/block;
				int Set = a%index;
				int Tag = a/index;
				vector<int>::iterator it = find(mem[Set].begin(),mem[Set].end(),Tag);
				if(it!=mem[Set].end()){//find it 
					hit_instr.push_back(count);
					++hit;
				}
				else{
					miss_instr.push_back(count);
					++miss;
					if(mem[Set].size()<max_size){
						mem[Set].push_back(Tag);
					}
					else if(mem[Set].size()==max_size){
						for(int i=0;i<mem[Set].size()-1;++i)
							mem[Set][i]=mem[Set][i+1];
						mem[Set][mem[Set].size()-1]=Tag;
					}
				}
				//cout<<a<<" "<<Set<<" "<<Tag<<endl;
			}
		}
		
		else if(ass == 2){//full  set = 1;
			vector<int> mem;
			int max_size = index;//cache can save  mem[set].size()<=man
			while(getline(file,l)){
				++count;
				unsigned int a = hex_to(l)/block;
				//int Set = a%tag;
				//int Tag = a/tag;
				vector<int>::iterator it = find(mem.begin(),mem.end(),a);
				if(it!=mem.end()){//find it 
					hit_instr.push_back(count);
					++hit;
				}
				else{
					miss_instr.push_back(count);
					++miss;
					if(mem.size()<max_size){
						mem.push_back(a);
					}
					else{
						for(int i=0;i<mem.size()-1;++i)
							mem[i]=mem[i+1];
						mem[mem.size()-1]=a;
					}
				}
				//cout<<a<<" "<<Set<<" "<<Tag<<endl;
			}
			
		}

	}
	else if(rep==1){//LRU
		if(ass==0){//the same as rep=0
			int mem[index]={0};
			while(getline(file,l)){
				++count;
				unsigned int a = hex_to(l)/block;
				int Set = a%index;//index
				int Tag = a/index;//tag
				if(mem[Set]==Tag){//find it 
					hit_instr.push_back(count);
					++hit;
				}
				else{
					miss_instr.push_back(count);
					++miss;
					mem[Set]=Tag;
				}
			}
		}
		else if(ass==1){
			int max_size=4;
			index/=max_size;
			vector<int> mem[index];
			while(getline(file,l)){
				++count;
				unsigned int a = hex_to(l)/block;
				int Set = a%index;
				int Tag = a/index;
				vector<int>::iterator it = find(mem[Set].begin(),mem[Set].end(),Tag);
				if(it!=mem[Set].end()){//find it 
					hit_instr.push_back(count);
					++hit;
					int x = it-mem[Set].begin();//get the position of it
					for(int i=x;i<mem[Set].size()-1;++i)
						mem[Set][i]=mem[Set][i+1];
					mem[Set][mem[Set].size()-1]=Tag;
				}
				else{
					miss_instr.push_back(count);
					++miss;
					if(mem[Set].size()<max_size){
						mem[Set].push_back(Tag);
					}
					else if(mem[Set].size()==max_size){
						for(int i=0;i<mem[Set].size()-1;++i)
							mem[Set][i]=mem[Set][i+1];
						mem[Set][mem[Set].size()-1]=Tag;
					}
				}
			}
		}
		else if(ass==2){
			//cout<<"B";    ///////////
			vector<unsigned> mem;
			int max_size = index;//cache can save  mem[set].size()<=man
			while(getline(file,l)){
				++count;
				unsigned int a = hex_to(l)/block;
				//cout<<index;
				// if(count==63){
				// 	for(int i=0;i<mem.size();++i)
				// 		cout<<"i"<<i<<" "<<mem[i]<<endl;
				// }
				//cout<<"count"<<count<<": "<<a<<endl;
				//int Set = a%tag;
				//int Tag = a/tag;

				vector<unsigned>::iterator it = find(mem.begin(),mem.end(),a);
				if(it!=mem.end()){//find it 
					hit_instr.push_back(count);
					++hit;
					int x = it-mem.begin();//get the position of it
					for(int i=x;i<mem.size()-1;++i)
						mem[i]=mem[i+1];
					mem[mem.size()-1]=a;
				}
				else{
					miss_instr.push_back(count);
					++miss;
					if(mem.size()<max_size){
						mem.push_back(a);
					}
					else if(mem.size()==max_size){
						for(int i=0;i<mem.size()-1;++i)
							mem[i]=mem[i+1];
						mem[mem.size()-1]=a;
					}
				}
				//cout<<a<<" "<<Set<<" "<<Tag<<endl;
			}
			//cout<<"A";    /////////
		}
	}
	// while(getline(file,l)){
	// 	unsigned int a = hex_to(l);
	// 	cout<<a/32<<endl;
	// }
	fstream file_out;
	file_out.open(output_name.c_str(),ios::out);
	if(!file_out){
		cout<<"can't write"<<output_name;
	}
	else{
		file_out<<"Hits instructions: ";
		for(int i=0;i<hit_instr.size();++i){
			file_out<<hit_instr[i];
			if(i!=hit_instr.size()-1)
				file_out<<",";
		}
		file_out<<'\n';
		file_out<<"Misses instructions: ";
		for(int i=0;i<miss_instr.size();++i){
			file_out<<miss_instr[i];
			if(i!=miss_instr.size()-1)
				file_out<<",";
		}
		file_out<<'\n';
		file_out<<"Miss rate: "<<(double)miss/(hit+miss);
		file_out<<endl;
	}
	

	file.close();
	file_out.close();
}