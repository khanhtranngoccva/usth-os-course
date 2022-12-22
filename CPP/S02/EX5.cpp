//
// Created by Admin on 12/21/2022.
//
#include <pthread.h>
#include <iostream>

int Acompute1() {
    return 2;
}

int Bcompute1() {
    return 3;
}

int Acompute2(int resB) {
    return resB * 2;
}

int Bcompute2(int resA) {
    return resA * 3;
}

int resA;
int resB;
bool AInitialized = false;
bool BInitialized = false;
pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;

void waitForB() {
    // Signals on A.
    AInitialized = true;
    pthread_cond_broadcast(&condA);

    // Waits for B.
    pthread_mutex_lock(&mutexB);
    while (!BInitialized) {
        pthread_cond_wait(&condB, &mutexB);
    }
    pthread_mutex_unlock(&mutexB);
}

void waitForA() {
    BInitialized = true;
    pthread_cond_broadcast(&condB);

    pthread_mutex_lock(&mutexA);
    while (!AInitialized) {
        pthread_cond_wait(&condA, &mutexA);
    }
    pthread_mutex_unlock(&mutexA);
}

void programA() {
    resA = Acompute1();
    waitForB();
    std::cout << Acompute2(resB) << std::endl;
}

void programB() {
    resB = Bcompute1();
    waitForA();
    std::cout << Bcompute2(resA) << std::endl;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, reinterpret_cast<void *(*)(void *)>(programA), nullptr);
    pthread_create(&t2, nullptr, reinterpret_cast<void *(*)(void *)>(programB), nullptr);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
}