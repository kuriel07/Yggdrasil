
/* pngconf.h - machine configurable file for libpng
 *
 * libpng version 1.5.1 - February 3, 2011
 *
 * Copyright (c) 1998-2011 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 */

/* Any machine specific code is near the front of this file, so if you
 * are configuring libpng for a machine, you may want to read the section
 * starting here down to where it starts to typedef png_color, png_text,
 * and png_info.
 */

#ifndef PNGCONF_H
#define PNGCONF_H

/* PNG_NO_LIMITS_H may be used to turn off the use of the standard C
 * definition file for  machine specific limits, this may impact the
 * correctness of the definitons below (see uses of INT_MAX).
 */
#ifndef PNG_NO_LIMITS_H
#  include <limits.h>
#endif

/* For the memory copy APIs (i.e. the standard definitions of these),
 * because this file defines png_memcpy and so on the base APIs must
 * be defined here.
 */
#ifdef BSD
#  include <strings.h>
#else
#  include <string.h>
#endif

/* For png_FILE_p - this provides the standard definition of a
 * FILE
 */
#ifdef PNG_STDIO_SUPPORTED
#  include <stdio.h>
#endif

/* This controls optimization of the reading of 16 and 32 bit values
 * from PNG files.  It can be set on a per-app-file basis - it
 * just changes whether a macro is used to the function is called.
 * The library builder sets the default, if read functions are not
 * built into the library the macro implementation is forced on.
 */
#ifndef PNG_READ_INT_FUNCTIONS_SUPPORTED
#  define PNG_USE_READ_MACROS
#endif
#if !defined(PNG_NO_USE_READ_MACROS) && !defined(PNG_USE_READ_MACROS)
#  if PNG_DEFAULT_READ_MACROS
#    define PNG_USE_READ_MACROS
#  endif
#endif

/* COMPILER SPECIFIC OPTIONS.
 *
 * These options are provided so that a variety of difficult compilers
 * can be used.  Some are fixed at build time (e.g. PNG_API_RULE
 * below) but still have compiler specific implementations, others
 * may be changed on a per-file basis when compiling against libpng.
 */

/* The PNGARG macro protects us against machines that don't have function
 * prototypes (ie K&R style headers).  If your compiler does not handle
 * function prototypes, define this macro and use the included ansi2knr.
 * I've always been able to use _NO_PROTO as the indicator, but you may
 * need to drag the empty declaration out in front of here, or change the
 * ifdef to suit your own needs.
 */
#ifndef PNGARG

#  ifdef OF /* zlib prototype munger */
#    define PNGARG(arglist) OF(arglist)
#  else

#    ifdef _NO_PROTO
#      define PNGARG(arglist) ()
#    else
#      define PNGARG(arglist) arglist
#    endif /* _NO_PROTO */

#  endif /* OF */

#endif /* PNGARG */

/* Function calling conventions.
 * =============================
 * Normally it is not necessary to specify to the compiler how to call
 * a function - it just does it - however on x86 systems derived from
 * Microsoft and Borland C compilers ('IBM PC', 'DOS', 'Windows' systems
 * and some others) there are multiple ways to call a function and the
 * default can be changed on the compiler command line.  For this reason
 * libpng specifies the calling convention of every exported function and
 * every function called via a user supplied function pointer.  This is
 * done in this file by defining the following macros:
 *
 * PNGAPI    Calling convention for exported functions.
 * PNGCBAPI  Calling convention for user provided (callback) functions.
 * PNGCAPI   Calling convention used by the ANSI-C library (required
 *           for longjmp callbacks and sometimes used internally to
 *           specify the calling convention for zlib).
 *
 * These macros should never be overridden.  If it is necessary to
 * change calling convention in a private build this can be done
 * by setting PNG_API_RULE (which defaults to 0) to one of the values
 * below to select the correct 'API' variants.
 *
 * PNG_API_RULE=0 Use PNGCAPI - the 'C' calling convention - throughout.
 *                This is correct in every known environment.
 * PNG_API_RULE=1 Use the operating system convention for PNGAPI and
 *                the 'C' calling convention (from PNGCAPI) for
 *                callbacks (PNGCBAPI).  This is no longer required
 *                in any known environment - if it has to be used
 *                please post an explanation of the problem to the
 *                libpng mailing list.
 *
 * These cases only differ if the operating system does not use the C
 * calling convention, at present this just means the above cases
 * (x86 DOS/Windows sytems) and, even then, this does not apply to
 * Cygwin running on those systems.
 *
 * Note that the value must be defined in pnglibconf.h so that what
 * the application uses to call the library matches the conventions
 * set when building the library.
 */

