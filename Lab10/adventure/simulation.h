#ifndef __SIMULATION_HEADER
#define __SIMULATION_HEADER


#include "heroes.h"
#include "locations.h"
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"
#include "heap.h"
#include "array_list.h"
#include <stdbool.h>

typedef struct _Args
{
  Hero* hero;
  ArrayList* locations;
  bool done;
} Args;

void* one_hero_advanture(void*);
ArrayList* execute_threads(int, Queue*);
Queue* create_args(ArrayList*, ArrayList*);

#endif
