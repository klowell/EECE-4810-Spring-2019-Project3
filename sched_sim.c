/*************************************************************************************
File Name: sched_sim.c

Objective: Provide the various functions needed for data structures and types for
		   a CPU scheduling simulator

Created By: Kristopher Lowell
Date Created: 4/20/2019

Time to Complete: Approx 24 hours, accumulative

File History:
	Submitted - KCL - 4/22/2019
*************************************************************************************/

#include "sched_sim.h"

Queue* make_queue()
{
	Queue *queue = (Queue *) malloc(sizeof(Queue));

	queue->head = NULL;									// An empty queue will have both head and tail pointing to NULL
	queue->tail = NULL;

	return queue;
}

void enqueue_process(Queue *queue, Process *process)
{
	Node *newNode = (Node *) malloc(sizeof(Node));		// Allocate new memory to store the new node in the queue

	newNode->data = process;							// New node will contain pointer to process and point to NULL
	newNode->next = NULL;								// as it will be the new tail

	if(is_queue_empty(queue))							// If queue is empty, the new node will be the only node
	{
		queue->head = newNode;
		queue->tail = newNode;
	}
	else												// In every other instance, the new node will be the new tail
	{
		queue->tail->next = newNode;					// Old tail must point to new tail
		queue->tail = newNode;							// Tail should point to new node/tail
	}
}

void burst_time_enqueue_process(Queue *queue, Process *process)
{
	Node *current;										// Temporary tracking node to progress through queue
	Node *newNode = (Node *) malloc(sizeof(Node));		// Allocate new memory to store the new node in the queue

	newNode->data = process;							// New node will contain pointer to process and point to NULL
	newNode->next = NULL;								// as it may become the new tail

	if(is_queue_empty(queue))							// If queue is empty, the new node will be the only node
	{
		queue->head = newNode;
		queue->tail = newNode;
		return;
	}

	current = queue->head;						// Start tracking through the queue starting from the head

	if(queue->head->data->burstTime > process->burstTime)	// If first node's burst time is greater than new node's
	{														// the new node becomes the first node/head
		newNode->next = queue->head;
		queue->head = newNode;
	}
	else
	{
		while(current->next != NULL && current->next->data->burstTime < process->burstTime)
		{
			current = current->next;					// Progress through the queue until either the end is reached or a longer burst time is found
		}

		if(queue->tail == current)						// If the end is reached, the new node will be the new tail
			queue->tail = newNode;

		newNode->next = current->next;					// The new node must point to the next in line and
		current->next = newNode;						// the previous node must point to the new node
	}
}

void time_remaining_enqueue_process(Queue *queue, Process *process)
{
	Node *current;										// Temporary tracking node to progress through queue
	Node *newNode = (Node *) malloc(sizeof(Node));		// Allocate new memory to store the new node in the queue

	newNode->data = process;							// New node will contain pointer to process and point to NULL
	newNode->next = NULL;								// as it may become the new tail

	if(is_queue_empty(queue))							// If queue is empty, the new node will be the only node
	{
		queue->head = newNode;
		queue->tail = newNode;
		return;
	}

	current = queue->head;							// Start tracking through the queue starting from the head

	if(queue->head->data->timeRemaining > process->timeRemaining)	// If first node's time remaining is greater than new node's
	{																// the new node becomes the first node/head
		newNode->next = queue->head;
		queue->head = newNode;

	}
	else
	{
		while(current->next != NULL && current->next->data->timeRemaining < process->timeRemaining)
		{
			current = current->next;				// Progress through the queue until either the end is reached or a longer time remaining is found
		}

		if(queue->tail == current)						// If the end is reached, the new node will be the new tail
			queue->tail = newNode;

		newNode->next = current->next;					// The new node must point to the next in line and
		current->next = newNode;						// the previous node must point to the new node
	}
}

