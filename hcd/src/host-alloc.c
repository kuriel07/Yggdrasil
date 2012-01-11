#include "defs.h"
#include "midgard.h"
#include "host.h"
#include "host-alloc.h"

void * dev_alloc(Heap_Manager * heap, size_t size, uint16 bound) {
	return m_heap_alloc_b(heap, size, bound);
}

void dev_free(Heap_Manager * heap, void * ptr) {
	m_heap_free(heap, ptr);
}
