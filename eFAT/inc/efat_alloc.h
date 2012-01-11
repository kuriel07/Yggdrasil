#include "defs.h"

#ifndef _EFAT_ALLOC_H
void init_efat_Alloc(void);
void * efat_Alloc(size_t size);
void efat_Free(void * ptr);

#define _EFAT_ALLOC_H
#endif