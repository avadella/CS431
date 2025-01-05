#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <random>

//global variable declarations
const int speed = 5;                                                        //smaller is faster!
const int num = 5;
sem_t mutex;
sem_t forks[5]; 

int getRandom(const int min, const int max)
{
  static std::random_device dev;
  static std::mt19937 twister(dev());
  static std::uniform_int_distribution<int> dist;

  dist.param(std::uniform_int_distribution<int>::param_type(min, max));
  return dist(twister);
}

void get_fork(int id, int fork)                                       
{
    sem_wait(&mutex);                                                       //not a pthread mutex lock, semaphore lock (can do everything a "normal" mutex lock can do + more)
    std::cout << "philosopher" << id << ": getting fork" << fork << std::endl;
    sem_post(&mutex);                                                       //only need mutex lock while printing, not for the whole function - if hold the whole time, could potentially cause deadlock between getting and putting

    sem_wait(&forks[fork]);                                                 //fork is parameter that supposed what we want to get (which fork id requires, L or R), pass in fork array b/c forks is an array of binary semaphores

    sem_wait(&mutex);  
    std::cout << "philosopher" << id << ": got fork" << fork << std::endl;
    sem_post(&mutex);
}

void put_fork(int id, int fork)                                     
{
    sem_wait(&mutex);                                                       //provides mutual exclusion as long as you put it around every cout statement
    std::cout  << "philosopher" << id << ": putting fork" << fork << std::endl;
    sem_post(&mutex);

    sem_post(&forks[fork]);

    sem_wait(&mutex);   
    std::cout << "philosopher" << id << ": put fork" << fork << std::endl;
    sem_post(&mutex);
}

void * philosopher(void *arg)
{   
    int id = *(int *)arg;

    sem_wait(&mutex);                                                       //locking (holding lock while printing, if scheduler changing context, will not let something else print b/c holding lock)
    std::cout << "philsopher " << id << " is starting." << std::endl;
    sem_post(&mutex);

    while (true) 
    {   
        sem_wait(&mutex);
        std::cout << "philsopher " << id << " is thinking." << std::endl;
        sem_post(&mutex);
        sleep(getRandom(0, speed));                                         //thinking, getRandom gets "good random" opposed to get_rand, sleep will "pause" program/thread

        if(id == 0)                                                         //id 0 will try to acquire in the opposite order as other ids!
        {
            get_fork(id, (id + 4) % num);  
            get_fork(id, id);
        }
        else 
        {
            get_fork(id, id);
            get_fork(id, (id + 4) % num);  
        }
        
        //get_fork(id, id);                                                 //take right fork

        /*if(id == 0)                                                       //ensuring "wrap around"
            get_fork(4);
        else
            get_fork(id - 1);                                               //take left fork*/

        //get_fork(id, (id + 4) % num);                                     //take left fork -- better than above if-else, treating as circular array

        //eat
        sem_wait(&mutex);
        std::cout << "philsopher " << id << " is eating." << std::endl;
        sem_post(&mutex);
        sleep(getRandom(0, speed));  

        if(id == 0)                                                         //id 0 will try to acquire in the opposite order as other ids! - could also do all evens or all odds if want a mod in there
        {
            put_fork(id, id); 
            put_fork(id, (id + 4) % num);  
        }
        else 
        {
            put_fork(id, (id + 4) % num);  
            put_fork(id, id); 
        }

        //put_fork(id, (id + 4) % num);                                       //release left fork
        //put_fork(id, id);                                                   //release right fork
    }
}

int main(int argc, char* argv[])
{
    pthread_t philosopher[num];                                             //(circular) array of philosophers, could also use a vector (array easier if you know specific size)
    int thread_ids[num];

    sem_init(&mutex, 0, 1);

    //initalizing forks 
    for(int i = 0; i < num; i++)
    {
        sem_init(&forks[i], 0, 1);                                          //initialize 0 = coordinate threads, initialize 1 = same as mutex (good if need to protect critical section, deadlock potential)
        //sem_init(&forks, 0, 5);                                           /single semaphore, pile of forks, would work if you change constraints 
    }
        
    //create threads
    for(int i = 0; i < num; i++)
    {
        //const char * arg = std::to_string(i).c_str();
        thread_ids[i] = i;
        pthread_create(&philosopher[i], NULL, philosopher, (void*)&i);
    }

    //join threads
    for(int i = 0; i < num; i++)
        pthread_join(philosopher[i], NULL);
    
    return EXIT_SUCCESS;
}


//Code from class! - would be in philosopher function
    /*think();
    sem_wait(i);            //left - pick up - elementary answer 
    sem_wait(i + 1);        //right - pick up - elementary answer 
    eat();                  //need two forks to eat
    sem_post(i);            //left - put down - elementary answer 
    sem_post(i + 1);        //right - put down - elementary answer*/
//create 5 philosophers in an array, use circular array to keep track
//structure of philosopher function: think, take a fork, take another fork, eat, release first fork, release second fork
//monitor: data structure like putting a lock in an object

//Solutions:
//make one philosopher opposite handed - ie, if all are left, make one right 
//have a "supervisor" to help coordinate eating between threads, detects this