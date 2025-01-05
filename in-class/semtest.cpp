#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <iostream>
#include <cstdio>

#define MAX 1

sem_t empty;
sem_t full;
sem_t mutex;

int buffer;
int count = 0;                  //initially empty
const int loops = 10;

void put(int value)
{
    assert(count == 0);
    count = 1;
    buffer = value;
    std::cout << "producer putting item into buffer" << std::endl;
}

int get()
{
    assert(count == 1);         
    count = 0;
    std::cout << "consumer getting item from buffer" << std::endl;
    return buffer;
}

void * producer(void *arg)
{
    int i;
    for(int i = 0; i < loops; i++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void * consumer(void *arg)
{
    while(1)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        int tmp = get();
        sem_post(&mutex);
        sem_post(&empty);
        printf("%d\n", tmp);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    //int i = 10;
    sem_init(&empty, 0, MAX);
    sem_init(&full, 0, MAX);
    sem_init(&mutex, 0, 1);
    pthread_t producer, consumer;

    std::cout << "main: creating producer thread" << std::endl;
    pthread_create(&producer, NULL, producer, NULL);

    std::cout << "main: thread consumer created" << std::endl;
    pthread_create(&consumer, NULL, consumer, NULL);

    pthread_join(producer, NULL);
    std:: cout << "main: thread producer joined: " << std::endl;
    pthread_join(consumer, NULL);
    std:: cout << "main: thread consumer joined: " << std::endl;

    return EXIT_SUCCESS;
}