void priority_enqueue_process(Queue *queue, Process *process)
{
	Node *current;										// Temporary tracking node to progress through queue
	Node *newNode = (Node *) malloc(sizeof(Node));		// Allocate new memory to store the new node in the queue

	newNode->data = process;							// New node will contain pointer to process and point to NULL
	newNode->next = NULL;								// as it may become the new tail

	if(is_queue_empty(queue))							// If queue is empty, the new node will be the only node
	{
		queue->head = newNode;
		queue->tail = newNode;
		return;
	}

	current = queue->head;								// Start tracking through the queue starting from the head

	if(queue->head->data->priority > process->priority)		// If first node's priority level is greater than new node's
	{														// the new node becomes the first node/head
		newNode->next = queue->head;
		queue->head = newNode;
	}
	else
	{
		while(current->next != NULL && current->next->data->priority < process->priority)
		{
			current = current->next;					// Progress through the queue until either the end is reached or a greater priority level is found
		}

		if(queue->tail == current)						// If the end is reached, the new node will be the new tail
			queue->tail = newNode;

		newNode->next = current->next;					// The new node must point to the next in line and
		current->next = newNode;						// the previous node must point to the new node
	}
}

int dequeue_process(Queue *queue)
{
	int ID;											// Holder variable to store ID of first node
	Node *temp;										// Holder variable to store the data of the first node

	if(is_queue_empty(queue))						// If queue is currently empty, return -1 to flag such
		return -1;

	temp = queue->head;								// Set temporary node to point to head
	ID = temp->data->ID;							// Store the head's ID since it will soon be unavailable

	queue->head = queue->head->next;				// Forget the previous head by advancing the head pointer up the list

	free(temp);										// Since the temporary node still points to the original head, we free the memory pointed to by the temporary node

	if(queue->head == NULL)							// If the queue only had one node, then signal tail pointer to point to NULL as well to avoid a dangling pointer
		queue->tail = NULL;

	return ID;										// Return the ID of the original first node
}

int peek_queue(Queue *queue)
{
	return queue->head->data->ID;					// Return the ID of the first node in the queue
}

bool is_queue_empty(Queue *queue)
{
	return queue->head == NULL && queue->tail == NULL;		// If the head and tail point to NULL, then the queue is empty
}

void print_queue(FILE *outFile, Queue *queue)
{
	Node current;										// Temporary tracking node to progress through queue

	if(is_queue_empty(queue))							// If queue is empty, print as such
	{
		fprintf(outFile, "Empty\n");
		return;
	}

	current.data = queue->head->data;					// Start tracking through the queue starting from the head
	current.next = queue->head->next;

	while(current.next != NULL)							// Progress through the queue until the end is reached
	{
		fprintf(outFile, "%d-", current.data->ID);		// Print out the ID's of the processes
		current.data = current.next->data;
		current.next = current.next->next;
	}

	fprintf(outFile, "%d\n", queue->tail->data->ID);	// Print out the tail of the queue; Accounting for a single node queue
}

void destroy_queue(Queue *queue)
{
	Node *current = queue->head;						// Progress through the queue starting from the head

	while(current != NULL)
	{
		current = queue->head->next;					// Destroy/deallocate any nodes left in the queue
		free(queue->head);
		queue->head = current;
	}

	free(queue);										// Destroy/deallocate the memory reserved for the queue
	queue = NULL;
}

LinkedList* make_linked_list()
{
	LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));

	ll->head = NULL;									// An empty list will have both head and tail pointing to NULL
	ll->tail = NULL;

	return ll;
}

void add_to_linked_list(LinkedList *ll, int data)
{
	LLNode *newNode = (LLNode *) malloc(sizeof(LLNode));	// Allocate new memory to store the new node in the list

	newNode->data = data;							// New node will contain pointer to process and point to NULL
	newNode->next = NULL;							// as it will be the new tail

	if(is_linked_list_empty(ll))					// If list is empty, the new node will be the only node
	{
		ll->head = newNode;
		ll->tail = newNode;
	}
	else											// In every other instance, the new node will be the new tail
	{
		ll->tail->next = newNode;					// Old tail must point to new tail
		ll->tail = newNode;							// Tail should point to new node/tail
	}
}

void print_linked_list(FILE *outFile, LinkedList *ll)
{
	LLNode current;									// Temporary tracking node to progress through list

	if(is_linked_list_empty(ll))					// If list is empty, print as such
	{
		fprintf(outFile, "Empty\n");
		return;
	}

	current.data = ll->head->data;					// Start tracking through the list starting from the head
	current.next = ll->head->next;

	while(current.next != NULL)
	{
		fprintf(outFile, "%d-", current.data);		// Print out the ID's of the processes
		current.data = current.next->data;
		current.next = current.next->next;
	}

	fprintf(outFile, "%d\n", ll->tail->data);		// Print out the tail of the list; Accounting for a single node list
}

