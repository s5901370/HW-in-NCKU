#include "scheduling_simulator.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
struct Process ctx[100];
int max_size;
int running;//ctx[running] is running
int high[100];//high priority queue
int h_size;
int low[100];//low priority
int l_size;
int w_size;
int flag;
struct itimerval v,ov;//don't need to care ov
ucontext_t _main,_run,test;
void del_high(int id){
	if(h_size>=1&&h_size>=id){
		if(id == h_size){//last oen in queue
			h_size--;
			return;
		}
		int i;
		for(i=id;i<h_size;++i)
			high[i]=high[i+1];
		h_size--;
	}
}
void del_low(int id){
	if(l_size>=1&&l_size>=id){
		if(id == l_size){//last oen in queue
			l_size--;
			return;
		}
		int i;
		for(i=id;i<l_size;++i)
			low[i]=low[i+1];
		l_size--;
	}
}
void add_high(int id){
	h_size++;
	high[h_size]=id;
	//printf("h_size = %d\n",h_size);
}
void add_low(int id){
	l_size++;
	low[l_size]=id;
	//printf("l_size = %d\n",l_size);
}
void start_timer(){
	v.it_value.tv_sec = 0;//initial value to count
	v.it_value.tv_usec = 10000;
	v.it_interval.tv_sec =0;//next to count
	v.it_interval.tv_usec =10000;
	if(setitimer(ITIMER_REAL,&v,NULL)!=0)printf("timerfail");
}
void stop_timer(){
	v.it_value.tv_sec = 0;//initial value to count
	v.it_value.tv_usec = 0;
	v.it_interval.tv_sec =0;//next to count
	v.it_interval.tv_usec =0;
	setitimer(ITIMER_REAL,&v,NULL);
}
void init_ctx(){
	int i;
	for(i=1;i<100;++i){
		memset(ctx[i].task_name,0,sizeof(ctx[i].task_name));
		ctx[i].state = TASK_READY;
		ctx[i].Qtime = 0;
		ctx[i].available = 1;
		ctx[i].waitTime = -1;//no go into wait
		ctx[i].flag=0;
		ctx[i].priority =0;
		ctx[i].runTime = 0;
	}
}
void sig(int signal);
void sig_time(int signal);
void simulate(void);
void handle();
void hw_suspend(int msec_10)
{
	/*change state to TASK_WAITING
	reschedule next to run
	change state to TASK_READY after msec_10*10ms
	*/
	//usleep(10000*msec_10);
	w_size++;
	// stop_timer();
	//char stack[1024*16];
	ctx[running].state = TASK_WAITING;
	ctx[running].waitTime = msec_10;
	handle();
	swapcontext(&ctx[running].context,&_run);
	//printf("hw_suspend\n");
	//ctx[running].state = 
	//fprintf(stdout, "task3: good morning~\n");
	// printf("in sus run = %d\n",running);
	//fflush(stdout);
	//pause();
}

void hw_wakeup_pid(int pid)
{
	/*
	change the state of pid from TASK_WAITING to TASK_READY
	reschedule
	*/
	if(pid>=1&&pid<=max_size){
		if(ctx[pid].available==1){
			if(ctx[pid].state == TASK_WAITING){
				ctx[pid].state = TASK_READY;
				ctx[pid].waitTime = 0;
				if(ctx[pid].priority == 'H')
					add_high(pid);
				else if(ctx[pid].priority == 'L')
					add_low(pid);
				// printf("wake up id %d\n",pid);
			}
		}
	}
	return;
}

int hw_wakeup_taskname(char *task_name)
{
	/*
	change all the task_name ....
	return how many tasks are wakeup!!
	reschedule
	*/
	int count = 0;
	int i;
	for(i=1;i<=max_size;++i){
		if(ctx[i].available==1){
			if(strcmp(ctx[i].task_name,task_name)==0){
				if(ctx[i].state == TASK_WAITING){
				ctx[i].state = TASK_READY;
				ctx[i].waitTime =0;
				count++;
				if(ctx[i].priority == 'H')
					add_high(i);
				else if(ctx[i].priority == 'L')
					add_low(i);
			}
			}
		}
	}
	// printf("count = %d\n",count);
    return count;
}

