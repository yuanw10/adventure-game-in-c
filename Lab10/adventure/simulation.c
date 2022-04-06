#include "simulation.h"
#include "heroes.h"
#include "locations.h"
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"
#include "heap.h"
#include <string.h>
#include "array_list.h"
#include <stdbool.h>


//Function for creating a thread
//One hero challenging all locations
void* one_hero_adventure(void* adv)
{
  //Casting
  Args* adventure = adv;
  Hero* h = adventure->hero;

  int i = 0;
  while(!adventure->done)
  {
    //Get each location
    Location* l = alist_get(adventure->locations,i);

    //Hero challenging the location
    bool success = challenge_location(h, l);

    //If failed, the adventure is done
    if(!success)
    {
      adventure->done = true;
    }

    //If challenging all loactions successfully, reached 200
    //Adventure is done and win
    if(adventure->hero->level == 200)
    {
      adventure->done = true;
      h->win = true;
    }

    i++;
  }

  return (void*)h;
}


//Run the thread pool, multiple heroes challenging locations
ArrayList* execute_threads(int poolSize, Queue* args)
{
  //Create a thread pool and a list containing Args
  pthread_t pool[poolSize];
  ArrayList* argList = alist_initialize(poolSize, sizeof(Args), "Args");

  //For each thread in the pool
  for(int i = 0;i < poolSize && queue_size(args) != 0;i++)
  {

    //Get an Args on the queue to execute
    Args* arg = (Args*)queue_dequeue(args);

    //Add to the Args list
    alist_add_at(argList, i, arg);

    //Create a new thread for the Args
    //let the hero inside the Args challenge all locations 
    pthread_create(&pool[i], NULL, one_hero_adventure, arg);
  }

  //When a hero's challenging is done, store them on the list
  ArrayList* heroList = alist_initialize(300, sizeof(Hero), "Hero");


  //Implement the chanllenging until all heroes are done
  while(heroList->size < 300)
  {
    for(int i = 0; i < poolSize; i++)
    {
      //Get the args on the list
      Args* ar = alist_get(argList,i);

      //If the adventure is done and there are Args left
      if(ar->done && queue_size(args) > 0)
      { 
	//Retrieve the hero who has done the adventure
	Hero* hero = NULL;
        pthread_join(pool[i], (void**) &hero);

	//Put the hero on the list
        alist_add(heroList, (void*)hero);

	//Get the next Args on the queue
        ar = (Args*)queue_dequeue(args);
	
    	//Create a new thread for the Args
        //let the hero inside the Args challenge all locations 
	pthread_create(&pool[i], NULL, one_hero_adventure, ar);
      }

      //If the adventure is done but no Args left
      if(ar->done && queue_size(args) == 0)
      {
        //Retrieve the hero
	Hero* hero = NULL;
        pthread_join(pool[i], (void**) &hero);

	//Put the hero on the list
        alist_add(heroList, (void*)hero);

	ar->done = false;
      }
    }
  }

  return heroList;
}


//Create a queue to store Args for running thread pool
Queue* create_args(ArrayList* heroes, ArrayList* path)
{
  //Create a queue to store Args for the thread pool
  Queue* args = queue_initialize(sizeof(Args),"Args");

  //Create Args for every hero
  for(int i = 0; i < 300; i++)
  {
    //Create an Args with one hero and the required path 
    Args* arg = malloc(sizeof(Args));
    arg->hero = alist_get(heroes,i);
    arg->locations = path;
    arg->done = false;

    //Append the Args to the queue
    queue_enqueue(args,arg);
  }

  //Return the queue
  return args;
}
