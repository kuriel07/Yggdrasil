#include "defs.h"
#include "yggdrasil.h"
#include "yggdrasil_pv.h"
#include "png.h"
#include "efat.h"
#include "midgard.h"
#include "resources.h"
#include "sd.h"
#include "lcd.h"
#include "pngpriv.h"
#include "png_alloc.h"
#include "yggdrasil_png.h"

void png_user_flush_data(png_struct * png_ptr) {
	//nothing here
}

void png_user_error(void) {
	
}

void png_user_warning(void) {

}

void HandlePVLauncher(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Heap_Manager * heap = sender->heap;
	efat_FileSystem * fs;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						//set to top layer, cek apakah sender memiliki sibling
						//jika tidak, brarti sender sudah berada pada top layer
						
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_MOVE:
						
						break;
					case TS_RELEASE:
						//SendSystemMessage(MS_KEYBOARD, 0, 0, NULL, _shell_task);
						//Uart_Printf("initializing sd driver");
						//Uart_Printf("fat fs initialized\n");
						CreatePicViewer();
						sender->cursor.x = 0;
						sender->cursor.y = 0;
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:
						break;
					case MS_REFRESH:
						if(smp->buffer != NULL)
							Buffer_PrintText(
								6,
								(smp->x + sender->rect.width) - (96 / 2), 
								smp->y + (sender->rect.height), 
								C_WHITE, 
								smp->buffer, "%s\n", sender->text);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		//RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

void png_set_read_fn_2(png_struct * png_ptr, void * io_ptr,
   void (* read_data_fn)(png_struct *, png_byte *, png_size_t))
{
   if (png_ptr == NULL)
      return;

   png_ptr->io_ptr = io_ptr;

#ifdef PNG_STDIO_SUPPORTED
   if (read_data_fn != NULL) {
      png_ptr->read_data_fn = read_data_fn;
   }
#else
   png_ptr->read_data_fn = read_data_fn;
#endif

   /* It is an error to write to a read device */
   if (png_ptr->write_data_fn != NULL)
   {
      png_ptr->write_data_fn = NULL;
   }

#ifdef PNG_WRITE_FLUSH_SUPPORTED
   png_ptr->output_flush_fn = NULL;
#endif
}

png_struct * png_create_read_struct_3(png_const_charp user_png_ver, void * error_ptr,
    void (* error_fn)(png_struct *, png_const_charp), void (* warn_fn)(png_struct *, png_const_charp), void * mem_ptr,
    void * (* malloc_fn)(png_struct *, png_alloc_size_t), void (* free_fn)(png_struct *, void *))
{

#ifdef PNG_SETJMP_SUPPORTED
   volatile
#endif
   png_struct * png_ptr;
   volatile int png_cleanup_needed = 0;

#ifdef PNG_SETJMP_SUPPORTED
#ifdef USE_FAR_KEYWORD
   jmp_buf png_jmpbuf;
#endif
#endif

   int i;

   //png_debug(1, "in png_create_read_struct");

#ifdef PNG_USER_MEM_SUPPORTED
   png_ptr = (png_struct *)png_create_struct_3(PNG_STRUCT_PNG,
       malloc_fn, mem_ptr);
#endif
   if (png_ptr == NULL)
      return (NULL);

  
#ifdef PNG_USER_LIMITS_SUPPORTED
   png_ptr->user_width_max = PNG_USER_WIDTH_MAX;
   png_ptr->user_height_max = PNG_USER_HEIGHT_MAX;

#ifdef PNG_USER_CHUNK_CACHE_MAX
   
   png_ptr->user_chunk_cache_max = PNG_USER_CHUNK_CACHE_MAX;
#endif

#ifdef PNG_SET_USER_CHUNK_MALLOC_MAX
   
   png_ptr->user_chunk_malloc_max = PNG_USER_CHUNK_MALLOC_MAX;
#endif
#endif

#ifdef PNG_USER_MEM_SUPPORTED
   png_set_mem_fn_2(png_ptr, mem_ptr, malloc_fn, free_fn);
#endif

   png_set_error_fn_2(png_ptr, error_ptr, error_fn, warn_fn);
/**************************PNG VERSION CHECKING*************************
   if (user_png_ver)
   {
      i = 0;

      do
      {
         if (user_png_ver[i] != png_libpng_ver[i])
            png_ptr->flags |= PNG_FLAG_LIBRARY_MISMATCH;
      } while (png_libpng_ver[i++]);
   }

   else
      png_ptr->flags |= PNG_FLAG_LIBRARY_MISMATCH;


   if (png_ptr->flags & PNG_FLAG_LIBRARY_MISMATCH)
   {
      if (user_png_ver == NULL || user_png_ver[0] != png_libpng_ver[0] ||
          (user_png_ver[0] == '1' && user_png_ver[2] != png_libpng_ver[2]) ||
          (user_png_ver[0] == '0' && user_png_ver[2] < '9'))
      {
#ifdef PNG_CONSOLE_IO_SUPPORTED
         char msg[80];
         if (user_png_ver)
         {
            
            //png_warning(png_ptr, msg);
         } 
#endif
#ifdef PNG_ERROR_NUMBERS_SUPPORTED
         png_ptr->flags = 0;
#endif

         png_cleanup_needed = 1;
      }
   }
***********************************************************************/
   if (!png_cleanup_needed)
   {
   png_ptr->zbuf_size = PNG_ZBUF_SIZE;
   png_ptr->zbuf = (png_bytep)png_malloc_warn_2(png_ptr, png_ptr->zbuf_size);

   if (png_ptr->zbuf == NULL)
      png_cleanup_needed = 1;
   }

   png_ptr->zstream.zalloc = png_zalloc_2;
   png_ptr->zstream.zfree = png_zfree_2;
   png_ptr->zstream.opaque = (voidpf)png_ptr;

   if (!png_cleanup_needed)
   {
      png_warning_2(png_ptr, "inflate init");
      switch (inflateInit(&png_ptr->zstream))
      {
         case Z_OK:
         
            break;
         case Z_MEM_ERROR:
            png_warning_2(png_ptr, "zlib memory error");
            png_cleanup_needed = 1;
            break;

         case Z_STREAM_ERROR:
            png_warning_2(png_ptr, "zlib stream error");
            png_cleanup_needed = 1;
            break;

         case Z_VERSION_ERROR:
            png_warning_2(png_ptr, "zlib version error");
            png_cleanup_needed = 1;
            break;

         default: png_warning_2(png_ptr, "Unknown zlib error");
            png_cleanup_needed = 1;
      }
   }

   if (png_cleanup_needed)
   {
      png_free_2(png_ptr, png_ptr->zbuf);
      png_ptr->zbuf = NULL;
#ifdef PNG_USER_MEM_SUPPORTED
      png_destroy_struct_3((void *)png_ptr,
          free_fn, (void *)mem_ptr);
#endif
      return (NULL);
   }

   png_ptr->zstream.next_out = png_ptr->zbuf;
   png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;

   png_set_read_fn_2(png_ptr, NULL, NULL);

   return (png_ptr);
}


void png_info_init_4(png_info ** ptr_ptr, png_size_t png_info_struct_size)
{
   png_info * info_ptr = * ptr_ptr;

   //png_debug(1, "in png_info_init_3");

   if (info_ptr == NULL)
      return;

   /*if (png_sizeof(png_info) > png_info_struct_size)
   {
      png_destroy_struct_4(info_ptr);
      info_ptr = (png_info *)png_create_struct(PNG_STRUCT_INFO);
      *ptr_ptr = info_ptr;
   }*/

   memset(info_ptr, 0, png_sizeof(png_info));
}


png_info * png_create_info_struct_2(png_struct * png_ptr)
{
   png_info * info_ptr;
   
   //png_debug(1, "in png_create_info_struct");
   if (png_ptr == NULL)
      return (NULL);

#ifdef PNG_USER_MEM_SUPPORTED
   info_ptr = (png_info *)png_create_struct_3(PNG_STRUCT_INFO,
      png_ptr->malloc_fn, png_ptr->mem_ptr);
#endif
   Uart_Printf("init info struct\n");
   if (info_ptr != NULL)
      png_info_init_4(&info_ptr, png_sizeof(png_info));
   Uart_Printf("initialized\n");
   return info_ptr;
}

void png_set_sig_bytes_2(png_struct * png_ptr, int num_bytes)
{
   //png_debug(1, "in png_set_sig_bytes");

   if (png_ptr == NULL)
      return;

   if (num_bytes > 8) {
      //png_error(png_ptr, "Too many bytes for PNG signature");
   }
   png_ptr->sig_bytes = (png_byte)(num_bytes < 0 ? 0 : num_bytes);
}

void png_set_user_limits_2 (png_struct * png_ptr, png_uint_32 user_width_max,
    png_uint_32 user_height_max)
{
   /* Images with dimensions larger than these limits will be
    * rejected by png_set_IHDR().  To accept any PNG datastream
    * regardless of dimensions, set both limits to 0x7ffffffL.
    */
   if (png_ptr == NULL)
      return;

   png_ptr->user_width_max = user_width_max;
   png_ptr->user_height_max = user_height_max;
}

png_size_t png_get_rowbytes_2(png_struct * png_ptr, png_info * info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->rowbytes);

   return(0);
}