int hw_task_create(char *task_name)
{
	/*
	create taskanme 
	return pid
	return -1 if there is 
	*/
	max_size++;
	ctx[max_size].runTime = 10;
	ctx[max_size].time = 'S';
	ctx[max_size].priority = 'L';
	add_low(max_size);
	strcpy(ctx[max_size].task_name,task_name);
	// printf("create %s",ctx[max_size].task_name);
    return max_size; // the pid of created task name
}
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
void sig(int signal){

	if(running==-1&&w_size==0){
		printf("\n$ ");	
		fflush(stdout);
	}
	else if(running == -1 && w_size>0){
		// printf("wating\n");
		getitimer(ITIMER_REAL,&v);
		swapcontext(&_run,&_main);
		setitimer(ITIMER_REAL,&v,NULL);
	}
	else{
		// printf("no oen run\n");
		getitimer(ITIMER_REAL,&v);
		swapcontext(&ctx[running].context,&_main);
		setitimer(ITIMER_REAL,&v,NULL);
	}
}
void sig_time(int signal){
	handle();
	if(running== -1)
		return;
	swapcontext(&ctx[running].context,&_run);
}
void handle(){
	// printf("get timer\n");
	stop_timer();
	flag = 0;//come in interrupt
	if(max_size>=1){//add Qtime
		int i;
		for(i=1;i<=max_size;++i)
		{
			if(i!=running)
			{//not the running context
				if(ctx[i].available == 1)
				{//not removed
					if(ctx[i].state == TASK_READY)
						ctx[i].Qtime+=10;
					if(ctx[i].state == TASK_WAITING)
					{
						ctx[i].waitTime--;
						// printf("i = %d,wait = %d\n",i,ctx[i].waitTime);
						if(ctx[i].waitTime==0)
						{//is it need to check runTime!=0??
							ctx[i].state = TASK_READY;
							if(ctx[i].priority=='H')
								add_high(i);
							else if(ctx[i].priority=='L')
								add_low(i);
						}
					}
				}
				//NOP
			}
			else{//running oen
				if(ctx[i].state ==TASK_WAITING){// just be hangup
					// printf("conti\n");
					continue;
				}
				else if(ctx[i].state == TASK_TERMINATED){//be waken up
					ctx[i].runTime =0;
					w_size--;
					continue;
				}
				ctx[i].runTime -= 10;
				// printf("runTime =%d\n",ctx[i].runTime);
				if(ctx[i].runTime==0){
					ctx[i].state = TASK_TERMINATED;
					// printf("%d %s termina\n",i,ctx[i].task_name);
				}
				else{
					ctx[i].state = TASK_READY;
					if(ctx[i].priority=='H'){
						add_high(i);
					}
					else if(ctx[i].priority=='L'){
						add_low(i);
					}
				}
			}
		}
	}	
	
}

