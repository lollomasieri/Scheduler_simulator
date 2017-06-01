#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "strutture.h"
#include "funzioni.h"

#define SCHEDULER_PREEMPTION "./scheduler_preemption"
#define SCHEDULER_NO_PREEMPTION "./scheduler_no_preemption"

const char* program_name;
bool debug = false;

void print_help(FILE* stream, int exit_code){
    fprintf(stream, "Usage: %s -op [outputfile] -on [outputfile] -i [inputfile] -q [quantum]\n", program_name);
    fprintf(stream,
        " -h  | --help         		Display this help information.\n"
        " -op | --output-preemption	Il file di output con i risultati dello scheduler con preemption.\n"
        " -on | --output-no-preemption	Il file di output con i risultati dello scheduler senza preemption.\n"
		" -i  | --input			Il file di input contenente la lista dei job.\n"
		" -q  | --quantum 		La durata di un quanto di tempo (misurato in cicli di clock) sullo scheduler con preemption.\n"
		);

   exit(exit_code);
}

int main(int argc, char* argv[]){
    int next_option;

    const char* const short_options = "ho:i:q:";

    const struct option long_options[] = {
        { "help",       0, NULL, 'h' },
        { "input",    1, NULL, 'i' },
        { "quantum",    1, NULL, 'q' },
        { NULL,         0, NULL, 0   }
    };

    const char* output_preemption_filename = NULL;
	const char* output_no_preemption_filename = NULL;
	const char* input_filename = NULL;
	int quantum = 0;

    program_name = argv[0];
	
	if(debug) fprintf(stdout, "numero argomenti: %d\n", argc);
	
	//Se parametri < 4
	if (argc - 1 != 8){
		if (argc == 1)
			fprintf(stderr, "No arguments!\n");
		print_help(stderr, -1);
	}

	//Acquisizione parametri -op & -on
	int controllo = 0;
	for(int i = 1; i < argc ; i+=2){		
		//Controllo se è presente il paramentro -op
		if(!strcmp(argv[i], "-op") || !strcmp(argv[i], "--output-preemption")){
			if(debug) fprintf(stdout, "-op OK\n");
			output_preemption_filename = argv[i+1];
			controllo++;
		}
		//Controllo se è presente il paramentro -on
		if(!strcmp(argv[i], "-on") || !strcmp(argv[i], "--output-no-preemption")){
			if(debug) fprintf(stdout, "-on OK\n");
			output_no_preemption_filename = argv[i+1];
			controllo++;
		}
	}
	if(controllo!=2){
		print_help(stderr, -1);
	}

	//Acquisizione altri paramatri 
    do{
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);
       
		switch(next_option){
            case 'h':
                print_help(stdout, 0);
                
			case 'o':
				break;
				
			case 'i':
					input_filename = optarg;
					break;	
					
			case 'q':
					quantum = (int) optarg;
					break;
				
            case '?':
                print_help(stderr, 1);

            case -1:
                break;

            default:
                abort();
        }
    }while(next_option != -1);

	//Avvio processi
	system(SCHEDULER_PREEMPTION);
	system(SCHEDULER_NO_PREEMPTION);
	
    return 0;
}

