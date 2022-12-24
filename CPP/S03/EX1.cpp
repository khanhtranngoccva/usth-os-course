#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cstring>

#define NB_DISK_BLOCK 5
#define BLOCK_SIZE 20
#define NB_MEM_BLOCK 4

class MemoryState {
public:
    int lastAccess = 0;
    bool free = true;
    // Index to refer back to which index of the page table has the item in memory so as to invalidate.
    int pageTableIndex = -1;
};

class MemoryManager {
    int curTime = 0;
    int blockSize;
    char **disk;
    int diskBlockCount;
    char **memory;
    int memoryBlockCount;
    int *pageTable;
    int pageTableSize;
    int allocated = 0;
    MemoryState **memoryState;
public:
    explicit MemoryManager(int blockSize = BLOCK_SIZE,
                  int memoryBlockCount = NB_MEM_BLOCK,
                  int diskBlockCount = NB_DISK_BLOCK) {
        if (diskBlockCount < memoryBlockCount) {
            std::throw_with_nested("Exception: The disk space must be able to contain the memory space.");
        }
        this->blockSize = blockSize;
        this->diskBlockCount = diskBlockCount;
        this->disk = (char**) calloc(diskBlockCount, sizeof(char*));
        for (int i = 0; i < diskBlockCount; i++) {
            this->disk[i] = (char*) calloc(blockSize + 1, sizeof(char));
        }
        this->memoryBlockCount = memoryBlockCount;
        this->memory = (char**) calloc(memoryBlockCount, sizeof(char*));
        for (int i = 0; i < memoryBlockCount; i++) {
            this->memory[i] = (char *) calloc(blockSize + 1, sizeof(char));
        }
        this->pageTableSize = diskBlockCount;
        this->pageTable = (int*) calloc(diskBlockCount, sizeof(int));
        this->memoryState = (MemoryState**) calloc(memoryBlockCount, sizeof (MemoryState*));
        for (int i = 0; i < memoryBlockCount; i++) {
            this->memoryState[i] = new MemoryState();
        }
    }

    // The table maps 1-1 to the disk.
    // The key is the actual address to the disk, while the pageTable is whether there
    char* read(int pageNum) {
        if (this->pageTable[pageNum] == -1) {
            this->pageFault(pageNum);
        }
        int memAddress = this->pageTable[pageNum];
        this->memoryState[memAddress]->lastAccess = curTime;
        curTime++;
        return this->memory[memAddress];
    }

    int alloc() {
        int total = this->allocated + 1;
        if (total > this->diskBlockCount) {
            return -1;
        }
        this->allocated = total;
        for (int i = 0; i < this->memoryBlockCount; i++) {
            if (this->memoryState[i]->free) {
                this->memoryState[i]->free = false;
                return i;
            }
        }
        return -1;
    }

    // If the disk's content is not found on the memory (-1),
    // then the memory is allocated (and address linked into the table) or swapped.
    void pageFault(int pageNum) {
        int newPhysicalAddress = this->alloc();
        if (newPhysicalAddress == -1) {
            newPhysicalAddress = this->lru();
            int pageTableIndex = this->memoryState[newPhysicalAddress]->pageTableIndex;
            // Swaps the redundant lru item off the memory and invalidate the page table.
            this->pageTable[pageTableIndex] = -1;
            // The index of the page table is also the index on the disk.
            bcopy(this->memory[newPhysicalAddress], this->disk[pageTableIndex], this->blockSize + 1);
        }
        bcopy(this->disk[pageNum], this->memory[newPhysicalAddress], this->blockSize + 1);
        this->pageTable[pageNum] = newPhysicalAddress;
        this->memoryState[newPhysicalAddress]->pageTableIndex = pageNum;
    }

    int lru() {
        int longestTimeSinceLastAccess = 0;
        int toRemove;
        for (int i = 0; i < this->memoryBlockCount; i++) {
            int timeSinceLastAccess = this->memoryState[i]->lastAccess;
            if (timeSinceLastAccess > longestTimeSinceLastAccess) {
                longestTimeSinceLastAccess = timeSinceLastAccess;
                toRemove = i;
            }
        }
        return toRemove;
    }
};

int main() {
    auto memoryManager = new MemoryManager();
}