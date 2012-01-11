//#include "png.h"
#include "png_alloc.h"
#include "pngpriv.h"
#include "efat.h"
#include "midgard.h"

void * png_get_mem_ptr_2(png_struct * png_ptr)
{
   if (png_ptr == NULL)
      return (NULL);
   return (void *)(png_ptr->mem_ptr);
}

void * png_get_io_ptr_2(png_struct * png_ptr)
{
   if (png_ptr == NULL)
      return (NULL);
   return (png_ptr->io_ptr);
}

void * usr_alloc(png_struct * png_ptr, uint32 size) {
	Heap_Manager * mem_ptr;
	if(png_ptr != NULL) {
		mem_ptr = (Heap_Manager *)png_get_mem_ptr_2(png_ptr);
		return (void *)m_heap_alloc(mem_ptr, size); 
	}
	return NULL;
}

void usr_release(png_struct * png_ptr, void * ptr) {
	Heap_Manager * mem_ptr;
	if(png_ptr != NULL) {
		mem_ptr = (Heap_Manager *)png_get_mem_ptr_2(png_ptr);
		m_heap_free(mem_ptr, ptr);
	}
}

void usr_read_data(png_struct * png_ptr, uchar * data, uint32 size) {
	efat_File * file;
	if(png_ptr != NULL) {
		file = (efat_File *)png_get_io_ptr_2(png_ptr);
		//efat_Read(512, myFile, (uchar *)buf);
		efat_Read(size, file, data);
	}
}

void usr_write_data(png_struct * png_ptr, uchar * data, uint32 size) {
	efat_File * file;
	if(png_ptr != NULL) {
		file = (efat_File *)png_get_io_ptr_2(png_ptr);
		//efat_Write(512, myFile, (uchar *)buf);
		efat_Write(size, file, data);
	}
}