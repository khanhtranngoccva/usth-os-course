//
// Created by khanh on 12/19/2022.
//

#include "Semaphore.h"

class Semaphore {
    int procAvailable = 1;
public:
    Semaphore() {}
    Semaphore(int initialProcAvailable) {
        this.procAvailable = initialProcAvailable;
    }
    void wait() {
        this->procAvailable--;

    }

    void signal() {

    }
};
