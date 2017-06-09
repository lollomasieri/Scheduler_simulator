#include <pthread.h>
#include <stdio.h>

#include "funzioni.h"

int main(){
	struct istruzione istru{0,5,8,NULL};
	
	struct job jobs{0,0,&istru,NEW,1,5};
	
	int clock=0;
	esegui(&clock,&jobs);
	return 0;
}
