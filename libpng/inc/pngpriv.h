
/* pngpriv.h - private declarations for use inside libpng
 *
 * For conditions of distribution and use, see copyright notice in png.h
 * Copyright (c) 1998-2011 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * Last changed in libpng 1.5.0 [January 6, 2011]
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 */

/* The symbols declared in this file (including the functions declared
 * as extern) are PRIVATE.  They are not part of the libpng public
 * interface, and are not recommended for use by regular applications.
 * Some of them may become public in the future; others may stay private,
 * change in an incompatible way, or even disappear.
 * Although the libpng users are not forbidden to include this header,
 * they should be well aware of the issues that may arise from doing so.
 */

#ifndef PNGPRIV_H

/* This is required for the definition of abort(), used as a last ditch
 * error handler when all else fails.
 */
#include <stdlib.h>

#define PNGLIB_BUILD
#include "png.h"
#include "pnginfo.h"
#include "pngstruct.h"
//#include "midgard.h"

/* This is used for 16 bit gamma tables - only the top level pointers are const,
 * this could be changed:
 */
typedef PNG_CONST png_uint_16p * png_const_uint_16pp;

#include <float.h>
#include <math.h>

/* Just a little check that someone hasn't tried to define something
 * contradictory.
 */
#if (PNG_ZBUF_SIZE > 65536L) && defined(PNG_MAX_MALLOC_64K)
#  undef PNG_ZBUF_SIZE
#  define PNG_ZBUF_SIZE 65536L
#endif

/* If warnings or errors are turned off the code is disabled
 * or redirected here.
 */
#ifndef PNG_WARNINGS_SUPPORTED
#define png_warning(s1,s2) ((void)0)
#define png_chunk_warning(s1,s2) ((void)0)
#endif
#ifndef PNG_ERROR_TEXT_SUPPORTED
#define png_error(s1,s2) png_err(s1)
#define png_chunk_error(s1,s2) png_err(s1)
#define png_fixed_error(s1,s2) png_err(s1)
#endif

#define CVT_PTR(ptr)         (ptr)
#define CVT_PTR_NOCHECK(ptr) (ptr)
#define png_strcpy  strcpy
#define png_strncpy strncpy     /* Added to v 1.2.6 */
#define png_strlen  strlen
#define png_memcmp  memcmp      /* SJT: added */
#define png_memcpy  memcpy
#define png_memset  memset
#define png_sprintf sprintf

/* End of memory model/platform independent support */

#define png_snprintf snprintf   /* Added to v 1.2.19 */
#define png_snprintf2 snprintf
#define png_snprintf6 snprintf
/* End of 1.5.0beta36 move from pngconf.h */

/* CONSTANTS and UTILITY MACROS
 * These are used internally by libpng and not exposed in the API
 */

/* Various modes of operation.  Note that after an init, mode is set to
 * zero automatically when the structure is created.
 */
#define PNG_HAVE_IHDR               0x01
#define PNG_HAVE_PLTE               0x02
#define PNG_HAVE_IDAT               0x04
#define PNG_AFTER_IDAT              0x08 /* Have complete zlib datastream */
#define PNG_HAVE_IEND               0x10
#define PNG_HAVE_gAMA               0x20
#define PNG_HAVE_cHRM               0x40
#define PNG_HAVE_sRGB               0x80
#define PNG_HAVE_CHUNK_HEADER      0x100
#define PNG_WROTE_tIME             0x200
#define PNG_WROTE_INFO_BEFORE_PLTE 0x400
#define PNG_BACKGROUND_IS_GRAY     0x800
#define PNG_HAVE_PNG_SIGNATURE    0x1000
#define PNG_HAVE_CHUNK_AFTER_IDAT 0x2000 /* Have another chunk after IDAT */

/* Flags for the transformations the PNG library does on the image data */
#define PNG_BGR                 0x0001
#define PNG_INTERLACE           0x0002
#define PNG_PACK                0x0004
#define PNG_SHIFT               0x0008
#define PNG_SWAP_BYTES          0x0010
#define PNG_INVERT_MONO         0x0020
#define PNG_QUANTIZE            0x0040
#define PNG_BACKGROUND          0x0080
#define PNG_BACKGROUND_EXPAND   0x0100
                          /*    0x0200 unused */
#define PNG_16_TO_8             0x0400
#define PNG_RGBA                0x0800
#define PNG_EXPAND              0x1000
#define PNG_GAMMA               0x2000
#define PNG_GRAY_TO_RGB         0x4000
#define PNG_FILLER              0x8000L
#define PNG_PACKSWAP           0x10000L
#define PNG_SWAP_ALPHA         0x20000L
#define PNG_STRIP_ALPHA        0x40000L
#define PNG_INVERT_ALPHA       0x80000L
#define PNG_USER_TRANSFORM    0x100000L
#define PNG_RGB_TO_GRAY_ERR   0x200000L
#define PNG_RGB_TO_GRAY_WARN  0x400000L
#define PNG_RGB_TO_GRAY       0x600000L  /* two bits, RGB_TO_GRAY_ERR|WARN */
                       /*     0x800000L     Unused */
#define PNG_ADD_ALPHA         0x1000000L  /* Added to libpng-1.2.7 */
#define PNG_EXPAND_tRNS       0x2000000L  /* Added to libpng-1.2.9 */
                       /*   0x4000000L  unused */
                       /*   0x8000000L  unused */
                       /*  0x10000000L  unused */
                       /*  0x20000000L  unused */
                       /*  0x40000000L  unused */

