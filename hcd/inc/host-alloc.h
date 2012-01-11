#include "midgard.h"
#include "host.h"
#ifndef _HOST_ALLOC_H

#define USB_HEAP_SIZE 	0x10000

void * dev_alloc(Heap_Manager * heap, size_t size, uint16 bound);

void dev_free(Heap_Manager * heap, void * ptr);

#define _HOST_ALLOC_H
#endif
