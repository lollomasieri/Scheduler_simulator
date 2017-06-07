#ifndef FUNZIONI_H
#define FUNZIONI_H

#include "strutture.h"

void fill_structure(char* input_filename);

int read_jobs(char* input_filename, struct job *jobs, struct istruzione *lista_istruzioni);
	
FILE* open_file(char *output_filename);

void write_log(FILE *fd, int core, int clock, int job_id, STATI stato_job);

void fork_error();
	
int random_number(int max);

#endif
