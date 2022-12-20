//
// Created by khanh on 12/20/2022.
//
#include <pthread.h>
#include "Thread.h"

Thread::Thread(const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
    this->thread = 0;
    pthread_create(&thread, attr, start_routine, arg);
}

pthread_t Thread::getId() const {
    return this->thread;
}

int Thread::join(void **status) const {
    return pthread_join(this->thread, status);
}