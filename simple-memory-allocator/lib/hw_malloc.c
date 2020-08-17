#include <stdlib.h>
#include <stdio.h>
#include "hw_malloc.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

void merge(int t)
{
    chunk_ptr_t h = Bin[t];
    while(h!=Bin[t]->prev) {
        if(h->info.alloc ==0 && h->next->info.alloc==0 ) {
            h->info.cur_size = two[t+1];
            break;
        }
        h = h->next;
    }
    if(h == Bin[t]->prev)return;
    del(&Bin[t],h);
    del(&Bin[t],h->next);
    if(t != 10) {
        add(&Bin[t+1],h);
        sortB(&Bin[t+1]);
        merge(t+1);
    }
}
void * hw_malloc(size_t bytes)
{
    // fflush(stdout);
    if((bytes+24)>32768) {
        chunk_ptr_t mem = mmap(NULL,bytes+24,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,0,0);
        if(mem == (void *)-1)
            printf("fu\n");
        else {
            printf("%p\n",mem+1);
            mem->info.cur_size = bytes+24;
            mem->info.mmap = 1;
            mem->info.alloc = 0;
            add(&mlist,mem);
            //printf("mlist = %p\n",mlist);
            return mem;
        }

    } else {
        size_t t = bytes+24;
        if(t<=32)t = 0;
        else if(t<=64) t = 1;
        else if(t<=128)t = 2;
        else if(t<=256)t = 3;
        else if(t<=512)t = 4;
        else if(t<=1024)t = 5;
        else if(t<=2048)t = 6;
        else if(t<=4096)t = 7;
        else if(t<=8192)t = 8;
        else if(t<=16384)t = 9;
        else if(t<=32768)t = 10;
        if(Bin[t]==NULL) {
            split(t);//create a chunk in Bin[t]
        }
        chunk_ptr_t p =Bin[t];
        if(p->info.alloc==0) {
            p->info.alloc = 1;
            printf("0x%.12lx\n",(void *)p - start_sbrk+24 );
            return p;
        }
        p = p->next;
        while(p->info.alloc==1 && p!= Bin[t])
            p = p->next;
        if(p->info.alloc==0) {
            p->info.alloc = 1;
            printf("0x%.12lx\n",(void *)p - start_sbrk+24 );
            return p;
        } else if(p == Bin[t])
            split(t);
        p = p->prev->prev;
        if(p->info.alloc==0) {
            p->info.alloc = 1;
            printf("0x%.12lx\n",(void *)p - start_sbrk+24 );
        }
    }
    return NULL;

}
int hw_free(chunk_ptr_t mem)
{
    // if(mem->info.mmap == 1){
    // 	if(munmap(mem,mem->info.cur_size)==0){
    // 		printf("memory free OK\n");
    // 		return 1;
    // 	}
    // }
    // else if(mem->info.alloc == 1){

    // }
    chunk_ptr_t p = mem-1;
    // printf("free = %p\n",p);
    if(p->info.alloc==1) {
        p->info.alloc = 0;
        int a = p->info.cur_size;
        int t;
        switch(a) {
        case 32:
            t = 0;
            break;
        case 64:
            t = 1;
            break;
        case 128:
            t = 2;
            break;
        case 256:
            t = 3;
            break;
        case 512:
            t = 4;
            break;
        case 1024:
            t = 5;
            break;
        case 2048:
            t = 6;
            break;
        case 4096:
            t = 7;
            break;
        case 8192:
            t = 8;
            break;
        case 16384:
            t = 9;
            break;
        case 32768:
            t = 10;
            break;
        }
        merge(t);
        return 1;
    }
    return 0;
}

void *get_start_sbrk(void)
{
    return start_sbrk;
}