#if defined(PNG_READ_INTERLACING_SUPPORTED) || \
    defined(PNG_WRITE_INTERLACING_SUPPORTED)
int png_set_interlace_handling_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_set_interlace handling");

   if (png_ptr && png_ptr->interlaced)
   {
      png_ptr->transformations |= PNG_INTERLACE;
      return (7);
   }

   return (1);
}
#endif

/* Initialize palette, background, etc, after transformations
 * are set, but before any reading takes place.  This allows
 * the user to obtain a gamma-corrected palette, for example.
 * If the user doesn't call this, we will do it ourselves.
 */
void png_start_read_image_2(png_struct * png_ptr)
{
   //png_debug(1, "in png_start_read_image");

   if (png_ptr == NULL)
      return;

   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT))
      png_read_start_row_2(png_ptr);
   else
      png_warning_2(png_ptr,
          "Ignoring extra png_start_read_image() call;"
          " row buffer not reallocated");
}
/* Read the entire image.  If the image has an alpha channel or a tRNS
 * chunk, and you have called png_handle_alpha()[*], you will need to
 * initialize the image to the current image that PNG will be overlaying.
 * We set the num_rows again here, in case it was incorrectly set in
 * png_read_start_row() by a call to png_read_update_info() or
 * png_start_read_image() if png_set_interlace_handling() wasn't called
 * prior to either of these functions like it should have been.  You can
 * only call this function once.  If you desire to have an image for
 * each pass of a interlaced image, use png_read_rows() instead.
 *
 * [*] png_handle_alpha() does not exist yet, as of this version of libpng
 */