bool is_linked_list_empty(LinkedList *ll)
{
	return ll->head == NULL && ll->tail == NULL;	// If either the head and tail point to NULL, then the list is empty
}

void destroy_linked_list(LinkedList *ll)
{
	LLNode *current= ll->head;						// Progress through the queue starting from the head

	while(current != NULL)
	{
		current = ll->head->next;
		free(ll->head);								// Destroy/deallocate any nodes left in the queue
		ll->head = current;	
	}

	free(ll);										// Destroy/deallocate the memory reserved for the list
	ll = NULL;										// Set pointer to NULL to avoid dangling pointer
}

Info* make_info()
{
	Info* information = (Info *) malloc(sizeof(Info));

	information->avgWaitTime = 0;
	information->avgTurnaroundTime = 0;
	information->processSequence = make_linked_list();		// Piggyback initialize function for included list
	information->contextSwitches = 0;

	return information;
}

void destroy_info(Info *information)
{
	destroy_linked_list(information->processSequence);		// Piggyback destroy function for included list

	free(information);										// Destroy/deallocate the memory reserved for the info type
	information = NULL;										// Set pointer to NULL to avoid dangling pointer
}

int find_num_of_processes(FILE *inFile)
{
	int numOfProcesses = 0;					// Variable for finding number of processes detailed in the input file
	char ch;								// Holder variable for getc conversion

	while (!feof(inFile))					// Read file until the end of file (EOF)
	{
		ch = fgetc(inFile);					// Store character into holder variable
		if(!isalnum(ch) && ch != ' ')		// Increment the number of processes variable when a new-line character is found
			numOfProcesses++;
	}

	rewind(inFile);							// Rewind the input file back to the beginning for future reading

	return numOfProcesses;
}

Process* load_processes(FILE *inFile, int numOfProcesses)
{
	int charCount = 0;						// Counter variable for current size of the buffer
	char buffer[255];						// Buffer string to hold the numeric characters stored in the input file

	Process *processes = (Process *) malloc(sizeof(Process) * numOfProcesses);	// Dynamically allocate space for all processes

	for(int i = 0; i < numOfProcesses; i++)
	{
		processes[i].ID = i;												// Set ID of processes starting from 0

		charCount = 0;
		for(int j = 0; j < 255; j++)										// Clear the buffer, also ensures string ends in NULL character '\0' for proper string printing
			buffer[j] = '\0';
		while(!isalnum(fgetc(inFile)));
		fseek(inFile, -1, SEEK_CUR);

		while(isalnum(fgetc(inFile)))										// Put characters into buffer until white-space character is found
		{
			fseek(inFile, -1, SEEK_CUR);
			buffer[charCount] = fgetc(inFile);
			charCount++;
		}
		processes[i].burstTime = atoi(buffer);								// Convert buffer string into integer and store as the burst time for process

		charCount = 0;
		for(int j = 0; j < 255; j++)
			buffer[j] = '\0';												// Clear the buffer and ensure string ends in NULL character
		while(!isalnum(fgetc(inFile)));
		fseek(inFile, -1, SEEK_CUR);										// Read characters until no more white-space is found

		while(isalnum(fgetc(inFile)))										// Put characters into buffer until white-space is found again
		{
			fseek(inFile, -1, SEEK_CUR);
			buffer[charCount] = fgetc(inFile);
			charCount++;
		}
		processes[i].priority = atoi(buffer);								// Convert buffer string into integer and store as the priority for process

		charCount = 0;
		for(int j = 0; j < 255; j++)
			buffer[j] = '\0';												// Clear the buffer and ensure string ends in NULL character
		while(!isalnum(fgetc(inFile)));
		fseek(inFile, -1, SEEK_CUR);										// Read characters until no more white-space is found

		while(isalnum(fgetc(inFile)))										// Put characters into buffer until white-space or end of file is found
		{
			fseek(inFile, -1, SEEK_CUR);
			buffer[charCount] = fgetc(inFile);
			charCount++;
		}
		buffer[charCount] = '\0';
		processes[i].arrivalTime = atoi(buffer);							// Convert buffer string into integer and store as arrival time for process
	}

	return processes;														// Return the pointer to the processes array
}

