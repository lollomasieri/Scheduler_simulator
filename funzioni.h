#ifndef FUNZIONI_H
#define FUNZIONI_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <malloc.h>

#include "strutture.h"

void print_help(FILE* stream, int exit_code);

void fill_structure(char* input_filename);

int read_jobs(char* input_filename, struct job *jobs, struct istruzione *lista_istruzioni);
	
FILE* open_file(char *output_filename);

void write_log(FILE *fd, int core, int clock, int job_id, STATI stato_job);
	
int random_number(int max);

#endif
