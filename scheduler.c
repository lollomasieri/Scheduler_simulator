#include "scheduler.h"

#define MAX_JOBS 2048

/* def.mutex condivisi tra threads */
pthread_mutex_t mutex_coda_jobs_pre; 
pthread_mutex_t mutex_file_output_pre; 
pthread_mutex_t mutex_coda_jobs_nopre; 
pthread_mutex_t mutex_file_output_nopre; 

FILE* f_pre;
FILE* f_nopre;

void* core_preemptive(void* parameters){
	/* Cast pointer to the right type. */
    struct params_sched_preemptive* params = (struct params_sched_preemptive*) parameters;
	
    printf("core%d preemptive avviato\n", params->core);
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
        
    unsigned long clock = 0;

	bool flag = 0; 
    int jobs_rimanenti = MAX_JOBS;

	
    while(jobs_rimanenti > 0){
		//printf("clock%d: %ld\n", params->core, clock);
		jobs_rimanenti = MAX_JOBS; 
		flag = 0; //Viene settato a 1 se durante un ciclo viene eseguito almeno un job
		for (int i = 0; i< MAX_JOBS; i++){
			pthread_mutex_lock(&mutex_coda_jobs_pre); //Inizio sessione critica
			switch(params->jobs[i].stato){
				case 0: //NEW
					if(clock >= params->jobs[i].arrival_time){
						printf("Loggo");
						cambiaStato(f_pre, params->core, clock, &params->jobs[i], READY, &mutex_file_output_pre);
					}
					break;
				case 1: //READY
					cambiaStato(f_pre, params->core, clock, &params->jobs[i], RUNNING, &mutex_file_output_pre);
					flag = 1;
					esegui_quantum(&clock, &(params->jobs[i]), params->quantum);
					cambiaStato(f_pre, params->core, clock, &params->jobs[i], params->jobs[i].stato, &mutex_file_output_pre);
					break;
				case 2: //RUNNING
					break;
				case 3: //BLOCKED					
					if(!(clock < params->jobs[i].arrival_time)){
						cambiaStato(f_pre, params->core, clock, &params->jobs[i], READY, &mutex_file_output_pre);
					}
					break;
				case 4: //EXIT
					jobs_rimanenti--;
					break;
				default:
					break;
			}
			pthread_mutex_unlock(&mutex_coda_jobs_pre); //Fine sessione critica			
		}
		if(!flag)
			clock++;			
	}
    
    
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
	
	//Apertura file
	f_pre = open_file(params.output_preemption_filename);
	
	  /* il mutex e` inizialmente libero: */
	pthread_mutex_init (&mutex_coda_jobs_pre, NULL);
	pthread_mutex_init (&mutex_file_output_pre, NULL);
	
	 pthread_t thread0_id;
	 pthread_t thread1_id;
	 
	 struct params_sched_preemptive params0 = params;
    params0.core = 0;
    struct params_sched_preemptive params1 = params;
    params1.core = 1;
    /* Create a new thread.
     * The new thread will run the core function. 
     */ 
     
     
     printf("job pre:\n");	
	for (int j=0; j<10; j++){		
		printf("id: %d, arrival_time: %ld, somma lunghezza istruzioni: %d\n",params.jobs[j].id, params.jobs[j].arrival_time, params.jobs[j].somma_lunghezza_istruzioni);
	}	
         
    int err;
    //Avvio primo thread
    err = pthread_create(&thread0_id, NULL, &core_preemptive, &params0);
	check_error_thread(err);
	//Avvio secondo thread
	err = pthread_create(&thread1_id, NULL, &core_preemptive, &params1);
	check_error_thread(err);
	//Termino primo thread
    err = pthread_join(thread0_id, NULL);
	check_error_thread(err);
	//Termino secondo thread
    err = pthread_join(thread1_id, NULL);
	check_error_thread(err);
	 
	  //Chiusura file
	fclose(f_pre);
	
	return 0;
}

void* core_not_preemptive(void* parameters){
	struct params_sched_not_preemptive* params = (struct params_sched_not_preemptive*) parameters;
	
	printf("core%d not preemptive avviato\n", params->core);
    //TODO
    
    unsigned long clock = 0;

	bool flag = 0; 
    int jobs_rimanenti = MAX_JOBS;
   // STATI stato_old;
 
	 
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
			pthread_mutex_lock(&mutex_coda_jobs_nopre); //Inizio sessione critica
			switch(params->jobs[i].stato){
				case 0: //NEW
					if(clock >= params->jobs[i].arrival_time){
						cambiaStato(f_nopre, params->core, clock, &params->jobs[i], READY, &mutex_file_output_nopre);
					}
					break;
				case 1: //READY
					cambiaStato(f_nopre, params->core, clock, &params->jobs[i], RUNNING, &mutex_file_output_nopre);
					flag = 1;
					esegui(&clock, &(params->jobs[i]));
					cambiaStato(f_nopre, params->core, clock, &params->jobs[i], params->jobs[i].stato, &mutex_file_output_nopre);
					break;
				case 2: //RUNNING
					break;
				case 3: //BLOCKED					
					if(!(clock < params->jobs[i].arrival_time)){
						cambiaStato(f_nopre, params->core, clock, &params->jobs[i], READY, &mutex_file_output_nopre);
					}
					break;
				case 4: //EXIT
					jobs_rimanenti--;
					break;
				default:
					break;
			}
			pthread_mutex_unlock(&mutex_coda_jobs_nopre); //Fine sessione critica			
		}
		if(!flag)
			clock++;
		
	/*	printf("clock:%ld\n", clock);
		if(jobs_rimanenti < 2048)
			printf("jobs rimanenti: %d\n", jobs_rimanenti);
*/			
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
	 
	//Apertura file
	f_nopre = open_file(params.output_no_preemption_filename);
	
	  /* il mutex e` inizialmente libero: */
	pthread_mutex_init (&mutex_coda_jobs_nopre, NULL);
	pthread_mutex_init (&mutex_file_output_nopre, NULL);
 
	struct job *jobs = params.jobs;		
	//ordino i jobs
	quickSort(jobs, 0, MAX_JOBS-1);	
	
	printf("job not pre:\n");	
	for (int j=0; j<2048; j++){		
		printf("id: %d, arrival_time: %ld, somma lunghezza istruzioni: %d\n",jobs[j].id, jobs[j].arrival_time, jobs[j].somma_lunghezza_istruzioni);
	}	
 
	  pthread_t thread0_id;
	  pthread_t thread1_id;
        
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
	fclose(f_nopre);
	
	return 0;
}
