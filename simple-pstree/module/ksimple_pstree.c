#include"ksimple_pstree.h"
#include<linux/pid.h>
#include <linux/list.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/pid_namespace.h>
#define NETLINK_TEST 30
#define MSG_LEN 125
#define USER_PORT 100
//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL");
struct sock *nlsk = NULL;
extern struct net init_net;
char *kmsg;
int u_id;
int p_id;
struct pid *kpid;
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
int send_usrmsg(char *pbuf,uint16_t len)
{
    struct sk_buff *nl_skb;
    struct nlmsghdr *nlh;
    int ret;
    //create buff
    nl_skb = nlmsg_new(len,GFP_ATOMIC);
    if(!nl_skb) {
        printk("netlink alloc failure\n");
        return -1;
    }
    nlh = nlmsg_put(nl_skb,0,0,NETLINK_TEST,len,0);
    if(nlh == NULL) {
        printk("put fail\n");
        return -1;
    }

    memcpy(nlmsg_data(nlh),pbuf,len);
    ret = netlink_unicast(nlsk,nl_skb,USER_PORT,MSG_DONTWAIT);
    return ret;
}

int send(int id,int times)
{
    struct task_struct *t;
    struct pid *k;
    k = find_get_pid(id);
    t = pid_task(k,PIDTYPE_PID);
    int i;
    for(i=0; i<times; ++i)
        strcat(kmsg,"    ");
    strcat(kmsg,t->comm);
    strcat(kmsg,"(");
    char tem[10];
    sprintf(tem,"%d",t->pid);
    strcat(kmsg,tem);
    strcat(kmsg,")\n");
    return t->real_parent->pid;
}

void myalert(void)
{
    char wr[10]="wrong";
    send_usrmsg(wr,strlen(wr));//send kernel message
    return;
}
void DFS(struct task_struct* task,int times)
{
    struct list_head *list=NULL;
    list_for_each(list,&task->children) {//cur points child
        struct task_struct *cur = list_entry(list,struct task_struct,sibling);
        char t[30];
        int i;
        for(i=0; i<times; ++i)
            strcat(kmsg,"    ");
        strcat(kmsg,cur->comm);
        strcat(kmsg,"(");
        sprintf(t,"%d",cur->pid);
        strcat(kmsg,t);
        strcat(kmsg,")\n");
        //printk("%dAloop:%s,%d",times,cur->comm,cur->pid);
        //printk("  len:%d",strlen(kmsg));
        //printk("%s, %d",cur->comm,cur->pid);
        //send(cur->pid,1);
        // if(!list_empty(&cur->children)) {
        //  DFS(cur,times+1);
        // }
        if(!list_empty(&cur->children)) {
            DFS(cur,times+1);
        }
        // if(&cur->children){
        //  // struct list_head *l;
        //  // list_for_each(l,&cur->children) {//cur points child
        //  // struct task_struct *c = list_entry(l,struct task_struct,sibling);
        //  // char tt[30];
        //  // for(i=0;i<times+1;++i)
        //  //  strcat(kmsg,"    ");
        //  // strcat(kmsg,c->comm);
        //  // strcat(kmsg,"(");
        //  // sprintf(tt,"%d",c->pid);
        //  // strcat(kmsg,tt);
        //  // strcat(kmsg,")\n");
        //  // printk("rww %s,%d",c->comm,c->pid);
        //  //printk("%s, %d",cur->comm,cur->pid);
        //  //send(cur->pid,1);
        //  // }
        // }
    }
}
static void netlink_rcv_msg(struct sk_buff* skb)
{
    struct nlmsghdr *nlh = NULL;
    char *umsg = NULL;
    struct task_struct *task;
    kmsg=(char*)kmalloc(sizeof(char)*6000,GFP_KERNEL);
    kmsg[0]='\0';
    if(!kmsg) {
        char wr[10]="wr malloc";
        send_usrmsg(wr,strlen(wr));//send kernel message
        return;
    }
    int i;
    //kmsg = (char*)kmalloc(1000*sizeof(char),GFP_KERNEL);
    if(skb->len >= nlmsg_total_size(0)) {
        nlh = nlmsg_hdr(skb);
        umsg = NLMSG_DATA(nlh);//get user message
        if(umsg) {//work here
            char temp[10];
            strncpy(temp,umsg+2,strlen(umsg)-2);
            temp[strlen(umsg)-2]='\0';
            u_id = char_to_int(temp);//get user id
            kpid = find_get_pid(u_id);
            task = pid_task(kpid,PIDTYPE_PID);
            if(!task) {
                printk("task no exist");
            } else {
                if(umsg[0]=='c') {
                    strcat(kmsg,task->comm);
                    strcat(kmsg,"(");
                    char tem[10];
                    sprintf(tem,"%d",task->pid);
                    strcat(kmsg,tem);
                    strcat(kmsg,")\n");
                    DFS(task,1);
                } else if(umsg[0]=='s') {
                    if((task->real_parent)) {//check if has parent
                        struct list_head *list=NULL;
                        printk("%s,%d",task->real_parent->comm,task->real_parent->pid);
                        list_for_each(list,&task->real_parent->children) {//cur points child
                            struct task_struct *cur = list_entry(list,struct task_struct,sibling);
                            if(cur->pid == u_id);
                            else {
                                char t[30];
                                strcat(kmsg,cur->comm);
                                strcat(kmsg,"(");
                                sprintf(t,"%d",cur->pid);
                                strcat(kmsg,t);
                                strcat(kmsg,")\n");
                            }
                        }
                    }
                } else if(umsg[0]=='p') {
                    char stack[30][30];
                    for(i=0; i<30; ++i)
                        stack[i][0]='\0';
                    char t[30];
                    strcpy(stack[0],task->comm);
                    strcat(stack[0],"(");
                    sprintf(t,"%d",task->pid);
                    strcat(stack[0],t);
                    strcat(stack[0],")\n");
                    int k=1;
                    if(task->pid!=1) {//not init so has parent
                        while(task->real_parent->pid != 1) {
                            char t[30];
                            task=task->real_parent;
                            strcpy(stack[k],task->comm);
                            strcat(stack[k],"(");
                            sprintf(t,"%d",task->pid);
                            strcat(stack[k],t);
                            strcat(stack[k++],")\n");
                            // printk("k=%d",k);
                        }
                        task=task->real_parent;
                        strcpy(stack[k],task->comm);
                        strcat(stack[k],"(");
                        sprintf(t,"%d",task->pid);
                        strcat(stack[k],t);
                        strcat(stack[k],")\n");
                        // for(i=k-1;i>=0;--i)
                        //  strcat(kmsg,stack[i]);
                    }
                    for(i=k; i>=0; --i) {
                        int j;
                        for(j=0; j<k-i; ++j)
                            strcat(kmsg,"    ");
                        strcat(kmsg,stack[i]);
                        printk("%s",stack[i]);
                    }
                }
            }
            printk("kernel recv from user: %s\n",umsg);
            send_usrmsg(kmsg,strlen(kmsg));//send kernel message
            // printk("%d",strlen(kmsg));
        }
    }
    kfree(kmsg);
}

