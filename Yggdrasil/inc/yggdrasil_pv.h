#ifndef _YGGDRASIL_PV__H
#include "defs.h"
#include "yggdrasil.h"
#include "png.h"
#include "efat.h"

typedef struct PViewer_Properties PViewer_Properties;

struct PViewer_Properties {
	Heap_Manager * heap;
	png_structp png_ptr;
};
png_fixed_point png_reciprocal_2(png_fixed_point a);
png_byte png_gamma_8bit_correct_2(unsigned int value, png_fixed_point gamma_val);
png_fixed_point png_reciprocal2_2(png_fixed_point a, png_fixed_point b);
int png_gamma_significant_2(png_fixed_point gamma_val);
png_fixed_point png_product2(png_fixed_point a, png_fixed_point b);
png_uint_16 png_gamma_16bit_correct_2(unsigned int value, png_fixed_point gamma_val);
/*png_voidp png_user_alloc(png_structp png_ptr, png_size_t size);
void png_user_release(png_structp png_ptr, png_voidp ptr);
void png_user_read_data(png_structp png_ptr, png_bytep data, png_size_t size);
void png_user_write_data(png_structp png_ptr, png_bytep data, png_size_t size);
void png_user_flush_data(png_structp png_ptr);
void png_user_error(void);
void png_user_warning(void);*/

extern Shell_Object * CreatePicViewer(void);
void HandlePVLauncher(Shell_Object * sender);
#define _YGGDRASIL_PV__H
#endif

