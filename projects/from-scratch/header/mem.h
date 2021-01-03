#ifndef _mem_h_
#define _mem_h_

#include <MALLOC.H>

#define HEAP_DEFALT_START_ADDR 0x800F8000
#define HEAP_DEFALT_SIZE 0x00100000

#define MEM_RESERVE(startAddr, size) InitHeap3((unsigned long*)startAddr, size)
#define MEM_ALLOC(type) (type*) malloc3(sizeof(type))
#define MEM_CALLOC(n, t) (t*) calloc3((size_t) n, sizeof(t))
#define MEM_FREE(ptr) free3(ptr), ptr = NULL


#endif