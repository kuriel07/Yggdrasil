#ifndef _YGGDRASIL_PNG_H
#include "png.h"
#include "pnglibconf.h"
#define png_get_uint_16_2(buf) \
     ((png_uint_16) \
      (((unsigned int)(*(buf)) << 8) + \
       ((unsigned int)(*((buf) + 1)))))

#define png_get_int_32_2(buf) \
     ((png_int_32)((*(buf) & 0x80) \
      ? -((png_int_32)((png_get_uint_32(buf) ^ 0xffffffffL) + 1)) \
      : (png_int_32)png_get_uint_32(buf)))
      
void * png_zalloc_2(void * png_ptr, uInt items, uInt size);
void * png_calloc_2(png_struct * png_ptr, png_alloc_size_t size);
void * png_malloc_2(png_struct * png_ptr, png_alloc_size_t size);
void png_free_3(png_struct * png_ptr, void * ptr);
void png_zfree_2(void * png_ptr, void * ptr);
void png_free_2(png_struct * png_ptr, void * ptr);
void * png_malloc_warn_2(png_struct * png_ptr, png_alloc_size_t size);
void png_set_error_fn_2(png_struct * png_ptr, void * error_ptr,
    void (* error_fn)(png_struct *, char *), void (* warning_fn)(png_struct *, char *));
void png_set_mem_fn_2(png_struct * png_ptr, void * mem_ptr, png_malloc_ptr 
    malloc_fn, void (* free_fn)(png_struct *, void *));    
void png_free_data_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 mask,
   int num);
int png_gamma_significant_2(png_fixed_point gamma_val);
png_uint_16 /* PRIVATE */
png_gamma_correct_2(png_struct * png_ptr, unsigned int value,
    png_int_32 gamma_val);  
png_fixed_point
png_reciprocal2_2(png_fixed_point a, png_fixed_point b);
png_fixed_point
png_reciprocal_2(png_fixed_point a); 
void png_build_gamma_table_2(png_struct * png_ptr, int bit_depth);
png_byte
png_gamma_8bit_correct_2(unsigned int value, png_fixed_point gamma_val);
png_uint_16
png_gamma_16bit_correct_2(unsigned int value, png_fixed_point gamma_val);

int png_sig_cmp_2(png_byte * sig, png_size_t start, png_size_t num_to_check);
#if defined(PNG_READ_GAMMA_SUPPORTED) || \
    defined(PNG_INCH_CONVERSIONS_SUPPORTED) || defined(PNG__READ_pHYs_SUPPORTED)
/* muldiv functions */
/* This API takes signed arguments and rounds the result to the nearest
 * integer (or, for a fixed point number - the standard argument - to
 * the nearest .00001).  Overflow and divide by zero are signalled in
 * the result, a boolean - true on success, false on overflow.
 */
int
png_muldiv_2(png_fixed_point * res, png_fixed_point a, png_int_32 times,
    png_int_32 divisor);
#endif
void /* PRIVATE */
png_check_IHDR_2(png_struct * png_ptr,
   png_uint_32 width, png_uint_32 height, int bit_depth,
   int color_type, int interlace_type, int compression_type,
   int filter_type);
png_fixed_point
png_muldiv_warn_2(png_struct * png_ptr, png_fixed_point a, png_int_32 times,
    png_int_32 divisor);
void /* PRIVATE */
png_calculate_crc_2(png_struct * png_ptr, png_byte * ptr, png_size_t length);
void /* PRIVATE */
png_reset_crc_2(png_struct * png_ptr);    
#if defined(PNG_FLOATING_POINT_SUPPORTED) && \
   !defined(PNG_FIXED_POINT_MACRO_SUPPORTED)
png_fixed_point
png_fixed_2(png_struct * png_ptr, double fp, char * text);
void /* PRIVATE */
png_info_destroy_2(png_struct * png_ptr, png_info * info_ptr);
void png_info_init_5(png_info ** ptr_ptr, png_size_t png_info_struct_size);
int png_handle_as_unknown_2(png_struct * png_ptr, png_byte * chunk_name);
void png_destroy_struct_3(void * struct_ptr, void (* free_fn)(png_struct *, void *),
void * mem_ptr);
#endif

