#ifndef __LOCATIONS_HEADER
#define __LOCATIONS_HEADER


typedef struct _Location
{
  char* name;
  int level;
  int power;
  int subtlety;
  int strategy;
  int charm;
} Location;


#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
#include "heroes.h"
#include "heap.h"
#include "array_list.h"

ArrayList* read_locations(char*);
bool compare_attributes(Hero*, Location*);

ArrayList* get_level_path(ArrayList*);
ArrayList* get_power_path(ArrayList*);
ArrayList* get_subtlety_path(ArrayList*);
ArrayList* get_strategy_path(ArrayList*);
ArrayList* get_charm_path(ArrayList*);

int compare_level(void*, void*);
int compare_power(void*, void*);
int compare_subtlety(void*, void*);
int compare_strategy(void*, void*);
int compare_charm(void*, void*);



#endif
