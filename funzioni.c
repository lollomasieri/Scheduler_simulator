#include "funzioni.h"

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

void fill_structure(char* input_filename){
	//TODO
}

int read_jobs(char* input_filename, struct job *jobs, struct istruzione *lista_istruzioni){
  FILE *fd;
  char linea[20];
    
 	if(jobs == NULL || lista_istruzioni == NULL)
    {
        printf("Errore. Impossibile allocare memoria\n");
        exit(1);
    }

		/* apre il file */
  fd=fopen(input_filename, "r");
  if( fd==NULL ) {
    perror("Errore in apertura del file");
    exit(1);
  }

	/* legge e stampa ogni riga */
	char tipo_linea;
	int contatore_job = 0;
	int contatore_istruzioni;
	long contatore_istruzioni_totali = 0;

  while(fgets(linea, sizeof(linea), fd) != NULL) {

   if (linea[0] == 'j'){
	   
	      if(contatore_job != 0){
			  jobs[contatore_job-1].num_istruzioni = contatore_istruzioni;
			printf("job: %d, numero di istruzioni: %d\n", contatore_job-1, contatore_istruzioni);
		}
	   sscanf(linea, "%c,%d,%d", &tipo_linea, &jobs[contatore_job].id, &jobs[contatore_job].arrival_time);
		contatore_job++;	
		contatore_istruzioni=0;		
	}		     
	
	
	else if(linea[0] == 'i'){
		
		sscanf(linea, "%c,%d,%d,%d", &tipo_linea, &lista_istruzioni[contatore_istruzioni].type_flag, &lista_istruzioni[contatore_istruzioni].lenght, &lista_istruzioni[contatore_istruzioni].IO_max);
		if(contatore_istruzioni == 0){
				
				jobs[contatore_job].instr_list = &lista_istruzioni[contatore_istruzioni];
			}
			else {
				lista_istruzioni[contatore_istruzioni-1].successiva = &lista_istruzioni[contatore_istruzioni];
			}
				contatore_istruzioni++;
				contatore_istruzioni_totali++;
		}
}

printf("job: %d, numero di istruzioni: %d\n", contatore_job-1, contatore_istruzioni);
jobs[contatore_job-1].num_istruzioni = contatore_istruzioni; //Ultimo job

   printf("\njob letti: %d, istruzioni totali: %ld\n",contatore_job, contatore_istruzioni_totali );

  
		/* chiude il file */
  fclose(fd);
  
  return contatore_job;
}
	
FILE* open_file(char *output_filename){
	FILE *fd;
	
	/* apre il file in scrittura */
	fd=fopen(output_filename, "w"); //a --> agginge, w --> sovrascrive
	if( fd==NULL ) {
		perror("Errore in apertura del file di output");
		exit(1);
	}
	
	return fd;
}

void write_log(FILE *fd, int core, int clock, int job_id, STATI stato_job){	
	const char *stato;
	switch(stato_job){
		case 0:
			stato = "new";
			break;
		case 1:
			stato = "ready";
			break;
		case 2:
			stato = "running";
			break;
		case 3: 
			stato = "blocked";
			break;
		case 4: 
			stato = "exit";
			break;
		default:
			stato = NULL;
	}
	
	/* scrive la nuova linea*/
	fprintf(fd, "core%d,%d,%d,%s\n", core, clock, job_id, stato);
}
	
int random_num(int max){
	srand(time(NULL));
	return 1 + rand() % max;
}
