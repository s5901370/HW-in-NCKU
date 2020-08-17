#ifndef HW_MALLOC_H
#define HW_MALLOC_H

struct chunk_info_t {
    unsigned int prev_size:31;
    unsigned int alloc:1;
    unsigned int cur_size:31;
    unsigned int mmap:1;
};
typedef struct chunk * chunk_ptr_t;
struct chunk {
    struct chunk_info_t info;
    chunk_ptr_t prev;
    chunk_ptr_t next;
};
void* start_sbrk;
void sortB(chunk_ptr_t *);
int two[12];
chunk_ptr_t Bin[11];
chunk_ptr_t mlist;
int hw_free(chunk_ptr_t);
void *get_start_sbrk(void);
void merge(int t);
void del(chunk_ptr_t *,chunk_ptr_t);
void add(chunk_ptr_t*,chunk_ptr_t );
void split(size_t );
void * hw_malloc(size_t );
#endif
