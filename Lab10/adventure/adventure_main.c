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
//Arguments required in order: PathName, PoolSize
//To implement the program: ./adventure_main [PathName] [PoolSize]
//Refer to README for more details
int main(int argc, char** argv)
{
  //Check the first argument indicating the required path
  if(strcmp(argv[1], "level") != 0 && strcmp(argv[1], "power") != 0 && strcmp(argv[1], "subtlety") != 0 
     && strcmp(argv[1], "strategy") != 0 && strcmp(argv[1], "charm") != 0)
  {
    printf("Invalid argument. The 1st argument is used to specify which path to complete in thread pool.\n");
    printf("Please re-run the program with valid path name: level, power, subtlety, strategy, charm.\n");
    return -1;
  }


  //Read in the size of the thread pool
  int poolSize = atoi(argv[2]);

  //Check the 2nd argument valid or not
  if(poolSize == 0)
  {
    printf("Invalid argument. The 2nd argument should be an integer greater than zero.\n");
    printf("Re-run the program with a valid integer.\n");
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
  printf("\nReading in Location data from the file...");
  ArrayList *locations = read_locations("../../CI/objects/adventure/locations.lot");

  if(locations == NULL)
  {
    printf("Failure.\n");
    return -2;
  }
  printf("Success.\n");



  //===== Step2: generate required path according to the first command line argument =====

  ArrayList* path = NULL;

  //If the argument specified level path
  if(strcmp(argv[1], "level") == 0)
  {
    //Generate level path
    printf("\nGenerating Level Path...");
    path = get_level_path(locations);
  }

  //If power path is specified
  if(strcmp(argv[1], "power") == 0)
  {
    //Generate power path
    printf("\nGenerating Power Path...");
    path = get_power_path(locations);
  }

  //If subtlety path is specified
  if(strcmp(argv[1], "subtlety") == 0)
  {
    //Generate subtlety path
    printf("\nGenerating Subtlety Path...");
    path = get_subtlety_path(locations);
  }

  //If strategy path is specified
  if(strcmp(argv[1], "strategy") == 0)
  {
    //Generate strategy path
    printf("\nGenerating Strategy Path...");
    path = get_strategy_path(locations);
  }

  //If charm path is specified
  if(strcmp(argv[1], "charm") == 0)
  {
    //Generate charm path
    printf("\nGenerating Charm Path...");
    path = get_charm_path(locations);
  }


  if(path == NULL)
  {
    printf("Failure.\n");
    return -3;
  }
  printf("Success.\n");  




  //===== Step3: run the specified path =====
  
  printf("\nRunning the required path within thread pool...");

  //Create a queue storing all Args for the thread pool
  Queue* args = create_args(heroes, path);

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

  //Initialize two strings to store the 1st argument, which indicates the required path
  //Copy the 1st argument to the two strings, to creat PathName_alive & PathName_dead later
  char* aliveFile = malloc(sizeof(char*));
  char* deadFile = malloc(sizeof(char*));;
  strcpy(aliveFile, argv[1]);
  strcpy(deadFile, argv[1]);

  //Complete file names
  strcat(aliveFile,"_alive");
  strcat(deadFile, "_dead");

  printf("\nProducing output files...\n");


  //Output heroes who successfully challanged all locations in the required path
  if(!output_alive(sortedHeroes, aliveFile))
  {
    printf("Failure.\n");
    return -5;
  }
  printf("%s is done.\n", aliveFile);


  //Output heroes who died during the path
  if(!output_dead(sortedHeroes, deadFile))
  {
    printf("Failure.\n");
    return -5;
  }
  printf("%s is done.\n", deadFile);


  printf("You can find the two files within the same directory.\n");

  return 0;
}

