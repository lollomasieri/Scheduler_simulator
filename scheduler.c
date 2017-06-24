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
	             
    unsigned long clock = 0;

	bool flag = 0; 
    int jobs_rimanenti = MAX_JOBS;

    while(jobs_rimanenti > 0){		
		jobs_rimanenti = MAX_JOBS; 
		flag = 0; //Viene settato a 1 se durante un ciclo viene eseguito almeno un job
		for (int i = 0; i< MAX_JOBS; i++){
			pthread_mutex_lock(&mutex_coda_jobs_pre); //Inizio sessione critica
			switch(params->jobs[i].stato){
				case 0: //NEW
					if(clock >= params->jobs[i].arrival_time){
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
    return NULL;
}

int sheduler_preemptive(struct params_sched_preemptive params){
	
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
	   
    unsigned long clock = 0;

	bool flag = 0; 
    int jobs_rimanenti = MAX_JOBS;
	
    while(jobs_rimanenti > 0){		
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
	}
    return NULL;
}

int scheduler_not_preemptive(struct params_sched_not_preemptive params){
		
	//Apertura file
	f_nopre = open_file(params.output_no_preemption_filename);
	
	//il mutex e` inizialmente libero: 
	pthread_mutex_init (&mutex_coda_jobs_nopre, NULL);
	pthread_mutex_init (&mutex_file_output_nopre, NULL);
 
	struct job *jobs = params.jobs;		
	//ordino i jobs
	quickSort(jobs, 0, MAX_JOBS-1);	

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
