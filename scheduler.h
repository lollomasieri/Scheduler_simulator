#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "funzioni.h"

void* core_preemptive(void* parameters);

int sheduler_preemptive(struct params_sched_preemptive params);

void* core_not_preemptive(void* parameters);

int scheduler_not_preemptive(struct params_sched_not_preemptive params);

#endif
