#include <pthread.h>
#include <cstdlib>
#include <iostream>

template<typename T>
class DynamicList {
protected:
    unsigned long long _length = 0;
    unsigned long long maxLength = 20;
    T *data;
public:
    explicit DynamicList(T *data) {
        this->data = (T *) calloc(maxLength, sizeof(T));
        for (int i = 0; data[i] != nullptr; i++) {
            this->push(data[i]);
        }
    }

    void push(T item) {
        if (this->_length >= this->maxLength) {
            this->maxLength *= 2;
            this->data = (T *) realloc(this->data, sizeof(T) * this->maxLength);
        }
        this->data[this->_length] = item;
        this->_length++;
    }

    T pop() {
        if (this->_length >= 0) {
            this->_length--;
            return this->data[this->_length];
        }
        std::throw_with_nested("Empty array.");
    }

    virtual int length() {
        return this->_length;
    }

    T get(int i) {
        if (i < 0 || i >= this->_length) {
            std::throw_with_nested("Out of bounds.");
        }
        return this->data[i];
    }

    DynamicList() {
        this->data = (T *) calloc(maxLength, sizeof(T));
    }
};


class ResManager {
    int resRemaining;
    int totalResources;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t blockedQueue = PTHREAD_COND_INITIALIZER;
public:
    explicit ResManager(int totalResources) {
        this->totalResources = totalResources;
        this->resRemaining = totalResources;
    }

    void alloc(int resCount) {
        pthread_mutex_lock(&mutex);
        while (this->resRemaining < resCount) {
            pthread_cond_wait(&blockedQueue, &mutex);
        }
        this->resRemaining -= resCount;
        std::cout << "Allocated " << resCount << " resources." << std::endl;
        pthread_mutex_unlock(&mutex);
        // Is there any extra blocked allocations?
        pthread_cond_signal(&blockedQueue);
        // Execute anything else over here.
    }

    void free(int resCount) {
        pthread_mutex_lock(&mutex);
        this->resRemaining += resCount;
        if (this->resRemaining > this->totalResources) {
            this->resRemaining = this->totalResources;
        }
        std::cout << "Freed " << resCount << " resources." << std::endl;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&blockedQueue);
    }
};

auto manager = new ResManager(20);

void alloc(int res) {
    manager->alloc(res);
}

void free(int res) {
    manager->free(res);
}

int main() {
    auto threads = new DynamicList<pthread_t>();
    for (int i = 0; i < 20; i++) {
        pthread_t t1;
        pthread_create(&t1, nullptr, reinterpret_cast<void *(*)(void *)>(alloc), reinterpret_cast<void *>(2));
        threads->push(t1);
    }
    free(7);
    free(7);
    for (int i = 0; i < threads->length(); i++) {
        pthread_join(threads->get(i), nullptr);
    }
    return 0;
}