void png_read_image_2(png_struct * png_ptr, png_byte ** image)
{
   png_uint_32 i, image_height;
   int pass, j;
   png_byte ** rp;
   Uart_Printf(1, "in png_read_image");

   if (png_ptr == NULL)
      return;

#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (!(png_ptr->flags & PNG_FLAG_ROW_INIT))
   {
   	  Uart_Printf("png_set_interlace_handling\n");
      pass = png_set_interlace_handling_2(png_ptr);
      /* And make sure transforms are initialized. */
      Uart_Printf("png_start_read_image\n");
      png_start_read_image_2(png_ptr);
   }
   else
   {
      if (png_ptr->interlaced && !(png_ptr->transformations & PNG_INTERLACE))
      {
         /* Caller called png_start_read_image or png_read_update_info without
          * first turning on the PNG_INTERLACE transform.  We can fix this here,
          * but the caller should do it!
          */
         png_warning_2(png_ptr, "Interlace handling should be turned on when "
            "using png_read_image");
         /* Make sure this is set correctly */
         png_ptr->num_rows = png_ptr->height;
      }

      /* Obtain the pass number, which also turns on the PNG_INTERLACE flag in
       * the above error case.
       */
      Uart_Printf("png_set_interlace_handling\n");
      pass = png_set_interlace_handling_2(png_ptr);
   }
#else
   //if (png_ptr->interlaced)
      //png_error(png_ptr,
       ///   "Cannot read interlaced image -- interlace handler disabled");

   pass = 1;
#endif

   image_height=png_ptr->height;

   for (j = 0; j < pass; j++)
   {
      rp = image;
      for (i = 0; i < image_height; i++)
      {
         //Uart_Printf("png_read_row\n");
         png_read_row_2(png_ptr, rp[i], NULL);
         //rp++;
      }
   }
}


