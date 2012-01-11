#include "defs.h"
#include "midgard.h"
#include "efat_alloc.h"

Heap_Manager * _efat_hm_alloc;

void init_efat_Alloc(void) {
	_efat_hm_alloc = m_create_heap(128 * 1024);
}

void * efat_Alloc(size_t size) {
	return m_heap_alloc(_efat_hm_alloc, size);
}

void efat_Free(void * ptr) {
	m_heap_free(_efat_hm_alloc, ptr);
}
