//CS431, Project #4 - Concurrency: Written by Anna Vadella
#include <iostream>
#include <random>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//global variable declarations
const int max_capacity = 200;                                                       //maximum capacity for dining hall

sem_t mutex;
sem_t readyToLeaveMutex;                                                            //signals when a student is able to leave
sem_t waitMutex;                                                                    //used for students waiting to leave
sem_t randMutex;

int capacity = 0;                                                                   //number of students currently in dining hall
int eating = 0;                                                                     //number of students currently eating
int readyToLeave = 0;                                                               //number of students ready to leave
int speed = 3;

int get_random(const int min, const int max) 
{
  static std::random_device dev;
  static std::mt19937 twister(dev());
  static std::uniform_int_distribution<int> dist;

  dist.param(std::uniform_int_distribution<int>::param_type(min, max));

  sem_wait(&randMutex);
  int rand = dist(twister);
  sem_post(&randMutex);

  return rand;
}

/*void dine()
{
    sem_wait(&mutex);
    eating++;                                                                       //increment number of students eating
    sem_post(&mutex);

    sem_wait(&mutex);
    sem_wait(&readyToLeaveMutex);

    if(eating > 1 && readyToLeave == 1)                                             //check conditions for leaving 
    {
        sem_post(&mutex);   
        sem_post(&readyToLeaveMutex);                                     

        sem_wait(&readyToLeaveMutex);
        readyToLeave--;                                                             //decrement number of students ready to leave
        sem_post(&readyToLeaveMutex);
    }                                                                        
    else 
    {
        sem_post(&mutex);
        sem_post(&readyToLeaveMutex);
    }
}*/

