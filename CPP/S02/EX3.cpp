//
// Created by Admin on 12/22/2022.
//
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;

template <typename T>
void println(T data) {
    pthread_mutex_lock(&printMutex);
    std::cout << data << std::endl;
    pthread_mutex_unlock(&printMutex);
}

template <typename T>
void print(T data) {
    pthread_mutex_lock(&printMutex);
    std::cout << data << std::endl;
    pthread_mutex_unlock(&printMutex);
}

int finished = 0;

void P1() {
    sleep(1);
    println("Process 1 completed.");
    finished++;
    sem_post(&sem);
}

void P2() {
    sleep(1);
    println("Process 2 completed.");
    finished++;
    sem_post(&sem);
}

void wait() {
    while (finished < 2) {
        sem_wait(&sem);
    }
    println("All processes completed.");
}

int main() {
    pthread_t t1, t2, t3;
    sem_init(&sem, 0, 0);
    pthread_create(&t1, nullptr, reinterpret_cast<void *(*)(void *)>(P1), nullptr);
    pthread_create(&t2, nullptr, reinterpret_cast<void *(*)(void *)>(P2), nullptr);
    pthread_create(&t3, nullptr, reinterpret_cast<void *(*)(void *)>(wait), nullptr);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
}