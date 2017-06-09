#include "scheduler.h"

#define MAX_JOBS 2048

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
    
    
    
    printf("id job 200: %d\n", params->jobs[200].id);
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
	
	 pthread_t thread_id;
    /* Create a new thread.
     * The new thread will run the core function. 
     */
     
    int err = pthread_create(&thread_id, NULL, &core_preemptive, &params);
	if (err != 0) {
		fprintf(stderr, "Can't create thread. Reason: '%s'", strerror(err));
		exit(EX_OSERR);
	}
	
	core_preemptive(&params);	
		
    pthread_join(thread_id, NULL);
	    
	return 0;
}

void* core_not_preemptive(void* parameters){
	struct params_sched_not_preemptive* params = (struct params_sched_not_preemptive*) parameters;
	
	printf("core not preemptive avviato\n");
    //TODO
    /*
    int clock = 0;
    int jobs_completati = 0;
    int count = 0;
    while(jobs_completati < MAX_JOBS){
		if(clock < params->jobs[count].arrival_time){
			clock++;
			continue;
		}
		
	}
    */
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
	 
	struct job *jobs = params.jobs;		
	//ordino i jobs
	quickSort(jobs, 0, MAX_JOBS-1);	
	
/*	for (int j=0; j<2048; j++){		
		printf("id: %d, somma lunghezza istruzioni: %d\n",jobs[j].id, jobs[j].somma_lunghezza_istruzioni);
	}	
*/ 
	 
	  pthread_t thread_id;
    /* Create a new thread.
     * The new thread will run the core function. 
     */
    int err = pthread_create(&thread_id, NULL, &core_not_preemptive, &params);
	if (err != 0) {
		fprintf(stderr, "Can't create thread. Reason: '%s'", strerror(err));
		exit(EX_OSERR);
	}
	
	core_not_preemptive(&params);
		
    pthread_join(thread_id, NULL);
    
	return 0;
}
