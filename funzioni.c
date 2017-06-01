#include "funzioni.h"

void fill_structure(char* input_filename){
	//TODO
}

void write_output(int core, int clock, int job_id, enum stato stato_attuale){
	//TODO
}
	
int random_num(int max){
	srand(time(NULL));
	return 1 + rand() % max;
}
