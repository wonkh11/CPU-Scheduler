/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32142858
*	    Student name : Won Kyu Ho
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"


int main(int argc, char *argv[]){
	int menu = 0;
	
	printf("select Scheduling Simulaytor.\n");
	printf("1. FIFO  2. RR  3. MLFQ  4. Stride \n");
	scanf("%d", &menu);

	switch(menu){
		case 1:FIFO();
			break;
		case 2: RR();
			break;

		case 3: MLFQ();
			break;
		case 4: STRIDE();
			break;
		default : printf("Please input value from 1~5\n");
	}
	return 0;
}

