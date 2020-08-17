/*
 * Copyright (c) 2016, Mathias Brossard <mathias@brossard.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "server.h"
#include "status.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <dirent.h>
typedef enum {
    immediate_shutdown = 1,
    graceful_shutdown  = 2
} threadpool_shutdown_t;

typedef struct {
    void (*function)(void *);
    void *argument;
} threadpool_task_t;

struct threadpool_t {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    threadpool_task_t *queue;
    int thread_count;
    int queue_size;
    int head;
    int tail;
    int count;
    int shutdown;
    int started;
};

static void *threadpool_thread(void *threadpool);

int threadpool_free(threadpool_t *pool);

pthread_mutex_t lock;

int sockfd = 0,forClientSockfd = 0;
char abs_path[100];
void find(void *arg)
{
    //printf("success in thread\n");
    //printf("A");
    char extend[10];
    memset(extend,'\0',sizeof(extend));
    char message[5000];
    struct stat sb;
    char *c = (char*)arg;
    char type[50];
    memset(type,'\0',sizeof(type));
    char temp[100];
    strcpy(temp,c);
    char path[300];
    memset(path,'\0',sizeof(path));
    strcpy(path,abs_path);
    strcat(path,"/");
    strcat(path,temp);
    int a=0;
    //printf("%s\n",path);                    ///////////////////////////////
    while(temp[a]!='.'&& a<=strlen(temp))a++;
    a++;

    if(a>=strlen(temp)) { //is a directory
        if(stat(path,&sb)==-1) {
            //perror("stat");
            //exit(EXIT_FAILURE);
            strcpy(message,"HTTP/1.x 404 NOT_FOUND\r\nContent-Type: ");
            strcat(message,"\r\nServer: httpserver/1.x\r\n\r\n");

        } else if(S_ISDIR(sb.st_mode)) { //dir
            strcpy(message,"HTTP/1.x 200 OK\r\nContent-Type: directory ");
            strcat(message,"\r\nServer: httpserver/1.x\r\n\r\n");
            DIR *d;
            struct dirent *dir;
            //strcat(path,temp);
            d = opendir(path);
            if(d) {
                while((dir = readdir(d))!= NULL) {
                    if(strcmp(dir->d_name,".") ==0||strcmp(dir->d_name,"..") ==0)
                        continue;
                    strcat(message,dir->d_name);
                    strcat(message," ");//there is a blank at last
                }
                strcat(message,"\r\n");
            }
        }
        send(forClientSockfd,message,sizeof(message),0);//message
        return;
    }
    // printf("path %s\n",path);
    // int d;scanf("%d",d);

    strncpy(extend,temp+a,strlen(temp)-a);//get entend
    // printf("ex = %s\n",extend);
    int istype = 0;
    for(a=0; a<8; ++a) { //get extend type
        if(strcmp(extend,extensions[a].ext)==0) {
            strcpy(type,extensions[a].mime_type);
            istype = 1;
            // int d=5;
            // scanf("%d",d);
            // printf("%d\n",d);
            break;
        }
    }

    if(istype==0) {
        strcpy(message,"HTTP/1.x 415 UNSUPPORT_MEDIA_TYPE\r\nContent-Type: ");
        strcat(message,"\r\nServer: httpserver/1.x\r\n\r\n");
        send(forClientSockfd,message,sizeof(message),0);//message
        return;
    }

    if(stat(path,&sb)==-1) {
        //perror("stat");
        //exit(EXIT_FAILURE);
        strcpy(message,"HTTP/1.x 404 NOT_FOUND\r\nContent-Type: ");
        strcat(message,"\r\nServer: httpserver/1.x\r\n\r\n");

    } else { //file
        //printf("%s is a file\n",(char*)arg );
        FILE *f;

        if((f = fopen(path,"r"))==NULL) {
            printf("wrong open!\n");
            return;
        }
        strcpy(message,"HTTP/1.x 200 OK\r\nContent-Type: ");
        strcat(message,type);
        strcat(message," ");
        strcat(message,"\r\nServer: httpserver/1.x\r\n\r\n");
        // printf("%s",c);

        while(fgets(temp,sizeof(temp),f)!=NULL) {
            strcat(message,temp);
        }
        fclose(f);
    }
    send(forClientSockfd,message,sizeof(message),0);//message

}
int main(int argc, char *argv[])
{
    //  ./server -r /home/os2018/hw2-simple-my-http-server-s5901370 -p 1234 -n 32
    //  HTTP/1.x 200 OK\r\nContent-Type: text/html\r\nServer: httpserver/1.x\r\n\r\n
    //  ./server -r /home/os2018/hw2-simple-my-http-server-s5901370/testdir -p 1234 -n 32
    //socket的建立
    if(argc!=7) {
        printf("wrong usage\n");
        return;
    }
    memset(abs_path,'\0',sizeof(abs_path));
    strcpy(abs_path,argv[2]);
    //printf("%s\n",abs_path);
    int thread_num = atoi(argv[6]);
    int port=atoi(argv[4]);
    char inputBuffer[256] = {};
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;//don't care local ip ,decided by kernel
    serverInfo.sin_port = htons(port);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    threadpool_t *pool;
    pool = threadpool_create(thread_num,256,0);//thread queue
    pthread_mutex_init(&lock,NULL);
    while(1) {
        char message[1000] = {};
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
        // printf("Get:\n%s\n",inputBuffer);
        char method[10];
        memset(method,'\0',sizeof(method));
        int a=0,b;
        char name[1000];
        while(inputBuffer[a]!=' ')a++;
        strncpy(method,inputBuffer,a);
        b=a;
        a++;
        while(inputBuffer[a]!=' ')a++;
        memset(name,'\0',sizeof(name));
        strncpy(name,inputBuffer+b+1,a-b-1);//get file_name
        if(name[0]=='/') {
            if(strcmp(method,"GET")==0) {
                strncpy(name,name+1,strlen(name)-1);
                name[a-b-2]='\0';
                threadpool_add(pool,&find,(void*)name,0);

                // printf("name =%s\n",name);                                  ////////////
            } else { //Error condition(1)
            	strcat(message,"HTTP/1.x 405 METHOD_NOT_ALLOWED\r\nContent-Type:\r\nServer: httpserver/1.x\r\n\r\n");
                send(forClientSockfd,message,sizeof(message),0);//message
            }
        } else { //Error condition(4)
            strcat(message,"HTTP/1.x 400 BAD_REQUEST\r\nContent-Type:\r\nServer: httpserver/1.x\r\n\r\n");
            send(forClientSockfd,message,sizeof(message),0);//message
        }


    }
    return 0;
}

threadpool_t *threadpool_create(int thread_count, int queue_size, int flags)
{
    threadpool_t *pool;
    int i;
    (void) flags;

    if(thread_count <= 0 || thread_count > MAX_THREADS || queue_size <= 0 || queue_size > MAX_QUEUE) {
        return NULL;
    }

    if((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL) {
        goto err;
    }

    /* Initialize */
    pool->thread_count = 0;
    pool->queue_size = queue_size;
    pool->head = pool->tail = pool->count = 0;
    pool->shutdown = pool->started = 0;

    /* Allocate thread and task queue */
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    pool->queue = (threadpool_task_t *)malloc
                  (sizeof(threadpool_task_t) * queue_size);

    /* Initialize mutex and conditional variable first */
    if((pthread_mutex_init(&(pool->lock), NULL) != 0) ||
            (pthread_cond_init(&(pool->notify), NULL) != 0) ||
            (pool->threads == NULL) ||
            (pool->queue == NULL)) {
        goto err;
    }

    /* Start worker threads */
    for(i = 0; i < thread_count; i++) {
        if(pthread_create(&(pool->threads[i]), NULL,
                          threadpool_thread, (void*)pool) != 0) {
            threadpool_destroy(pool, 0);
            return NULL;
        }
        pool->thread_count++;
        pool->started++;
    }

    return pool;

