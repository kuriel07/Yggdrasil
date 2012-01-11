//#define MAX_MEMORY 1024 * 1024 * MEMORY_SIZE_IN_MEGABYTES
// using a fixed size array isn’t the best idea,
// the best implementation would be to manage the data with a chained list
//#define LIST_ITEMS 100
#ifndef _MIDGARD_H
#include "defs.h"
#include "stdlib.h"
//tested on x86
/*#define _MIDGARD_MM_TEST 0
#define _MIDGARD_DEBUG 0
#if _MIDGARD_DEBUG
#define MIDGARD_DEBUG_ACTIVATED
#endif*/
extern uint32 _total_heap;
extern uint32 _used_heap;
extern char Image$$RW$$Limit[];
#define _USE_MIDGARD
struct alloc_chain {
    struct alloc_chain * next;
    //prev never used actually, can be eliminated for better resources
	struct alloc_chain * prev;
    uint32 size;
};

typedef struct alloc_chain alloc_chain;

struct Heap_Manager {
	alloc_chain * root;
	uint32 size;
};
typedef struct Heap_Manager Heap_Manager;

uint32 m_init_alloc(void);
#ifdef _USE_MIDGARD
//standard ANSI C for heap management
void * m_alloc(uint32 size);
void m_free(void *ptr);
#define malloc 	m_alloc
#define free	m_free
#else
#define m_alloc c_malloc
#define m_free c_free
#endif
//heap management for better performance on multitasking
//also to avoid memory sharing for task
Heap_Manager * m_create_heap(uint32 size);
void m_delete_heap(Heap_Manager * heap); 
void * m_heap_alloc(struct Heap_Manager * heap, uint32 size);
void * m_heap_alloc_b(struct Heap_Manager * heap, uint32 size, uint16 bound);
void m_heap_free(struct Heap_Manager * heap, void * ptr);
uint32 m_heap_used(struct Heap_Manager * heap);
uint32 m_used_space(void);
void m_memcopy(void * dst, const void * src, size_t size);

static uint32 m_shift(uchar *dst, uchar *src, uchar size);
void m_gc(void);
void m_mem_dump(void);
extern void m_memcopy(void * dst, const void * src, size_t size);
#define _MIDGARD_H
#endif
