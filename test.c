//#include <pthread.h>
#include <stdio.h>

//#include "funzioni.h"

int main(){
	//struct istruzione istru{0,5,8,NULL};
	
	//struct job jobs{0,0,&istru,NEW,1,5};
	
	//int clock=0;
	//esegui(&clock,&jobs);
	
	int a[3]= {1,2,3};
	
	
	printf ("puntatore: %p\n", &a);
	
	printf ("puntatore: %p\n", &a+1);
	
	printf ("puntatore: %p\n", &a+2);
	
	int *c = &a[1];
	printf ("puntatore: %d\n", *c);
	return 0;
}
