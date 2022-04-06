#define _GNU_SOURCE
#include <stdio.h>
#include "locations.h"
#include "array_list.h"
#include <stdbool.h>

//Read in hero data from the file
ArrayList* read_heroes(char* filePath)
{
  FILE *filePtr = fopen(filePath,"r");
  size_t len = 0;

  //Create a list to store heroes
  ArrayList* heroes = alist_initialize(200,sizeof(Hero),"Hero");

  //Check if the list or the file path can be found
  if(heroes == NULL || filePath == NULL || filePtr == NULL)
    return NULL;


  //Read in 300 heroes
  for(int i = 0; i < 300; i++)
  {
    char* nameLine = NULL;
    char* classLine = NULL;
    char* discard = NULL;

    //Create one hero from the input data
    Hero *hero = malloc(sizeof(Hero));
    hero -> level = 0;
    hero -> primary = -1;
    hero -> diedLocation = NULL;
    hero -> win = false;


    //Get the first line, the hero name
    getline(&nameLine, &len, filePtr);
    hero->name = malloc(strlen(nameLine));
    strncpy(hero->name, nameLine, strlen(nameLine)-1);
    hero->name[strlen(nameLine)-1] = '\0';

    //Get the second line, the hero job (class)
    getline(&classLine, &len, filePtr);
    hero->class = malloc(strlen(classLine));
    strncpy(hero->class, classLine, strlen(classLine)-1);
    hero->class[strlen(classLine)-1] = '\0';

    //Read in data from the file, the hero attributes
    //Attributes order: 0-strength 1-agility, 2-intelligence, 3-charisma
    fscanf(filePtr,"%lf, %lf, %lf, %lf", &(hero->attributes[0]), &(hero->attributes[1]),
		    &(hero->attributes[2]), &(hero->attributes[3]));

    //Get the primary attribute of the hero
    find_primary(hero);

    //Discard the new line sign
    getline(&discard, &len, filePtr);

    //Add the hero to the list
    alist_add(heroes, hero);

  }

  //File inputing is done
  fclose(filePtr);
  
  return heroes;
} 


//If challenging a location successfully, hero gains levels
void gain_levels(Hero* hero)
{
  if(hero == NULL) return;

  //Increase the hero's level by 1
  hero->level++;

  //Increase each attribute on attributes[] array
  for(int i = 0; i < 4; i++)
  {
    //Primary attribute increases faster, by 0.35
    if(i == hero->primary)
      hero->attributes[i] += 0.35;

    //Other attributes, 0.25
    else
      hero->attributes[i] += 0.25;
  }

}



//Find the hero's primary attribute, return its index on attributes[]
int find_primary(Hero* hero)
{
  double maxAttr = -1;
  int priIndex = -1;

  //Primary attribute is the greatest attribute
  for(int i = 0; i < 4; i++)
  {
    if(hero->attributes[i] > maxAttr)
    {
      maxAttr = hero->attributes[i];
      priIndex = i;
    }
  }

  //Get the index of primary attribute, store it within the Hero struct
  hero->primary = priIndex;

  return priIndex;
}


//Hero challenges a location by comparing attributes with location's corresponding rating
bool challenge_location(Hero* hero, Location* location)
{

  //Call compare_attributes() to ompare each attribute with location's corresponding 
  //rating. compare_attributes() is within Location struct
  //If hero's attribute >= location's corresponding rating, then winning the test
  //If winning any of the four tests, then challenging successful
  bool success = compare_attributes(hero, location);

  //If challenging successful, the hero gains levels
  if(success)
    gain_levels(hero);

  //Otherwise, the hero died, record the locations where they died
  else
    hero->diedLocation = location->name;

  return success;
}


//Sort heroes in the order of sum of their attributes, smallest to greatest
//This function helps organize output
ArrayList* sort_heroes(ArrayList* heroes)
{
  //Create a heap to sort heroes
  Heap* heap = heap_initialize(sizeof(Hero), "Hero", compare_heroes, NULL);

  //Insert heroes to the heap for sorting
  for(int i = 0; i < heroes->size; i++)
  {
    heap_insert(heap, alist_get(heroes,i));
  }

  //Create a list to store sorted heores
  ArrayList* sortedHeroes = alist_initialize(300, sizeof(Hero), "Hero");

  //Insert heores in order(total attributes, smallest to greatest)
  int i = 0;
  while(heap_size(heap) > 0)
  {
    alist_add_at(sortedHeroes,i, (heap_remove(heap)));
    i++;
  }

  //returen the list storing sorted heroes
  return sortedHeroes;
}


//Compare heroes with their total attributes
int compare_heroes(void* a, void* b)
{
  double sumA = 0;
  double sumB = 0;

  //Sum all the four attributes of the hero
  for(int i = 0; i < 4; i++)
  {
    sumA += ((Hero*)a)->attributes[i];
  }

  //Sum all the four attributes of the hero
  for(int i = 0; i < 4; i++)
  {
    sumB += ((Hero*)b)->attributes[i];
  }

  //Compare the two totals
  return (int)(sumB - sumA);
}



//Output heroes who are alive one per line to a file
bool output_alive(ArrayList* heroes, char* fileName)
{
  //Null check
  if(heroes == NULL || fileName == NULL)
  {
    return false;
  }
  
  //Create a file pointer
  FILE *filePtr = fopen(fileName, "w");

  //Iterate the whole list
  for(int i = 0; i < heroes->size; i++)
  {
    //Get each hero on the list
    Hero* h = alist_get(heroes,i);

    //If their died location not NULL, they died
    //skip the hero and move to the next one
    if(h->diedLocation != NULL)
      continue;

    //Otherwise, they are alive
    //output their information in the order: name, four attributes
    fprintf(filePtr,"%s %.2lf %.2lf %.2lf %.2lf\n", h->name, h->attributes[0],
		      h->attributes[1], h->attributes[2], h->attributes[3]);

  }

  //Writing to file is done
  fclose(filePtr);
  return true;
}


//Output heroes who are dead one per line to a file
bool output_dead(ArrayList* heroes, char* fileName)
{
  //NULL check
  if(heroes == NULL || fileName == NULL)
  {
    return false ;
  }
  

  //Create a file pointer
  FILE *filePtr = fopen(fileName, "w");


  //Iterate the whole list
  for(int i = 0; i < heroes->size; i++)
  {
    //Get each hero on the list
    Hero* h = alist_get(heroes,i);

    //If their died location NULL, they alive
    //skip the hero and move to the next one
    if(h->diedLocation == NULL)
      continue;
    
    //Otherwise, they are dead
    //output their information in the order: name, four attributes, level, died location
    fprintf(filePtr,"%s %.2lf %.2lf %.2lf %.2lf %d %s\n", h->name, h->attributes[0], 
		    h->attributes[1], h->attributes[2], h->attributes[3], h->level, h->diedLocation);

  }

  //Writing to file is done
  fclose(filePtr);

  return true;

}