err:
    if(pool) {
        threadpool_free(pool);
    }
    return NULL;
}

static void *threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    threadpool_task_t task;

    for(;;) {
        /* Lock must be taken to wait on conditional variable */
        //printf("A");
        pthread_mutex_lock(&(pool->lock));
        //printf("B");

        /* Wait on condition variable, check for spurious wakeups.
           When returning from pthread_cond_wait(), we own the lock. */
        while((pool->count == 0) && (!pool->shutdown)) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }
        //printf("C");

        if((pool->shutdown == immediate_shutdown) ||
                ((pool->shutdown == graceful_shutdown) &&
                 (pool->count == 0))) {
            break;
        }
        //printf("shudown = %d",pool->shutdown);

        /* Grab our task */
        task.function = pool->queue[pool->head].function;
        task.argument = pool->queue[pool->head].argument;
        pool->head = (pool->head + 1) % pool->queue_size;
        pool->count -= 1;

        /* Unlock */
        pthread_mutex_unlock(&(pool->lock));
        //printf("D");

        /* Get to work */
        (*(task.function))(task.argument);
        //printf("E");

    }

    pool->started--;

    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return(NULL);
}

int threadpool_add(threadpool_t *pool, void (*function)(void *),
                   void *argument, int flags)
{
    int err = 0;
    int next;
    (void) flags;

    if(pool == NULL || function == NULL) {
        return threadpool_invalid;
    }

    if(pthread_mutex_lock(&(pool->lock)) != 0) {
        return threadpool_lock_failure;
    }

    next = (pool->tail + 1) % pool->queue_size;

    do {
        /* Are we full ? */
        if(pool->count == pool->queue_size) {
            err = threadpool_queue_full;
            break;
        }

        /* Are we shutting down ? */
        if(pool->shutdown) {
            err = threadpool_shutdown;
            break;
        }

        /* Add task to queue */
        pool->queue[pool->tail].function = function;
        pool->queue[pool->tail].argument = argument;
        pool->tail = next;
        pool->count += 1;

        /* pthread_cond_broadcast */
        if(pthread_cond_signal(&(pool->notify)) != 0) {
            err = threadpool_lock_failure;
            break;
        }
    } while(0);

    if(pthread_mutex_unlock(&pool->lock) != 0) {
        err = threadpool_lock_failure;
    }

    return err;
}

