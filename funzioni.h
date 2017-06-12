#ifndef FUNZIONI_H
#define FUNZIONI_H

#include "strutture.h"

void fill_structure(char* input_filename);

int read_jobs(char* input_filename, struct job *jobs, struct istruzione *lista_istruzioni);
	
FILE* open_file(const char *output_filename);

void write_log(FILE *fd, int core, int clock, int job_id, STATI stato_job, pthread_mutex_t *mutex_file_output);

void fork_error();
	
int random_number(int max);

void swap(struct job array[], int id1, int id2);

void quickSort(struct job array[], int begin, int end);

void check_error_thread(int err);
	
void esegui(unsigned long *clock, struct job *exec_job);
		
#endif
