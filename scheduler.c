#include "scheduler.h"

#include <pthread.h>

void* core(void* unused){
    //TODO
    return NULL;
}

int sheduler_preemptive(){
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
	
	 pthread_t thread1_id;
	 pthread_t thread2_id;
    /* Create a new thread.
     * The new thread will run the core function. 
     */
    pthread_create(&thread1_id, NULL, &core, NULL);
	pthread_create(&thread2_id, NULL, &core, NULL);
	
	
    pthread_join (thread1_id, NULL);
	pthread_join (thread2_id, NULL);
        
	return 0;
}

int scheduler_not_preemptive(){
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
	 
	return 0;
}