Info* simulate(FILE *outFile, int interval, Process *processes, int numOfProcesses, Algorithm algo)
{
	int currentTime = 0, processesComplete = 0, activeProcess = -1;							// Simulation Time, Number of processes that have been completed, the PID of the currently running process
	Info *summary = make_info();															// The information pertaining to the current simulation of the selected algorithm
	Queue *readyQueue = make_queue();														// The ready queue - contains pointers to processes that are ready to be run

	for (int i = 0; i < numOfProcesses; i++)												// Resetting auxillary variables for processes for proper simulation of multiple algorithms
	{
		processes[i].timeRemaining = processes[i].burstTime;
		processes[i].startTime = -1;
		processes[i].lastStart = -1;
		processes[i].endTime = -1;
		processes[i].turnaroundTime = -1;
		processes[i].waitTime = -1;
		processes[i].status = NEW;
	}

	switch(algo)															// Print out title for each type of algorithm
	{
		case FCFS:
			fprintf(outFile, "***** FCFS Scheduling *****\n");
			break;

		case SJF:
			fprintf(outFile, "***** SJF Scheduling *****\n");
			break;

		case STCF:
			fprintf(outFile, "***** STCF Scheduling *****\n");
			break;

		case RR:
			fprintf(outFile, "***** Round Robin Scheduling *****\n");
			break;

		case NPP:
			fprintf(outFile, "***** Priority Scheduling *****\n");
			break;
	}

	while(processesComplete < numOfProcesses)		// Main simulation loop;  Ends when all processes are complete
	{
		for(int i = 0; i < numOfProcesses; i++)									// Scan through processes and add proper processes to ready queue according to different algorithms
		{
			if(currentTime >= processes[i].arrivalTime)
			{
				if(algo == NPP && processes[i].status == WAITING && (currentTime - processes[i].lastStart) % 25 == 0)		// Implement aging by increasing (decrementing) prirority in priority scheduling
					processes[i].priority--;

				if(processes[i].status == NEW || processes[i].status == WAITING)			// Processes should only be added if they are either new processes or processes that were preempted (waiting)
				{
					switch(algo)
					{
						case FCFS: case RR:
								enqueue_process(readyQueue, &processes[i]);					// Simple enqueue for FCFS; Processes should only be in NEW status for FCFS; Process should only be in NEW or WAIT status for RR
								processes[i].status = READY;
							break;

						case SJF:
								burst_time_enqueue_process(readyQueue, &processes[i]);		// Enqueue while ordering shortest burst time first;  Processes should only be in NEW status for SJF
								processes[i].status = READY;
							break;

						case STCF:
								time_remaining_enqueue_process(readyQueue, &processes[i]);	// Enqueue while ordering shortest time remaining first; Processes should be in either NEW or WAIT status for STCF
								processes[i].status = READY;
							break;

						case NPP:
								priority_enqueue_process(readyQueue, &processes[i]);		// Enqueue while ordering lowest priority level first;  Processes should only be in NEW status for non-preemptive priority
								processes[i].status = READY;
							break;
					}
				}
			}
		}
		
		if(activeProcess == -1 && !is_queue_empty(readyQueue))						// Loading first process
		{
			if(currentTime % interval == 0)											// Print out info for each interval
			{
				fprintf(outFile, "t = %d\n", currentTime);
				fprintf(outFile, "CPU: Loading Process %d (CPU Burst = %d)\n", peek_queue(readyQueue), processes[peek_queue(readyQueue)].burstTime);
				fprintf(outFile, "Ready Queue: ");
				print_queue(outFile, readyQueue);
				fprintf(outFile, "\n");
			}

			activeProcess = dequeue_process(readyQueue);							// Pull first process from ready queue				
			processes[activeProcess].status = RUNNING;								// Set process as running
			processes[activeProcess].startTime = currentTime;						// Set start time for first process
			processes[activeProcess].lastStart = currentTime;						// Set last time process was started for round robin scheduling
			processes[activeProcess].timeRemaining--;								// Decrement time remaining for currently running process
			summary->contextSwitches++;												// Increment number of context switches
			add_to_linked_list(summary->processSequence, activeProcess);			// Add process to sequence list
		}

		else if(activeProcess == -1 && is_queue_empty(readyQueue))					// No process running, and no processes in the ready queue
		{
			if(currentTime % interval == 0)											// Print out the info for each interval
			{
				fprintf(outFile, "t = %d\n", currentTime);
				fprintf(outFile, "CPU: Idle, Waiting for a Process\n");
				fprintf(outFile, "Ready Queue: ");
				print_queue(outFile, readyQueue);
				fprintf(outFile, "\n");
			}
		}

		else if(processes[activeProcess].timeRemaining <= 0 && !is_queue_empty(readyQueue))		// Loading in next process - when one process finishes, another begins
		{
			if(currentTime % interval == 0)														// Print out info for each interval
			{
				fprintf(outFile, "t = %d\n", currentTime);
				fprintf(outFile, "CPU: Finishing Process %d; Loading Process %d (CPU Burst = %d)\n", activeProcess, peek_queue(readyQueue), processes[peek_queue(readyQueue)].timeRemaining);
				fprintf(outFile, "Ready Queue: ");
				print_queue(outFile, readyQueue);
				fprintf(outFile, "\n");
			}

			processes[activeProcess].status = TERMINATED;										// Set completed process as terminated
			processes[activeProcess].endTime = currentTime;										// Set end time for completed process
			processesComplete++;																// Increment number of completed processes
			activeProcess = dequeue_process(readyQueue);										// Pull next process from ready queue
			processes[activeProcess].status = RUNNING;											// Set new process to running

			if(processes[activeProcess].timeRemaining == processes[activeProcess].burstTime)
				processes[activeProcess].startTime = currentTime;								// Set start time for new process, not for a preempted one

			processes[activeProcess].lastStart = currentTime;									// Set last time process was started for round robin scheduling
			processes[activeProcess].timeRemaining--;											// Decrement time remaining for currently running process
			summary->contextSwitches++;															// Increment number of context switches
			add_to_linked_list(summary->processSequence, activeProcess);						// Add new process to sequence list
		}

		else if(processes[activeProcess].timeRemaining <= 0 && is_queue_empty(readyQueue))		// Finishing last process; When all processes are done, the ready queue will be empty
		{
			if(currentTime % interval == 0)											// Print out info for each interval
			{
				fprintf(outFile, "t = %d\n", currentTime);
				fprintf(outFile, "CPU: Finishing Process %d\n", activeProcess);
				fprintf(outFile, "Ready Queue: ");
				print_queue(outFile, readyQueue);
				fprintf(outFile, "\n");
			}

			processes[activeProcess].status = TERMINATED;							// Set last process as terminated
			processes[activeProcess].endTime = currentTime;							// Set end time for last process
			processesComplete++;													// Increment number of completed processes
			activeProcess = -1;														// Reset active process holder variable
		}

		else if(algo == STCF && !is_queue_empty(readyQueue) && processes[activeProcess].timeRemaining > processes[peek_queue(readyQueue)].timeRemaining)		// STCF preemption - the next process in the ready queue should have the lowest time remaining
		{
			if(currentTime % interval == 0)											// Print out the info for each interval
			{
				fprintf(outFile, "t = %d\n", currentTime);
				fprintf(outFile, "CPU: Preempting Process %d (Remaining CPU Burst = %d); Loading Process %d (CPU Burst = %d)\n", activeProcess, processes[activeProcess].timeRemaining, peek_queue(readyQueue), processes[peek_queue(readyQueue)].timeRemaining);
				fprintf(outFile, "Ready Queue: ");
				print_queue(outFile, readyQueue);
				fprintf(outFile, "\n");
			}

			processes[activeProcess].status = WAITING;											// Set preempted process to waiting
			activeProcess = dequeue_process(readyQueue);										// Load in the next process
			processes[activeProcess].status = RUNNING;											// Set new process as running

			if(processes[activeProcess].timeRemaining == processes[activeProcess].burstTime)
				processes[activeProcess].startTime = currentTime;								// Set start time for new process, not for a preempted one

			processes[activeProcess].lastStart = currentTime;									// Set last time process was started
			processes[activeProcess].timeRemaining--;
			summary->contextSwitches++;															// Increment number of context switches
			add_to_linked_list(summary->processSequence, activeProcess);						// Add process to sequence list
		}

		else if(algo == RR && !is_queue_empty(readyQueue) && currentTime - processes[activeProcess].lastStart >= QUANTUM)		// Round robin preemption - when the process has run for as long  as (or longer than) the quantum
		{
			if(currentTime % interval == 0)														// Print out the info for each interval
			{
				fprintf(outFile, "t = %d\n", currentTime);
				fprintf(outFile, "CPU: Preempting Process %d (Remaining CPU Burst = %d); Loading Process %d (CPU Burst = %d)\n", activeProcess, processes[activeProcess].timeRemaining, peek_queue(readyQueue), processes[peek_queue(readyQueue)].timeRemaining);
				fprintf(outFile, "Ready Queue: ");
				print_queue(outFile, readyQueue);
				fprintf(outFile, "\n");
			}
			processes[activeProcess].status = WAITING;											// Set preempted process to waiting
			activeProcess = dequeue_process(readyQueue);										// Load in next process
			processes[activeProcess].status = RUNNING;											// Set new process as running

			if(processes[activeProcess].timeRemaining == processes[activeProcess].burstTime)
				processes[activeProcess].startTime = currentTime;								// Set start time for new process, not for a preempted one

			processes[activeProcess].lastStart = currentTime;									// Set last time process was started
			processes[activeProcess].timeRemaining--;
			summary->contextSwitches++;															// Increment number of context switches
			add_to_linked_list(summary->processSequence, activeProcess);						// Add process to sequence list
		}

		else																					// Simple process execution, no finishing of a process, no loading of a new process, no preemption
		{
			if(currentTime % interval == 0)														// Print out the info for each interval
			{
				fprintf(outFile, "t = %d\n", currentTime);
				fprintf(outFile, "CPU: Running Process %d (Remaining CPU Burst = %d)\n", activeProcess, processes[activeProcess].timeRemaining);
				fprintf(outFile, "Ready Queue: ");
				print_queue(outFile, readyQueue);
				fprintf(outFile, "\n");
			}
			processes[activeProcess].timeRemaining--;										// Decrement time remaining for currently running process
		}

		currentTime++;																		// Increment time to progress simulation
	}

	for(int i = 0; i < numOfProcesses; i++)													// Calculate the wait and turnaround times for each process
	{
		processes[i].turnaroundTime = processes[i].endTime - processes[i].arrivalTime;
		processes[i].waitTime = processes[i].turnaroundTime - processes[i].burstTime;

		summary->avgTurnaroundTime += processes[i].turnaroundTime;							// Sum up wait and turnaround times for averaging purposes
		summary->avgWaitTime += processes[i].waitTime;
	}

	summary->avgTurnaroundTime /= numOfProcesses;
	summary->avgWaitTime /= numOfProcesses;													// Calculate the average wait and turnaroudn time for current algorithm

	destroy_queue(readyQueue);
	
	return summary;
}

