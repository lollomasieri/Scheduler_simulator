#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strutture.h"

const char* program_name;

void print_help(FILE* stream, int exit_code)
{
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

int main(int argc, char* argv[])
{
    int next_option;

    const char* const short_options = "hi:q:";

    const struct option long_options[] = {
        { "help",       0, NULL, 'h' },
  //    { "output-preemption",     1, NULL, 'p' },
  //    { "output-no-preemption",    1, NULL, 'n' },
        { "input",    1, NULL, 'i' },
        { "quantum",    1, NULL, 'q' },
        { NULL,         0, NULL, 0   }
    };

    const char* output_preemption_filename = NULL;
	const char* output_no_preemption_filename = NULL;
	const char* input_filename = NULL;
	int quantum = 0;

    program_name = argv[0];
	
	//Se parametri < 4
	if (argc - 1 != 8){
		if (argc == 1)
			fprintf(stderr, "No arguments!\n");
		print_help(stderr, -1);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	int controllo = 0;
	for(int i = 1; i < argc ; i+=2){
		//Controllo se è presente il paramentro -op
		if(!strcmp(argv[i], "-op") || !strcmp(argv[i], "--output-preemption")){
			output_preemption_filename = argv[i+1];
			controllo++;
			break;
		}
		//Controllo se è presente il paramentro -on
		if(!strcmp(argv[i], "-on") || !strcmp(argv[i], "--output-no-preemption")){
			output_no_preemption_filename = argv[i+1];
			controllo++;
			break;
		}
	}
	if(controllo!=2){
		print_help(stderr, -1);
	}

	////////////////////////////////////////////////////////////////////////////////////////

    do{
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);
       
		switch(next_option){
            case 'h':
                print_help(stdout, 0);

  /*          case 'p':
                output_preemption_filename = optarg;
                break;

            case 'n':
                output_no_preemption_filename = optarg;
                break;
*/
	    case 'i':
                input_filename = optarg;
				//TODO
				//Gestione errori durante lettura file
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

    //DA TOGLIERE
	int verbose = 1;
    if(verbose)
    {
        int i;
        for (i = optind; i < argc; i++)
        {
            printf("Argument: %s\n", argv[i]);
        }
    }
    ////////////

    return 0;
}