/* Flags for png_create_struct */
#define PNG_STRUCT_PNG   0x0001
#define PNG_STRUCT_INFO  0x0002

/* Scaling factor for filter heuristic weighting calculations */
#define PNG_WEIGHT_FACTOR (1<<(PNG_WEIGHT_SHIFT))
#define PNG_COST_FACTOR (1<<(PNG_COST_SHIFT))

/* Flags for the png_ptr->flags rather than declaring a byte for each one */
#define PNG_FLAG_ZLIB_CUSTOM_STRATEGY     0x0001
#define PNG_FLAG_ZLIB_CUSTOM_LEVEL        0x0002
#define PNG_FLAG_ZLIB_CUSTOM_MEM_LEVEL    0x0004
#define PNG_FLAG_ZLIB_CUSTOM_WINDOW_BITS  0x0008
#define PNG_FLAG_ZLIB_CUSTOM_METHOD       0x0010
#define PNG_FLAG_ZLIB_FINISHED            0x0020
#define PNG_FLAG_ROW_INIT                 0x0040
#define PNG_FLAG_FILLER_AFTER             0x0080
#define PNG_FLAG_CRC_ANCILLARY_USE        0x0100
#define PNG_FLAG_CRC_ANCILLARY_NOWARN     0x0200
#define PNG_FLAG_CRC_CRITICAL_USE         0x0400
#define PNG_FLAG_CRC_CRITICAL_IGNORE      0x0800
                                /*        0x1000  unused */
                                /*        0x2000  unused */
                                /*        0x4000  unused */
#define PNG_FLAG_KEEP_UNKNOWN_CHUNKS      0x8000L
#define PNG_FLAG_KEEP_UNSAFE_CHUNKS       0x10000L
#define PNG_FLAG_LIBRARY_MISMATCH         0x20000L
#define PNG_FLAG_STRIP_ERROR_NUMBERS      0x40000L
#define PNG_FLAG_STRIP_ERROR_TEXT         0x80000L
#define PNG_FLAG_MALLOC_NULL_MEM_OK       0x100000L
#define PNG_FLAG_ADD_ALPHA                0x200000L  /* Added to libpng-1.2.8 */
#define PNG_FLAG_STRIP_ALPHA              0x400000L  /* Added to libpng-1.2.8 */
#define PNG_FLAG_BENIGN_ERRORS_WARN       0x800000L  /* Added to libpng-1.4.0 */
                                  /*     0x1000000L  unused */
                                  /*     0x2000000L  unused */
                                  /*     0x4000000L  unused */
                                  /*     0x8000000L  unused */
                                  /*    0x10000000L  unused */
                                  /*    0x20000000L  unused */
                                  /*    0x40000000L  unused */

#define PNG_FLAG_CRC_ANCILLARY_MASK (PNG_FLAG_CRC_ANCILLARY_USE | \
                                     PNG_FLAG_CRC_ANCILLARY_NOWARN)

#define PNG_FLAG_CRC_CRITICAL_MASK  (PNG_FLAG_CRC_CRITICAL_USE | \
                                     PNG_FLAG_CRC_CRITICAL_IGNORE)

#define PNG_FLAG_CRC_MASK           (PNG_FLAG_CRC_ANCILLARY_MASK | \
                                     PNG_FLAG_CRC_CRITICAL_MASK)

/* zlib.h declares a magic type 'uInt' that limits the amount of data that zlib
 * can handle at once.  This type need be no larger than 16 bits (so maximum of
 * 65535), this define allows us to discover how big it is, but limited by the
 * maximuum for png_size_t.  The value can be overriden in a library build
 * (pngusr.h, or set it in CPPFLAGS) and it works to set it to a considerably
 * lower value (e.g. 255 works).  A lower value may help memory usage (slightly)
 * and may even improve performance on some systems (and degrade it on others.)
 */
#ifndef ZLIB_IO_MAX
#  define ZLIB_IO_MAX ((uInt)-1)
#endif

/* Save typing and make code easier to understand */

#define PNG_COLOR_DIST(c1, c2) (abs((int)((c1).red) - (int)((c2).red)) + \
   abs((int)((c1).green) - (int)((c2).green)) + \
   abs((int)((c1).blue) - (int)((c2).blue)))

/* Added to libpng-1.2.6 JB */
#define PNG_ROWBYTES(pixel_bits, width) \
    ((pixel_bits) >= 8 ? \
    ((png_size_t)(width) * (((png_size_t)(pixel_bits)) >> 3)) : \
    (( ((png_size_t)(width) * ((png_size_t)(pixel_bits))) + 7) >> 3) )

/* PNG_OUT_OF_RANGE returns true if value is outside the range
 * ideal-delta..ideal+delta.  Each argument is evaluated twice.
 * "ideal" and "delta" should be constants, normally simple
 * integers, "value" a variable. Added to libpng-1.2.6 JB
 */
#define PNG_OUT_OF_RANGE(value, ideal, delta) \
   ( (value) < (ideal)-(delta) || (value) > (ideal)+(delta) )

/* Conversions between fixed and floating point, only defined if
 * required (to make sure the code doesn't accidentally use float
 * when it is supposedly disabled.)
 */
#ifdef PNG_FLOATING_POINT_SUPPORTED
/* The floating point conversion can't overflow, though it can and
 * does lose accuracy relative to the original fixed point value.
 * In practice this doesn't matter because png_fixed_point only
 * stores numbers with very low precision.  The png_ptr and s
 * arguments are unused by default but are there in case error
 * checking becomes a requirement.
 */
