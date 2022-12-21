//
// Created by Admin on 12/21/2022.
//
#include <iostream>
#include <pthread.h>
#define MAX_BUF_SIZE 16
#include <cmath>
#include <unistd.h>

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

void sendBuffer(char* data) {
    printf("%s\n", data);
}

// We divide the large packet into segments of N items and push them all to the server until done.
class Message {
    int seek = 0;
    static const int seekSize = MAX_BUF_SIZE - 1;
    int seekCount;
    int messageLength = 0;
    int finished = 0;
    std::string packet;
    DynamicList<char*>* buffers = new DynamicList<char*>();
    pthread_mutex_t seekMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t seekCond = PTHREAD_COND_INITIALIZER;
    static void _addBuf(Message* thisArg) {
        pthread_mutex_lock(&thisArg->seekMutex);
        int curSeek = thisArg->seek;
        thisArg->seek++;
        pthread_mutex_unlock(&thisArg->seekMutex);
        int firstIndex = curSeek * Message::seekSize;
        int lastIndex = std::min((curSeek + 1) * Message::seekSize, thisArg->messageLength);
        auto buffer = (char *)calloc(MAX_BUF_SIZE, sizeof (char));
        int i = 0;
        for (int j = firstIndex; j < lastIndex; j++) {
            buffer[i] = thisArg->packet[j];
            i++;
        }
        buffer[i] = '\0';
        thisArg->buffers->push(buffer);
        std::cout << "Finished!" << std::endl;
        thisArg->finished++;
        if (thisArg->finished == thisArg->seekCount) {
            pthread_cond_broadcast(&thisArg->seekCond);
        }
    }

public:
    explicit Message(const std::string& packet) {
        this->packet = packet;
        this->messageLength = (int) this->packet.length();
        this->seekCount = ceil((double) messageLength / Message::seekSize);
        for (int i = 0; i < seekCount; i++) {
            pthread_t thread;
            pthread_create(&thread, nullptr, reinterpret_cast<void *(*)(void *)>(Message::_addBuf), this);
        }
    }

    void send() {
        pthread_mutex_lock(&seekMutex);
        while (seek < seekCount) {
            pthread_cond_wait(&this->seekCond, &seekMutex);
        }
        pthread_mutex_unlock(&seekMutex);
        for (int i = 0; i < buffers->length(); i++) {
            sendBuffer(buffers->get(i));
        }
    }
};

int main() {
    auto message = new Message("foo foo foo bar bar bar foo foo foo bar bar bar foo foo foo bar bar bar");
    message->send();
}