void simulate(void){
	while(1){
		//getcontext(&_simu);
		if(h_size>=1){//handle high pri
			// printf("handle high pri\n");                             ///////////////////////////
			running = high[1];
			del_high(1);
		}
		else if(l_size>=1){
			// printf("handle low pri\n");                             ///////////////////////////
			running = low[1];
			del_low(1);
		}
		else if(w_size>=1){
			//printf("remain waiting");
			running = -1;
			//printf("waiting\n");
			//if(v.it_interval.tv_usec == 0){
			// if(flag==0){
			// 	flag=1;
			// }
			//}
			//getitimer(TIMER_REAL,&ov);
			if(v.it_interval.tv_usec==0)
				start_timer();
			continue;
			
		}
		else{//no task to handle
			printf("handle nothing\n");                             ///////////////////////////
			stop_timer();
			running = -1;//no one running
			//setitimer(ITIMER_VIRTUAL,&n,NULL);
			//setcontext(&_main);
			swapcontext(&_run,&_main);
		}
		//start simulate
		start_timer();
		flag =1;
		if(strcmp(ctx[running].task_name,"task1")==0){
			printf("handle task1\n");
			if(ctx[running].flag==0){
				ctx[running].flag=1;
				ctx[running].state = TASK_RUNNING;
				getcontext(&ctx[running].context);
				ctx[running].context.uc_stack.ss_sp = ctx[max_size].stack;
				ctx[running].context.uc_stack.ss_size = 1024*128;
				ctx[running].context.uc_stack.ss_flags = 0;
				ctx[running].context.uc_link = &_run;
				makecontext(&ctx[running].context,task1,0);
			}
			swapcontext(&_run,&ctx[running].context);
			if(flag ==0);
			else if(flag ==1){
				printf("task1 ter early\n");
				handle();
				// ctx[running].state = TASK_TERMINATED;
				// ctx[running].runTime = 0;
			}
		}
		else if(strcmp(ctx[running].task_name,"task2")==0){
			printf("handle task2\n");
			if(ctx[running].flag==0){
				ctx[running].flag=1;
				ctx[running].state = TASK_RUNNING;
				getcontext(&ctx[running].context);
				ctx[running].context.uc_stack.ss_sp = ctx[running].stack;
				ctx[running].context.uc_stack.ss_size = 1024*128;
				ctx[running].context.uc_stack.ss_flags = 0;
				ctx[running].context.uc_link = &_run;
				makecontext(&ctx[running].context,task2,0);
			}
			swapcontext(&_run,&ctx[running].context);
		}
		else if(strcmp(ctx[running].task_name,"task3")==0){
			printf("handle task3\n");
			if(ctx[running].flag==0){
				ctx[running].flag=1;
				ctx[running].state = TASK_RUNNING;
				getcontext(&ctx[running].context);
				ctx[running].context.uc_stack.ss_sp = ctx[running].stack;
				ctx[running].context.uc_stack.ss_size = 1024*128;
				ctx[running].context.uc_stack.ss_flags = 0;
				ctx[running].context.uc_link = &_run;
				makecontext(&ctx[running].context,task3,0);
				swapcontext(&_run,&ctx[running].context);
			}
			if(ctx[running].waitTime==0){
				swapcontext(&_run,&ctx[running].context);
				ctx[running].state = TASK_TERMINATED;
				w_size--;
			}
		}
		else if(strcmp(ctx[running].task_name,"task4")==0){
			printf("handle task4\n");
			if(ctx[running].flag==0){
				ctx[running].flag=1;
				ctx[running].state = TASK_RUNNING;
				getcontext(&ctx[running].context);
				ctx[running].context.uc_stack.ss_sp = ctx[running].stack;
				ctx[running].context.uc_stack.ss_size = 1024*128;
				ctx[running].context.uc_stack.ss_flags = 0;
				ctx[running].context.uc_link = &_run;
				makecontext(&ctx[running].context,task4,0);
				swapcontext(&_run,&ctx[running].context);
			}
			if(ctx[running].waitTime==0){
				swapcontext(&_run,&ctx[running].context);
				ctx[running].state = TASK_TERMINATED;
				w_size--;
			}
			
		}
		else if(strcmp(ctx[running].task_name,"task5")==0){
			printf("handle task5\n");
			if(ctx[running].flag==0){
				ctx[running].flag=1;
				ctx[running].state = TASK_RUNNING;
				getcontext(&ctx[running].context);
				ctx[running].context.uc_stack.ss_sp = ctx[running].stack;
				ctx[running].context.uc_stack.ss_size = 1024*128;
				ctx[running].context.uc_stack.ss_flags = 0;
				ctx[running].context.uc_link = &_run;
				makecontext(&ctx[running].context,task5,0);
				swapcontext(&_run,&ctx[running].context);
			}
			if(ctx[running].waitTime==0){
				swapcontext(&_run,&ctx[running].context);
				ctx[running].state = TASK_TERMINATED;
				w_size--;
			}
			
		}
		else if(strcmp(ctx[running].task_name,"task6")==0){
			printf("handle task6\n");
			if(ctx[running].flag==0){
				ctx[running].flag=1;
				ctx[running].state = TASK_RUNNING;
				getcontext(&ctx[running].context);
				ctx[running].context.uc_stack.ss_sp = ctx[running].stack;
				ctx[running].context.uc_stack.ss_size = 1024*128;
				ctx[running].context.uc_stack.ss_flags = 0;
				ctx[running].context.uc_link = &_run;
				makecontext(&ctx[running].context,task6,0);
				swapcontext(&_run,&ctx[running].context);
			}
			if(ctx[running].waitTime==0){
				swapcontext(&_run,&ctx[running].context);
				ctx[running].state = TASK_TERMINATED;
				w_size--;
			}
			
		}


	}//while loop
}

