#ifndef STRUTTURE_H
#define STRUTTRE_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <stdbool.h>

typedef enum {NEW, READY, RUNNING, BLOCKED, EXIT} STATI;

struct istruzione{
	bool type_flag;
	int lenght;
	int IO_max;	
	struct istruzione *successiva; // puntatore alla prossima istruzione
};

struct job{
	int id;
	int arrival_time;
	struct istruzione *instr_list; //puntatore alla prima istruzione
	STATI stato;
	int num_istruzioni;
};

struct params_sched_preemptive{
	const char* output_preemption_filename;
	int quantum;
	struct job *jobs;
	struct istruzione *lista_istruzioni;
};

struct params_sched_not_preemptive{
	const char* output_no_preemption_filename;
	struct job *jobs;
	struct istruzione *lista_istruzioni;
};

#endif
