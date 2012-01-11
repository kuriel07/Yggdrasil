#include "png.h"
#include "pngpriv.h"
#include "yggdrasil_png.h"
      
#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
png_byte png_IHDR[5] = { 73,  72,  68,  82, '\0'};
png_byte png_IDAT[5] = { 73,  68,  65,  84, '\0'};
png_byte png_IEND[5] = { 73,  69,  78,  68, '\0'};
png_byte png_PLTE[5] = { 80,  76,  84,  69, '\0'};
#ifdef PNG_READ_bKGD_SUPPORTED
png_byte png_bKGD[5] = { 98,  75,  71,  68, '\0'};
#endif
#ifdef PNG_READ_cHRM_SUPPORTED
png_byte png_cHRM[5] = { 99,  72,  82,  77, '\0'};
#endif
#ifdef PNG_READ_gAMA_SUPPORTED
png_byte png_gAMA[5] = {103,  65,  77,  65, '\0'};
#endif
#ifdef PNG_READ_hIST_SUPPORTED
png_byte png_hIST[5] = {104,  73,  83,  84, '\0'};
#endif
#ifdef PNG_READ_iCCP_SUPPORTED
png_byte png_iCCP[5] = {105,  67,  67,  80, '\0'};
#endif
#ifdef PNG_READ_iTXt_SUPPORTED
png_byte png_iTXt[5] = {105,  84,  88, 116, '\0'};
#endif
#ifdef PNG_READ_oFFs_SUPPORTED
png_byte png_oFFs[5] = {111,  70,  70, 115, '\0'};
#endif
#ifdef PNG_READ_pCAL_SUPPORTED
png_byte png_pCAL[5] = {112,  67,  65,  76, '\0'};
#endif
#ifdef PNG_READ_pHYs_SUPPORTED
png_byte png_pHYs[5] = {112,  72,  89, 115, '\0'};
#endif
#ifdef PNG_READ_sBIT_SUPPORTED
png_byte png_sBIT[5] = {115,  66,  73,  84, '\0'};
#endif
#ifdef PNG_READ_sCAL_SUPPORTED
png_byte png_sCAL[5] = {115,  67,  65,  76, '\0'};
#endif
#ifdef PNG_READ_sPLT_SUPPORTED
png_byte png_sPLT[5] = {115,  80,  76,  84, '\0'};
#endif
#ifdef PNG_READ_sRGB_SUPPORTED
png_byte png_sRGB[5] = {115,  82,  71,  66, '\0'};
#endif
#ifdef PNG_READ_tEXt_SUPPORTED
png_byte png_tEXt[5] = {116,  69,  88, 116, '\0'};
#endif
#ifdef PNG_READ_tIME_SUPPORTED
png_byte png_tIME[5] = {116,  73,  77,  69, '\0'};
#endif
#ifdef PNG_READ_tRNS_SUPPORTED
png_byte png_tRNS[5] = {116,  82,  78,  83, '\0'};
#endif
#ifdef PNG_READ_zTXt_SUPPORTED
png_byte png_zTXt[5] = {122,  84,  88, 116, '\0'};
#endif
/* Fixed point gamma.
 *
 * To calculate gamma this code implements fast log() and exp() calls using only
 * fixed point arithmetic.  This code has sufficient precision for either 8 or
 * 16 bit sample values.
 *
 * The tables used here were calculated using simple 'bc' programs, but C double
 * precision floating point arithmetic would work fine.  The programs are given
 * at the head of each table.
 *
 * 8 bit log table
 *   This is a table of -log(value/255)/log(2) for 'value' in the range 128 to
 *   255, so it's the base 2 logarithm of a normalized 8 bit floating point
 *   mantissa.  The numbers are 32 bit fractions.
 */
static png_uint_32
png_8bit_l2_2[128] =
{
   4270715492U, 4222494797U, 4174646467U, 4127164793U, 4080044201U, 4033279239U,
   3986864580U, 3940795015U, 3895065449U, 3849670902U, 3804606499U, 3759867474U,
   3715449162U, 3671346997U, 3627556511U, 3584073329U, 3540893168U, 3498011834U,
   3455425220U, 3413129301U, 3371120137U, 3329393864U, 3287946700U, 3246774933U,
   3205874930U, 3165243125U, 3124876025U, 3084770202U, 3044922296U, 3005329011U,
   2965987113U, 2926893432U, 2888044853U, 2849438323U, 2811070844U, 2772939474U,
   2735041326U, 2697373562U, 2659933400U, 2622718104U, 2585724991U, 2548951424U,
   2512394810U, 2476052606U, 2439922311U, 2404001468U, 2368287663U, 2332778523U,
   2297471715U, 2262364947U, 2227455964U, 2192742551U, 2158222529U, 2123893754U,
   2089754119U, 2055801552U, 2022034013U, 1988449497U, 1955046031U, 1921821672U,
   1888774511U, 1855902668U, 1823204291U, 1790677560U, 1758320682U, 1726131893U,
   1694109454U, 1662251657U, 1630556815U, 1599023271U, 1567649391U, 1536433567U,
   1505374214U, 1474469770U, 1443718700U, 1413119487U, 1382670639U, 1352370686U,
   1322218179U, 1292211689U, 1262349810U, 1232631153U, 1203054352U, 1173618059U,
   1144320946U, 1115161701U, 1086139034U, 1057251672U, 1028498358U, 999877854U,
   971388940U, 943030410U, 914801076U, 886699767U, 858725327U, 830876614U,
   803152505U, 775551890U, 748073672U, 720716771U, 693480120U, 666362667U,
   639363374U, 612481215U, 585715177U, 559064263U, 532527486U, 506103872U,
   479792461U, 453592303U, 427502463U, 401522014U, 375650043U, 349885648U,
   324227938U, 298676034U, 273229066U, 247886176U, 222646516U, 197509248U,
   172473545U, 147538590U, 122703574U, 97967701U, 73330182U, 48790236U,
   24347096U, 0U
#if 0
   /* The following are the values for 16 bit tables - these work fine for the 8
    * bit conversions but produce very slightly larger errors in the 16 bit log
    * (about 1.2 as opposed to 0.7 absolute error in the final value).  To use
    * these all the shifts below must be adjusted appropriately.
    */
   65166, 64430, 63700, 62976, 62257, 61543, 60835, 60132, 59434, 58741, 58054,
   57371, 56693, 56020, 55352, 54689, 54030, 53375, 52726, 52080, 51439, 50803,
   50170, 49542, 48918, 48298, 47682, 47070, 46462, 45858, 45257, 44661, 44068,
   43479, 42894, 42312, 41733, 41159, 40587, 40020, 39455, 38894, 38336, 37782,
   37230, 36682, 36137, 35595, 35057, 34521, 33988, 33459, 32932, 32408, 31887,
   31369, 30854, 30341, 29832, 29325, 28820, 28319, 27820, 27324, 26830, 26339,
   25850, 25364, 24880, 24399, 23920, 23444, 22970, 22499, 22029, 21562, 21098,
   20636, 20175, 19718, 19262, 18808, 18357, 17908, 17461, 17016, 16573, 16132,
   15694, 15257, 14822, 14390, 13959, 13530, 13103, 12678, 12255, 11834, 11415,
   10997, 10582, 10168, 9756, 9346, 8937, 8531, 8126, 7723, 7321, 6921, 6523,
   6127, 5732, 5339, 4947, 4557, 4169, 3782, 3397, 3014, 2632, 2251, 1872, 1495,
   1119, 744, 372
#endif
};
/* The above gives exact (to 16 binary places) log2 values for 8 bit images,
 * for 16 bit images we use the most significant 8 bits of the 16 bit value to
 * get an approximation then multiply the approximation by a correction factor
 * determined by the remaining up to 8 bits.  This requires an additional step
 * in the 16 bit case.
 *
 * We want log2(value/65535), we have log2(v'/255), where:
 *
 *    value = v' * 256 + v''
 *          = v' * f
 *
 * So f is value/v', which is equal to (256+v''/v') since v' is in the range 128
 * to 255 and v'' is in the range 0 to 255 f will be in the range 256 to less
 * than 258.  The final factor also needs to correct for the fact that our 8 bit
 * value is scaled by 255, whereas the 16 bit values must be scaled by 65535.
 *
 * This gives a final formula using a calculated value 'x' which is value/v' and
 * scaling by 65536 to match the above table:
 *
 *   log2(x/257) * 65536
 *
 * Since these numbers are so close to '1' we can use simple linear
 * interpolation between the two end values 256/257 (result -368.61) and 258/257
 * (result 367.179).  The values used below are scaled by a further 64 to give
 * 16 bit precision in the interpolation:
 *
 * Start (256): -23591
 * Zero  (257):      0
 * End   (258):  23499
 */
static png_int_32
png_log16bit_2(png_uint_32 x)
{
   unsigned int lg2 = 0;

   /* As above, but now the input has 16 bits. */
   if ((x &= 0xffff) == 0)
      return 0xffffffff;

   if ((x & 0xff00) == 0)
      lg2  = 8, x <<= 8;

   if ((x & 0xf000) == 0)
      lg2 += 4, x <<= 4;

   if ((x & 0xc000) == 0)
      lg2 += 2, x <<= 2;

   if ((x & 0x8000) == 0)
      lg2 += 1, x <<= 1;

   /* Calculate the base logarithm from the top 8 bits as a 28 bit fractional
    * value.
    */
   lg2 <<= 28;
   lg2 += (png_8bit_l2_2[(x>>8)-128]+8) >> 4;

   /* Now we need to interpolate the factor, this requires a division by the top
    * 8 bits.  Do this with maximum precision.
    */
   x = ((x << 16) + (x >> 9)) / (x >> 8);

   /* Since we divided by the top 8 bits of 'x' there will be a '1' at 1<<24,
    * the value at 1<<16 (ignoring this) will be 0 or 1; this gives us exactly
    * 16 bits to interpolate to get the low bits of the result.  Round the
    * answer.  Note that the end point values are scaled by 64 to retain overall
    * precision and that 'lg2' is current scaled by an extra 12 bits, so adjust
    * the overall scaling by 6-12.  Round at every step.
    */
   x -= 1U << 24;

   if (x <= 65536U) /* <= '257' */
      lg2 += ((23591U * (65536U-x)) + (1U << (16+6-12-1))) >> (16+6-12);

   else
      lg2 -= ((23499U * (x-65536U)) + (1U << (16+6-12-1))) >> (16+6-12);

   /* Safe, because the result can't have more than 20 bits: */
   return (png_int_32)((lg2 + 2048) >> 12);
}

/* A local convenience routine. */
static png_fixed_point
png_product2_2(png_fixed_point a, png_fixed_point b)
{
	png_fixed_point res;
   /* The required result is 1/a * 1/b; the following preserves accuracy. */
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
   double r = a * 1E-5;
   r *= b;
   r = floor(r+.5);

   if (r <= 2147483647. && r >= -2147483648.)
      return (png_fixed_point)r;
#else

   if (png_muldiv_2(&res, a, b, 100000))
      return res;
#endif

   return 0; /* overflow */
}
/* Internal function to build a single 16 bit table - the table consists of
 * 'num' 256 entry subtables, where 'num' is determined by 'shift' - the amount
 * to shift the input values right (or 16-number_of_signifiant_bits).
 *
 * The caller is responsible for ensuring that the table gets cleaned up on
 * png_error (i.e. if one of the mallocs below fails) - i.e. the *table argument
 * should be somewhere that will be cleaned.
 */
static void
png_build_16bit_table_2(png_struct * png_ptr, png_uint_16 *** ptable,
   unsigned int shift, png_fixed_point gamma_val)
{
   /* Various values derived from 'shift': */
   unsigned int num = 1U << (8U - shift);
   unsigned int max_int = (1U << (16U - shift))-1U;
   unsigned int max_by_2 = 1U << (15U-shift);
   unsigned int i;
   unsigned int j;
   png_uint_32 ig;
   double d;
   png_uint_16pp table = *ptable =
       (png_uint_16pp)png_calloc_2(png_ptr, num * png_sizeof(png_uint_16p));

   for (i = 0; i < num; i++)
   {
      png_uint_16p sub_table = table[i] =
          (png_uint_16p)png_malloc_2(png_ptr, 256 * png_sizeof(png_uint_16));

      /* The 'threshold' test is repeated here because it can arise for one of
       * the 16 bit tables even if the others don't hit it.
       */
      if (png_gamma_significant_2(gamma_val))
      {
         /* The old code would overflow at the end and this would cause the
          * 'pow' function to return a result >1, resulting in an
          * arithmetic error.  This code follows the spec exactly; ig is
          * the recovered input sample, it always has 8-16 bits.
          *
          * We want input * 65535/max, rounded, the arithmetic fits in 32
          * bits (unsigned) so long as max <= 32767.
          */
         for (j = 0; j < 256; j++)
         {
               ig = (j << (8-shift)) + i;
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
               /* Inline the 'max' scaling operation: */
               d = floor(65535*pow(ig/(double)max_int, gamma_val*.00001)+.5);
               sub_table[j] = (png_uint_16)d;
#else
               if (shift)
                  ig = (ig * 65535U + max_by_2)/max_int;

               sub_table[j] = png_gamma_16bit_correct_2(ig, gamma_val);
#endif
         }
      }
      else
      {
         /* We must still build a table, but do it the fast way. */
         
         for (j = 0; j < 256; j++)
         {
            ig = (j << (8-shift)) + i;

            if (shift)
               ig = (ig * 65535U + max_by_2)/max_int;

            sub_table[j] = (png_uint_16)ig;
         }
      }
   }
}

/* Transform the image from the file_gamma to the screen_gamma.  We
 * only do transformations on images where the file_gamma and screen_gamma
 * are not close reciprocals, otherwise it slows things down slightly, and
 * also needlessly introduces small errors.
 *
 * We will turn off gamma transformation later if no semitransparent entries
 * are present in the tRNS array for palette images.  We can't do it here
 * because we don't necessarily have the tRNS chunk yet.
 */
static int /* PRIVATE */
png_gamma_threshold_2(png_fixed_point scrn_gamma, png_fixed_point file_gamma)
{
   /* PNG_GAMMA_THRESHOLD is the threshold for performing gamma
    * correction as a difference of the overall transform from 1.0
    *
    * We want to compare the threshold with s*f - 1, if we get
    * overflow here it is because of wacky gamma values so we
    * turn on processing anyway.
    */
   png_fixed_point gtest;
   return !png_muldiv_2(&gtest, scrn_gamma, file_gamma, PNG_FP_1) ||
       png_gamma_significant_2(gtest);
}

/* Initialize everything needed for the read.  This includes modifying
 * the palette.
 */
void /* PRIVATE */
png_init_read_transformations_2(png_struct * png_ptr)
{
  int i, k;
  int istop;
  png_color back, back_1;
  png_color * palette;
  png_fixed_point g, gs;
  int num_palette = png_ptr->num_palette;
  int sr;
  int sg;
  int sb;
   //png_debug(1, "in png_init_read_transformations_2");
#if defined(PNG_READ_BACKGROUND_SUPPORTED) || \
    defined(PNG_READ_SHIFT_SUPPORTED) || \
    defined(PNG_READ_GAMMA_SUPPORTED)
   int color_type = png_ptr->color_type;
#endif
  {
#if defined(PNG_READ_EXPAND_SUPPORTED) && defined(PNG_READ_BACKGROUND_SUPPORTED)

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   /* Detect gray background and attempt to enable optimization
    * for gray --> RGB case
    *
    * Note:  if PNG_BACKGROUND_EXPAND is set and color_type is either RGB or
    * RGB_ALPHA (in which case need_expand is superfluous anyway), the
    * background color might actually be gray yet not be flagged as such.
    * This is not a problem for the current code, which uses
    * PNG_BACKGROUND_IS_GRAY only to decide when to do the
    * png_do_gray_to_rgb() transformation.
    */
   if ((png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
       !(color_type & PNG_COLOR_MASK_COLOR))
   {
      png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
   }

   else if ((png_ptr->transformations & PNG_BACKGROUND) &&
       !(png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
       (png_ptr->transformations & PNG_GRAY_TO_RGB) &&
       png_ptr->background.red == png_ptr->background.green &&
       png_ptr->background.red == png_ptr->background.blue)
   {
      png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
      png_ptr->background.gray = png_ptr->background.red;
   }
#endif

   if ((png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
       (png_ptr->transformations & PNG_EXPAND))
   {
      if (!(color_type & PNG_COLOR_MASK_COLOR))  /* i.e., GRAY or GRAY_ALPHA */
      {
         /* Expand background and tRNS chunks */
         switch (png_ptr->bit_depth)
         {
            case 1:
               png_ptr->background.gray *= (png_uint_16)0xff;
               png_ptr->background.red = png_ptr->background.green
                   =  png_ptr->background.blue = png_ptr->background.gray;
               if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
               {
                 png_ptr->trans_color.gray *= (png_uint_16)0xff;
                 png_ptr->trans_color.red = png_ptr->trans_color.green
                     = png_ptr->trans_color.blue = png_ptr->trans_color.gray;
               }
               break;

            case 2:
               png_ptr->background.gray *= (png_uint_16)0x55;
               png_ptr->background.red = png_ptr->background.green
                   = png_ptr->background.blue = png_ptr->background.gray;
               if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
               {
                  png_ptr->trans_color.gray *= (png_uint_16)0x55;
                  png_ptr->trans_color.red = png_ptr->trans_color.green
                      = png_ptr->trans_color.blue = png_ptr->trans_color.gray;
               }
               break;

            case 4:
               png_ptr->background.gray *= (png_uint_16)0x11;
               png_ptr->background.red = png_ptr->background.green
                   = png_ptr->background.blue = png_ptr->background.gray;
               if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
               {
                  png_ptr->trans_color.gray *= (png_uint_16)0x11;
                  png_ptr->trans_color.red = png_ptr->trans_color.green
                      = png_ptr->trans_color.blue = png_ptr->trans_color.gray;
               }
               break;

            default:

            case 8:

            case 16:
               png_ptr->background.red = png_ptr->background.green
                   = png_ptr->background.blue = png_ptr->background.gray;
               break;
         }
      }
      else if (color_type == PNG_COLOR_TYPE_PALETTE)
      {
         png_ptr->background.red   =
             png_ptr->palette[png_ptr->background.index].red;
         png_ptr->background.green =
             png_ptr->palette[png_ptr->background.index].green;
         png_ptr->background.blue  =
             png_ptr->palette[png_ptr->background.index].blue;

#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
        if (png_ptr->transformations & PNG_INVERT_ALPHA)
        {
#ifdef PNG_READ_EXPAND_SUPPORTED
           if (!(png_ptr->transformations & PNG_EXPAND_tRNS))
#endif
           {
              /* Invert the alpha channel (in tRNS) unless the pixels are
               * going to be expanded, in which case leave it for later
               */
              istop = (int)png_ptr->num_trans;
              for (i=0; i<istop; i++)
                 png_ptr->trans_alpha[i] = (png_byte)(255 -
                    png_ptr->trans_alpha[i]);
           }
        }
#endif

      }
   }
#endif

#if defined(PNG_READ_BACKGROUND_SUPPORTED) && defined(PNG_READ_GAMMA_SUPPORTED)
   png_ptr->background_1 = png_ptr->background;
#endif
#ifdef PNG_READ_GAMMA_SUPPORTED

   if ((color_type == PNG_COLOR_TYPE_PALETTE && png_ptr->num_trans != 0)
       && png_gamma_threshold_2(png_ptr->screen_gamma, png_ptr->gamma))
   {
      k=0;
      for (i=0; i<png_ptr->num_trans; i++)
      {
        if (png_ptr->trans_alpha[i] != 0 && png_ptr->trans_alpha[i] != 0xff)
           k=1; /* Partial transparency is present */
      }
      if (k == 0)
         png_ptr->transformations &= ~PNG_GAMMA;
   }

   if ((png_ptr->transformations & (PNG_GAMMA | PNG_RGB_TO_GRAY)) &&
       png_ptr->gamma != 0)
   {
      png_build_gamma_table_2(png_ptr, png_ptr->bit_depth);

#ifdef PNG_READ_BACKGROUND_SUPPORTED
      if (png_ptr->transformations & PNG_BACKGROUND)
      {
         if (color_type == PNG_COLOR_TYPE_PALETTE)
         {
            /* Could skip if no transparency */
            palette = png_ptr->palette;
            num_palette = png_ptr->num_palette;
            if (png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_FILE)
            {

               back.red = png_ptr->gamma_table[png_ptr->background.red];
               back.green = png_ptr->gamma_table[png_ptr->background.green];
               back.blue = png_ptr->gamma_table[png_ptr->background.blue];

               back_1.red = png_ptr->gamma_to_1[png_ptr->background.red];
               back_1.green = png_ptr->gamma_to_1[png_ptr->background.green];
               back_1.blue = png_ptr->gamma_to_1[png_ptr->background.blue];
            }
            else
            {
               switch (png_ptr->background_gamma_type)
               {
                  case PNG_BACKGROUND_GAMMA_SCREEN:
                     g = (png_ptr->screen_gamma);
                     gs = PNG_FP_1;
                     break;

                  case PNG_BACKGROUND_GAMMA_FILE:
                     g = png_reciprocal_2(png_ptr->gamma);
                     gs = png_reciprocal2_2(png_ptr->gamma,
                        png_ptr->screen_gamma);
                     break;

                  case PNG_BACKGROUND_GAMMA_UNIQUE:
                     g = png_reciprocal_2(png_ptr->background_gamma);
                     gs = png_reciprocal2_2(png_ptr->background_gamma,
                        png_ptr->screen_gamma);
                     break;
                  default:
                     g = PNG_FP_1;    /* back_1 */
                     gs = PNG_FP_1;   /* back */
                     break;
               }

               if (png_gamma_significant_2(gs))
               {
                  back.red   = (png_byte)png_ptr->background.red;
                  back.green = (png_byte)png_ptr->background.green;
                  back.blue  = (png_byte)png_ptr->background.blue;
               }

               else
               {
                  back.red = png_gamma_8bit_correct_2(png_ptr->background.red,
                      gs);
                  back.green = png_gamma_8bit_correct_2(png_ptr->background.green,
                      gs);
                  back.blue = png_gamma_8bit_correct_2(png_ptr->background.blue,
                      gs);
               }
               back_1.red = png_gamma_8bit_correct_2(png_ptr->background.red, g);
               back_1.green = png_gamma_8bit_correct_2(png_ptr->background.green,
                   g);
               back_1.blue = png_gamma_8bit_correct_2(png_ptr->background.blue,
                   g);
            }
            for (i = 0; i < num_palette; i++)
            {
               if (i < (int)png_ptr->num_trans &&
                   png_ptr->trans_alpha[i] != 0xff)
               {
                  if (png_ptr->trans_alpha[i] == 0)
                  {
                     palette[i] = back;
                  }
                  else /* if (png_ptr->trans_alpha[i] != 0xff) */
                  {
                     png_byte v, w;

                     v = png_ptr->gamma_to_1[palette[i].red];
                     png_composite(w, v, png_ptr->trans_alpha[i], back_1.red);
                     palette[i].red = png_ptr->gamma_from_1[w];

                     v = png_ptr->gamma_to_1[palette[i].green];
                     png_composite(w, v, png_ptr->trans_alpha[i], back_1.green);
                     palette[i].green = png_ptr->gamma_from_1[w];

                     v = png_ptr->gamma_to_1[palette[i].blue];
                     png_composite(w, v, png_ptr->trans_alpha[i], back_1.blue);
                     palette[i].blue = png_ptr->gamma_from_1[w];
                  }
               }
               else
               {
                  palette[i].red = png_ptr->gamma_table[palette[i].red];
                  palette[i].green = png_ptr->gamma_table[palette[i].green];
                  palette[i].blue = png_ptr->gamma_table[palette[i].blue];
               }
            }
            /* Prevent the transformations being done again, and make sure
             * that the now spurious alpha channel is stripped - the code
             * has just reduced background composition and gamma correction
             * to a simple alpha channel strip.
             */
            png_ptr->transformations &= ~PNG_BACKGROUND;
            png_ptr->transformations &= ~PNG_GAMMA;
            png_ptr->flags |= PNG_FLAG_STRIP_ALPHA;
         }

         /* if (png_ptr->background_gamma_type!=PNG_BACKGROUND_GAMMA_UNKNOWN) */
         else
         /* color_type != PNG_COLOR_TYPE_PALETTE */
         {
            g = PNG_FP_1;
            gs = PNG_FP_1;

            switch (png_ptr->background_gamma_type)
            {
               case PNG_BACKGROUND_GAMMA_SCREEN:
                  g = png_ptr->screen_gamma;
                  /* gs = PNG_FP_1; */
                  break;

               case PNG_BACKGROUND_GAMMA_FILE:
                  g = png_reciprocal_2(png_ptr->gamma);
                  gs = png_reciprocal2_2(png_ptr->gamma, png_ptr->screen_gamma);
                  break;

               case PNG_BACKGROUND_GAMMA_UNIQUE:
                  g = png_reciprocal_2(png_ptr->background_gamma);
                  gs = png_reciprocal2_2(png_ptr->background_gamma,
                      png_ptr->screen_gamma);
                  break;

               default:
               	g=g;
                  //png_error(png_ptr, "invalid background gamma type");
            }

            png_ptr->background_1.gray = png_gamma_correct_2(png_ptr,
                png_ptr->background.gray, g);

            png_ptr->background.gray = png_gamma_correct_2(png_ptr,
                png_ptr->background.gray, gs);

            if ((png_ptr->background.red != png_ptr->background.green) ||
                (png_ptr->background.red != png_ptr->background.blue) ||
                (png_ptr->background.red != png_ptr->background.gray))
            {
               /* RGB or RGBA with color background */
               png_ptr->background_1.red = png_gamma_correct_2(png_ptr,
                   png_ptr->background.red, g);

               png_ptr->background_1.green = png_gamma_correct_2(png_ptr,
                   png_ptr->background.green, g);

               png_ptr->background_1.blue = png_gamma_correct_2(png_ptr,
                   png_ptr->background.blue, g);

               png_ptr->background.red = png_gamma_correct_2(png_ptr,
                   png_ptr->background.red, gs);

               png_ptr->background.green = png_gamma_correct_2(png_ptr,
                   png_ptr->background.green, gs);

               png_ptr->background.blue = png_gamma_correct_2(png_ptr,
                   png_ptr->background.blue, gs);
            }

            else
            {
               /* GRAY, GRAY ALPHA, RGB, or RGBA with gray background */
               png_ptr->background_1.red = png_ptr->background_1.green
                   = png_ptr->background_1.blue = png_ptr->background_1.gray;

               png_ptr->background.red = png_ptr->background.green
                   = png_ptr->background.blue = png_ptr->background.gray;
            }
         }
      }
      else
      /* Transformation does not include PNG_BACKGROUND */
#endif /* PNG_READ_BACKGROUND_SUPPORTED */
      if (color_type == PNG_COLOR_TYPE_PALETTE)
      {
         png_colorp palette = png_ptr->palette;
         int num_palette = png_ptr->num_palette;
         int i;

         for (i = 0; i < num_palette; i++)
         {
            palette[i].red = png_ptr->gamma_table[palette[i].red];
            palette[i].green = png_ptr->gamma_table[palette[i].green];
            palette[i].blue = png_ptr->gamma_table[palette[i].blue];
         }

         /* Done the gamma correction. */
         png_ptr->transformations &= ~PNG_GAMMA;
      }
   }
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   else
#endif
#endif /* PNG_READ_GAMMA_SUPPORTED */
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   /* No GAMMA transformation */
   if ((png_ptr->transformations & PNG_BACKGROUND) &&
       (color_type == PNG_COLOR_TYPE_PALETTE))
   {
      istop = (int)png_ptr->num_trans;
      palette = png_ptr->palette;

      back.red   = (png_byte)png_ptr->background.red;
      back.green = (png_byte)png_ptr->background.green;
      back.blue  = (png_byte)png_ptr->background.blue;

      for (i = 0; i < istop; i++)
      {
         if (png_ptr->trans_alpha[i] == 0)
         {
            palette[i] = back;
         }

         else if (png_ptr->trans_alpha[i] != 0xff)
         {
            /* The png_composite() macro is defined in png.h */
            png_composite(palette[i].red, palette[i].red,
                png_ptr->trans_alpha[i], back.red);

            png_composite(palette[i].green, palette[i].green,
                png_ptr->trans_alpha[i], back.green);

            png_composite(palette[i].blue, palette[i].blue,
                png_ptr->trans_alpha[i], back.blue);
         }
      }

      /* Handled alpha, still need to strip the channel. */
      png_ptr->transformations &= ~PNG_BACKGROUND;
      png_ptr->flags |= PNG_FLAG_STRIP_ALPHA;
   }
#endif /* PNG_READ_BACKGROUND_SUPPORTED */

#ifdef PNG_READ_SHIFT_SUPPORTED
   if ((png_ptr->transformations & PNG_SHIFT) &&
       (color_type == PNG_COLOR_TYPE_PALETTE))
   {
      istop = png_ptr->num_palette;
      sr = 8 - png_ptr->sig_bit.red;
      sg = 8 - png_ptr->sig_bit.green;
      sb = 8 - png_ptr->sig_bit.blue;

      if (sr < 0 || sr > 8)
         sr = 0;

      if (sg < 0 || sg > 8)
         sg = 0;

      if (sb < 0 || sb > 8)
         sb = 0;

      for (i = 0; i < istop; i++)
      {
         png_ptr->palette[i].red >>= sr;
         png_ptr->palette[i].green >>= sg;
         png_ptr->palette[i].blue >>= sb;
      }
   }
#endif  /* PNG_READ_SHIFT_SUPPORTED */
 }
#if !defined(PNG_READ_GAMMA_SUPPORTED) && !defined(PNG_READ_SHIFT_SUPPORTED) \
 && !defined(PNG_READ_BACKGROUND_SUPPORTED)
   if (png_ptr)
      return;
#endif
}

#ifdef PNG_READ_INTERLACING_SUPPORTED
/* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */
/* Start of interlace block */
int png_pass_start[7] = {0, 4, 0, 2, 0, 1, 0};
/* Offset to next interlace block */
int png_pass_inc[7] = {8, 8, 4, 4, 2, 2, 1};
/* Start of interlace block in the y direction */
int png_pass_ystart[7] = {0, 0, 4, 0, 2, 0, 1};
/* Offset to next interlace block in the y direction */
int png_pass_yinc[7] = {8, 8, 8, 4, 4, 2, 2};
#endif
#include "2440lib.h"
void /* PRIVATE */
png_read_start_row_2(png_struct * png_ptr)
{
   int max_pixel_depth;
   int user_pixel_depth;
   png_size_t row_bytes;
   png_warning_2(png_ptr, "in png_read_start_row");
   png_ptr->zstream.avail_in = 0;
   png_init_read_transformations_2(png_ptr);
#ifdef PNG_READ_INTERLACING_SUPPORTED
   png_warning_2(png_ptr, "PNG_READ_INTERLACING_SUPPORTED");
   if (png_ptr->interlaced)
   {
      if (!(png_ptr->transformations & PNG_INTERLACE)) {
         png_ptr->num_rows = (png_ptr->height + png_pass_yinc[0] - 1 -
             png_pass_ystart[0]) / png_pass_yinc[0];
      }
      else {
         png_ptr->num_rows = png_ptr->height;
      }
      png_ptr->iwidth = (png_ptr->width +
          png_pass_inc[png_ptr->pass] - 1 -
          png_pass_start[png_ptr->pass]) /
          png_pass_inc[png_ptr->pass];
   }

   else
#endif /* PNG_READ_INTERLACING_SUPPORTED */
   {
      png_ptr->num_rows = png_ptr->height;
      png_ptr->iwidth = png_ptr->width;
   }

   max_pixel_depth = png_ptr->pixel_depth;

#ifdef PNG_READ_PACK_SUPPORTED
   png_warning_2(png_ptr, "PNG_READ_PACK_SUPPORTED");
   if ((png_ptr->transformations & PNG_PACK) && png_ptr->bit_depth < 8)
      max_pixel_depth = 8;
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
   png_warning_2(png_ptr, "PNG_READ_EXPAND_SUPPORTED");
   if (png_ptr->transformations & PNG_EXPAND)
   {
      if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         if (png_ptr->num_trans)
            max_pixel_depth = 32;

         else
            max_pixel_depth = 24;
      }

      else if (png_ptr->color_type == PNG_COLOR_TYPE_GRAY)
      {
         if (max_pixel_depth < 8)
            max_pixel_depth = 8;

         if (png_ptr->num_trans)
            max_pixel_depth *= 2;
      }

      else if (png_ptr->color_type == PNG_COLOR_TYPE_RGB)
      {
         if (png_ptr->num_trans)
         {
            max_pixel_depth *= 4;
            max_pixel_depth /= 3;
         }
      }
   }
#endif

#ifdef PNG_READ_FILLER_SUPPORTED
   png_warning_2(png_ptr, "PNG_READ_FILLER_SUPPORTED");
   if (png_ptr->transformations & (PNG_FILLER))
   {
      if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
         max_pixel_depth = 32;

      else if (png_ptr->color_type == PNG_COLOR_TYPE_GRAY)
      {
         if (max_pixel_depth <= 8)
            max_pixel_depth = 16;

         else
            max_pixel_depth = 32;
      }

      else if (png_ptr->color_type == PNG_COLOR_TYPE_RGB)
      {
         if (max_pixel_depth <= 32)
            max_pixel_depth = 32;

         else
            max_pixel_depth = 64;
      }
   }
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   png_warning_2(png_ptr, "PNG_READ_GRAY_TO_RGB_SUPPORTED");
   if (png_ptr->transformations & PNG_GRAY_TO_RGB)
   {
      if (
#ifdef PNG_READ_EXPAND_SUPPORTED
          (png_ptr->num_trans && (png_ptr->transformations & PNG_EXPAND)) ||
#endif
#ifdef PNG_READ_FILLER_SUPPORTED
          (png_ptr->transformations & (PNG_FILLER)) ||
#endif
          png_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (max_pixel_depth <= 16)
            max_pixel_depth = 32;

         else
            max_pixel_depth = 64;
      }

      else
      {
         if (max_pixel_depth <= 8)
         {
            if (png_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
               max_pixel_depth = 32;

            else
               max_pixel_depth = 24;
         }

         else if (png_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            max_pixel_depth = 64;

         else
            max_pixel_depth = 48;
      }
   }
#endif
   png_warning_2(png_ptr, "PNG_READ_USER_TRANSFORM_SUPPORTED");
#if defined(PNG_READ_USER_TRANSFORM_SUPPORTED) && defined(PNG_USER_TRANSFORM_PTR_SUPPORTED)
   if (png_ptr->transformations & PNG_USER_TRANSFORM)
   {
      user_pixel_depth = png_ptr->user_transform_depth *
         png_ptr->user_transform_channels;

      if (user_pixel_depth > max_pixel_depth)
         max_pixel_depth=user_pixel_depth;
   }
#endif
   png_warning_2(png_ptr, "align width");
   /* Align the width on the next larger 8 pixels.  Mainly used
    * for interlacing
    */
   row_bytes = ((png_ptr->width + 7) & ~((png_uint_32)7));
   png_warning_2(png_ptr, "calculate maks byte");
   /* Calculate the maximum bytes needed, adding a byte and a pixel
    * for safety's sake
    */
   row_bytes = PNG_ROWBYTES(max_pixel_depth, row_bytes) +
       1 + ((max_pixel_depth + 7) >> 3);
   png_warning_2(png_ptr, "PNG_READ_USER_TRANSFORM_SUPPORTED");
#ifdef PNG_MAX_MALLOC_64K
   if (row_bytes > (png_uint_32)65536L)
      png_warning_2(png_ptr, "This image requires a row greater than 64KB");
#endif

   if (row_bytes + 48 > png_ptr->old_big_row_buf_size)
   {
     png_free_2(png_ptr, png_ptr->big_row_buf);

     if (png_ptr->interlaced) {
        png_ptr->big_row_buf = (png_byte *)png_malloc_2(png_ptr,
            row_bytes + 48);
     } else {
        png_ptr->big_row_buf = (png_byte *)png_malloc_2(png_ptr,
            row_bytes + 48);
	 }
     png_ptr->old_big_row_buf_size = row_bytes + 48;

#ifdef PNG_ALIGNED_MEMORY_SUPPORTED
     /* Use 16-byte aligned memory for row_buf with at least 16 bytes
      * of padding before and after row_buf.
      */
     png_ptr->row_buf = png_ptr->big_row_buf + 32 -
         (((png_alloc_size_t)png_ptr->big_row_buf + 15) & 0x0F);

     png_ptr->old_big_row_buf_size = row_bytes + 48;
#else
     /* Use 32 bytes of padding before and 16 bytes after row_buf. */
     png_ptr->row_buf = png_ptr->big_row_buf + 32;
#endif
     png_ptr->old_big_row_buf_size = row_bytes + 48;
   }
   
#ifdef PNG_MAX_MALLOC_64K
   if (png_ptr->rowbytes > 65535) {
      png_warning_2(png_ptr, "This image requires a row greater than 64KB");
   }
#endif
   png_warning_2(png_ptr, "check row bytes");
   if (png_ptr->rowbytes > (PNG_SIZE_MAX - 1)) {
      png_warning_2(png_ptr, "Row has too many bytes to allocate in memory");
   }
   png_warning_2(png_ptr, "check row bytes again");
   if ((png_ptr->rowbytes + 1) > png_ptr->old_prev_row_size)
   {
      png_free_2(png_ptr, png_ptr->prev_row);
      png_ptr->prev_row = png_malloc_2(png_ptr, (png_ptr->rowbytes + 1));
      png_ptr->old_prev_row_size = png_ptr->rowbytes + 1;
   }
   png_warning_2(png_ptr, "use memset");
   memset(png_ptr->prev_row, 0, png_ptr->rowbytes + 1);

   Uart_Printf("width = %u,", png_ptr->width);
   Uart_Printf("height = %u,", png_ptr->height);
   Uart_Printf("iwidth = %u,", png_ptr->iwidth);
   Uart_Printf("num_rows = %u,", png_ptr->num_rows);
   Uart_Printf("rowbytes = %lu,", (unsigned long)png_ptr->rowbytes);
   Uart_Printf("irowbytes = %lu",
       (unsigned long)PNG_ROWBYTES(png_ptr->pixel_depth, png_ptr->iwidth) + 1);

   png_ptr->flags |= PNG_FLAG_ROW_INIT;
}

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
    png_int_32 divisor)
{
   int negative = 0;
   png_uint_32 A, T, D;
   png_uint_32 s16, s32, s00;
   int bitshift;
   png_fixed_point result; /* NOTE: signed */
   png_uint_32 d32, d00;
   double r;
   /* Return a * times / divisor, rounded. */
   if (divisor != 0)
   {
      if (a == 0 || times == 0)
      {
         *res = 0;
         return 1;
      }
      else
      {
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
         r = a;
         r *= times;
         r /= divisor;
         r = floor(r+.5);

         /* A png_fixed_point is a 32 bit integer. */
         if (r <= 2147483647. && r >= -2147483648.)
         {
            *res = (png_fixed_point)r;
            return 1;
         }
#else
         if (a < 0)
            negative = 1, A = -a;
         else
            A = a;

         if (times < 0)
            negative = !negative, T = -times;
         else
            T = times;

         if (divisor < 0)
            negative = !negative, D = -divisor;
         else
            D = divisor;

         /* Following can't overflow because the arguments only
          * have 31 bits each, however the result may be 32 bits.
          */
         s16 = (A >> 16) * (T & 0xffff) +
                           (A & 0xffff) * (T >> 16);
         /* Can't overflow because the a*times bit is only 30
          * bits at most.
          */
         s32 = (A >> 16) * (T >> 16) + (s16 >> 16);
         s00 = (A & 0xffff) * (T & 0xffff);

         s16 = (s16 & 0xffff) << 16;
         s00 += s16;

         if (s00 < s16)
            ++s32; /* carry */

         if (s32 < D) /* else overflow */
         {
            /* s32.s00 is now the 64 bit product, do a standard
             * division, we know that s32 < D, so the maximum
             * required shift is 31.
             */
            bitshift = 32;
            result = 0; /* NOTE: signed */

            while (--bitshift >= 0)
            {
               if (bitshift > 0)
                  d32 = D >> (32-bitshift), d00 = D << bitshift;

               else
                  d32 = 0, d00 = D;

               if (s32 > d32)
               {
                  if (s00 < d00) --s32; /* carry */
                  s32 -= d32, s00 -= d00, result += 1<<bitshift;
               }

               else
                  if (s32 == d32 && s00 >= d00)
                     s32 = 0, s00 -= d00, result += 1<<bitshift;
            }

            /* Handle the rounding. */
            if (s00 >= (D >> 1))
               ++result;

            if (negative)
               result = -result;

            /* Check for overflow. */
            if ((negative && result <= 0) || (!negative && result >= 0))
            {
               *res = result;
               return 1;
            }
         }
#endif
      }
   }

   return 0;
}
#endif /* READ_GAMMA || INCH_CONVERSIONS */

/* NOTE: this function expects the *inverse* of the overall gamma transformation
 * required.
 */
static void
png_build_16to8_table(png_struct * png_ptr, png_uint_16 *** ptable,
   unsigned int shift, png_fixed_point gamma_val)
{
   unsigned int num = 1U << (8U - shift);
   unsigned int max_int = (1U << (16U - shift))-1U;
   unsigned int i;
   png_uint_32 last;
   png_uint_16 out;
   png_uint_32 bound;
   png_uint_16pp table = *ptable =
       (png_uint_16pp)png_calloc_2(png_ptr, num * png_sizeof(png_uint_16p));

   /* 'num' is the number of tables and also the number of low bits of low
    * bits of the input 16 bit value used to select a table.  Each table is
    * itself index by the high 8 bits of the value.
    */
   for (i = 0; i < num; i++)
      table[i] = (png_uint_16p)png_malloc_2(png_ptr,
          256 * png_sizeof(png_uint_16));

   /* 'gamma_val' is set to the reciprocal of the value calculated above, so
    * pow(out,g) is an *input* value.  'last' is the last input value set.
    *
    * In the loop 'i' is used to find output values.  Since the output is 8
    * bit there are only 256 possible values.  The tables are set up to
    * select the closest possible output value for each input by finding
    * the input value at the boundary between each pair of output values
    * and filling the table up to that boundary with the lower output
    * value.
    *
    * The boundary values are 0.5,1.5..253.5,254.5.  Since these are 9 bit
    * values the code below uses a 16 bit value in i; the values start at
    * 128.5 (for 0.5) and step by 257, for a total of 254 values (the last
    * entries are filled with 255).  Start i at 128 and fill all 'last'
    * table entries <= 'max'
    */
   last = 0;
   for (i = 0; i < 255; ++i) /* 8 bit output value */
   {
      /* Find the corresponding maximum input value */
      out = (png_uint_16)(i * 257U); /* 16 bit output value */

      /* Find the boundary value in 16 bits: */
      bound = png_gamma_16bit_correct_2(out+128U, gamma_val);

      /* Adjust (round) to (16-shift) bits: */
      bound = (bound * max_int + 32768U)/65535U + 1U;

      while (last < bound)
      {
         table[last & (0xffU >> shift)][last >> (8U - shift)] = out;
         last++;
      }
   }

   /* And fill in the final entries. */
   while (last < (num << 8))
   {
      table[last & (0xff >> shift)][last >> (8U - shift)] = 65535U;
      last++;
   }
}
/* Build a single 8 bit table: same as the 16 bit case but much simpler (and
 * typically much faster).  Note that libpng currently does no sBIT processing
 * (apparently contrary to the spec) so a 256 entry table is always generated.
 */
static void
png_build_8bit_table_2(png_struct * png_ptr, png_byte ** ptable,
   png_fixed_point gamma_val)
{
   unsigned int i;
   png_bytep table = *ptable = (png_bytep)png_malloc_2(png_ptr, 256);

   if (png_gamma_significant_2(gamma_val)) for (i=0; i<256; i++)
      table[i] = png_gamma_8bit_correct_2(i, gamma_val);

   else for (i=0; i<256; ++i)
      table[i] = (png_byte)i;
}

/* We build the 8- or 16-bit gamma tables here.  Note that for 16-bit
 * tables, we don't make a full table if we are reducing to 8-bit in
 * the future.  Note also how the gamma_16 tables are segmented so that
 * we don't need to allocate > 64K chunks for a full 16-bit table.
 */
void png_build_gamma_table_2(png_struct * png_ptr, int bit_depth)
{
  //png_debug(1, "in png_build_gamma_table_2");
  png_byte shift, sig_bit;
  if (bit_depth <= 8)
  {
     png_build_8bit_table_2(png_ptr, &png_ptr->gamma_table,
         png_ptr->screen_gamma > 0 ?  png_reciprocal2_2(png_ptr->gamma,
         png_ptr->screen_gamma) : PNG_FP_1);

#if defined(PNG_READ_BACKGROUND_SUPPORTED) || \
   defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
     if (png_ptr->transformations & ((PNG_BACKGROUND) | PNG_RGB_TO_GRAY))
     {
        png_build_8bit_table_2(png_ptr, &png_ptr->gamma_to_1,
            png_reciprocal_2(png_ptr->gamma));

        png_build_8bit_table_2(png_ptr, &png_ptr->gamma_from_1,
            png_ptr->screen_gamma > 0 ?  png_reciprocal_2(png_ptr->screen_gamma) :
            png_ptr->gamma/* Probably doing rgb_to_gray */);
     }
#endif /* PNG_READ_BACKGROUND_SUPPORTED || PNG_RGB_TO_GRAY_SUPPORTED */
  }
  else
  {
     if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
     {
        sig_bit = png_ptr->sig_bit.red;
        if (png_ptr->sig_bit.green > sig_bit)
           sig_bit = png_ptr->sig_bit.green;
        if (png_ptr->sig_bit.blue > sig_bit)
           sig_bit = png_ptr->sig_bit.blue;
     }
     else
        sig_bit = png_ptr->sig_bit.gray;

     /* 16 bit gamma code uses this equation:
      *
      *   ov = table[(iv & 0xff) >> gamma_shift][iv >> 8]
      *
      * Where 'iv' is the input color value and 'ov' is the output value -
      * pow(iv, gamma).
      *
      * Thus the gamma table consists of up to 256 256 entry tables.  The table
      * is selected by the (8-gamma_shift) most significant of the low 8 bits of
      * the color value then indexed by the upper 8 bits:
      *
      *   table[low bits][high 8 bits]
      *
      * So the table 'n' corresponds to all those 'iv' of:
      *
      *   <all high 8 bit values><n << gamma_shift>..<(n+1 << gamma_shift)-1>
      *
      */
     if (sig_bit > 0 && sig_bit < 16U)
        shift = (png_byte)(16U - sig_bit); /* shift == insignificant bits */

     else
        shift = 0; /* keep all 16 bits */

     if (png_ptr->transformations & PNG_16_TO_8)
     {
        /* PNG_MAX_GAMMA_8 is the number of bits to keep - effectively
         * the significant bits in the *input* when the output will
         * eventually be 8 bits.  By default it is 11.
         */
        if (shift < (16U - PNG_MAX_GAMMA_8))
           shift = (16U - PNG_MAX_GAMMA_8);
     }

     if (shift > 8U)
        shift = 8U; /* Guarantees at least one table! */

     png_ptr->gamma_shift = shift;

#ifdef PNG_16BIT_SUPPORTED
     if (png_ptr->transformations & (PNG_16_TO_8 | PNG_BACKGROUND))
#endif
         png_build_16to8_table(png_ptr, &png_ptr->gamma_16_table, shift,
         png_ptr->screen_gamma > 0 ? png_product2_2(png_ptr->gamma,
         png_ptr->screen_gamma) : PNG_FP_1);

#ifdef PNG_16BIT_SUPPORTED
     else
         png_build_16bit_table_2(png_ptr, &png_ptr->gamma_16_table, shift,
         png_ptr->screen_gamma > 0 ? png_reciprocal2_2(png_ptr->gamma,
         png_ptr->screen_gamma) : PNG_FP_1);
#endif

#if defined(PNG_READ_BACKGROUND_SUPPORTED) || \
   defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
     if (png_ptr->transformations & (PNG_BACKGROUND | PNG_RGB_TO_GRAY))
     {
        png_build_16bit_table_2(png_ptr, &png_ptr->gamma_16_to_1, shift,
            png_reciprocal_2(png_ptr->gamma));

        /* Notice that the '16 from 1' table should be full precision, however
         * the lookup on this table still uses gamma_shift, so it can't be.
         * TODO: fix this.
         */
        png_build_16bit_table_2(png_ptr, &png_ptr->gamma_16_from_1, shift,
            png_ptr->screen_gamma > 0 ? png_reciprocal_2(png_ptr->screen_gamma) :
            png_ptr->gamma/* Probably doing rgb_to_gray */);
     }
#endif /* PNG_READ_BACKGROUND_SUPPORTED || PNG_RGB_TO_GRAY_SUPPORTED */
  }
}

png_byte
png_gamma_8bit_correct_2(unsigned int value, png_fixed_point gamma_val)
{
	png_int_32 lg2;
    png_fixed_point res;
    double r;
   if (value > 0 && value < 255)
   {
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
         r = floor(255*pow(value/255.,gamma_val*.00001)+.5);
         return (png_byte)r;
#else
         lg2 = png_log8bit(value);

         if (png_muldiv_2(&res, gamma_val, lg2, PNG_FP_1))
            return png_exp8bit(res);

         /* Overflow. */
         value = 0;
#endif
   }

   return (png_byte)value;
}

#ifdef PNG_READ_16_TO_8_SUPPORTED
/* Strip 16 bit depth files to 8 bit depth */
void png_set_strip_16_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_strip_16");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= PNG_16_TO_8;
}
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
void png_set_strip_alpha_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_strip_alpha");

   if (png_ptr == NULL)
      return;

   png_ptr->flags |= PNG_FLAG_STRIP_ALPHA;
}
#endif

#if defined(PNG_READ_PACK_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
/* Turn on pixel packing */
void png_set_packing_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_packing");

   if (png_ptr == NULL)
      return;

   if (png_ptr->bit_depth < 8)
   {
      png_ptr->transformations |= PNG_PACK;
      png_ptr->usr_bit_depth = 8;
   }
}
#endif

#if defined(PNG_READ_PACKSWAP_SUPPORTED)||defined(PNG_WRITE_PACKSWAP_SUPPORTED)
/* Turn on packed pixel swapping */
void png_set_packswap_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_packswap");

   if (png_ptr == NULL)
      return;

   if (png_ptr->bit_depth < 8)
      png_ptr->transformations |= PNG_PACKSWAP;
}
#endif

png_uint_32 png_get_valid_2(png_struct * png_ptr, png_info * info_ptr,
    png_uint_32 flag)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->valid & flag);

   return(0);
}

/* Expand paletted images to RGB, expand grayscale images of
 * less than 8-bit depth to 8-bit depth, and expand tRNS chunks
 * to alpha channels.
 */
void png_set_expand_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_expand");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}

void png_set_invert_mono_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_invert_mono");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= PNG_INVERT_MONO;
}

#ifdef PNG_sBIT_SUPPORTED
png_uint_32 png_get_sBIT_2(png_struct * png_ptr, png_info * info_ptr,
    png_color_8 ** sig_bit)
{
   //png_debug1(1, "in %s retrieval function", "sBIT");

   if (png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_sBIT)
       && sig_bit != NULL)
   {
      *sig_bit = &(info_ptr->sig_bit);
      return (PNG_INFO_sBIT);
   }

   return (0);
}
#endif

#if defined(PNG_READ_SHIFT_SUPPORTED) || defined(PNG_WRITE_SHIFT_SUPPORTED)
void png_set_shift_2(png_struct * png_ptr, png_color_8 * true_bits)
{
   //png_debug(1, "in png_set_shift");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= PNG_SHIFT;
   png_ptr->shift = *true_bits;
}
#endif

#if defined(PNG_READ_BGR_SUPPORTED) || defined(PNG_WRITE_BGR_SUPPORTED)
/* Turn on BGR-to-RGB mapping */
void png_set_bgr_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_bgr");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= PNG_BGR;
}
#endif

#if defined(PNG_READ_SWAP_ALPHA_SUPPORTED) || \
    defined(PNG_WRITE_SWAP_ALPHA_SUPPORTED)
void png_set_swap_alpha_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_swap_alpha");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= PNG_SWAP_ALPHA;
}
#endif

#if defined(PNG_READ_SWAP_SUPPORTED) || defined(PNG_WRITE_SWAP_SUPPORTED)
/* Turn on 16 bit byte swapping */
void png_set_swap_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_swap");

   if (png_ptr == NULL)
      return;

   if (png_ptr->bit_depth == 16)
      png_ptr->transformations |= PNG_SWAP_BYTES;
}
#endif

#if defined(PNG_READ_INVERT_ALPHA_SUPPORTED) || \
    defined(PNG_WRITE_INVERT_ALPHA_SUPPORTED)
void png_set_invert_alpha_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_invert_alpha");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= PNG_INVERT_ALPHA;
}
#endif

/* Expand grayscale images of less than 8-bit depth to 8 bits. */
void png_set_expand_gray_1_2_4_to_8_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_expand_gray_1_2_4_to_8");

   if (png_ptr == NULL)
      return;

   png_ptr->transformations |= PNG_EXPAND;
   png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
void png_set_gray_to_rgb_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_gray_to_rgb");

   if (png_ptr != NULL)
   {
      /* Because rgb must be 8 bits or more: */
      png_set_expand_gray_1_2_4_to_8_2(png_ptr);
      png_ptr->transformations |= PNG_GRAY_TO_RGB;
      png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
   }
}
#endif

void /* PRIVATE */
png_read_transform_info_2(png_struct * png_ptr, png_info * info_ptr)
{
   //png_debug(1, "in png_read_transform_info");

#ifdef PNG_READ_EXPAND_SUPPORTED
   if (png_ptr->transformations & PNG_EXPAND)
   {
      if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         if (png_ptr->num_trans &&
             (png_ptr->transformations & PNG_EXPAND_tRNS))
            info_ptr->color_type = PNG_COLOR_TYPE_RGB_ALPHA;

         else
            info_ptr->color_type = PNG_COLOR_TYPE_RGB;

         info_ptr->bit_depth = 8;
         info_ptr->num_trans = 0;
      }
      else
      {
         if (png_ptr->num_trans)
         {
            if (png_ptr->transformations & PNG_EXPAND_tRNS)
               info_ptr->color_type |= PNG_COLOR_MASK_ALPHA;
         }
         if (info_ptr->bit_depth < 8)
            info_ptr->bit_depth = 8;

         info_ptr->num_trans = 0;
      }
   }
#endif

#ifdef PNG_READ_BACKGROUND_SUPPORTED
   if (png_ptr->transformations & PNG_BACKGROUND)
   {
      info_ptr->color_type = (png_byte)(info_ptr->color_type &
          ~PNG_COLOR_MASK_ALPHA);
      info_ptr->num_trans = 0;
      info_ptr->background = png_ptr->background;
   }
#endif

#ifdef PNG_READ_GAMMA_SUPPORTED
   if (png_ptr->transformations & PNG_GAMMA)
   {
      info_ptr->gamma = png_ptr->gamma;
   }
#endif

#ifdef PNG_READ_16_TO_8_SUPPORTED
#ifdef PNG_READ_16BIT_SUPPORTED
   if ((png_ptr->transformations & PNG_16_TO_8) && (info_ptr->bit_depth == 16))
      info_ptr->bit_depth = 8;
#else
   /* Force chopping 16-bit input down to 8 */
   if (info_ptr->bit_depth == 16)
   {
      png_ptr->transformations |= PNG_16_TO_8;
      info_ptr->bit_depth = 8;
   }
#endif
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   if (png_ptr->transformations & PNG_GRAY_TO_RGB)
      info_ptr->color_type |= PNG_COLOR_MASK_COLOR;
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
   if (png_ptr->transformations & PNG_RGB_TO_GRAY)
      info_ptr->color_type &= ~PNG_COLOR_MASK_COLOR;
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
   if (png_ptr->transformations & PNG_QUANTIZE)
   {
      if (((info_ptr->color_type == PNG_COLOR_TYPE_RGB) ||
          (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)) &&
          png_ptr->palette_lookup && info_ptr->bit_depth == 8)
      {
         info_ptr->color_type = PNG_COLOR_TYPE_PALETTE;
      }
   }
#endif

#ifdef PNG_READ_PACK_SUPPORTED
   if ((png_ptr->transformations & PNG_PACK) && (info_ptr->bit_depth < 8))
      info_ptr->bit_depth = 8;
#endif

   if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      info_ptr->channels = 1;

   else if (info_ptr->color_type & PNG_COLOR_MASK_COLOR)
      info_ptr->channels = 3;

   else
      info_ptr->channels = 1;

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   if (png_ptr->flags & PNG_FLAG_STRIP_ALPHA)
      info_ptr->color_type &= ~PNG_COLOR_MASK_ALPHA;
#endif

   if (info_ptr->color_type & PNG_COLOR_MASK_ALPHA)
      info_ptr->channels++;

#ifdef PNG_READ_FILLER_SUPPORTED
   /* STRIP_ALPHA and FILLER allowed:  MASK_ALPHA bit stripped above */
   if ((png_ptr->transformations & PNG_FILLER) &&
       ((info_ptr->color_type == PNG_COLOR_TYPE_RGB) ||
       (info_ptr->color_type == PNG_COLOR_TYPE_GRAY)))
   {
      info_ptr->channels++;
      /* If adding a true alpha channel not just filler */
      if (png_ptr->transformations & PNG_ADD_ALPHA)
         info_ptr->color_type |= PNG_COLOR_MASK_ALPHA;
   }
#endif

#if defined(PNG_USER_TRANSFORM_PTR_SUPPORTED) && \
defined(PNG_READ_USER_TRANSFORM_SUPPORTED)
   if (png_ptr->transformations & PNG_USER_TRANSFORM)
   {
      if (info_ptr->bit_depth < png_ptr->user_transform_depth)
         info_ptr->bit_depth = png_ptr->user_transform_depth;

      if (info_ptr->channels < png_ptr->user_transform_channels)
         info_ptr->channels = png_ptr->user_transform_channels;
   }
#endif

   info_ptr->pixel_depth = (png_byte)(info_ptr->channels *
       info_ptr->bit_depth);

   info_ptr->rowbytes = PNG_ROWBYTES(info_ptr->pixel_depth, info_ptr->width);

#ifndef PNG_READ_EXPAND_SUPPORTED
   if (png_ptr)
      return;
#endif
}

/* Optional call to update the users info_ptr structure */
void png_read_update_info_2(png_struct * png_ptr, png_info * info_ptr)
{
   //png_debug(1, "in png_read_update_info");

   if (png_ptr == NULL)
      return;

   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT)) {
      png_warning_2(png_ptr, "read start row");
      png_read_start_row_2(png_ptr);
   }

   else
      png_warning_2(png_ptr,
          "Ignoring extra png_read_update_info() call;"
          " row buffer not reallocated");
   png_warning_2(png_ptr, "read transform info");
   png_read_transform_info_2(png_ptr, info_ptr);
}

png_uint_16
png_gamma_16bit_correct_2(unsigned int value, png_fixed_point gamma_val)
{
   png_int_32 lg2;
   png_fixed_point res;
   double r;
   if (value > 0 && value < 65535)
   {
   		 lg2 = png_log16bit_2(value);
         
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
         r = floor(65535*pow(value/65535.,gamma_val*.00001)+.5);
         return (png_uint_16)r;
#else
         if (png_muldiv_2(&res, gamma_val, lg2, PNG_FP_1))
            return png_exp16bit(res);
         /* Overflow. */
         value = 0;
#endif
   }
   return (png_uint_16)value;
}

void * png_zalloc_2(void * png_ptr, uInt items, uInt size)
{
   void * ptr;
   png_struct * p = (png_struct *)png_ptr;
   png_uint_32 save_flags=p->flags;
   png_alloc_size_t num_bytes;

   if (png_ptr == NULL)
      return (NULL);

   if (items > PNG_UINT_32_MAX/size)
   {
     ///png_warning (p, "Potential overflow in png_zalloc()");
     return (NULL);
   }
   num_bytes = (png_alloc_size_t)(items * size);

   p->flags|=PNG_FLAG_MALLOC_NULL_MEM_OK;
   ptr = (void *)png_malloc_2((png_struct *)png_ptr, num_bytes);
   p->flags=save_flags;

   return ((void *)ptr);
}

void png_free_3(png_struct * png_ptr, void * ptr)
{
   if (png_ptr == NULL || ptr == NULL)
      return;
   if (png_ptr->free_fn != NULL)
   {
      (*(png_ptr->free_fn))(png_ptr, ptr);
      return;
   }
}

/* Function to free memory for zlib */
void /* PRIVATE */
png_zfree_2(void * png_ptr, void * ptr)
{
   png_free_3((png_struct *)png_ptr, (void *)ptr);
}

void png_set_mem_fn_2(png_struct * png_ptr, void * mem_ptr, png_malloc_ptr 
    malloc_fn, void (* free_fn)(png_struct *, void *))
{
   if (png_ptr != NULL)
   {
      png_ptr->mem_ptr = mem_ptr;
      png_ptr->malloc_fn = malloc_fn;
      png_ptr->free_fn = free_fn;
   }
}

void png_set_error_fn_2(png_struct * png_ptr, void * error_ptr,
    void (* error_fn)(png_struct *, png_const_charp), void (* warning_fn)(png_struct *, png_const_charp))
{
   if (png_ptr == NULL)
      return;

   png_ptr->error_ptr = error_ptr;
   png_ptr->error_fn = error_fn;
   png_ptr->warning_fn = warning_fn;
}

void * png_malloc_warn_2(png_struct * png_ptr, png_alloc_size_t size)
{
   void * ptr;
   png_uint_32 save_flags;
   if (png_ptr == NULL)
      return (NULL);

   save_flags = png_ptr->flags;
   png_ptr->flags|=PNG_FLAG_MALLOC_NULL_MEM_OK;
   ptr = (void *)png_malloc_2((png_struct *)png_ptr, size);
   png_ptr->flags=save_flags;
   return(ptr);
}

void png_free_2(png_struct * png_ptr, void * ptr)
{
   if (png_ptr == NULL || ptr == NULL)
      return;

#ifdef PNG_USER_MEM_SUPPORTED
   if (png_ptr->free_fn != NULL)
   {
      (*(png_ptr->free_fn))(png_ptr, ptr);
      return;
   }
#endif
   //else
   ///   png_free_default(png_ptr, ptr);
}

void * png_malloc_2(png_struct * png_ptr, png_alloc_size_t size)
{
   void * ret;
   if (png_ptr == NULL || size == 0)
      return (NULL);

   if (png_ptr->malloc_fn != NULL)
      ret = ((void *)(*(png_ptr->malloc_fn))(png_ptr, (png_size_t)size));

   return (ret);
}

void png_free_data_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 mask,
   int num)
{
   int i;
   int row;
   //png_debug(1, "in png_free_data_2");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

#ifdef PNG_TEXT_SUPPORTED
   /* Free text item num or (if num == -1) all text items */
   if ((mask & PNG_FREE_TEXT) & info_ptr->free_me)
   {
      if (num != -1)
      {
         if (info_ptr->text && info_ptr->text[num].key)
         {
            png_free_2(png_ptr, info_ptr->text[num].key);
            info_ptr->text[num].key = NULL;
         }
      }

      else
      {
         for (i = 0; i < info_ptr->num_text; i++)
             png_free_data_2(png_ptr, info_ptr, PNG_FREE_TEXT, i);
         png_free_2(png_ptr, info_ptr->text);
         info_ptr->text = NULL;
         info_ptr->num_text=0;
      }
   }
#endif

#ifdef PNG_tRNS_SUPPORTED
   /* Free any tRNS entry */
   if ((mask & PNG_FREE_TRNS) & info_ptr->free_me)
   {
      png_free_2(png_ptr, info_ptr->trans_alpha);
      info_ptr->trans_alpha = NULL;
      info_ptr->valid &= ~PNG_INFO_tRNS;
   }
#endif

#ifdef PNG_sCAL_SUPPORTED
   /* Free any sCAL entry */
   if ((mask & PNG_FREE_SCAL) & info_ptr->free_me)
   {
#if defined(PNG_FIXED_POINT_SUPPORTED) && !defined(PNG_FLOATING_POINT_SUPPORTED)
      png_free_2(png_ptr, info_ptr->scal_s_width);
      png_free_2(png_ptr, info_ptr->scal_s_height);
      info_ptr->scal_s_width = NULL;
      info_ptr->scal_s_height = NULL;
#endif
      info_ptr->valid &= ~PNG_INFO_sCAL;
   }
#endif

#ifdef PNG_pCAL_SUPPORTED
   /* Free any pCAL entry */
   if ((mask & PNG_FREE_PCAL) & info_ptr->free_me)
   {
      png_free_2(png_ptr, info_ptr->pcal_purpose);
      png_free_2(png_ptr, info_ptr->pcal_units);
      info_ptr->pcal_purpose = NULL;
      info_ptr->pcal_units = NULL;
      if (info_ptr->pcal_params != NULL)
      {
        for (i = 0; i < (int)info_ptr->pcal_nparams; i++)
        {
           png_free_2(png_ptr, info_ptr->pcal_params[i]);
           info_ptr->pcal_params[i] = NULL;
        }
        png_free_2(png_ptr, info_ptr->pcal_params);
        info_ptr->pcal_params = NULL;
      }
      info_ptr->valid &= ~PNG_INFO_pCAL;
   }
#endif

#ifdef PNG_iCCP_SUPPORTED
   /* Free any iCCP entry */
   if ((mask & PNG_FREE_ICCP) & info_ptr->free_me)
   {
      png_free_2(png_ptr, info_ptr->iccp_name);
      png_free_2(png_ptr, info_ptr->iccp_profile);
      info_ptr->iccp_name = NULL;
      info_ptr->iccp_profile = NULL;
      info_ptr->valid &= ~PNG_INFO_iCCP;
   }
#endif

#ifdef PNG_sPLT_SUPPORTED
   /* Free a given sPLT entry, or (if num == -1) all sPLT entries */
   if ((mask & PNG_FREE_SPLT) & info_ptr->free_me)
   {
      if (num != -1)
      {
         if (info_ptr->splt_palettes)
         {
            png_free_2(png_ptr, info_ptr->splt_palettes[num].name);
            png_free_2(png_ptr, info_ptr->splt_palettes[num].entries);
            info_ptr->splt_palettes[num].name = NULL;
            info_ptr->splt_palettes[num].entries = NULL;
         }
      }

      else
      {
         if (info_ptr->splt_palettes_num)
         {
            for (i = 0; i < (int)info_ptr->splt_palettes_num; i++)
               png_free_data_2(png_ptr, info_ptr, PNG_FREE_SPLT, i);

            png_free_2(png_ptr, info_ptr->splt_palettes);
            info_ptr->splt_palettes = NULL;
            info_ptr->splt_palettes_num = 0;
         }
         info_ptr->valid &= ~PNG_INFO_sPLT;
      }
   }
#endif

#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
   if (png_ptr->unknown_chunk.data)
   {
      png_free_2(png_ptr, png_ptr->unknown_chunk.data);
      png_ptr->unknown_chunk.data = NULL;
   }

   if ((mask & PNG_FREE_UNKN) & info_ptr->free_me)
   {
      if (num != -1)
      {
          if (info_ptr->unknown_chunks)
          {
             png_free_2(png_ptr, info_ptr->unknown_chunks[num].data);
             info_ptr->unknown_chunks[num].data = NULL;
          }
      }

      else
      {
         if (info_ptr->unknown_chunks_num)
         {
            for (i = 0; i < info_ptr->unknown_chunks_num; i++)
               png_free_data_2(png_ptr, info_ptr, PNG_FREE_UNKN, i);

            png_free_2(png_ptr, info_ptr->unknown_chunks);
            info_ptr->unknown_chunks = NULL;
            info_ptr->unknown_chunks_num = 0;
         }
      }
   }
#endif

#ifdef PNG_hIST_SUPPORTED
   /* Free any hIST entry */
   if ((mask & PNG_FREE_HIST)  & info_ptr->free_me)
   {
      png_free_2(png_ptr, info_ptr->hist);
      info_ptr->hist = NULL;
      info_ptr->valid &= ~PNG_INFO_hIST;
   }
#endif

   /* Free any PLTE entry that was internally allocated */
   if ((mask & PNG_FREE_PLTE) & info_ptr->free_me)
   {
      png_zfree_2(png_ptr, info_ptr->palette);
      info_ptr->palette = NULL;
      info_ptr->valid &= ~PNG_INFO_PLTE;
      info_ptr->num_palette = 0;
   }

#ifdef PNG_INFO_IMAGE_SUPPORTED
   /* Free any image bits attached to the info structure */
   if ((mask & PNG_FREE_ROWS) & info_ptr->free_me)
   {
      if (info_ptr->row_pointers)
      {
         for (row = 0; row < (int)info_ptr->height; row++)
         {
            png_free_2(png_ptr, info_ptr->row_pointers[row]);
            info_ptr->row_pointers[row] = NULL;
         }
         png_free_2(png_ptr, info_ptr->row_pointers);
         info_ptr->row_pointers = NULL;
      }
      info_ptr->valid &= ~PNG_INFO_IDAT;
   }
#endif

   if (num != -1)
      mask &= ~PNG_FREE_MUL;

   info_ptr->free_me &= ~mask;
}

#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_INCH_CONVERSIONS_SUPPORTED)
/* The following is for when the caller doesn't much care about the
 * result.
 */
png_fixed_point
png_muldiv_warn_2(png_struct * png_ptr, png_fixed_point a, png_int_32 times,
    png_int_32 divisor)
{
   png_fixed_point result;

   if (png_muldiv_2(&result, a, times, divisor))
      return result;

   //png_warning(png_ptr, "fixed point overflow ignored");
   return 0;
}


#endif

void /* PRIVATE */
png_check_IHDR_2(png_struct * png_ptr,
   png_uint_32 width, png_uint_32 height, int bit_depth,
   int color_type, int interlace_type, int compression_type,
   int filter_type)
{
   int error = 0;

   /* Check for width and height valid values */
   if (width == 0)
   {
      //png_warning(png_ptr, "Image width is zero in IHDR");
      error = 1;
   }

   if (height == 0)
   {
      //png_warning(png_ptr, "Image height is zero in IHDR");
      error = 1;
   }

#ifdef PNG_SET_USER_LIMITS_SUPPORTED
   if (width > png_ptr->user_width_max || width > PNG_USER_WIDTH_MAX)

#else
   if (width > PNG_USER_WIDTH_MAX)
#endif
   {
      //png_warning(png_ptr, "Image width exceeds user limit in IHDR");
      error = 1;
   }

#ifdef PNG_SET_USER_LIMITS_SUPPORTED
   if (height > png_ptr->user_height_max || height > PNG_USER_HEIGHT_MAX)
#else
   if (height > PNG_USER_HEIGHT_MAX)
#endif
   {
      //png_warning(png_ptr, "Image height exceeds user limit in IHDR");
      error = 1;
   }

   if (width > PNG_UINT_31_MAX)
   {
      //png_warning(png_ptr, "Invalid image width in IHDR");
      error = 1;
   }

   if (height > PNG_UINT_31_MAX)
   {
      //png_warning(png_ptr, "Invalid image height in IHDR");
      error = 1;
   }

   //if (width > (PNG_UINT_32_MAX
   //              >> 3)      /* 8-byte RGBA pixels */
   //              - 48       /* bigrowbuf hack */
   //              - 1        /* filter byte */
   //              - 7*8      /* rounding of width to multiple of 8 pixels */
   //              - 8)       /* extra max_pixel_depth pad */
      //png_warning(png_ptr, "Width is too large for libpng to process pixels");

   /* Check other values */
   if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 &&
       bit_depth != 8 && bit_depth != 16)
   {
      //png_warning(png_ptr, "Invalid bit depth in IHDR");
      error = 1;
   }

   if (color_type < 0 || color_type == 1 ||
       color_type == 5 || color_type > 6)
   {
      //png_warning(png_ptr, "Invalid color type in IHDR");
      error = 1;
   }

   if (((color_type == PNG_COLOR_TYPE_PALETTE) && bit_depth > 8) ||
       ((color_type == PNG_COLOR_TYPE_RGB ||
         color_type == PNG_COLOR_TYPE_GRAY_ALPHA ||
         color_type == PNG_COLOR_TYPE_RGB_ALPHA) && bit_depth < 8))
   {
      //png_warning(png_ptr, "Invalid color type/bit depth combination in IHDR");
      error = 1;
   }

   if (interlace_type >= PNG_INTERLACE_LAST)
   {
      //png_warning(png_ptr, "Unknown interlace method in IHDR");
      error = 1;
   }

   if (compression_type != PNG_COMPRESSION_TYPE_BASE)
   {
      //png_warning(png_ptr, "Unknown compression method in IHDR");
      error = 1;
   }

#ifdef PNG_MNG_FEATURES_SUPPORTED
   /* Accept filter_method 64 (intrapixel differencing) only if
    * 1. Libpng was compiled with PNG_MNG_FEATURES_SUPPORTED and
    * 2. Libpng did not read a PNG signature (this filter_method is only
    *    used in PNG datastreams that are embedded in MNG datastreams) and
    * 3. The application called png_permit_mng_features with a mask that
    *    included PNG_FLAG_MNG_FILTER_64 and
    * 4. The filter_method is 64 and
    * 5. The color_type is RGB or RGBA
    */
   if ((png_ptr->mode & PNG_HAVE_PNG_SIGNATURE) &&
       png_ptr->mng_features_permitted)
      //png_warning(png_ptr, "MNG features are not allowed in a PNG datastream");

   if (filter_type != PNG_FILTER_TYPE_BASE)
   {
      if (!((png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
          (filter_type == PNG_INTRAPIXEL_DIFFERENCING) &&
          ((png_ptr->mode & PNG_HAVE_PNG_SIGNATURE) == 0) &&
          (color_type == PNG_COLOR_TYPE_RGB ||
          color_type == PNG_COLOR_TYPE_RGB_ALPHA)))
      {
         //png_warning(png_ptr, "Unknown filter method in IHDR");
         error = 1;
      }

      if (png_ptr->mode & PNG_HAVE_PNG_SIGNATURE)
      {
         //png_warning(png_ptr, "Invalid filter method in IHDR");
         error = 1;
      }
   }

#else
   if (filter_type != PNG_FILTER_TYPE_BASE)
   {
      //png_warning(png_ptr, "Unknown filter method in IHDR");
      error = 1;
   }
#endif

   //if (error == 1)
      //png_error(png_ptr, "Invalid IHDR data");
}

#ifdef PNG_ERROR_TEXT_SUPPORTED
#ifdef PNG_FLOATING_POINT_SUPPORTED
void png_fixed_error_2(png_struct * png_ptr, char * name)
{
/*#define fixed_message "fixed point overflow in "
#define fixed_message_ln ((sizeof fixed_message)-1)
   int  iin;
   char msg[fixed_message_ln + PNG_MAX_ERROR_TEXT];
   png_memcpy(msg, fixed_message, fixed_message_ln);
   iin = 0;
   if (name != NULL) while (iin < (PNG_MAX_ERROR_TEXT-1) && name[iin] != 0)
   {
      msg[fixed_message_ln + iin] = name[iin];
      ++iin;
   }
   msg[fixed_message_ln + iin] = 0;*/
   //png_error(png_ptr, msg);
}
#endif
#endif

#if defined(PNG_FLOATING_POINT_SUPPORTED) && \
   !defined(PNG_FIXED_POINT_MACRO_SUPPORTED)
png_fixed_point
png_fixed_2(png_struct * png_ptr, double fp, char * text)
{
   double r = floor(100000 * fp + .5);

   if (r > 2147483647. || r < -2147483648.)
      png_fixed_error_2(png_ptr, text);

   return (png_fixed_point)r;
}
#endif


void * png_calloc_2(png_struct * png_ptr, png_alloc_size_t size)
{
   void * ret;

   ret = (png_malloc_2(png_ptr, size));

   if (ret != NULL)
      memset(ret, 0, (png_size_t)size);

   return (ret);
}

/* more fixed point functions for gammma */
/* Calculate a reciprocal, return 0 on div-by-zero or overflow. */
png_fixed_point
png_reciprocal_2(png_fixed_point a)
{
	png_fixed_point res;
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
   double r = floor(1E10/a+.5);

   if (r <= 2147483647. && r >= -2147483648.)
      return (png_fixed_point)r;
#else
   if (png_muldiv_2(&res, 100000, 100000, a))
      return res;
#endif

   return 0; /* error/overflow */
}

/* The inverse of the above. */
png_fixed_point
png_reciprocal2_2(png_fixed_point a, png_fixed_point b)
{
	png_fixed_point res;
   /* The required result is 1/a * 1/b; the following preserves accuracy. */
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
   double r = 1E15/a;
   r /= b;
   r = floor(r+.5);

   if (r <= 2147483647. && r >= -2147483648.)
      return (png_fixed_point)r;
#else
   /* This may overflow because the range of png_fixed_point isn't symmetric,
    * but this API is only used for the product of file and screen gamma so it
    * doesn't matter that the smallest number it can produce is 1/21474, not
    * 1/100000
    */
   res = png_product2_2(a, b);

   if (res != 0)
      return png_reciprocal_2_2(res);
#endif

   return 0; /* overflow */
}
//#endif /* READ_GAMMA */
/* This does the right thing based on the bit_depth field of the
 * png_struct, interpreting values as 8 or 16 bit.  While the result
 * is nominally a 16 bit value if bit depth is 8 then the result is
 * 8 bit (as are the arguments.)
 */
png_uint_16 /* PRIVATE */
png_gamma_correct_2(png_struct * png_ptr, unsigned int value,
    png_int_32 gamma_val)
{
   if (png_ptr->bit_depth == 8)
      return png_gamma_8bit_correct_2(value, gamma_val);
   else
      return png_gamma_16bit_correct_2(value, gamma_val);
}
/* This is the shared test on whether a gamma value is 'significant' - whether
 * it is worth doing gamma correction.
 */
int /* PRIVATE */
png_gamma_significant_2(png_fixed_point gamma_val)
{
   return gamma_val < PNG_FP_1 - PNG_GAMMA_THRESHOLD_FIXED ||
       gamma_val > PNG_FP_1 + PNG_GAMMA_THRESHOLD_FIXED;
}

//extern int png_sig_cmp(png_const_bytep sig, png_size_t start, png_size_t num_to_check);
int png_sig_cmp_2(png_byte * sig, png_size_t start, png_size_t num_to_check)
{
   png_byte png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

   if (num_to_check > 8)
      num_to_check = 8;

   else if (num_to_check < 1)
      return (-1);

   if (start > 7)
      return (-1);

   if (start + num_to_check > 8)
      num_to_check = 8 - start;

   return ((int)(memcmp(&sig[start], &png_signature[start], num_to_check)));
}

/* Calculate the CRC over a section of data.  We can only pass as
 * much data to this routine as the largest single buffer size.  We
 * also check that this data will actually be used before going to the
 * trouble of calculating it.
 */
void /* PRIVATE */
png_calculate_crc_2(png_struct * png_ptr, png_byte * ptr, png_size_t length)
{
   int need_crc = 1;

   if (png_ptr->chunk_name[0] & 0x20)                     /* ancillary */
   {
      if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_MASK) ==
          (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN))
         need_crc = 0;
   }

   else                                                    /* critical */
   {
      if (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE)
         need_crc = 0;
   }

   if (need_crc)
      png_ptr->crc = crc32(png_ptr->crc, ptr, (uInt)length);
}
/* Reset the CRC variable to 32 bits of 1's.  Care must be taken
 * in case CRC is > 32 bits to leave the top bits 0.
 */
void /* PRIVATE */
png_reset_crc_2(png_struct * png_ptr)
{
   png_ptr->crc = crc32(0, Z_NULL, 0);
}
/* This is an internal routine to free any memory that the info struct is
 * pointing to before re-using it or freeing the struct itself.  Recall
 * that png_free() checks for NULL pointers for us.
 */
void /* PRIVATE */
png_info_destroy_2(png_struct * png_ptr, png_info * info_ptr)
{
   //png_debug(1, "in png_info_destroy");

   png_free_data_2(png_ptr, info_ptr, PNG_FREE_ALL, -1);

#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
   if (png_ptr->num_chunk_list)
   {
      png_free_2(png_ptr, png_ptr->chunk_list);
      png_ptr->chunk_list = NULL;
      png_ptr->num_chunk_list = 0;
   }
#endif

   png_info_init_5(&info_ptr, png_sizeof(png_info));
}

void 
png_destroy_struct_3(void * struct_ptr, void (* free_fn)(png_struct *, void *),
    void * mem_ptr)
{
   png_struct dummy_struct;
   png_structp png_ptr = &dummy_struct;
   if (struct_ptr != NULL)
   {
#ifdef PNG_USER_MEM_SUPPORTED
      if (free_fn != NULL)
      {
         
         png_ptr->mem_ptr=mem_ptr;
         (*(free_fn))(png_ptr, struct_ptr);
         return;
      }
#endif 
   }
}

void * 
png_create_struct_3(int type, void * (* malloc_fn)(png_struct *, 
    png_alloc_size_t), void * mem_ptr)
{
   png_size_t size;
   void * struct_ptr;
   static png_struct dummy_struct;
   png_struct * png_ptr = &dummy_struct;
   
   if (type == PNG_STRUCT_INFO) {
      size = png_sizeof(png_info); 
   } else if (type == PNG_STRUCT_PNG) {
      size = png_sizeof(png_struct);
   } else {
      return (NULL);
   }

#ifdef PNG_USER_MEM_SUPPORTED
   if (malloc_fn != NULL)
   {
      png_ptr->mem_ptr=mem_ptr;
      struct_ptr = (*(malloc_fn))(png_ptr, size);

      if (struct_ptr != NULL)
         memset(struct_ptr, 0, size);

      return (struct_ptr);
   }
#endif

   if (struct_ptr != NULL)
      memset(struct_ptr, 0, size);

   return (struct_ptr);
}

/* Allocate memory for a png_struct or a png_info.  The malloc and
   memset can be replaced by a single call to calloc() if this is thought
   to improve performance noticably. */
void * png_create_struct_4(int type)
{
//#ifdef PNG_USER_MEM_SUPPORTED
   return (png_create_struct_3(type, NULL, NULL));
}

/* Initialize the info structure.  This is now an internal function (0.89)
 * and applications using it are urged to use png_create_info_struct()
 * instead.
 */
/* Free memory allocated by a png_create_struct() call */
void /* PRIVATE */
png_destroy_struct_4(void * struct_ptr)
{
//#  ifdef PNG_USER_MEM_SUPPORTED
   png_destroy_struct_3(struct_ptr, NULL, NULL);
}

void png_info_init_5(png_info ** ptr_ptr, png_size_t png_info_struct_size)
{
   png_info * info_ptr = * ptr_ptr;

   //png_debug(1, "in png_info_init_3");

   if (info_ptr == NULL)
      return;

   if (png_sizeof(png_info) > png_info_struct_size)
   {
      png_destroy_struct_4(info_ptr);
      info_ptr = (png_infop)png_create_struct_4(PNG_STRUCT_INFO);
      *ptr_ptr = info_ptr;
   }

   /* Set everything to 0 */
   memset(info_ptr, 0, png_sizeof(png_info));
}

#if defined(PNG_READ_SUPPORTED) || defined(PNG_WRITE_SUPPORTED)
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
int png_handle_as_unknown_2(png_struct * png_ptr, png_byte * chunk_name)
{
   /* Check chunk_name and return "keep" value if it's on the list, else 0 */
   int i;
   png_byte * p;
   if (png_ptr == NULL || chunk_name == NULL || png_ptr->num_chunk_list<=0)
      return 0;

   p = png_ptr->chunk_list + png_ptr->num_chunk_list*5 - 5;
   for (i = png_ptr->num_chunk_list; i; i--, p -= 5)
      if (!memcmp(chunk_name, p, 4))
        return ((int)*(p + 4));
   return 0;
}
#  endif
#endif /* defined(PNG_READ_SUPPORTED) || defined(PNG_WRITE_SUPPORTED) */


/* ASCII to fp functions */
/* Check an ASCII formated floating point value, see the more detailed
 * comments in pngpriv.h
 */
/* The following is used internally to preserve the 'valid' flag */
#define png_fp_add_2(state, flags) ((state) |= (flags))
#define png_fp_set_2(state, value)\
   ((state) = (value) | ((state) & PNG_FP_WAS_VALID))

/* Internal type codes: bits above the base state! */
#define PNG_FP_SIGN   0  /* [+-] */
#define PNG_FP_DOT    4  /* . */
#define PNG_FP_DIGIT  8  /* [0123456789] */
#define PNG_FP_E     12  /* [Ee] */
int /* PRIVATE */
png_check_fp_number_2(char * string, png_size_t size, int * statep,
   png_size_tp whereami)
{
   int state = *statep;
   png_size_t i = *whereami;
   char ch;
   int type;
   while (i < size)
   {
      /* First find the type of the next character */
      {
         ch = string[i];
         if (ch >= 48 && ch <= 57)
            type = PNG_FP_DIGIT;

         else switch (ch)
         {
         case 43: case 45:  type = PNG_FP_SIGN;  break;
         case 46:           type = PNG_FP_DOT;   break;
         case 69: case 101: type = PNG_FP_E;     break;
         default:           goto PNG_FP_End;
         }
      }

      /* Now deal with this type according to the current
       * state, the type is arranged to not overlap the
       * bits of the PNG_FP_STATE.
       */
      switch ((state & PNG_FP_STATE) + type)
      {
      case PNG_FP_INTEGER + PNG_FP_SIGN:
         if (state & PNG_FP_SAW_ANY)
            goto PNG_FP_End; /* not a part of the number */

         png_fp_add_2(state, PNG_FP_SAW_SIGN);
         break;

      case PNG_FP_INTEGER + PNG_FP_DOT:
         /* Ok as trailer, ok as lead of fraction. */
         if (state & PNG_FP_SAW_DOT) /* two dots */
            goto PNG_FP_End;

         else if (state & PNG_FP_SAW_DIGIT) /* trailing dot? */
            png_fp_add_2(state, PNG_FP_SAW_DOT);

         else
            png_fp_set_2(state, PNG_FP_FRACTION | PNG_FP_SAW_DOT);

         break;

      case PNG_FP_INTEGER + PNG_FP_DIGIT:
         if (state & PNG_FP_SAW_DOT) /* delayed fraction */
            png_fp_set_2(state, PNG_FP_FRACTION | PNG_FP_SAW_DOT);

         png_fp_add_2(state, PNG_FP_SAW_DIGIT + PNG_FP_WAS_VALID);

         break;
      case PNG_FP_INTEGER + PNG_FP_E:
         if ((state & PNG_FP_SAW_DIGIT) == 0)
            goto PNG_FP_End;

         png_fp_set_2(state, PNG_FP_EXPONENT);

         break;

   /* case PNG_FP_FRACTION + PNG_FP_SIGN:
         goto PNG_FP_End; ** no sign in exponent */

   /* case PNG_FP_FRACTION + PNG_FP_DOT:
         goto PNG_FP_End; ** Because SAW_DOT is always set */

      case PNG_FP_FRACTION + PNG_FP_DIGIT:
         png_fp_add_2(state, PNG_FP_SAW_DIGIT + PNG_FP_WAS_VALID);
         break;

      case PNG_FP_FRACTION + PNG_FP_E:
         /* This is correct because the trailing '.' on an
          * integer is handled above - so we can only get here
          * with the sequence ".E" (with no preceding digits).
          */
         if ((state & PNG_FP_SAW_DIGIT) == 0)
            goto PNG_FP_End;

         png_fp_set_2(state, PNG_FP_EXPONENT);

         break;

      case PNG_FP_EXPONENT + PNG_FP_SIGN:
         if (state & PNG_FP_SAW_ANY)
            goto PNG_FP_End; /* not a part of the number */

         png_fp_add_2(state, PNG_FP_SAW_SIGN);

         break;

   /* case PNG_FP_EXPONENT + PNG_FP_DOT:
         goto PNG_FP_End; */

      case PNG_FP_EXPONENT + PNG_FP_DIGIT:
         png_fp_add_2(state, PNG_FP_SAW_DIGIT + PNG_FP_WAS_VALID);

         break;

   /* case PNG_FP_EXPONEXT + PNG_FP_E:
         goto PNG_FP_End; */

      default: goto PNG_FP_End; /* I.e. break 2 */
      }

      /* The character seems ok, continue. */
      ++i;
   }

PNG_FP_End:
   /* Here at the end, update the state and return the correct
    * return code.
    */
   *statep = state;
   *whereami = i;

   return (state & PNG_FP_SAW_DIGIT) != 0;
}


/* The same but for a complete string. */
int png_check_fp_string_2(char * string, png_size_t size)
{
   int        state=0;
   png_size_t char_index=0;

   return png_check_fp_number_2(string, size, &state, &char_index) &&
      (char_index == size || string[char_index] == 0);
}

/* Added at libpng version 1.2.34 and 1.4.0 (moved from pngset.c) */
#  ifdef PNG_CHECK_cHRM_SUPPORTED
/* Added at libpng version 1.2.34 (Dec 8, 2008) and 1.4.0 (Jan 2,
 * 2010: moved from pngset.c) */
/*
 *    Multiply two 32-bit numbers, V1 and V2, using 32-bit
 *    arithmetic, to produce a 64 bit result in the HI/LO words.
 *
 *                  A B
 *                x C D
 *               ------
 *              AD || BD
 *        AC || CB || 0
 *
 *    where A and B are the high and low 16-bit words of V1,
 *    C and D are the 16-bit words of V2, AD is the product of
 *    A and D, and X || Y is (X << 16) + Y.
*/

void /* PRIVATE */
png_64bit_product_2 (long v1, long v2, unsigned long * hi_product,
    unsigned long * lo_product)
{
   int a, b, c, d;
   long lo, hi, x, y;

   a = (v1 >> 16) & 0xffff;
   b = v1 & 0xffff;
   c = (v2 >> 16) & 0xffff;
   d = v2 & 0xffff;

   lo = b * d;                   /* BD */
   x = a * d + c * b;            /* AD + CB */
   y = ((lo >> 16) & 0xffff) + x;

   lo = (lo & 0xffff) | ((y & 0xffff) << 16);
   hi = (y >> 16) & 0xffff;

   hi += a * c;                  /* AC */

   *hi_product = (unsigned long)hi;
   *lo_product = (unsigned long)lo;
}

int /* PRIVATE */
png_check_cHRM_fixed_2(png_struct * png_ptr,
   png_fixed_point white_x, png_fixed_point white_y, png_fixed_point red_x,
   png_fixed_point red_y, png_fixed_point green_x, png_fixed_point green_y,
   png_fixed_point blue_x, png_fixed_point blue_y)
{
   int ret = 1;
   unsigned long xy_hi,xy_lo,yx_hi,yx_lo;

   //png_debug(1, "in function png_check_cHRM_fixed");

   if (png_ptr == NULL)
      return 0;

   if (white_x < 0 || white_y <= 0 ||
         red_x < 0 ||   red_y <  0 ||
       green_x < 0 || green_y <  0 ||
        blue_x < 0 ||  blue_y <  0)
   {
      //png_warning(png_ptr,
       // "Ignoring attempt to set negative chromaticity value");
      ret = 0;
   }
   if (white_x > (png_fixed_point)PNG_UINT_31_MAX ||
       white_y > (png_fixed_point)PNG_UINT_31_MAX ||
         red_x > (png_fixed_point)PNG_UINT_31_MAX ||
         red_y > (png_fixed_point)PNG_UINT_31_MAX ||
       green_x > (png_fixed_point)PNG_UINT_31_MAX ||
       green_y > (png_fixed_point)PNG_UINT_31_MAX ||
        blue_x > (png_fixed_point)PNG_UINT_31_MAX ||
        blue_y > (png_fixed_point)PNG_UINT_31_MAX )
   {
      //png_warning(png_ptr,
      //  "Ignoring attempt to set chromaticity value exceeding 21474.83");
      ret = 0;
   }
   if (white_x > 100000L - white_y)
   {
      //png_warning(png_ptr, "Invalid cHRM white point");
      ret = 0;
   }

   if (red_x > 100000L - red_y)
   {
      //png_warning(png_ptr, "Invalid cHRM red point");
      ret = 0;
   }

   if (green_x > 100000L - green_y)
   {
      //png_warning(png_ptr, "Invalid cHRM green point");
      ret = 0;
   }

   if (blue_x > 100000L - blue_y)
   {
      //png_warning(png_ptr, "Invalid cHRM blue point");
      ret = 0;
   }

   png_64bit_product_2(green_x - red_x, blue_y - red_y, &xy_hi, &xy_lo);
   png_64bit_product_2(green_y - red_y, blue_x - red_x, &yx_hi, &yx_lo);

   if (xy_hi == yx_hi && xy_lo == yx_lo)
   {
      //png_warning(png_ptr,
      //   "Ignoring attempt to set cHRM RGB triangle with zero area");
      ret = 0;
   }

   return ret;
}
#  endif /* PNG_CHECK_cHRM_SUPPORTED */


#ifdef PNG_READ_sCAL_SUPPORTED
#  ifdef PNG_FLOATING_POINT_SUPPORTED
/* Utility used below - a simple accurate power of ten from an integral
 * exponent.
 */
static double
png_pow10(int power)
{
   int recip = 0;
   double d = 1;
   double mult;

   /* Handle negative exponent with a reciprocal at the end because
    * 10 is exact whereas .1 is inexact in base 2
    */
   if (power < 0)
   {
      if (power < DBL_MIN_10_EXP) return 0;
      recip = 1, power = -power;
   }

   if (power > 0)
   {
      /* Decompose power bitwise. */
      mult = 10;
      do
      {
         if (power & 1) d *= mult;
         mult *= mult;
         power >>= 1;
      }
      while (power > 0);

      if (recip) d = 1/d;
   }
   /* else power is 0 and d is 1 */
   return d;
}
/* Function to format a floating point value in ASCII with a given
 * precision.
 */
void /* PRIVATE */
png_ascii_from_fp_2(png_struct * png_ptr, char * ascii, png_size_t size,
    double fp, unsigned int precision)
{
   int czero, clead, cdigits;
   char exponent[10];
   double d;
   int exp_b10;       /* A base 10 exponent */
   double base;   /* 10^exp_b10 */
   double test;
   /* We use standard functions from math.h, but not printf because
    * that would require stdio.  The caller must supply a buffer of
    * sufficient size or we will png_error.  The tests on size and
    * the space in ascii[] consumed are indicated below.
    */
   if (precision < 1)
      precision = DBL_DIG;

   /* Enforce the limit of the implementation precision too. */
   if (precision > DBL_DIG+1)
      precision = DBL_DIG+1;

   /* Basic sanity checks */
   if (size >= precision+5) /* See the requirements below. */
   {
      if (fp < 0)
      {
         fp = -fp;
         *ascii++ = 45; /* '-'  PLUS 1 TOTAL 1*/
         --size;
      }

      if (fp >= DBL_MIN && fp <= DBL_MAX)
      {
         /* First extract a base 10 exponent of the number,
          * the calculation below rounds down when converting
          * from base 2 to base 10 (multiply by log10(2) -
          * 0.3010, but 77/256 is 0.3008, so exp_b10 needs to
          * be increased.  Note that the arithmetic shift
          * performs a floor() unlike C arithmetic - using a
          * C multiply would break the following for negative
          * exponents.
          */
         (void)frexp(fp, &exp_b10); /* exponent to base 2 */

         exp_b10 = (exp_b10 * 77) >> 8; /* <= exponent to base 10 */

         /* Avoid underflow here. */
         base = png_pow10(exp_b10); /* May underflow */

         while (base < DBL_MIN || base < fp)
         {
            /* And this may overflow. */
            test = png_pow10(exp_b10+1);
            if (test <= DBL_MAX)
               ++exp_b10, base = test;
            else
               break;
         }

         /* Normalize fp and correct exp_b10, after this fp is in the
          * range [.1,1) and exp_b10 is both the exponent and the digit
          * *before* which the decimal point should be inserted
          * (starting with 0 for the first digit).  Note that this
          * works even if 10^exp_b10 is out of range because of the
          * test on DBL_MAX above.
          */
         fp /= base;
         while (fp >= 1) fp /= 10, ++exp_b10;

         /* Because of the code above fp may, at this point, be
          * less than .1, this is ok because the code below can
          * handle the leading zeros this generates, so no attempt
          * is made to correct that here.
          */

         {
            /* Allow up to two leading zeros - this will not lengthen
             * the number compared to using E-n.
             */
            if (exp_b10 < 0 && exp_b10 > -3) /* PLUS 3 TOTAL 4 */
            {
               czero = -exp_b10; /* PLUS 2 digits: TOTAL 3 */
               exp_b10 = 0;      /* Dot added below before first output. */
            }
            else
               czero = 0;    /* No zeros to add */

            /* Generate the digit list, stripping trailing zeros and
             * inserting a '.' before a digit if the exponent is 0.
             */
            clead = czero; /* Count of leading zeros */
            cdigits = 0;   /* Count of digits in list. */

            do
            {
               fp *= 10;
               /* Use modf here, not floor and subtract, so that
                * the separation is done in one step.  At the end
                * of the loop don't break the number into parts so
                * that the final digit is rounded.
                */
               if (cdigits+czero-clead+1 < (int)precision)
                  fp = modf(fp, &d);

               else
               {
                  d = floor(fp + .5);

                  if (d > 9)
                  {
                     /* Rounding up to 10, handle that here. */
                     if (czero > 0)
                     {
                        --czero, d = 1;
                        if (cdigits == 0) --clead;
                     }
                     else
                     {
                        while (cdigits > 0 && d > 9)
                        {
                           int ch = *--ascii;

                           if (exp_b10 != (-1))
                              ++exp_b10;

                           else if (ch == 46)
                           {
                              ch = *--ascii, ++size;
                              /* Advance exp_b10 to '1', so that the
                               * decimal point happens after the
                               * previous digit.
                               */
                              exp_b10 = 1;
                           }

                           --cdigits;
                           d = ch - 47;  /* I.e. 1+(ch-48) */
                        }

                        /* Did we reach the beginning? If so adjust the
                         * exponent but take into account the leading
                         * decimal point.
                         */
                        if (d > 9)  /* cdigits == 0 */
                        {
                           if (exp_b10 == (-1))
                           {
                              /* Leading decimal point (plus zeros?), if
                               * we lose the decimal point here it must
                               * be reentered below.
                               */
                              int ch = *--ascii;

                              if (ch == 46)
                                 ++size, exp_b10 = 1;

                              /* Else lost a leading zero, so 'exp_b10' is
                               * still ok at (-1)
                               */
                           }
                           else
                              ++exp_b10;

                           /* In all cases we output a '1' */
                           d = 1;
                        }
                     }
                  }
                  fp = 0; /* Guarantees termination below. */
               }

               if (d == 0)
               {
                  ++czero;
                  if (cdigits == 0) ++clead;
               }
               else
               {
                  /* Included embedded zeros in the digit count. */
                  cdigits += czero - clead;
                  clead = 0;

                  while (czero > 0)
                  {
                     /* exp_b10 == (-1) means we just output the decimal
                      * place - after the DP don't adjust 'exp_b10' any
                      * more!
                      */
                     if (exp_b10 != (-1))
                     {
                        if (exp_b10 == 0) *ascii++ = 46, --size;
                        /* PLUS 1: TOTAL 4 */
                        --exp_b10;
                     }
                     *ascii++ = 48, --czero;
                  }

                  if (exp_b10 != (-1))
                  {
                     if (exp_b10 == 0) *ascii++ = 46, --size; /* counted
                                                                 above */
                     --exp_b10;
                  }
                  *ascii++ = (char)(48 + (int)d), ++cdigits;
               }
            }
            while (cdigits+czero-clead < (int)precision && fp > DBL_MIN);

            /* The total output count (max) is now 4+precision */

            /* Check for an exponent, if we don't need one we are
             * done and just need to terminate the string.  At
             * this point exp_b10==(-1) is effectively if flag - it got
             * to '-1' because of the decrement after outputing
             * the decimal point above (the exponent required is
             * *not* -1!)
             */
            if (exp_b10 >= (-1) && exp_b10 <= 2)
            {
               /* The following only happens if we didn't output the
                * leading zeros above for negative exponent, so this
                * doest add to the digit requirement.  Note that the
                * two zeros here can only be output if the two leading
                * zeros were *not* output, so this doesn't increase
                * the output count.
                */
               while (--exp_b10 >= 0) *ascii++ = 48;

               *ascii = 0;

               /* Total buffer requirement (including the '\0') is
                * 5+precision - see check at the start.
                */
               return;
            }

            /* Here if an exponent is required, adjust size for
             * the digits we output but did not count.  The total
             * digit output here so far is at most 1+precision - no
             * decimal point and no leading or trailing zeros have
             * been output.
             */
            size -= cdigits;

            *ascii++ = 69, --size;    /* 'E': PLUS 1 TOTAL 2+precision*/
            if (exp_b10 < 0)
            {
               *ascii++ = 45, --size; /* '-': PLUS 1 TOTAL 3+precision */
               exp_b10 = -exp_b10;
            }

            cdigits = 0;

            while (exp_b10 > 0)
            {
               exponent[cdigits++] = (char)(48 + exp_b10 % 10);
               exp_b10 /= 10;
            }

            /* Need another size check here for the exponent digits, so
             * this need not be considered above.
             */
            if ((int)size > cdigits)
            {
               while (cdigits > 0) *ascii++ = exponent[--cdigits];

               *ascii = 0;

               return;
            }
         }
      }
      else if (!(fp >= DBL_MIN))
      {
         *ascii++ = 48; /* '0' */
         *ascii = 0;
         return;
      }
      else
      {
         *ascii++ = 105; /* 'i' */
         *ascii++ = 110; /* 'n' */
         *ascii++ = 102; /* 'f' */
         *ascii = 0;
         return;
      }
   }

   /* Here on buffer too small. */
   //png_error(png_ptr, "ASCII conversion buffer too small");
}

//#  endif /* FLOATING_POINT */

#  ifdef PNG_FIXED_POINT_SUPPORTED
/* Function to format a fixed point value in ASCII.
 */
void /* PRIVATE */
png_ascii_from_fixed_2(png_struct * png_ptr, char * ascii, png_size_t size,
    png_fixed_point fp)
{
   png_uint_32 num;
   unsigned int ndigits = 0, first = 16/*flag value*/;
   char digits[10];
   unsigned int tmp;
   unsigned int i;
   /* Require space for 10 decimal digits, a decimal point, a minus sign and a
    * trailing \0, 13 characters:
    */
   if (size > 12)
   {
      /* Avoid overflow here on the minimum integer. */
      if (fp < 0)
         *ascii++ = 45, --size, num = -fp;
      else
         num = fp;

      if (num <= 0x80000000U) /* else overflowed */
      {
         ndigits = 0;
         first = 16;  /*flag value*/;

         while (num)
         {
            /* Split the low digit off num: */
            tmp = num/10;
            num -= tmp*10;
            digits[ndigits++] = (char)(48 + num);
            /* Record the first non-zero digit, note that this is a number
             * starting at 1, it's not actually the array index.
             */
            if (first == 16 && num > 0)
               first = ndigits;
            num = tmp;
         }

         if (ndigits > 0)
         {
            while (ndigits > 5) *ascii++ = digits[--ndigits];
            /* The remaining digits are fractional digits, ndigits is '5' or
             * smaller at this point.  It is certainly not zero.  Check for a
             * non-zero fractional digit:
             */
            if (first <= 5)
            {
               *ascii++ = 46; /* decimal point */
               /* ndigits may be <5 for small numbers, output leading zeros
                * then ndigits digits to first:
                */
               i = 5;
               while (ndigits < i) *ascii++ = 48, --i;
               while (ndigits >= first) *ascii++ = digits[--ndigits];
               /* Don't output the trailing zeros! */
            }
         }
         else
            *ascii++ = 48;

         /* And null terminate the string: */
         *ascii = 0;
         return;
      }
   }

   /* Here on buffer too small. */
   //png_error(png_ptr, "ASCII conversion buffer too small");
}
#endif
#   endif /* FIXED_POINT */
#endif /* READ_SCAL */

/* Optionally skip data and then check the CRC.  Depending on whether we
 * are reading a ancillary or critical chunk, and how the program has set
 * things up, we may calculate the CRC on the data and print a message.
 * Returns '1' if there was a CRC error, '0' otherwise.
 */
int /* PRIVATE */
png_crc_finish_2(png_struct * png_ptr, png_uint_32 skip)
{
   png_size_t i;
   png_size_t istop = png_ptr->zbuf_size;

   for (i = (png_size_t)skip; i > istop; i -= istop)
   {
      png_crc_read_2(png_ptr, png_ptr->zbuf, png_ptr->zbuf_size);
   }
   if (i)
   {
      png_crc_read_2(png_ptr, png_ptr->zbuf, i);
   }
   if (png_crc_error_2(png_ptr))
   {
      if (((png_ptr->chunk_name[0] & 0x20) &&                /* Ancillary */
          !(png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN)) ||
          (!(png_ptr->chunk_name[0] & 0x20) &&             /* Critical  */
          (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_USE)))
      {
         //png_chunk_warning(png_ptr, "CRC error");
      }

      else
      {
         //png_chunk_benign_error(png_ptr, "CRC error");
         return (0);
      }

      return (1);
   }

   return (0);
}


/* Grab an unsigned 32-bit integer from a buffer in big-endian format. */
png_uint_32 png_get_uint_32_2(png_const_bytep buf)
{
   png_uint_32 uval =
       ((png_uint_32)(*(buf    )) << 24) +
       ((png_uint_32)(*(buf + 1)) << 16) +
       ((png_uint_32)(*(buf + 2)) <<  8) +
       ((png_uint_32)(*(buf + 3))      ) ;

   return uval;
}
/* Compare the CRC stored in the PNG file with that calculated by libpng from
 * the data it has read thus far.
 */
int /* PRIVATE */
png_crc_error_2(png_struct * png_ptr)
{
   png_byte crc_bytes[4];
   png_uint_32 crc;
   int need_crc = 1;

   if (png_ptr->chunk_name[0] & 0x20)                     /* ancillary */
   {
      if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_MASK) ==
          (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN))
         need_crc = 0;
   }

   else                                                    /* critical */
   {
      if (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE)
         need_crc = 0;
   }

#ifdef PNG_IO_STATE_SUPPORTED
   png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_CRC;
#endif

   /* The chunk CRC must be serialized in a single I/O call. */
   png_read_data_2(png_ptr, crc_bytes, 4);

   if (need_crc)
   {
      crc = png_get_uint_32_2(crc_bytes);
      return ((int)(crc != png_ptr->crc));
   }

   else
      return (0);
}

/* This function is called to verify that a chunk name is valid.
 * This function can't have the "critical chunk check" incorporated
 * into it, since in the future we will need to be able to call user
 * functions to handle unknown critical chunks after we check that
 * the chunk name itself is valid.
 */

#define isnonalpha_2(c) ((c) < 65 || (c) > 122 || ((c) > 90 && (c) < 97))
static PNG_CONST char png_digit_2[16] = {
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   'A', 'B', 'C', 'D', 'E', 'F'
};

#define PNG_MAX_ERROR_TEXT 64
#if defined(PNG_WARNINGS_SUPPORTED) || defined(PNG_ERROR_TEXT_SUPPORTED)
static void /* PRIVATE */
png_format_buffer_2(png_struct * png_ptr, char * buffer, char *
    error_message)
{
   int iout = 0, iin = 0;

   while (iin < 4)
   {
      int c = png_ptr->chunk_name[iin++];
      if (isnonalpha_2(c))
      {
         buffer[iout++] = PNG_LITERAL_LEFT_SQUARE_BRACKET;
         buffer[iout++] = png_digit_2[(c & 0xf0) >> 4];
         buffer[iout++] = png_digit_2[c & 0x0f];
         buffer[iout++] = PNG_LITERAL_RIGHT_SQUARE_BRACKET;
      }

      else
      {
         buffer[iout++] = (png_byte)c;
      }
   }

   if (error_message == NULL)
      buffer[iout] = '\0';

   else
   {
      buffer[iout++] = ':';
      buffer[iout++] = ' ';
      memcpy(buffer + iout, error_message, PNG_MAX_ERROR_TEXT);
      buffer[iout + PNG_MAX_ERROR_TEXT - 1] = '\0';
   }
}
#endif /* PNG_WARNINGS_SUPPORTED || PNG_ERROR_TEXT_SUPPORTED */

#if defined(PNG_READ_SUPPORTED) && defined(PNG_ERROR_TEXT_SUPPORTED)
void png_chunk_error_2(png_struct * png_ptr, char * error_message)
{
   char msg[18 + PNG_MAX_ERROR_TEXT];
   if (png_ptr == NULL) {
      //png_error(png_ptr, error_message);
   }
   else
   {
      //png_format_buffer_2(png_ptr, msg, error_message);
      //png_error(png_ptr, msg);
   }
}
#endif /* PNG_READ_SUPPORTED && PNG_ERROR_TEXT_SUPPORTED */

void /* PRIVATE */
png_check_chunk_name_2(png_struct * png_ptr, png_byte * chunk_name)
{
   //png_debug(1, "in png_check_chunk_name");
   if (isnonalpha_2(chunk_name[0]) || isnonalpha_2(chunk_name[1]) ||
       isnonalpha_2(chunk_name[2]) || isnonalpha_2(chunk_name[3]))
   {
      png_chunk_error_2(png_ptr, "invalid chunk type");
   }
}

png_uint_32 png_get_uint_31_2(png_struct * png_ptr, png_byte * buf)
{
   png_uint_32 uval = png_get_uint_32_2(buf);

   //if (uval > PNG_UINT_31_MAX)
      //png_error(png_ptr, "PNG unsigned integer out of range");

   return (uval);
}
/* Read the chunk header (length + type name).
 * Put the type name into png_ptr->chunk_name, and return the length.
 */
//#include "2440lib.h"
png_uint_32 /* PRIVATE */
png_read_chunk_header_2(png_struct * png_ptr)
{
   png_byte buf[8];
   png_uint_32 length;

#ifdef PNG_IO_STATE_SUPPORTED
   png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_HDR;
#endif

   /* Read the length and the chunk name.
    * This must be performed in a single I/O call.
    */
   png_read_data_2(png_ptr, buf, 8);
   length = png_get_uint_31_2(png_ptr, buf);

   /* Put the chunk name into png_ptr->chunk_name. */
   memcpy(png_ptr->chunk_name, buf + 4, 4);

   Uart_Printf("Reading %s chunk, length = %u",
       png_ptr->chunk_name, length);

   /* Reset the crc and run it over the chunk name. */
   png_reset_crc_2(png_ptr);
   png_calculate_crc_2(png_ptr, png_ptr->chunk_name, 4);

   /* Check to see if chunk name is valid. */
   png_check_chunk_name_2(png_ptr, png_ptr->chunk_name);

#ifdef PNG_IO_STATE_SUPPORTED
   png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_DATA;
#endif

   return length;
}

/* Read the data from whatever input you are using.  The default routine
 * reads from a file pointer.  Note that this routine sometimes gets called
 * with very small lengths, so you should implement some kind of simple
 * buffering if you are using unbuffered reads.  This should never be asked
 * to read more then 64K on a 16 bit machine.
 */
void /* PRIVATE */
png_read_data_2(png_struct * png_ptr, png_byte * data, png_size_t length)
{
   //png_debug1(4, "reading %d bytes", (int)length);

   if (png_ptr->read_data_fn != NULL)
      (*(png_ptr->read_data_fn))(png_ptr, data, length);

   //else
      //png_error(png_ptr, "Call to NULL read function");
}

/* Read data, and (optionally) run it through the CRC. */
void /* PRIVATE */
png_crc_read_2(png_struct * png_ptr, png_byte * buf, png_size_t length)
{
   if (png_ptr == NULL)
      return;

   png_read_data_2(png_ptr, buf, length);
   png_calculate_crc_2(png_ptr, buf, length);
}

#ifdef PNG_READ_EXPAND_SUPPORTED
/* Expands a palette row to an RGB or RGBA row depending
 * upon whether you supply trans and num_trans.
 */
void /* PRIVATE */
png_do_expand_palette_2(png_row_info * row_info, png_byte * row,
   png_color * palette, png_byte * trans_alpha, int num_trans)
{
   int shift, value;
   png_bytep sp, dp;
   png_uint_32 i;
   png_uint_32 row_width=row_info->width;

   //png_debug(1, "in png_do_expand_palette");

   if (row_info->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (row_info->bit_depth < 8)
      {
         switch (row_info->bit_depth)
         {
            case 1:
            {
               sp = row + (png_size_t)((row_width - 1) >> 3);
               dp = row + (png_size_t)row_width - 1;
               shift = 7 - (int)((row_width + 7) & 0x07);
               for (i = 0; i < row_width; i++)
               {
                  if ((*sp >> shift) & 0x01)
                     *dp = 1;

                  else
                     *dp = 0;

                  if (shift == 7)
                  {
                     shift = 0;
                     sp--;
                  }

                  else
                     shift++;

                  dp--;
               }
               break;
            }

            case 2:
            {
               sp = row + (png_size_t)((row_width - 1) >> 2);
               dp = row + (png_size_t)row_width - 1;
               shift = (int)((3 - ((row_width + 3) & 0x03)) << 1);
               for (i = 0; i < row_width; i++)
               {
                  value = (*sp >> shift) & 0x03;
                  *dp = (png_byte)value;
                  if (shift == 6)
                  {
                     shift = 0;
                     sp--;
                  }

                  else
                     shift += 2;

                  dp--;
               }
               break;
            }

            case 4:
            {
               sp = row + (png_size_t)((row_width - 1) >> 1);
               dp = row + (png_size_t)row_width - 1;
               shift = (int)((row_width & 0x01) << 2);
               for (i = 0; i < row_width; i++)
               {
                  value = (*sp >> shift) & 0x0f;
                  *dp = (png_byte)value;
                  if (shift == 4)
                  {
                     shift = 0;
                     sp--;
                  }

                  else
                     shift += 4;

                  dp--;
               }
               break;
            }

            default:
               break;
         }
         row_info->bit_depth = 8;
         row_info->pixel_depth = 8;
         row_info->rowbytes = row_width;
      }

      if (row_info->bit_depth == 8)
      {
         {
            if (trans_alpha != NULL)
            {
               sp = row + (png_size_t)row_width - 1;
               dp = row + (png_size_t)(row_width << 2) - 1;

               for (i = 0; i < row_width; i++)
               {
                  if ((int)(*sp) >= num_trans)
                     *dp-- = 0xff;

                  else
                     *dp-- = trans_alpha[*sp];

                  *dp-- = palette[*sp].blue;
                  *dp-- = palette[*sp].green;
                  *dp-- = palette[*sp].red;
                  sp--;
               }
               row_info->bit_depth = 8;
               row_info->pixel_depth = 32;
               row_info->rowbytes = row_width * 4;
               row_info->color_type = 6;
               row_info->channels = 4;
            }

            else
            {
               sp = row + (png_size_t)row_width - 1;
               dp = row + (png_size_t)(row_width * 3) - 1;

               for (i = 0; i < row_width; i++)
               {
                  *dp-- = palette[*sp].blue;
                  *dp-- = palette[*sp].green;
                  *dp-- = palette[*sp].red;
                  sp--;
               }

               row_info->bit_depth = 8;
               row_info->pixel_depth = 24;
               row_info->rowbytes = row_width * 3;
               row_info->color_type = 2;
               row_info->channels = 3;
            }
         }
      }
   }
}

/* If the bit depth < 8, it is expanded to 8.  Also, if the already
 * expanded transparency value is supplied, an alpha channel is built.
 */
void /* PRIVATE */
png_do_expand_2(png_row_info * row_info, png_byte * row,
    png_color_16 * trans_value)
{
   int shift, value;
   png_bytep sp, dp;
   png_uint_32 i;
   png_uint_32 row_width=row_info->width;
   png_byte red_high;
   png_byte green_high;
   png_byte blue_high;
   png_byte red_low;
   png_byte green_low;
   png_byte blue_low;
   png_byte gray_high;
   png_byte gray_low;
   png_byte red, green, blue;
   png_uint_16 gray;
   //png_debug(1, "in png_do_expand");

   {
      if (row_info->color_type == PNG_COLOR_TYPE_GRAY)
      {
         gray = (png_uint_16)(trans_value ? trans_value->gray : 0);

         if (row_info->bit_depth < 8)
         {
            switch (row_info->bit_depth)
            {
               case 1:
               {
                  gray = (png_uint_16)((gray & 0x01) * 0xff);
                  sp = row + (png_size_t)((row_width - 1) >> 3);
                  dp = row + (png_size_t)row_width - 1;
                  shift = 7 - (int)((row_width + 7) & 0x07);
                  for (i = 0; i < row_width; i++)
                  {
                     if ((*sp >> shift) & 0x01)
                        *dp = 0xff;

                     else
                        *dp = 0;

                     if (shift == 7)
                     {
                        shift = 0;
                        sp--;
                     }

                     else
                        shift++;

                     dp--;
                  }
                  break;
               }

               case 2:
               {
                  gray = (png_uint_16)((gray & 0x03) * 0x55);
                  sp = row + (png_size_t)((row_width - 1) >> 2);
                  dp = row + (png_size_t)row_width - 1;
                  shift = (int)((3 - ((row_width + 3) & 0x03)) << 1);
                  for (i = 0; i < row_width; i++)
                  {
                     value = (*sp >> shift) & 0x03;
                     *dp = (png_byte)(value | (value << 2) | (value << 4) |
                        (value << 6));
                     if (shift == 6)
                     {
                        shift = 0;
                        sp--;
                     }

                     else
                        shift += 2;

                     dp--;
                  }
                  break;
               }

               case 4:
               {
                  gray = (png_uint_16)((gray & 0x0f) * 0x11);
                  sp = row + (png_size_t)((row_width - 1) >> 1);
                  dp = row + (png_size_t)row_width - 1;
                  shift = (int)((1 - ((row_width + 1) & 0x01)) << 2);
                  for (i = 0; i < row_width; i++)
                  {
                     value = (*sp >> shift) & 0x0f;
                     *dp = (png_byte)(value | (value << 4));
                     if (shift == 4)
                     {
                        shift = 0;
                        sp--;
                     }

                     else
                        shift = 4;

                     dp--;
                  }
                  break;
               }

               default:
                  break;
            }

            row_info->bit_depth = 8;
            row_info->pixel_depth = 8;
            row_info->rowbytes = row_width;
         }

         if (trans_value != NULL)
         {
            if (row_info->bit_depth == 8)
            {
               gray = gray & 0xff;
               sp = row + (png_size_t)row_width - 1;
               dp = row + (png_size_t)(row_width << 1) - 1;

               for (i = 0; i < row_width; i++)
               {
                  if (*sp == gray)
                     *dp-- = 0;

                  else
                     *dp-- = 0xff;

                  *dp-- = *sp--;
               }
            }

            else if (row_info->bit_depth == 16)
            {
               gray_high = (png_byte)((gray >> 8) & 0xff);
               gray_low = (png_byte)(gray & 0xff);
               sp = row + row_info->rowbytes - 1;
               dp = row + (row_info->rowbytes << 1) - 1;
               for (i = 0; i < row_width; i++)
               {
                  if (*(sp - 1) == gray_high && *(sp) == gray_low)
                  {
                     *dp-- = 0;
                     *dp-- = 0;
                  }

                  else
                  {
                     *dp-- = 0xff;
                     *dp-- = 0xff;
                  }

                  *dp-- = *sp--;
                  *dp-- = *sp--;
               }
            }

            row_info->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
            row_info->channels = 2;
            row_info->pixel_depth = (png_byte)(row_info->bit_depth << 1);
            row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth,
               row_width);
         }
      }
      else if (row_info->color_type == PNG_COLOR_TYPE_RGB && trans_value)
      {
         if (row_info->bit_depth == 8)
         {
            red = (png_byte)(trans_value->red & 0xff);
            green = (png_byte)(trans_value->green & 0xff);
            blue = (png_byte)(trans_value->blue & 0xff);
            sp = row + (png_size_t)row_info->rowbytes - 1;
            dp = row + (png_size_t)(row_width << 2) - 1;
            for (i = 0; i < row_width; i++)
            {
               if (*(sp - 2) == red && *(sp - 1) == green && *(sp) == blue)
                  *dp-- = 0;

               else
                  *dp-- = 0xff;

               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
            }
         }
         else if (row_info->bit_depth == 16)
         {
            red_high = (png_byte)((trans_value->red >> 8) & 0xff);
            green_high = (png_byte)((trans_value->green >> 8) & 0xff);
            blue_high = (png_byte)((trans_value->blue >> 8) & 0xff);
            red_low = (png_byte)(trans_value->red & 0xff);
            green_low = (png_byte)(trans_value->green & 0xff);
            blue_low = (png_byte)(trans_value->blue & 0xff);
            sp = row + row_info->rowbytes - 1;
            dp = row + (png_size_t)(row_width << 3) - 1;
            for (i = 0; i < row_width; i++)
            {
               if (*(sp - 5) == red_high &&
                   *(sp - 4) == red_low &&
                   *(sp - 3) == green_high &&
                   *(sp - 2) == green_low &&
                   *(sp - 1) == blue_high &&
                   *(sp    ) == blue_low)
               {
                  *dp-- = 0;
                  *dp-- = 0;
               }

               else
               {
                  *dp-- = 0xff;
                  *dp-- = 0xff;
               }

               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
            }
         }
         row_info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;
         row_info->channels = 4;
         row_info->pixel_depth = (png_byte)(row_info->bit_depth << 2);
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }
   }
}
#endif

#if defined(PNG_WRITE_FILLER_SUPPORTED) || \
    defined(PNG_READ_STRIP_ALPHA_SUPPORTED)
/* Remove filler or alpha byte(s) */
void /* PRIVATE */
png_do_strip_filler_2(png_row_info * row_info, png_byte * row, png_uint_32 flags)
{
   //png_debug(1, "in png_do_strip_filler");
   png_bytep sp = row;
   png_bytep dp = row;
   png_uint_32 row_width = row_info->width;
   png_uint_32 i;
   {
      if ((row_info->color_type == PNG_COLOR_TYPE_RGB ||
          (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA &&
          (flags & PNG_FLAG_STRIP_ALPHA))) &&
          row_info->channels == 4)
      {
         if (row_info->bit_depth == 8)
         {
            /* This converts from RGBX or RGBA to RGB */
            if (flags & PNG_FLAG_FILLER_AFTER)
            {
               dp += 3; sp += 4;
               for (i = 1; i < row_width; i++)
               {
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  sp++;
               }
            }

            /* This converts from XRGB or ARGB to RGB */
            else
            {
               for (i = 0; i < row_width; i++)
               {
                  sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
               }
            }
            row_info->pixel_depth = 24;
            row_info->rowbytes = row_width * 3;
         }

         else /* if (row_info->bit_depth == 16) */
         {
            if (flags & PNG_FLAG_FILLER_AFTER)
            {
               /* This converts from RRGGBBXX or RRGGBBAA to RRGGBB */
               sp += 8; dp += 6;
               for (i = 1; i < row_width; i++)
               {
                  /* This could be (although png_memcpy is probably slower):
                  png_memcpy(dp, sp, 6);
                  sp += 8;
                  dp += 6;
                  */

                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  sp += 2;
               }
            }

            else
            {
               /* This converts from XXRRGGBB or AARRGGBB to RRGGBB */
               for (i = 0; i < row_width; i++)
               {
                  /* This could be (although png_memcpy is probably slower):
                  png_memcpy(dp, sp, 6);
                  sp += 8;
                  dp += 6;
                  */

                  sp += 2;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
               }
            }

            row_info->pixel_depth = 48;
            row_info->rowbytes = row_width * 6;
         }
         row_info->channels = 3;
      }

      else if ((row_info->color_type == PNG_COLOR_TYPE_GRAY ||
         (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
         (flags & PNG_FLAG_STRIP_ALPHA))) &&
          row_info->channels == 2)
      {
         if (row_info->bit_depth == 8)
         {
            if (flags & PNG_FLAG_FILLER_AFTER)
            {
               /* This converts from GX or GA to G */
               for (i = 0; i < row_width; i++)
               {
                  *dp++ = *sp++;
                  sp++;
               }
            }

            else
            {
               /* This converts from XG or AG to G */
               for (i = 0; i < row_width; i++)
               {
                  sp++;
                  *dp++ = *sp++;
               }
            }

            row_info->pixel_depth = 8;
            row_info->rowbytes = row_width;
         }

         else /* if (row_info->bit_depth == 16) */
         {
            if (flags & PNG_FLAG_FILLER_AFTER)
            {
               /* This converts from GGXX or GGAA to GG */
               sp += 4; dp += 2;
               for (i = 1; i < row_width; i++)
               {
                  *dp++ = *sp++;
                  *dp++ = *sp++;
                  sp += 2;
               }
            }

            else
            {
               /* This converts from XXGG or AAGG to GG */
               for (i = 0; i < row_width; i++)
               {
                  sp += 2;
                  *dp++ = *sp++;
                  *dp++ = *sp++;
               }
            }

            row_info->pixel_depth = 16;
            row_info->rowbytes = row_width * 2;
         }
         row_info->channels = 1;
      }

      if (flags & PNG_FLAG_STRIP_ALPHA)
        row_info->color_type = (png_byte)(row_info->color_type &
            ~PNG_COLOR_MASK_ALPHA);
   }
}
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
/* Reduce RGB files to grayscale, with or without alpha
 * using the equation given in Poynton's ColorFAQ at
 * <http://www.inforamp.net/~poynton/>  (THIS LINK IS DEAD June 2008)
 * New link:
 * <http://www.poynton.com/notes/colour_and_gamma/>
 * Charles Poynton poynton at poynton.com
 *
 *     Y = 0.212671 * R + 0.715160 * G + 0.072169 * B
 *
 *  We approximate this with
 *
 *     Y = 0.21268 * R    + 0.7151 * G    + 0.07217 * B
 *
 *  which can be expressed with integers as
 *
 *     Y = (6969 * R + 23434 * G + 2365 * B)/32768
 *
 *  The calculation is to be done in a linear colorspace.
 *
 *  Other integer coefficents can be used via png_set_rgb_to_gray().
 */
int /* PRIVATE */
png_do_rgb_to_gray_2(png_struct * png_ptr, png_row_info * row_info, png_byte * row)

{
   png_uint_32 i;
   png_byte * sp;
   png_byte * dp;
   png_uint_16 red, green, blue, gray16, w;
   png_uint_32 row_width;
   int rgb_error = 0;
   png_uint_32 rc = png_ptr->rgb_to_gray_red_coeff;
   png_uint_32 gc = png_ptr->rgb_to_gray_green_coeff;
   png_uint_32 bc = png_ptr->rgb_to_gray_blue_coeff;
   png_uint_16 red_1;
   png_uint_16 green_1;
   png_uint_16 blue_1;
   //png_debug(1, "in png_do_rgb_to_gray");

   if (!(row_info->color_type & PNG_COLOR_MASK_PALETTE) &&
       (row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      rc = png_ptr->rgb_to_gray_red_coeff;
      gc = png_ptr->rgb_to_gray_green_coeff;
      bc = png_ptr->rgb_to_gray_blue_coeff;
      if (row_info->color_type == PNG_COLOR_TYPE_RGB)
      {
         if (row_info->bit_depth == 8)
         {
#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
            if (png_ptr->gamma_from_1 != NULL && png_ptr->gamma_to_1 != NULL)
            {
               sp = row;
               dp = row;

               for (i = 0; i < row_width; i++)
               {
                  red   = png_ptr->gamma_to_1[*(sp++)];
                  green = png_ptr->gamma_to_1[*(sp++)];
                  blue  = png_ptr->gamma_to_1[*(sp++)];

                  if (red != green || red != blue)
                  {
                     rgb_error |= 1;
                     *(dp++) = png_ptr->gamma_from_1[
                         (rc*red + gc*green + bc*blue)>>15];
                  }

                  else
                     *(dp++) = *(sp - 1);
               }
            }
            else
#endif
            {
               sp = row;
               dp = row;
               for (i = 0; i < row_width; i++)
               {
                  red   = *(sp++);
                  green = *(sp++);
                  blue  = *(sp++);

                  if (red != green || red != blue)
                  {
                     rgb_error |= 1;
                     *(dp++) = (png_byte)((rc*red + gc*green + bc*blue)>>15);
                  }

                  else
                     *(dp++) = *(sp - 1);
               }
            }
         }

         else /* RGB bit_depth == 16 */
         {
#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
            if (png_ptr->gamma_16_to_1 != NULL &&
                png_ptr->gamma_16_from_1 != NULL)
            {
               //png_bytep sp = row;
               //png_bytep dp = row;
               for (i = 0; i < row_width; i++)
               {
                  //png_uint_16 red, green, blue, w;

                  red   = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
                  green = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
                  blue  = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;

                  if (red == green && red == blue)
                     w = red;

                  else
                  {
                     red_1   = png_ptr->gamma_16_to_1[(red&0xff)
                         >> png_ptr->gamma_shift][red>>8];
                     green_1 =
                         png_ptr->gamma_16_to_1[(green&0xff) >>
                         png_ptr->gamma_shift][green>>8];
                     blue_1  = png_ptr->gamma_16_to_1[(blue&0xff)
                         >> png_ptr->gamma_shift][blue>>8];
                     gray16  = (png_uint_16)((rc*red_1 + gc*green_1
                         + bc*blue_1)>>15);
                     w = png_ptr->gamma_16_from_1[(gray16&0xff) >>
                         png_ptr->gamma_shift][gray16 >> 8];
                     rgb_error |= 1;
                  }

                  *(dp++) = (png_byte)((w>>8) & 0xff);
                  *(dp++) = (png_byte)(w & 0xff);
               }
            }
            else
#endif
            {
               sp = row;
               dp = row;
               for (i = 0; i < row_width; i++)
               {
                  //png_uint_16 red, green, blue, gray16;

                  red   = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
                  green = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
                  blue  = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;

                  if (red != green || red != blue)
                     rgb_error |= 1;

                  gray16  = (png_uint_16)((rc*red + gc*green + bc*blue)>>15);
                  *(dp++) = (png_byte)((gray16>>8) & 0xff);
                  *(dp++) = (png_byte)(gray16 & 0xff);
               }
            }
         }
      }
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
            if (png_ptr->gamma_from_1 != NULL && png_ptr->gamma_to_1 != NULL)
            {
               sp = row;
               dp = row;
               for (i = 0; i < row_width; i++)
               {
                  red   = png_ptr->gamma_to_1[*(sp++)];
                  green = png_ptr->gamma_to_1[*(sp++)];
                  blue  = png_ptr->gamma_to_1[*(sp++)];

                  if (red != green || red != blue)
                     rgb_error |= 1;

                  *(dp++) =  png_ptr->gamma_from_1
                      [(rc*red + gc*green + bc*blue)>>15];

                  *(dp++) = *(sp++);  /* alpha */
               }
            }
            else
#endif
            {
               sp = row;
               dp = row;
               for (i = 0; i < row_width; i++)
               {
                  red   = *(sp++);
                  green = *(sp++);
                  blue  = *(sp++);
                  if (red != green || red != blue)
                     rgb_error |= 1;

                  *(dp++) =  (png_byte)((rc*red + gc*green + bc*blue)>>15);
                  *(dp++) = *(sp++);  /* alpha */
               }
            }
         }
         else /* RGBA bit_depth == 16 */
         {
#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
            if (png_ptr->gamma_16_to_1 != NULL &&
                png_ptr->gamma_16_from_1 != NULL)
            {
               sp = row;
               dp = row;
               for (i = 0; i < row_width; i++)
               {
                  red   = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
                  green = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
                  blue  = (png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;

                  if (red == green && red == blue)
                     w = red;

                  else
                  {
                     red_1   = png_ptr->gamma_16_to_1[(red&0xff) >>
                         png_ptr->gamma_shift][red>>8];

                     green_1 =
                         png_ptr->gamma_16_to_1[(green&0xff) >>
                         png_ptr->gamma_shift][green>>8];

                     blue_1  = png_ptr->gamma_16_to_1[(blue&0xff) >>
                         png_ptr->gamma_shift][blue>>8];

                     gray16  = (png_uint_16)((rc * red_1
                         + gc * green_1 + bc * blue_1)>>15);

                     w = png_ptr->gamma_16_from_1[(gray16&0xff) >>
                         png_ptr->gamma_shift][gray16 >> 8];

                     rgb_error |= 1;
                  }

                  *(dp++) = (png_byte)((w>>8) & 0xff);
                  *(dp++) = (png_byte)(w & 0xff);
                  *(dp++) = *(sp++);  /* alpha */
                  *(dp++) = *(sp++);
               }
            }
            else
#endif
            {
               for (i = 0; i < row_width; i++)
               {
                  
                  red   = (png_uint_16)((*(sp)<<8) | *(sp + 1)); sp += 2;
                  green = (png_uint_16)((*(sp)<<8) | *(sp + 1)); sp += 2;
                  blue  = (png_uint_16)((*(sp)<<8) | *(sp + 1)); sp += 2;

                  if (red != green || red != blue)
                     rgb_error |= 1;

                  gray16  = (png_uint_16)((rc*red + gc*green + bc*blue)>>15);
                  *(dp++) = (png_byte)((gray16>>8) & 0xff);
                  *(dp++) = (png_byte)(gray16 & 0xff);
                  *(dp++) = *(sp++);  /* alpha */
                  *(dp++) = *(sp++);
               }
            }
         }
      }
      row_info->channels -= 2;
      row_info->color_type = (png_byte)(row_info->color_type &
          ~PNG_COLOR_MASK_COLOR);
      row_info->pixel_depth = (png_byte)(row_info->channels *
          row_info->bit_depth);
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
   }
   return rgb_error;
}
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
/* Expand grayscale files to RGB, with or without alpha */
void /* PRIVATE */
png_do_gray_to_rgb_2(png_row_info * row_info, png_byte * row)
{
   png_uint_32 i;
   png_uint_32 row_width = row_info->width;
   png_byte * sp;
   png_byte * dp;
   //png_debug(1, "in png_do_gray_to_rgb");

   if (row_info->bit_depth >= 8 &&
       !(row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      if (row_info->color_type == PNG_COLOR_TYPE_GRAY)
      {
         if (row_info->bit_depth == 8)
         {
            /* This changes G to RGB */
            sp = row + (png_size_t)row_width - 1;
            dp = sp  + (png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *sp;
               *(dp--) = *sp;
               *(dp--) = *(sp--);
            }
         }

         else
         {
            /* This changes GG to RRGGBB */
            sp = row + (png_size_t)row_width * 2 - 1;
            dp = sp  + (png_size_t)row_width * 4;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *(sp--);
               *(dp--) = *(sp--);
            }
         }
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This changes GA to RGBA */
            sp = row + (png_size_t)row_width * 2 - 1;
            dp = sp  + (png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *(sp--);
               *(dp--) = *sp;
               *(dp--) = *sp;
               *(dp--) = *(sp--);
            }
         }

         else
         {
            /* This changes GGAA to RRGGBBAA */
            sp = row + (png_size_t)row_width * 4 - 1;
            dp = sp  + (png_size_t)row_width * 4;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *(sp--);
               *(dp--) = *(sp--);
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *(sp--);
               *(dp--) = *(sp--);
            }
         }
      }
      row_info->channels += (png_byte)2;
      row_info->color_type |= PNG_COLOR_MASK_COLOR;
      row_info->pixel_depth = (png_byte)(row_info->channels *
          row_info->bit_depth);
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
   }
}
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
    )
{
   png_bytep sp, dp;
   png_uint_32 i;
   png_uint_32 row_width = row_info->width;
   int shift;
   png_uint_16 v, g, w, a, x, p;
   png_uint_16 r;
   png_uint_16 b;
   //png_debug(1, "in png_do_background");

   if (background != NULL &&
      (!(row_info->color_type & PNG_COLOR_MASK_ALPHA) ||
      (row_info->color_type != PNG_COLOR_TYPE_PALETTE && trans_color)))
   {
      switch (row_info->color_type)
      {
         case PNG_COLOR_TYPE_GRAY:
         {
            switch (row_info->bit_depth)
            {
               case 1:
               {
                  sp = row;
                  shift = 7;
                  for (i = 0; i < row_width; i++)
                  {
                     if ((png_uint_16)((*sp >> shift) & 0x01)
                        == trans_color->gray)
                     {
                        *sp &= (png_byte)((0x7f7f >> (7 - shift)) & 0xff);
                        *sp |= (png_byte)(background->gray << shift);
                     }

                     if (!shift)
                     {
                        shift = 7;
                        sp++;
                     }

                     else
                        shift--;
                  }
                  break;
               }

               case 2:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_table != NULL)
                  {
                     sp = row;
                     shift = 6;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((png_uint_16)((*sp >> shift) & 0x03)
                            == trans_color->gray)
                        {
                           *sp &= (png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                           *sp |= (png_byte)(background->gray << shift);
                        }

                        else
                        {
                           p = (png_byte)((*sp >> shift) & 0x03);
                           g = (png_byte)((gamma_table [p | (p << 2) |
                               (p << 4) | (p << 6)] >> 6) & 0x03);
                           *sp &= (png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                           *sp |= (png_byte)(g << shift);
                        }

                        if (!shift)
                        {
                           shift = 6;
                           sp++;
                        }

                        else
                           shift -= 2;
                     }
                  }

                  else
#endif
                  {
                     sp = row;
                     shift = 6;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((png_uint_16)((*sp >> shift) & 0x03)
                            == trans_color->gray)
                        {
                           *sp &= (png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                           *sp |= (png_byte)(background->gray << shift);
                        }

                        if (!shift)
                        {
                           shift = 6;
                           sp++;
                        }

                        else
                           shift -= 2;
                     }
                  }
                  break;
               }

               case 4:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_table != NULL)
                  {
                     sp = row;
                     shift = 4;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((png_uint_16)((*sp >> shift) & 0x0f)
                            == trans_color->gray)
                        {
                           *sp &= (png_byte)((0xf0f >> (4 - shift)) & 0xff);
                           *sp |= (png_byte)(background->gray << shift);
                        }

                        else
                        {
                           p = (png_byte)((*sp >> shift) & 0x0f);
                           g = (png_byte)((gamma_table[p |
                               (p << 4)] >> 4) & 0x0f);
                           *sp &= (png_byte)((0xf0f >> (4 - shift)) & 0xff);
                           *sp |= (png_byte)(g << shift);
                        }

                        if (!shift)
                        {
                           shift = 4;
                           sp++;
                        }

                        else
                           shift -= 4;
                     }
                  }

                  else
#endif
                  {
                     sp = row;
                     shift = 4;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((png_uint_16)((*sp >> shift) & 0x0f)
                            == trans_color->gray)
                        {
                           *sp &= (png_byte)((0xf0f >> (4 - shift)) & 0xff);
                           *sp |= (png_byte)(background->gray << shift);
                        }

                        if (!shift)
                        {
                           shift = 4;
                           sp++;
                        }

                        else
                           shift -= 4;
                     }
                  }
                  break;
               }

               case 8:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_table != NULL)
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp++)
                     {
                        if (*sp == trans_color->gray)
                           *sp = (png_byte)background->gray;

                        else
                           *sp = gamma_table[*sp];
                     }
                  }
                  else
#endif
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp++)
                     {
                        if (*sp == trans_color->gray)
                           *sp = (png_byte)background->gray;
                     }
                  }
                  break;
               }

               case 16:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_16 != NULL)
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp += 2)
                     {
                        v = (png_uint_16)(((*sp) << 8) + *(sp + 1));

                        if (v == trans_color->gray)
                        {
                           /* Background is already in screen gamma */
                           *sp = (png_byte)((background->gray >> 8) & 0xff);
                           *(sp + 1) = (png_byte)(background->gray & 0xff);
                        }

                        else
                        {
                           v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                           *sp = (png_byte)((v >> 8) & 0xff);
                           *(sp + 1) = (png_byte)(v & 0xff);
                        }
                     }
                  }
                  else
#endif
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp += 2)
                     {
                        v = (png_uint_16)(((*sp) << 8) + *(sp + 1));

                        if (v == trans_color->gray)
                        {
                           *sp = (png_byte)((background->gray >> 8) & 0xff);
                           *(sp + 1) = (png_byte)(background->gray & 0xff);
                        }
                     }
                  }
                  break;
               }

               default:
                  break;
            }
            break;
         }

         case PNG_COLOR_TYPE_RGB:
         {
            if (row_info->bit_depth == 8)
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_table != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 3)
                  {
                     if (*sp == trans_color->red &&
                         *(sp + 1) == trans_color->green &&
                         *(sp + 2) == trans_color->blue)
                     {
                        *sp = (png_byte)background->red;
                        *(sp + 1) = (png_byte)background->green;
                        *(sp + 2) = (png_byte)background->blue;
                     }

                     else
                     {
                        *sp = gamma_table[*sp];
                        *(sp + 1) = gamma_table[*(sp + 1)];
                        *(sp + 2) = gamma_table[*(sp + 2)];
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 3)
                  {
                     if (*sp == trans_color->red &&
                         *(sp + 1) == trans_color->green &&
                         *(sp + 2) == trans_color->blue)
                     {
                        *sp = (png_byte)background->red;
                        *(sp + 1) = (png_byte)background->green;
                        *(sp + 2) = (png_byte)background->blue;
                     }
                  }
               }
            }
            else /* if (row_info->bit_depth == 16) */
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_16 != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 6)
                  {
                     r = (png_uint_16)(((*sp) << 8) + *(sp + 1));

                     g = (png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     b = (png_uint_16)(((*(sp + 4)) << 8)
                         + *(sp + 5));

                     if (r == trans_color->red && g == trans_color->green &&
                         b == trans_color->blue)
                     {
                        /* Background is already in screen gamma */
                        *sp = (png_byte)((background->red >> 8) & 0xff);
                        *(sp + 1) = (png_byte)(background->red & 0xff);
                        *(sp + 2) = (png_byte)((background->green >> 8) & 0xff);
                        *(sp + 3) = (png_byte)(background->green & 0xff);
                        *(sp + 4) = (png_byte)((background->blue >> 8) & 0xff);
                        *(sp + 5) = (png_byte)(background->blue & 0xff);
                     }

                     else
                     {
                        v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                        *sp = (png_byte)((v >> 8) & 0xff);
                        *(sp + 1) = (png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 3) >> gamma_shift][*(sp + 2)];
                        *(sp + 2) = (png_byte)((v >> 8) & 0xff);
                        *(sp + 3) = (png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 5) >> gamma_shift][*(sp + 4)];
                        *(sp + 4) = (png_byte)((v >> 8) & 0xff);
                        *(sp + 5) = (png_byte)(v & 0xff);
                     }
                  }
               }

               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 6)
                  {
                     r = (png_uint_16)(((*sp) << 8) + *(sp + 1));

                     g = (png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     b = (png_uint_16)(((*(sp + 4)) << 8)
                         + *(sp + 5));

                     if (r == trans_color->red && g == trans_color->green &&
                         b == trans_color->blue)
                     {
                        *sp = (png_byte)((background->red >> 8) & 0xff);
                        *(sp + 1) = (png_byte)(background->red & 0xff);
                        *(sp + 2) = (png_byte)((background->green >> 8) & 0xff);
                        *(sp + 3) = (png_byte)(background->green & 0xff);
                        *(sp + 4) = (png_byte)((background->blue >> 8) & 0xff);
                        *(sp + 5) = (png_byte)(background->blue & 0xff);
                     }
                  }
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_GRAY_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_to_1 != NULL && gamma_from_1 != NULL &&
                   gamma_table != NULL)
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 2, dp++)
                  {
                     a = *(sp + 1);

                     if (a == 0xff)
                        *dp = gamma_table[*sp];

                     else if (a == 0)
                     {
                        /* Background is already in screen gamma */
                        *dp = (png_byte)background->gray;
                     }

                     else
                     {
                        v = gamma_to_1[*sp];
                        png_composite(w, v, a, background_1->gray);
                        *dp = gamma_from_1[w];
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 2, dp++)
                  {
                     a = *(sp + 1);

                     if (a == 0xff)
                        *dp = *sp;

#ifdef PNG_READ_GAMMA_SUPPORTED
                     else if (a == 0)
                        *dp = (png_byte)background->gray;

                     else
                        png_composite(*dp, *sp, a, background_1->gray);

#else
                     *dp = (png_byte)background->gray;
#endif
                  }
               }
            }
            else /* if (png_ptr->bit_depth == 16) */
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_16 != NULL && gamma_16_from_1 != NULL &&
                   gamma_16_to_1 != NULL)
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 4, dp += 2)
                  {
                     a = (png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     if (a == (png_uint_16)0xffff)
                     {
                        v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                        *dp = (png_byte)((v >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(v & 0xff);
                     }

#ifdef PNG_READ_GAMMA_SUPPORTED
                     else if (a == 0)
#else
                     else
#endif
                     {
                        /* Background is already in screen gamma */
                        *dp = (png_byte)((background->gray >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(background->gray & 0xff);
                     }

#ifdef PNG_READ_GAMMA_SUPPORTED
                     else
                     {
                        g = gamma_16_to_1[*(sp + 1) >> gamma_shift][*sp];
                        png_composite_16(v, g, a, background_1->gray);
                        w = gamma_16_from_1[(v&0xff) >> gamma_shift][v >> 8];
                        *dp = (png_byte)((w >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(w & 0xff);
                     }
#endif
                  }
               }
               else
#endif
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 4, dp += 2)
                  {
                     a = (png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     if (a == (png_uint_16)0xffff)
                        memcpy(dp, sp, 2);

#ifdef PNG_READ_GAMMA_SUPPORTED
                     else if (a == 0)
#else
                     else
#endif
                     {
                        *dp = (png_byte)((background->gray >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(background->gray & 0xff);
                     }

#ifdef PNG_READ_GAMMA_SUPPORTED
                     else
                     {
                        g = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                        png_composite_16(v, g, a, background_1->gray);
                        *dp = (png_byte)((v >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(v & 0xff);
                     }
#endif
                  }
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_RGB_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_to_1 != NULL && gamma_from_1 != NULL &&
                   gamma_table != NULL)
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 4, dp += 3)
                  {
                     a = *(sp + 3);

                     if (a == 0xff)
                     {
                        *dp = gamma_table[*sp];
                        *(dp + 1) = gamma_table[*(sp + 1)];
                        *(dp + 2) = gamma_table[*(sp + 2)];
                     }

                     else if (a == 0)
                     {
                        /* Background is already in screen gamma */
                        *dp = (png_byte)background->red;
                        *(dp + 1) = (png_byte)background->green;
                        *(dp + 2) = (png_byte)background->blue;
                     }

                     else
                     {
                        v = gamma_to_1[*sp];
                        png_composite(w, v, a, background_1->red);
                        *dp = gamma_from_1[w];

                        v = gamma_to_1[*(sp + 1)];
                        png_composite(w, v, a, background_1->green);
                        *(dp + 1) = gamma_from_1[w];

                        v = gamma_to_1[*(sp + 2)];
                        png_composite(w, v, a, background_1->blue);
                        *(dp + 2) = gamma_from_1[w];
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 4, dp += 3)
                  {
                     a = *(sp + 3);

                     if (a == 0xff)
                     {
                        *dp = *sp;
                        *(dp + 1) = *(sp + 1);
                        *(dp + 2) = *(sp + 2);
                     }

                     else if (a == 0)
                     {
                        *dp = (png_byte)background->red;
                        *(dp + 1) = (png_byte)background->green;
                        *(dp + 2) = (png_byte)background->blue;
                     }

                     else
                     {
                        png_composite(*dp, *sp, a, background->red);

                        png_composite(*(dp + 1), *(sp + 1), a,
                            background->green);

                        png_composite(*(dp + 2), *(sp + 2), a,
                            background->blue);
                     }
                  }
               }
            }
            else /* if (row_info->bit_depth == 16) */
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_16 != NULL && gamma_16_from_1 != NULL &&
                   gamma_16_to_1 != NULL)
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 8, dp += 6)
                  {
                     a = (png_uint_16)(((png_uint_16)(*(sp + 6))
                         << 8) + (png_uint_16)(*(sp + 7)));

                     if (a == (png_uint_16)0xffff)
                     {
                        v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                        *dp = (png_byte)((v >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 3) >> gamma_shift][*(sp + 2)];
                        *(dp + 2) = (png_byte)((v >> 8) & 0xff);
                        *(dp + 3) = (png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 5) >> gamma_shift][*(sp + 4)];
                        *(dp + 4) = (png_byte)((v >> 8) & 0xff);
                        *(dp + 5) = (png_byte)(v & 0xff);
                     }

                     else if (a == 0)
                     {
                        /* Background is already in screen gamma */
                        *dp = (png_byte)((background->red >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(background->red & 0xff);
                        *(dp + 2) = (png_byte)((background->green >> 8) & 0xff);
                        *(dp + 3) = (png_byte)(background->green & 0xff);
                        *(dp + 4) = (png_byte)((background->blue >> 8) & 0xff);
                        *(dp + 5) = (png_byte)(background->blue & 0xff);
                     }

                     else
                     {
                        v = gamma_16_to_1[*(sp + 1) >> gamma_shift][*sp];
                        png_composite_16(w, v, a, background_1->red);

                        x = gamma_16_from_1[((w&0xff) >> gamma_shift)][w >> 8];
                        *dp = (png_byte)((x >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(x & 0xff);

                        v = gamma_16_to_1[*(sp + 3) >> gamma_shift][*(sp + 2)];
                        png_composite_16(w, v, a, background_1->green);

                        x = gamma_16_from_1[((w&0xff) >> gamma_shift)][w >> 8];
                        *(dp + 2) = (png_byte)((x >> 8) & 0xff);
                        *(dp + 3) = (png_byte)(x & 0xff);

                        v = gamma_16_to_1[*(sp + 5) >> gamma_shift][*(sp + 4)];
                        png_composite_16(w, v, a, background_1->blue);

                        x = gamma_16_from_1[(w & 0xff) >> gamma_shift][w >> 8];
                        *(dp + 4) = (png_byte)((x >> 8) & 0xff);
                        *(dp + 5) = (png_byte)(x & 0xff);
                     }
                  }
               }

               else
#endif
               {
                  sp = row;
                  dp = row;
                  for (i = 0; i < row_width; i++, sp += 8, dp += 6)
                  {
                     a = (png_uint_16)(((png_uint_16)(*(sp + 6))
                         << 8) + (png_uint_16)(*(sp + 7)));

                     if (a == (png_uint_16)0xffff)
                     {
                        memcpy(dp, sp, 6);
                     }

                     else if (a == 0)
                     {
                        *dp = (png_byte)((background->red >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(background->red & 0xff);
                        *(dp + 2) = (png_byte)((background->green >> 8) & 0xff);
                        *(dp + 3) = (png_byte)(background->green & 0xff);
                        *(dp + 4) = (png_byte)((background->blue >> 8) & 0xff);
                        *(dp + 5) = (png_byte)(background->blue & 0xff);
                     }

                     else
                     {
                        r = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                        g = (png_uint_16)(((*(sp + 2)) << 8)
                            + *(sp + 3));
                        b = (png_uint_16)(((*(sp + 4)) << 8)
                            + *(sp + 5));

                        png_composite_16(v, r, a, background->red);
                        *dp = (png_byte)((v >> 8) & 0xff);
                        *(dp + 1) = (png_byte)(v & 0xff);

                        png_composite_16(v, g, a, background->green);
                        *(dp + 2) = (png_byte)((v >> 8) & 0xff);
                        *(dp + 3) = (png_byte)(v & 0xff);

                        png_composite_16(v, b, a, background->blue);
                        *(dp + 4) = (png_byte)((v >> 8) & 0xff);
                        *(dp + 5) = (png_byte)(v & 0xff);
                     }
                  }
               }
            }
            break;
         }

         default:
            break;
      }

      if (row_info->color_type & PNG_COLOR_MASK_ALPHA)
      {
         row_info->color_type = (png_byte)(row_info->color_type &
             ~PNG_COLOR_MASK_ALPHA);
         row_info->channels--;
         row_info->pixel_depth = (png_byte)(row_info->channels *
             row_info->bit_depth);
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }
   }
}
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
    int gamma_shift)
{
   png_bytep sp;
   png_uint_32 i;
   png_uint_32 row_width=row_info->width;
   png_uint_16 v;
   int a;
   int b;
   int c;
   int d;
   int msb, lsb;
   if (((row_info->bit_depth <= 8 && gamma_table != NULL) ||
       (row_info->bit_depth == 16 && gamma_16_table != NULL)))
   {
      switch (row_info->color_type)
      {
         case PNG_COLOR_TYPE_RGB:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
               }
            }

            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_RGB_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;

                  *sp = gamma_table[*sp];
                  sp++;

                  *sp = gamma_table[*sp];
                  sp++;

                  sp++;
               }
            }

            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 4;
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_GRAY_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp += 2;
               }
            }

            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 4;
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_GRAY:
         {
            if (row_info->bit_depth == 2)
            {
               sp = row;
               for (i = 0; i < row_width; i += 4)
               {
                  a = *sp & 0xc0;
                  b = *sp & 0x30;
                  c = *sp & 0x0c;
                  d = *sp & 0x03;

                  *sp = (png_byte)(
                      ((((int)gamma_table[a|(a>>2)|(a>>4)|(a>>6)])   ) & 0xc0)|
                      ((((int)gamma_table[(b<<2)|b|(b>>2)|(b>>4)])>>2) & 0x30)|
                      ((((int)gamma_table[(c<<4)|(c<<2)|c|(c>>2)])>>4) & 0x0c)|
                      ((((int)gamma_table[(d<<6)|(d<<4)|(d<<2)|d])>>6) ));
                  sp++;
               }
            }

            if (row_info->bit_depth == 4)
            {
               sp = row;
               for (i = 0; i < row_width; i += 2)
               {
                  msb = *sp & 0xf0;
                  lsb = *sp & 0x0f;

                  *sp = (png_byte)((((int)gamma_table[msb | (msb >> 4)]) & 0xf0)
                      | (((int)gamma_table[(lsb << 4) | lsb]) >> 4));
                  sp++;
               }
            }

            else if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;
               }
            }

            else if (row_info->bit_depth == 16)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
               }
            }
            break;
         }

         default:
            break;
      }
   }
}
#endif

#ifdef PNG_READ_16_TO_8_SUPPORTED
/* Chop rows of bit depth 16 down to 8 */
void /* PRIVATE */
png_do_chop_2(png_row_info * row_info, png_byte * row)
{
   //png_debug(1, "in png_do_chop");
	png_bytep sp;
    png_bytep dp;
    png_uint_32 i;
    png_uint_32 istop;
   if (row_info->bit_depth == 16)
   {
      	sp = row;
    	dp = row;
		istop = row_info->width * row_info->channels;
      for (i = 0; i<istop; i++, sp += 2, dp++)
      {
#ifdef PNG_READ_16_TO_8_ACCURATE_SCALE_SUPPORTED
      /* This does a more accurate scaling of the 16-bit color
       * value, rather than a simple low-byte truncation.
       *
       * What the ideal calculation should be:
       *   *dp = (((((png_uint_32)(*sp) << 8) |
       *          (png_uint_32)(*(sp + 1))) * 255 + 127)
       *          / (png_uint_32)65535L;
       *
       * GRR: no, I think this is what it really should be:
       *   *dp = (((((png_uint_32)(*sp) << 8) |
       *           (png_uint_32)(*(sp + 1))) + 128L)
       *           / (png_uint_32)257L;
       *
       * GRR: here's the exact calculation with shifts:
       *   temp = (((png_uint_32)(*sp) << 8) |
       *           (png_uint_32)(*(sp + 1))) + 128L;
       *   *dp = (temp - (temp >> 8)) >> 8;
       *
       * Approximate calculation with shift/add instead of multiply/divide:
       *   *dp = ((((png_uint_32)(*sp) << 8) |
       *          (png_uint_32)((int)(*(sp + 1)) - *sp)) + 128) >> 8;
       *
       * What we actually do to avoid extra shifting and conversion:
       */

         *dp = *sp + ((((int)(*(sp + 1)) - *sp) > 128) ? 1 : 0);
#else
       /* Simply discard the low order byte */
         *dp = *sp;
#endif
      }
      row_info->bit_depth = 8;
      row_info->pixel_depth = (png_byte)(8 * row_info->channels);
      row_info->rowbytes = row_info->width * row_info->channels;
   }
}
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
void /* PRIVATE */
png_do_quantize_2(png_row_info * row_info, png_byte * row,
    png_byte * palette_lookup, png_byte * quantize_lookup)
{
   png_bytep sp, dp;
   png_uint_32 i;
   png_uint_32 row_width=row_info->width;
   int r, g, b, p;
   //png_debug(1, "in png_do_quantize");

   if (row_info->bit_depth == 8)
   {
      if (row_info->color_type == PNG_COLOR_TYPE_RGB && palette_lookup)
      {
         
         sp = row;
         dp = row;
         for (i = 0; i < row_width; i++)
         {
            r = *sp++;
            g = *sp++;
            b = *sp++;

            /* This looks real messy, but the compiler will reduce
             * it down to a reasonable formula.  For example, with
             * 5 bits per color, we get:
             * p = (((r >> 3) & 0x1f) << 10) |
             *    (((g >> 3) & 0x1f) << 5) |
             *    ((b >> 3) & 0x1f);
             */
            p = (((r >> (8 - PNG_QUANTIZE_RED_BITS)) &
                ((1 << PNG_QUANTIZE_RED_BITS) - 1)) <<
                (PNG_QUANTIZE_GREEN_BITS + PNG_QUANTIZE_BLUE_BITS)) |
                (((g >> (8 - PNG_QUANTIZE_GREEN_BITS)) &
                ((1 << PNG_QUANTIZE_GREEN_BITS) - 1)) <<
                (PNG_QUANTIZE_BLUE_BITS)) |
                ((b >> (8 - PNG_QUANTIZE_BLUE_BITS)) &
                ((1 << PNG_QUANTIZE_BLUE_BITS) - 1));

            *dp++ = palette_lookup[p];
         }

         row_info->color_type = PNG_COLOR_TYPE_PALETTE;
         row_info->channels = 1;
         row_info->pixel_depth = row_info->bit_depth;
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA &&
         palette_lookup != NULL)
      {
         sp = row;
         dp = row;
         for (i = 0; i < row_width; i++)
         {
            r = *sp++;
            g = *sp++;
            b = *sp++;
            sp++;

            p = (((r >> (8 - PNG_QUANTIZE_RED_BITS)) &
                ((1 << PNG_QUANTIZE_RED_BITS) - 1)) <<
                (PNG_QUANTIZE_GREEN_BITS + PNG_QUANTIZE_BLUE_BITS)) |
                (((g >> (8 - PNG_QUANTIZE_GREEN_BITS)) &
                ((1 << PNG_QUANTIZE_GREEN_BITS) - 1)) <<
                (PNG_QUANTIZE_BLUE_BITS)) |
                ((b >> (8 - PNG_QUANTIZE_BLUE_BITS)) &
                ((1 << PNG_QUANTIZE_BLUE_BITS) - 1));

            *dp++ = palette_lookup[p];
         }

         row_info->color_type = PNG_COLOR_TYPE_PALETTE;
         row_info->channels = 1;
         row_info->pixel_depth = row_info->bit_depth;
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_PALETTE &&
         quantize_lookup)
      {
         sp = row;

         for (i = 0; i < row_width; i++, sp++)
         {
            *sp = quantize_lookup[*sp];
         }
      }
   }
}
#endif /* PNG_READ_QUANTIZE_SUPPORTED */

void /* PRIVATE */
png_do_invert_2(png_row_info * row_info, png_byte * row)
{
   //png_debug(1, "in png_do_invert");
   png_bytep rp;
   png_size_t i;
   png_size_t istop;
  /* This test removed from libpng version 1.0.13 and 1.2.0:
   *   if (row_info->bit_depth == 1 &&
   */
   if (row_info->color_type == PNG_COLOR_TYPE_GRAY)
   {
      rp = row;
      istop = row_info->rowbytes;

      for (i = 0; i < istop; i++)
      {
         *rp = (png_byte)(~(*rp));
         rp++;
      }
   }

   else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
      row_info->bit_depth == 8)
   {
      rp = row;
      istop = row_info->rowbytes;

      for (i = 0; i < istop; i += 2)
      {
         *rp = (png_byte)(~(*rp));
         rp += 2;
      }
   }

#ifdef PNG_16BIT_SUPPORTED
   else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
      row_info->bit_depth == 16)
   {
      rp = row;
      istop = row_info->rowbytes;

      for (i = 0; i < istop; i += 4)
      {
         *rp = (png_byte)(~(*rp));
         *(rp + 1) = (png_byte)(~(*(rp + 1)));
         rp += 4;
      }
   }
#endif
}

#ifdef PNG_READ_SHIFT_SUPPORTED
/* Reverse the effects of png_do_shift.  This routine merely shifts the
 * pixels back to their significant bits values.  Thus, if you have
 * a row of bit depth 8, but only 5 are significant, this will shift
 * the values back to 0 through 31.
 */
void /* PRIVATE */
png_do_unshift_2(png_row_info * row_info, png_byte * row,
    png_color_8 * sig_bits)
{
   //png_debug(1, "in png_do_unshift");
   int shift[4];
   int channels = 0;
   int c;
   png_uint_16 value = 0;
   png_uint_32 row_width = row_info->width;
   png_bytep bp;
   png_size_t i;
   png_size_t istop = row_info->rowbytes;
   png_byte mask;
   if (
       row_info->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      channels = 0;
      value = 0;
      row_width = row_info->width;
      if (row_info->color_type & PNG_COLOR_MASK_COLOR)
      {
         shift[channels++] = row_info->bit_depth - sig_bits->red;
         shift[channels++] = row_info->bit_depth - sig_bits->green;
         shift[channels++] = row_info->bit_depth - sig_bits->blue;
      }

      else
      {
         shift[channels++] = row_info->bit_depth - sig_bits->gray;
      }

      if (row_info->color_type & PNG_COLOR_MASK_ALPHA)
      {
         shift[channels++] = row_info->bit_depth - sig_bits->alpha;
      }

      for (c = 0; c < channels; c++)
      {
         if (shift[c] <= 0)
            shift[c] = 0;

         else
            value = 1;
      }

      if (!value)
         return;

      switch (row_info->bit_depth)
      {
         default:
            break; 

         case 2:
         {
            istop = row_info->rowbytes;

            for (bp = row, i = 0; i < istop; i++)
            {
               *bp >>= 1;
               *bp++ &= 0x55;
            }
            break;
         }

         case 4:
         {
            bp = row;
            istop = row_info->rowbytes;
            mask = (png_byte)((((int)0xf0 >> shift[0]) & (int)0xf0) |
                (png_byte)((int)0xf >> shift[0]));

            for (i = 0; i < istop; i++)
            {
               *bp >>= shift[0];
               *bp++ &= mask;
            }
            break;
         }

         case 8:
         {
            bp = row;
            istop = row_width * channels;

            for (i = 0; i < istop; i++)
            {
               *bp++ >>= shift[i%channels];
            }
            break;
         }

#ifdef PNG_READ_16BIT_SUPPORTED
         case 16:
         {
            bp = row;
            istop = channels * row_width;

            for (i = 0; i < istop; i++)
            {
               value = (png_uint_16)((*bp << 8) + *(bp + 1));
               value >>= shift[i%channels];
               *bp++ = (png_byte)(value >> 8);
               *bp++ = (png_byte)(value & 0xff);
            }
            break;
         }
#endif
      }
   }
}
#endif

#ifdef PNG_READ_PACK_SUPPORTED
/* Unpack pixels of 1, 2, or 4 bits per pixel into 1 byte per pixel,
 * without changing the actual values.  Thus, if you had a row with
 * a bit depth of 1, you would end up with bytes that only contained
 * the numbers 0 or 1.  If you would rather they contain 0 and 255, use
 * png_do_shift() after this.
 */
void /* PRIVATE */
png_do_unpack_2(png_row_info * row_info, png_byte * row)
{
   //png_debug(1, "in png_do_unpack");
   png_bytep sp;
   png_bytep dp;
   png_uint_32 shift;
   png_uint_32 i;
   png_uint_32 row_width;
   if (row_info->bit_depth < 8)
   {
      row_width=row_info->width;
      switch (row_info->bit_depth)
      {
         case 1:
         {
            sp = row + (png_size_t)((row_width - 1) >> 3);
            dp = row + (png_size_t)row_width - 1;
            shift = 7 - (int)((row_width + 7) & 0x07);
            for (i = 0; i < row_width; i++)
            {
               *dp = (png_byte)((*sp >> shift) & 0x01);

               if (shift == 7)
               {
                  shift = 0;
                  sp--;
               }

               else
                  shift++;

               dp--;
            }
            break;
         }

         case 2:
         {

            sp = row + (png_size_t)((row_width - 1) >> 2);
            dp = row + (png_size_t)row_width - 1;
            shift = (int)((3 - ((row_width + 3) & 0x03)) << 1);
            for (i = 0; i < row_width; i++)
            {
               *dp = (png_byte)((*sp >> shift) & 0x03);

               if (shift == 6)
               {
                  shift = 0;
                  sp--;
               }

               else
                  shift += 2;

               dp--;
            }
            break;
         }

         case 4:
         {
            sp = row + (png_size_t)((row_width - 1) >> 1);
            dp = row + (png_size_t)row_width - 1;
            shift = (int)((1 - ((row_width + 1) & 0x01)) << 2);
            for (i = 0; i < row_width; i++)
            {
               *dp = (png_byte)((*sp >> shift) & 0x0f);

               if (shift == 4)
               {
                  shift = 0;
                  sp--;
               }

               else
                  shift = 4;

               dp--;
            }
            break;
         }

         default:
            break;
      }
      row_info->bit_depth = 8;
      row_info->pixel_depth = (png_byte)(8 * row_info->channels);
      row_info->rowbytes = row_width * row_info->channels;
   }
}
#endif

#if defined(PNG_READ_BGR_SUPPORTED) || defined(PNG_WRITE_BGR_SUPPORTED)
/* Swaps red and blue bytes within a pixel */
void /* PRIVATE */
png_do_bgr_2(png_row_info * row_info, png_byte * row)
{
   //png_debug(1, "in png_do_bgr");
   png_bytep rp;
   png_uint_32 i;
   png_byte save;
   png_uint_32 row_width;
   if ((row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      row_width = row_info->width;
      if (row_info->bit_depth == 8)
      {
         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
         {
            //png_bytep rp;
            //png_uint_32 i;

            for (i = 0, rp = row; i < row_width; i++, rp += 3)
            {
               save = *rp;
               *rp = *(rp + 2);
               *(rp + 2) = save;
            }
         }

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
         {
            //png_bytep rp;
            //png_uint_32 i;

            for (i = 0, rp = row; i < row_width; i++, rp += 4)
            {
               save = *rp;
               *rp = *(rp + 2);
               *(rp + 2) = save;
            }
         }
      }

#ifdef PNG_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
         {
            //png_bytep rp;
            /// png_uint_32 i;

            for (i = 0, rp = row; i < row_width; i++, rp += 6)
            {
               save = *rp;
               *rp = *(rp + 4);
               *(rp + 4) = save;
               save = *(rp + 1);
               *(rp + 1) = *(rp + 5);
               *(rp + 5) = save;
            }
         }

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
         {
         	for (i = 0, rp = row; i < row_width; i++, rp += 8)
            {
               save = *rp;
               *rp = *(rp + 4);
               *(rp + 4) = save;
               save = *(rp + 1);
               *(rp + 1) = *(rp + 5);
               *(rp + 5) = save;
            }
         }
      }
#endif
   }
}
#endif /* PNG_READ_BGR_SUPPORTED or PNG_WRITE_BGR_SUPPORTED */

#if defined(PNG_READ_PACKSWAP_SUPPORTED)||defined(PNG_WRITE_PACKSWAP_SUPPORTED)
static png_byte onebppswaptable[256] = {
   0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
   0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
   0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
   0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
   0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
   0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
   0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
   0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
   0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
   0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
   0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
   0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
   0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
   0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
   0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
   0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
   0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
   0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
   0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
   0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
   0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
   0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
   0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
   0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
   0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
   0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
   0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
   0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
   0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
   0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
   0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
   0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

static png_byte twobppswaptable[256] = {
   0x00, 0x40, 0x80, 0xC0, 0x10, 0x50, 0x90, 0xD0,
   0x20, 0x60, 0xA0, 0xE0, 0x30, 0x70, 0xB0, 0xF0,
   0x04, 0x44, 0x84, 0xC4, 0x14, 0x54, 0x94, 0xD4,
   0x24, 0x64, 0xA4, 0xE4, 0x34, 0x74, 0xB4, 0xF4,
   0x08, 0x48, 0x88, 0xC8, 0x18, 0x58, 0x98, 0xD8,
   0x28, 0x68, 0xA8, 0xE8, 0x38, 0x78, 0xB8, 0xF8,
   0x0C, 0x4C, 0x8C, 0xCC, 0x1C, 0x5C, 0x9C, 0xDC,
   0x2C, 0x6C, 0xAC, 0xEC, 0x3C, 0x7C, 0xBC, 0xFC,
   0x01, 0x41, 0x81, 0xC1, 0x11, 0x51, 0x91, 0xD1,
   0x21, 0x61, 0xA1, 0xE1, 0x31, 0x71, 0xB1, 0xF1,
   0x05, 0x45, 0x85, 0xC5, 0x15, 0x55, 0x95, 0xD5,
   0x25, 0x65, 0xA5, 0xE5, 0x35, 0x75, 0xB5, 0xF5,
   0x09, 0x49, 0x89, 0xC9, 0x19, 0x59, 0x99, 0xD9,
   0x29, 0x69, 0xA9, 0xE9, 0x39, 0x79, 0xB9, 0xF9,
   0x0D, 0x4D, 0x8D, 0xCD, 0x1D, 0x5D, 0x9D, 0xDD,
   0x2D, 0x6D, 0xAD, 0xED, 0x3D, 0x7D, 0xBD, 0xFD,
   0x02, 0x42, 0x82, 0xC2, 0x12, 0x52, 0x92, 0xD2,
   0x22, 0x62, 0xA2, 0xE2, 0x32, 0x72, 0xB2, 0xF2,
   0x06, 0x46, 0x86, 0xC6, 0x16, 0x56, 0x96, 0xD6,
   0x26, 0x66, 0xA6, 0xE6, 0x36, 0x76, 0xB6, 0xF6,
   0x0A, 0x4A, 0x8A, 0xCA, 0x1A, 0x5A, 0x9A, 0xDA,
   0x2A, 0x6A, 0xAA, 0xEA, 0x3A, 0x7A, 0xBA, 0xFA,
   0x0E, 0x4E, 0x8E, 0xCE, 0x1E, 0x5E, 0x9E, 0xDE,
   0x2E, 0x6E, 0xAE, 0xEE, 0x3E, 0x7E, 0xBE, 0xFE,
   0x03, 0x43, 0x83, 0xC3, 0x13, 0x53, 0x93, 0xD3,
   0x23, 0x63, 0xA3, 0xE3, 0x33, 0x73, 0xB3, 0xF3,
   0x07, 0x47, 0x87, 0xC7, 0x17, 0x57, 0x97, 0xD7,
   0x27, 0x67, 0xA7, 0xE7, 0x37, 0x77, 0xB7, 0xF7,
   0x0B, 0x4B, 0x8B, 0xCB, 0x1B, 0x5B, 0x9B, 0xDB,
   0x2B, 0x6B, 0xAB, 0xEB, 0x3B, 0x7B, 0xBB, 0xFB,
   0x0F, 0x4F, 0x8F, 0xCF, 0x1F, 0x5F, 0x9F, 0xDF,
   0x2F, 0x6F, 0xAF, 0xEF, 0x3F, 0x7F, 0xBF, 0xFF
};

static png_byte fourbppswaptable[256] = {
   0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
   0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
   0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71,
   0x81, 0x91, 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1,
   0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72,
   0x82, 0x92, 0xA2, 0xB2, 0xC2, 0xD2, 0xE2, 0xF2,
   0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73,
   0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
   0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74,
   0x84, 0x94, 0xA4, 0xB4, 0xC4, 0xD4, 0xE4, 0xF4,
   0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75,
   0x85, 0x95, 0xA5, 0xB5, 0xC5, 0xD5, 0xE5, 0xF5,
   0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76,
   0x86, 0x96, 0xA6, 0xB6, 0xC6, 0xD6, 0xE6, 0xF6,
   0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77,
   0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
   0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78,
   0x88, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8,
   0x09, 0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79,
   0x89, 0x99, 0xA9, 0xB9, 0xC9, 0xD9, 0xE9, 0xF9,
   0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A, 0x7A,
   0x8A, 0x9A, 0xAA, 0xBA, 0xCA, 0xDA, 0xEA, 0xFA,
   0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B,
   0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
   0x0C, 0x1C, 0x2C, 0x3C, 0x4C, 0x5C, 0x6C, 0x7C,
   0x8C, 0x9C, 0xAC, 0xBC, 0xCC, 0xDC, 0xEC, 0xFC,
   0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D, 0x6D, 0x7D,
   0x8D, 0x9D, 0xAD, 0xBD, 0xCD, 0xDD, 0xED, 0xFD,
   0x0E, 0x1E, 0x2E, 0x3E, 0x4E, 0x5E, 0x6E, 0x7E,
   0x8E, 0x9E, 0xAE, 0xBE, 0xCE, 0xDE, 0xEE, 0xFE,
   0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F,
   0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF
};

/* Swaps pixel packing order within bytes */
void /* PRIVATE */
png_do_packswap_2(png_row_info * row_info, png_byte * row)
{
   //png_debug(1, "in png_do_packswap");
   png_byte * rp;
   png_byte * end;
   png_byte * table;
   if (row_info->bit_depth < 8)
   {
      end = row + row_info->rowbytes;

      if (row_info->bit_depth == 1)
         table = onebppswaptable;

      else if (row_info->bit_depth == 2)
         table = twobppswaptable;

      else if (row_info->bit_depth == 4)
         table = fourbppswaptable;

      else
         return;

      for (rp = row; rp < end; rp++)
         *rp = table[*rp];
   }
}
#endif /* PNG_READ_PACKSWAP_SUPPORTED or PNG_WRITE_PACKSWAP_SUPPORTED */

#ifdef PNG_READ_FILLER_SUPPORTED
/* Add filler channel if we have RGB color */
void /* PRIVATE */
png_do_read_filler_2(png_row_info * row_info, png_byte * row,
    png_uint_32 filler, png_uint_32 flags)
{
   png_uint_32 i;
   png_uint_32 row_width = row_info->width;

#ifdef PNG_READ_16BIT_SUPPORTED
   png_byte hi_filler = (png_byte)((filler>>8) & 0xff);
#endif
   png_byte lo_filler = (png_byte)(filler & 0xff);
   png_byte * sp;
   png_byte * dp;
   //png_debug(1, "in png_do_read_filler");

   if (
       row_info->color_type == PNG_COLOR_TYPE_GRAY)
   {
      if (row_info->bit_depth == 8)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from G to GX */
            sp = row + (png_size_t)row_width;
            dp =  sp + (png_size_t)row_width;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
            }
            *(--dp) = lo_filler;
            row_info->channels = 2;
            row_info->pixel_depth = 16;
            row_info->rowbytes = row_width * 2;
         }

         else
         {
            /* This changes the data from G to XG */
            sp = row + (png_size_t)row_width;
            dp = sp  + (png_size_t)row_width;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = lo_filler;
            }
            row_info->channels = 2;
            row_info->pixel_depth = 16;
            row_info->rowbytes = row_width * 2;
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from GG to GGXX */
            sp = row + (png_size_t)row_width * 2;
            dp = sp  + (png_size_t)row_width * 2;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
            }
            *(--dp) = hi_filler;
            *(--dp) = lo_filler;
            row_info->channels = 2;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }

         else
         {
            /* This changes the data from GG to XXGG */
            sp = row + (png_size_t)row_width * 2;
            dp = sp  + (png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
            }
            row_info->channels = 2;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }
      }
#endif
   } /* COLOR_TYPE == GRAY */
   else if (row_info->color_type == PNG_COLOR_TYPE_RGB)
   {
      if (row_info->bit_depth == 8)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from RGB to RGBX */
            sp = row + (png_size_t)row_width * 3;
            dp = sp  + (png_size_t)row_width;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
            }
            *(--dp) = lo_filler;
            row_info->channels = 4;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }

         else
         {
            /* This changes the data from RGB to XRGB */
            sp = row + (png_size_t)row_width * 3;
            dp = sp + (png_size_t)row_width;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = lo_filler;
            }
            row_info->channels = 4;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from RRGGBB to RRGGBBXX */
            sp = row + (png_size_t)row_width * 6;
            dp = sp  + (png_size_t)row_width * 2;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
            }
            *(--dp) = hi_filler;
            *(--dp) = lo_filler;
            row_info->channels = 4;
            row_info->pixel_depth = 64;
            row_info->rowbytes = row_width * 8;
         }

         else
         {
            /* This changes the data from RRGGBB to XXRRGGBB */
            sp = row + (png_size_t)row_width * 6;
            dp = sp  + (png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
            }

            row_info->channels = 4;
            row_info->pixel_depth = 64;
            row_info->rowbytes = row_width * 8;
         }
      }
#endif
   } /* COLOR_TYPE == RGB */
}
#endif

#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
void /* PRIVATE */
png_do_read_invert_alpha_2(png_row_info * row_info, png_byte * row)
{
   png_uint_32 row_width;
   png_byte * sp;
   png_byte * dp;
   png_uint_32 i;
   //png_debug(1, "in png_do_read_invert_alpha");

   row_width = row_info->width;
   if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
   {
      if (row_info->bit_depth == 8)
      {
         /* This inverts the alpha channel in RGBA */
         sp = row + row_info->rowbytes;
         dp = sp;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (png_byte)(255 - *(--sp));

/*          This does nothing:
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            We can replace it with:
*/
            sp-=3;
            dp=sp;
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      /* This inverts the alpha channel in RRGGBBAA */
      else
      {
         sp = row + row_info->rowbytes;
         dp = sp;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (png_byte)(255 - *(--sp));
            *(--dp) = (png_byte)(255 - *(--sp));

/*          This does nothing:
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            We can replace it with:
*/
            sp-=6;
            dp=sp;
         }
      }
#endif
   }
   else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
   {
      if (row_info->bit_depth == 8)
      {
         /* This inverts the alpha channel in GA */
         sp = row + row_info->rowbytes;
         dp = sp;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (png_byte)(255 - *(--sp));
            *(--dp) = *(--sp);
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      else
      {
         /* This inverts the alpha channel in GGAA */
         sp  = row + row_info->rowbytes;
         dp = sp;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (png_byte)(255 - *(--sp));
            *(--dp) = (png_byte)(255 - *(--sp));
/*
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
*/
            sp-=2;
            dp=sp;
         }
      }
#endif
   }
}
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
void /* PRIVATE */
png_do_read_swap_alpha_2(png_row_info * row_info, png_byte * row)
{
   //png_debug(1, "in png_do_read_swap_alpha");
   png_bytep sp;
   png_bytep dp;
   png_byte save[2];
   png_uint_32 i;
   png_uint_32 row_width;
   {
      row_width = row_info->width;
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
         /* This converts from RGBA to ARGB */
         if (row_info->bit_depth == 8)
         {
            sp = row + row_info->rowbytes;
            dp = sp;

            for (i = 0; i < row_width; i++)
            {
               save[0] = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save[0];
            }
         }

#ifdef PNG_READ_16BIT_SUPPORTED
         /* This converts from RRGGBBAA to AARRGGBB */
         else
         {
            sp = row + row_info->rowbytes;
            dp = sp;

            for (i = 0; i < row_width; i++)
            {
               save[0] = *(--sp);
               save[1] = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save[0];
               *(--dp) = save[1];
            }
         }
#endif
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         /* This converts from GA to AG */
         if (row_info->bit_depth == 8)
         {
            sp = row + row_info->rowbytes;
            dp = sp;

            for (i = 0; i < row_width; i++)
            {
               save[0] = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save[0];
            }
         }

#ifdef PNG_READ_16BIT_SUPPORTED
         /* This converts from GGAA to AAGG */
         else
         {
            sp = row + row_info->rowbytes;
            dp = sp;

            for (i = 0; i < row_width; i++)
            {
               save[0] = *(--sp);
               save[1] = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save[0];
               *(--dp) = save[1];
            }
         }
#endif
      }
   }
}
#endif

#ifdef PNG_16BIT_SUPPORTED
#if defined(PNG_READ_SWAP_SUPPORTED) || defined(PNG_WRITE_SWAP_SUPPORTED)
/* Swaps byte order on 16 bit depth images */
void /* PRIVATE */
png_do_swap_2(png_row_info * row_info, png_byte * row)
{
   //png_debug(1, "in png_do_swap");
   png_bytep rp;
   png_uint_32 i;
   png_uint_32 istop;
   png_byte t;
   if (row_info->bit_depth == 16)
   {
      rp = row;
      istop= row_info->width * row_info->channels;

      for (i = 0; i < istop; i++, rp += 2)
      {
         t = *rp;
         *rp = *(rp + 1);
         *(rp + 1) = t;
      }
   }
}
#endif
#endif

void /* PRIVATE */
png_read_filter_row_2(png_struct * png_ptr, png_row_info * row_info, png_byte * row,
    png_byte * prev_row, int filter)
{
   //png_debug(1, "in png_read_filter_row");
   //png_debug2(2, "row = %u, filter = %d", png_ptr->row_number, filter);
   png_size_t i;
   png_byte * rp;
   png_byte * pp;
   png_byte * lp;
   png_byte * cp;
   unsigned int bpp;
   png_size_t istop;
   int a, b, c, pa, pb, pc, p;
   
   switch (filter)
   {
      case PNG_FILTER_VALUE_NONE:
         break;

      case PNG_FILTER_VALUE_SUB:
      {
         istop = row_info->rowbytes;
         bpp = (row_info->pixel_depth + 7) >> 3;
         rp = row + bpp;
         lp = row;

         for (i = bpp; i < istop; i++)
         {
            *rp = (png_byte)(((int)(*rp) + (int)(*lp++)) & 0xff);
            rp++;
         }
         break;
      }
      case PNG_FILTER_VALUE_UP:
      {
         istop = row_info->rowbytes;
         rp = row;
         pp = prev_row;

         for (i = 0; i < istop; i++)
         {
            *rp = (png_byte)(((int)(*rp) + (int)(*pp++)) & 0xff);
            rp++;
         }
         break;
      }
      case PNG_FILTER_VALUE_AVG:
      {
         rp = row;
         pp = prev_row;
         lp = row;
         bpp = (row_info->pixel_depth + 7) >> 3;
         istop = row_info->rowbytes - bpp;

         for (i = 0; i < bpp; i++)
         {
            *rp = (png_byte)(((int)(*rp) +
                ((int)(*pp++) / 2 )) & 0xff);

            rp++;
         }

         for (i = 0; i < istop; i++)
         {
            *rp = (png_byte)(((int)(*rp) +
                (int)(*pp++ + *lp++) / 2 ) & 0xff);

            rp++;
         }
         break;
      }
      case PNG_FILTER_VALUE_PAETH:
      {
         rp = row;
         pp = prev_row;
         lp = row;
         cp = prev_row;
         bpp = (row_info->pixel_depth + 7) >> 3;
         istop=row_info->rowbytes - bpp;
            
         for (i = 0; i < bpp; i++)
         {
            *rp = (png_byte)(((int)(*rp) + (int)(*pp++)) & 0xff);
            rp++;
         }

         for (i = 0; i < istop; i++)   /* Use leftover rp,pp */
         {
            
            a = *lp++;
            b = *pp++;
            c = *cp++;

            p = b - c;
            pc = a - c;

#ifdef PNG_USE_ABS
            pa = abs(p);
            pb = abs(pc);
            pc = abs(p + pc);
#else
            pa = p < 0 ? -p : p;
            pb = pc < 0 ? -pc : pc;
            pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#endif

            /*
               if (pa <= pb && pa <= pc)
                  p = a;

               else if (pb <= pc)
                  p = b;

               else
                  p = c;
             */

            p = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;

            *rp = (png_byte)(((int)(*rp) + p) & 0xff);
            rp++;
         }
         break;
      }
      default:
         p = p;
         //png_error(png_ptr, "Ignoring bad adaptive filter type");
         /*NOT REACHED */
         break;
   }
}

#ifdef PNG_MNG_FEATURES_SUPPORTED
/* Undoes intrapixel differencing  */
void /* PRIVATE */
png_do_read_intrapixel_2(png_row_info * row_info, png_byte * row)
{
   int bytes_per_pixel;
   png_uint_32 row_width;
   png_uint_32 s0;
   png_uint_32 s1;
   png_uint_32 s2;
   png_uint_32 red;
   png_uint_32 blue;
   png_bytep rp;
   png_uint_32 i;         
   //png_debug(1, "in png_do_read_intrapixel");

   if ((row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      row_width = row_info->width;

      if (row_info->bit_depth == 8)
      {
         //png_bytep rp;
         //png_uint_32 i;

         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
            bytes_per_pixel = 3;

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            bytes_per_pixel = 4;

         else
            return;

         for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
         {
            *(rp) = (png_byte)((256 + *rp + *(rp + 1)) & 0xff);
            *(rp+2) = (png_byte)((256 + *(rp + 2) + *(rp + 1)) & 0xff);
         }
      }
      else if (row_info->bit_depth == 16)
      {
         //png_bytep rp;
         //png_uint_32 i;

         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
            bytes_per_pixel = 6;

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            bytes_per_pixel = 8;

         else
            return;

         for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
         {
            s0   = (*(rp    ) << 8) | *(rp + 1);
            s1   = (*(rp + 2) << 8) | *(rp + 3);
            s2   = (*(rp + 4) << 8) | *(rp + 5);
            red  = (png_uint_32)((s0 + s1 + 65536L) & 0xffffL);
            blue = (png_uint_32)((s2 + s1 + 65536L) & 0xffffL);
            *(rp    ) = (png_byte)((red >> 8) & 0xff);
            *(rp + 1) = (png_byte)(red & 0xff);
            *(rp + 4) = (png_byte)((blue >> 8) & 0xff);
            *(rp + 5) = (png_byte)(blue & 0xff);
         }
      }
   }
}
#endif /* PNG_MNG_FEATURES_SUPPORTED */

#if defined(PNG_READ_gAMA_SUPPORTED) || defined(PNG_READ_cHRM_SUPPORTED)
/* The following is a variation on the above for use with the fixed
 * point values used for gAMA and cHRM.  Instead of png_error it
 * issues a warning and returns (-1) - an invalid value because both
 * gAMA and cHRM use *unsigned* integers for fixed point values.
 */
#define PNG_FIXED_ERROR (-1)

static png_fixed_point /* PRIVATE */
png_get_fixed_point_2(png_struct * png_ptr, png_const_bytep buf)
{
   png_uint_32 uval = png_get_uint_32_2(buf);

   if (uval <= PNG_UINT_31_MAX)
      return (png_fixed_point)uval; /* known to be in range */

   /* The caller can turn off the warning by passing NULL. */
   //if (png_ptr != NULL)
   //   png_warning(png_ptr, "PNG fixed point integer out of range");

   return PNG_FIXED_ERROR;
}
#endif

#ifdef PNG_TEXT_SUPPORTED
void png_set_text_4(png_struct * png_ptr, png_info * info_ptr, png_text * text_ptr,
    int num_text)
{
   int ret;
   ret = png_set_text_3(png_ptr, info_ptr, text_ptr, num_text);

   //if (ret)
      //png_error(png_ptr, "Insufficient memory to store text");
}

int /* PRIVATE */
png_set_text_3(png_struct * png_ptr, png_info * info_ptr,
    png_text * text_ptr, int num_text)
{
   int i;
   png_text * old_text;
   int old_max;
   png_size_t text_length, key_len;
   png_size_t lang_len, lang_key_len;
   png_text * textp;
   //png_debug1(1, "in %s storage function", ((png_ptr == NULL ||
   //    png_ptr->chunk_name[0] == '\0') ?
    ///   "text" : (png_const_charp)png_ptr->chunk_name));

   if (png_ptr == NULL || info_ptr == NULL || num_text == 0)
      return(0);

   /* Make sure we have enough space in the "text" array in info_struct
    * to hold all of the incoming text_ptr objects.
    */
   if (info_ptr->num_text + num_text > info_ptr->max_text)
   {
      if (info_ptr->text != NULL)
      {
         
         old_max = info_ptr->max_text;
         info_ptr->max_text = info_ptr->num_text + num_text + 8;
         old_text = info_ptr->text;
         info_ptr->text = (png_text *)png_malloc_warn_2(png_ptr,
            (png_size_t)(info_ptr->max_text * png_sizeof(png_text)));

         if (info_ptr->text == NULL)
         {
            png_free_2(png_ptr, old_text);
            return(1);
         }

         memcpy(info_ptr->text, old_text, (png_size_t)(old_max *
             png_sizeof(png_text)));
         png_free_2(png_ptr, old_text);
      }

      else
      {
         info_ptr->max_text = num_text + 8;
         info_ptr->num_text = 0;
         info_ptr->text = (png_text *)png_malloc_warn_2(png_ptr,
             (png_size_t)(info_ptr->max_text * png_sizeof(png_text)));
         if (info_ptr->text == NULL)
            return(1);
         info_ptr->free_me |= PNG_FREE_TEXT;
      }

      //png_debug1(3, "allocated %d entries for info_ptr->text",
      //    info_ptr->max_text);
   }
   for (i = 0; i < num_text; i++)
   {
      textp = &(info_ptr->text[info_ptr->num_text]);

      if (text_ptr[i].key == NULL)
          continue;

      if (text_ptr[i].compression < PNG_TEXT_COMPRESSION_NONE ||
          text_ptr[i].compression >= PNG_TEXT_COMPRESSION_LAST)
      {
         //png_warning(png_ptr, "text compression mode is out of range");
         continue;
      }

      key_len = strlen(text_ptr[i].key);

      if (text_ptr[i].compression <= 0)
      {
         lang_len = 0;
         lang_key_len = 0;
      }

      else
#  ifdef PNG_iTXt_SUPPORTED
      {
         /* Set iTXt data */

         if (text_ptr[i].lang != NULL)
            lang_len = strlen(text_ptr[i].lang);

         else
            lang_len = 0;

         if (text_ptr[i].lang_key != NULL)
            lang_key_len = strlen(text_ptr[i].lang_key);

         else
            lang_key_len = 0;
      }
#  else /* PNG_iTXt_SUPPORTED */
      {
         //png_warning(png_ptr, "iTXt chunk not supported");
         continue;
      }
#  endif

      if (text_ptr[i].text == NULL || text_ptr[i].text[0] == '\0')
      {
         text_length = 0;
#  ifdef PNG_iTXt_SUPPORTED
         if (text_ptr[i].compression > 0)
            textp->compression = PNG_ITXT_COMPRESSION_NONE;

         else
#  endif
            textp->compression = PNG_TEXT_COMPRESSION_NONE;
      }

      else
      {
         text_length = strlen(text_ptr[i].text);
         textp->compression = text_ptr[i].compression;
      }

      textp->key = (char *)png_malloc_warn_2(png_ptr,
          (png_size_t)
          (key_len + text_length + lang_len + lang_key_len + 4));

      if (textp->key == NULL)
         return(1);

      //png_debug2(2, "Allocated %lu bytes at %p in png_set_text",
      //    (unsigned long)(png_uint_32)
      //    (key_len + lang_len + lang_key_len + text_length + 4),
      //    textp->key);

      memcpy(textp->key, text_ptr[i].key,(png_size_t)(key_len));
      *(textp->key + key_len) = '\0';

      if (text_ptr[i].compression > 0)
      {
         textp->lang = textp->key + key_len + 1;
         memcpy(textp->lang, text_ptr[i].lang, lang_len);
         *(textp->lang + lang_len) = '\0';
         textp->lang_key = textp->lang + lang_len + 1;
         memcpy(textp->lang_key, text_ptr[i].lang_key, lang_key_len);
         *(textp->lang_key + lang_key_len) = '\0';
         textp->text = textp->lang_key + lang_key_len + 1;
      }

      else
      {
         textp->lang=NULL;
         textp->lang_key=NULL;
         textp->text = textp->key + key_len + 1;
      }

      if (text_length)
         memcpy(textp->text, text_ptr[i].text,
             (png_size_t)(text_length));

      *(textp->text + text_length) = '\0';

#  ifdef PNG_iTXt_SUPPORTED
      if (textp->compression > 0)
      {
         textp->text_length = 0;
         textp->itxt_length = text_length;
      }

      else
#  endif
      {
         textp->text_length = text_length;
         textp->itxt_length = 0;
      }

      info_ptr->num_text++;
      //png_debug1(3, "transferred text chunk %d", info_ptr->num_text);
   }
   return(0);
}
#endif

#ifdef PNG_bKGD_SUPPORTED
void png_set_bKGD_2(png_struct * png_ptr, png_info * info_ptr,
    png_color_16 * background)
{
   //png_debug1(1, "in %s storage function", "bKGD");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   memcpy(&(info_ptr->background), background, png_sizeof(png_color_16));
   info_ptr->valid |= PNG_INFO_bKGD;
}
#endif

#ifdef PNG_tIME_SUPPORTED
void png_set_tIME_2(png_struct * png_ptr, png_info * info_ptr, png_time * mod_time)
{
   //png_debug1(1, "in %s storage function", "tIME");

   if (png_ptr == NULL || info_ptr == NULL ||
       (png_ptr->mode & PNG_WROTE_tIME))
      return;

   memcpy(&(info_ptr->mod_time), mod_time, png_sizeof(png_time));
   info_ptr->valid |= PNG_INFO_tIME;
}
#endif

#ifdef PNG_tRNS_SUPPORTED
void png_set_tRNS_2(png_struct * png_ptr, png_info * info_ptr,
    png_byte * trans_alpha, int num_trans, png_color_16 * trans_color)
{
   //png_debug1(1, "in %s storage function", "tRNS");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   if (trans_alpha != NULL)
   {
       /* It may not actually be necessary to set png_ptr->trans_alpha here;
        * we do it for backward compatibility with the way the png_handle_tRNS
        * function used to do the allocation.
        */

       png_free_data_2(png_ptr, info_ptr, PNG_FREE_TRNS, 0);

       /* Changed from num_trans to PNG_MAX_PALETTE_LENGTH in version 1.2.1 */
       png_ptr->trans_alpha = info_ptr->trans_alpha =
           (png_bytep)png_malloc_2(png_ptr, (png_size_t)PNG_MAX_PALETTE_LENGTH);

       if (num_trans > 0 && num_trans <= PNG_MAX_PALETTE_LENGTH)
          memcpy(info_ptr->trans_alpha, trans_alpha, (png_size_t)num_trans);
   }

   if (trans_color != NULL)
   {
      int sample_max = (1 << info_ptr->bit_depth);

      if ((info_ptr->color_type == PNG_COLOR_TYPE_GRAY &&
          (int)trans_color->gray > sample_max) ||
          (info_ptr->color_type == PNG_COLOR_TYPE_RGB &&
          ((int)trans_color->red > sample_max ||
          (int)trans_color->green > sample_max ||
          (int)trans_color->blue > sample_max)))
         //png_warning(png_ptr,
         //   "tRNS chunk has out-of-range samples for bit_depth");

      memcpy(&(info_ptr->trans_color), trans_color,
         png_sizeof(png_color_16));

      if (num_trans == 0)
         num_trans = 1;
   }

   info_ptr->num_trans = (png_uint_16)num_trans;

   if (num_trans != 0)
   {
      info_ptr->valid |= PNG_INFO_tRNS;
      info_ptr->free_me |= PNG_FREE_TRNS;
   }
}
#endif

#ifdef PNG_sPLT_SUPPORTED
void png_set_sPLT_2(png_struct * png_ptr,
    png_info * info_ptr, png_sPLT_tp entries, int nentries)
/*
 *  entries        - array of png_sPLT_t structures
 *                   to be added to the list of palettes
 *                   in the info structure.
 *
 *  nentries       - number of palette structures to be
 *                   added.
 */
{
   png_sPLT_tp np;
   int i;
   png_sPLT_tp to;
   png_sPLT_tp from;
   png_uint_32 length;

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   np = (png_sPLT_tp)png_malloc_warn_2(png_ptr,
       (info_ptr->splt_palettes_num + nentries) *
       (png_size_t)png_sizeof(png_sPLT_t));

   if (np == NULL)
   {
      //png_warning(png_ptr, "No memory for sPLT palettes");
      return;
   }

   memcpy(np, info_ptr->splt_palettes,
       info_ptr->splt_palettes_num * png_sizeof(png_sPLT_t));

   png_free_2(png_ptr, info_ptr->splt_palettes);
   info_ptr->splt_palettes=NULL;

   for (i = 0; i < nentries; i++)
   {
      to = np + info_ptr->splt_palettes_num + i;
      from = entries + i;

      length = strlen(from->name) + 1;
      to->name = (char *)png_malloc_warn_2(png_ptr, (png_size_t)length);

      if (to->name == NULL)
      {
         //png_warning(png_ptr,
         //    "Out of memory while processing sPLT chunk");
         continue;
      }

      memcpy(to->name, from->name, length);
      to->entries = (png_sPLT_entry *)png_malloc_warn_2(png_ptr,
          (png_size_t)(from->nentries * png_sizeof(png_sPLT_entry)));

      if (to->entries == NULL)
      {
         //png_warning(png_ptr,
         //    "Out of memory while processing sPLT chunk");
         png_free_2(png_ptr, to->name);
         to->name = NULL;
         continue;
      }

      memcpy(to->entries, from->entries,
          from->nentries * png_sizeof(png_sPLT_entry));

      to->nentries = from->nentries;
      to->depth = from->depth;
   }

   info_ptr->splt_palettes = np;
   info_ptr->splt_palettes_num += nentries;
   info_ptr->valid |= PNG_INFO_sPLT;
   info_ptr->free_me |= PNG_FREE_SPLT;
}
#endif /* PNG_sPLT_SUPPORTED */

#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
void png_set_unknown_chunks_2(png_struct * png_ptr,
   png_info * info_ptr, png_unknown_chunk * unknowns, int num_unknowns)
{
   png_unknown_chunk * np;
   png_unknown_chunk * to;	// = np + info_ptr->unknown_chunks_num + i;
   png_unknown_chunk * from;	// = unknowns + i;
   int i;

   if (png_ptr == NULL || info_ptr == NULL || num_unknowns == 0)
      return;

   np = (png_unknown_chunk *)png_malloc_warn_2(png_ptr,
       (png_size_t)(info_ptr->unknown_chunks_num + num_unknowns) *
       png_sizeof(png_unknown_chunk));

   if (np == NULL)
   {
      //png_warning(png_ptr,
      //    "Out of memory while processing unknown chunk");
      return;
   }

   memcpy(np, info_ptr->unknown_chunks,
       (png_size_t)info_ptr->unknown_chunks_num *
       png_sizeof(png_unknown_chunk));

   png_free_2(png_ptr, info_ptr->unknown_chunks);
   info_ptr->unknown_chunks = NULL;

   for (i = 0; i < num_unknowns; i++)
   {
      to = np + info_ptr->unknown_chunks_num + i;
      from = unknowns + i;

      memcpy(to->name, from->name, png_sizeof(from->name));
      to->name[png_sizeof(to->name)-1] = '\0';
      to->size = from->size;

      /* Note our location in the read or write sequence */
      to->location = (png_byte)(png_ptr->mode & 0xff);

      if (from->size == 0)
         to->data=NULL;

      else
      {
         to->data = (png_bytep)png_malloc_warn_2(png_ptr,
             (png_size_t)from->size);

         if (to->data == NULL)
         {
            //png_warning(png_ptr,
            //    "Out of memory while processing unknown chunk");
            to->size = 0;
         }

         else
            memcpy(to->data, from->data, from->size);
      }
   }

   info_ptr->unknown_chunks = np;
   info_ptr->unknown_chunks_num += num_unknowns;
   info_ptr->free_me |= PNG_FREE_UNKN;
}
#endif

#ifdef PNG_cHRM_SUPPORTED
void png_set_cHRM_fixed_2(png_struct * png_ptr, png_info * info_ptr,
    png_fixed_point white_x, png_fixed_point white_y, png_fixed_point red_x,
    png_fixed_point red_y, png_fixed_point green_x, png_fixed_point green_y,
    png_fixed_point blue_x, png_fixed_point blue_y)
{
   //png_debug1(1, "in %s storage function", "cHRM fixed");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

#  ifdef PNG_CHECK_cHRM_SUPPORTED
   if (png_check_cHRM_fixed_2(png_ptr,
       white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y))
#  endif
   {
      info_ptr->x_white = white_x;
      info_ptr->y_white = white_y;
      info_ptr->x_red   = red_x;
      info_ptr->y_red   = red_y;
      info_ptr->x_green = green_x;
      info_ptr->y_green = green_y;
      info_ptr->x_blue  = blue_x;
      info_ptr->y_blue  = blue_y;
      info_ptr->valid |= PNG_INFO_cHRM;
   }
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
void png_set_cHRM_2(png_struct * png_ptr, png_info * info_ptr,
    double white_x, double white_y, double red_x, double red_y,
    double green_x, double green_y, double blue_x, double blue_y)
{
   png_set_cHRM_fixed_2(png_ptr, info_ptr,
      png_fixed_2(png_ptr, white_x, "cHRM White X"),
      png_fixed_2(png_ptr, white_y, "cHRM White Y"),
      png_fixed_2(png_ptr, red_x, "cHRM Red X"),
      png_fixed_2(png_ptr, red_y, "cHRM Red Y"),
      png_fixed_2(png_ptr, green_x, "cHRM Green X"),
      png_fixed_2(png_ptr, green_y, "cHRM Green Y"),
      png_fixed_2(png_ptr, blue_x, "cHRM Blue X"),
      png_fixed_2(png_ptr, blue_y, "cHRM Blue Y"));
}
#  endif /* PNG_FLOATING_POINT_SUPPORTED */

#endif /* PNG_cHRM_SUPPORTED */

#ifdef PNG_gAMA_SUPPORTED
void png_set_gAMA_fixed_2(png_struct * png_ptr, png_info * info_ptr, png_fixed_point
    file_gamma)
{
   //png_debug1(1, "in %s storage function", "gAMA");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   /* Previously these values were limited, however they must be
    * wrong, therefore storing them (and setting PNG_INFO_gAMA)
    * must be wrong too.
    */
   if (file_gamma > (png_fixed_point)PNG_UINT_31_MAX) {
      //png_warning(png_ptr, "Gamma too large, ignored");
	}
   else if (file_gamma <= 0) {
      //png_warning(png_ptr, "Negative or zero gamma ignored");
	}
   else
   {
   		info_ptr->gamma = file_gamma;
   		info_ptr->valid |= PNG_INFO_gAMA;
   }
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
void png_set_gAMA_2(png_struct * png_ptr, png_info * info_ptr, double file_gamma)
{
   //png_set_gAMA_fixed(png_ptr, info_ptr, png_fixed_2(png_ptr, file_gamma,
   //    "png_set_gAMA"));
}
#  endif
#endif

#ifdef PNG_hIST_SUPPORTED
void png_set_hIST_2(png_struct * png_ptr, png_info * info_ptr, png_uint_16 * hist)
{
   int i;

   //png_debug1(1, "in %s storage function", "hIST");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   if (info_ptr->num_palette == 0 || info_ptr->num_palette
       > PNG_MAX_PALETTE_LENGTH)
   {
      //png_warning(png_ptr,
      //    "Invalid palette size, hIST allocation skipped");

      return;
   }

   png_free_data_2(png_ptr, info_ptr, PNG_FREE_HIST, 0);

   /* Changed from info->num_palette to PNG_MAX_PALETTE_LENGTH in
    * version 1.2.1
    */
   png_ptr->hist = (png_uint_16p)png_malloc_warn_2(png_ptr,
       PNG_MAX_PALETTE_LENGTH * png_sizeof(png_uint_16));

   if (png_ptr->hist == NULL)
   {
      //png_warning(png_ptr, "Insufficient memory for hIST chunk data");
      return;
   }

   for (i = 0; i < info_ptr->num_palette; i++)
      png_ptr->hist[i] = hist[i];

   info_ptr->hist = png_ptr->hist;
   info_ptr->valid |= PNG_INFO_hIST;
   info_ptr->free_me |= PNG_FREE_HIST;
}
#endif

void png_set_IHDR_2(png_struct * png_ptr, png_info * info_ptr,
    png_uint_32 width, png_uint_32 height, int bit_depth,
    int color_type, int interlace_type, int compression_type,
    int filter_type)
{
   //png_debug1(1, "in %s storage function", "IHDR");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   info_ptr->width = width;
   info_ptr->height = height;
   info_ptr->bit_depth = (png_byte)bit_depth;
   info_ptr->color_type = (png_byte)color_type;
   info_ptr->compression_type = (png_byte)compression_type;
   info_ptr->filter_type = (png_byte)filter_type;
   info_ptr->interlace_type = (png_byte)interlace_type;

   png_check_IHDR_2 (png_ptr, info_ptr->width, info_ptr->height,
       info_ptr->bit_depth, info_ptr->color_type, info_ptr->interlace_type,
       info_ptr->compression_type, info_ptr->filter_type);

   if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      info_ptr->channels = 1;

   else if (info_ptr->color_type & PNG_COLOR_MASK_COLOR)
      info_ptr->channels = 3;

   else
      info_ptr->channels = 1;

   if (info_ptr->color_type & PNG_COLOR_MASK_ALPHA)
      info_ptr->channels++;

   info_ptr->pixel_depth = (png_byte)(info_ptr->channels * info_ptr->bit_depth);

   /* Check for potential overflow */
   if (width >
       (PNG_UINT_32_MAX >> 3)      /* 8-byte RRGGBBAA pixels */
       - 48       /* bigrowbuf hack */
       - 1        /* filter byte */
       - 7*8      /* rounding of width to multiple of 8 pixels */
       - 8)       /* extra max_pixel_depth pad */
      info_ptr->rowbytes = 0;
   else
      info_ptr->rowbytes = PNG_ROWBYTES(info_ptr->pixel_depth, width);
}

#ifdef PNG_oFFs_SUPPORTED
void png_set_oFFs_2(png_struct * png_ptr, png_info * info_ptr,
    png_int_32 offset_x, png_int_32 offset_y, int unit_type)
{
   //png_debug1(1, "in %s storage function", "oFFs");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   info_ptr->x_offset = offset_x;
   info_ptr->y_offset = offset_y;
   info_ptr->offset_unit_type = (png_byte)unit_type;
   info_ptr->valid |= PNG_INFO_oFFs;
}
#endif

#ifdef PNG_pCAL_SUPPORTED
void png_set_pCAL_2(png_struct * png_ptr, png_info * info_ptr,
    char * purpose, png_int_32 X0, png_int_32 X1, int type,
    int nparams, char * units, char ** params)
{
   png_size_t length;
   int i;

   //png_debug1(1, "in %s storage function", "pCAL");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   length = strlen(purpose) + 1;
   //png_debug1(3, "allocating purpose for info (%lu bytes)",
   //    (unsigned long)length);

   /* TODO: validate format of calibration name and unit name */

   /* Check that the type matches the specification. */
   //if (type < 0 || type > 3)
      //png_error(png_ptr, "Invalid pCAL equation type");

   /* Validate params[nparams] */
   //for (i=0; i<nparams; ++i)
      //if (!png_check_fp_string_2(params[i], png_strlen(params[i])))
         //png_error(png_ptr, "Invalid format for pCAL parameter");

   info_ptr->pcal_purpose = (char *)png_malloc_warn_2(png_ptr, length);

   if (info_ptr->pcal_purpose == NULL)
   {
      //png_warning(png_ptr, "Insufficient memory for pCAL purpose");
      return;
   }

   memcpy(info_ptr->pcal_purpose, purpose, length);

   //png_debug(3, "storing X0, X1, type, and nparams in info");
   info_ptr->pcal_X0 = X0;
   info_ptr->pcal_X1 = X1;
   info_ptr->pcal_type = (png_byte)type;
   info_ptr->pcal_nparams = (png_byte)nparams;

   length = strlen(units) + 1;
   //png_debug1(3, "allocating units for info (%lu bytes)",
   //  (unsigned long)length);

   info_ptr->pcal_units = (char *)png_malloc_warn_2(png_ptr, length);

   if (info_ptr->pcal_units == NULL)
   {
      //png_warning(png_ptr, "Insufficient memory for pCAL units");
      return;
   }

   memcpy(info_ptr->pcal_units, units, length);

   info_ptr->pcal_params = (char **)png_malloc_warn_2(png_ptr,
       (png_size_t)((nparams + 1) * png_sizeof(char *)));

   if (info_ptr->pcal_params == NULL)
   {
      //png_warning(png_ptr, "Insufficient memory for pCAL params");
      return;
   }

   memset(info_ptr->pcal_params, 0, (nparams + 1) * png_sizeof(char *));

   for (i = 0; i < nparams; i++)
   {
      length = strlen(params[i]) + 1;
      //png_debug2(3, "allocating parameter %d for info (%lu bytes)", i,
      //    (unsigned long)length);

      info_ptr->pcal_params[i] = (char *)png_malloc_warn_2(png_ptr, length);

      if (info_ptr->pcal_params[i] == NULL)
      {
         //png_warning(png_ptr, "Insufficient memory for pCAL parameter");
         return;
      }

      memcpy(info_ptr->pcal_params[i], params[i], length);
   }

   info_ptr->valid |= PNG_INFO_pCAL;
   info_ptr->free_me |= PNG_FREE_PCAL;
}
#endif

#ifdef PNG_sCAL_SUPPORTED
void png_set_sCAL_s_2(png_struct * png_ptr, png_info * info_ptr,
    int unit, char * swidth, char * sheight)
{
   png_size_t lengthw = 0, lengthh = 0;

   //png_debug1(1, "in %s storage function", "sCAL");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   /* Double check the unit (should never get here with an invalid
    * unit unless this is an API call.)
    */
   //if (unit != 1 && unit != 2)
   //   png_error(png_ptr, "Invalid sCAL unit");

   //if (swidth == NULL || (lengthw = png_strlen(swidth)) <= 0 ||
   //    swidth[0] == 45 /*'-'*/ || !png_check_fp_string_2(swidth, lengthw))
   //   png_error(png_ptr, "Invalid sCAL width");

   //if (sheight == NULL || (lengthh = png_strlen(sheight)) <= 0 ||
   //    sheight[0] == 45 /*'-'*/ || !png_check_fp_string_2(sheight, lengthh))
   //   png_error(png_ptr, "Invalid sCAL height");

   info_ptr->scal_unit = (png_byte)unit;

   ++lengthw;

   //png_debug1(3, "allocating unit for info (%u bytes)", (unsigned int)lengthw);

   info_ptr->scal_s_width = (char *)png_malloc_warn_2(png_ptr, lengthw);

   if (info_ptr->scal_s_width == NULL)
   {
      //png_warning(png_ptr, "Memory allocation failed while processing sCAL");
      return;
   }

   memcpy(info_ptr->scal_s_width, swidth, lengthw);

   ++lengthh;

   //png_debug1(3, "allocating unit for info (%u bytes)", (unsigned int)lengthh);

   info_ptr->scal_s_height = (char *)png_malloc_warn_2(png_ptr, lengthh);

   if (info_ptr->scal_s_height == NULL)
   {
      png_free_2 (png_ptr, info_ptr->scal_s_width);
      info_ptr->scal_s_width = NULL;

      //png_warning(png_ptr, "Memory allocation failed while processing sCAL");
      return;
   }

   memcpy(info_ptr->scal_s_height, sheight, lengthh);

   info_ptr->valid |= PNG_INFO_sCAL;
   info_ptr->free_me |= PNG_FREE_SCAL;
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
void png_set_sCAL_2(png_struct * png_ptr, png_info * info_ptr, int unit, double width,
    double height)
{
   char swidth[PNG_sCAL_MAX_DIGITS+1];
   char sheight[PNG_sCAL_MAX_DIGITS+1];
   //png_debug1(1, "in %s storage function", "sCAL");

   /* Check the arguments. */
   //if (width <= 0)
   //   png_warning(png_ptr, "Invalid sCAL width ignored");

   //else if (height <= 0)
   //   png_warning(png_ptr, "Invalid sCAL height ignored");

   //else
   //{
      /* Convert 'width' and 'height' to ASCII. */
  png_ascii_from_fp_2(png_ptr, swidth, sizeof swidth, width,
     PNG_sCAL_PRECISION);
  png_ascii_from_fp_2(png_ptr, sheight, sizeof sheight, height,
     PNG_sCAL_PRECISION);

  png_set_sCAL_s_2(png_ptr, info_ptr, unit, swidth, sheight);
   //}
}
#  endif

#  ifdef PNG_FIXED_POINT_SUPPORTED
void png_set_sCAL_fixed_2(png_struct * png_ptr, png_info * info_ptr, int unit,
    png_fixed_point width, png_fixed_point height)
{
   char swidth[PNG_sCAL_MAX_DIGITS+1];
   char sheight[PNG_sCAL_MAX_DIGITS+1];
   //png_debug1(1, "in %s storage function", "sCAL");

   /* Check the arguments. */
   //if (width <= 0)
   //   png_warning(png_ptr, "Invalid sCAL width ignored");

   //else if (height <= 0)
   //   png_warning(png_ptr, "Invalid sCAL height ignored");

   //else
   //{
      /* Convert 'width' and 'height' to ASCII. */
      

      png_ascii_from_fixed_2(png_ptr, swidth, sizeof swidth, width);
      png_ascii_from_fixed_2(png_ptr, sheight, sizeof sheight, height);

      png_set_sCAL_s_2(png_ptr, info_ptr, unit, swidth, sheight);
   //}
}
#  endif
#endif

#ifdef PNG_pHYs_SUPPORTED
void png_set_pHYs_2(png_struct * png_ptr, png_info * info_ptr,
    png_uint_32 res_x, png_uint_32 res_y, int unit_type)
{
   //png_debug1(1, "in %s storage function", "pHYs");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   info_ptr->x_pixels_per_unit = res_x;
   info_ptr->y_pixels_per_unit = res_y;
   info_ptr->phys_unit_type = (png_byte)unit_type;
   info_ptr->valid |= PNG_INFO_pHYs;
}
#endif

void png_set_PLTE_2(png_struct * png_ptr, png_info * info_ptr,
    png_color * palette, int num_palette)
{

   //png_debug1(1, "in %s storage function", "PLTE");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   if (num_palette < 0 || num_palette > PNG_MAX_PALETTE_LENGTH)
   {
      if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE) {
         //png_error(png_ptr, "Invalid palette length");
      }
      else
      {
         //png_warning(png_ptr, "Invalid palette length");
         return;
      }
   }

   /* It may not actually be necessary to set png_ptr->palette here;
    * we do it for backward compatibility with the way the png_handle_tRNS
    * function used to do the allocation.
    */
   png_free_data_2(png_ptr, info_ptr, PNG_FREE_PLTE, 0);

   /* Changed in libpng-1.2.1 to allocate PNG_MAX_PALETTE_LENGTH instead
    * of num_palette entries, in case of an invalid PNG file that has
    * too-large sample values.
    */
   png_ptr->palette = (png_color *)png_calloc_2(png_ptr,
       PNG_MAX_PALETTE_LENGTH * png_sizeof(png_color));

   memcpy(png_ptr->palette, palette, num_palette * png_sizeof(png_color));
   info_ptr->palette = png_ptr->palette;
   info_ptr->num_palette = png_ptr->num_palette = (png_uint_16)num_palette;

   info_ptr->free_me |= PNG_FREE_PLTE;

   info_ptr->valid |= PNG_INFO_PLTE;
}

#ifdef PNG_sBIT_SUPPORTED
void png_set_sBIT_2(png_struct * png_ptr, png_info * info_ptr,
    png_color_8 * sig_bit)
{
   //png_debug1(1, "in %s storage function", "sBIT");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   memcpy(&(info_ptr->sig_bit), sig_bit, png_sizeof(png_color_8));
   info_ptr->valid |= PNG_INFO_sBIT;
}
#endif

#ifdef PNG_sRGB_SUPPORTED
void png_set_sRGB_2(png_struct * png_ptr, png_info * info_ptr, int srgb_intent)
{
   //png_debug1(1, "in %s storage function", "sRGB");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   info_ptr->srgb_intent = (png_byte)srgb_intent;
   info_ptr->valid |= PNG_INFO_sRGB;
}

void png_set_sRGB_gAMA_and_cHRM_2(png_struct * png_ptr, png_info * info_ptr,
    int srgb_intent)
{
   //png_debug1(1, "in %s storage function", "sRGB_gAMA_and_cHRM");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

   png_set_sRGB_2(png_ptr, info_ptr, srgb_intent);

#ifdef PNG_gAMA_SUPPORTED
   png_set_gAMA_fixed_2(png_ptr, info_ptr, 45455L);
#endif

#ifdef PNG_cHRM_SUPPORTED
   png_set_cHRM_fixed_2(png_ptr, info_ptr,
      /* color      x       y */
      /* white */ 31270L, 32900L,
      /* red   */ 64000L, 33000L,
      /* green */ 30000L, 60000L,
      /* blue  */ 15000L,  6000L
   );
#endif /* cHRM */
}
#endif /* sRGB */


#ifdef PNG_iCCP_SUPPORTED
void png_set_iCCP_2(png_struct * png_ptr, png_info * info_ptr,
    char * name, int compression_type,
    png_byte * profile, png_uint_32 proflen)
{
   char * new_iccp_name;
   png_byte * new_iccp_profile;
   png_uint_32 length;

   //png_debug1(1, "in %s storage function", "iCCP");

   if (png_ptr == NULL || info_ptr == NULL || name == NULL || profile == NULL)
      return;

   length = strlen(name)+1;
   new_iccp_name = (char *)png_malloc_warn_2(png_ptr, length);

   if (new_iccp_name == NULL)
   {
        //png_warning(png_ptr, "Insufficient memory to process iCCP chunk");
      return;
   }

   memcpy(new_iccp_name, name, length);
   new_iccp_profile = (png_byte *)png_malloc_warn_2(png_ptr, proflen);

   if (new_iccp_profile == NULL)
   {
      png_free_2(png_ptr, new_iccp_name);
      //png_warning(png_ptr,
      //    "Insufficient memory to process iCCP profile");
      return;
   }

   memcpy(new_iccp_profile, profile, (png_size_t)proflen);

   png_free_data_2(png_ptr, info_ptr, PNG_FREE_ICCP, 0);

   info_ptr->iccp_proflen = proflen;
   info_ptr->iccp_name = new_iccp_name;
   info_ptr->iccp_profile = new_iccp_profile;
   /* Compression is always zero but is here so the API and info structure
    * does not have to change if we introduce multiple compression types
    */
   info_ptr->iccp_compression = (png_byte)compression_type;
   info_ptr->free_me |= PNG_FREE_ICCP;
   info_ptr->valid |= PNG_INFO_iCCP;
}
#endif

/* Read and check the IDHR chunk */
void /* PRIVATE */
png_handle_IHDR_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_byte buf[13];
   png_uint_32 width, height;
   int bit_depth, color_type, compression_type, filter_type;
   int interlace_type;

   //png_debug(1, "in png_handle_IHDR");

   //if (png_ptr->mode & PNG_HAVE_IHDR)
   //   png_error(png_ptr, "Out of place IHDR");

   /* Check the length */
   //if (length != 13)
   //   png_error(png_ptr, "Invalid IHDR chunk");

   png_ptr->mode |= PNG_HAVE_IHDR;

   png_crc_read_2(png_ptr, buf, 13);
   png_crc_finish_2(png_ptr, 0);

   width = png_get_uint_31_2(png_ptr, buf);
   height = png_get_uint_31_2(png_ptr, buf + 4);
   bit_depth = buf[8];
   color_type = buf[9];
   compression_type = buf[10];
   filter_type = buf[11];
   interlace_type = buf[12];

   /* Set internal variables */
   png_ptr->width = width;
   png_ptr->height = height;
   png_ptr->bit_depth = (png_byte)bit_depth;
   png_ptr->interlaced = (png_byte)interlace_type;
   png_ptr->color_type = (png_byte)color_type;
#ifdef PNG_MNG_FEATURES_SUPPORTED
   png_ptr->filter_type = (png_byte)filter_type;
#endif
   png_ptr->compression_type = (png_byte)compression_type;

   /* Find number of channels */
   switch (png_ptr->color_type)
   {
      default: /* invalid, png_set_IHDR calls png_error */
      case PNG_COLOR_TYPE_GRAY:
      case PNG_COLOR_TYPE_PALETTE:
         png_ptr->channels = 1;
         break;

      case PNG_COLOR_TYPE_RGB:
         png_ptr->channels = 3;
         break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
         png_ptr->channels = 2;
         break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
         png_ptr->channels = 4;
         break;
   }

   /* Set up other useful info */
   png_ptr->pixel_depth = (png_byte)(png_ptr->bit_depth *
   png_ptr->channels);
   png_ptr->rowbytes = PNG_ROWBYTES(png_ptr->pixel_depth, png_ptr->width);
   //png_debug1(3, "bit_depth = %d", png_ptr->bit_depth);
   //png_debug1(3, "channels = %d", png_ptr->channels);
   //png_debug1(3, "rowbytes = %lu", (unsigned long)png_ptr->rowbytes);
   png_set_IHDR_2(png_ptr, info_ptr, width, height, bit_depth,
       color_type, interlace_type, compression_type, filter_type);
}

/* Read and check the palette */
void /* PRIVATE */
png_handle_PLTE_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_color palette[PNG_MAX_PALETTE_LENGTH];
   int num, i;
#ifdef PNG_POINTER_INDEXING_SUPPORTED
   png_color * pal_ptr;
#endif
   png_byte buf[3];

   //png_debug(1, "in png_handle_PLTE");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before PLTE");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid PLTE after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   //else if (png_ptr->mode & PNG_HAVE_PLTE)
   //   png_error(png_ptr, "Duplicate PLTE chunk");

   png_ptr->mode |= PNG_HAVE_PLTE;

   if (!(png_ptr->color_type&PNG_COLOR_MASK_COLOR))
   {
      //png_warning(png_ptr,
      //    "Ignoring PLTE chunk in grayscale PNG");
      png_crc_finish_2(png_ptr, length);
      return;
   }

#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   if (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      png_crc_finish_2(png_ptr, length);
      return;
   }
#endif

   if (length > 3*PNG_MAX_PALETTE_LENGTH || length % 3)
   {
      if (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
      {
         //png_warning(png_ptr, "Invalid palette chunk");
         png_crc_finish_2(png_ptr, length);
         return;
      }

      //else
      //{
         //png_error(png_ptr, "Invalid palette chunk");
      //}
   }

   num = (int)length / 3;

#ifdef PNG_POINTER_INDEXING_SUPPORTED
   for (i = 0, pal_ptr = palette; i < num; i++, pal_ptr++)
   {
      //png_byte buf[3];

      png_crc_read_2(png_ptr, buf, 3);
      pal_ptr->red = buf[0];
      pal_ptr->green = buf[1];
      pal_ptr->blue = buf[2];
   }
#else
   for (i = 0; i < num; i++)
   {
         png_crc_read_2(png_ptr, buf, 3);
      /* Don't depend upon png_color being any order */
      palette[i].red = buf[0];
      palette[i].green = buf[1];
      palette[i].blue = buf[2];
   }
#endif

   /* If we actually need the PLTE chunk (ie for a paletted image), we do
    * whatever the normal CRC configuration tells us.  However, if we
    * have an RGB image, the PLTE can be considered ancillary, so
    * we will act as though it is.
    */
#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#endif
   {
      png_crc_finish_2(png_ptr, 0);
   }

#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   else if (png_crc_error_2(png_ptr))  /* Only if we have a CRC error */
   {
      /* If we don't want to use the data from an ancillary chunk,
       * we have two options: an error abort, or a warning and we
       * ignore the data in this chunk (which should be OK, since
       * it's considered ancillary for a RGB or RGBA image).
       */
      if (!(png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_USE))
      {
         if (png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN)
         {
            png_chunk_benign_error_2(png_ptr, "CRC error");
         }

         else
         {
            png_chunk_warning_2(png_ptr, "CRC error");
            return;
         }
      }

      /* Otherwise, we (optionally) emit a warning and use the chunk. */
      else if (!(png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN))
      {
         png_chunk_warning_2(png_ptr, "CRC error");
      }
   }
#endif

   png_set_PLTE_2(png_ptr, info_ptr, palette, num);

#ifdef PNG_READ_tRNS_SUPPORTED
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS))
      {
         if (png_ptr->num_trans > (png_uint_16)num)
         {
            //png_warning(png_ptr, "Truncating incorrect tRNS chunk length");
            png_ptr->num_trans = (png_uint_16)num;
         }

         if (info_ptr->num_trans > (png_uint_16)num)
         {
            //png_warning(png_ptr, "Truncating incorrect info tRNS chunk length");
            info_ptr->num_trans = (png_uint_16)num;
         }
      }
   }
#endif

}

void /* PRIVATE */
png_handle_IEND_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_debug(1, "in png_handle_IEND");

   if (!(png_ptr->mode & PNG_HAVE_IHDR) || !(png_ptr->mode & PNG_HAVE_IDAT))
   {
      //png_error(png_ptr, "No image in file");
   }

   png_ptr->mode |= (PNG_AFTER_IDAT | PNG_HAVE_IEND);

   if (length != 0)
   {
      //png_warning(png_ptr, "Incorrect IEND chunk length");
   }

   png_crc_finish_2(png_ptr, length);

   //PNG_UNUSED(info_ptr) /* Quiet compiler warnings about unused info_ptr */
}

#ifdef PNG_READ_gAMA_SUPPORTED
void /* PRIVATE */
png_handle_gAMA_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_fixed_point igamma;
   png_byte buf[4];

   //png_debug(1, "in png_handle_gAMA");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before gAMA");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid gAMA after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   //else if (png_ptr->mode & PNG_HAVE_PLTE)
   //   /* Should be an error, but we can cope with it */
      //png_warning(png_ptr, "Out of place gAMA chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA)
#ifdef PNG_READ_sRGB_SUPPORTED
       && !(info_ptr->valid & PNG_INFO_sRGB)
#endif
       )
   {
      //png_warning(png_ptr, "Duplicate gAMA chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (length != 4)
   {
      //png_warning(png_ptr, "Incorrect gAMA chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, 4);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   igamma = png_get_fixed_point_2(NULL, buf);

   /* Check for zero gamma or an error. */
   if (igamma <= 0)
   {
      //png_warning(png_ptr,
      //    "Ignoring gAMA chunk with out of range gamma");

      return;
   }

#  ifdef PNG_READ_sRGB_SUPPORTED
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB))
   {
      if (PNG_OUT_OF_RANGE(igamma, 45500L, 500))
      {
         //png_warning(png_ptr,
         //    "Ignoring incorrect gAMA value when sRGB is also present");

#    ifdef PNG_CONSOLE_IO_SUPPORTED
         //fprintf(stderr, "gamma = (%d/100000)", (int)igamma);
#    endif
         return;
      }
   }
#  endif /* PNG_READ_sRGB_SUPPORTED */

#  ifdef PNG_READ_GAMMA_SUPPORTED
   /* Gamma correction on read is supported. */
   png_ptr->gamma = igamma;
#  endif
   /* And set the 'info' structure members. */
   png_set_gAMA_fixed_2(png_ptr, info_ptr, igamma);
}
#endif

#ifdef PNG_READ_sBIT_SUPPORTED
void /* PRIVATE */
png_handle_sBIT_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_size_t truelen;
   png_byte buf[4];

   //png_debug(1, "in png_handle_sBIT");

   buf[0] = buf[1] = buf[2] = buf[3] = 0;

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before sBIT");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid sBIT after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (png_ptr->mode & PNG_HAVE_PLTE)
   {
      /* Should be an error, but we can cope with it */
      //png_warning(png_ptr, "Out of place sBIT chunk");
   }

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sBIT))
   {
      //png_warning(png_ptr, "Duplicate sBIT chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      truelen = 3;

   else
      truelen = (png_size_t)png_ptr->channels;

   if (length != truelen || length > 4)
   {
      //png_warning(png_ptr, "Incorrect sBIT chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, truelen);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
   {
      png_ptr->sig_bit.red = buf[0];
      png_ptr->sig_bit.green = buf[1];
      png_ptr->sig_bit.blue = buf[2];
      png_ptr->sig_bit.alpha = buf[3];
   }

   else
   {
      png_ptr->sig_bit.gray = buf[0];
      png_ptr->sig_bit.red = buf[0];
      png_ptr->sig_bit.green = buf[0];
      png_ptr->sig_bit.blue = buf[0];
      png_ptr->sig_bit.alpha = buf[1];
   }

   png_set_sBIT_2(png_ptr, info_ptr, &(png_ptr->sig_bit));
}
#endif

#ifdef PNG_READ_cHRM_SUPPORTED
void /* PRIVATE */
png_handle_cHRM_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_byte buf[32];
   png_fixed_point x_white, y_white, x_red, y_red, x_green, y_green, x_blue,
      y_blue;

   //png_debug(1, "in png_handle_cHRM");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before cHRM");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid cHRM after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   //else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      //png_warning(png_ptr, "Missing PLTE before cHRM");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM)
#  ifdef PNG_READ_sRGB_SUPPORTED
       && !(info_ptr->valid & PNG_INFO_sRGB)
#  endif
      )
   {
      //png_warning(png_ptr, "Duplicate cHRM chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (length != 32)
   {
      //png_warning(png_ptr, "Incorrect cHRM chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, 32);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   x_white = png_get_fixed_point_2(NULL, buf);
   y_white = png_get_fixed_point_2(NULL, buf + 4);
   x_red   = png_get_fixed_point_2(NULL, buf + 8);
   y_red   = png_get_fixed_point_2(NULL, buf + 12);
   x_green = png_get_fixed_point_2(NULL, buf + 16);
   y_green = png_get_fixed_point_2(NULL, buf + 20);
   x_blue  = png_get_fixed_point_2(NULL, buf + 24);
   y_blue  = png_get_fixed_point_2(NULL, buf + 28);

   if (x_white == PNG_FIXED_ERROR ||
       y_white == PNG_FIXED_ERROR ||
       x_red   == PNG_FIXED_ERROR ||
       y_red   == PNG_FIXED_ERROR ||
       x_green == PNG_FIXED_ERROR ||
       y_green == PNG_FIXED_ERROR ||
       x_blue  == PNG_FIXED_ERROR ||
       y_blue  == PNG_FIXED_ERROR)
   {
      //png_warning(png_ptr, "Ignoring cHRM chunk with negative chromaticities");
      return;
   }

#ifdef PNG_READ_sRGB_SUPPORTED
   if ((info_ptr != NULL) && (info_ptr->valid & PNG_INFO_sRGB))
   {
      if (PNG_OUT_OF_RANGE(x_white, 31270,  1000) ||
          PNG_OUT_OF_RANGE(y_white, 32900,  1000) ||
          PNG_OUT_OF_RANGE(x_red,   64000L, 1000) ||
          PNG_OUT_OF_RANGE(y_red,   33000,  1000) ||
          PNG_OUT_OF_RANGE(x_green, 30000,  1000) ||
          PNG_OUT_OF_RANGE(y_green, 60000L, 1000) ||
          PNG_OUT_OF_RANGE(x_blue,  15000,  1000) ||
          PNG_OUT_OF_RANGE(y_blue,   6000,  1000))
      {
         //png_warning(png_ptr,
         //    "Ignoring incorrect cHRM value when sRGB is also present");

#ifdef PNG_CONSOLE_IO_SUPPORTED
         //fprintf(stderr, "wx=%d, wy=%d, rx=%d, ry=%d\n",
         //    x_white, y_white, x_red, y_red);

         //fprintf(stderr, "gx=%d, gy=%d, bx=%d, by=%d\n",
         //    x_green, y_green, x_blue, y_blue);
#endif /* PNG_CONSOLE_IO_SUPPORTED */
      }
      return;
   }
#endif /* PNG_READ_sRGB_SUPPORTED */

   png_set_cHRM_fixed_2(png_ptr, info_ptr, x_white, y_white, x_red, y_red,
      x_green, y_green, x_blue, y_blue);
}
#endif

#ifdef PNG_READ_sRGB_SUPPORTED
void /* PRIVATE */
png_handle_sRGB_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   int intent;
   png_byte buf[1];

   //png_debug(1, "in png_handle_sRGB");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before sRGB");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid sRGB after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   //else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
   //   png_warning(png_ptr, "Out of place sRGB chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB))
   {
      //png_warning(png_ptr, "Duplicate sRGB chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (length != 1)
   {
      //png_warning(png_ptr, "Incorrect sRGB chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, 1);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   intent = buf[0];

   /* Check for bad intent */
   if (intent >= PNG_sRGB_INTENT_LAST)
   {
      //png_warning(png_ptr, "Unknown sRGB intent");
      return;
   }

#if defined(PNG_READ_gAMA_SUPPORTED) && defined(PNG_READ_GAMMA_SUPPORTED)
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA))
   {
      if (PNG_OUT_OF_RANGE(info_ptr->gamma, 45500L, 500))
      {
         //png_warning(png_ptr,
          //   "Ignoring incorrect gAMA value when sRGB is also present");
#ifdef PNG_CONSOLE_IO_SUPPORTED
         //fprintf(stderr, "incorrect gamma=(%d/100000)\n", info_ptr->gamma);
#endif
      }
   }
#endif /* PNG_READ_gAMA_SUPPORTED */

#ifdef PNG_READ_cHRM_SUPPORTED
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
      if (PNG_OUT_OF_RANGE(info_ptr->x_white, 31270,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_white, 32900,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->x_red,   64000L, 1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_red,   33000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->x_green, 30000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_green, 60000L, 1000) ||
          PNG_OUT_OF_RANGE(info_ptr->x_blue,  15000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_blue,   6000,  1000))
      {
         //png_warning(png_ptr,
         //    "Ignoring incorrect cHRM value when sRGB is also present");
      }
#endif /* PNG_READ_cHRM_SUPPORTED */

   png_set_sRGB_gAMA_and_cHRM_2(png_ptr, info_ptr, intent);
}
#endif /* PNG_READ_sRGB_SUPPORTED */

//#if defined(PNG_READ_zTXt_SUPPORTED) || defined(PNG_READ_iTXt_SUPPORTED) || defined(PNG_READ_iCCP_SUPPORTED)
static png_size_t
png_inflate_2(png_struct * png_ptr, png_byte * data, png_size_t size,
    png_byte * output, png_size_t output_size)
{
   png_size_t count = 0;
   char *msg;
#ifdef PNG_CONSOLE_IO_SUPPORTED
   char umsg[52];
#endif
   png_size_t space;
   int ret, avail;
   png_size_t copy;
   /* zlib can't necessarily handle more than 65535 bytes at once (i.e. it can't
    * even necessarily handle 65536 bytes) because the type uInt is "16 bits or
    * more".  Consequently it is necessary to chunk the input to zlib.  This
    * code uses ZLIB_IO_MAX, from pngpriv.h, as the maximum (the maximum value
    * that can be stored in a uInt.)  It is possible to set ZLIB_IO_MAX to a
    * lower value in pngpriv.h and this may sometimes have a performance
    * advantage, because it forces access of the input data to be separated from
    * at least some of the use by some period of time.
    */
   png_ptr->zstream.next_in = data;
   /* avail_in is set below from 'size' */
   png_ptr->zstream.avail_in = 0;

   while (1)
   {
      /* The setting of 'avail_in' used to be outside the loop, by setting it
       * inside it is possible to chunk the input to zlib and simply rely on
       * zlib to advance the 'next_in' pointer.  This allows arbitrary amounts o
       * data to be passed through zlib at the unavoidable cost of requiring a
       * window save (memcpy of up to 32768 output bytes) every ZLIB_IO_MAX
       * input bytes.
       */
      if (png_ptr->zstream.avail_in == 0 && size > 0)
      {
         if (size <= ZLIB_IO_MAX)
         {
            /* The value is less than ZLIB_IO_MAX so the cast is safe: */
            png_ptr->zstream.avail_in = (uInt)size;
            size = 0;
         }

         else
         {
            png_ptr->zstream.avail_in = ZLIB_IO_MAX;
            size -= ZLIB_IO_MAX;
         }
      }

      /* Reset the output buffer each time round - we empty it
       * after every inflate call.
       */
      png_ptr->zstream.next_out = png_ptr->zbuf;
      png_ptr->zstream.avail_out = png_ptr->zbuf_size;

      ret = inflate(&png_ptr->zstream, Z_NO_FLUSH);
      avail = png_ptr->zbuf_size - png_ptr->zstream.avail_out;

      /* First copy/count any new output - but only if we didn't
       * get an error code.
       */
      if ((ret == Z_OK || ret == Z_STREAM_END) && avail > 0)
      {
         space = avail; /* > 0, see above */

         if (output != 0 && output_size > count)
         {
            copy = output_size - count;

            if (space < copy)
               copy = space;

            memcpy(output + count, png_ptr->zbuf, copy);
         }
         count += space;
      }

      if (ret == Z_OK)
         continue;

      /* Termination conditions - always reset the zstream, it
       * must be left in inflateInit state.
       */
      png_ptr->zstream.avail_in = 0;
      inflateReset(&png_ptr->zstream);

      if (ret == Z_STREAM_END)
         return count; /* NOTE: may be zero. */

      /* Now handle the error codes - the API always returns 0
       * and the error message is dumped into the uncompressed
       * buffer if available.
       */
      {
         
         if (png_ptr->zstream.msg != 0)
            msg = png_ptr->zstream.msg;

         else
         {
#ifdef PNG_CONSOLE_IO_SUPPORTED
            switch (ret)
            {
               case Z_BUF_ERROR:
                  msg = "Buffer error in compressed datastream in %s chunk";
                  break;

               case Z_DATA_ERROR:
                  msg = "Data error in compressed datastream in %s chunk";
                  break;

               default:
                  msg = "Incomplete compressed datastream in %s chunk";
                  break;
            }

            //png_snprintf(umsg, sizeof umsg, msg, png_ptr->chunk_name);
            //msg = umsg;
#else
            //msg = "Damaged compressed datastream in chunk other than IDAT";
#endif
         }

         //png_warning(png_ptr, msg);
      }

      /* 0 means an error - notice that this code simply ignores
       * zero length compressed chunks as a result.
       */
      return 0;
   }
}
/*
 * Decompress trailing data in a chunk.  The assumption is that chunkdata
 * points at an allocated area holding the contents of a chunk with a
 * trailing compressed part.  What we get back is an allocated area
 * holding the original prefix part and an uncompressed version of the
 * trailing part (the malloc area passed in is freed).
 */
void /* PRIVATE */
png_decompress_chunk_2(png_struct * png_ptr, int comp_type,
    png_size_t chunklength,
    png_size_t prefix_size, png_size_t *newlength)
{
   png_size_t expanded_size;
   png_size_t new_size = 0;
   char * text;
   /* The caller should guarantee this */
   if (prefix_size > chunklength)
   {
      /* The recovery is to delete the chunk. */
      //png_warning(png_ptr, "invalid chunklength");
      prefix_size = 0; /* To delete everything */
   }

   else if (comp_type == PNG_COMPRESSION_TYPE_BASE)
   {
      expanded_size = png_inflate_2(png_ptr,
          (png_byte *)(png_ptr->chunkdata + prefix_size),
          chunklength - prefix_size,
          0,            /*output*/
          0);           /*output size*/

      /* Now check the limits on this chunk - if the limit fails the
       * compressed data will be removed, the prefix will remain.
       */
#ifdef PNG_SET_CHUNK_MALLOC_LIMIT_SUPPORTED
      if (png_ptr->user_chunk_malloc_max &&
          (prefix_size + expanded_size >= png_ptr->user_chunk_malloc_max - 1))
#else
#  ifdef PNG_USER_CHUNK_MALLOC_MAX
      if ((PNG_USER_CHUNK_MALLOC_MAX > 0) &&
          prefix_size + expanded_size >= PNG_USER_CHUNK_MALLOC_MAX - 1)
#  endif
#endif
         //png_warning(png_ptr, "Exceeded size limit while expanding chunk");

      /* If the size is zero either there was an error and a message
       * has already been output (warning) or the size really is zero
       * and we have nothing to do - the code will exit through the
       * error case below.
       */
#if defined(PNG_SET_CHUNK_MALLOC_LIMIT_SUPPORTED) || \
    defined(PNG_USER_CHUNK_MALLOC_MAX)
      if (expanded_size > 0)
#else
      if (expanded_size > 0)
#endif
      {
         /* Success (maybe) - really uncompress the chunk. */
         new_size = 0;
         text = png_malloc_warn_2(png_ptr,
             prefix_size + expanded_size + 1);

         if (text != NULL)
         {
            memcpy(text, png_ptr->chunkdata, prefix_size);
            new_size = png_inflate_2(png_ptr,
                (png_byte *)(png_ptr->chunkdata + prefix_size),
                chunklength - prefix_size,
                (png_byte *)(text + prefix_size), expanded_size);
            text[prefix_size + expanded_size] = 0; /* just in case */

            if (new_size == expanded_size)
            {
               png_free_2(png_ptr, png_ptr->chunkdata);
               png_ptr->chunkdata = text;
               *newlength = prefix_size + expanded_size;
               return; /* The success return! */
            }

            //png_warning(png_ptr, "png_inflate logic error");
            png_free_2(png_ptr, text);
         }

         //else
            //png_warning(png_ptr, "Not enough memory to decompress chunk");
      }
   }

   else /* if (comp_type != PNG_COMPRESSION_TYPE_BASE) */
   {
#ifdef PNG_STDIO_SUPPORTED
      //char umsg[50];

      //png_snprintf(umsg, sizeof umsg,
      //    "Unknown zTXt compression type %d", comp_type);
      //png_warning(png_ptr, umsg);
#else
      //png_warning(png_ptr, "Unknown zTXt compression type");
#endif

      /* The recovery is to simply drop the data. */
   }

   /* Generic error return - leave the prefix, delete the compressed
    * data, reallocate the chunkdata to remove the potentially large
    * amount of compressed data.
    */
   {
   
      text = png_malloc_warn_2(png_ptr, prefix_size + 1);

      if (text != NULL)
      {
         if (prefix_size > 0)
            memcpy(text, png_ptr->chunkdata, prefix_size);

         png_free_2(png_ptr, png_ptr->chunkdata);
         png_ptr->chunkdata = text;

         /* This is an extra zero in the 'uncompressed' part. */
         *(png_ptr->chunkdata + prefix_size) = 0x00;
      }
      /* Ignore a malloc error here - it is safe. */
   }
   *newlength = prefix_size;
}

#ifdef PNG_READ_iCCP_SUPPORTED
void /* PRIVATE */
png_handle_iCCP_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
/* Note: this does not properly handle chunks that are > 64K under DOS */
{
   png_byte compression_type;
   png_byte * pC;
   char * profile;
   png_uint_32 skip = 0;
   png_uint_32 profile_size;
   png_alloc_size_t profile_length;
   png_size_t slength, prefix_length, data_length;

   //png_debug(1, "in png_handle_iCCP");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
      //png_error(png_ptr, "Missing IHDR before iCCP");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid iCCP after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   //else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
   //   png_warning(png_ptr, "Out of place iCCP chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_iCCP))
   {
      //png_warning(png_ptr, "Duplicate iCCP chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

#ifdef PNG_MAX_MALLOC_64K
   if (length > (png_uint_32)65535L)
   {
      //png_warning(png_ptr, "iCCP chunk too large to fit in memory");
      skip = length - (png_uint_32)65535L;
      length = (png_uint_32)65535L;
   }
#endif

   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (char *)png_malloc_2(png_ptr, length + 1);
   slength = (png_size_t)length;
   png_crc_read_2(png_ptr, (png_byte *)png_ptr->chunkdata, slength);

   if (png_crc_finish_2(png_ptr, skip))
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00;

   for (profile = png_ptr->chunkdata; *profile; profile++)
      /* Empty loop to find end of name */ ;

   ++profile;

   /* There should be at least one zero (the compression type byte)
    * following the separator, and we should be on it
    */
   if (profile >= png_ptr->chunkdata + slength - 1)
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      //png_warning(png_ptr, "Malformed iCCP chunk");
      return;
   }

   /* Compression_type should always be zero */
   compression_type = *profile++;

   if (compression_type)
   {
      //png_warning(png_ptr, "Ignoring nonzero compression type in iCCP chunk");
      compression_type = 0x00;  /* Reset it to zero (libpng-1.0.6 through 1.0.8
                                 wrote nonzero) */
   }

   prefix_length = profile - png_ptr->chunkdata;
   png_decompress_chunk_2(png_ptr, compression_type,
       slength, prefix_length, &data_length);

   profile_length = data_length - prefix_length;

   if (prefix_length > data_length || profile_length < 4)
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      //png_warning(png_ptr, "Profile size field missing from iCCP chunk");
      return;
   }

   /* Check the profile_size recorded in the first 32 bits of the ICC profile */
   pC = (png_byte *)(png_ptr->chunkdata + prefix_length);
   profile_size = ((*(pC    )) << 24) |
                  ((*(pC + 1)) << 16) |
                  ((*(pC + 2)) <<  8) |
                  ((*(pC + 3))      );

   /* NOTE: the following guarantees that 'profile_length' fits into 32 bits,
    * because profile_size is a 32 bit value.
    */
   if (profile_size < profile_length)
      profile_length = profile_size;

   /* And the following guarantees that profile_size == profile_length. */
   if (profile_size > profile_length)
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
#ifdef PNG_STDIO_SUPPORTED
      {
         //char umsg[80];

         //png_snprintf2(umsg, 80,
         //    "Ignoring iCCP chunk with declared size = %u "
         //     "and actual length = %u",
         //     (unsigned int) profile_size,
         //     (unsigned int) profile_length);
         //png_warning(png_ptr, umsg);
      }
#else
      //png_warning(png_ptr,
      //   "Ignoring iCCP chunk with uncompressed size mismatch");
#endif
      return;
   }

   png_set_iCCP_2(png_ptr, info_ptr, png_ptr->chunkdata,
       compression_type, (png_byte *)png_ptr->chunkdata + prefix_length,
       profile_size);
   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
}
#endif /* PNG_READ_iCCP_SUPPORTED */

#ifdef PNG_READ_sPLT_SUPPORTED
void /* PRIVATE */
png_handle_sPLT_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
/* Note: this does not properly handle chunks that are > 64K under DOS */
{
   png_byte * entry_start;
   png_sPLT_t new_palette;
#ifdef PNG_POINTER_INDEXING_SUPPORTED
   png_sPLT_entry * pp;
#endif
   png_uint_32 data_length;
   int entry_size, i;
   png_uint_32 skip = 0;
   png_size_t slength;
   png_uint_32 dl;
   png_size_t max_dl;

   //png_debug(1, "in png_handle_sPLT");

#ifdef PNG_USER_LIMITS_SUPPORTED

   if (png_ptr->user_chunk_cache_max != 0)
   {
      if (png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish_2(png_ptr, length);
         return;
      }

      if (--png_ptr->user_chunk_cache_max == 1)
      {
         //png_warning(png_ptr, "No space in chunk cache for sPLT");
         png_crc_finish_2(png_ptr, length);
         return;
      }
   }
#endif

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before sPLT");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid sPLT after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

#ifdef PNG_MAX_MALLOC_64K
   if (length > (png_uint_32)65535L)
   {
      //png_warning(png_ptr, "sPLT chunk too large to fit in memory");
      skip = length - (png_uint_32)65535L;
      length = (png_uint_32)65535L;
   }
#endif

   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (char *)png_malloc_2(png_ptr, length + 1);

   /* WARNING: this may break if size_t is less than 32 bits; it is assumed
    * that the PNG_MAX_MALLOC_64K test is enabled in this case, but this is a
    * potential breakage point if the types in pngconf.h aren't exactly right.
    */
   slength = (png_size_t)length;
   png_crc_read_2(png_ptr, (png_byte *)png_ptr->chunkdata, slength);

   if (png_crc_finish_2(png_ptr, skip))
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00;

   for (entry_start = (png_byte *)png_ptr->chunkdata; *entry_start;
       entry_start++)
      /* Empty loop to find end of name */ ;

   ++entry_start;

   /* A sample depth should follow the separator, and we should be on it  */
   if (entry_start > (png_byte *)png_ptr->chunkdata + slength - 2)
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      //png_warning(png_ptr, "malformed sPLT chunk");
      return;
   }

   new_palette.depth = *entry_start++;
   entry_size = (new_palette.depth == 8 ? 6 : 10);
   /* This must fit in a png_uint_32 because it is derived from the original
    * chunk data length (and use 'length', not 'slength' here for clarity -
    * they are guaranteed to be the same, see the tests above.)
    */
   data_length = length - (png_uint_32)(entry_start -
      (png_byte *)png_ptr->chunkdata);

   /* Integrity-check the data length */
   if (data_length % entry_size)
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      //png_warning(png_ptr, "sPLT chunk has bad length");
      return;
   }

   dl = (png_int_32)(data_length / entry_size);
   max_dl = PNG_SIZE_MAX / png_sizeof(png_sPLT_entry);

   if (dl > max_dl)
   {
       //png_warning(png_ptr, "sPLT chunk too long");
       return;
   }

   new_palette.nentries = (png_int_32)(data_length / entry_size);

   new_palette.entries = (png_sPLT_entryp)png_malloc_warn_2(
       png_ptr, new_palette.nentries * png_sizeof(png_sPLT_entry));

   if (new_palette.entries == NULL)
   {
       //png_warning(png_ptr, "sPLT chunk requires too much memory");
       return;
   }

#ifdef PNG_POINTER_INDEXING_SUPPORTED
   for (i = 0; i < new_palette.nentries; i++)
   {
      pp = new_palette.entries + i;

      if (new_palette.depth == 8)
      {
         pp->red = *entry_start++;
         pp->green = *entry_start++;
         pp->blue = *entry_start++;
         pp->alpha = *entry_start++;
      }

      else
      {
         pp->red   = png_get_uint_16_2(entry_start); entry_start += 2;
         pp->green = png_get_uint_16_2(entry_start); entry_start += 2;
         pp->blue  = png_get_uint_16_2(entry_start); entry_start += 2;
         pp->alpha = png_get_uint_16_2(entry_start); entry_start += 2;
      }

      pp->frequency = png_get_uint_16_2(entry_start); entry_start += 2;
   }
#else
   pp = new_palette.entries;

   for (i = 0; i < new_palette.nentries; i++)
   {

      if (new_palette.depth == 8)
      {
         pp[i].red   = *entry_start++;
         pp[i].green = *entry_start++;
         pp[i].blue  = *entry_start++;
         pp[i].alpha = *entry_start++;
      }

      else
      {
         pp[i].red   = png_get_uint_16_2(entry_start); entry_start += 2;
         pp[i].green = png_get_uint_16_2(entry_start); entry_start += 2;
         pp[i].blue  = png_get_uint_16_2(entry_start); entry_start += 2;
         pp[i].alpha = png_get_uint_16_2(entry_start); entry_start += 2;
      }

      pp->frequency = png_get_uint_16_2(entry_start); entry_start += 2;
   }
#endif

   /* Discard all chunk data except the name and stash that */
   new_palette.name = png_ptr->chunkdata;

   png_set_sPLT_2(png_ptr, info_ptr, &new_palette, 1);

   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
   png_free_2(png_ptr, new_palette.entries);
}
#endif /* PNG_READ_sPLT_SUPPORTED */

#ifdef PNG_READ_tRNS_SUPPORTED
void /* PRIVATE */
png_handle_tRNS_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_byte readbuf[PNG_MAX_PALETTE_LENGTH];
   png_byte buf[6];
   //png_debug(1, "in png_handle_tRNS");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before tRNS");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid tRNS after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS))
   {
      //png_warning(png_ptr, "Duplicate tRNS chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (png_ptr->color_type == PNG_COLOR_TYPE_GRAY)
   {
      png_byte buf[2];

      if (length != 2)
      {
         //png_warning(png_ptr, "Incorrect tRNS chunk length");
         png_crc_finish_2(png_ptr, length);
         return;
      }

      png_crc_read_2(png_ptr, buf, 2);
      png_ptr->num_trans = 1;
      png_ptr->trans_color.gray = png_get_uint_16_2(buf);
   }

   else if (png_ptr->color_type == PNG_COLOR_TYPE_RGB)
   {
      
      if (length != 6)
      {
         //png_warning(png_ptr, "Incorrect tRNS chunk length");
         png_crc_finish_2(png_ptr, length);
         return;
      }

      png_crc_read_2(png_ptr, buf, (png_size_t)length);
      png_ptr->num_trans = 1;
      png_ptr->trans_color.red = png_get_uint_16_2(buf);
      png_ptr->trans_color.green = png_get_uint_16_2(buf + 2);
      png_ptr->trans_color.blue = png_get_uint_16_2(buf + 4);
   }

   else if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (!(png_ptr->mode & PNG_HAVE_PLTE))
      {
         /* Should be an error, but we can cope with it. */
         //png_warning(png_ptr, "Missing PLTE before tRNS");
         length = length;
      }

      if (length > (png_uint_32)png_ptr->num_palette ||
          length > PNG_MAX_PALETTE_LENGTH)
      {
         //png_warning(png_ptr, "Incorrect tRNS chunk length");
         png_crc_finish_2(png_ptr, length);
         return;
      }

      if (length == 0)
      {
         //png_warning(png_ptr, "Zero length tRNS chunk");
         png_crc_finish_2(png_ptr, length);
         return;
      }

      png_crc_read_2(png_ptr, readbuf, (png_size_t)length);
      png_ptr->num_trans = (png_uint_16)length;
   }

   else
   {
      //png_warning(png_ptr, "tRNS chunk not allowed with alpha channel");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (png_crc_finish_2(png_ptr, 0))
   {
      png_ptr->num_trans = 0;
      return;
   }

   png_set_tRNS_2(png_ptr, info_ptr, readbuf, png_ptr->num_trans,
       &(png_ptr->trans_color));
}
#endif

#ifdef PNG_READ_bKGD_SUPPORTED
void /* PRIVATE */
png_handle_bKGD_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_size_t truelen;
   png_byte buf[6];

   //png_debug(1, "in png_handle_bKGD");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
      //png_error(png_ptr, "Missing IHDR before bKGD");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid bKGD after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
       !(png_ptr->mode & PNG_HAVE_PLTE))
   {
      //png_warning(png_ptr, "Missing PLTE before bKGD");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_bKGD))
   {
      //png_warning(png_ptr, "Duplicate bKGD chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      truelen = 1;

   else if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
      truelen = 6;

   else
      truelen = 2;

   if (length != truelen)
   {
      //png_warning(png_ptr, "Incorrect bKGD chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, truelen);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   /* We convert the index value into RGB components so that we can allow
    * arbitrary RGB values for background when we have transparency, and
    * so it is easy to determine the RGB values of the background color
    * from the info_ptr struct.
    */
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      png_ptr->background.index = buf[0];

      if (info_ptr && info_ptr->num_palette)
      {
         if (buf[0] >= info_ptr->num_palette)
         {
            //png_warning(png_ptr, "Incorrect bKGD chunk index value");
            return;
         }

         png_ptr->background.red =
             (png_uint_16)png_ptr->palette[buf[0]].red;

         png_ptr->background.green =
             (png_uint_16)png_ptr->palette[buf[0]].green;

         png_ptr->background.blue =
             (png_uint_16)png_ptr->palette[buf[0]].blue;
      }
   }

   else if (!(png_ptr->color_type & PNG_COLOR_MASK_COLOR)) /* GRAY */
   {
      png_ptr->background.red =
      png_ptr->background.green =
      png_ptr->background.blue =
      png_ptr->background.gray = png_get_uint_16_2(buf);
   }

   else
   {
      png_ptr->background.red = png_get_uint_16_2(buf);
      png_ptr->background.green = png_get_uint_16_2(buf + 2);
      png_ptr->background.blue = png_get_uint_16_2(buf + 4);
   }

   png_set_bKGD_2(png_ptr, info_ptr, &(png_ptr->background));
}
#endif

#ifdef PNG_READ_hIST_SUPPORTED
void /* PRIVATE */
png_handle_hIST_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   unsigned int num, i;
   png_uint_16 readbuf[PNG_MAX_PALETTE_LENGTH];
   png_byte buf[2];
   //png_debug(1, "in png_handle_hIST");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before hIST");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid hIST after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (!(png_ptr->mode & PNG_HAVE_PLTE))
   {
      //png_warning(png_ptr, "Missing PLTE before hIST");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_hIST))
   {
      //png_warning(png_ptr, "Duplicate hIST chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   num = length / 2 ;

   if (num != (unsigned int)png_ptr->num_palette || num >
       (unsigned int)PNG_MAX_PALETTE_LENGTH)
   {
      //png_warning(png_ptr, "Incorrect hIST chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   for (i = 0; i < num; i++)
   {
      

      png_crc_read_2(png_ptr, buf, 2);
      readbuf[i] = png_get_uint_16_2(buf);
   }

   if (png_crc_finish_2(png_ptr, 0))
      return;

   png_set_hIST_2(png_ptr, info_ptr, readbuf);
}
#endif

#ifdef PNG_READ_pHYs_SUPPORTED
void /* PRIVATE */
png_handle_pHYs_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_byte buf[9];
   png_uint_32 res_x, res_y;
   int unit_type;

   //png_debug(1, "in png_handle_pHYs");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before pHYs");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid pHYs after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      //png_warning(png_ptr, "Duplicate pHYs chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (length != 9)
   {
      //png_warning(png_ptr, "Incorrect pHYs chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, 9);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   res_x = png_get_uint_32_2(buf);
   res_y = png_get_uint_32_2(buf + 4);
   unit_type = buf[8];
   png_set_pHYs_2(png_ptr, info_ptr, res_x, res_y, unit_type);
}
#endif

#ifdef PNG_READ_oFFs_SUPPORTED
void /* PRIVATE */
png_handle_oFFs_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_byte buf[9];
   png_int_32 offset_x, offset_y;
   int unit_type;

   //png_debug(1, "in png_handle_oFFs");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before oFFs");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid oFFs after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      //png_warning(png_ptr, "Duplicate oFFs chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (length != 9)
   {
      //png_warning(png_ptr, "Incorrect oFFs chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, 9);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   offset_x = png_get_int_32_2(buf);
   offset_y = png_get_int_32_2(buf + 4);
   unit_type = buf[8];
   png_set_oFFs_2(png_ptr, info_ptr, offset_x, offset_y, unit_type);
}
#endif

#ifdef PNG_READ_pCAL_SUPPORTED
/* Read the pCAL chunk (described in the PNG Extensions document) */
void /* PRIVATE */
png_handle_pCAL_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_int_32 X0, X1;
   png_byte type, nparams;
   char * buf;
   char * units;
   char * endptr;
   char ** params;
   png_size_t slength;
   int i;

   //png_debug(1, "in png_handle_pCAL");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before pCAL");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid pCAL after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_pCAL))
   {
      //png_warning(png_ptr, "Duplicate pCAL chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   //png_debug1(2, "Allocating and reading pCAL chunk data (%u bytes)",
   //    length + 1);
   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (char *)png_malloc_warn_2(png_ptr, length + 1);

   if (png_ptr->chunkdata == NULL)
   {
      //png_warning(png_ptr, "No memory for pCAL purpose");
      return;
   }

   slength = (png_size_t)length;
   png_crc_read_2(png_ptr, (png_byte *)png_ptr->chunkdata, slength);

   if (png_crc_finish_2(png_ptr, 0))
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00; /* Null terminate the last string */

   //png_debug(3, "Finding end of pCAL purpose string");
   for (buf = png_ptr->chunkdata; *buf; buf++)
      /* Empty loop */ ;

   endptr = png_ptr->chunkdata + slength;

   /* We need to have at least 12 bytes after the purpose string
    * in order to get the parameter information.
    */
   if (endptr <= buf + 12)
   {
      //png_warning(png_ptr, "Invalid pCAL data");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_debug(3, "Reading pCAL X0, X1, type, nparams, and units");
   X0 = png_get_int_32_2((png_byte *)buf+1);
   X1 = png_get_int_32_2((png_byte *)buf+5);
   type = buf[9];
   nparams = buf[10];
   units = buf + 11;

   //png_debug(3, "Checking pCAL equation type and number of parameters");
   /* Check that we have the right number of parameters for known
    * equation types.
    */
   if ((type == PNG_EQUATION_LINEAR && nparams != 2) ||
       (type == PNG_EQUATION_BASE_E && nparams != 3) ||
       (type == PNG_EQUATION_ARBITRARY && nparams != 3) ||
       (type == PNG_EQUATION_HYPERBOLIC && nparams != 4))
   {
      //png_warning(png_ptr, "Invalid pCAL parameters for equation type");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   else if (type >= PNG_EQUATION_LAST)
   {
      //png_warning(png_ptr, "Unrecognized equation type for pCAL chunk");
   }

   for (buf = units; *buf; buf++)
      /* Empty loop to move past the units string. */ ;

   //png_debug(3, "Allocating pCAL parameters array");

   params = (char **)png_malloc_warn_2(png_ptr,
       (png_size_t)(nparams * png_sizeof(char *)));

   if (params == NULL)
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      //png_warning(png_ptr, "No memory for pCAL params");
      return;
   }

   /* Get pointers to the start of each parameter string. */
   for (i = 0; i < (int)nparams; i++)
   {
      buf++; /* Skip the null string terminator from previous parameter. */

      //png_debug1(3, "Reading pCAL parameter %d", i);

      for (params[i] = buf; buf <= endptr && *buf != 0x00; buf++)
         /* Empty loop to move past each parameter string */ ;

      /* Make sure we haven't run out of data yet */
      if (buf > endptr)
      {
         //png_warning(png_ptr, "Invalid pCAL data");
         png_free_2(png_ptr, png_ptr->chunkdata);
         png_ptr->chunkdata = NULL;
         png_free_2(png_ptr, params);
         return;
      }
   }

   png_set_pCAL_2(png_ptr, info_ptr, png_ptr->chunkdata, X0, X1, type, nparams,
      units, params);

   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
   png_free_2(png_ptr, params);
}
#endif

#ifdef PNG_READ_sCAL_SUPPORTED
/* Read the sCAL chunk */
void /* PRIVATE */
png_handle_sCAL_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_size_t slength, i;
   int state;

   //png_debug(1, "in png_handle_sCAL");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before sCAL");

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      //png_warning(png_ptr, "Invalid sCAL after IDAT");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sCAL))
   {
      //png_warning(png_ptr, "Duplicate sCAL chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   //png_debug1(2, "Allocating and reading sCAL chunk data (%u bytes)",
   //   length + 1);

   png_ptr->chunkdata = (char *)png_malloc_warn_2(png_ptr, length + 1);

   if (png_ptr->chunkdata == NULL)
   {
      //png_warning(png_ptr, "Out of memory while processing sCAL chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   slength = (png_size_t)length;
   png_crc_read_2(png_ptr, (png_byte *)png_ptr->chunkdata, slength);
   png_ptr->chunkdata[slength] = 0x00; /* Null terminate the last string */

   if (png_crc_finish_2(png_ptr, 0))
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   /* Validate the unit. */
   if (png_ptr->chunkdata[0] != 1 && png_ptr->chunkdata[0] != 2)
   {
      //png_warning(png_ptr, "Invalid sCAL ignored: invalid unit");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   /* Validate the ASCII numbers, need two ASCII numbers separated by
    * a '\0' and they need to fit exactly in the chunk data.
    */
   i = 0;
   state = 0;

   if (png_ptr->chunkdata[1] == 45 /* negative width */ ||
       !png_check_fp_number_2(png_ptr->chunkdata, slength, &state, &i) ||
       i >= slength || png_ptr->chunkdata[i++] != 0) {
      //png_warning(png_ptr, "Invalid sCAL chunk ignored: bad width format");
   		i=i;
   }
   else
   {
      png_size_t heighti = i;

      if (png_ptr->chunkdata[i] == 45 /* negative height */ ||
          !png_check_fp_number_2(png_ptr->chunkdata, slength, &state, &i) ||
          i != slength) {
         //png_warning(png_ptr, "Invalid sCAL chunk ignored: bad height format");
	  	i=i;
	  }
      else
         /* This is the (only) success case. */
         png_set_sCAL_s_2(png_ptr, info_ptr, png_ptr->chunkdata[0],
            png_ptr->chunkdata+1, png_ptr->chunkdata+heighti);
   }

   /* Clean up - just free the temporarily allocated buffer. */
   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
}
#endif

#ifdef PNG_READ_tIME_SUPPORTED
void /* PRIVATE */
png_handle_tIME_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_byte buf[7];
   png_time mod_time;

   //png_debug(1, "in png_handle_tIME");

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Out of place tIME chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tIME))
   {
      //png_warning(png_ptr, "Duplicate tIME chunk");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   if (png_ptr->mode & PNG_HAVE_IDAT)
      png_ptr->mode |= PNG_AFTER_IDAT;

   if (length != 7)
   {
      //png_warning(png_ptr, "Incorrect tIME chunk length");
      png_crc_finish_2(png_ptr, length);
      return;
   }

   png_crc_read_2(png_ptr, buf, 7);

   if (png_crc_finish_2(png_ptr, 0))
      return;

   mod_time.second = buf[6];
   mod_time.minute = buf[5];
   mod_time.hour = buf[4];
   mod_time.day = buf[3];
   mod_time.month = buf[2];
   mod_time.year = png_get_uint_16_2(buf);

   png_set_tIME_2(png_ptr, info_ptr, &mod_time);
}
#endif

#ifdef PNG_READ_tEXt_SUPPORTED
/* Note: this does not properly handle chunks that are > 64K under DOS */
void /* PRIVATE */
png_handle_tEXt_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_text * text_ptr;
   char * key;
   char * text;
   png_uint_32 skip = 0;
   png_size_t slength;
   int ret;

   //png_debug(1, "in png_handle_tEXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (png_ptr->user_chunk_cache_max != 0)
   {
      if (png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish_2(png_ptr, length);
         return;
      }

      if (--png_ptr->user_chunk_cache_max == 1)
      {
         //png_warning(png_ptr, "No space in chunk cache for tEXt");
         png_crc_finish_2(png_ptr, length);
         return;
      }
   }
#endif

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before tEXt");

   if (png_ptr->mode & PNG_HAVE_IDAT)
      png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
   if (length > (png_uint_32)65535L)
   {
      //png_warning(png_ptr, "tEXt chunk too large to fit in memory");
      skip = length - (png_uint_32)65535L;
      length = (png_uint_32)65535L;
   }
#endif

   png_free_2(png_ptr, png_ptr->chunkdata);

   png_ptr->chunkdata = (char *)png_malloc_warn_2(png_ptr, length + 1);

   if (png_ptr->chunkdata == NULL)
   {
     //png_warning(png_ptr, "No memory to process text chunk");
     return;
   }

   slength = (png_size_t)length;
   png_crc_read_2(png_ptr, (png_byte *)png_ptr->chunkdata, slength);

   if (png_crc_finish_2(png_ptr, skip))
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   key = png_ptr->chunkdata;

   key[slength] = 0x00;

   for (text = key; *text; text++)
      /* Empty loop to find end of key */ ;

   if (text != key + slength)
      text++;

   text_ptr = (png_textp)png_malloc_warn_2(png_ptr,
       png_sizeof(png_text));

   if (text_ptr == NULL)
   {
      //png_warning(png_ptr, "Not enough memory to process text chunk");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   text_ptr->compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr->key = key;
   text_ptr->lang = NULL;
   text_ptr->lang_key = NULL;
   text_ptr->itxt_length = 0;
   text_ptr->text = text;
   text_ptr->text_length = strlen(text);

   ret = png_set_text_3(png_ptr, info_ptr, text_ptr, 1);

   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
   png_free_2(png_ptr, text_ptr);

   //if (ret)
   //   png_warning(png_ptr, "Insufficient memory to process text chunk");
}
#endif

#ifdef PNG_READ_zTXt_SUPPORTED
/* Note: this does not correctly handle chunks that are > 64K under DOS */
void /* PRIVATE */
png_handle_zTXt_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_text * text_ptr;
   char * text;
   int comp_type;
   int ret;
   png_size_t slength, prefix_len, data_len;

   //png_debug(1, "in png_handle_zTXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (png_ptr->user_chunk_cache_max != 0)
   {
      if (png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish_2(png_ptr, length);
         return;
      }

      if (--png_ptr->user_chunk_cache_max == 1)
      {
         //png_warning(png_ptr, "No space in chunk cache for zTXt");
         png_crc_finish_2(png_ptr, length);
         return;
      }
   }
#endif

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before zTXt");

   if (png_ptr->mode & PNG_HAVE_IDAT)
      png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
   /* We will no doubt have problems with chunks even half this size, but
    * there is no hard and fast rule to tell us where to stop.
    */
   if (length > (png_uint_32)65535L)
   {
      //png_warning(png_ptr, "zTXt chunk too large to fit in memory");
      png_crc_finish_2(png_ptr, length);
      return;
   }
#endif

   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (char *)png_malloc_warn_2(png_ptr, length + 1);

   if (png_ptr->chunkdata == NULL)
   {
      //png_warning(png_ptr, "Out of memory processing zTXt chunk");
      return;
   }

   slength = (png_size_t)length;
   png_crc_read_2(png_ptr, (png_byte *)png_ptr->chunkdata, slength);

   if (png_crc_finish_2(png_ptr, 0))
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00;

   for (text = png_ptr->chunkdata; *text; text++)
      /* Empty loop */ ;

   /* zTXt must have some text after the chunkdataword */
   if (text >= png_ptr->chunkdata + slength - 2)
   {
      //png_warning(png_ptr, "Truncated zTXt chunk");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   else
   {
       comp_type = *(++text);

       if (comp_type != PNG_TEXT_COMPRESSION_zTXt)
       {
          //png_warning(png_ptr, "Unknown compression type in zTXt chunk");
          comp_type = PNG_TEXT_COMPRESSION_zTXt;
       }

       text++;        /* Skip the compression_method byte */
   }

   prefix_len = text - png_ptr->chunkdata;

   png_decompress_chunk_2(png_ptr, comp_type,
       (png_size_t)length, prefix_len, &data_len);

   text_ptr = (png_text *)png_malloc_warn_2(png_ptr,
       png_sizeof(png_text));

   if (text_ptr == NULL)
   {
      //png_warning(png_ptr, "Not enough memory to process zTXt chunk");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   text_ptr->compression = comp_type;
   text_ptr->key = png_ptr->chunkdata;
   text_ptr->lang = NULL;
   text_ptr->lang_key = NULL;
   text_ptr->itxt_length = 0;
   text_ptr->text = png_ptr->chunkdata + prefix_len;
   text_ptr->text_length = data_len;

   ret = png_set_text_3(png_ptr, info_ptr, text_ptr, 1);

   png_free_2(png_ptr, text_ptr);
   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;

   //if (ret)
      //png_error(png_ptr, "Insufficient memory to store zTXt chunk");
}
#endif

#ifdef PNG_READ_iTXt_SUPPORTED
/* Note: this does not correctly handle chunks that are > 64K under DOS */
void /* PRIVATE */
png_handle_iTXt_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_text * text_ptr;
   char * key;
   char * lang;
   char * text;
   char * lang_key;
   int comp_flag;
   int comp_type = 0;
   int ret;
   png_size_t slength, prefix_len, data_len;

   //png_debug(1, "in png_handle_iTXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (png_ptr->user_chunk_cache_max != 0)
   {
      if (png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish_2(png_ptr, length);
         return;
      }

      if (--png_ptr->user_chunk_cache_max == 1)
      {
         //png_warning(png_ptr, "No space in chunk cache for iTXt");
         png_crc_finish_2(png_ptr, length);
         return;
      }
   }
#endif

   //if (!(png_ptr->mode & PNG_HAVE_IHDR))
   //   png_error(png_ptr, "Missing IHDR before iTXt");

   if (png_ptr->mode & PNG_HAVE_IDAT)
      png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
   /* We will no doubt have problems with chunks even half this size, but
    * there is no hard and fast rule to tell us where to stop.
    */
   if (length > (png_uint_32)65535L)
   {
      //png_warning(png_ptr, "iTXt chunk too large to fit in memory");
      png_crc_finish_2(png_ptr, length);
      return;
   }
#endif

   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (char *)png_malloc_warn_2(png_ptr, length + 1);

   if (png_ptr->chunkdata == NULL)
   {
      //png_warning(png_ptr, "No memory to process iTXt chunk");
      return;
   }

   slength = (png_size_t)length;
   png_crc_read_2(png_ptr, (png_byte *)png_ptr->chunkdata, slength);

   if (png_crc_finish_2(png_ptr, 0))
   {
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00;

   for (lang = png_ptr->chunkdata; *lang; lang++)
      /* Empty loop */ ;

   lang++;        /* Skip NUL separator */

   /* iTXt must have a language tag (possibly empty), two compression bytes,
    * translated keyword (possibly empty), and possibly some text after the
    * keyword
    */

   if (lang >= png_ptr->chunkdata + slength - 3)
   {
      //png_warning(png_ptr, "Truncated iTXt chunk");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   else
   {
      comp_flag = *lang++;
      comp_type = *lang++;
   }

   for (lang_key = lang; *lang_key; lang_key++)
      /* Empty loop */ ;

   lang_key++;        /* Skip NUL separator */

   if (lang_key >= png_ptr->chunkdata + slength)
   {
      //png_warning(png_ptr, "Truncated iTXt chunk");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   for (text = lang_key; *text; text++)
      /* Empty loop */ ;

   text++;        /* Skip NUL separator */

   if (text >= png_ptr->chunkdata + slength)
   {
      //png_warning(png_ptr, "Malformed iTXt chunk");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   prefix_len = text - png_ptr->chunkdata;

   key=png_ptr->chunkdata;

   if (comp_flag)
      png_decompress_chunk_2(png_ptr, comp_type,
          (size_t)length, prefix_len, &data_len);

   else
      data_len = strlen(png_ptr->chunkdata + prefix_len);

   text_ptr = (png_text *)png_malloc_warn_2(png_ptr,
       png_sizeof(png_text));

   if (text_ptr == NULL)
   {
      //png_warning(png_ptr, "Not enough memory to process iTXt chunk");
      png_free_2(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   text_ptr->compression = (int)comp_flag + 1;
   text_ptr->lang_key = png_ptr->chunkdata + (lang_key - key);
   text_ptr->lang = png_ptr->chunkdata + (lang - key);
   text_ptr->itxt_length = data_len;
   text_ptr->text_length = 0;
   text_ptr->key = png_ptr->chunkdata;
   text_ptr->text = png_ptr->chunkdata + prefix_len;

   ret = png_set_text_3(png_ptr, info_ptr, text_ptr, 1);

   png_free_2(png_ptr, text_ptr);
   png_free_2(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;

   //if (ret)
      //png_error(png_ptr, "Insufficient memory to store iTXt chunk");
}
#endif

/* This function is called when we haven't found a handler for a
 * chunk.  If there isn't a problem with the chunk itself (ie bad
 * chunk name, CRC, or a critical chunk), the chunk is silently ignored
 * -- unless the PNG_FLAG_UNKNOWN_CHUNKS_SUPPORTED flag is on in which
 * case it will be saved away to be written out later.
 */
void /* PRIVATE */
png_handle_unknown_2(png_struct * png_ptr, png_info * info_ptr, png_uint_32 length)
{
   png_uint_32 skip = 0;
   int ret;
   //png_debug(1, "in png_handle_unknown");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (png_ptr->user_chunk_cache_max != 0)
   {
      if (png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish_2(png_ptr, length);
         return;
      }

      if (--png_ptr->user_chunk_cache_max == 1)
      {
         //png_warning(png_ptr, "No space in chunk cache for unknown chunk");
         png_crc_finish_2(png_ptr, length);
         return;
      }
   }
#endif

   if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      if (memcmp(png_ptr->chunk_name, png_IDAT, 4))  /* Not an IDAT */
         png_ptr->mode |= PNG_AFTER_IDAT;
   }

   if (!(png_ptr->chunk_name[0] & 0x20))
   {
#ifdef png_handle_as_unknown_2_SUPPORTED
      if (png_handle_as_unknown_2(png_ptr, png_ptr->chunk_name) !=
          PNG_HANDLE_CHUNK_ALWAYS
#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
          && png_ptr->read_user_chunk_fn == NULL
#endif
          )
#endif
         png_chunk_error_2(png_ptr, "unknown critical chunk");
   }

#ifdef PNG_READ_UNKNOWN_CHUNKS_SUPPORTED
   if ((png_ptr->flags & PNG_FLAG_KEEP_UNKNOWN_CHUNKS)
#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
       || (png_ptr->read_user_chunk_fn != NULL)
#endif
       )
   {
#ifdef PNG_MAX_MALLOC_64K
      if (length > (png_uint_32)65535L)
      {
         //png_warning(png_ptr, "unknown chunk too large to fit in memory");
         skip = length - (png_uint_32)65535L;
         length = (png_uint_32)65535L;
      }
#endif

      memcpy((char *)png_ptr->unknown_chunk.name,
          (char *)png_ptr->chunk_name,
          png_sizeof(png_ptr->unknown_chunk.name));

      png_ptr->unknown_chunk.name[png_sizeof(png_ptr->unknown_chunk.name)-1]
          = '\0';

      png_ptr->unknown_chunk.size = (png_size_t)length;

      if (length == 0)
         png_ptr->unknown_chunk.data = NULL;

      else
      {
         png_ptr->unknown_chunk.data = (png_bytep)png_malloc_2(png_ptr, length);
         png_crc_read_2(png_ptr, (png_bytep)png_ptr->unknown_chunk.data, length);
      }

#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
      if (png_ptr->read_user_chunk_fn != NULL)
      {
         /* Callback to user unknown chunk handler */
         

         ret = (*(png_ptr->read_user_chunk_fn))
             (png_ptr, &png_ptr->unknown_chunk);

         if (ret < 0)
            png_chunk_error_2(png_ptr, "error in user chunk");

         if (ret == 0)
         {
            if (!(png_ptr->chunk_name[0] & 0x20))
            {
#ifdef png_handle_as_unknown_2_SUPPORTED
               if (png_handle_as_unknown_2(png_ptr, png_ptr->chunk_name) !=
                   PNG_HANDLE_CHUNK_ALWAYS)
#endif
                  png_chunk_error_2(png_ptr, "unknown critical chunk");
            }

            png_set_unknown_chunks_2(png_ptr, info_ptr,
                &png_ptr->unknown_chunk, 1);
         }
      }

      else
#endif
         png_set_unknown_chunks_2(png_ptr, info_ptr, &png_ptr->unknown_chunk, 1);

      png_free_2(png_ptr, png_ptr->unknown_chunk.data);
      png_ptr->unknown_chunk.data = NULL;
   }

   else
#endif
      skip = length;

   png_crc_finish_2(png_ptr, skip);

#ifndef PNG_READ_USER_CHUNKS_SUPPORTED
   PNG_UNUSED(info_ptr) /* Quiet compiler warnings about unused info_ptr */
#endif
}

/* Read and check the PNG file signature */
void /* PRIVATE */
png_read_sig_2(png_struct * png_ptr, png_info * info_ptr)
{
   png_size_t num_checked, num_to_check;

   /* Exit if the user application does not expect a signature. */
   if (png_ptr->sig_bytes >= 8)
      return;

   num_checked = png_ptr->sig_bytes;
   num_to_check = 8 - num_checked;

#ifdef PNG_IO_STATE_SUPPORTED
   png_ptr->io_state = PNG_IO_READING | PNG_IO_SIGNATURE;
#endif

   /* The signature must be serialized in a single I/O call. */
   png_read_data_2(png_ptr, &(info_ptr->signature[num_checked]), num_to_check);
   png_ptr->sig_bytes = 8;

   if (png_sig_cmp_2(info_ptr->signature, num_checked, num_to_check))
   {
      //if (num_checked < 4 &&
      //    png_sig_cmp_2(info_ptr->signature, num_checked, num_to_check - 4))
         //png_error(png_ptr, "Not a PNG file");
      //else
         //png_error(png_ptr, "PNG file corrupted by ASCII conversion");
   }
   if (num_checked < 3)
      png_ptr->mode |= PNG_HAVE_PNG_SIGNATURE;
}

/* Read the information before the actual image data.  This has been
 * changed in v0.90 to allow reading a file that already has the magic
 * bytes read from the stream.  You can tell libpng how many bytes have
 * been read from the beginning of the stream (up to the maximum of 8)
 * via png_set_sig_bytes(), and we will only check the remaining bytes
 * here.  The application can then have access to the signature bytes we
 * read if it is determined that this isn't a valid PNG file.
 */
void png_read_info_2(png_struct * png_ptr, png_info * info_ptr)
{
   //png_debug(1, "in png_read_info");
   png_uint_32 length;
   png_byte * chunk_name;
   if (png_ptr == NULL || info_ptr == NULL)
      return;

   /* Read and check the PNG file signature. */
   png_read_sig_2(png_ptr, info_ptr);
   png_warning_2(png_ptr, "signature checked");
   for (;;)
   {
      png_warning_2(png_ptr, "read chunk header");
      length = png_read_chunk_header_2(png_ptr);
      chunk_name = png_ptr->chunk_name;
      png_warning_2(png_ptr, "chunk header readed");
      /* This should be a binary subdivision search or a hash for
       * matching the chunk name rather than a linear search.
       */
      if (memcmp(chunk_name, png_IDAT, 4) == 0) {
         png_warning_2(png_ptr, "handle IDAT");
         if (png_ptr->mode & PNG_AFTER_IDAT)
            png_ptr->mode |= PNG_HAVE_CHUNK_AFTER_IDAT;
      }
      if (memcmp(chunk_name, png_IHDR, 4) == 0) {
         png_warning_2(png_ptr, "handle IHDR");
         png_handle_IHDR_2(png_ptr, info_ptr, length);
      }
      else if (memcmp(chunk_name, png_IEND, 4) == 0) {
         png_warning_2(png_ptr, "handle IEND");
         png_handle_IEND_2(png_ptr, info_ptr, length);
      }
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
      else if (png_handle_as_unknown_2(png_ptr, chunk_name))
      {
         if (memcmp(chunk_name, png_IDAT, 4) == 0)
            png_ptr->mode |= PNG_HAVE_IDAT;

         png_handle_unknown_2(png_ptr, info_ptr, length);

         if (memcmp(chunk_name, png_PLTE, 4) == 0)
            png_ptr->mode |= PNG_HAVE_PLTE;

         else if (memcmp(chunk_name, png_IDAT, 4) == 0)
         {
            if ((png_ptr->mode & PNG_HAVE_IHDR) == PNG_HAVE_IHDR) {
             	png_warning_2(png_ptr, "Missing IHDR before IDAT");
			}
            else if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
                (png_ptr->mode & PNG_HAVE_PLTE) == PNG_HAVE_PLTE) {
               png_warning_2(png_ptr, "Missing PLTE before IDAT");
            }
            break;
         }
      }
#endif
      else if (memcmp(chunk_name, png_PLTE, 4) == 0) {
         png_warning_2(png_ptr, "handle PLTE");
         png_handle_PLTE_2(png_ptr, info_ptr, length);
	  }
      else if (memcmp(chunk_name, png_IDAT, 4) == 0)
      {
         if ((png_ptr->mode & PNG_HAVE_IHDR) == PNG_HAVE_IHDR)
            png_warning_2(png_ptr, "Missing IHDR before IDAT");

         else if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
             (png_ptr->mode & PNG_HAVE_PLTE) == PNG_HAVE_PLTE)
             png_warning_2(png_ptr, "Missing PLTE before IDAT");

         png_ptr->idat_size = length;
         png_ptr->mode |= PNG_HAVE_IDAT;
         break;
      }

#ifdef PNG_READ_bKGD_SUPPORTED
      else if (memcmp(chunk_name, png_bKGD, 4) == 0) {
         png_warning_2(png_ptr, "handle bKGD");
         png_handle_bKGD_2(png_ptr, info_ptr, length);  
      }
#endif
#ifdef PNG_READ_cHRM_SUPPORTED
      else if (memcmp(chunk_name, png_cHRM, 4) == 0) {
         png_warning_2(png_ptr, "handle cHRM");
         png_handle_cHRM_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_gAMA_SUPPORTED
      else if (memcmp(chunk_name, png_gAMA, 4) == 0) {
         png_warning_2(png_ptr, "handle gAMA");
         png_handle_gAMA_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_hIST_SUPPORTED
      else if (memcmp(chunk_name, png_hIST, 4) == 0) {
         png_warning_2(png_ptr, "handle hIST");
         png_handle_hIST_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_oFFs_SUPPORTED
      else if (memcmp(chunk_name, png_oFFs, 4) == 0) {
         png_warning_2(png_ptr, "handle oFFs");
         png_handle_oFFs_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_pCAL_SUPPORTED
      else if (memcmp(chunk_name, png_pCAL, 4) == 0) {
         png_warning_2(png_ptr, "handle pCAL2");
         png_handle_pCAL_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_sCAL_SUPPORTED
      else if (memcmp(chunk_name, png_sCAL, 4) == 0) {
         png_warning_2(png_ptr, "handle sCAL");
         png_handle_sCAL_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_pHYs_SUPPORTED
      else if (memcmp(chunk_name, png_pHYs, 4) == 0) {
         png_warning_2(png_ptr, "handle pHYs");
         png_handle_pHYs_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_sBIT_SUPPORTED
      else if (memcmp(chunk_name, png_sBIT, 4) == 0) {
         png_warning_2(png_ptr, "handle sBIT");
         png_handle_sBIT_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_sRGB_SUPPORTED
      else if (memcmp(chunk_name, png_sRGB, 4) == 0) {
         png_warning_2(png_ptr, "handle sRGB");
         png_handle_sRGB_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_iCCP_SUPPORTED
      else if (memcmp(chunk_name, png_iCCP, 4) == 0) {
         png_warning_2(png_ptr, "handle iCCP");
         png_handle_iCCP_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_sPLT_SUPPORTED
      else if (memcmp(chunk_name, png_sPLT, 4) == 0) {
         png_warning_2(png_ptr, "handle sPLT");
         png_handle_sPLT_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_tEXt_SUPPORTED
      else if (memcmp(chunk_name, png_tEXt, 4) == 0) {
         png_warning_2(png_ptr, "handle tEXt");
         png_handle_tEXt_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_tIME_SUPPORTED
      else if (memcmp(chunk_name, png_tIME, 4) == 0) {
         png_warning_2(png_ptr, "handle tIME");
         png_handle_tIME_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_tRNS_SUPPORTED
      else if (memcmp(chunk_name, png_tRNS, 4) == 0) {
         png_warning_2(png_ptr, "handle tRNS");
         png_handle_tRNS_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_zTXt_SUPPORTED
      else if (memcmp(chunk_name, png_zTXt, 4) == 0) {
         png_warning_2(png_ptr, "handle zTXt");
         png_handle_zTXt_2(png_ptr, info_ptr, length);
      }
#endif
#ifdef PNG_READ_iTXt_SUPPORTED
      else if (memcmp(chunk_name, png_iTXt, 4) == 0) {
         png_warning_2(png_ptr, "handle iTXt");
         png_handle_iTXt_2(png_ptr, info_ptr, length);
      }
#endif
      else {
         png_warning_2(png_ptr, "handle unknown");
         png_handle_unknown_2(png_ptr, info_ptr, length);
      }
   }
}

void png_read_end_2(png_struct * png_ptr, png_info * info_ptr)
{
   png_uint_32 length;
   png_byte * chunk_name;
   //png_debug(1, "in png_read_end");

   if (png_ptr == NULL)
      return;

   png_crc_finish_2(png_ptr, 0); /* Finish off CRC from last IDAT chunk */
   png_warning_2(png_ptr, "read end");
   do
   {
      length = png_read_chunk_header_2(png_ptr);
      chunk_name = png_ptr->chunk_name;

      if (memcmp(chunk_name, png_IHDR, 4) == 0) {
         png_warning_2(png_ptr, "handle IHDR");
         png_handle_IHDR_2(png_ptr, info_ptr, length);
      }
      else if (memcmp(chunk_name, png_IEND, 4) == 0) {
         png_warning_2(png_ptr, "handle IEND");
         png_handle_IEND_2(png_ptr, info_ptr, length);
      }
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
      else if (png_handle_as_unknown_2(png_ptr, chunk_name))
      {
         if (memcmp(chunk_name, png_IDAT, 4) == 0)
         {
            //if ((length > 0) || (png_ptr->mode & PNG_HAVE_CHUNK_AFTER_IDAT))
               //png_benign_error(png_ptr, "Too many IDATs found");
         }
         png_handle_unknown_2(png_ptr, info_ptr, length);
         if (memcmp(chunk_name, png_PLTE, 4) == 0)
            png_ptr->mode |= PNG_HAVE_PLTE;
      }
#endif

      else if (memcmp(chunk_name, png_IDAT, 4) == 0)
      {
         /* Zero length IDATs are legal after the last IDAT has been
          * read, but not after other chunks have been read.
          */
         //if ((length > 0) || (png_ptr->mode & PNG_HAVE_CHUNK_AFTER_IDAT))
         //   png_benign_error(png_ptr, "Too many IDATs found");

         png_crc_finish_2(png_ptr, length);
      }
      else if (memcmp(chunk_name, png_PLTE, 4) == 0) {
         png_warning_2(png_ptr, "handle PLTE");
         png_handle_PLTE_2(png_ptr, info_ptr, length);
	  }
#ifdef PNG_READ_bKGD_SUPPORTED
      else if (memcmp(chunk_name, png_bKGD, 4) == 0) {
         png_warning_2(png_ptr, "handle bKGD");
         png_handle_bKGD_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_cHRM_SUPPORTED
      else if (memcmp(chunk_name, png_cHRM, 4) == 0) {
         png_warning_2(png_ptr, "handle cHRM");
         png_handle_cHRM_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_gAMA_SUPPORTED
      else if (memcmp(chunk_name, png_gAMA, 4) == 0) {
         png_warning_2(png_ptr, "handle gAMA");
         png_handle_gAMA_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_hIST_SUPPORTED
      else if (memcmp(chunk_name, png_hIST, 4) == 0) {
         png_warning_2(png_ptr, "handle hIST");
         png_handle_hIST_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_oFFs_SUPPORTED
      else if (memcmp(chunk_name, png_oFFs, 4) == 0) {
         png_warning_2(png_ptr, "handle oFFs");
         png_handle_oFFs_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_pCAL_SUPPORTED
      else if (memcmp(chunk_name, png_pCAL, 4) == 0) {
         png_warning_2(png_ptr, "handle pCAL");
         png_handle_pCAL_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_sCAL_SUPPORTED
      else if (memcmp(chunk_name, png_sCAL, 4) == 0) {
         png_warning_2(png_ptr, "handle sCAL");
         png_handle_sCAL_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_pHYs_SUPPORTED
      else if (memcmp(chunk_name, png_pHYs, 4) == 0) {
         png_warning_2(png_ptr, "handle pHYs");
         png_handle_pHYs_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_sBIT_SUPPORTED
      else if (memcmp(chunk_name, png_sBIT, 4) == 0) {
         png_warning_2(png_ptr, "handle sBIT");
         png_handle_sBIT_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_sRGB_SUPPORTED
      else if (memcmp(chunk_name, png_sRGB, 4) == 0) {
         png_warning_2(png_ptr, "handle sRGB");
         png_handle_sRGB_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_iCCP_SUPPORTED
      else if (memcmp(chunk_name, png_iCCP, 4) == 0) {
         png_warning_2(png_ptr, "handle iCCP");
         png_handle_iCCP_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_sPLT_SUPPORTED
      else if (memcmp(chunk_name, png_sPLT, 4) == 0) {
         png_warning_2(png_ptr, "handle sPLT");
         png_handle_sPLT_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_tEXt_SUPPORTED
      else if (memcmp(chunk_name, png_tEXt, 4) == 0) {
         png_warning_2(png_ptr, "handle tEXt");
         png_handle_tEXt_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_tIME_SUPPORTED
      else if (memcmp(chunk_name, png_tIME, 4) == 0) {
         png_warning_2(png_ptr, "handle tIME");
         png_handle_tIME_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_tRNS_SUPPORTED
      else if (memcmp(chunk_name, png_tRNS, 4) == 0) {
         png_warning_2(png_ptr, "handle tRNS");
         png_handle_tRNS_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_zTXt_SUPPORTED
      else if (memcmp(chunk_name, png_zTXt, 4) == 0) {
         png_warning_2(png_ptr, "handle zTXt");
         png_handle_zTXt_2(png_ptr, info_ptr, length);
      }
#endif

#ifdef PNG_READ_iTXt_SUPPORTED
      else if (memcmp(chunk_name, png_iTXt, 4) == 0) {
         png_warning_2(png_ptr, "handle iTXt");
         png_handle_iTXt_2(png_ptr, info_ptr, length);
      }
#endif
      else {
         png_warning_2(png_ptr, "handle unknown");
         return;
         png_handle_unknown_2(png_ptr, info_ptr, length);
      }
   } while ((png_ptr->mode & PNG_HAVE_IEND) == PNG_HAVE_IEND);
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
#ifdef PNG_INFO_IMAGE_SUPPORTED

/* Easy access to info, added in libpng-0.99 */
png_uint_32 png_get_image_width_2(png_struct * png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->width;
   return (0);
}

png_uint_32 png_get_image_height_2(png_structp png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->height;
   return (0);
}

png_byte png_get_bit_depth_2(png_struct * png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->bit_depth;
   return (0);
}

png_byte png_get_color_type_2(png_struct * png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->color_type;
   return (0);
}

png_byte png_get_filter_type_2(png_struct * png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->filter_type;
   return (0);
}

png_byte png_get_interlace_type_2(png_struct * png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->interlace_type;
   return (0);
}

png_byte png_get_compression_type_2(png_struct * png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return info_ptr->compression_type;
   return (0);
}

png_uint_32 png_get_x_pixels_per_meter_2(png_struct * png_ptr, png_info * info_ptr)
{
#ifdef PNG_pHYs_SUPPORTED
   if (png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
      {
         //png_debug1(1, "in %s retrieval function",
         //    "png_get_x_pixels_per_meter");

         if (info_ptr->phys_unit_type == PNG_RESOLUTION_METER)
            return (info_ptr->x_pixels_per_unit);
      }
#endif

   return (0);
}

png_uint_32 png_get_y_pixels_per_meter_2(png_struct * png_ptr, png_info * info_ptr)
{
#ifdef PNG_pHYs_SUPPORTED
   if (png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      //png_debug1(1, "in %s retrieval function",
          //"png_get_y_pixels_per_meter");

      if (info_ptr->phys_unit_type == PNG_RESOLUTION_METER)
         return (info_ptr->y_pixels_per_unit);
   }
#endif
   return (0);
}

#endif
#endif

/* Combines the row recently read in with the existing pixels in the
 * row.  This routine takes care of alpha and transparency if requested.
 * This routine also handles the two methods of progressive display
 * of interlaced images, depending on the mask value.
 * The mask value describes which pixels are to be combined with
 * the row.  The pattern always repeats every 8 pixels, so just 8
 * bits are needed.  A one indicates the pixel is to be combined,
 * a zero indicates the pixel is to be skipped.  This is in addition
 * to any alpha or transparency value associated with the pixel.  If
 * you want all pixels to be combined, pass 0xff (255) in mask.
 */
void /* PRIVATE */
png_combine_row_2(png_struct * png_ptr, png_byte * row, int mask)
{
   png_byte * sp;
   png_byte * dp;
   int s_inc, s_start, s_end;
   int m;
   int shift;
   png_uint_32 i;
   png_uint_32 row_width;
   png_size_t pixel_bytes;
   int value;
   //png_debug(1, "in png_combine_row_2");

   if (mask == 0xff)
   {
      memcpy(row, png_ptr->row_buf + 1,
          PNG_ROWBYTES(png_ptr->row_info.pixel_depth, png_ptr->width));
   }

   else
   {
      switch (png_ptr->row_info.pixel_depth)
      {
         case 1:
         {
            sp = png_ptr->row_buf + 1;
            dp = row;
            m = 0x80;
            row_width = png_ptr->width;

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (png_ptr->transformations & PNG_PACKSWAP)
            {
                s_start = 0;
                s_end = 7;
                s_inc = 1;
            }

            else
#endif
            {
                s_start = 7;
                s_end = 0;
                s_inc = -1;
            }

            shift = s_start;

            for (i = 0; i < row_width; i++)
            {
               if (m & mask)
               {
                  int value;

                  value = (*sp >> shift) & 0x01;
                  *dp &= (png_byte)((0x7f7f >> (7 - shift)) & 0xff);
                  *dp |= (png_byte)(value << shift);
               }

               if (shift == s_end)
               {
                  shift = s_start;
                  sp++;
                  dp++;
               }

               else
                  shift += s_inc;

               if (m == 1)
                  m = 0x80;

               else
                  m >>= 1;
            }
            break;
         }

         case 2:
         {
            sp = png_ptr->row_buf + 1;
            dp = row;
            m = 0x80;
            row_width = png_ptr->width;

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (png_ptr->transformations & PNG_PACKSWAP)
            {
               s_start = 0;
               s_end = 6;
               s_inc = 2;
            }

            else
#endif
            {
               s_start = 6;
               s_end = 0;
               s_inc = -2;
            }

            shift = s_start;

            for (i = 0; i < row_width; i++)
            {
               if (m & mask)
               {
                  value = (*sp >> shift) & 0x03;
                  *dp &= (png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                  *dp |= (png_byte)(value << shift);
               }

               if (shift == s_end)
               {
                  shift = s_start;
                  sp++;
                  dp++;
               }

               else
                  shift += s_inc;

               if (m == 1)
                  m = 0x80;

               else
                  m >>= 1;
            }
            break;
         }

         case 4:
         {
            sp = png_ptr->row_buf + 1;
            dp = row;
            m = 0x80;
            row_width = png_ptr->width;

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (png_ptr->transformations & PNG_PACKSWAP)
            {
               s_start = 0;
               s_end = 4;
               s_inc = 4;
            }

            else
#endif
            {
               s_start = 4;
               s_end = 0;
               s_inc = -4;
            }
            shift = s_start;

            for (i = 0; i < row_width; i++)
            {
               if (m & mask)
               {
                  value = (*sp >> shift) & 0xf;
                  *dp &= (png_byte)((0xf0f >> (4 - shift)) & 0xff);
                  *dp |= (png_byte)(value << shift);
               }

               if (shift == s_end)
               {
                  shift = s_start;
                  sp++;
                  dp++;
               }

               else
                  shift += s_inc;

               if (m == 1)
                  m = 0x80;

               else
                  m >>= 1;
            }
            break;
         }

         default:
         {
            sp = png_ptr->row_buf + 1;
            dp = row;
            pixel_bytes = (png_ptr->row_info.pixel_depth >> 3);
            row_width = png_ptr->width;
            m = 0x80;

            for (i = 0; i < row_width; i++)
            {
               if (m & mask)
               {
                  memcpy(dp, sp, pixel_bytes);
               }

               sp += pixel_bytes;
               dp += pixel_bytes;

               if (m == 1)
                  m = 0x80;

               else
                  m >>= 1;
            }
            break;
         }
      }
   }
}

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
/* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */
/* Start of interlace block */
int png_pass_start_2[7]  = {0, 4, 0, 2, 0, 1, 0};
/* Offset to next interlace block */
int png_pass_inc_2[7]    = {8, 8, 4, 4, 2, 2, 1};
/* Start of interlace block in the y direction */
int png_pass_ystart_2[7] = {0, 0, 4, 0, 2, 0, 1};
/* Offset to next interlace block in the y direction */
int png_pass_yinc_2[7]   = {8, 8, 8, 4, 4, 2, 2};
void png_read_finish_row_2(png_struct * png_ptr)
{
#ifdef PNG_READ_INTERLACING_SUPPORTED
   
   //png_byte png_IDAT[5] = { 73,  68,  65,  84, '\0'};
   char extra;
   int ret;
#endif /* PNG_READ_INTERLACING_SUPPORTED */

   //png_debug(1, "in png_read_finish_row_2");
   png_ptr->row_number++;
   if (png_ptr->row_number < png_ptr->num_rows)
      return;

#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (png_ptr->interlaced)
   {
      png_ptr->row_number = 0;

      memset(png_ptr->prev_row, 0, png_ptr->rowbytes + 1);

      do
      {
         png_ptr->pass++;

         if (png_ptr->pass >= 7)
            break;

         png_ptr->iwidth = (png_ptr->width +
            png_pass_inc_2[png_ptr->pass] - 1 -
            png_pass_start_2[png_ptr->pass]) /
            png_pass_inc_2[png_ptr->pass];

         if (!(png_ptr->transformations & PNG_INTERLACE))
         {
            png_ptr->num_rows = (png_ptr->height +
                png_pass_yinc_2[png_ptr->pass] - 1 -
                png_pass_ystart_2[png_ptr->pass]) /
                png_pass_yinc_2[png_ptr->pass];
         }

         else  /* if (png_ptr->transformations & PNG_INTERLACE) */
            break; /* libpng deinterlacing sees every row */

      } while (png_ptr->num_rows == 0 || png_ptr->iwidth == 0);

      if (png_ptr->pass < 7)
         return;
   }
#endif /* PNG_READ_INTERLACING_SUPPORTED */

   if (!(png_ptr->flags & PNG_FLAG_ZLIB_FINISHED))
   {
      //png_byte png_IDAT[5] = { 73,  68,  65,  84, '\0'};
      //char extra;
      //int ret;

      png_ptr->zstream.next_out = (Byte *)&extra;
      png_ptr->zstream.avail_out = (uInt)1;

      for (;;)
      {
         if (!(png_ptr->zstream.avail_in))
         {
            while (!png_ptr->idat_size)
            {
               png_crc_finish_2(png_ptr, 0);
               png_ptr->idat_size = png_read_chunk_header_2(png_ptr);
               if (png_memcmp(png_ptr->chunk_name, png_IDAT, 4))
                  png_warning_2(png_ptr, "Not enough image data");
            }

            png_ptr->zstream.avail_in = (uInt)png_ptr->zbuf_size;
            png_ptr->zstream.next_in = png_ptr->zbuf;

            if (png_ptr->zbuf_size > png_ptr->idat_size)
               png_ptr->zstream.avail_in = (uInt)png_ptr->idat_size;

            png_crc_read_2(png_ptr, png_ptr->zbuf, png_ptr->zstream.avail_in);
            png_ptr->idat_size -= png_ptr->zstream.avail_in;
         }

         ret = inflate(&png_ptr->zstream, Z_PARTIAL_FLUSH);

         if (ret == Z_STREAM_END)
         {
            //if (!(png_ptr->zstream.avail_out) || png_ptr->zstream.avail_in ||
            //    png_ptr->idat_size)
               //png_warning_2(png_ptr, "Extra compressed data");

            png_ptr->mode |= PNG_AFTER_IDAT;
            png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
            break;
         }

         /*if (ret != Z_OK)
            png_warning_2(png_ptr, png_ptr->zstream.msg ? png_ptr->zstream.msg :
                "Decompression Error");*/

         if (!(png_ptr->zstream.avail_out))
         {
            png_warning_2(png_ptr, "Extra compressed data");
            png_ptr->mode |= PNG_AFTER_IDAT;
            png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
            break;
         }

      }
      png_ptr->zstream.avail_out = 0;
   }

   //if (png_ptr->idat_size || png_ptr->zstream.avail_in)
      //png_warning_2(png_ptr, "Extra compression data");

   inflateReset(&png_ptr->zstream);

   png_ptr->mode |= PNG_AFTER_IDAT;
}

/* Transform the row.  The order of transformations is significant,
 * and is very touchy.  If you add a transformation, take care to
 * decide how it fits in with the other transformations here.
 */
void png_do_read_transformations_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_do_read_transformations");

   if (png_ptr->row_buf == NULL)
   {
#ifdef PNG_CONSOLE_IO_SUPPORTED
      //char msg[50];

      //png_snprintf2(msg, 50,
      //    "NULL row buffer for row %ld, pass %d", (long)png_ptr->row_number,
      //    png_ptr->pass);
      //png_error(png_ptr, msg);
#else
      //png_error(png_ptr, "NULL row buffer");
#endif
   }
#ifdef PNG_WARN_UNINITIALIZED_ROW
   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT))
      /* Application has failed to call either png_read_start_image()
       * or png_read_update_info() after setting transforms that expand
       * pixels.  This check added to libpng-1.2.19
       */
#if (PNG_WARN_UNINITIALIZED_ROW==1)
      //png_error(png_ptr, "Uninitialized row");
#else
      //png_warning(png_ptr, "Uninitialized row");
#endif
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
   if (png_ptr->transformations & PNG_EXPAND)
   {
      if (png_ptr->row_info.color_type == PNG_COLOR_TYPE_PALETTE)
      {
         png_do_expand_palette_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
             png_ptr->palette, png_ptr->trans_alpha, png_ptr->num_trans);
      }
      else
      {
         if (png_ptr->num_trans &&
             (png_ptr->transformations & PNG_EXPAND_tRNS))
            png_do_expand_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
                &(png_ptr->trans_color));
         else

            png_do_expand_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
                NULL);
      }
   }
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   if (png_ptr->flags & PNG_FLAG_STRIP_ALPHA)
      png_do_strip_filler_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
          PNG_FLAG_FILLER_AFTER | (png_ptr->flags & PNG_FLAG_STRIP_ALPHA));
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
   if (png_ptr->transformations & PNG_RGB_TO_GRAY)
   {
      int rgb_error =
          png_do_rgb_to_gray_2(png_ptr, &(png_ptr->row_info),
              png_ptr->row_buf + 1);

      if (rgb_error)
      {
         png_ptr->rgb_to_gray_status=1;
         if ((png_ptr->transformations & PNG_RGB_TO_GRAY) ==
             PNG_RGB_TO_GRAY_WARN)
            png_warning_2(png_ptr, "png_do_rgb_to_gray found nongray pixel");

         //if ((png_ptr->transformations & PNG_RGB_TO_GRAY) ==
            // PNG_RGB_TO_GRAY_ERR)
            //png_error(png_ptr, "png_do_rgb_to_gray found nongray pixel");
      }
   }
#endif

/* From Andreas Dilger e-mail to png-implement, 26 March 1998:
 *
 *   In most cases, the "simple transparency" should be done prior to doing
 *   gray-to-RGB, or you will have to test 3x as many bytes to check if a
 *   pixel is transparent.  You would also need to make sure that the
 *   transparency information is upgraded to RGB.
 *
 *   To summarize, the current flow is:
 *   - Gray + simple transparency -> compare 1 or 2 gray bytes and composite
 *                                   with background "in place" if transparent,
 *                                   convert to RGB if necessary
 *   - Gray + alpha -> composite with gray background and remove alpha bytes,
 *                                   convert to RGB if necessary
 *
 *   To support RGB backgrounds for gray images we need:
 *   - Gray + simple transparency -> convert to RGB + simple transparency,
 *                                   compare 3 or 6 bytes and composite with
 *                                   background "in place" if transparent
 *                                   (3x compare/pixel compared to doing
 *                                   composite with gray bkgrnd)
 *   - Gray + alpha -> convert to RGB + alpha, composite with background and
 *                                   remove alpha bytes (3x float
 *                                   operations/pixel compared with composite
 *                                   on gray background)
 *
 *  Greg's change will do this.  The reason it wasn't done before is for
 *  performance, as this increases the per-pixel operations.  If we would check
 *  in advance if the background was gray or RGB, and position the gray-to-RGB
 *  transform appropriately, then it would save a lot of work/time.
 */

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   /* If gray -> RGB, do so now only if background is non-gray; else do later
    * for performance reasons
    */
   if ((png_ptr->transformations & PNG_GRAY_TO_RGB) &&
       !(png_ptr->mode & PNG_BACKGROUND_IS_GRAY))
      png_do_gray_to_rgb_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_BACKGROUND_SUPPORTED
   if ((png_ptr->transformations & PNG_BACKGROUND) &&
       ((png_ptr->num_trans != 0) ||
       (png_ptr->color_type & PNG_COLOR_MASK_ALPHA)))
      png_do_background_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
          &(png_ptr->trans_color), &(png_ptr->background)
#ifdef PNG_READ_GAMMA_SUPPORTED
          , &(png_ptr->background_1),
          png_ptr->gamma_table, png_ptr->gamma_from_1,
          png_ptr->gamma_to_1, png_ptr->gamma_16_table,
          png_ptr->gamma_16_from_1, png_ptr->gamma_16_to_1,
          png_ptr->gamma_shift
#endif
          );
#endif

#ifdef PNG_READ_GAMMA_SUPPORTED
   if ((png_ptr->transformations & PNG_GAMMA) &&
#ifdef PNG_READ_BACKGROUND_SUPPORTED
       !((png_ptr->transformations & PNG_BACKGROUND) &&
       ((png_ptr->num_trans != 0) ||
       (png_ptr->color_type & PNG_COLOR_MASK_ALPHA))) &&
#endif
       (png_ptr->color_type != PNG_COLOR_TYPE_PALETTE))
      png_do_gamma_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
          png_ptr->gamma_table, png_ptr->gamma_16_table,
          png_ptr->gamma_shift);
#endif

#ifdef PNG_READ_16_TO_8_SUPPORTED
   if (png_ptr->transformations & PNG_16_TO_8)
      png_do_chop_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
   if (png_ptr->transformations & PNG_QUANTIZE)
   {
      png_do_quantize_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
          png_ptr->palette_lookup, png_ptr->quantize_index);

      //if (png_ptr->row_info.rowbytes == 0)
         //png_error(png_ptr, "png_do_quantize returned rowbytes=0");
   }
#endif /* PNG_READ_QUANTIZE_SUPPORTED */

#ifdef PNG_READ_INVERT_SUPPORTED
   if (png_ptr->transformations & PNG_INVERT_MONO)
      png_do_invert_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_SHIFT_SUPPORTED
   if (png_ptr->transformations & PNG_SHIFT)
      png_do_unshift_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
          &(png_ptr->shift));
#endif

#ifdef PNG_READ_PACK_SUPPORTED
   if (png_ptr->transformations & PNG_PACK)
      png_do_unpack_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_BGR_SUPPORTED
   if (png_ptr->transformations & PNG_BGR)
      png_do_bgr_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_PACKSWAP_SUPPORTED
   if (png_ptr->transformations & PNG_PACKSWAP)
      png_do_packswap_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   /* If gray -> RGB, do so now only if we did not do so above */
   if ((png_ptr->transformations & PNG_GRAY_TO_RGB) &&
       (png_ptr->mode & PNG_BACKGROUND_IS_GRAY))
      png_do_gray_to_rgb_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_FILLER_SUPPORTED
   if (png_ptr->transformations & PNG_FILLER)
      png_do_read_filler_2(&(png_ptr->row_info), png_ptr->row_buf + 1,
          (png_uint_32)png_ptr->filler, png_ptr->flags);
#endif

#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
   if (png_ptr->transformations & PNG_INVERT_ALPHA)
      png_do_read_invert_alpha_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
   if (png_ptr->transformations & PNG_SWAP_ALPHA)
      png_do_read_swap_alpha_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_16BIT_SUPPORTED
#ifdef PNG_READ_SWAP_SUPPORTED
   if (png_ptr->transformations & PNG_SWAP_BYTES)
      png_do_swap_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
#endif
#endif

#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
   if (png_ptr->transformations & PNG_USER_TRANSFORM)
    {
      if (png_ptr->read_user_transform_fn != NULL)
         (*(png_ptr->read_user_transform_fn)) /* User read transform function */
             (png_ptr,                    /* png_ptr */
             &(png_ptr->row_info),     /* row_info: */
                /*  png_uint_32 width;       width of row */
                /*  png_size_t rowbytes;     number of bytes in row */
                /*  png_byte color_type;     color type of pixels */
                /*  png_byte bit_depth;      bit depth of samples */
                /*  png_byte channels;       number of channels (1-4) */
                /*  png_byte pixel_depth;    bits per pixel (depth*channels) */
             png_ptr->row_buf + 1);    /* start of pixel data for row */
#ifdef PNG_USER_TRANSFORM_PTR_SUPPORTED
      if (png_ptr->user_transform_depth)
         png_ptr->row_info.bit_depth = png_ptr->user_transform_depth;

      if (png_ptr->user_transform_channels)
         png_ptr->row_info.channels = png_ptr->user_transform_channels;
#endif
      png_ptr->row_info.pixel_depth = (png_byte)(png_ptr->row_info.bit_depth *
          png_ptr->row_info.channels);

      png_ptr->row_info.rowbytes = PNG_ROWBYTES(png_ptr->row_info.pixel_depth,
          png_ptr->row_info.width);
   }
#endif

}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */


#ifdef PNG_READ_INTERLACING_SUPPORTED
int png_pass_inc_3[7] = {8, 8, 4, 4, 2, 2, 1};
void /* PRIVATE */
png_do_read_interlace_2(png_struct * png_ptr)
{
   png_row_info * row_info = &(png_ptr->row_info);
   png_byte * row = png_ptr->row_buf + 1;
   int pass = png_ptr->pass;
   png_uint_32 transformations = png_ptr->transformations;
   /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */
   /* Offset to next interlace block */
   png_size_t pixel_bytes = (row_info->pixel_depth >> 3);
   png_byte v[8];
   int j;
   png_byte * sp;
   png_byte * dp;
   int sshift, dshift;
   int s_start, s_end, s_inc;
   png_uint_32 i;
   png_uint_32 final_width;
   int jstop;
   //png_debug(1, "in png_do_read_interlace_2");
   if (row != NULL && row_info != NULL)
   {
      final_width = row_info->width * png_pass_inc_3[pass];

      switch (row_info->pixel_depth)
      {
         case 1:
         {
            sp = row + (png_size_t)((row_info->width - 1) >> 3);
            dp = row + (png_size_t)((final_width - 1) >> 3);
            jstop = png_pass_inc_3[pass];

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (transformations & PNG_PACKSWAP)
            {
                sshift = (int)((row_info->width + 7) & 0x07);
                dshift = (int)((final_width + 7) & 0x07);
                s_start = 7;
                s_end = 0;
                s_inc = -1;
            }

            else
#endif
            {
                sshift = 7 - (int)((row_info->width + 7) & 0x07);
                dshift = 7 - (int)((final_width + 7) & 0x07);
                s_start = 0;
                s_end = 7;
                s_inc = 1;
            }

            for (i = 0; i < row_info->width; i++)
            {
               v[0] = (png_byte)((*sp >> sshift) & 0x01);
               for (j = 0; j < jstop; j++)
               {
                  *dp &= (png_byte)((0x7f7f >> (7 - dshift)) & 0xff);
                  *dp |= (png_byte)(v[0] << dshift);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift += s_inc;
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift += s_inc;
            }
            break;
         }

         case 2:
         {
            sp = row + (png_uint_32)((row_info->width - 1) >> 2);
            dp = row + (png_uint_32)((final_width - 1) >> 2);
            jstop = png_pass_inc_3[pass];

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (transformations & PNG_PACKSWAP)
            {
               sshift = (int)(((row_info->width + 3) & 0x03) << 1);
               dshift = (int)(((final_width + 3) & 0x03) << 1);
               s_start = 6;
               s_end = 0;
               s_inc = -2;
            }

            else
#endif
            {
               sshift = (int)((3 - ((row_info->width + 3) & 0x03)) << 1);
               dshift = (int)((3 - ((final_width + 3) & 0x03)) << 1);
               s_start = 0;
               s_end = 6;
               s_inc = 2;
            }

            for (i = 0; i < row_info->width; i++)
            {
               v[0] = (png_byte)((*sp >> sshift) & 0x03);
               for (j = 0; j < jstop; j++)
               {
                  *dp &= (png_byte)((0x3f3f >> (6 - dshift)) & 0xff);
                  *dp |= (png_byte)(v[0] << dshift);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift += s_inc;
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift += s_inc;
            }
            break;
         }

         case 4:
         {
            sp = row + (png_size_t)((row_info->width - 1) >> 1);
            dp = row + (png_size_t)((final_width - 1) >> 1);
            jstop = png_pass_inc_3[pass];

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (transformations & PNG_PACKSWAP)
            {
               sshift = (int)(((row_info->width + 1) & 0x01) << 2);
               dshift = (int)(((final_width + 1) & 0x01) << 2);
               s_start = 4;
               s_end = 0;
               s_inc = -4;
            }

            else
#endif
            {
               sshift = (int)((1 - ((row_info->width + 1) & 0x01)) << 2);
               dshift = (int)((1 - ((final_width + 1) & 0x01)) << 2);
               s_start = 0;
               s_end = 4;
               s_inc = 4;
            }

            for (i = 0; i < row_info->width; i++)
            {
               v[0] = (png_byte)((*sp >> sshift) & 0xf);

               for (j = 0; j < jstop; j++)
               {
                  *dp &= (png_byte)((0xf0f >> (4 - dshift)) & 0xff);
                  *dp |= (png_byte)(v[0] << dshift);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift += s_inc;
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift += s_inc;
            }
            break;
         }
         default:
         {
            pixel_bytes = (row_info->pixel_depth >> 3);

            sp = row + (png_size_t)(row_info->width - 1)
                * pixel_bytes;

            dp = row + (png_size_t)(final_width - 1) * pixel_bytes;

            jstop = png_pass_inc_3[pass];
            for (i = 0; i < row_info->width; i++)
            {
               memcpy(v, sp, pixel_bytes);

               for (j = 0; j < jstop; j++)
               {
                  memcpy(dp, v, pixel_bytes);
                  dp -= pixel_bytes;
               }

               sp -= pixel_bytes;
            }
            break;
         }
      }
      row_info->width = final_width;
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, final_width);
   }
#ifndef PNG_READ_PACKSWAP_SUPPORTED
   //PNG_UNUSED(transformations)  /* Silence compiler warning */
#endif
}
#endif /* PNG_READ_INTERLACING_SUPPORTED */

int png_pass_dsp_mask[7] = {0xff, 0x0f, 0xff, 0x33, 0xff, 0x55, 0xff};
int png_pass_mask[7]     = {0x80, 0x08, 0x88, 0x22, 0xaa, 0x55, 0xff};
void png_read_row_2(png_struct * png_ptr, png_byte * row, png_byte * dsp_row)
{
   //png_byte png_IDAT[5] = { 73,  68,  65,  84, '\0'};
   int ret;

   if (png_ptr == NULL)
      return;

   //png_debug2(1, "in png_read_row (row %lu, pass %d)",
       //(unsigned long)png_ptr->row_number, png_ptr->pass);

   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT))
      png_read_start_row_2(png_ptr);

   if (png_ptr->row_number == 0 && png_ptr->pass == 0)
   {
   /* Check for transforms that have been set but were defined out */
#if defined(PNG_WRITE_INVERT_SUPPORTED) && !defined(PNG_READ_INVERT_SUPPORTED)
   if (png_ptr->transformations & PNG_INVERT_MONO)
     png_warning_2(png_ptr, "PNG_READ_INVERT_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_FILLER_SUPPORTED) && !defined(PNG_READ_FILLER_SUPPORTED)
   if (png_ptr->transformations & PNG_FILLER)
      png_warning_2(png_ptr, "PNG_READ_FILLER_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_PACKSWAP_SUPPORTED) && \
    !defined(PNG_READ_PACKSWAP_SUPPORTED)
    if (png_ptr->transformations & PNG_PACKSWAP)
      png_warning_2(png_ptr, "PNG_READ_PACKSWAP_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_PACK_SUPPORTED) && !defined(PNG_READ_PACK_SUPPORTED)
   if (png_ptr->transformations & PNG_PACK)
      png_warning_2(png_ptr, "PNG_READ_PACK_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_SHIFT_SUPPORTED) && !defined(PNG_READ_SHIFT_SUPPORTED)
   if (png_ptr->transformations & PNG_SHIFT)
      png_warning_2(png_ptr, "PNG_READ_SHIFT_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_BGR_SUPPORTED) && !defined(PNG_READ_BGR_SUPPORTED)
   if (png_ptr->transformations & PNG_BGR)
      png_warning_2(png_ptr, "PNG_READ_BGR_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_SWAP_SUPPORTED) && !defined(PNG_READ_SWAP_SUPPORTED)
   if (png_ptr->transformations & PNG_SWAP_BYTES)
      png_warning_2(png_ptr, "PNG_READ_SWAP_SUPPORTED is not defined");
#endif
   }

#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* If interlaced and we do not need a new row, combine row and return */
   if (png_ptr->interlaced && (png_ptr->transformations & PNG_INTERLACE))
   {
      switch (png_ptr->pass)
      {
         case 0:
            //png_warning_2(png_ptr, "case 0");
            if (png_ptr->row_number & 0x07)
            {
               if (dsp_row != NULL) {
                  png_combine_row_2(png_ptr, dsp_row,
                     png_pass_dsp_mask[png_ptr->pass]);
               }
               png_read_finish_row_2(png_ptr);
               return;
            }
            break;

         case 1:
            //png_warning_2(png_ptr, "case 1");
            if ((png_ptr->row_number & 0x07) || png_ptr->width < 5)
            {
               if (dsp_row != NULL) {
                  png_combine_row_2(png_ptr, dsp_row,
                      png_pass_dsp_mask[png_ptr->pass]);
			   }
               png_read_finish_row_2(png_ptr);
               return;
            }
            break;

         case 2:
            //png_warning_2(png_ptr, "case 2");
            if ((png_ptr->row_number & 0x07) != 4)
            {
               if (dsp_row != NULL && (png_ptr->row_number & 4)) {
                  png_combine_row_2(png_ptr, dsp_row,
                      png_pass_dsp_mask[png_ptr->pass]);
			   }
               png_read_finish_row_2(png_ptr);
               return;
            }
            break;

         case 3:
            //png_warning_2(png_ptr, "case 3");
            if ((png_ptr->row_number & 3) || png_ptr->width < 3)
            {
               if (dsp_row != NULL) {
                  png_combine_row_2(png_ptr, dsp_row,
                      png_pass_dsp_mask[png_ptr->pass]);
			   }
               png_read_finish_row_2(png_ptr);
               return;
            }
            break;

         case 4:
            //png_warning_2(png_ptr, "case 4");
            if ((png_ptr->row_number & 3) != 2)
            {
               if (dsp_row != NULL && (png_ptr->row_number & 2)) {
                  png_combine_row_2(png_ptr, dsp_row,
                      png_pass_dsp_mask[png_ptr->pass]);
			   }
               png_read_finish_row_2(png_ptr);
               return;
            }
            break;
         case 5:
            //png_warning_2(png_ptr, "case 5");
            if ((png_ptr->row_number & 1) || png_ptr->width < 2)
            {
               if (dsp_row != NULL) {
                  png_combine_row_2(png_ptr, dsp_row,
                      png_pass_dsp_mask[png_ptr->pass]);
			   }
               png_read_finish_row_2(png_ptr);
               return;
            }
            break;

         default:
         case 6:
            //png_warning_2(png_ptr, "case 6");
            if (!(png_ptr->row_number & 1))
            {
               png_read_finish_row_2(png_ptr);
               return;
            }
            break;
      }
   }
#endif

   //if (!(png_ptr->mode & PNG_HAVE_IDAT))
   //   png_error(png_ptr, "Invalid attempt to read row data");
   //png_warning_2(png_ptr, "end case");
   png_ptr->zstream.next_out = png_ptr->row_buf;
   png_ptr->zstream.avail_out =
       (uInt)(PNG_ROWBYTES(png_ptr->pixel_depth,
       png_ptr->iwidth) + 1);

   do
   {
      if (!(png_ptr->zstream.avail_in))
      {
         //png_warning_2(png_ptr, "png_ptr->zstream.avail_in not available");
         while (!png_ptr->idat_size)
         {
            png_crc_finish_2(png_ptr, 0);

            png_ptr->idat_size = png_read_chunk_header_2(png_ptr);
            if (memcmp(png_ptr->chunk_name, png_IDAT, 4))
               png_warning_2(png_ptr, "Not enough image data");
         }
         png_warning_2(png_ptr, "set png_ptr->zstream.avail_in");
         png_ptr->zstream.avail_in = (uInt)png_ptr->zbuf_size;
         png_ptr->zstream.next_in = png_ptr->zbuf;
         if (png_ptr->zbuf_size > png_ptr->idat_size)
            png_ptr->zstream.avail_in = (uInt)png_ptr->idat_size;
         png_crc_read_2(png_ptr, png_ptr->zbuf,
             (png_size_t)png_ptr->zstream.avail_in);
         png_ptr->idat_size -= png_ptr->zstream.avail_in;
      }
      //png_warning_2(png_ptr, "inflate");
      ret = inflate(&png_ptr->zstream, Z_PARTIAL_FLUSH);
      if ((ret & Z_STREAM_END) == Z_STREAM_END)
      {
         //if (png_ptr->zstream.avail_out || png_ptr->zstream.avail_in ||
         //   png_ptr->idat_size)
            //png_warning_2(png_ptr, "Extra compressed data");
         png_ptr->mode |= PNG_AFTER_IDAT;
         png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
         break;
      }

      if (ret == Z_OK) {
      
      } else {
         png_warning_2(png_ptr, "Decompression error");
      }  //png_error(png_ptr, png_ptr->zstream.msg ? png_ptr->zstream.msg :
         //    "Decompression error");

   } while (png_ptr->zstream.avail_out);

   png_ptr->row_info.color_type = png_ptr->color_type;
   png_ptr->row_info.width = png_ptr->iwidth;
   png_ptr->row_info.channels = png_ptr->channels;
   png_ptr->row_info.bit_depth = png_ptr->bit_depth;
   png_ptr->row_info.pixel_depth = png_ptr->pixel_depth;
   png_ptr->row_info.rowbytes = PNG_ROWBYTES(png_ptr->row_info.pixel_depth,
       png_ptr->row_info.width);

   if (png_ptr->row_buf[0])
   png_read_filter_row_2(png_ptr, &(png_ptr->row_info),
       png_ptr->row_buf + 1, png_ptr->prev_row + 1,
       (int)(png_ptr->row_buf[0]));

   memcpy(png_ptr->prev_row, png_ptr->row_buf, png_ptr->rowbytes + 1);

#ifdef PNG_MNG_FEATURES_SUPPORTED
   
   if ((png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
       (png_ptr->filter_type == PNG_INTRAPIXEL_DIFFERENCING))
   {
      png_warning_2(png_ptr, "use mng");
      /* Intrapixel differencing */
      png_do_read_intrapixel_2(&(png_ptr->row_info), png_ptr->row_buf + 1);
   }
#endif


   if (png_ptr->transformations || (png_ptr->flags&PNG_FLAG_STRIP_ALPHA))
      png_do_read_transformations_2(png_ptr);

#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* Blow up interlaced rows to full size */
   if (png_ptr->interlaced &&
      (png_ptr->transformations & PNG_INTERLACE))
   {
      png_warning_2(png_ptr, "use interlacing");
      if (png_ptr->pass < 6)
         /* Old interface (pre-1.0.9):
          * png_do_read_interlace_2(&(png_ptr->row_info),
          *    png_ptr->row_buf + 1, png_ptr->pass, png_ptr->transformations);
          */
         png_do_read_interlace_2(png_ptr);

      if (dsp_row != NULL)
         png_combine_row_2(png_ptr, dsp_row, png_pass_dsp_mask[png_ptr->pass]);

      if (row != NULL)
         png_combine_row_2(png_ptr, row, png_pass_mask[png_ptr->pass]);
   }

   else
#endif
   {
      if (row != NULL)
         png_combine_row_2(png_ptr, row, 0xff);

      if (dsp_row != NULL)
         png_combine_row_2(png_ptr, dsp_row, 0xff);
   }
   //png_warning_2(png_ptr, "png_read_2");
   png_read_finish_row_2(png_ptr);
   //png_warning_2(png_ptr, "finish_row_2");
   if (png_ptr->read_row_fn != NULL) 
      (*(png_ptr->read_row_fn))(png_ptr, png_ptr->row_number, png_ptr->pass);
}

#include "2440lib.h"
void png_warning_2(png_struct * png_ptr, char * message) {
	Uart_Printf(message);
	Uart_Printf("\n");
}