#include "array_list.h"
#include "queue.h"
#include "heap.h"
#include "heroes.h"
#include "locations.h"
#include "simulation.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

//Execute the simulation
//Arguments required: PoolSize
//To implement the program: ./deusex_main [PoolSize]
//Refer to README for more details
int main(int argc, char** argv)
{
  //Read in the size of the thread pool
  int poolSize = atoi(argv[1]);

  if(poolSize == 0)
  {
    printf("Invalid argument. It should be an integer greater than zero.\n");
    printf("Re-run the program with a valid argument.\n");
    return -1;
  }


  //=====Step1: read in data from files =====

  //Read in heroes from the file
  printf("Reading in Hero data from the file...");
  ArrayList *heroes = read_heroes("../../CI/objects/adventure/heroes.lot");

  if(heroes == NULL)
  {
    printf("Failure.\n");
    return -2;
  }
  printf("Success.\n");


  //Read in locations from the file
  printf("Reading in Location data from the file...");
  ArrayList *locations = read_locations("../../CI/objects/adventure/locations.lot");

  if(locations == NULL)
  {
    printf("Failure.\n");
    return -2;
  }
  printf("Success.\n");



  //===== Step2: generate all paths using the input locations =====

    printf("\nNext, generate all paths using the input locations:\n");
    
    //Generate power path
    printf("Generating Power Path...");
    ArrayList* powerPath = get_power_path(locations);

    if(powerPath == NULL)
    {
      printf("Failure.\n");
      return -3;
    }
    printf("Success.\n");  



    //Generate subtlety path
    printf("Generating Subtlety Path...");
    ArrayList* subtletyPath = get_subtlety_path(locations);

    if(subtletyPath == NULL)
    {
      printf("Failure.\n");
      return -3;
    }
    printf("Success.\n");  



    //Generate strategy path
    printf("Generating Strategy Path...");
    ArrayList* strategyPath = get_strategy_path(locations);

    if(strategyPath == NULL)
    {
      printf("Failure.\n");
      return -3;
    }
    printf("Success.\n");  



    //Generate charm path
    printf("Generating Charm Path...");
    ArrayList* charmPath = get_charm_path(locations);

    if(charmPath == NULL)
    {
      printf("Failure.\n");
      return -3;
    }
    printf("Success.\n");  



  //===== Step3: run the thread pool, let all heroes challenge their optimal path =====
  
  printf("\nHeroes have different optimal paths.\nNow, assign an optimal path to each hero.\n");

  //Create a queue storing all Args for the thread pool
  Queue* args = create_args(heroes, powerPath, subtletyPath, strategyPath, charmPath);


  printf("Done.\n\nRunning the thread pool, let heroes challenge their optimal path...");


  //Create timer
  struct timeval before, after;
  unsigned long beforeMicros, afterMicros, executionTime;

  //Record the starting time
  gettimeofday(&before, NULL);

  //Run the thread pool
  //and get a list of heroes who are done with challengings
  ArrayList* heroList = execute_threads(poolSize, args);


  //Record the finishing time
  gettimeofday(&after, NULL);


  if(heroList == NULL)
  {
    printf("Failure.\n");
    return -4;
  }

  //Compute execution time in micro seconds
  beforeMicros = 1000000 * before.tv_sec + before.tv_usec;
  afterMicros = 1000000 * after.tv_sec + after.tv_usec;
  executionTime = afterMicros - beforeMicros;


  //Print the time taken for the simulation in microseconds to the screen
  printf("Completed.\nExecution time: %ld microseconds.\n", executionTime);



  //===== Step4: produce output files  =====

  //Sort all the heroes in the order of the sum of their attributes, smallest to greatest
  ArrayList* sortedHeroes = sort_heroes(heroList);


  char* aliveFile = "deusex_alive";
  char* deadFile = "deusex_dead";

  printf("\nProducing output files...\n");


  //Output heroes who successfully challanged all locations in their optimal path
  if(!output_alive(sortedHeroes, aliveFile))
  {
    printf("Failure.\n");
    return -5;
  }
  printf("%s is done.\n", aliveFile);


  //Output heroes who died during their optiaml path
  if(!output_dead(sortedHeroes, deadFile))
  {
    printf("Failure.\n");
    return -5;
  }
  printf("%s is done.\n", deadFile);


  printf("You can find the two files within the same directory.\n");



  return 0;
}
