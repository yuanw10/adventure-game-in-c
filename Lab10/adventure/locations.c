#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "heroes.h"
#include <string.h>
#include "heap.h"
#include "array_list.h"

//Read in location data from the file
ArrayList* read_locations(char* filePath)
{
  FILE *filePtr = fopen(filePath,"r");
  size_t len = 0;

  //Create a list to store locations
  ArrayList* locations = alist_initialize(200,sizeof(Location),"Location");

  //Check if the list or the file path can be found
  if(locations == NULL || filePath == NULL || filePtr == NULL)
    return NULL;


  //Read in 200 locations
  for(int i = 0; i < 200; i++)
  {
    char* nameLine = NULL;
    char* discard = NULL;

    //Create a location from the input data
    Location *loc = malloc(sizeof(Location));

    //Get the first line, the location name
    getline(&nameLine, &len, filePtr);
    loc->name = malloc(strlen(nameLine));
    strncpy(loc->name, nameLine, strlen(nameLine)-1);
    loc->name[strlen(nameLine)-1] = '\0';

    //Read in data from the file, the location attributes
    fscanf(filePtr,"%d, %d, %d, %d, %d", &(loc->level), &(loc->power),
		    &(loc->subtlety), &(loc->strategy), &(loc->charm));

    //Discard the new line sign
    getline(&discard, &len, filePtr);

    //Put the location into the list
    alist_add(locations, loc);

  }

  //File inputing is done
  fclose(filePtr);
  
  return locations;
}

//Perform the comparisons between the hero and the location
//Compare hero's attributes with location's corresponding ratings
bool compare_attributes(Hero* hero, Location* location)
{
  bool winPower, winSubtlety, winStrategy, winCharm;


  //For each comparison, if the hero's attribute is greater than or equal to
  //the corresponding location rating, then the hero wins that test

  //Strength vs. Power
  winPower = hero->attributes[0] >= location->power;

  //Agility vs. Subtlety
  winSubtlety = hero->attributes[1] >= location->subtlety;

  //Intelligence vs. Strategy
  winStrategy = hero->attributes[2] >= location->strategy;

  //Charisma vs. Charm
  winCharm = hero->attributes[3] >= location->charm;

  //If the hero wins any tests in the challenge
  //they have successfully completed the challenge
  return (winPower || winSubtlety || winStrategy || winCharm);

}



//Use a list of locations to create Level Path
ArrayList* get_level_path(ArrayList* locations)
{
  //Create a heap to sort the locations
  Heap* heap = heap_initialize(sizeof(Location), "Location", compare_level, NULL);

  //Insert locations to the heap
  for(int i = 0; i < 200; i++)
  {
    heap_insert(heap, alist_get(locations,i));
  }

  //Create a list to store locations
  ArrayList* level_path = alist_initialize(200,sizeof(Location),"Location");

  //Insert locations to the list in order of level
  while(heap_size(heap) > 0)
  {
    alist_add(level_path, (heap_remove(heap)));
  }


  //return the list where locations stored as level path
  return level_path;
}



//Use a list of locations to create Power Path
ArrayList* get_power_path(ArrayList* locations)
{
  //Create a heap to sort the locations
  Heap* heap = heap_initialize(sizeof(Location), "Location", compare_power, NULL);

  //Insert locations to the heap
  for(int i = 0; i < 200; i++)
  {
    heap_insert(heap, alist_get(locations,i));
  }

  //Create a list to store locations
  ArrayList* power_path = alist_initialize(200,sizeof(Location),"Location");

  //Insert locations to the list in order of power
  while(heap_size(heap) > 0)
  {
    alist_add(power_path, (heap_remove(heap)));
  }

  //return the list where locations stored as power path
  return power_path;
}



//Use a list of locations to create Subtlety Path
ArrayList* get_subtlety_path(ArrayList* locations)
{
  //Create a heap to sort the locations
  Heap* heap = heap_initialize(sizeof(Location), "Location", compare_subtlety, NULL);

  //Insert locations to the heap
  for(int i = 0; i < 200; i++)
  {
    heap_insert(heap, alist_get(locations,i));
  }

  //Create a list to store locations
  ArrayList* subtlety_path = alist_initialize(200,sizeof(Location),"Location");

  //Insert locations to the list in order of subtlety
  while(heap_size(heap) > 0)
  {
    alist_add(subtlety_path, (heap_remove(heap)));
  }


  //return the list where locations stored as subtlety path
  return subtlety_path;
}



//Use a list of locations to create Strategy Path
ArrayList* get_strategy_path(ArrayList* locations)
{
  //Create a heap to sort the locations
  Heap* heap = heap_initialize(sizeof(Location), "Location", compare_strategy, NULL);

  //Insert locations to the heap
  for(int i = 0; i < 200; i++)
  {
    heap_insert(heap, alist_get(locations,i));
  }

  //Create a list to store locations
  ArrayList* strategy_path = alist_initialize(200,sizeof(Location),"Location");

  //Insert locations to the list in order of strategy
  while(heap_size(heap) > 0)
  {
    alist_add(strategy_path, (heap_remove(heap)));
  }

  return strategy_path;
}



//Use a list of locations to create Charm Path
ArrayList* get_charm_path(ArrayList* locations)
{
  //Create a heap to sort the locations
  Heap* heap = heap_initialize(sizeof(Location), "Location", compare_charm, NULL);

  //Insert locations to the heap
  for(int i = 0; i < 200; i++)
  {
    heap_insert(heap, alist_get(locations,i));
  }

  //Create a list to store locations
  ArrayList* charm_path = alist_initialize(200,sizeof(Location),"Location");

  //Insert locations to the list in order of charm
  while(heap_size(heap) > 0)
  {
    alist_add(charm_path, (heap_remove(heap)));
  }

  return charm_path;
}



//Compare two locations with Level
//used for Heap Sorting and creating Level Path
int compare_level(void* a, void* b)
{
  return ((Location*) b) ->level - ((Location*)a)->level;
}

//Compare two locations with Power
//used for Heap Sorting and creating Power Path
int compare_power(void* a, void* b)
{
  return ((Location*) b) ->power - ((Location*)a)->power;
}

//Compare two locations with Subtlety
//used for Heap Sorting and creating Subtley Path
int compare_subtlety(void* a, void* b)
{
  return ((Location*) b) ->subtlety - ((Location*)a)->subtlety;
}

//Compare two locations with Strategy
//used for Heap Sorting and creating Strategy Path
int compare_strategy(void* a, void* b)
{
  return ((Location*) b) ->strategy - ((Location*)a)->strategy;
}

//Compare two locations with Charm
//used for Heap Sorting and creating Charm Path
int compare_charm(void* a, void* b)
{
  return ((Location*) b) ->charm - ((Location*)a)->charm;
}

