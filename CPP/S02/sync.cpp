#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include "Thread.h"
#include <iostream>

#define NBC 2
#define NBP 2

#define BUFFER_SIZE 20

int count, in, out;
bool buffer[BUFFER_SIZE];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t products = PTHREAD_COND_INITIALIZER;
pthread_cond_t freeslots = PTHREAD_COND_INITIALIZER;

pthread_cond_t sleepcond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sleepmutex = PTHREAD_MUTEX_INITIALIZER;

int nbdisplay = 0;

void init() {
    int i;
    count = in = out = 0;
    for (i = 0; i < BUFFER_SIZE; i++) buffer[i] = false;
    srand(time(nullptr));
}

void display() {
    int i;
    printf("-");
    for (i = 0; i < BUFFER_SIZE; i++) printf("--");
    printf("\n");
    printf("|");
    for (i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i]) printf("x");
        else printf(" ");
        printf("|");
    }
    printf("%d", nbdisplay++);
    printf("\n");
    printf("-");
    for (i = 0; i < BUFFER_SIZE; i++) printf("--");
    printf("\n");
    printf("\n");
}


void sleep() {
    struct timespec t;
    t.tv_sec = time(0) + (rand() % 2) + 1;
    t.tv_nsec = 0;
    pthread_mutex_lock(&sleepmutex);
    pthread_cond_timedwait(&sleepcond, &sleepmutex, &t);
    pthread_mutex_unlock(&sleepmutex);
}


void produce() {
    // you must re-implement this function
    // it should synchronize access to the shared buffer

    // Mutex allows only 1 resource to run, if it's free, the thread runs and locks the mutex.
    // If's it's not, the thread pauses running until the mutex is free.
    pthread_mutex_lock(&mutex);
    while (count >= BUFFER_SIZE) {
        std::cout << "Buffer full!" << std::endl;
        // First unlocks the mutex and allows other threads to be unlocked. Thread put in a queue.
        // Once the queue is signaled, the mutex attempts to be locked again and a thread will be picked up to resume.
        pthread_cond_wait(&products, &mutex);
    }

    printf("I am producing ...\n");
    buffer[in] = true;
    in = (in + 1) % BUFFER_SIZE;
    count++;
    display();
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&freeslots);
}

void consume() {
    // you must re-implement this function
    // it should synchronize access to the shared buffer
    pthread_mutex_lock(&mutex);
    while (count <= 0) {
        pthread_cond_wait(&freeslots, &mutex);
    }
    printf("I am consuming ...\n");
    buffer[out] = false;
    out = (out + 1) % BUFFER_SIZE;
    count--;
    display();
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&products);
}


[[noreturn]] void *consumer(void *threadid) {
    for (;;) {
        consume();
        sleep();
    }
}

[[noreturn]] void *producer(void *threadid) {
    for (;;) {
        produce();
        sleep();
    }
}

int main() {
    // you must re-implement this function
    // it should create a number of threads that execute the comsumer and producer routines
    init();
    auto t1 = new Thread(nullptr, consumer, nullptr);
    auto t2 = new Thread(nullptr, consumer, nullptr);
    auto t3 = new Thread(nullptr, consumer, nullptr);
    auto t4 = new Thread(nullptr, producer, nullptr);
    t1->join();
    t2->join();
    t3->join();
    t4->join();
}