/* Symbol export
 * =============
 * When building a shared library it is almost always necessary to tell
 * the compiler which symbols to export.  The png.h macro 'PNG_EXPORT'
 * is used to mark the symbols.  On some systems these symbols can be
 * extracted at link time and need no special processing by the compiler,
 * on other systems the symbols are flagged by the compiler and just
 * the declaration requires a special tag applied (unfortunately) in a
 * compiler dependent way.  Some systems can do either.
 *
 * A small number of older systems also require a symbol from a DLL to
 * be flagged to the program that calls it.  This is a problem because
 * we do not know in the header file included by application code that
 * the symbol will come from a shared library, as opposed to a statically
 * linked one.  For this reason the application must tell us by setting
 * the magic flag PNG_USE_DLL to turn on the special processing before
 * it includes png.h.
 *
 * Four additional macros are used to make this happen:
 *
 * PNG_IMPEXP The magic (if any) to cause a symbol to be exported from
 *            the build or imported if PNG_USE_DLL is set - compiler
 *            and system specific.
 *
 * PNG_EXPORT_TYPE(type) A macro that pre or appends PNG_IMPEXP to
 *                       'type', compiler specific.
 *
 * PNG_DLL_EXPORT Set to the magic to use during a libpng build to
 *                make a symbol exported from the DLL.
 *
 * PNG_DLL_IMPORT Set to the magic to force the libpng symbols to come
 *                from a DLL - used to define PNG_IMPEXP when
 *                PNG_USE_DLL is set.
 */

/* The default for PNG_IMPEXP depends on whether the library is
 * being built or used.
 */

/* THe following complexity is concerned with getting the 'attributes' of the
 * declared function in the correct place.  This potentially requires a separate
 * PNG_EXPORT function for every compiler.
 */


#ifndef PNG_NO_PEDANTIC_WARNINGS
#  ifndef PNG_PEDANTIC_WARNINGS_SUPPORTED
#    define PNG_PEDANTIC_WARNINGS_SUPPORTED
#  endif
#endif

/* The following uses const char * instead of char * for error
 * and warning message functions, so some compilers won't complain.
 * If you do not want to use const, define PNG_NO_CONST here.
 *
 * This should not change how the APIs are called, so it can be done
 * on a per-file basis in the application.
 */
#ifndef PNG_CONST
#ifndef PNG_NO_CONST
#define PNG_CONST const
#else
#define PNG_CONST
#endif
#endif

/* Some typedefs to get us started.  These should be safe on most of the
 * common platforms.  The typedefs should be at least as large as the
 * numbers suggest (a png_uint_32 must be at least 32 bits long), but they
 * don't have to be exactly that size.  Some compilers dislike passing
 * unsigned shorts as function parameters, so you may be better off using
 * unsigned int for png_uint_16.
 */

#if defined(INT_MAX) && (INT_MAX > 0x7ffffffeL)
typedef unsigned int png_uint_32;
typedef int png_int_32;
#else
typedef unsigned long png_uint_32;
typedef long png_int_32;
#endif
typedef unsigned short png_uint_16;
typedef short png_int_16;
typedef unsigned char png_byte;

#ifdef PNG_NO_SIZE_T
typedef unsigned int png_size_t;
#else
typedef size_t png_size_t;
#endif
#define png_sizeof(x) (sizeof (x))

/* Typedef for floating-point numbers that are converted
 * to fixed-point with a multiple of 100,000, e.g., gamma
 */
typedef png_int_32 png_fixed_point;

/* Add typedefs for pointers */
typedef void                      * png_voidp;
typedef PNG_CONST void            * png_const_voidp;
typedef png_byte                  * png_bytep;
typedef PNG_CONST png_byte        * png_const_bytep;
typedef png_uint_32               * png_uint_32p;
typedef PNG_CONST png_uint_32     * png_const_uint_32p;
typedef png_int_32                * png_int_32p;
typedef PNG_CONST png_int_32      * png_const_int_32p;
typedef png_uint_16               * png_uint_16p;
typedef PNG_CONST png_uint_16     * png_const_uint_16p;
typedef png_int_16                * png_int_16p;
typedef PNG_CONST png_int_16      * png_const_int_16p;
typedef char                      * png_charp;
typedef PNG_CONST char            * png_const_charp;
typedef png_fixed_point           * png_fixed_point_p;
typedef PNG_CONST png_fixed_point * png_const_fixed_point_p;
typedef png_size_t                * png_size_tp;
typedef PNG_CONST png_size_t      * png_const_size_tp;

#ifdef PNG_STDIO_SUPPORTED
typedef FILE             * png_FILE_p;
#endif

#ifdef PNG_FLOATING_POINT_SUPPORTED
typedef double           * png_doublep;
typedef PNG_CONST double * png_const_doublep;
#endif

/* Pointers to pointers; i.e. arrays */
typedef png_byte        * * png_bytepp;
typedef png_uint_32     * * png_uint_32pp;
typedef png_int_32      * * png_int_32pp;
typedef png_uint_16     * * png_uint_16pp;
typedef png_int_16      * * png_int_16pp;
typedef PNG_CONST char  * * png_const_charpp;
typedef char            * * png_charpp;
typedef png_fixed_point * * png_fixed_point_pp;
#ifdef PNG_FLOATING_POINT_SUPPORTED
typedef double          * * png_doublepp;
#endif

/* Pointers to pointers to pointers; i.e., pointer to array */
typedef char            * * * png_charppp;

/* png_alloc_size_t is guaranteed to be no smaller than png_size_t,
 * and no smaller than png_uint_32.  Casts from png_size_t or png_uint_32
 * to png_alloc_size_t are not necessary; in fact, it is recommended
 * not to use them at all so that the compiler can complain when something
 * turns out to be problematic.
 * Casts in the other direction (from png_alloc_size_t to png_size_t or
 * png_uint_32) should be explicitly applied; however, we do not expect
 * to encounter practical situations that require such conversions.
 */
typedef png_size_t    png_alloc_size_t;

#endif /* PNGCONF_H */