#define png_float(png_ptr, fixed, s) (.00001 * (fixed))

/* The fixed point conversion performs range checking and evaluates
 * its argument multiple times, so must be used with care.  The
 * range checking uses the PNG specification values for a signed
 * 32 bit fixed point value except that the values are deliberately
 * rounded-to-zero to an integral value - 21474 (21474.83 is roughly
 * (2^31-1) * 100000). 's' is a string that describes the value being
 * converted.
 *
 * NOTE: this macro will raise a png_error if the range check fails,
 * therefore it is normally only appropriate to use this on values
 * that come from API calls or other sources where an out of range
 * error indicates a programming error, not a data error!
 *
 * NOTE: by default this is off - the macro is not used - because the
 * function call saves a lot of code.
 */
#ifdef PNG_FIXED_POINT_MACRO_SUPPORTED
#define png_fixed(png_ptr, fp, s) ((fp) <= 21474 && (fp) >= -21474 ?\
    ((png_fixed_point)(100000 * (fp))) : (png_fixed_error(png_ptr, s),0))
#else
extern png_fixed_point png_fixed PNGARG((png_structp png_ptr, double fp,
   png_const_charp text));
#endif
#endif

/* Constant strings for known chunk types.  If you need to add a chunk,
 * define the name here, and add an invocation of the macro wherever it's
 * needed.
 */
#define PNG_IHDR PNG_CONST png_byte png_IHDR[5] = { 73,  72,  68,  82, '\0'}
#define PNG_IDAT PNG_CONST png_byte png_IDAT[5] = { 73,  68,  65,  84, '\0'}
#define PNG_IEND PNG_CONST png_byte png_IEND[5] = { 73,  69,  78,  68, '\0'}
#define PNG_PLTE PNG_CONST png_byte png_PLTE[5] = { 80,  76,  84,  69, '\0'}
#define PNG_bKGD PNG_CONST png_byte png_bKGD[5] = { 98,  75,  71,  68, '\0'}
#define PNG_cHRM PNG_CONST png_byte png_cHRM[5] = { 99,  72,  82,  77, '\0'}
#define PNG_gAMA PNG_CONST png_byte png_gAMA[5] = {103,  65,  77,  65, '\0'}
#define PNG_hIST PNG_CONST png_byte png_hIST[5] = {104,  73,  83,  84, '\0'}
#define PNG_iCCP PNG_CONST png_byte png_iCCP[5] = {105,  67,  67,  80, '\0'}
#define PNG_iTXt PNG_CONST png_byte png_iTXt[5] = {105,  84,  88, 116, '\0'}
#define PNG_oFFs PNG_CONST png_byte png_oFFs[5] = {111,  70,  70, 115, '\0'}
#define PNG_pCAL PNG_CONST png_byte png_pCAL[5] = {112,  67,  65,  76, '\0'}
#define PNG_sCAL PNG_CONST png_byte png_sCAL[5] = {115,  67,  65,  76, '\0'}
#define PNG_pHYs PNG_CONST png_byte png_pHYs[5] = {112,  72,  89, 115, '\0'}
#define PNG_sBIT PNG_CONST png_byte png_sBIT[5] = {115,  66,  73,  84, '\0'}
#define PNG_sPLT PNG_CONST png_byte png_sPLT[5] = {115,  80,  76,  84, '\0'}
#define PNG_sRGB PNG_CONST png_byte png_sRGB[5] = {115,  82,  71,  66, '\0'}
#define PNG_sTER PNG_CONST png_byte png_sTER[5] = {115,  84,  69,  82, '\0'}
#define PNG_tEXt PNG_CONST png_byte png_tEXt[5] = {116,  69,  88, 116, '\0'}
#define PNG_tIME PNG_CONST png_byte png_tIME[5] = {116,  73,  77,  69, '\0'}
#define PNG_tRNS PNG_CONST png_byte png_tRNS[5] = {116,  82,  78,  83, '\0'}
#define PNG_zTXt PNG_CONST png_byte png_zTXt[5] = {122,  84,  88, 116, '\0'}

/* These functions are used internally in the code.  They generally
 * shouldn't be used unless you are writing code to add or replace some
 * functionality in libpng.  More information about most functions can
 * be found in the files where the functions are located.
 */

/* Allocate memory for an internal libpng struct */
extern png_voidp png_create_struct(int type);

/* Free memory from internal libpng struct */
extern void png_destroy_struct(png_voidp struct_ptr);

extern png_voidp png_create_struct_2
   (int type, png_malloc_ptr malloc_fn, png_voidp mem_ptr);
extern void png_destroy_struct_2(png_voidp struct_ptr,
    png_free_ptr free_fn, png_voidp mem_ptr);

/* Free any memory that info_ptr points to and reset struct. */
extern void png_info_destroy(png_structp png_ptr,
    png_infop info_ptr);

/* Function to allocate memory for zlib.  PNGAPI is disallowed. */
extern voidpf png_zalloc(voidpf png_ptr, uInt items,
   uInt size);

/* Function to free memory for zlib.  PNGAPI is disallowed. */
extern void png_zfree(voidpf png_ptr, voidpf ptr);

/* Next four functions are used internally as callbacks.  PNGCBAPI is required
 * but not PNG_EXPORT.  PNGAPI added at libpng version 1.2.3, changed to
 * PNGCBAPI at 1.5.0
 */