void /* PRIVATE */
png_read_data_2(png_struct * png_ptr, png_byte * data, png_size_t length);
void /* PRIVATE */
png_crc_read_2(png_struct * png_ptr, png_byte * buf, png_size_t length);
png_uint_32 /* PRIVATE */
png_read_chunk_header_2(png_struct * png_ptr);
void /* PRIVATE */
png_check_chunk_name_2(png_struct * png_ptr, png_byte * chunk_name);
int /* PRIVATE */
png_crc_finish_2(png_struct * png_ptr, png_uint_32 skip);
void /* PRIVATE */
png_do_expand_2(png_row_info * row_info, png_byte * row,
    png_color_16p trans_value);
    void /* PRIVATE */
png_do_expand_palette_2(png_row_info * row_info, png_byte * row,
   png_color * palette, png_byte * trans_alpha, int num_trans);
#if defined(PNG_WRITE_FILLER_SUPPORTED) || \
    defined(PNG_READ_STRIP_ALPHA_SUPPORTED)
/* Remove filler or alpha byte(s) */
void /* PRIVATE */
png_do_strip_filler_2(png_row_info * row_info, png_byte * row, png_uint_32 flags);
#endif
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
int /* PRIVATE */
png_do_rgb_to_gray_2(png_struct * png_ptr, png_row_info * row_info, png_byte * row);
#endif
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
/* Expand grayscale files to RGB, with or without alpha */
void /* PRIVATE */
png_do_gray_to_rgb_2(png_row_info * row_info, png_byte * row);
#endif
#ifdef PNG_READ_BACKGROUND_SUPPORTED
/* Replace any alpha or transparency with the supplied background color.
 * "background" is already in the screen gamma, while "background_1" is
 * at a gamma of 1.0.  Paletted files have already been taken care of.
 */
void /* PRIVATE */
png_do_background_2(png_row_info * row_info, png_byte * row,
    png_color_16 * trans_color, png_color_16 * background
#ifdef PNG_READ_GAMMA_SUPPORTED
    , png_color_16 * background_1, png_byte * gamma_table,
    png_byte * gamma_from_1, png_byte * gamma_to_1,
    png_uint_16 ** gamma_16, png_uint_16 ** gamma_16_from_1,
    png_uint_16 ** gamma_16_to_1, int gamma_shift
#endif
    );
#endif
#ifdef PNG_READ_GAMMA_SUPPORTED
/* Gamma correct the image, avoiding the alpha channel.  Make sure
 * you do this after you deal with the transparency issue on grayscale
 * or RGB images. If your bit depth is 8, use gamma_table, if it
 * is 16, use gamma_16_table and gamma_shift.  Build these with
 * build_gamma_table().
 */
void /* PRIVATE */
png_do_gamma_2(png_row_info * row_info, png_byte * row,
    png_byte * gamma_table, png_uint_16 ** gamma_16_table,
    int gamma_shift);
#endif
#ifdef PNG_READ_16_TO_8_SUPPORTED
/* Chop rows of bit depth 16 down to 8 */
void /* PRIVATE */
png_do_chop_2(png_row_info * row_info, png_byte * row);
#endif
#ifdef PNG_READ_QUANTIZE_SUPPORTED
void /* PRIVATE */
png_do_quantize_2(png_row_info * row_info, png_byte * row,
    png_byte * palette_lookup, png_byte * quantize_lookup);
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
/* Function to format a fixed point value in ASCII.
 */
void /* PRIVATE */
png_ascii_from_fixed_2(png_struct * png_ptr, char * ascii, png_size_t size,
    png_fixed_point fp);
#endif
int png_check_fp_string_2(char * string, png_size_t size);
int /* PRIVATE */
png_check_fp_number_2(char * string, png_size_t size, int * statep,
   png_size_tp whereami);
