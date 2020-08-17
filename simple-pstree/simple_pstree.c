#include"simple_pstree.h"
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<linux/netlink.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <asm/types.h>
#include <errno.h>
#define NETLINK_TEST 30
#define MAX_PLOAD 10240
#define MSG_LEN 10240
struct sockaddr_nl saddr,daddr;
struct nlmsghdr *nlh = NULL;
int skfd;
int ret;
socklen_t len;
struct msghdr msg;
char umsg[100] ;
typedef struct _user_msg_info {
    struct nlmsghdr hdr;
    char msg[MSG_LEN];
} user_msg_info;
user_msg_info u_info;
int main(int argc, char const *argv[])
{
    if(argc == 2) {// ./simple-pstree + -c or -s or -p
        if(argv[1][1]=='c') {
            if(strlen(argv[1])>2) { //-cXXXX
                umsg[0] = 'c';
                umsg[1]=' ';
                char w[100];
                strncpy(w,argv[1]+2,strlen(argv[1])-2);
                w[strlen(argv[1])-2]='\0';
                strcat(umsg,w);
            } else {
                strcpy(umsg,"c 1");
            }
        } else if(argv[1][1]=='s') {
            if(strlen(argv[1])>2) {//-cXXXX
                umsg[0] = 's';
                umsg[1]=' ';
                char w[100];
                strncpy(w,argv[1]+2,strlen(argv[1])-2);
                w[strlen(argv[1])-2]='\0';
                strcat(umsg,w);
            } else {
                char str[10];
                sprintf(str,"%d",getpid());
                strcpy(umsg,"s ");
                strcat(umsg,str);
            }
        } else if(argv[1][1]=='p') {
            if(strlen(argv[1])>2) {//-cXXXX
                umsg[0] = 'p';
                umsg[1]=' ';
                char w[100];
                strncpy(w,argv[1]+2,strlen(argv[1])-2);
                w[strlen(argv[1])-2]='\0';
                strcat(umsg,w);
            } else {
                char str[10];
                sprintf(str,"%d",getpid());
                strcpy(umsg,"p ");
                strcat(umsg,str);
            }
        } else { //default
            strcpy(umsg,"c ");
            char w[100];
            strncpy(w,argv[1],strlen(argv[1]));
            w[strlen(argv[1])]='\0';
            strcat(umsg,w);
        }
    } else { //default one argument
        strcpy(umsg,"c 1");
    }
    skfd = socket(AF_NETLINK, SOCK_RAW,NETLINK_TEST);
    memset(&msg,0,sizeof(msg));
    memset(&saddr,0,sizeof(saddr));
    saddr.nl_family = AF_NETLINK;
    saddr.nl_pid = 100;//self pid
    saddr.nl_groups = 0;
    bind(skfd,(struct sockaddr*)&saddr,sizeof(saddr));
    memset(&daddr,0,sizeof(daddr));
    daddr.nl_family = AF_NETLINK;
    daddr.nl_pid = 0;  //for linux kernel
    daddr.nl_groups = 0;//unicast

    nlh=(struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PLOAD));
    memset(nlh,0,sizeof(struct nlmsghdr));
    //fill netlink message header
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
    nlh->nlmsg_pid = saddr.nl_pid;//self pid
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_type = 0;
    nlh->nlmsg_seq = 0;

    memcpy(NLMSG_DATA(nlh),umsg,strlen(umsg));
    ret = sendto(skfd,nlh,nlh->nlmsg_len,0,(struct sockaddr*)&daddr,sizeof(struct sockaddr_nl));

    if(!ret) {
        perror("sendto error\n");
        close(skfd);
        exit(-1);
    }
    memset(&u_info,0,sizeof(u_info));
    len = sizeof(struct sockaddr_nl);
    ret = recvfrom(skfd,&u_info,sizeof(user_msg_info),0,(struct sockaddr*)&daddr,&len);
    if(!ret) {
        perror("recr from kernel error \n");
        close(skfd);
        exit(-1);
    }
    printf("%s\n",u_info.msg);
    close(skfd);
    free((void*)nlh);
    return 0;
}
