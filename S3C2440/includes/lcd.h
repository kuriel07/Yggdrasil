/**************************************************************
The initial and control for 240¡Á320 16Bpp TFT LCD----LtS350Q1
**************************************************************/
#include "defs.h"
#include "option.h"
#include "yggdrasil.h"
#include "midgard.h"

#define LCD_TFT_XSIZE 	LCD_WIDTH	
#define LCD_TFT_YSIZE 	LCD_HEIGHT

#define SCR_XSIZE_TFT 	LCD_WIDTH
#define SCR_YSIZE_TFT 	LCD_HEIGHT

extern uint16 * _LCD_BUFFER;

void Lcd_Init(uint16 * lcdbuffer);
void Lcd_EnvidOnOff(int onoff);
void Lcd_PowerEnable(int invpwren,int pwren);
void Lcd_MoveViewPort(int vx,int vy);
void Lcd_Present(uint16 * lcdbuffer);

void Lcd_ClearScr( uint16 c);
void PutPixel(uint32 x, uint32 y, uint16 c );
void GUI_Point(uint32 x,uint32 y, uint16 c );
//void PutPixel(unsigned int x , unsigned int y , unsigned int c);
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_Rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_FilledRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_Line(int x1,int y1,int x2,int y2,int color);
//void Glib_Line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Lcd_Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[]);
//buffer text print based on Uart_Printf
void Buffer_PrintText(uchar length, int16 x, int16 y, uint16 forecolor, uint16 * buffer, const char *fmt,...);
void Buffer_PrintfColor(uchar length, uint16 color, int16 x, int16 y, uint16 * buffer, const char *fmt,...);
void Buffer_Printf(uchar length, int16 x, int16 y, uint16 * buffer, const char *fmt,...);
void f_fast_mem_blit(uint16 key, uint16 * dst, const uint16 * src, size_t size);
void Buffer_PrintTextVar(uchar length, int16 x, int16 y, uint16 forecolor, uint16 xsize, uint16 ysize, uint16 * buffer, const char *fmt,...);
void Buffer_PutChar(int16 x, int16 y, uchar c, uint16 forecolor, uint16 backcolor, uint16 * buffer, uint16 xsize, uint16 ysize);

//void Buffer_Paint_Bmp(Rect r, unsigned char buffer[], unsigned char bmp[]);
//void Buffer_Copy_Key(uint16 key, int16 startx, uint16 marginx, uint16 * buffer, uint16 * image, size_t size);
//void Buffer_Paint_Bmp(int16 x, int16 y, uint16 h, uint16 l, uchar * buffer, Bitmap * bmp);
//paint display buffer with specified bitmap file 16bpp with image transparency (key color)
/*#define Buffer_Copy_Key(key, startx, marginx, buffer, image, size)\
{\
	uint16 c;\
	uint16 j, k = (size % 8);\
	k = (size % 8);\
	for(j=0; j<k; j++) {\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		size--;\
	}\
	for(j=0;j<size;)\
	{\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		c = *(image + marginx++);\
		if ((c ^ key) != 0) {\
			*(buffer + startx) = c ;\
		}\
		startx++;\
		j+=8;\
	}\
}*/

#define Buffer_Paint_Bmp_Key_Var(x, y, h, l, xsize, ysize, buffer, bmp) {\
	int16 x1, y1;\
	int16 x2, y2;\
	int16 startx = x, starty = y;\
	int16 marginx = 0, marginy = 0;\
	int16 endx = (x+h), endy = (y+l);\
	uint16 c;\
	uint32 posy_img, posy_buf;\
	int p = 0;\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++) marginx++;\
		endx = endx;\
	}\
	if(endx > xsize) {\
		for(; endx > xsize; endx--);\
		if(startx > xsize)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++) marginy++;\
		endy = endy;\
	}\
	if(endy > ysize) {\
		for(; endy > ysize; endy--);\
		if(starty > ysize)\
			starty = endy;\
	}\
	if(bmp->image == NULL) {\
		starty = endy;\
		startx = endx;\
	}\
    for( y1 = starty, y2 = marginy; y1 < endy ; y1++, y2++){\
    	p = (y2 * bmp->property.width);\
    	posy_buf = (y1 * xsize);\
    	/*f_fast_mem_blit(bmp->property.key, (uint16 *)(buffer + (posy_buf + startx)), (uint16 *)(bmp->image + (( p + marginx) << 1)), (endx - startx));*/\
    	for( x1 = startx, p = (p+marginx); x1 < endx ; x1++, p++){\
    		c = *(uint16 *)(bmp->image + p);\
			if ((c ^ bmp->property.key) != 0) {\
				*(uint16 *)(buffer + posy_buf + x1) = c ;\
			}\
		}\
    	/*Buffer_Copy_Key(bmp->property.key, startx, (p+marginx), (buffer + posy_buf), bmp->image, endx - startx);*/\
    }\
}