extern void png_default_read_data(png_structp png_ptr,
    png_bytep data, png_size_t length);

#ifdef PNG_PROGRESSIVE_READ_SUPPORTED
extern void png_push_fill_buffer(png_structp png_ptr,
    png_bytep buffer, png_size_t length);
#endif

extern void png_default_write_data(png_structp png_ptr,
    png_bytep data, png_size_t length);

#ifdef PNG_WRITE_FLUSH_SUPPORTED
#  ifdef PNG_STDIO_SUPPORTED
extern void png_default_flush(png_structp png_ptr);
#  endif
#endif

/* Reset the CRC variable */
extern void png_reset_crc(png_structp png_ptr);

/* Write the "data" buffer to whatever output you are using */
extern void png_write_data(png_structp png_ptr,
    png_const_bytep data, png_size_t length);

/* Read and check the PNG file signature */
extern void png_read_sig(png_structp png_ptr, png_infop info_ptr);

/* Read the chunk header (length + type name) */
extern png_uint_32 png_read_chunk_header(png_structp png_ptr);

/* Read data from whatever input you are using into the "data" buffer */
extern void png_read_data(png_structp png_ptr, png_bytep data,
    png_size_t length);

/* Read bytes into buf, and update png_ptr->crc */
extern void png_crc_read(png_structp png_ptr, png_bytep buf,
    png_size_t length);

/* Decompress data in a chunk that uses compression */
#if defined(PNG_zTXt_SUPPORTED) || defined(PNG_iTXt_SUPPORTED) || \
    defined(PNG_iCCP_SUPPORTED) || defined(PNG_sPLT_SUPPORTED)
extern void png_decompress_chunk(png_structp png_ptr,
    int comp_type, png_size_t chunklength, png_size_t prefix_length,
    png_size_t *data_length);
#endif

/* Read "skip" bytes, read the file crc, and (optionally) verify png_ptr->crc */
extern int png_crc_finish(png_structp png_ptr, png_uint_32 skip);

/* Read the CRC from the file and compare it to the libpng calculated CRC */
extern int png_crc_error(png_structp png_ptr);

/* Calculate the CRC over a section of data.  Note that we are only
 * passing a maximum of 64K on systems that have this as a memory limit,
 * since this is the maximum buffer size we can specify.
 */
extern void png_calculate_crc(png_structp png_ptr,
    png_const_bytep ptr, png_size_t length);

#ifdef PNG_WRITE_FLUSH_SUPPORTED
extern void png_flush(png_structp png_ptr);
#endif

/* Write various chunks */

/* Write the IHDR chunk, and update the png_struct with the necessary
 * information.
 */
extern void png_write_IHDR(png_structp png_ptr, png_uint_32 width,
    png_uint_32 height,
    int bit_depth, int color_type, int compression_method, int filter_method,
    int interlace_method);

extern void png_write_PLTE(png_structp png_ptr,
    png_const_colorp palette, png_uint_32 num_pal);

extern void png_write_IDAT(png_structp png_ptr, png_bytep data,
    png_size_t length);

extern void png_write_IEND PNGARG((png_structp png_ptr));

#ifdef PNG_WRITE_gAMA_SUPPORTED
#  ifdef PNG_FLOATING_POINT_SUPPORTED
extern void png_write_gAMA(png_structp png_ptr, double file_gamma);
#  endif
#  ifdef PNG_FIXED_POINT_SUPPORTED
extern void png_write_gAMA_fixed(png_structp png_ptr,
    png_fixed_point file_gamma);
#  endif
#endif

#ifdef PNG_WRITE_sBIT_SUPPORTED
extern void png_write_sBIT(png_structp png_ptr,
    png_const_color_8p sbit, int color_type);
#endif

#ifdef PNG_WRITE_cHRM_SUPPORTED
#  ifdef PNG_FLOATING_POINT_SUPPORTED
extern void png_write_cHRM(png_structp png_ptr,
    double white_x, double white_y,
    double red_x, double red_y, double green_x, double green_y,
    double blue_x, double blue_y);
#  endif
extern void png_write_cHRM_fixed(png_structp png_ptr,
    png_fixed_point int_white_x, png_fixed_point int_white_y,
    png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point
    int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x,
    png_fixed_point int_blue_y);
#endif

#ifdef PNG_WRITE_sRGB_SUPPORTED
extern void png_write_sRGB(png_structp png_ptr,
    int intent);
#endif

#ifdef PNG_WRITE_iCCP_SUPPORTED
extern void png_write_iCCP(png_structp png_ptr,
    png_const_charp name, int compression_type,
    png_const_charp profile, int proflen);
   /* Note to maintainer: profile should be png_bytep */
#endif

#ifdef PNG_WRITE_sPLT_SUPPORTED
extern void png_write_sPLT(png_structp png_ptr,
    png_const_sPLT_tp palette);
#endif

#ifdef PNG_WRITE_tRNS_SUPPORTED
extern void png_write_tRNS(png_structp png_ptr,
    png_const_bytep trans, png_const_color_16p values, int number,
    int color_type);
#endif

#ifdef PNG_WRITE_bKGD_SUPPORTED
extern void png_write_bKGD(png_structp png_ptr,
    png_const_color_16p values, int color_type);
#endif

#ifdef PNG_WRITE_hIST_SUPPORTED
extern void png_write_hIST(png_structp png_ptr,
    png_const_uint_16p hist, int num_hist);
