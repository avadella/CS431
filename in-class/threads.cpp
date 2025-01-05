#include <pthread.h>
#include <iostream>

int max = 1;
static volatile unsigned int counter = 0; 

pthread_mutex_t lock;       //need to access it from both threads and the main 

//function to include the work to be completed in the thread -- pthread is a C API, so doesn't really do objects, hence functions
void *mythread(void *arg)
{
    //std::cout << "mythread: " << (char*) arg << std::endl;
    //return NULL;                                                //typically don't return data from thread, easier to more safely return data by placing it on the heap

    int i = 0; 
    std::cout << (const char*) arg << " [addr of i:] " << &i << std::endl;      //will show address of i, which is a thread local var
    for(i = 0; i < max; i++)
    {
        pthread_mutex_lock(&lock);
        counter = counter + 1;
        pthread_mutex_unlock(&lock);
    }
        

    std::cout << (const char*) arg << " done." << std::endl;
    return NULL;
}

int main(int argc, char * argv[])
{
    //creating and joining a new thread 
    /*pthread_t p1;
    std::cout << "main: creating thread" << std::endl;

    pthread_create(&p1, NULL, mythread, (void *)"A");       //type casting "A" as a void * instead of a const *
    //void * = type of point which can be any type of data, still have to tell language what it is so it knows how to use to correctly 
    std::cout << "main: thread created" << std::endl;
    //thread will run to completetion before joining

    pthread_join(p1, NULL);
    std::cout << "main: thread joined" << std::endl;*/

    int num_threads = 5;
    std:: cout << "max: " << max << "\n counting to " << num_threads * max << std::endl;

    assert(pthread_mutex_init(&lock, NULL) == 0);       //assert = guarantee this condition exists or exit the program

    pthread_t threads[num_threads];     //keeping track to be able to know what thread is doing is, OS views these all as being "individually schedulable"

    //will have num_threads + 1 threads in process b/c how num_threads from loop and +1 from the "main" thread which runs the loop
    for(int i = 0; i < num_threads; i++)
    {
        std::cout << "main: creating thread" << i << std::endl;
        //const char* arg = std::to_string(i).c_str();
        pthread_create(&threads[i], NULL, mythread, (void *)"A"); 
        //pthread_create(&threads[i], NULL, mythread,  (void *) arg);
        std::cout << "main: thread created" << i << std::endl;
    }

    for(int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        std::cout << "main: thread joined" << std::endl;
    }

    std:: cout << "counter: " << counter << " ?= " << num_threads * max << std::endl;
    pthread_mutex_destroy(&lock); 

    return EXIT_SUCCESS;
}