#ifdef PNG_INFO_IMAGE_SUPPORTED
void png_read_png_2(png_struct * png_ptr, png_info * info_ptr,
                           int transforms,
                           void * params)
{
   int row;
   png_uint_32 iptr;
   png_color_8 * sig_bit;
   if (png_ptr == NULL)
      return;

   /* png_read_info() gives us all of the information from the
    * PNG file before the first IDAT (image data chunk).
    */
   png_read_info_2(png_ptr, info_ptr);
   
   //if (info_ptr->height > PNG_UINT_32_MAX/png_sizeof(png_bytep))
   //   png_error(png_ptr, "Image is too high to process with png_read_png()");

   /* -------------- image transformations start here ------------------- */
   png_warning_2(png_ptr, "Read info finished");
#ifdef PNG_READ_16_TO_8_SUPPORTED
   /* Tell libpng to strip 16 bit/color files down to 8 bits per color.
    */
   if (transforms & PNG_TRANSFORM_STRIP_16) {
      png_warning_2(png_ptr, "transform strip 16");
      png_set_strip_16_2(png_ptr);
   }
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   /* Strip alpha bytes from the input data without combining with
    * the background (not recommended).
    */
   if (transforms & PNG_TRANSFORM_STRIP_ALPHA) {
      png_warning_2(png_ptr, "transform strip alpha");
      png_set_strip_alpha_2(png_ptr);
   }
#endif

#if defined(PNG_READ_PACK_SUPPORTED) && !defined(PNG_READ_EXPAND_SUPPORTED)
   /* Extract multiple pixels with bit depths of 1, 2, or 4 from a single
    * byte into separate bytes (useful for paletted and grayscale images).
    */
   if (transforms & PNG_TRANSFORM_PACKING) {
      png_warning_2(png_ptr, "transform packing");
      png_set_packing_2(png_ptr);
   }
#endif

#ifdef PNG_READ_PACKSWAP_SUPPORTED
   /* Change the order of packed pixels to least significant bit first
    * (not useful if you are using png_set_packing).
    */
   if (transforms & PNG_TRANSFORM_PACKSWAP) {
      png_warning_2(png_ptr, "transform packswap");
      png_set_packswap_2(png_ptr);
   }
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
   /* Expand paletted colors into true RGB triplets
    * Expand grayscale images to full 8 bits from 1, 2, or 4 bits/pixel
    * Expand paletted or RGB images with transparency to full alpha
    * channels so the data will be available as RGBA quartets.
    */
   if (transforms & PNG_TRANSFORM_EXPAND)
      if ((png_ptr->bit_depth < 8) ||
          (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE) ||
          (png_get_valid_2(png_ptr, info_ptr, PNG_INFO_tRNS)))
         png_set_expand_2(png_ptr);
#endif

   /* We don't handle background color or gamma transformation or quantizing.
    */

#ifdef PNG_READ_INVERT_SUPPORTED
   /* Invert monochrome files to have 0 as white and 1 as black
    */
   if (transforms & PNG_TRANSFORM_INVERT_MONO)
      png_set_invert_mono_2(png_ptr);
#endif

#ifdef PNG_READ_SHIFT_SUPPORTED
   /* If you want to shift the pixel values from the range [0,255] or
    * [0,65535] to the original [0,7] or [0,31], or whatever range the
    * colors were originally in:
    */
   if ((transforms & PNG_TRANSFORM_SHIFT)
       && png_get_valid_2(png_ptr, info_ptr, PNG_INFO_sBIT))
   {
      png_get_sBIT_2(png_ptr, info_ptr, &sig_bit);
      png_set_shift_2(png_ptr, sig_bit);
   }
#endif

#ifdef PNG_READ_BGR_SUPPORTED
   /* Flip the RGB pixels to BGR (or RGBA to BGRA) */
   if (transforms & PNG_TRANSFORM_BGR) {
      png_warning_2(png_ptr, "transform bgr");
      png_set_bgr_2(png_ptr);
   }
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
   /* Swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */
   if (transforms & PNG_TRANSFORM_SWAP_ALPHA) {
      png_warning_2(png_ptr, "transform swap alpha");
      png_set_swap_alpha_2(png_ptr);
   }
#endif

#ifdef PNG_READ_SWAP_SUPPORTED
   /* Swap bytes of 16 bit files to least significant byte first */
   if (transforms & PNG_TRANSFORM_SWAP_ENDIAN)
      png_set_swap_2(png_ptr);
#endif

/* Added at libpng-1.2.41 */
#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
   /* Invert the alpha channel from opacity to transparency */
   if (transforms & PNG_TRANSFORM_INVERT_ALPHA) {
      png_warning_2(png_ptr, "transform invert alpha");
      png_set_invert_alpha_2(png_ptr);  
   }
#endif

/* Added at libpng-1.2.41 */
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   /* Expand grayscale image to RGB */
   if (transforms & PNG_TRANSFORM_GRAY_TO_RGB) {
      png_warning_2(png_ptr, "transform gray to rgb");
      png_set_gray_to_rgb_2(png_ptr);
   }
#endif

   /* We don't handle adding filler bytes */

   /* Optional call to gamma correct and add the background to the palette
    * and update info structure.  REQUIRED if you are expecting libpng to
    * update the palette for you (i.e., you selected such a transform above).
    */
   png_warning_2(png_ptr, "read update info");
   png_read_update_info_2(png_ptr, info_ptr);
   png_warning_2(png_ptr, "image transformations end here");

   /* -------------- image transformations end here ------------------- */

   png_free_data_2(png_ptr, info_ptr, PNG_FREE_ROWS, 0);
   png_warning_2(png_ptr, "data freed");
   if (info_ptr->row_pointers == NULL)
   {
      
      info_ptr->row_pointers = (png_bytepp)png_malloc_2(png_ptr,
          info_ptr->height * png_sizeof(png_bytep));
      for (iptr=0; iptr<info_ptr->height; iptr++)
         info_ptr->row_pointers[iptr] = NULL;

      info_ptr->free_me |= PNG_FREE_ROWS;

      for (row = 0; row < (int)info_ptr->height; row++)
         info_ptr->row_pointers[row] = (png_bytep)png_malloc_2(png_ptr,
            png_get_rowbytes_2(png_ptr, info_ptr));
   }
   png_warning_2(png_ptr, "png read image");
   png_read_image_2(png_ptr, info_ptr->row_pointers);
   info_ptr->valid |= PNG_INFO_IDAT;

   /* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
   //masalah disini
   png_read_end_2(png_ptr, info_ptr);

   //PNG_UNUSED(transforms)   /* Quiet compiler warnings */
   //PNG_UNUSED(params)

}
#endif /* PNG_INFO_IMAGE_SUPPORTED */
    
