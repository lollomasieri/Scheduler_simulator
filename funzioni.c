#include "funzioni.h"

int read_jobs(char* input_filename, struct job *jobs, struct istruzione *lista_istruzioni){
  FILE *fd;
  char linea[20];
    
 	if(jobs == NULL || lista_istruzioni == NULL)
    {
        printf("Errore. Impossibile allocare memoria\n");
        exit(1);
    }

	// apre il file 
  fd=fopen(input_filename, "r");
  if( fd==NULL ) {
    perror("Errore in apertura del file");
    exit(1);
  }
	
	
	char tipo_linea;
	int contatore_job = 0;
	int contatore_istruzioni = 0;
	long contatore_istruzioni_totali = 0;
	int somma_lunghezza_istruzioni = 0;
	
	// legge ogni riga 
	while(fgets(linea, sizeof(linea), fd) != NULL) {
		if (linea[0] == 'j'){
			if(contatore_job != 0){
				jobs[contatore_job-1].num_istruzioni = contatore_istruzioni;
				jobs[contatore_job-1].somma_lunghezza_istruzioni = somma_lunghezza_istruzioni;			
			}
			sscanf(linea, "%c,%d,%lud", &tipo_linea, &jobs[contatore_job].id, &jobs[contatore_job].arrival_time);
			contatore_job++;	
			contatore_istruzioni=0;		
			somma_lunghezza_istruzioni = 0;
		}		  	
		else if(linea[0] == 'i'){		
			sscanf(linea, "%c,%d,%d,%d", &tipo_linea,(int *) &lista_istruzioni[contatore_istruzioni_totali].type_flag, &lista_istruzioni[contatore_istruzioni_totali].lenght, &lista_istruzioni[contatore_istruzioni_totali].IO_max);
			if(contatore_istruzioni == 0){				
				jobs[contatore_job-1].instr_list = &lista_istruzioni[contatore_istruzioni_totali];			
			}
			else {
				lista_istruzioni[contatore_istruzioni_totali-1].successiva = &lista_istruzioni[contatore_istruzioni_totali];
			}
			somma_lunghezza_istruzioni += lista_istruzioni[contatore_istruzioni_totali].lenght;
			contatore_istruzioni++;
			contatore_istruzioni_totali++;			
		}
	}
	//Ultimo job	
	jobs[contatore_job-1].somma_lunghezza_istruzioni = somma_lunghezza_istruzioni;
	jobs[contatore_job-1].num_istruzioni = contatore_istruzioni; 

	
	// chiude il file 
	fclose(fd);
  
	return contatore_job;
}
	
FILE* open_file(const char *output_filename){
	FILE *fd;
	
	// apre il file in scrittura 
	fd=fopen(output_filename, "w"); //a --> agginge, w --> sovrascrive
	if( fd==NULL ) {
		perror("Errore in apertura del file di output");
		exit(1);
	}
	
	return fd;
}

void cambiaStato(FILE *fd, int core, int clock, struct job* job_p, STATI stato_new, pthread_mutex_t *mutex_file_output){	
	//Imposto nuovo stato
	job_p->stato = stato_new;
	
	const char *stato;
	switch(job_p->stato){
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
	pthread_mutex_lock(mutex_file_output); 
	// scrive la nuova linea
	fprintf(fd, "core%d,%d,%d,%s\n", core, clock, job_p->id, stato);
	pthread_mutex_unlock(mutex_file_output); 	
}
	
void fork_error(){
	extern int errno;
	
	fprintf(stderr, "Value of errno: %d\n", errno);
	fprintf(stderr, "Error during fork: %s\n", strerror(errno));
	perror("Error printed by perror");
	exit(EX_OSERR); // https://goo.gl/Y40V0r
}
	
int random_num(int max){
	srand(time(NULL));
	return 1 + rand() % max;
}

void swap(struct job array[], int id1, int id2){
	struct job temp = array[id1];
	array[id1] = array[id2];
	array[id2] = temp;
}

void quickSort(struct job *array, int begin, int end) {
    int pivot, l, r; 
    if (end > begin) {
       pivot = array[begin].somma_lunghezza_istruzioni;
       l = begin + 1;
       r = end+1;
       while(l < r)
          if (array[l].somma_lunghezza_istruzioni < pivot) 
             l++;
          else {
             r--;
             swap(array, l, r); 
          }
       l--;
       swap(array, begin, l);
       quickSort(array, begin, l);
       quickSort(array, r, end);
    }
}

void check_error_thread(int err){
	if (err != 0) {
		fprintf(stderr, "Problem with thread. Reason: '%s'", strerror(err));
		exit(EX_OSERR);
	}
}

void esegui(unsigned long *clock, struct job *puntatore_job){
	while(1){
		if(puntatore_job->num_istruzioni == 0){ //Istruzioni finite, job terminato
			 puntatore_job->stato = EXIT;
			 break;
		 }
		if(puntatore_job->instr_list->type_flag == 1){ //Istruzione bloccante			
			puntatore_job->stato = BLOCKED;
			puntatore_job->instr_list->type_flag = 0;
			puntatore_job->arrival_time = random_num(puntatore_job->instr_list->IO_max) + *clock;				
			break;
		}
		else{//Eseguo istruzione
			*clock = *clock + puntatore_job->instr_list->lenght;
			puntatore_job->instr_list = puntatore_job->instr_list->successiva;
			puntatore_job->num_istruzioni--;
		}
	}
}

void esegui_quantum(unsigned long *clock, struct job *puntatore_job, int quantum){	
	while(1){
		if(puntatore_job->num_istruzioni == 1){ //Istruzioni finite, job terminato
			 puntatore_job->stato = EXIT;
			 break;
		}
		if(puntatore_job->instr_list->type_flag == 1){ //Istruzione bloccante
			puntatore_job->stato = BLOCKED;
			puntatore_job->instr_list->type_flag = 0;
			puntatore_job->arrival_time = random_num(puntatore_job->instr_list->IO_max) + *clock;				
			break;
		}
		//Eseguo istruzione
		if (puntatore_job->instr_list->lenght <= quantum){			
			*clock = *clock + puntatore_job->instr_list->lenght;
			puntatore_job->instr_list = puntatore_job->instr_list->successiva;
			puntatore_job->num_istruzioni--;
			quantum = quantum - puntatore_job->instr_list->lenght;
		}
		else{ //sforo il quanto di tempo
			puntatore_job->stato = BLOCKED;
			*clock = *clock + quantum;
			puntatore_job->instr_list->lenght = puntatore_job->instr_list->lenght - quantum;
			break;
		}			
	}
}
