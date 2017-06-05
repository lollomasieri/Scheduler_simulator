#include "scheduler.h"

void* core_preemptive(void* unused){
    //TODO
    return NULL;
}

int sheduler_preemptive(const char* output_preemption_filename, int quantum){
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
    pthread_create(&thread_id, NULL, &core_preemptive, NULL);
	
	core_preemptive(NULL);	
		
    pthread_join(thread_id, NULL);
	    
	return 0;
}

void* core_not_preemptive(void* unused){
    //TODO
    return NULL;
}

int scheduler_not_preemptive(const char* output_no_preemption_filename){
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
	 
	  pthread_t thread_id;
    /* Create a new thread.
     * The new thread will run the core function. 
     */
    pthread_create(&thread_id, NULL, &core_not_preemptive, NULL);
		
	core_not_preemptive(NULL);
		
    pthread_join(thread_id, NULL);
    
	return 0;
}