Shell_Object * CreatePicViewer(void) {
	Shell_Object * parent;
	uchar buf_head[8];
	uint32 length;
	uchar is_png;
	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_ptr;
	Heap_Manager * png_heap;
	Heap_Manager * pic_heap;
	void * temp_ptr;
	png_byte * row_pointers[480];
	efat_FileSystem * fs;
	efat_File * file;
	uint32 i, j;
	uchar r, g, b;
	uint16 * background;
	uint32 p_width, p_height, p_bdepth, p_ctype;
	init_sd_card(&_osc._fm_drives[0]);
	//Uart_Printf("sd driver initialized\n");
	fs = efat_Init(0, &_osc._fm_drives[0]);
	file = efat_Open("test.png", fs, "r");
	if(file == NULL) {
		Uart_Printf("file not found\n");
		return NULL;
	}
	Uart_Printf("file found\n");
	//efat_Read(512, myFile, (uchar *)buf);
	length = file_Read(8, file, buf_head);
	if(length != 8) {
		return NULL;
	}
	//memcmp("aaaa", buf_head, 3);
	is_png = !png_sig_cmp_2(buf_head, 0, 8);
	if(!is_png) {
		Uart_Printf("this is not a png file\n");
		return NULL;
	}
	Uart_Printf("this is png file\n");
	
	png_heap = m_create_heap(1024 * 2048);
	//usr_alloc(NULL, 20);
	Uart_Printf("create read struct\n");
	png_ptr = png_create_read_struct_3(PNG_LIBPNG_VER_STRING, 
	(void *)temp_ptr, png_user_error, png_user_warning,
	(void *)png_heap, usr_alloc, usr_release);
	if(png_ptr == NULL) {
		return NULL;
	}
	Uart_Printf("create info struct\n");
	info_ptr = png_create_info_struct_2(png_ptr);
	if(info_ptr == NULL) {
		png_heap = png_get_mem_ptr_2(png_ptr);
		//png_destroy_read_struct(&png_ptr, NULL, NULL);
		m_delete_heap(png_heap);
		return NULL;
	}
	end_ptr = png_create_info_struct_2(png_ptr);
	if(end_ptr == NULL) {
		png_heap = png_get_mem_ptr_2(png_ptr);
		//png_destroy_read_struct(&png_ptr, info_ptr, NULL);
		m_delete_heap(png_heap);
		return NULL;
	}
	Uart_Printf("info created\n");
	png_set_read_fn_2(png_ptr, file, usr_read_data);
	png_set_sig_bytes_2(png_ptr, 8);
	png_set_user_limits_2(png_ptr, 640, 480);
	//row_pointers = usr_alloc(png_ptr, png_ptr->height * sizeof(png_byte *));
	Uart_Printf("allocating rows pointer\n");
	/*for(i=0;i<png_ptr->height;i++) {
		row_pointers[i] = NULL;
	}*/
	for(i=0;i<320;i++) {
	    row_pointers[i] = usr_alloc(png_ptr, png_ptr->width * 4);
	    Uart_Printf("%d\n", row_pointers[i]);
	}
	//use high level interface
	Uart_Printf("use high level interface\n");
	png_read_png_2(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_SWAP_ENDIAN, NULL);
	Uart_Printf("png decoded\n");
	p_width = png_get_image_width_2(png_ptr, info_ptr);
	p_height = png_get_image_height_2(png_ptr, info_ptr); 
	p_bdepth = png_get_bit_depth_2(png_ptr, info_ptr);
	p_ctype = png_get_color_type_2(png_ptr, info_ptr);
	Uart_Printf("allocating background\n");
	//background = m_alloc(320 * 240 * sizeof(uint16));
	png_read_image_2(png_ptr, row_pointers);
	pic_heap = m_create_heap((320 * 240 * sizeof(uint16)) + 1024);
	background = m_heap_alloc(pic_heap, 320 * 240 * sizeof(uint16));
	(p_height > 320)?(p_height = 320):(p_height = p_height);
	(p_width > 240)?(p_width = 240):(p_width = p_width);
	Uart_Printf("rendering image\n");
	for(j=0;j<p_height;j++) {
		for(i=0;i<p_width;i++) {
			r = *(uchar *)((uint32)row_pointers[j] + (3 * i));
			g = *(uchar *)((uint32)row_pointers[j] + ((3 * i) + 1));
			b = *(uchar *)((uint32)row_pointers[j] + ((3 * i) + 2));
			r >>= 3;
			g >>= 2;
			b >>= 3;
			/*r = *(uchar *)((uint32)row_pointers[j] + i);
			b = r << 2;
			g = b << 2;*/
			background[(j * 240) + i] = (uint16)((r<<11)|(g<<5)|b); 
		}
	}
	//Uart_Printf("read image\n");
	//png_read_image_2(png_ptr, row_pointers);
	Uart_Printf("png decoding success\n");
	//m_alloc(2500);
	parent = CreateFrame("Viewer", background);
	/*Uart_Printf("cleaning up\n");
	for(i=0;i<320;i++) {
		//row_pointers[i] = usr_alloc(png_ptr, png_ptr->width * 4);
		usr_release(png_ptr, row_pointers[i]);
	}
	png_destroy_struct_3(png_ptr, usr_release, (void *)png_heap);*/
	return parent;
}