void *student(void *arg)
{
    int id = *static_cast<int*>(arg);

    sem_wait(&mutex);
    sem_wait(&readyToLeaveMutex);
    std::cout << "thread " << id << ": started" << std::endl;
    //std::cout << "eating: " << eating << " leaving: " << readyToLeave << std::endl;
    sem_post(&mutex);
    sem_post(&readyToLeaveMutex);
    sleep(get_random(0, speed));                                                        //randomize arrival time

    /*sem_wait(&mutex);
    sem_wait(&readyToLeaveMutex);
    std::cout << "thread " << id << ": getting hungry" << std::endl;
    std::cout << "eating: " << eating << " leaving: " << readyToLeave << std::endl;
    sem_post(&mutex);
    sem_post(&readyToLeaveMutex);
    sleep(get_random(0, speed));*/

    sem_wait(&readyToLeaveMutex);                                                       //hold this to protect capacity

    if(capacity >= max_capacity)                                                        //dining hall is full, student must wait
    {
        sem_post(&readyToLeaveMutex);

        sem_wait(&mutex);
        std::cout << "thread " << id << ": waiting and will leave." << std::endl; 
        sem_post(&mutex);
    }
    else
    {
        sem_post(&readyToLeaveMutex);
        
        sem_wait(&readyToLeaveMutex);
        capacity++;                                                                     //new student entering dining hall
        sem_post(&readyToLeaveMutex);

        sem_wait(&mutex);
        sem_wait(&readyToLeaveMutex);
        std::cout << "thread " << id << ": getting food" << std::endl;
        //std::cout << "eating: " << eating << " leaving: " << readyToLeave << std::endl;
        sem_post(&mutex);
        sem_post(&readyToLeaveMutex);
        sleep(get_random(0, speed));

        sem_wait(&mutex);
        eating++;                                                                       //increment number of students eating
        sem_post(&mutex);

        sem_wait(&mutex);
        sem_wait(&readyToLeaveMutex);

        if(eating > 1 && readyToLeave == 1)                                             //check conditions for leaving 
        {
            sem_post(&mutex);   
            sem_post(&readyToLeaveMutex);                                     

            sem_wait(&readyToLeaveMutex);
            readyToLeave--;                                                             //decrement number of students ready to leave
            sem_post(&readyToLeaveMutex);
        }                                                                        
        else 
        {
            sem_post(&mutex);
            sem_post(&readyToLeaveMutex);
        }

        sem_wait(&mutex);
        sem_wait(&readyToLeaveMutex);
        std::cout << "thread " << id << ": eating" << std::endl;
        //std::cout << "eating: " << eating << " leaving: " << readyToLeave << std::endl;
        sem_post(&mutex);
        sem_post(&readyToLeaveMutex);
        sleep(get_random(0, speed));

        sem_wait(&mutex);                                                               //done eating, ready to leave
        eating--;                                                                       //decrement number of students eating
        sem_post(&mutex);

        //sem_wait(&mutex);
        sem_wait(&readyToLeaveMutex);
        readyToLeave++;                                                                 //increment number of students ready to leave
        //std::cout << "LEAVING NUM: " << readyToLeave << std::endl;
        //sem_post(&mutex);
        sem_post(&readyToLeaveMutex);

        sem_post(&waitMutex);

        sem_wait(&mutex);
        sem_wait(&readyToLeaveMutex);

        if(eating == 1 && readyToLeave == 1)                                             //only one student eating and one ready to leave, prevent leaving student alone
        {
            std::cout << "thread " << id << ": waiting to leave" << std::endl;

            //sem_wait(&mutex); 
            //sem_wait(&readyToLeaveMutex);
            
            if(eating == 1 && readyToLeave == 1)                                        //wait until this condition is not longer true
            {  
                sem_post(&mutex);
                sem_post(&readyToLeaveMutex);

                sem_wait(&waitMutex);
            }
            
            sem_post(&mutex);
            sem_post(&readyToLeaveMutex);

            sem_wait(&readyToLeaveMutex); 
            readyToLeave--;                                                             //now "safe" for students to leave
            capacity--;
            sem_post(&readyToLeaveMutex);

            //sem_post(&mutex);
        }
       else if(eating == 0 && readyToLeave == 2)                                        //if no one is eating and two or more students are ready to leave
       {
            sem_post(&mutex);
            sem_post(&readyToLeaveMutex);
            
            sem_wait(&readyToLeaveMutex);
            readyToLeave--;                                                             //students leave in pair
            capacity--;
            sem_post(&readyToLeaveMutex);
       }
       else
       {
            sem_post(&mutex);
            sem_post(&readyToLeaveMutex);
            
            sem_wait(&readyToLeaveMutex);
            readyToLeave--;                                                             //student can leave without waiting
            capacity--;
            sem_post(&readyToLeaveMutex);
       }

        sem_wait(&mutex);
        sem_wait(&readyToLeaveMutex);
        std::cout << "thread " << id << ": finished" << std::endl;
        //std::cout << "eating: " << eating << " leaving: " << readyToLeave << std::endl;
        sem_post(&mutex);
        sem_post(&readyToLeaveMutex);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int numStudents = 10;                                                             //provide default num of students if a num isn't provided
    if (argc == 2)
        numStudents = atoi(argv[1]);
    
    //const int numStudents = atoi(argv[1]);                                          //atoi function used to convert string to int, stands for "ASCII to integer"
    pthread_t threads[numStudents];                 

    sem_init(&mutex, 0, 1);
    sem_init(&readyToLeaveMutex, 0, 1);
    sem_init(&waitMutex, 0, 0);
    sem_init(&randMutex, 0, 1);

    //creating threads
    for (int i = 0; i < numStudents; ++i) 
    {
        sem_wait(&mutex);
        std::cout << "Main    : create and start thread " << i << "." << std::endl;
        sem_post(&mutex);
        int *id = new int(i);
        pthread_create(&threads[i], NULL, student, id);
    }

    //joining threads
    for (int i = 0; i < numStudents; ++i) 
    {
        sem_wait(&mutex);
        std::cout << "Main    : before joining thread." << std::endl;
        sem_post(&mutex);
        pthread_join(threads[i], NULL);
        std::cout << "Main    : thread done" << std::endl;
    }

    return 0;
}