#include "scheduler.h"

#define MAX_JOBS 2048

/* def.mutex condivisi tra threads */
pthread_mutex_t mutex_coda_jobs; 
pthread_mutex_t mutex_file_output; 

void* core_preemptive(void* parameters){
	/* Cast pointer to the right type. */
    struct params_sched_preemptive* params = (struct params_sched_preemptive*) parameters;
	
    printf("core preemptive avviato\n");
    //TODO
    /*
    int clock=0;
    int jobs_completati = 0;
    int count = 0;
    struct istruzione istr_corrente;
    while(jobs_completati < MAX_JOBS){
		if(clock < params->jobs[count].arrival_time){
			clock++;
			continue;
		}
		istr_corrente = *(params->jobs[count].instr_list);
		if(istr_corrente.type_flag == 1){
			random_num(istr_corrente.IO_max);
		
	}
    
    */
    
    
    
    //printf("id job 200: %d\n", params->jobs[200].id);
    return NULL;
}

int sheduler_preemptive(struct params_sched_preemptive params){
	printf("Io sono lo scheduler con preemption\n");
	
	/* Strategia di funzionamento:
	 * 
	 * Lo scheduler NON deve mandare in esecuzione un job se: clock(core) < clock(job)
	 * 
	 * I job vengono eseguiti nell'ordine di arrivo
	 * Se l'esecuzione dura più del "quanto di tempo" stabilito (input utente)
	 * il job viene messo alla fine della coda dei job in attesa
	 * l'esecuzione procede con il successivo job in attesa
	 * I job vengono eseguiti in parallelo sui due thread
	 * 
	 * Finita l'esecuzione di un job, aggiorno lo stato e scrivo il log su file
	 * 
	 * Se arriva un'istruzione con una operazione bloccante
	 * viene sospesa l'istruzione
	 * e viene schedulato un altro job
	 * 
	 * Lo scheduler termina quando non ci sono più job da eseguire
	 */
	
	 pthread_t thread0_id;
	 pthread_t thread1_id;
	 
    /* Create a new thread.
     * The new thread will run the core function. 
     */     
    int err;
    //Avvio primo thread
    err = pthread_create(&thread0_id, NULL, &core_preemptive, &params);
	check_error_thread(err);
	//Avvio secondo thread
	err = pthread_create(&thread1_id, NULL, &core_preemptive, &params);
	check_error_thread(err);
	//Termino primo thread
    err = pthread_join(thread0_id, NULL);
	check_error_thread(err);
	//Termino secondo thread
    err = pthread_join(thread1_id, NULL);
	check_error_thread(err);
	 
	return 0;
}