int main()//add task1 -t L(S) -p H(L)
{
	signal(SIGTSTP,sig);
	signal(SIGALRM,sig_time);
	// struct sigaction sa;
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = 0;
	// sa.sa_handler = sig_time;
	// sigaction(SIGALRM,&sa,NULL);
	
	//setitimer(ITIMER_VIRTUAL,&v,&ov);
	//signal(SIGINT,sig);
	max_size = 0;//for loop start from 1
	l_size = 0;
	h_size = 0;
	running = -1;//no run
	w_size=0;
	char stack[1024*128];
	_run.uc_stack.ss_sp = stack;
	_run.uc_stack.ss_size = 1024*128;
	_run.uc_stack.ss_flags = 0;
	_run.uc_link = NULL;
	flag=0;
	// int x;
	// for(x=1;x<100;++x){
	// 	ctx[x].
	// }
	init_ctx();
	while(1){
		//getcontext(&_main);//come back shell mode
		printf("$ ");
		char input[30];memset(input,0,sizeof(input));
		fgets(input,sizeof(input),stdin);//get input
		
		if(input[0]=='a'&&input[1]=='d'&&input[2]=='d'){
			//printf("add mode\n");
			int ptr = 0;
			while(input[ptr]!=' '&&ptr<30)ptr++; 
			ptr++;
			int a =ptr;
			while(input[a]!=' '&&a<30)a++;
			char task[10];memset(task,0,sizeof(task));
			strncpy(task,input+4,5);//get task
			// getcontext(&ctx[max_size].context);
			// ctx[max_size].context.uc_stack.ss_sp = ctx[max_size].stack;
			// ctx[max_size].context.uc_stack.ss_size = 1024*128;
			// ctx[max_size].context.uc_stack.ss_flags = 0;
			// ctx[max_size].context.uc_link = &_run;
			// if(task[4]=='1'){
			// 	printf("create task1\n");
			// 	makecontext(&ctx[max_size].context,start_timer,0);
			// }
			// else if(task[4]=='2')
			// 	makecontext(&ctx[max_size].context,task2,0);
			// else if(task[4]=='3')
			// 	makecontext(&ctx[max_size].context,task3,0);
			// else if(task[4]=='4')
			// 	makecontext(&ctx[max_size].context,task4,0);
			// else if(task[4]=='5')
			// 	makecontext(&ctx[max_size].context,task5,0);
			// else if(task[4]=='6')
			// 	makecontext(&ctx[max_size].context,task6,0);
			a++;
			max_size++;
			
			strcpy(ctx[max_size].task_name,task);
			if(input[a]=='-'){
				if(input[a+1]=='t'){
					if(input[a+3]=='L')
					{
						// printf("-t L ");
						ctx[max_size].time = 'L';
						ctx[max_size].runTime = 20;
					}
					else if(input[a+3]=='S')
					{
						// printf("-t S ");
						ctx[max_size].time = 'S';
						ctx[max_size].runTime = 10;
					}

					if(input[a+5]=='-')
					{
						if(input[a+8]=='H')
						{
							// printf("-p H ");
							ctx[max_size].priority = 'H';
							add_high(max_size);
						}
						else if(input[a+8]=='L')
						{
						// printf("-p L ");
							ctx[max_size].priority = 'L';
							add_low(max_size);
						}
					}
					else
					{//def pri L
						// printf("-p L ");
						ctx[max_size].priority = 'L';
						add_low(max_size);
					}
				}
				else if(input[a+1]=='p')
				{
					if(input[a+3]=='H')
					{
						// printf("-t S -p H ");
						ctx[max_size].runTime = 10;
						ctx[max_size].time = 'S';
						ctx[max_size].priority = 'H';
						add_high(max_size);
					}
					else if(input[a+3]=='L')
					{
						// printf("-t S -p L ");
						ctx[max_size].runTime = 10;
						ctx[max_size].time = 'S';
						ctx[max_size].priority = 'L';
						add_low(max_size);
					}
				}
			}
			else{//ALL def
						// printf("-t S -p L");
				ctx[max_size].runTime = 10;
				ctx[max_size].time = 'S';
				ctx[max_size].priority = 'L';
				add_low(max_size);
			}
		}
		else if(input[0]=='r'&&input[1]=='e'&&input[2]=='m'&&input[3]=='o'&&input[4]=='v'&&input[5]=='e'){
			//printf("remove mode\n");
			int a=6;
			char pid[10];memset(pid,0,sizeof(pid));
			while(input[a]!='\n')a++;
			strncpy(pid,input+7,a-7);//get pid
			a = char_to_int(pid);
			if(a<=max_size){
				if(ctx[a].available == 1)
				ctx[a].available = 0;//remove
				if(ctx[a].state == TASK_WAITING&&w_size>=0)
					w_size--;
			}
		}
		else if(input[0]=='p'&&input[1]=='s'){
			//printf("ps mode\n");
			int i=1;
			for(i=1;i<=max_size;++i){
				if(ctx[i].available==1){
					char tt[20];
					switch(ctx[i].state){
						case TASK_READY:
						strcpy(tt,"TASK_READY        ");break;
						case TASK_WAITING:
						strcpy(tt,"TASK_WAITING       ");break;
						case TASK_RUNNING:
						strcpy(tt,"TASK_RUNNING      ");break;
						case TASK_TERMINATED:
						strcpy(tt,"TASK_TERMINATED   ");break;
					}
					printf("%-2d %s  %s %-3d %c  %c\n",i,ctx[i].task_name,tt,ctx[i].Qtime,ctx[i].priority,ctx[i].time);
				}
			}
		}
		else if(input[0]=='s'&&input[1]=='t'&&input[2]=='a'&&input[3]=='r'&&input[4]=='t'){
			//printf("start mode\n");
			getcontext(&_run);
			makecontext(&_run,simulate,0);
			swapcontext(&_main,&_run);
		}
		else if(input[0]=='8')
			return 0;
		else{
			printf("error usage\n");
		}
	}
}