void /* PRIVATE */
png_do_invert_2(png_row_info * row_info, png_byte * row);
#ifdef PNG_READ_SHIFT_SUPPORTED
/* Reverse the effects of png_do_shift.  This routine merely shifts the
 * pixels back to their significant bits values.  Thus, if you have
 * a row of bit depth 8, but only 5 are significant, this will shift
 * the values back to 0 through 31.
 */
void /* PRIVATE */
png_do_unshift_2(png_row_info * row_info, png_byte * row,
    png_color_8 * sig_bits);
#endif
void /* PRIVATE */
png_do_unpack_2(png_row_info * row_info, png_byte * row);
#if defined(PNG_READ_BGR_SUPPORTED) || defined(PNG_WRITE_BGR_SUPPORTED)
/* Swaps red and blue bytes within a pixel */
void /* PRIVATE */
png_do_bgr_2(png_row_info * row_info, png_byte * row);
#endif

#if defined(PNG_READ_PACKSWAP_SUPPORTED)||defined(PNG_WRITE_PACKSWAP_SUPPORTED)
void /* PRIVATE */
png_do_packswap_2(png_row_info * row_info, png_byte * row);
#endif
#ifdef PNG_READ_FILLER_SUPPORTED
/* Add filler channel if we have RGB color */
void /* PRIVATE */
png_do_read_filler_2(png_row_info * row_info, png_byte * row,
    png_uint_32 filler, png_uint_32 flags);
#endif
#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
void /* PRIVATE */
png_do_read_invert_alpha_2(png_row_info * row_info, png_byte * row);
#endif
#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
void /* PRIVATE */
png_do_read_swap_alpha_2(png_row_info * row_info, png_byte * row);
#endif
#ifdef PNG_16BIT_SUPPORTED
#if defined(PNG_READ_SWAP_SUPPORTED) || defined(PNG_WRITE_SWAP_SUPPORTED)
/* Swaps byte order on 16 bit depth images */
void /* PRIVATE */
png_do_swap_2(png_row_info * row_info, png_byte * row);
#endif
#endif
void /* PRIVATE */
png_read_filter_row_2(png_struct * png_ptr, png_row_info * row_info, png_byte * row,
    png_byte * prev_row, int filter);
#ifdef PNG_MNG_FEATURES_SUPPORTED
/* Undoes intrapixel differencing  */
void /* PRIVATE */
png_do_read_intrapixel_2(png_row_info * row_info, png_byte * row);
#endif
#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
void png_read_info_2(png_struct * png_ptr, png_info * info_ptr);
#endif
int /* PRIVATE */
png_check_cHRM_fixed_2(png_struct * png_ptr,
   png_fixed_point white_x, png_fixed_point white_y, png_fixed_point red_x,
   png_fixed_point red_y, png_fixed_point green_x, png_fixed_point green_y,
   png_fixed_point blue_x, png_fixed_point blue_y);
#ifdef PNG_cHRM_SUPPORTED
void png_set_cHRM_fixed_2(png_struct * png_ptr, png_info * info_ptr,
    png_fixed_point white_x, png_fixed_point white_y, png_fixed_point red_x,
    png_fixed_point red_y, png_fixed_point green_x, png_fixed_point green_y,
    png_fixed_point blue_x, png_fixed_point blue_y);
#endif
void /* PRIVATE */
png_ascii_from_fp_2(png_struct * png_ptr, char * ascii, png_size_t size,
    double fp, unsigned int precision);
int /* PRIVATE */
png_set_text_3(png_struct * png_ptr, png_info * info_ptr,
    png_text * text_ptr, int num_text);
#ifdef PNG_READ_16_TO_8_SUPPORTED
/* Strip 16 bit depth files to 8 bit depth */
void png_set_strip_16_2(png_struct * png_ptr);
#endif
#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
void png_set_strip_alpha_2(png_struct * png_ptr);
#endif
#if defined(PNG_READ_PACK_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
/* Turn on pixel packing */
void png_set_packing_2(png_struct * png_ptr);
#endif
#if defined(PNG_READ_PACKSWAP_SUPPORTED)||defined(PNG_WRITE_PACKSWAP_SUPPORTED)
/* Turn on packed pixel swapping */
void png_set_packswap_2(png_struct * png_ptr);
#endif
png_uint_32 png_get_valid_2(png_struct * png_ptr, png_info * info_ptr,
    png_uint_32 flag);
