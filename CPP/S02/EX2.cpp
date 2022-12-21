//
// Created by Admin on 12/21/2022.
//
#include <cstring>
#include <iostream>
#include <pthread.h>

#define BUF_SIZE 1024

template<typename T>
class ProdCons {
    T data[BUF_SIZE];
    int in = 0;
    int out = 0;
    int count = 0;
    pthread_mutex_t mainMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t mainQueue = PTHREAD_COND_INITIALIZER;

public:
    void produce(T item) {
        pthread_mutex_lock(&mainMutex);
        if (count < BUF_SIZE) {
            count++;
            data[in] = item;
            in = (in + 1) % BUF_SIZE;
        }
        pthread_mutex_unlock(&mainMutex);
        pthread_cond_signal(&mainQueue);
    }

    T consume() {
        pthread_mutex_lock(&mainMutex);
        do {
            pthread_cond_wait(&mainQueue, &mainMutex);
        } while (count <= 0);
        auto result = data[out];
        out = (out + 1) % BUF_SIZE;
        count--;
        pthread_mutex_unlock(&mainMutex);
        return result;
    }
};

auto obj = new ProdCons<std::string>();

void *produce(void *args) {
    while (true) {
        obj->produce("oof");
    }
}

void *consume(void *args) {
    while (true) {
        std::cout << obj->consume() << std::endl;
    }
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, produce, nullptr);
    pthread_create(&t2, nullptr, consume, nullptr);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
}

