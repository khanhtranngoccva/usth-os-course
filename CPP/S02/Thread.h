//
// Created by khanh on 12/20/2022.
//
extern "C" {
#include <pthread.h>
}

#ifndef USTH_OSCOURSE_S02_THREAD_H
#define USTH_OSCOURSE_S02_THREAD_H


class Thread {
protected:
    pthread_t thread;
public:
    Thread(const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
    pthread_t getId() const;
    int join(void **status=nullptr) const;
};


#endif //USTH_OSCOURSE_S02_THREAD_H
