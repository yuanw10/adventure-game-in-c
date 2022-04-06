#ifndef __HEROES_HEADER
#define __HEROES_HEADER


#include "array_list.h"
#include <stdbool.h>
#include <stdio.h>
#include "queue.h"

typedef struct _Hero
{
  char* name;
  char* class;
  int level;

  //Attributes order: 0-strength
  //1-agility, 2-intelligence, 3-charisma
  double attributes[4];

  //Index of primary attribute on attributes[]
  int primary;
  
  //Where the hero died if they failed
  //NULL if they win
  char* diedLocation;

  //Whether or not they challenged all locations
  //and reached 200 level successfully
  bool win;

} Hero;


#include "locations.h"


ArrayList* read_heroes(char*);
void gain_levels(Hero*);
int find_primary(Hero*);
bool challenge_location(Hero*, Location*);
ArrayList* sort_heroes(ArrayList*);
int compare_heroes(void*, void*);
bool output_alive(ArrayList*, char*);
bool output_dead(ArrayList*, char*);


#endif
