#ifndef STRUTTURE_H
#define STRUTTRE_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <stdbool.h>

struct istruzione{
	bool type_flag;
	int lenght;
	int IO_max;
	// puntatore alla prossima istruzione
	struct istruzione *successiva;
};

struct job {
	int id;
	int arrival_time;
	int *instr_list;
	char *state;
};

enum stato{new, ready, running, blocked, exit};

#endif
