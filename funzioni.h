#ifndef FUNZIONI_H
#define FUNZIONI_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <time.h>

void fill_structure(char* input_filename);

void write_output(int core, int clock, int job_id, enum stato stato_attuale);

int random_number(int max);

#endif
