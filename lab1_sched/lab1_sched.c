/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32142858
*	    Student name : Won Kyu Ho
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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


int queNum = 3; // Queue의 최대 수 3
int timeSlice = 1; //Time slice의 수 1 
int processNum = 5; //Process의 수 5개
int maxServTime = 20; //최대 Service Time 20


int arrivalTime[5]; // Store Arrival Times from Sample
int serviceTime[5]; //Store Service Time from Sample

struct Queue {
	int max, front, rear;
	int *data;
};

void qInit(struct Queue *q, int max) {
	q->max = max;
	q->front = q->rear = -1;
	q->data = malloc(sizeof(int) * max);
}

void qDestroy(struct Queue *q) {
	free(q->data);

}

void qPush(struct Queue *q, int param) {
	if (q->front == q->max - 1) {
		q->max *= 2;
		int *newArray = malloc(sizeof(int) * q->max);
		for (int i = 0; i < q->max/2; i++)
			newArray[i] = q->data[i];
		
		free(q->data);
		q->data = newArray;
		
	}
	q->data[++(q->front)] = param;
}

int qPop(struct Queue *q) {
	if (q->front == q->rear) {
		return -1; 
	}
	return q->data[++(q->rear)];
}

int qPeek(struct Queue *q) { // Just to look a data
	return q->data[q->rear+1];
}

int qSize(struct Queue *q) {
	return q->front - q->rear;
}


int getLeftTime(int *leftTimeArr, int processNum) { // calculate Left time of Scheduler
	int totalLeftTime = 0;
		for (int i = 0; i < processNum; i++)
			totalLeftTime += leftTimeArr[i];
	return totalLeftTime;
}

void getResult(int result[], int procTime){  // Print out result of Scheduler (except Stride)
 
	printf("----------------------------------------------------------------\n");
	printf("                         RESULT\n");
	printf("----------------------------------------------------------------\n");
	printf("T|  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20\n");
	printf("----------------------------------------------------------------\n");


	for(int i=0; i<5; i++){

		if(i == 0){
			printf("A| ");
		}
		
		if(i == 1){
			printf("B| ");
		}

		if(i == 2){ 
			printf("C| ");
		}

		if(i == 3){
			printf("D| ");
		}

		if(i == 4){
			printf("E| ");
		}
		
		
		for(int j= 0; j<procTime; j++){
			if(result[j]== i){
				printf(" O ");

			}else{
				printf(" X ");
			}
		
		}
		printf("\n");
	}

	printf("----------------------------------------------------------------\n");

}



void sample(){
		
	int option = 0;
	int option_arrtime_one[5] = {0, 2, 4, 6, 8}; //Option 1's Arrival Time
	int option_servtime_one[5] = {3, 6, 4, 5, 2}; //Option 1's Service Time
	int option_arrtime_two[5] = {0, 3, 5, 7, 9}; // Option 2's Arrival Time
	int option_servtime_two[5] = {4, 7, 3, 4, 2}; // Option 2's Service Time

	printf("which sample would you like you like to choose?\n");
	printf("1.Example from page 24\n");
	printf("2.Another Example\n");

	scanf("%d", &option);

	switch(option){
	
		case 1:
			
			memcpy(arrivalTime, option_arrtime_one, sizeof(arrivalTime));
			memcpy(serviceTime, option_servtime_one, sizeof(serviceTime));
			
			printf("\n");
			printf("Arrival Time: [0, 2, 4, 6, 8]\n");
			printf("Service Time: [3, 6, 4, 5, 2}]\n");
			printf("\n");

			break;

		case 2:
			
			memcpy(arrivalTime, option_arrtime_two, sizeof(arrivalTime));	
			memcpy(serviceTime, option_servtime_two, sizeof(serviceTime));
			
			printf("\n");
			printf("Arrival Time: [0, 3, 5, 7, 9]\n");
			printf("Service Time: [4, 7, 3, 4, 2}]\n");
			printf("\n");

			break;

		default : printf("Please input value from 1~2\n");

	}
		
}



