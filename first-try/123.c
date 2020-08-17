#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int n;
typedef struct pile{
	int data;
	struct pile *next;
}Pile;
Pile *pile[30];
void find(int a,Pile *ptr,Pile *p1){
	int i;
	for(i=0;i<n;i++){
		ptr=pile[i];
		p1=pile[i];
		while(ptr->next!=NULL){
			if(ptr->data==a)
				return;
			ptr=ptr->next;	
		}
	}
}
void clear_block(Pile *ptr){
	while(ptr->next!=NULL){
		pile[ptr->data]=ptr;
		ptr=ptr->next;
	}
	pile[ptr->data]=ptr;
}
void move_block(Pile *p1,Pile *p2,Pile *h1,Pile *h2){
	if(h2==p2)return;
	while(h1!=NULL)
		h1=h1->next;
	h1->next=p1;	
}
int main(){
	int a,b,i,j;
	char s1[6],s2[6];
	for(i=0;i<n;i++){
		Pile *p=(Pile*)malloc(sizeof(Pile));
		pile[i]=p;
		p->data=i;
	}
	Pile *p1,*p2,*h1,*h2;
	while(~scanf("%d",&n)){
		Pile *ptr=pile[0];
		for(i=0;i<n;i++){
			while(ptr->next!=NULL){
				pile[ptr->data]=ptr;
				ptr=ptr->next;
			}
			pile[ptr->data]=ptr;
		}
		
		while(~scanf("%s",s1)){
			if(strcmp(s1,"quit")==0)
				break;
			scanf("%d %s %d",&a,s2,&b);
			if(a==b)
				continue;
			find(a,p1,p2);
			find(b,h1,h2);
			if(strcmp(s1,"move")==0)
				clear_block(p1);
			if(strcmp(s2,"onto")==0)
				clear_block(h1);
			move_block(p1,p2,h1,h2);		
		}
		for(i=0;i<n;i++){
			printf("%d:",i);
			Pile *ptr2=pile[i];
			while(ptr->next!=NULL){
				printf(" %d",ptr2->data);
				ptr2=ptr2->next;
			}
			printf(" %d\n",ptr2->data);
		}
	}
} 
