#ifndef _PNG_ALLOC_H
#include "defs.h"
#include "png.h"

void * png_get_mem_ptr_2(png_struct * png_ptr);
void * png_get_io_ptr_2(png_struct * png_ptr);

void * usr_alloc(png_struct * png_ptr, uint32 size);
void usr_release(png_struct * png_ptr, void * ptr);
void usr_read_data(png_struct * png_ptr, uchar * data, uint32 size);
void usr_write_data(png_struct * png_ptr, uchar * data, uint32 size);
#define _PNG_ALLOC_H
#endif