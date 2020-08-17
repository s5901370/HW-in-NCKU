#include "lib/hw_malloc.h"
#include "hw4_mm_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define THRESH 32768

int char_to_int(char* t)
{
    int a=0;
    int i;
    for( i=0; i<strlen(t); ++i) {
        int star=1,j;
        for(j=strlen(t)-1; j>i; --j)
            star*=10;
        a+=star*(t[i]-'0');
    }
    return a;
}
void sortM(chunk_ptr_t *h)
{
    chunk_ptr_t head = *h;
    head = head->prev;//last element
    // int count = 1;
    // while(head != *h){
    // 	head = head->prev;
    // 	count++;
    // }
    // printf("count = %d\n",count);
    // head = *h;
    // printf("head = %p\n",head);
    // head = head->prev;

    while(head->info.cur_size < head->prev->info.cur_size) {
        if(head->info.cur_size < (*h)->info.cur_size) {
            *h = head;
            break;
        }

        // printf("head = %p\n",head);
        // printf("head->prev->prev = %p\n",head->prev->prev);
        // printf("head->prev->next = %p\n",head->prev->next);
        // printf("head->next = %p\n",head->next);

        chunk_ptr_t temp = head->prev->prev;
        head->prev->prev = head;
        head->prev->next = head->next;
        head->next = head->prev;
        head->prev = temp;
        head->prev->next = head;
        head->next->next->prev = head->next;
    }
}
void add(chunk_ptr_t* head,chunk_ptr_t node)
{
    if(*head == NULL) {
        // printf("first add\n");

        *head = node;
        node->prev = node;
        node->next = node;
        // printf("%p\n",mlist);
    } else {
        chunk_ptr_t current = *head;
        current = current->next;
        while(current->next != *head)
            current = current->next;
        node->next = current->next;
        node->prev = current;
        current->next = node;
        node->next->prev = node;
    }
    // chunk_ptr_t cur = *head;
    // while(cur->next != mlist){
    // 	printf("%p--------%d\n",cur,cur->info.cur_size);
    // 	cur = cur->next;
    // }
    // printf("%p--------%d\n",cur,cur->info.cur_size);

    sortM(head);
}
void del(chunk_ptr_t *head,chunk_ptr_t node)
{
    if(node == *head) { //delete head
        if(node->next==node)
            *head = NULL;
        else {
            *head = node->next;
            // printf("head = %p\n",*head);
            (*head)->prev = node->prev;
            node->prev->next = *head;
        }
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
}

void sortB(chunk_ptr_t * h)
{
    chunk_ptr_t head = *h;
    head = head->prev;//last element

    while((long unsigned)head < (long unsigned)head->prev) {
        if((long unsigned)head <(long unsigned)(*h)) {
            *h = head;
            break;
        }
        chunk_ptr_t temp = head->prev->prev;
        head->prev->prev = head;
        head->prev->next = head->next;
        head->next = head->prev;
        head->prev = temp;
        head->prev->next = head;
        head->next->next->prev = head->next;
    }
}
void split(size_t t)
{
    if(t == 10) {
        if(Bin[10]==NULL) {
            chunk_ptr_t p = start_sbrk;
            p->info.cur_size = two[10];
            p->info.alloc = 0;
            p->info.mmap = 0;
            // printf("%p\n",p);
            add(&Bin[10],p);
            chunk_ptr_t p1 = start_sbrk + THRESH;
            // printf("%p\n",p1);
            p1->info.cur_size = two[10];
            p1->info.alloc = 0;
            p1->info.mmap = 0;
            add(&Bin[10],p1);
            return;
        }
    } else if(Bin[t+1]==NULL)
        split(t+1);
    chunk_ptr_t p = Bin[t+1];
    // printf("t = %d %p\n",t,p);
    if(p->info.alloc==0) {
        del(&Bin[t+1],p);
        p->info.cur_size = two[t];
        p->info.alloc = 0;
        p->info.mmap = 0;
        add(&Bin[t],p);
        chunk_ptr_t p1 = (void*)p + two[t];
        p1->info.cur_size = two[t];
        p1->info.alloc = 0;
        p1->info.mmap = 0;
        add(&Bin[t],p1);
        return;
    } else
        p = p->next;
    while(p->info.alloc==1 && p!= Bin[t+1])
        p = p->next;
    if(p->info.alloc==0) {
        del(&Bin[t+1],p);
        p->info.cur_size = two[t];//do the same thing
        p->info.alloc = 0;
        p->info.mmap = 0;
        add(&Bin[t],p);
        chunk_ptr_t p1 = (void*)p + two[t];
        p1->info.cur_size = two[t];
        p1->info.alloc = 0;
        p1->info.mmap = 0;
        add(&Bin[t],p1);
        return;
    } else if(p == Bin[t+1])
        split(t+1);
    p = p->prev->prev;
    if(p->info.alloc==0) {
        del(&Bin[t+1],p);
        p->info.cur_size = two[t];//do the same thing
        p->info.alloc = 0;
        p->info.mmap = 0;
        add(&Bin[t],p);
        chunk_ptr_t p1 = (void*)p + two[t];
        p1->info.cur_size = two[t];
        p1->info.alloc = 0;
        p1->info.mmap = 0;
        add(&Bin[t],p1);
    }
    return;
}

int main(int argc, char *argv[])
{
    two[0]=32;
    two[1]=64;
    two[2]=128;
    two[3]=256;
    two[4]=512;
    two[5]=1024;
    two[6]=2048;
    two[7]=4096;
    two[8]=8192;
    two[9]=16384;
    two[10]=32768;
    two[11]=65536;
    int l;
    mlist = NULL;
    for(l=0; l<11; ++l) {
        Bin[l]=NULL;
    }
    if((start_sbrk = sbrk(65536)) == (void *)-1)
        printf("error init sbrk\n");
    char input[30];
    memset(input,0,sizeof(input));
    while(fgets(input,sizeof(input),stdin)!=NULL) {
        if(strstr(input,"print")!= NULL) {
            if(strstr(input,"bin")!= NULL) {
                char *p1 =strchr(input,'[');
                char *p2 = strchr(input,']');
                char p3[3];
                memset(p3,0,sizeof(p3));
                strncpy(p3,p1+1,p2-p1-1);
                int i = char_to_int(p3);
                // printf("bin %d\n",i);
                chunk_ptr_t cur = Bin[i];
                if(cur!=NULL) {
                    while(cur->next != Bin[i]) {
                        printf("%p--------%d\n",cur,cur->info.cur_size);
                        cur = cur->next;
                    }
                    printf("%p--------%d\n",cur,cur->info.cur_size);
                }
            } else if(strstr(input,"mmap")!= NULL) { //mmap_alloc_list
                chunk_ptr_t cur = mlist;
                // printf("cur = %p\n",cur);
                if(cur!=NULL) {
                    while(cur->next != mlist) {
                        printf("%p--------%d\n",cur,cur->info.cur_size);
                        cur = cur->next;
                    }
                    printf("%p--------%d\n",cur,cur->info.cur_size);
                }
            }

        } else if(strstr(input,"alloc")!= NULL) {
            char N[10];
            memset(N,0,sizeof(N));
            strncpy(N,input+6,strlen(input)-7);
            int n = char_to_int(N);
            //struct chunk *p
            hw_malloc(n);
            //mmap(NULL,n+24,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,0,0);
            // printf("n=%d\n",n);
            // mlist = p;

        } else if(strstr(input,"free")!= NULL) {
            long unsigned p = strtol(input+5,NULL,16);
            // printf("%p\n",p+start_sbrk);
            if(hw_free(p+start_sbrk)==1)
                printf("success\n");
            else
                printf("fail\n");
            // printf("%d\n",p->info.cur_size);
        } else
            printf("error usage\n");
    }

    return 0;
}