void png_set_expand_2(png_struct * png_ptr);
void png_set_invert_mono_2(png_struct * png_ptr);
#ifdef PNG_sBIT_SUPPORTED
png_uint_32 png_get_sBIT_2(png_struct * png_ptr, png_info * info_ptr,
    png_color_8 ** sig_bit);
#endif
#if defined(PNG_READ_SHIFT_SUPPORTED) || defined(PNG_WRITE_SHIFT_SUPPORTED)
void png_set_shift_2(png_struct * png_ptr, png_color_8 * true_bits);
#endif
#if defined(PNG_READ_BGR_SUPPORTED) || defined(PNG_WRITE_BGR_SUPPORTED)
/* Turn on BGR-to-RGB mapping */
void png_set_bgr_2(png_struct * png_ptr);
#endif
#if defined(PNG_READ_SWAP_ALPHA_SUPPORTED) || \
    defined(PNG_WRITE_SWAP_ALPHA_SUPPORTED)
void png_set_swap_alpha_2(png_struct * png_ptr);
#endif
#if defined(PNG_READ_SWAP_SUPPORTED) || defined(PNG_WRITE_SWAP_SUPPORTED)
/* Turn on 16 bit byte swapping */
void png_set_swap_2(png_struct * png_ptr);
#endif
#if defined(PNG_READ_INVERT_ALPHA_SUPPORTED) || \
    defined(PNG_WRITE_INVERT_ALPHA_SUPPORTED)
void png_set_invert_alpha_2(png_struct * png_ptr);
#endif
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
void png_set_gray_to_rgb_2(png_struct * png_ptr);
#endif
void /* PRIVATE */
png_read_transform_info_2(png_struct * png_ptr, png_info * info_ptr);
void /* PRIVATE */
png_read_start_row_2(png_struct * png_ptr);
void /* PRIVATE */
png_init_read_transformations_2(png_struct * png_ptr);
void * 
png_create_struct_3(int type, void * (* malloc_fn)(png_struct *, 
    png_alloc_size_t), void * mem_ptr);
void * png_create_struct_4(int type);
/* Optional call to update the users info_ptr structure */
void png_read_update_info_2(png_struct * png_ptr, png_info * info_ptr);
int /* PRIVATE */
png_crc_error_2(png_struct * png_ptr);
void 
png_destroy_struct_3(void * struct_ptr, void (* free_fn)(png_struct *, void *),
    void * mem_ptr);
void /* PRIVATE */
png_destroy_struct_4(void * struct_ptr);    
void png_read_end_2(png_struct * png_ptr, png_info * info_ptr);

png_uint_32 png_get_image_width_2(png_struct * png_ptr, png_info * info_ptr);
png_uint_32 png_get_image_height_2(png_structp png_ptr, png_info * info_ptr);
png_byte png_get_bit_depth_2(png_struct * png_ptr, png_info * info_ptr);
png_byte png_get_color_type_2(png_struct * png_ptr, png_info * info_ptr);
png_byte png_get_filter_type_2(png_struct * png_ptr, png_info * info_ptr);
png_byte png_get_interlace_type_2(png_struct * png_ptr, png_info * info_ptr);
png_byte png_get_compression_type_2(png_struct * png_ptr, png_info * info_ptr);
png_uint_32 png_get_x_pixels_per_meter_2(png_struct * png_ptr, png_info * info_ptr);
png_uint_32 png_get_y_pixels_per_meter_2(png_struct * png_ptr, png_info * info_ptr);
void png_read_row_2(png_struct * png_ptr, png_byte * row, png_byte * dsp_row);
void png_warning_2(png_struct * png_ptr, char * message);
#define _YGGDRASIL_PNG_H
#endif