#define Buffer_Paint_Bmp_Key(x, y, h, l, buffer, bmp) {\
	int16 x1, y1;\
	int16 x2, y2;\
	int16 startx = x, starty = y;\
	int16 marginx = 0, marginy = 0;\
	int16 endx = (x+h), endy = (y+l);\
	uint16 c;\
	uint32 posy_img, posy_buf;\
	int p = 0;\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++) marginx++;\
		endx = endx;\
	}\
	if(endx > SCR_XSIZE_TFT) {\
		for(; endx > SCR_XSIZE_TFT; endx--);\
		if(startx > SCR_XSIZE_TFT)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++) marginy++;\
		endy = endy;\
	}\
	if(endy > SCR_YSIZE_TFT) {\
		for(; endy > SCR_YSIZE_TFT; endy--);\
		if(starty > SCR_YSIZE_TFT)\
			starty = endy;\
	}\
	if(bmp->image == NULL) {\
		starty = endy;\
		startx = endx;\
	}\
    for( y1 = starty, y2 = marginy; y1 < endy ; y1++, y2++){\
    	p = (y2 * bmp->property.width);\
    	posy_buf = (y1 * SCR_XSIZE_TFT);\
    	/*f_fast_mem_blit(bmp->property.key, (uint16 *)(buffer + (posy_buf + startx)), (uint16 *)(bmp->image + (( p + marginx) << 1)), (endx - startx));*/\
    	for( x1 = startx, p = (p+marginx); x1 < endx ; x1++, p++){\
    		c = *(uint16 *)(bmp->image + p);\
			if ((c ^ bmp->property.key) != 0) {\
				*(uint16 *)(buffer + posy_buf + x1) = c ;\
			}\
		}\
    	/*Buffer_Copy_Key(bmp->property.key, startx, (p+marginx), (buffer + posy_buf), bmp->image, endx - startx);*/\
    }\
}

/*
for( x1 = startx, p = (p+marginx)<<1; x1 < endx ; x1++, p+=2){\
    		c = *(uint16 *)(bmp->image + p);\
			if ((c ^ bmp->property.key) != 0) {\
				*(uint16 *)(buffer + posy_buf + x1) = c ;\
			}\
    	}\
*/
#define Buffer_Paint_Bmp_Alpha(x, y, h, l, buffer, bitmap) {\
	int16 x1, y1;\
	int16 x2, y2;\
	uchar r, g, b;\
	int16 startx = x, starty = y;\
	int16 marginx = 0, marginy = 0;\
	int16 endx = (x+h), endy = (y+l);\
	uint16 c;\
	uint16 c2;\
	uint32 posy_img, posy_buf;\
	uchar alpha = bitmap->property.alpha;\
	int p = 0;\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++) marginx++;\
		endx = endx;\
	}\
	if(endx > SCR_XSIZE_TFT) {\
		for(; endx > SCR_XSIZE_TFT; endx--);\
		if(startx > SCR_XSIZE_TFT)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++) marginy++;\
		endy = endy;\
	}\
	if(endy > SCR_YSIZE_TFT) {\
		for(; endy > SCR_YSIZE_TFT; endy--);\
		if(starty > SCR_YSIZE_TFT)\
			starty = endy;\
	}\
    for( y1 = starty, y2 = marginy; y1 < endy ; y1++, y2++){\
    	p = (y2 * bitmap->property.width);\
    	posy_buf = (y1 * SCR_XSIZE_TFT);\
    	for( x1 = startx, p = (p+marginx); x1 < endx ; x1++, p++){\
    		c = *(uint16 *)(bitmap->image + p);\
			c2 = *(buffer + posy_buf + x1);\
			r = (((100 - alpha) * (uchar)(c2 >> 11)) + (alpha * (uchar)(c >> 11))) / 100;\
			g = (((100 - alpha) * (uchar)((c2 >> 5) & 0x3f)) + (alpha * (uchar)((c >> 5) & 0x3f))) / 100;\
			b = (((100 - alpha) * (uchar)(c2 & 0x1f)) + (alpha * (uchar)(c & 0x1f))) / 100;\
			*(buffer + posy_buf + x1) = (uint16)((r<<11)|(g<<5)|b);\
    	}\
    }\
}