void print_results(FILE *outFile, Process *processes, int numOfProcesses, Info **summaries, Algorithm algo)
{
	fprintf(outFile, "\n***************************************************\n");						// Print out title for summary after each algorithm simulation

	switch(algo)
	{
		case FCFS:
			fprintf(outFile, "FCFS Summary (WT = Wait Time, TT = Turnaround Time)\n\n");
			break;

		case SJF:
			fprintf(outFile, "SJF Summary (WT = Wait Time, TT = Turnaround Time)\n\n");
			break;

		case STCF:
			fprintf(outFile, "STCF Summary (WT = Wait Time, TT = Turnaround Time)\n\n");
			break;

		case RR:
			fprintf(outFile, "Round Robin Summary (WT = Wait Time, TT = Turnaround Time)\n\n");
			break;

		case NPP:
			fprintf(outFile, "Priority Summary (WT = Wait Time, TT = Turnaround Time)\n\n");
			break;
	}

	fprintf(outFile, "PID\tWT\tTT\n");																					// Table Header

	for(int i = 0; i < numOfProcesses; i++)
		fprintf(outFile, "%d\t%d\t%d\n", i, processes[i].waitTime, processes[i].turnaroundTime);						// Table Contents -- process number, wait time, and turnaround time

	fprintf(outFile, "AVG\t%4.2f\t%4.2f\n\n", summaries[algo]->avgWaitTime, summaries[algo]->avgTurnaroundTime);		// Averages -- average wait time, average turnaround time

	fprintf(outFile, "Process Sequence: ");																				// Process Sequence - the sequence in which the processes were executed
	print_linked_list(outFile, summaries[algo]->processSequence);

	fprintf(outFile, "Context Switches: %d\n\n\n", summaries[algo]->contextSwitches);									// Number of context switches in algorithm
}


