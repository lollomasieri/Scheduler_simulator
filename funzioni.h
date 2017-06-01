#ifndef FUNZIONI_H
#define FUNZIONI_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

void fill_structure(char* input_filename);

void write_output(int core, int clock, int job_id, stato stato_attuale);

#endif
