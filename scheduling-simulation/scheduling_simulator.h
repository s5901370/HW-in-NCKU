#ifndef SCHEDULING_SIMULATOR_H
#define SCHEDULING_SIMULATOR_H

#include <stdio.h>
#include <ucontext.h>

#include "task.h"

enum TASK_STATE {
	TASK_RUNNING,
	TASK_READY,
	TASK_WAITING,
	TASK_TERMINATED
};
struct Process{
	ucontext_t context;
	char stack[1024*128];
	//int PID;
	char task_name[10];
	enum TASK_STATE state;
	int Qtime;//wait in ready Q
	int runTime;//L for 20 S for 10
	char priority;//H L
	char time;//L S
	int available;//is it remove?
	int waitTime;
	int flag;
	//struct Process *next;
};
// struct Thread{
// 	//ucontext_t main;
// 	struct Process v[100];
// 	int max_size;
// 	int running;//v[running] is running
// 	int high[100];//high priority queue
// 	int h_size;
// 	int low[100];//low priority
// 	int l_size;
// };

void hw_suspend(int msec_10);
void hw_wakeup_pid(int pid);
int hw_wakeup_taskname(char *task_name);
int hw_task_create(char *task_name);

#endif