void print_overall_results(FILE *outFile, Info **summaries)
{
	int wt[5] = { 0, 1, 2, 3, 4 };
	int tt[5] = { 0, 1, 2, 3, 4 };
	int cs[5] = { 0, 1, 2, 3, 4 };
	int temp;

	for(int i = 0; i < 4; i++)																		// Simple bubble sort to sort wait times, turnaround times, and number of contexts switches in ascending order
	{
		for(int j = 0; j < 4 - i; j++)
		{
			if(summaries[wt[j]]->avgWaitTime > summaries[wt[j + 1]]->avgWaitTime)
			{
				temp = wt[j];
				wt[j] = wt[j + 1];
				wt[j + 1] = temp;
			}
			if(summaries[tt[j]]->avgTurnaroundTime > summaries[tt[j + 1]]->avgTurnaroundTime)
			{
				temp = tt[j];
				tt[j] = tt[j + 1];
				tt[j + 1] = temp;
			}
			if(summaries[cs[j]]->contextSwitches > summaries[cs[j + 1]]->contextSwitches)
			{
				temp = cs[j];
				cs[j] = cs[j + 1];
				cs[j + 1] = temp;
			}
		}
	}

	fprintf(outFile, "***** OVERALL SUMMARY *****\n\n");											// Summary title and header

	fprintf(outFile, "Wait Time Comparison\n");														// Wait times from lowest to highest

	for(int i = 0; i < 5; i++)
	{
		switch(wt[i])
		{
			case FCFS:
				fprintf(outFile, "FCFS\t\t%4.2f\n", summaries[0]->avgWaitTime);
				break;
			case SJF:
				fprintf(outFile, "SJF\t\t%4.2f\n", summaries[1]->avgWaitTime);
				break;
			case STCF:
				fprintf(outFile, "STCF\t\t%4.2f\n", summaries[2]->avgWaitTime);
				break;
			case RR:
				fprintf(outFile, "Round Robin\t%4.2f\n", summaries[3]->avgWaitTime);
				break;
			case NPP:
				fprintf(outFile, "Priority\t%4.2f\n", summaries[4]->avgWaitTime);
		}
	}

	fprintf(outFile, "\nTurnaround Time Comparison\n");												// Turnaround times from lowest to highest

	for(int i = 0; i < 5; i++)
	{
		switch(tt[i])
		{
			case FCFS:
				fprintf(outFile, "FCFS\t\t%4.2f\n", summaries[0]->avgTurnaroundTime);
				break;
			case SJF:
				fprintf(outFile, "SJF\t\t%4.2f\n", summaries[1]->avgTurnaroundTime);
				break;
			case STCF:
				fprintf(outFile, "STCF\t\t%4.2f\n", summaries[2]->avgTurnaroundTime);
				break;
			case RR:
				fprintf(outFile, "Round Robin\t%4.2f\n", summaries[3]->avgTurnaroundTime);
				break;
			case NPP:
				fprintf(outFile, "Priority\t%4.2f\n", summaries[4]->avgTurnaroundTime);
				break;
		}
	}
	

	fprintf(outFile, "\nContext Switch Comparison\n");												// Number of context switches from lowest to highest

	for(int i = 0; i < 5; i++)
	{
		switch(cs[i])
		{
			case FCFS:
				fprintf(outFile, "FCFS\t\t%d\n", summaries[0]->contextSwitches);
				break;
			case SJF:
				fprintf(outFile, "SJF\t\t%d\n", summaries[1]->contextSwitches);
				break;
			case STCF:
				fprintf(outFile, "STCF\t\t%d\n", summaries[2]->contextSwitches);
				break;
			case RR:
				fprintf(outFile, "Round Robin\t%d\n", summaries[3]->contextSwitches);
				break;
			case NPP:
				fprintf(outFile, "Priority\t%d\n", summaries[4]->contextSwitches);
				break;
		}
	}
}