struct netlink_kernel_cfg cfg= {
    .input = netlink_rcv_msg,
};

int test_netlink_init(void)
{
    nlsk = (struct sock*)netlink_kernel_create(&init_net,NETLINK_TEST,&cfg);
    if(nlsk == NULL) {
        printk("net_ker_crea error\n");
        return -1;
    }
    printk("test_net_init");
    return 0;
}
void test_netlink_exit(void)
{
    if(nlsk) {
        netlink_kernel_release(nlsk);
        nlsk=NULL;
    }
    //remove_proc_entry(proc_name,NULL);
    //kfree(kmsg);
    printk("<0>""test_netlink_exit\n");
}
module_init(test_netlink_init);
module_exit(test_netlink_exit);
// static int hello_init(void){
//  printk(KERN_ALERT "hello\n");
//  return 0;
// }
// static void hello_exit(void){
// printk(KERN_ALERT "goodbye\n");
//  return 0;

// }
// module_init(hello_init);
// module_exit(hello_exit);
// struct pid* pid_struct;
// struct task_struct *task;
// static struct proc_dir_entry *proc_write_entry;
// char *proc_name = "task_by_pid";

// int read_proc(char *buf,char **start,off_t offset,int count,
//  int *eof,void *data)
// {
//  int len =0;
//  pid_struct = find_get_pid(p_id);
//  task = pid_task(pid_struct,PIDTYPE_PID);
//  len = sprintf(buf,"\nname &s\n",task->comm);
//  return len;
// }

// int write_proc(struct file *file,const char *buf,int count,void *data){
//  int ret;
//  char *id;
//  id = (char*)kmalloc(1000*sizeof(char),GFP_KERNEL);
//  printk(KERN_INFO "buf passed %s",buf);
//  if(copy_from_user(id,buf,count))
//      return -EFAULT;
//  printk(KERN_INFO "id passed %s",id);
//  p_id = simple_strtoul(id,NULL,0);
//  return sizeof(buf);
// }

// void create_new_proc_entry(void){
//  proc_write_entry = create_proc_entry(proc_name,0666,NULL);
//  if(!proc_write_entry){
//      printk(KERN_INFO "error create proc entry");
//      return -ENOMEN;
//  }
//  proc_write_entry->read_proc = read_proc;
//  proc_write_entry->write_proc = write_proc;
// }