#define Buffer_Paint_Inactive(x, y, h, l, buffer) {\
	int16 x1, y1;\
	int16 x2, y2;\
	uchar r, g, b;\
	int16 startx = x, starty = y;\
	int16 marginx = 0, marginy = 0;\
	int16 endx = (x+h), endy = (y+l);\
	uint16 c;\
	uint16 c2;\
	uint32 posy_img, posy_buf;\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++) marginx++;\
		endx = endx;\
	}\
	if(endx > SCR_XSIZE_TFT) {\
		for(; endx > SCR_XSIZE_TFT; endx--);\
		if(startx > SCR_XSIZE_TFT)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++) marginy++;\
		endy = endy;\
	}\
	if(endy > SCR_YSIZE_TFT) {\
		for(; endy > SCR_YSIZE_TFT; endy--);\
		if(starty > SCR_YSIZE_TFT)\
			starty = endy;\
	}\
    for( y1 = starty, y2 = marginy; y1 < endy ; y1++, y2++) {\
    	posy_buf = (y1 * SCR_XSIZE_TFT);\
    	for( x1 = startx; x1 < endx ; x1++) {\
			c2 = *(buffer + posy_buf + x1);\
			*(buffer + posy_buf + x1) = (uint16)((c2>>1) & 0xfbef);\
    	}\
    }\
}

#define Buffer_Paint_Bmp_AlphaKey_Var(x, y, h, l, xsize, ysize, buffer, bitmap) {\
	int16 x1, y1;\
	int16 x2, y2;\
	uchar r, g, b;\
	int16 startx = x, starty = y;\
	int16 marginx = 0, marginy = 0;\
	int16 endx = (x+h), endy = (y+l);\
	uint16 c;\
	uint16 c2;\
	uint32 posy_img, posy_buf;\
	uchar alpha = bitmap->property.alpha;\
	int p = 0;\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++) marginx++;\
		endx = endx;\
	}\
	if(endx > xsize) {\
		for(; endx > xsize; endx--);\
		if(startx > xsize)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++) marginy++;\
		endy = endy;\
	}\
	if(endy > ysize) {\
		for(; endy > ysize; endy--);\
		if(starty > ysize)\
			starty = endy;\
	}\
    for( y1 = starty, y2 = marginy; y1 < endy ; y1++, y2++){\
    	p = (y2 * bitmap->property.width);\
    	posy_buf = (y1 * xsize);\
    	for( x1 = startx, p = (p+marginx); x1 < endx ; x1++, p++){\
    		c = *(uint16 *)(bitmap->image + p);\
			if(c ^ bitmap->property.key) {\
			c2 = *(buffer + posy_buf + x1);\
				r = (((100 - alpha) * (uchar)(c2 >> 11)) + (alpha * (uchar)(c >> 11))) / 100;\
				g = (((100 - alpha) * (uchar)((c2 >> 5) & 0x3f)) + (alpha * (uchar)((c >> 5) & 0x3f))) / 100;\
				b = (((100 - alpha) * (uchar)(c2 & 0x1f)) + (alpha * (uchar)(c & 0x1f))) / 100;\
				*(buffer + posy_buf + x1) = (uint16)((r<<11)|(g<<5)|b);\
			}\
    	}\
    }\
}

#define Buffer_Paint_Bmp_AlphaKey(x, y, h, l, buffer, bitmap) {\
	int16 x1, y1;\
	int16 x2, y2;\
	uchar r, g, b;\
	int16 startx = x, starty = y;\
	int16 marginx = 0, marginy = 0;\
	int16 endx = (x+h), endy = (y+l);\
	uint16 c;\
	uint16 c2;\
	uint32 posy_img, posy_buf;\
	uchar alpha = bitmap->property.alpha;\
	int p = 0;\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++) marginx++;\
		endx = endx;\
	}\
	if(endx > SCR_XSIZE_TFT) {\
		for(; endx > SCR_XSIZE_TFT; endx--);\
		if(startx > SCR_XSIZE_TFT)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++) marginy++;\
		endy = endy;\
	}\
	if(endy > SCR_YSIZE_TFT) {\
		for(; endy > SCR_YSIZE_TFT; endy--);\
		if(starty > SCR_YSIZE_TFT)\
			starty = endy;\
	}\
    for( y1 = starty, y2 = marginy; y1 < endy ; y1++, y2++){\
    	p = (y2 * bitmap->property.width);\
    	posy_buf = (y1 * SCR_XSIZE_TFT);\
    	for( x1 = startx, p = (p+marginx); x1 < endx ; x1++, p++){\
    		c = *(uint16 *)(bitmap->image + p);\
			if(c ^ bitmap->property.key) {\
			c2 = *(buffer + posy_buf + x1);\
				r = (((100 - alpha) * (uchar)(c2 >> 11)) + (alpha * (uchar)(c >> 11))) / 100;\
				g = (((100 - alpha) * (uchar)((c2 >> 5) & 0x3f)) + (alpha * (uchar)((c >> 5) & 0x3f))) / 100;\
				b = (((100 - alpha) * (uchar)(c2 & 0x1f)) + (alpha * (uchar)(c & 0x1f))) / 100;\
				*(buffer + posy_buf + x1) = (uint16)((r<<11)|(g<<5)|b);\
			}\
    	}\
    }\
}

