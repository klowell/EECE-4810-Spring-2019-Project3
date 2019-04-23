/*************************************************************************************
File Name: sched_sim.h

Objective: Provide data structures and types and the respective function prototypes
		   for a CPU scheduling simulator

Created By: Kristopher Lowell
Date Created: 4/20/2019

Time to Complete: Approx 24 hours, accumulative

File History:
	Submitted - KCL - 4/22/2019
*************************************************************************************/

#ifndef SCHED_SIM_H
#define SCHED_SIM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

extern const int QUANTUM;

extern const int NUM_OF_ALGORITHMS;

typedef enum algorithm { FCFS, SJF, STCF, RR, NPP } Algorithm;				// Enumerated type for different algorithm types

typedef enum state { NEW, READY, RUNNING, WAITING, TERMINATED } State;		// Enumerated type for different states of a process

typedef struct process {					// Process structure type to store appropriate information for a process

	int ID;									// Process ID (PID)
	int burstTime;							// CPU Burst Time
	int priority;							// Priority Level (lower number denotes higher priority)
	int arrivalTime;						// Arrival Time
	int timeRemaining;						// Time Remaining (how much time is left in order to finish process)
	int startTime;							// Start Time (for calcuating wait time)
	int lastStart;							// Last Start Time (for calculating when to switch in Round Robin)
	int endTime;							// End Time (for calculating turnaround time)
	int turnaroundTime;						// Turnaround Time (how long it took for the process to finish after arrival)
	int waitTime;							// Wait Time (how long the process had to wait)
	State status;							// Process State (for tracking when to put into ready queue)

} Process;

typedef struct node {						// Queue node structure type - using linked list structure to allow for flexibility in size
											// Used for ready queue
	Process *data;
	struct node *next;

} Node;

typedef struct queue {						// Queue structure type - using linked list structure to allow for flexibility in size
											// Used for ready queue
	Node *head;
	Node *tail;

} Queue;

typedef struct llnode {						// Linked list node structure type - allows for flexibility in size without having to reallocate memory everytime
											// something has to be added
	int data;								// Used for tracking sequence of processes
	struct llnode *next;

} LLNode;

typedef struct linkedlist {					// Linked list node structure type - allows for flexibility in size without having to reallocate memory everytime
											// something has to be added
	LLNode *head;							// Used for tracking sequence of processes
	LLNode *tail;

} LinkedList;

typedef struct info {						// Algorithm summary info structure type
	
	double avgWaitTime;
	double avgTurnaroundTime;
	LinkedList *processSequence;
	int contextSwitches;

} Info;

Queue* make_queue();																							// Initialize queue data structure

void enqueue_process(Queue *queue, Process *process);															// Add process to queue, new process is new tail for FIFO operation

void burst_time_enqueue_process(Queue *queue, Process *process);												// Add process to queue, burst time acts as priority; Used for SJF

void time_remaining_enqueue_process(Queue *queue, Process *process);											// Add process to queue, time remaining acts as priority: Used for STCF

void priority_enqueue_process(Queue *queue, Process *process);													// Add process to priority queue

int dequeue_process(Queue *queue);																				// Returns first process's ID from queue and removes it

int peek_queue(Queue *queue);																					// Returns first process's ID without removing it from the queue

bool is_queue_empty(Queue *queue);																				// Returns whether or not the queue is currently empty

void print_queue(FILE *outFile, Queue *queue);																	// Prints the contents of the queue (just the processes' IDs)

void destroy_queue(Queue *queue);																				// Destroys/deallocates queue to avoid memory leaks

LinkedList* make_linked_list();																					// Initialize linked list data structure

void add_to_linked_list(LinkedList *ll, int data);																// Adds data to linked list, emulating a queue for FIFO operation

void print_linked_list(FILE *outFile, LinkedList *ll);															// Prints the contents of the linked list

bool is_linked_list_empty(LinkedList *ll);																		// Returns whether or not the linked list is currently empty

void destroy_linked_list(LinkedList *ll);																		// Destroys/deallocates linked list to avoid memory leaks		

Info* make_info();																								// Initialize info data structure	

void destroy_info(Info* information);																			// Destroys/deallocates info structure type to avoid memory leaks

int find_num_of_processes(FILE *inFile);																		// Read input file for number of processes to simulate

Process* load_processes(FILE *inFile, int numOfProcesses);														// Read input file and load contents into process objects

Info* simulate(FILE *outFile, int interval, Process *processes, int numOfProcesses, Algorithm algo);			// Main simulation

void print_results(FILE *outFile, Process *processes, int numOfProcesses, Info **summaries, Algorithm algo);	// Print the end-of-algorithm statistics and results

void print_overall_results(FILE *outFile, Info **summaries);													// Print the summary of performances for every algorithm

#endif