void FIFO(){

	sample();
	
	int totalServTime = 0; 
	int *leftServTime = malloc(sizeof(int) * processNum); 
	int procTime = 0; //Processed Time to show how much Time passed
	int curPrcs = -1; //Current Process (Process A == 0), Starts with -1

	struct Queue *queue = malloc(sizeof(struct Queue) *  queNum); //Make Queue
	struct Queue temp;

	qInit(&temp, 100);//Initialize Queue
	queue[0] = temp;
	
	for (int i = 0; i < processNum; i++) {
		totalServTime += arrivalTime[i];
		leftServTime[i] = serviceTime[i];
	}
	
	int *result = malloc(sizeof(int) * totalServTime * 10); //Make result

	for (int i = 0; i < totalServTime * 10; i++){ //Initilaze Result
		result[i] = -1;
	}
	

	while (getLeftTime(leftServTime, processNum) > 0) { //Scheduling Start

		for (int i = 0; i < processNum; i++){
			if (arrivalTime[i] == procTime) {
				for (int j = 0; j < serviceTime[i]; j++){
					qPush(&queue[0], i); //Push Processes in to Queue up to Servicetime
				}
			}
		}

		curPrcs = qPop(&queue[0]);
				
		result[procTime] = curPrcs; 
		leftServTime[curPrcs]--;
		procTime++;
	}
	
	if(procTime !=0){
		getResult(result,procTime);
	}
	
}
	




void RR(){

	sample();

	int totalServTime = 0; 	
	int *leftServTime = malloc(sizeof(int) * processNum); 
	int procTime = 0; //Processed Time to show how much Time passed
	int curPrcs = -1; //Current Process (Process A == 0), Starts with -1
	int timeCounter = timeSlice; //To use as Counter 

	struct Queue *queue = malloc(sizeof(struct Queue) *  queNum); //Struct Queue
	struct Queue temp;
		
	qInit(&temp, 100);//Initialize Queue
	queue[0] = temp;
	
	
	for (int i = 0; i < processNum; i++) {
		totalServTime += arrivalTime[i];
		leftServTime[i] = serviceTime[i];
	}
	
	int *result = malloc(sizeof(int) * totalServTime * 10);

	for (int i = 0; i < totalServTime * 10; i++){ //Initialize Result
		result[i] = -1;
	}
	
	while (getLeftTime(leftServTime, processNum) > 0) { // Scheduling Starts

		for (int i = 0; i < processNum; i++){
			if (arrivalTime[i] == procTime) {
				for (int j = 0; j < serviceTime[i]; j++){
					qPush(&queue[0], i);
				}
			}
		}


		int totalQueNum = 0; // for add Size of Queue
		totalQueNum += qSize(&queue[0]);
		

		if (leftServTime[curPrcs] == 0){
			timeCounter = timeSlice;
		}

		
			
			if (qSize(&queue[0]) > 0) {
				if (curPrcs == qPeek(&queue[0])){ //Peek Next Process (If Process was Scheduled before, time counter--)
					timeCounter--;
				}
				if (timeCounter <= 0 && totalQueNum != leftServTime[curPrcs]) {
					for (int j = 0; j < leftServTime[curPrcs]; j++){ 
						qPush(&queue[0], qPop(&queue[0])); //Push Processes to Back of Queue up to left Service Time
	
					}
					timeCounter = timeSlice;
					
				}
			}



		curPrcs = qPop(&queue[0]);
		result[procTime] = curPrcs; 
		leftServTime[curPrcs]--;
		procTime++;
		
	}
	
	if(procTime !=0){
		getResult(result,procTime);
	}
	
}




void MLFQ(){

	sample();

	int totalServTime = 0; 	
	int *leftServTime = malloc(sizeof(int) * processNum); 
	int procTime = 0; //Processed Time to show how much Time passed
	int curPrcs = -1; //Current Process (Process A == 0), Starts with -1
	int timeCounter = timeSlice; //To use as Counter 


	struct Queue *queue = malloc(sizeof(struct Queue) *  queNum); //Struct Queue
	
	for (int i = 0; i <  queNum; i++) { // Initialize Queue up to max number of Que (In this Code, Max is 3)
		struct Queue temp;
		qInit(&temp, 100);
		queue[i] = temp;
	}
	
	for (int i = 0; i < processNum; i++) {
		totalServTime += arrivalTime[i];
		leftServTime[i] = serviceTime[i];
	}
	
	int *result = malloc(sizeof(int) * totalServTime * 10);

	for (int i = 0; i < totalServTime * 10; i++){
		result[i] = -1;
	}
	
	while (getLeftTime(leftServTime, processNum) > 0) {//Scheduling Start

		for (int i = 0; i < processNum; i++){
			if (arrivalTime[i] == procTime) {
				for (int j = 0; j < serviceTime[i]; j++){
					qPush(&queue[0], i);
				}
			}
		}


		int totalQueNum = 0;

		for (int i = 0; i <  queNum; i++){
			totalQueNum += qSize(queue+i);
		}

		if (leftServTime[curPrcs] == 0){
			timeCounter = timeSlice;
		}

		for (int i = 0; i <  queNum; i++){//Inquiry all Queue to find Process
			if (qSize(&queue[i]) > 0) {
				if (curPrcs == qPeek(&queue[i])){//Peek Next Process (If Process was Scheduled before, time counter--)
					timeCounter--;
				}
				if (timeCounter <= 0 && totalQueNum != leftServTime[curPrcs]) {
					for (int j = 0; j < leftServTime[curPrcs]; j++){
						if (i !=  queNum - 1){  //If Process was Scheduled before, Put value of curPrcs to next Queue Line up to Left Service Time
							qPush(&queue[i+1], qPop(&queue[i]));
							
						}else{
							qPush(&queue[i], qPop(&queue[i])); //If this QueLine is Last, Push process
							
						}
					}
					timeCounter = timeSlice;
					break;
				}
			}
		}


		for (int i = 0; i <  queNum; i++){ //Inquiry Queue lines and Pop process
			if (qSize(&queue[i]) > 0) {
				curPrcs = qPop(&queue[i]);
				break;
			}
			else{
				curPrcs = -1;
			}
		}

		if (curPrcs > -1) { 
			result[procTime] = curPrcs; 
			leftServTime[curPrcs]--;
		}

		procTime++;
	}
	
	if(procTime !=0){
		getResult(result,procTime);
	}
	
}