#endif

#if defined(PNG_WRITE_TEXT_SUPPORTED) || defined(PNG_WRITE_pCAL_SUPPORTED) || \
    defined(PNG_WRITE_iCCP_SUPPORTED) || defined(PNG_WRITE_sPLT_SUPPORTED)
extern png_size_t png_check_keyword(png_structp png_ptr,
    png_const_charp key, png_charpp new_key);
#endif

#ifdef PNG_WRITE_tEXt_SUPPORTED
extern void png_write_tEXt(png_structp png_ptr, png_const_charp key,
    png_const_charp text, png_size_t text_len);
#endif

#ifdef PNG_WRITE_zTXt_SUPPORTED
extern void png_write_zTXt(png_structp png_ptr, png_const_charp key,
    png_const_charp text, png_size_t text_len, int compression);
#endif

#ifdef PNG_WRITE_iTXt_SUPPORTED
extern void png_write_iTXt(png_structp png_ptr,
    int compression, png_const_charp key, png_const_charp lang,
    png_const_charp lang_key, png_const_charp text);
#endif

#ifdef PNG_TEXT_SUPPORTED  /* Added at version 1.0.14 and 1.2.4 */
extern int png_set_text_2(png_structp png_ptr,
    png_infop info_ptr, png_const_textp text_ptr, int num_text);
#endif

#ifdef PNG_WRITE_oFFs_SUPPORTED
extern void png_write_oFFs(png_structp png_ptr,
    png_int_32 x_offset, png_int_32 y_offset, int unit_type);
#endif

#ifdef PNG_WRITE_pCAL_SUPPORTED
extern void png_write_pCAL(png_structp png_ptr, png_charp purpose,
    png_int_32 X0, png_int_32 X1, int type, int nparams,
    png_const_charp units, png_charpp params);
#endif

#ifdef PNG_WRITE_pHYs_SUPPORTED
extern void png_write_pHYs(png_structp png_ptr,
    png_uint_32 x_pixels_per_unit, png_uint_32 y_pixels_per_unit,
    int unit_type);
#endif

#ifdef PNG_WRITE_tIME_SUPPORTED
extern void png_write_tIME(png_structp png_ptr,
    png_const_timep mod_time);
#endif

#ifdef PNG_WRITE_sCAL_SUPPORTED
extern void png_write_sCAL_s(png_structp png_ptr,
    int unit, png_const_charp width, png_const_charp height);
#endif

/* Called when finished processing a row of data */
extern void png_write_finish_row(png_structp png_ptr);

/* Internal use only.   Called before first row of data */
extern void png_write_start_row(png_structp png_ptr);

/* Combine a row of data, dealing with alpha, etc. if requested */
extern void png_combine_row(png_structp png_ptr, png_bytep row,
    int mask);

#ifdef PNG_READ_INTERLACING_SUPPORTED
/* Expand an interlaced row */
/* OLD pre-1.0.9 interface:
extern void png_do_read_interlace PNGARG((png_row_infop row_info,
    png_bytep row, int pass, png_uint_32 transformations));
 */
extern void png_do_read_interlace(png_structp png_ptr);
#endif

/* GRR TO DO (2.0 or whenever):  simplify other internal calling interfaces */

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
/* Grab pixels out of a row for an interlaced pass */
extern void png_do_write_interlace(png_row_infop row_info,
    png_bytep row, int pass);
#endif

/* Unfilter a row */
extern void png_read_filter_row(png_structp png_ptr,
    png_row_infop row_info, png_bytep row, png_const_bytep prev_row,
    int filter);

/* Choose the best filter to use and filter the row data */
extern void png_write_find_filter(png_structp png_ptr,
    png_row_infop row_info);

/* Write out the filtered row. */
extern void png_write_filtered_row(png_structp png_ptr,
    png_bytep filtered_row);
/* Finish a row while reading, dealing with interlacing passes, etc. */
extern void png_read_finish_row(png_structp png_ptr);

/* Initialize the row buffers, etc. */
extern void png_read_start_row(png_structp png_ptr);
/* Optional call to update the users info structure */
extern void png_read_transform_info(png_structp png_ptr,
    png_infop info_ptr);