void* core_not_preemptive(void* parameters){
	struct params_sched_not_preemptive* params = (struct params_sched_not_preemptive*) parameters;
	
	printf("core%d not preemptive avviato\n", params->core);
    //TODO
    
    unsigned long clock = 0;

	bool flag = 0; 
    int jobs_rimanenti = MAX_JOBS;
    STATI stato_old;
 
	 
	/*DEBUG
    for(int i=0; i<10; i++){
		printf("\nAAA \n", params->jobs[i].instr_list->lenght);
	}
	*/
	
    while(jobs_rimanenti > 0){
		//printf("clock%d: %ld\n", params->core, clock);
		jobs_rimanenti = MAX_JOBS; 
		flag = 0; //Viene settato a 1 se durante un ciclo viene eseguito almeno un job
		for (int i = 0; i< MAX_JOBS; i++){
			
			pthread_mutex_lock(&mutex_coda_jobs); //Inizio sessione critica
				
				stato_old = params->jobs[i].stato;
				switch(stato_old){
					case 0: //NEW
						if(clock >= params->jobs[i].arrival_time){
							params->jobs[i].stato = READY;
							flag = 1;						
						}
						break;
					case 1: //READY
						params->jobs[i].stato = RUNNING;
						write_log(params->fd, params->core, clock, params->jobs[i].id, params->jobs[i].stato, &mutex_file_output);
						stato_old = params->jobs[i].stato;	
						esegui(&clock, &(params->jobs[i]));
						break;
					case 2: //RUNNING
						break;
					case 3: //BLOCKED
					
						if(!(clock < params->jobs[i].arrival_time)){
							params->jobs[i].stato = READY;
							write_log(params->fd, params->core, clock, params->jobs[i].id, params->jobs[i].stato, &mutex_file_output);
							stato_old = params->jobs[i].stato;
							esegui(&clock, &(params->jobs[i])); 
						}
						break;
					case 4: //EXIT
						jobs_rimanenti--;
						break;
					default:
						break;
				}
						
			
			if(stato_old != params->jobs[i].stato){				
				//TODO
				//MUTEX!!!
				
				write_log(params->fd, params->core, clock, params->jobs[i].id, params->jobs[i].stato, &mutex_file_output);
				
				
				flag = 1; // --> è stato eseguito almeno 1 job
				break; //rinizia il ciclo for da i = 0
			}
			
			pthread_mutex_unlock(&mutex_coda_jobs); //Fine sessione critica			
			printf("clok:%ld\n", clock);
			
		}
		if(!flag){
			clock++;
		}
		if(jobs_rimanenti < 2048)
			printf("jobs rimanenti: %d\n", jobs_rimanenti);
			
	}

	

    return NULL;
}

int scheduler_not_preemptive(struct params_sched_not_preemptive params){
	printf("Io sono lo scheduler senza preemption\n");
	/* Strategia di funzionamento:
	 * 
	 * Lo scheduler NON deve mandare in esecuzione un job se: clock(core) < clock(job)
	 * 
	 * Per ogni job in coda calcolo la somma della lunghezza di istruzioni da eseguire
	 * Mando in esecuzione al primo thread quello con la somma minore 
	 * Il secondo in "classifica" lo mando al secondo thread
	 * Finita l'esecuzione di un job, aggiorno lo stato e scrivo il log su file
	 * Il thread si libera e gli invio il job successivo 
	 * 
	 * Se arriva un'istruzione con una operazione bloccante
	 * viene sospesa l'istruzione
	 * e viene schedulato un altro job
	 * 
	 * Lo scheduler termina quando non ci sono più job da eseguire
	 */
	 
	
	  /* il mutex e` inizialmente libero: */
	pthread_mutex_init (&mutex_coda_jobs, NULL);
	pthread_mutex_init (&mutex_file_output, NULL);
 
	 //Apertura file
	params.fd = open_file(params.output_no_preemption_filename);
	 
	struct job *jobs = params.jobs;		
	//ordino i jobs
	quickSort(jobs, 0, MAX_JOBS-1);	
		
/*	for (int j=0; j<10; j++){		
		printf("id: %d, arrival_time: %d, somma lunghezza istruzioni: %d\n",jobs[j].id, jobs[j].arrival_time, jobs[j].somma_lunghezza_istruzioni);
	}	
 */	 
	  pthread_t thread0_id;
	  pthread_t thread1_id;
    /* Create a new thread.
     * The new thread will run the core function. 
     */
     
    
     
    struct params_sched_not_preemptive params0 = params;
    params0.core = 0;
    struct params_sched_not_preemptive params1 = params;
    params1.core = 1;
	
    int err;
    //Avvio primo thread    
    err = pthread_create(&thread0_id, NULL, &core_not_preemptive, &params0);
	check_error_thread(err);
	//Avvio secondo thread
	err = pthread_create(&thread1_id, NULL, &core_not_preemptive, &params1);
	check_error_thread(err);
	//Termino primo thread
    err = pthread_join(thread0_id, NULL);
    check_error_thread(err);
	//Termino secondo thread
    err = pthread_join(thread1_id, NULL);
	check_error_thread(err);
    
    //Chiusura file
	fclose(params.fd);
    
	return 0;
}