void STRIDE(){
	
	int strprocessNum = 3; //Stride Processes are 3
	int tickets[3] = {100, 50, 250}; //ticket value for each Processes
	int ticketStride[3]= {0,0,0}; //To put Stride value
	int strArrTime[3] = {0,0,0}; // Stride Processes Start From 0 at Once
	int strServTime[3] = {5,5,5}; //Stride Proceeses share there Service Times
	int value[3] = {0,0,0};	// To add Values
	int totalTicket = 10000; // To Divde Ticket



	struct Queue *queue = malloc(sizeof(struct Queue) *  queNum);
	
		struct Queue temp;
		qInit(&temp, 100);
		queue[0] = temp;
	

	
	int totalServiceTime = 0; 
	int *leftServiceTimeArr = malloc(sizeof(int) * processNum); 
	
	for (int i = 0; i < strprocessNum; i++) {	
		ticketStride[i] = (totalTicket/tickets[i]); //Devide 10000 by Ticket 
		totalServiceTime += strServTime[i];
		leftServiceTimeArr[i] = strServTime[i];
	}
	
	int *result = malloc(sizeof(int) * totalServiceTime * 10);

	for (int i = 0; i < totalServiceTime * 10; i++){
		result[i] = -1;
	}

	int procTime = 0; 
	int curProc; 

	while (getLeftTime(leftServiceTimeArr, strprocessNum) > 0) { // Start Scheduler

		while (1){
			if(value[0] == value[1]&& value[0]==value[2]){ // If value is all equal, Push processes A>B>C

				for (int i = 0; i < strprocessNum; i++){
			
					leftServiceTimeArr[i]--;
					value[i]+=ticketStride[i];
					qPush(&queue[0], i);
					result[procTime] = qPop(&queue[0]);
					procTime++;
				}
					
				}else{ // Find min value
				int min=0;

				int num1 = value[0];
				int num2 = value[1];
				int num3 = value[2];

				if(num1 >= num2){
					min = num2;
				}else {
					min = num1;
				}

				if(num3<min){
					min = num3;
				}

				if(min==num1){
					curProc=0;
					value[0]+=ticketStride[0];
				}

				if(min==num2){
					curProc=1;
					value[1]+=ticketStride[1];
				}

				if(min==num3){
					curProc=2;
					value[2]+=ticketStride[2];
				}					
			break;
			}

		}				
		qPush(&queue[0], curProc);
		result[procTime] = qPop(&queue[0]); 
		leftServiceTimeArr[curProc]--;	
		procTime++;
	}

printf("------------------------------------------------------\n");
printf("                   STRIDE RESULT\n");
printf("------------------------------------------------------\n");
	for(int i=0; i<3; i++){	
		
		if(i == 0){
			printf("A (100)| ");
		}
		
		if(i == 1){
			printf("B (200)| ");
		}

		if(i == 2){ 
			printf("C (40) | ");
		}

		for(int j= 0; j<procTime; j++){
			if(result[j]== i){
				printf(" O ");

			}else{
				printf(" X ");
			}
		
		}
		printf("\n");
	
	}

	printf("------------------------------------------------------\n");
}







