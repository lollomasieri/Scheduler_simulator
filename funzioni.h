#ifndef FUNZIONI_H
#define FUNZIONI_H

#include "strutture.h"

int read_jobs(char* input_filename, struct job *jobs, struct istruzione *lista_istruzioni);
	
FILE* open_file(const char *output_filename);

void cambiaStato(FILE *fd, int core, int clock, struct job* job_p, STATI stato_new, pthread_mutex_t *mutex_file_output);

void fork_error();
	
int random_number(int max);

void swap(struct job array[], int id1, int id2);

void quickSort(struct job array[], int begin, int end);

void check_error_thread(int err);
	
void esegui(unsigned long *clock, struct job *exec_job);
	
void esegui_quantum(unsigned long *clock, struct job *exec_job, int quantum);	

#endif
