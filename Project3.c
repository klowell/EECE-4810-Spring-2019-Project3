/*************************************************************************************
File Name: Project3.c

Objective: Simulate different CPU scheduling algorithms.

Created By: Kristopher Lowell
Date Created: 4/20/2019

Time to Complete: Approx 24 hours, accumulative

File History:
	Submitted - KCL - 4/22/2019
*************************************************************************************/

#include "sched_sim.h"

const int QUANTUM = 2;

const int NUM_OF_ALGORITHMS = 5;

int main(int argc, char* argv[])
{
	FILE *inFile, *outFile;
	int numOfProcesses, interval;
	Process *processes;
	Info **summaries = (Info **) malloc(sizeof(Info *) * NUM_OF_ALGORITHMS);

	if (argc < 4)																		// Simple detection if not enough command line arguments were sent
	{
		printf("ERROR: Incorrect amount of command line inputs.\n");
		return 0;
	}

	inFile = fopen(argv[1], "r");														// Open input file for reading
	outFile = fopen(argv[2], "w");														// Open output file for writing
	interval = atoi(argv[3]);															// Convert last argument to integer to determine interval in which to print the simulation

	numOfProcesses = find_num_of_processes(inFile);										// Find how many processes are detailed in the input file

	processes =	load_processes(inFile, numOfProcesses);									// Load processes' information from input file and store in array of process objects

	for(int i = 0; i < NUM_OF_ALGORITHMS; i++)
	{
		summaries[i] = simulate(outFile, interval, processes, numOfProcesses, i);		// Simulate first-come-first-served (FCFS) algorithm
		print_results(outFile, processes, numOfProcesses, summaries, i);				// Print FCFS results
	}

	print_overall_results(outFile, summaries);											// Sort and print results from all algorithms
	
	fclose(inFile);																		// Close the files
	fclose(outFile);

	free(processes);																	// Destroy/deallocate memory for array of process objects in order to avoid memory leaks

	for(int i = 0; i < NUM_OF_ALGORITHMS; i++)											// Destroy/deallocate memory for array of summary objects in order to avoid memory leaks
		destroy_info(summaries[i]);
	free(summaries);																	// Destroy/dellocate memory for pointer for summary objects

	processes = NULL;
	summaries = NULL;																	// Set pointers to NULL to avoid dangling pointer

	return 0;
}