//void Buffer_Paint_Bmp(uint16 x, uint16 y, uint16 h, uint16 l, uchar * buffer, uchar * bmp);
#define Buffer_PutASCII(x, y, c, forecolor, buffer) {\
	uchar mask, buf;\
	int16 i,j;\
	int16 startx = x, starty = y;\
	int16 x1, y1;\
	int16 endx = (x+8), endy = (y+16);\
	uint32 posy_img, posy_buf;\
	uchar *pZK;\
	pZK = &__VGA[c*16];\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++);\
		endx = endx;\
	}\
	if(endx > SCR_XSIZE_TFT) {\
		for(; endx > SCR_XSIZE_TFT; endx--);\
		if(startx > SCR_XSIZE_TFT)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++);\
		endy = endy;\
	}\
	for( i = 0, y1 = starty; y1 < endy ; y1++, i++) {\
		mask = 0x80;\
		buf = pZK[i];\
		posy_buf = (y1 * SCR_XSIZE_TFT);\
        for( j = startx; j < endx ; j++) {\
            if( buf & mask ) {\
                *(buffer + posy_buf + j) = (uint16)forecolor;\
            }\
            mask = mask >> 1;\
        }\
	}\
}

#define Buffer_PutASCIIVar(x, y, c, forecolor, buffer, xsize, ysize) {\
	uchar mask, buf;\
	int16 i,j;\
	int16 startx = x, starty = y;\
	int16 x1, y1;\
	int16 endx = (x+8), endy = (y+16);\
	uint32 posy_img, posy_buf;\
	uchar *pZK;\
	pZK = &__VGA[c*16];\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++);\
		endx = endx;\
	}\
	if(endx > xsize) {\
		for(; endx > xsize; endx--);\
		if(startx > ysize)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++);\
		endy = endy;\
	}\
	if(endy > ysize) {\
		for(; endy > ysize; endy--);\
		if(starty > ysize)\
			starty = endy;\
	}\
	for( i = 0, y1 = starty; y1 < endy ; y1++, i++) {\
		mask = 0x80;\
		buf = pZK[i];\
		posy_buf = (y1 * xsize);\
        for( j = startx; j < endx ; j++) {\
            if( buf & mask ) {\
                *(buffer + posy_buf + j) = (uint16)forecolor;\
            }\
            mask = mask >> 1;\
        }\
	}\
}

#define Buffer_PutASCIIBackVar(x, y, c, forecolor, backcolor, buffer, xsize, ysize) {\
	uchar mask, buf;\
	int16 i,j;\
	int16 startx = x, starty = y;\
	int16 x1, y1;\
	int16 endx = (x+8), endy = (y+16);\
	uint32 posy_img, posy_buf;\
	uchar *pZK;\
	pZK = &__VGA[c*16];\
	/* preprocessing startx and endx for rendering */\
	if(startx < 0) {\
		for(; startx<0; startx++);\
		endx = endx;\
	}\
	if(endx > xsize) {\
		for(; endx > xsize; endx--);\
		if(startx > ysize)\
			startx = endx;\
	}\
	/* preprocessing starty and endy for rendering */\
	if(starty < 0) {\
		for(; starty<0; starty++);\
		endy = endy;\
	}\
	if(endy > ysize) {\
		for(; endy > ysize; endy--);\
		if(starty > ysize)\
			starty = endy;\
	}\
	for( i = 0, y1 = starty; y1 < endy ; y1++, i++) {\
		mask = 0x80;\
		buf = pZK[i];\
		posy_buf = (y1 * xsize);\
        for( j = startx; j < endx ; j++) {\
            if( buf & mask ) {\
                *(buffer + posy_buf + j) = (uint16)forecolor;\
            } else {\
            	*(buffer + posy_buf + j) = (uint16)backcolor;\
            }\
            mask = mask >> 1;\
        }\
	}\
}
//================================
// ×ÖÌåÊä³ö
//================================
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st); 
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st);  
void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...); 