/* These are the functions that do the transformations */
#ifdef PNG_READ_FILLER_SUPPORTED
extern void png_do_read_filler(png_row_infop row_info,
    png_bytep row, png_uint_32 filler, png_uint_32 flags);
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
extern void png_do_read_swap_alpha(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_WRITE_SWAP_ALPHA_SUPPORTED
extern void png_do_write_swap_alpha(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
extern void png_do_read_invert_alpha(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
extern void png_do_write_invert_alpha(png_row_infop row_info,
    png_bytep row);
#endif

#if defined(PNG_WRITE_FILLER_SUPPORTED) || \
    defined(PNG_READ_STRIP_ALPHA_SUPPORTED)
extern void png_do_strip_filler(png_row_infop row_info,
    png_bytep row, png_uint_32 flags);
#endif

#ifdef PNG_16BIT_SUPPORTED
#if defined(PNG_READ_SWAP_SUPPORTED) || defined(PNG_WRITE_SWAP_SUPPORTED)
extern void png_do_swap(png_row_infop row_info,
    png_bytep row);
#endif
#endif

#if defined(PNG_READ_PACKSWAP_SUPPORTED) || \
    defined(PNG_WRITE_PACKSWAP_SUPPORTED)
extern void png_do_packswap(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
extern int png_do_rgb_to_gray(png_structp png_ptr,
    png_row_infop row_info, png_bytep row);
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
extern void png_do_gray_to_rgb(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_READ_PACK_SUPPORTED
extern void png_do_unpack(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_READ_SHIFT_SUPPORTED
extern void png_do_unshift(png_row_infop row_info,
    png_bytep row, png_const_color_8p sig_bits);
#endif

#if defined(PNG_READ_INVERT_SUPPORTED) || defined(PNG_WRITE_INVERT_SUPPORTED)
extern void png_do_invert(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_READ_16_TO_8_SUPPORTED
extern void png_do_chop(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
extern void png_do_quantize(png_row_infop row_info,
    png_bytep row, png_const_bytep palette_lookup,
    png_const_bytep quantize_lookup);

#  ifdef PNG_CORRECT_PALETTE_SUPPORTED
extern void png_correct_palette(png_structp png_ptr,
    png_colorp palette, int num_palette);
#  endif
#endif

#if defined(PNG_READ_BGR_SUPPORTED) || defined(PNG_WRITE_BGR_SUPPORTED)
extern void png_do_bgr(png_row_infop row_info,
    png_bytep row);
#endif

#ifdef PNG_WRITE_PACK_SUPPORTED
extern void png_do_pack(png_row_infop row_info,
   png_bytep row, png_uint_32 bit_depth);
#endif

#ifdef PNG_WRITE_SHIFT_SUPPORTED
extern void png_do_shift(png_row_infop row_info,
    png_bytep row, png_const_color_8p bit_depth);
#endif

#ifdef PNG_READ_BACKGROUND_SUPPORTED
#  ifdef PNG_READ_GAMMA_SUPPORTED
extern void png_do_background(png_row_infop row_info,
    png_bytep row, png_const_color_16p trans_color,
    png_const_color_16p background, png_const_color_16p background_1,
    png_const_bytep gamma_table, png_const_bytep gamma_from_1,
    png_const_bytep gamma_to_1, png_const_uint_16pp gamma_16,
    png_const_uint_16pp gamma_16_from_1, png_const_uint_16pp gamma_16_to_1,
    int gamma_shift);
#  else
extern void png_do_background(png_row_infop row_info,
    png_bytep row, png_const_color_16p trans_color,
    png_const_color_16p background);
#  endif
#endif

#ifdef PNG_READ_GAMMA_SUPPORTED
extern void png_do_gamma(png_row_infop row_info,
    png_bytep row, png_const_bytep gamma_table,
    png_const_uint_16pp gamma_16_table, int gamma_shift);
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
extern void png_do_expand_palette(png_row_infop row_info,
    png_bytep row, png_const_colorp palette, png_const_bytep trans,
    int num_trans);
extern void png_do_expand(png_row_infop row_info,
    png_bytep row, png_const_color_16p trans_color);
#endif

/* The following decodes the appropriate chunks, and does error correction,
 * then calls the appropriate callback for the chunk if it is valid.
 */

/* Decode the IHDR chunk */
extern void png_handle_IHDR(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
extern void png_handle_PLTE(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
extern void png_handle_IEND(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);

#ifdef PNG_READ_bKGD_SUPPORTED
extern void png_handle_bKGD(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_cHRM_SUPPORTED
extern void png_handle_cHRM(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_gAMA_SUPPORTED
extern void png_handle_gAMA(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_hIST_SUPPORTED
extern void png_handle_hIST(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_iCCP_SUPPORTED
extern void png_handle_iCCP(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif /* PNG_READ_iCCP_SUPPORTED */

#ifdef PNG_READ_iTXt_SUPPORTED
extern void png_handle_iTXt(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_oFFs_SUPPORTED
extern void png_handle_oFFs(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_pCAL_SUPPORTED
extern void png_handle_pCAL(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_pHYs_SUPPORTED
extern void png_handle_pHYs(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_sBIT_SUPPORTED
extern void png_handle_sBIT(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_sCAL_SUPPORTED
extern void png_handle_sCAL(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_sPLT_SUPPORTED
extern void png_handle_sPLT(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif /* PNG_READ_sPLT_SUPPORTED */

#ifdef PNG_READ_sRGB_SUPPORTED
extern void png_handle_sRGB(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_tEXt_SUPPORTED
extern void png_handle_tEXt(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_tIME_SUPPORTED
extern void png_handle_tIME(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_tRNS_SUPPORTED
extern void png_handle_tRNS(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

#ifdef PNG_READ_zTXt_SUPPORTED
extern void png_handle_zTXt(png_structp png_ptr, png_infop info_ptr,
    png_uint_32 length);
#endif

extern void png_handle_unknown(png_structp png_ptr,
    png_infop info_ptr, png_uint_32 length);

extern void png_check_chunk_name(png_structp png_ptr,
    png_const_bytep chunk_name);

/* Handle the transformations for reading and writing */
extern void png_do_read_transformations(png_structp png_ptr);
extern void png_do_write_transformations(png_structp png_ptr);

extern void png_init_read_transformations(png_structp png_ptr);

#ifdef PNG_PROGRESSIVE_READ_SUPPORTED
extern void png_push_read_chunk(png_structp png_ptr,
    png_infop info_ptr);
extern void png_push_read_sig(png_structp png_ptr,
    png_infop info_ptr);
extern void png_push_check_crc(png_structp png_ptr);
extern void png_push_crc_skip(png_structp png_ptr,
    png_uint_32 length);
extern void png_push_crc_finish(png_structp png_ptr);
extern void png_push_save_buffer(png_structp png_ptr);
extern void png_push_restore_buffer(png_structp png_ptr,
    png_bytep buffer, png_size_t buffer_length);
extern void png_push_read_IDAT(png_structp png_ptr);
extern void png_process_IDAT_data(png_structp png_ptr,
    png_bytep buffer, png_size_t buffer_length);
extern void png_push_process_row(png_structp png_ptr);
extern void png_push_handle_unknown(png_structp png_ptr,
   png_infop info_ptr, png_uint_32 length);
extern void png_push_have_info(png_structp png_ptr,
   png_infop info_ptr);
extern void png_push_have_end(png_structp png_ptr,
   png_infop info_ptr);
extern void png_push_have_row(png_structp png_ptr, png_bytep row);
extern void png_push_read_end(png_structp png_ptr,
    png_infop info_ptr);
extern void png_process_some_data(png_structp png_ptr,
    png_infop info_ptr);
extern void png_read_push_finish_row(png_structp png_ptr);
#  ifdef PNG_READ_tEXt_SUPPORTED
extern void png_push_handle_tEXt(png_structp png_ptr,
    png_infop info_ptr, png_uint_32 length);
extern void png_push_read_tEXt(png_structp png_ptr,
    png_infop info_ptr);
#  endif
#  ifdef PNG_READ_zTXt_SUPPORTED
extern void png_push_handle_zTXt(png_structp png_ptr,
    png_infop info_ptr, png_uint_32 length);
extern void png_push_read_zTXt(png_structp png_ptr,
    png_infop info_ptr);
#  endif
#  ifdef PNG_READ_iTXt_SUPPORTED
extern void png_push_handle_iTXt(png_structp png_ptr,
    png_infop info_ptr, png_uint_32 length);
extern void png_push_read_iTXt(png_structp png_ptr,
    png_infop info_ptr);
#  endif

#endif /* PNG_PROGRESSIVE_READ_SUPPORTED */

#ifdef PNG_MNG_FEATURES_SUPPORTED
extern void png_do_read_intrapixel(png_row_infop row_info,
    png_bytep row);
extern void png_do_write_intrapixel(png_row_infop row_info,
    png_bytep row);
#endif

/* Added at libpng version 1.4.0 */
#ifdef PNG_CHECK_cHRM_SUPPORTED
extern int png_check_cHRM_fixed(png_structp png_ptr,
    png_fixed_point int_white_x, png_fixed_point int_white_y,
    png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point
    int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x,
    png_fixed_point int_blue_y);
#endif

#ifdef PNG_CHECK_cHRM_SUPPORTED
/* Added at libpng version 1.2.34 and 1.4.0 */
/* Currently only used by png_check_cHRM_fixed */
extern void png_64bit_product(long v1, long v2,
    unsigned long *hi_product, unsigned long *lo_product);
#endif

/* Added at libpng version 1.4.0 */
extern void png_check_IHDR(png_structp png_ptr,
    png_uint_32 width, png_uint_32 height, int bit_depth,
    int color_type, int interlace_type, int compression_type,
    int filter_type);

/* Free all memory used by the read (old method - NOT DLL EXPORTED) */
extern void png_read_destroy(png_structp png_ptr,
    png_infop info_ptr, png_infop end_info_ptr);

/* Free any memory used in png_ptr struct (old method - NOT DLL EXPORTED) */
extern void png_write_destroy(png_structp png_ptr);

#ifdef USE_FAR_KEYWORD  /* memory model conversion function */
extern void *png_far_to_near(png_structp png_ptr, png_voidp ptr,
    int check);
#endif /* USE_FAR_KEYWORD */

#if defined(PNG_FLOATING_POINT_SUPPORTED) && defined(PNG_ERROR_TEXT_SUPPORTED)
extern void png_fixed_error(png_structp png_ptr,
   png_const_charp name);
#endif

/* ASCII to FP interfaces, currently only implemented if sCAL
 * support is required.
 */
#if defined(PNG_READ_sCAL_SUPPORTED)
/* MAX_DIGITS is actually the maximum number of characters in an sCAL
 * width or height, derived from the precision (number of significant
 * digits - a build time settable option) and assumpitions about the
 * maximum ridiculous exponent.
 */
#define PNG_sCAL_MAX_DIGITS (PNG_sCAL_PRECISION+1/*.*/+1/*E*/+10/*exponent*/)

#ifdef PNG_FLOATING_POINT_SUPPORTED
extern void png_ascii_from_fp(png_structp png_ptr, png_charp ascii,
    png_size_t size, double fp, unsigned int precision);
#endif /* FLOATING_POINT */

#ifdef PNG_FIXED_POINT_SUPPORTED
extern void png_ascii_from_fixed(png_structp png_ptr,
    png_charp ascii, png_size_t size, png_fixed_point fp);
#endif /* FIXED_POINT */
#endif /* READ_sCAL */

#if defined(PNG_sCAL_SUPPORTED) || defined(PNG_pCAL_SUPPORTED)
/* An internal API to validate the format of a floating point number.
 * The result is the index of the next character.  If the number is
 * not valid it will be the index of a character in the supposed number.
 *
 * The format of a number is defined in the PNG extensions specification
 * and this API is strictly conformant to that spec, not anyone elses!
 *
 * The format as a regular expression is:
 *
 * [+-]?[0-9]+.?([Ee][+-]?[0-9]+)?
 *
 * or:
 *
 * [+-]?.[0-9]+(.[0-9]+)?([Ee][+-]?[0-9]+)?
 *
 * The complexity is that either integer or fraction must be present and the
 * fraction is permitted to have no digits only if the integer is present.
 *
 * NOTE: The dangling E problem.
 *   There is a PNG valid floating point number in the following:
 *
 *       PNG floating point numb1.ers are not greedy.
 *
 *   Working this out requires *TWO* character lookahead (because of the
 *   sign), the parser does not do this - it will fail at the 'r' - this
 *   doesn't matter for PNG sCAL chunk values, but it requires more care
 *   if the value were ever to be embedded in something more complex.  Use
 *   ANSI-C strtod if you need the lookahead.
 */
/* State table for the parser. */
#define PNG_FP_INTEGER    0  /* before or in integer */
#define PNG_FP_FRACTION   1  /* before or in fraction */
#define PNG_FP_EXPONENT   2  /* before or in exponent */
#define PNG_FP_STATE      3  /* mask for the above */
#define PNG_FP_SAW_SIGN   4  /* Saw +/- in current state */
#define PNG_FP_SAW_DIGIT  8  /* Saw a digit in current state */
#define PNG_FP_SAW_DOT   16  /* Saw a dot in current state */
#define PNG_FP_SAW_E     32  /* Saw an E (or e) in current state */
#define PNG_FP_SAW_ANY   60  /* Saw any of the above 4 */
#define PNG_FP_WAS_VALID 64  /* Preceding substring is a valid fp number */
#define PNG_FP_INVALID  128  /* Available for callers as a distinct value */

/* Result codes for the parser (boolean - true meants ok, false means
 * not ok yet.)
 */
#define PNG_FP_MAYBE      0  /* The number may be valid in the future */
#define PNG_FP_OK         1  /* The number is valid */

/* The actual parser.  This can be called repeatedly, it updates
 * the index into the string and the state variable (which must
 * be initialzed to 0).  It returns a result code, as above.  There
 * is no point calling the parser any more if it fails to advance to
 * the end of the string - it is stuck on an invalid character (or
 * terminated by '\0').
 *
 * Note that the pointer will consume an E or even an E+ then leave
 * a 'maybe' state even though a preceding integer.fraction is valid.
 * The PNG_FP_WAS_VALID flag indicates that a preceding substring was
 * a valid number.  It's possible to recover from this by calling
 * the parser again (from the start, with state 0) but with a string
 * that omits the last character (i.e. set the size to the index of
 * the problem character.)  This has not been tested within libpng.
 */
extern int png_check_fp_number(png_const_charp string,
    png_size_t size, int *statep, png_size_tp whereami);

/* This is the same but it checks a complete string and returns true
 * only if it just contains a floating point number.
 */
extern int png_check_fp_string(png_const_charp string,
    png_size_t size);
#endif /* pCAL || sCAL */

#if defined(PNG_READ_GAMMA_SUPPORTED) ||\
    defined(PNG_INCH_CONVERSIONS_SUPPORTED) || defined(PNG_READ_pHYs_SUPPORTED)
/* Added at libpng version 1.5.0 */
/* This is a utility to provide a*times/div (rounded) and indicate
 * if there is an overflow.  The result is a boolean - false (0)
 * for overflow, true (1) if no overflow, in which case *res
 * holds the result.
 */
extern int png_muldiv(png_fixed_point_p res, png_fixed_point a,
    png_int_32 times, png_int_32 div);
#endif

#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_INCH_CONVERSIONS_SUPPORTED)
/* Same deal, but issue a warning on overflow and return 0. */
extern png_fixed_point png_muldiv_warn(png_structp png_ptr,
    png_fixed_point a, png_int_32 times, png_int_32 div);
#endif

#ifdef PNG_READ_GAMMA_SUPPORTED
/* Calculate a reciprocal - used for gamma values.  This returns
 * 0 if the argument is 0 in order to maintain an undefined value,
 * there are no warnings.
 */
extern png_fixed_point png_reciprocal(png_fixed_point a);

/* The same but gives a reciprocal of the product of two fixed point
 * values.  Accuracy is suitable for gamma calculations but this is
 * not exact - use png_muldiv for that.
 */
extern png_fixed_point png_reciprocal2(png_fixed_point a,
    png_fixed_point b);
#endif

#ifdef PNG_READ_GAMMA_SUPPORTED
/* Internal fixed point gamma correction.  These APIs are called as
 * required to convert single values - they don't need to be fast,
 * they are not used when processing image pixel values.
 *
 * While the input is an 'unsigned' value it must actually be the
 * correct bit value - 0..255 or 0..65535 as required.
 */
extern png_uint_16 png_gamma_correct(png_structp png_ptr,
    unsigned int value, png_fixed_point gamma);
extern int png_gamma_significant(png_fixed_point gamma);
extern png_uint_16 png_gamma_16bit_correct(unsigned int value,
    png_fixed_point gamma);
extern png_byte png_gamma_8bit_correct(unsigned int value,
    png_fixed_point gamma);
extern void png_build_gamma_table(png_structp png_ptr,
    int bit_depth);
#endif

/* Maintainer: Put new private prototypes here ^ and in libpngpf.3 */


#include "pngdebug.h"

#define PNGPRIV_H
#endif /* PNGPRIV_H */