int threadpool_destroy(threadpool_t *pool, int flags)
{
    int i, err = 0;

    if(pool == NULL) {
        return threadpool_invalid;
    }

    if(pthread_mutex_lock(&(pool->lock)) != 0) {
        return threadpool_lock_failure;
    }

    do {
        /* Already shutting down */
        if(pool->shutdown) {
            err = threadpool_shutdown;
            break;
        }

        pool->shutdown = (flags & threadpool_graceful) ?
                         graceful_shutdown : immediate_shutdown;

        /* Wake up all worker threads */
        if((pthread_cond_broadcast(&(pool->notify)) != 0) ||
                (pthread_mutex_unlock(&(pool->lock)) != 0)) {
            err = threadpool_lock_failure;
            break;
        }

        /* Join all worker thread */
        for(i = 0; i < pool->thread_count; i++) {
            if(pthread_join(pool->threads[i], NULL) != 0) {
                err = threadpool_thread_failure;
            }
        }
    } while(0);

    /* Only if everything went well do we deallocate the pool */
    if(!err) {
        threadpool_free(pool);
    }
    return err;
}

int threadpool_free(threadpool_t *pool)
{
    if(pool == NULL || pool->started > 0) {
        return -1;
    }

    /* Did we manage to allocate ? */
    if(pool->threads) {
        free(pool->threads);
        free(pool->queue);

        /* Because we allocate pool->threads after initializing the
           mutex and condition variable, we're sure they're
           initialized. Let's lock the mutex just in case. */
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->notify));
    }
    free(pool);
    return 0;
}



