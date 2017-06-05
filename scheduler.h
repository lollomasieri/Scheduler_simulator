#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <pthread.h>

void* core_preemptive(void* unused);

int sheduler_preemptive(const char* output_preemption_filename, int quantum);

void* core_not_preemptive(void* unused);

int scheduler_not_preemptive(const char* output_no_preemption_filename);

#endif
