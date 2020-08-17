#include "client.h"
#include "status.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <pthread.h>

int sockfd = 0;
struct sockaddr_in info;
int port;
void* func(void *data)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Fail to create a socket.");
    }
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(port);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1) {
        printf("Connection error");
    }


    char *c=(char*)data;
    char message[100] = {"GET "};
    strcat(message,c);
    strcat(message," HTTP/1.x\r\nHOST: 127.0.0.1:1234\r\n\r\n");
    char receiveMessage[10000] = {};
    send(sockfd,message,sizeof(message),0);
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
    close(sockfd);
    printf("%s",receiveMessage);
    int a=0;
    while(receiveMessage[a]!=' ')a++;
    a++;
    char temp[100];
    memset(temp,0,sizeof(temp));
    strncpy(temp,receiveMessage+a,3);//get status
    if(strcmp(temp,"200")==0) { //do something
        while(receiveMessage[a]!=' ')a++;
        a++;
        while(receiveMessage[a]!=' ')a++;
        a++;
        int b=a;
        // printf("%s",receiveMessage+a);
        while(receiveMessage[a]!=' ')a++;
        strncpy(temp,receiveMessage+b,a-b);//get type
        if(strcmp(temp,"directory")==0) {
            a=0;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;//receiv+a
            b=0;
            while(receiveMessage[a+b]!='\r') {
                while(receiveMessage[a+b]!=' '&&receiveMessage[a+b]!='\r')b++;
                if(receiveMessage[a+b]=='\r')
                    break;
                char ts[100];
                memset(ts,0,sizeof(ts));
                strncpy(ts,receiveMessage+a,b);
                a +=(b+1);
                b=0;
                char tt[100];
                memset(tt,0,sizeof(tt));
                strcpy(tt,c);
                strcat(tt,"/");
                strcat(tt,ts);
                // printf("tt = %s\n",tt );
                pthread_t thread;
                // char nija[100];
                // strcpy(nija,tt);
                pthread_create(&thread,NULL,func,tt);
                pthread_join(thread,NULL);
                // t = strtok(NULL," ");
                // printf("t=%s\n",t);
            }
        } else {
            struct stat sb;
            char path[100];
            memset(path,0,sizeof(path));
            strcpy(path,"output");
            if(stat("output",&sb)==-1) {
                mkdir("output",0777);
            }
            char *tok1;
            char *tok2;
            tok1 = strtok(c,"/");
            while(tok1!=NULL) { //create dir
                tok2=tok1;
                strcat(path,"/");
                strcat(path,tok1);
                tok1 = strtok(NULL,"/");
                if(tok1==NULL)break;
                if(stat(path,&sb)==-1) {
                    mkdir(path,0777);
                }
            }
            a=0;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;//receiv+a
            FILE *f = fopen(path,"w");
            fprintf(f,"%s",receiveMessage+a);
            fclose(f);
        }
    }
}

int main(int argc, char *argv[])
{

    // GET QUERY_FILE_OR_DIR HTTP/1.x\r\nHOST: LOCALHOST:PORT\r\n\r\n
    // ./client -t /example.html -h 127.0.0.1 -p 1234
    // ./client -t /testdir/example.html -h 127.0.0.1 -p 1234
    //max_FILENAME_size 128

    //socket的建立
    if(argc!=7) {
        printf("wrong usage\n");
        return;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Fail to create a socket.");
    }
    port=atoi(argv[6]);
    //socket的連線

    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(port);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1) {
        printf("Connection error");
    }


    //Send a message to server
    char message[100] = {"GET "};
    strcat(message,argv[2]);
    strcat(message," HTTP/1.x\r\nHOST: 127.0.0.1:1234\r\n\r\n");
    char receiveMessage[10000] = {};
    send(sockfd,message,sizeof(message),0);
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
    printf("%s",receiveMessage);
    close(sockfd);



    int a=0;
    while(receiveMessage[a]!=' ')a++;
    a++;
    char temp[100];
    memset(temp,0,sizeof(temp));
    strncpy(temp,receiveMessage+a,3);//get status
    if(strcmp(temp,"200")==0) { //do something
        while(receiveMessage[a]!=' ')a++;
        a++;
        while(receiveMessage[a]!=' ')a++;
        a++;
        int b=a;
        // printf("%s",receiveMessage+a);
        while(receiveMessage[a]!=' ')a++;
        strncpy(temp,receiveMessage+b,a-b);//get type
        if(strcmp(temp,"directory")==0) {
            a=0;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;//receiv+a

            b=0;
            while(receiveMessage[a+b]!='\r') {
                while(receiveMessage[a+b]!=' '&&receiveMessage[a+b]!='\r')b++;
                if(receiveMessage[a+b]=='\r')
                    break;
                char ts[100];
                memset(ts,0,sizeof(ts));
                strncpy(ts,receiveMessage+a,b);
                a +=(b+1);
                b=0;
                char tt[100];
                memset(tt,0,sizeof(tt));
                strcpy(tt,argv[2]);
                strcat(tt,"/");
                strcat(tt,ts);
                // printf("tt = %s\n",tt );
                pthread_t thread;
                // char nija[100];
                // strcpy(nija,tt);
                pthread_create(&thread,NULL,func,tt);
                pthread_join(thread,NULL);
                // t = strtok(NULL," ");
                // printf("t=%s\n",t);
            }
            // printf("\n??\n");

        } else {
            struct stat sb;
            char path[100];
            memset(path,0,sizeof(path));
            strcpy(path,"output");
            if(stat("output",&sb)==-1) {
                mkdir("output",0777);
            }
            char *tok1;
            char *tok2;
            tok1 = strtok(argv[2],"/");
            while(tok1!=NULL) { //create dir
                tok2=tok1;
                strcat(path,"/");
                strcat(path,tok1);
                tok1 = strtok(NULL,"/");
                if(tok1==NULL)break;
                if(stat(path,&sb)==-1) {
                    mkdir(path,0777);
                }
            }
            a=0;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;
            while(receiveMessage[a]!='\n')a++;
            a++;//receiv+a
            FILE *f = fopen(path,"w");
            fprintf(f,"%s",receiveMessage+a);
            fclose(f);
        }
    }
    //printf("\n");
    //printf("close Socket\n");


    return 0;


}
// int main(int argc, char *argv[])
// {


//  if(argc==7){
//      if(strlen())
//  }
//  else{

//  }
//  return 0;
// }

