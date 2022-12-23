#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cstring>

#define NB_DISK_BLOCK 5
#define BLOCK_SIZE 20
#define NB_MEM_BLOCK 4

class MemoryState {
public:
    int lastAccess;
    bool free = true;
    int nPage;
};

class MemoryManager {
    int curTime = 0;
    char **disk;
    int diskBlockCount;
    char **memory;
    int memoryBlockCount;
    int *pageTable;
    int pageTableSize;
    int allocated = 0;
    MemoryState *memoryState;
public:
    MemoryManager(int blockSize = BLOCK_SIZE,
                  int memoryBlockCount = NB_MEM_BLOCK,
                  int diskBlockCount = NB_DISK_BLOCK,
                  int pageTableSize = NB_DISK_BLOCK) {
        this->memoryBlockCount = memoryBlockCount;
        this->diskBlockCount = diskBlockCount;
        this->pageTableSize = pageTableSize;
        std::cout << memoryBlockCount << " " << diskBlockCount << " " << pageTableSize << std::endl;
        this->disk = (char **) malloc(diskBlockCount * blockSize * sizeof(char));
        this->memory = (char **) malloc(memoryBlockCount * blockSize * sizeof(char));
        this->pageTable = (int *) malloc(pageTableSize * sizeof(int));
        this->memoryState = (MemoryState *) malloc(memoryBlockCount * sizeof(MemoryState));
    }

    int alloc(int allocCount = 1) {
        int newAllocated = this->allocated + allocCount;
        if (newAllocated > diskBlockCount + memoryBlockCount) {
            std::throw_with_nested("Not enough memory to allocate.");
        }
        for (let i = 0; i < 10; i++)
    }
};

int main() {
    auto memoryManager = new MemoryManager();
}