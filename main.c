#include "scheduler.h"

#define MAX_JOBS 2048

const char* program_name;
const bool debug = false;

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
    
    //Strutture per il passaggio dati agli scheduler
	struct params_sched_preemptive params_preemptive;
	struct params_sched_not_preemptive params_not_preemptive;
		
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
			params_preemptive.output_preemption_filename = argv[i+1];
			controllo++;
		}
		//Controllo se è presente il paramentro -on
		if(!strcmp(argv[i], "-on") || !strcmp(argv[i], "--output-no-preemption")){			
			params_not_preemptive.output_no_preemption_filename = argv[i+1];
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
					if(optarg[1] != '\0'){
						quantum = ((int) optarg[0] - 48) * 10 + ((int) optarg[1] - 48); //Conversione ASCII -> dec
					}
					else{
						quantum = (int) optarg[0] - 48;
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
	 
	//Lettura file di input
	struct job *jobs;  
	jobs = (struct job *) malloc(MAX_JOBS*sizeof(struct job));
	struct istruzione *lista_istruzioni;
	lista_istruzioni = (struct istruzione *) malloc(2000000*sizeof(struct istruzione));
      
	int num_job = read_jobs(input_filename, jobs, lista_istruzioni);
    if(debug) printf("Jobs da schedulare: %d\n", num_job);
	
	
	//Avvio processi scheduler		
	int child_status;
    pid_t sched_preemptive_pid;
	pid_t sched_not_preemptive_pid;

    if(debug) printf("Il processo master ha il PID %d.\n", getpid());
    
    sched_preemptive_pid = fork();
	
	if (sched_preemptive_pid == -1)	{
		fork_error();
	}	 
    if(sched_preemptive_pid != 0) { //master
        sched_not_preemptive_pid = fork();
        if (sched_not_preemptive_pid == -1)	{
			fork_error();
		}	 
        
        if(sched_not_preemptive_pid != 0) { //master			
			waitpid(sched_not_preemptive_pid,&child_status,0); // waits for the child to die 
			if(WIFEXITED(child_status)) {
				if(debug) printf("NOT Preemptive exited normally with exit code %d.\n", WEXITSTATUS(child_status));
			}
			else {
				printf("NOT Preemptive process exited abnormally.\n");
			}
			
			waitpid(sched_preemptive_pid,&child_status,0); // waits for the child to die 
			 if(WIFEXITED(child_status) ) {
				if(debug) printf("Preemptive exited normally with exit code %d.\n", WEXITSTATUS(child_status));
			}
			else {
				printf("Preemptive process exited abnormally.\n");
			}
						
			if(debug) printf("Programma terminato.\n");			
        }        
        else{ //scheduler not preemptive	
			if(debug) printf("Scheduler not preemptive avviato con PID %d.\n", getpid());				
			params_not_preemptive.jobs = jobs;
			params_not_preemptive.lista_istruzioni = lista_istruzioni;			
			scheduler_not_preemptive(params_not_preemptive);
		}
    }
    else{ //scheduler preemptive		
		if(debug) printf("Scheduler preemptive avviato con PID %d.\n", getpid());		
		params_preemptive.jobs = jobs;
		params_preemptive.lista_istruzioni = lista_istruzioni;
		params_preemptive.quantum = quantum;
		sheduler_preemptive(params_preemptive);
	}

	//Deallocazione memoria 
	free(jobs);
	free(lista_istruzioni);
	
    return 0;
}
