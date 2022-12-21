#include <cstdio>
#include <pthread.h>
#include <iostream>
#include "Thread.h"
#include <cerrno>
#include <unistd.h>

class Component {
private:
    int processingThreads = 0;
    pthread_t lastThread;
    pthread_mutex_t isAllowedToWrite = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t stopQueue = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t stopMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t startMutex = PTHREAD_MUTEX_INITIALIZER;
public:
    Component() {
        pthread_mutex_lock(&isAllowedToWrite);
        pthread_mutex_lock(&stopMutex);
        pthread_mutex_unlock(&startMutex);
    }

    void start() {
        std::cout << "Starting component." << std::endl;
        // Only allowed the thread that shuts down the component to run, or there were no threads IDs to start with.
        if (pthread_equal(pthread_self(), lastThread) || lastThread == 0) {
            // Only 1 thread can start the component. If the component is already started, exit the function.
            pthread_mutex_lock(&startMutex);
            // Allow the mutex to be stopped.
            pthread_mutex_unlock(&stopMutex);
            // Unlock the mutex to allow writes.
            pthread_mutex_unlock(&isAllowedToWrite);
            std::cout << "Component successfully started." << std::endl;
        } else {
            std::cout << "Can't start component - this isn't the thread that stops the component." << std::endl;
        }
    }

    void stop() {
        std::cout << "Stopping component." << std::endl;
        // Only 1 thread can stop the component, and it will be recorded in the lastThread property. After stopping, it can't be stopped again.
        int stopMutexLock = pthread_mutex_trylock(&stopMutex);
        if (stopMutexLock != 0) {
            if (stopMutexLock == EBUSY) {
                std::cout << "The component is not running." << std::endl;
                return;
            } else {
                std::throw_with_nested("Mutex error.");
            }
        }
        // std::cout << "No other threads can invoke stop()." << std::endl;
        // Prevents any new threads from entering the component so that we can safely check how many threads are pending.
        pthread_mutex_lock(&isAllowedToWrite);
        // Record the stopper thread in the component property.
        lastThread = pthread_self();
        // If there's a thread still pending, wait for the queue.
        while (processingThreads > 0) {
            std::cout << "Threads left: " << processingThreads << std::endl;
            pthread_cond_wait(&stopQueue, &isAllowedToWrite);
        }
        // After that, it can be started again.
        pthread_mutex_unlock(&startMutex);
        std::cout << "Component stopped." << std::endl;
    }

    void enter() {
        processingThreads++;
        // Enable synchronization. Only 1 thread is allowed to write at once.
        pthread_mutex_lock(&isAllowedToWrite);
    }

    void exit() {
        // After it's done, unlock the mutex so that we can resume writing.
        processingThreads--;
        pthread_mutex_unlock(&isAllowedToWrite);
        if (processingThreads <= 0) {
            pthread_cond_signal(&stopQueue);
        }
    }
};

// C++ global component to allow easier interaction.
auto *mainComponent = new Component();

void alienThread() {

}

void *writer(void *emptyArg) {
    mainComponent->enter();
    for (int i = 0; i < 2; i++) {
        std::cout << "oof" << std::endl;
        sleep(1);
    }
    mainComponent->exit();
    return nullptr;
}

void *alienFunc(void *emptyArg) {
    mainComponent->start();
    mainComponent->stop();
    return nullptr;
}

int main() {
    pthread_t thread1, thread2, thread3, thread4, alienThread;
    // This is also blocking.
    mainComponent->start();
    pthread_create(&thread1, nullptr, writer, nullptr);
    pthread_create(&thread2, nullptr, writer, nullptr);
    pthread_create(&thread3, nullptr, writer, nullptr);
    pthread_create(&thread4, nullptr, writer, nullptr);
    // This is blocking.
    mainComponent->stop();
    pthread_create(&alienThread, nullptr, alienFunc, nullptr);
    pthread_join(alienThread, nullptr);
}