//
// Created by khanh on 12/24/2022.
//
#include <stdio.h>
#include <string.h>

#define NB_DISK_PAGE 5
#define PAGE_SIZE 20
#define NB_MEM_PAGE 4

#define boolean int
#define true 1
#define false 0

char Disk[NB_DISK_PAGE][PAGE_SIZE];
char Memory[NB_MEM_PAGE][PAGE_SIZE];
int PageTable[NB_DISK_PAGE];
struct {
    boolean free;
    int date;
    int npage;
} MemState[NB_MEM_PAGE];

int Date = 1;

void init();
char* memory_read(int npage);
int memory_alloc();
void page_fault(int npage);
int lru_select();

void init()
{
    int i;

    // initialize Disk
    for (i = 0; i < NB_DISK_PAGE; i++) {
        strcpy(Disk[i], "page");
        Disk[i][4] = '0' + i;
        Disk[i][5] = 0;
    }
    /*
    for (i=0;i<NB_DISK_PAGE;i++) {
        printf(Disk[i]);
        printf("\n");
    }
    */

    // initialize Memory
    for (i = 0; i < NB_MEM_PAGE; i++)
        MemState[i].free = true;

    // initialize PageTable
    for (i = 0; i < NB_DISK_PAGE; i++)
        PageTable[i] = -1;
}

char* memory_read(int npage)
{
    printf("memory_read (%d)\n", npage);
    if (PageTable[npage] == -1) {
        page_fault(npage);
    }
    int r_mem = PageTable[npage];
    MemState[r_mem].date = Date++;
    return Memory[r_mem];
}

int memory_alloc()
{
    printf("memory_alloc: %d\n", 0);
    for (int i = 0; i < NB_MEM_PAGE; i++) {
        if (MemState[i].free == true) {
            MemState[i].free = false;
            return i;
        }
    }
    return -1;
}

void page_fault(int npage)
{
    printf("page_fault (%d)\n", npage);
//    int r_mem = memory_alloc();
//    if (r_mem == -1) {
//        r_mem = lru_select();
//        PageTable[MemState[r_mem].npage] = -1;
//        bcopy(Memory[r_mem], Disk[MemState[r_mem].npage], PAGE_SIZE);
//    }
//
//    bcopy(Disk[npage], Memory[r_mem], PAGE_SIZE);
//    PageTable[npage] = r_mem;
//    MemState[r_mem].npage = npage;
}

int lru_select()
{
    printf("lru_select: %d\n", 0);
//    int oldest = 0;
//    for (int i = 0; i < NB_MEM_PAGE; i++) {
//        if (MemState[i].date > MemState[oldest].date)
//            oldest = i;
//    }
//    return oldest;
}

int main(int argc, char* argv[])
{
    int i;
    printf("initialization\n");
    init();

    /*
        printf("linear access of all pages\n");
        for (i=0;i<NB_DISK_PAGE;i++)
            printf("read access page %d : %s\n",i,memory_read(i));
    */

    printf("access pages as in lecture (0,1,2,3,0,1,4,0,1,2,3,4,1,2)\n");
    int serie[14] = { 0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4, 1, 2 };
    for (i = 0; i < 14; i++) {
        printf("read access page %d : %s\n", serie[i], memory_read(serie[i]));
        printf("current memory state: ");
        for (int j = 0; j < NB_MEM_PAGE; j++)
            printf("%d ", MemState[j].npage);
        printf("\n");
    }
    printf("completed\n");
    printf("print memory_state\n");
    for (i = 0; i < NB_MEM_PAGE; i++)
        printf("%d ", MemState[i].npage);
    printf("\n");
    printf("completed\n");
}