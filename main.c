#include "funzioni.h"

#define SCHEDULER_PREEMPTION "./scheduler_preemption"
#define SCHEDULER_NO_PREEMPTION "./scheduler_no_preemption"

const char* program_name;
bool debug = true;

int main(int argc, char* argv[]){

	if(debug){
		argc = 9;
		argv[1]="-op";
		argv[2]="output_preemption.csv";
		argv[3]="-on";
		argv[4]="output_no_preemption.csv";
		argv[5]="-i";
		argv[6]="input_jobs/1_jobs.dat";
		argv[7]="-q";
		argv[8]="1";
		fprintf(stdout, "numero argomenti: %d\n", argc);
	}

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
	char* input_filename = NULL;
	int quantum = 0;

    program_name = argv[0];
	
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
			output_preemption_filename = argv[i+1];
			controllo++;
		}
		//Controllo se è presente il paramentro -on
		if(!strcmp(argv[i], "-on") || !strcmp(argv[i], "--output-no-preemption")){			
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
					quantum = ((int) optarg[0]) - 48; //Conversione ASCII -> dec
					if (quantum < 0 || quantum > 10){
						fprintf(stderr, "Valore del parametro -q errato!\n");
						print_help(stderr, -1);
					}
					break;
				
            case '?':
                print_help(stderr, 1);

            case -1:
                break;

            default:
                abort();
        }
    }while(next_option != -1);

	if(debug){
		fprintf(stdout, "-op %s\n", output_preemption_filename);
		fprintf(stdout, "-on %s\n", output_no_preemption_filename);	
		fprintf(stdout, "-i %s\n", input_filename);
		fprintf(stdout, "-q %d\n", quantum);	
	 }

	 
	 //Lettura file di input
	 struct job *jobs;  
	 jobs = (struct job *) malloc(2048*sizeof(struct job));
	 struct istruzione *lista_istruzioni;
	lista_istruzioni = (struct istruzione *) malloc(2000000*sizeof(struct istruzione));
      
  int num_job = read_jobs(input_filename, jobs, lista_istruzioni);
    printf("\njob letti da schedulare: %d\n", num_job);
	
	free(jobs);
	free(lista_istruzioni);
	

	
	//Avvio processi
	system(SCHEDULER_PREEMPTION);
	system(SCHEDULER_NO_PREEMPTION);

    return 0;
}

