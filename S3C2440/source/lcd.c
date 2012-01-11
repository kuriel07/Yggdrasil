/**************************************************************
The initial and control for 320×240 16Bpp TFT LCD----LCD_LTV350QV-F04
**************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 
#include "lcd.h"
#include "defs.h"
#include "yggdrasil.h"

// GPB1/TOUT1 for Backlight control(PWM)
#define GPB1_TO_OUT()   (rGPBUP &= 0xfffd, rGPBCON &= 0xfffffff3, rGPBCON |= 0x00000004)
#define GPB1_TO_1()     (rGPBDAT |= 0x0002)
#define GPB1_TO_0()     (rGPBDAT &= 0xfffd)
#define	M5D(n)			((n)&0x1fffff)
//extern void Uart_Printf(char *f, ...) ;

extern unsigned char __CHS[];
extern unsigned char __VGA[];

//volatile static
//unsigned short LCD_BUFFER[SCR_YSIZE_TFT][SCR_XSIZE_TFT];
uint16 * _LCD_BUFFER;

/**************************************************************
320×240 16Bpp TFT LCD功能模块初始化
**************************************************************/
void Lcd_PowerEnable(int invpwren,int pwren);
void Lcd_Init(uint16 * lcdbuffer)
{
	rLCDCON1 = (LCD_PIXCLOCK << 8) | (3 <<  5) | (12 << 1);
   	rLCDCON2 = (LCD_UPPER_MARGIN << 24) | ((LCD_HEIGHT - 1) << 14) | (LCD_LOWER_MARGIN << 6) | (LCD_VSYNC_LEN << 0);
   	rLCDCON3 = (LCD_RIGHT_MARGIN << 19) | ((LCD_WIDTH  - 1) <<  8) | (LCD_LEFT_MARGIN << 0);
   	rLCDCON4 = (13 <<  8) | (LCD_HSYNC_LEN << 0);

	#if !defined(LCD_CON5)
	#    define LCD_CON5 ((1<<11) | (1 << 9) | (1 << 8) | (1 << 3) | (1 << 0))
	#endif
    rLCDCON5   =  LCD_CON5;
	
	//Set_PaintBuffer(lcdbuffer);
	//Lcd_Present(lcdbuffer);  
	_LCD_BUFFER = lcdbuffer;
	//change display buffer
    rLCDSADDR1 = (((uint32)lcdbuffer >> 22) << 21) | ((M5D((uint32)lcdbuffer >> 1)) <<  0);
    rLCDSADDR2 = M5D(((uint32)lcdbuffer + LCD_WIDTH * LCD_HEIGHT * 2) >> 1);
    rLCDSADDR3 = LCD_WIDTH;      

    rLCDINTMSK |= 3;
  	rTCONSEL   &= (~7);
 

   	rTPAL     = 0x0;
   	rTCONSEL &= ~((1<<4) | 1);
}

/**************************************************************
LCD视频和控制信号输出或者停止，1开启视频输出
**************************************************************/
void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
	rLCDCON1|=1; // ENVID=ON
    else
	rLCDCON1 =rLCDCON1 & 0x3fffe; // ENVID Off
}

/**************************************************************
320×240 16Bpp TFT LCD 电源控制引脚使能
**************************************************************/
void Lcd_PowerEnable(int invpwren,int pwren)
{
    //GPG4 is setted as LCD_PWREN
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4); // Pull-up disable
    rGPGCON=rGPGCON&(~(3<<8))|(3<<8); //GPG4=LCD_PWREN
    rGPGDAT = rGPGDAT | (1<<4) ;
//	invpwren=pwren;
    //Enable LCD POWER ENABLE Function
    rLCDCON5=rLCDCON5&(~(1<<3))|(pwren<<3);   // PWREN
    rLCDCON5=rLCDCON5&(~(1<<5))|(invpwren<<5);   // INVPWREN
}

/**************************************************************
320×240 16Bpp TFT LCD移动观察窗口
**************************************************************/
void Lcd_MoveViewPort(int vx,int vy)
{
    uint32 addr;

    SET_IF(); 
	#if (LCD_TFT_XSIZE<32)
    	    while((rLCDCON1>>18)<=1); // if x<32
	#else	
    	    while((rLCDCON1>>18)==0); // if x>32
	#endif
    
    addr=(uint32)_LCD_BUFFER+(vx*2)+vy*(SCR_XSIZE_TFT*2);
	rLCDSADDR1= ( (addr>>22)<<21 ) | M5D(addr>>1);
	rLCDSADDR2= M5D(((addr+(SCR_XSIZE_TFT*LCD_TFT_YSIZE*2))>>1));
	CLR_IF();
}    

/**************************************************************
320×240 16Bpp TFT LCD移动观察窗口
**************************************************************/
static void MoveViewPort(void)
{
    int vx=0,vy=0,vd=1;

    Uart_Printf("\n*Move the LCD view windos:\n");
    Uart_Printf(" press 8 is up\n");
    Uart_Printf(" press 2 is down\n");
    Uart_Printf(" press 4 is left\n");
    Uart_Printf(" press 6 is right\n");
    Uart_Printf(" press Enter to exit!\n");

    while(1)
    {
    	switch( Uart_GetKey() )
    	{
    	case '8':
	    if(vy>=vd)vy-=vd;    
	    Uart_Printf("vx=%3d,vy=%3d\n",vx,vy);
		Lcd_MoveViewPort(vx,vy);	   	
        break;

    	case '4':
    	    if(vx>=vd)vx-=vd;
    	    Uart_Printf("vx=%3d,vy=%3d\n",vx,vy);
			Lcd_MoveViewPort(vx,vy);	
    	break;

    	case '6':
                if(vx<=(SCR_XSIZE_TFT-LCD_TFT_XSIZE-vd))vx+=vd;  
                 Uart_Printf("vx=%3d,vy=%3d\n",vx,vy);
			    Lcd_MoveViewPort(vx,vy);	 	    
   	    break;

    	case '2':
                if(vy<=(SCR_YSIZE_TFT-LCD_TFT_YSIZE-vd))vy+=vd; 
                 Uart_Printf("vx=%3d,vy=%3d\n",vx,vy);
			   Lcd_MoveViewPort(vx,vy);	  	    
   	    break;

    	case '\r':
   	    return;

    	default:
	    break;
		}
	//Uart_Printf("vx=%3d,vy=%3d\n",vx,vy);
	//Lcd_MoveViewPort(vx,vy);
    }
}

/**************************************************************
320×240 16Bpp TFT LCD单个象素的显示数据输出
**************************************************************/
void PutPixel(uint32 x, uint32 y, uint16 c )
{
	if ( (x < SCR_XSIZE_TFT) && (y < SCR_YSIZE_TFT) )
		*(_LCD_BUFFER + (y * SCR_XSIZE_TFT) + x) = c;
}
 
void GUI_Point(uint32 x,uint32 y, uint16 c )
{
	if ( (x < SCR_XSIZE_TFT) && (y < SCR_YSIZE_TFT) )
		*(_LCD_BUFFER + (y * SCR_XSIZE_TFT) + x) = c;
}

/**************************************************************
320×240 16Bpp TFT LCD全屏填充特定颜色单元或清屏
**************************************************************/
void Lcd_ClearScr( uint16 c)
{
	unsigned int x,y ;
		
    for( y = 0 ; y < SCR_YSIZE_TFT ; y++ )
    {
    	for( x = 0 ; x < SCR_XSIZE_TFT ; x++ )
    	{
			*(_LCD_BUFFER + (y * SCR_XSIZE_TFT) + x) = c ;
    	}
    }
}

/**************************************************************
LCD屏幕显示垂直翻转
// LCD display is flipped vertically
// But, think the algorithm by mathematics point.
//   3I2
//   4 I 1
//  --+--   <-8 octants  mathematical cordinate
//   5 I 8
//   6I7
**************************************************************/
void Glib_Line(int x1,int y1,int x2,int y2,int color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}

/**************************************************************
在LCD屏幕上画一个矩形
**************************************************************/
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color)
{
    Glib_Line(x1,y1,x2,y1,color);
    Glib_Line(x2,y1,x2,y2,color);
    Glib_Line(x1,y2,x2,y2,color);
    Glib_Line(x1,y1,x1,y2,color);
}

/**************************************************************
在LCD屏幕上用颜色填充一个矩形
**************************************************************/
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color)
{
    int i;

    for(i=y1;i<=y2;i++)
	Glib_Line(x1,i,x2,i,color);
}

/**************************************************************
在LCD屏幕上指定坐标点画一个指定大小的图片
**************************************************************/
void Lcd_Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[])
{
	int x,y;
	uint32 c;
	int p = 0;
	
    for( y = y0 ; y < l ; y++ )
    {
    	for( x = x0 ; x < h ; x++ )
    	{
    		c = bmp[p+1] | (bmp[p]<<8) ;

			if ( ( (x0+x) < SCR_XSIZE_TFT) && ( (y0+y) < SCR_YSIZE_TFT) )
				*(_LCD_BUFFER + ((y0+y) * SCR_XSIZE_TFT) + (x0+x)) = c ;
			
    		p = p + 2 ;
    	}
    }
}

/*void Buffer_Paint_Bmp(uint16 x, uint16 y, uint16 h, uint16 l, uchar * buffer, uchar bmp[]) {
	int x1,y1;
	uint32 c;
	int p = 0;
    for( y1 = y ; y1 < l ; y1++ ){
    	for( x1 = x ; x1 < h ; x1++ ){
    		c = bmp[p+1] | (bmp[p]<<8) ;
			if ( ( (x+x1) < SCR_XSIZE_TFT) && ( (y+y1) < SCR_YSIZE_TFT) )
				*(buffer + ((y+y1) * SCR_XSIZE_TFT) + (x+x1)) = c ;
    		p = p + 2 ;
    	}
    }
}*/

/*void Buffer_Paint_Bmp(int16 x, int16 y, uint16 h, uint16 l, uchar * buffer, Bitmap * bmp)
{
	int16 x1, y1;
	uint32 c;
	uint16 p = 0;
    for( y1 = 0 ; y1 < l ; y1++ )
    {
    	for( x1 = 0 ; x1 < h ; x1++ )
    	{
    		c = bmp->image[p+1] | (bmp->image[p]<<8) ;
			if ( ((uint16)(x1+x) < SCR_XSIZE_TFT) && ((uint16)(y1+y) < SCR_YSIZE_TFT) &&  (c ^ bmp->property.key))
				*(buffer + ((y+y1) * SCR_XSIZE_TFT) + (x+x1)) = c ;
    		p = p + 2 ;
    	}
    }
}*/

#if 1
/**************************************************************
在LCD屏幕上指定坐标点写ASCII码
**************************************************************/
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;
	
	pZK = &__VGA[ch*16];
	for( i = 0 ; i < 16 ; i++ )
	{
		mask = 0x80;
		buf = pZK[i];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j,y+i,bk_c);
                }
            }
            mask = mask >> 1;
        }
	}
}

/**************************************************************
在LCD屏幕上指定坐标点写汉字
**************************************************************/
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;

	pZK = &__CHS[ (  ( (QW >> 8) - 1 )*94 + (QW & 0x00FF)- 1 )*32 ];
	for( i = 0 ; i < 16 ; i++ )
	{
		//左
		mask = 0x80;
        buf = pZK[i*2];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j,y+i,bk_c);
                }
            }
            mask = mask >> 1;
        } 
        
		//右
		mask = 0x80;
        buf = pZK[i*2 + 1];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j + 8,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j + 8,y+i,bk_c);
                }
            }
            mask = mask >> 1;
        }                 
	}
}

void Buffer_PrintText(uchar length, int16 x, int16 y, uint16 forecolor, uint16 * buffer, const char *fmt,...)
{
	char __LCD_Printf_Buf[256];
    unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;
    va_list ap;
	
    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
    while(*pStr != 0)
	{
		switch(*pStr)
		{
			case '\n':
        		break;
			default:
				Buffer_PutASCII(x, y, *pStr, forecolor, buffer);
				x += 8;
				break;
		}
		pStr++;
        i++;		
        if( i > length ) break;
	}
}

void Buffer_PrintTextVar(uchar length, int16 x, int16 y, uint16 forecolor, uint16 xsize, uint16 ysize, uint16 * buffer, const char *fmt,...)
{
	char __LCD_Printf_Buf[256];
    unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;
    va_list ap;
	
    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
    while(*pStr != 0)
	{
		switch(*pStr)
		{
			case '\n':
        		break;
			default:
				Buffer_PutASCIIVar(x, y, *pStr, forecolor, buffer, xsize, ysize);
				x += 8;
				break;
		}
		pStr++;
        i++;		
        if( i > length ) break;
	}
}

void Buffer_PutChar(int16 x, int16 y, uchar c, uint16 forecolor, uint16 backcolor, uint16 * buffer, uint16 xsize, uint16 ysize)
{
	Buffer_PutASCIIBackVar(x, y, c, forecolor, backcolor, buffer, xsize, ysize);
}

void Buffer_PrintfColor(uchar length, uint16 color, int16 x, int16 y, uint16 * buffer, const char *fmt,...)
{
	char __LCD_Printf_Buf[256];
    unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;
    va_list ap;
	
    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
    
    while(*pStr != 0 )
	{
		switch(*pStr)
		{
			case '\n':
        		break;
			default:
				Buffer_PutASCII(x, y, *pStr, color, buffer);
				x += 8;
				break;
		}
		pStr++;
        i++;		
        if( i > length ) break;
	}
}

void Buffer_Printf(uchar length, int16 x, int16 y, uint16 * buffer, const char *fmt,...)
{
	char __LCD_Printf_Buf[256];
    unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;
    va_list ap;
	
    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
    
    while(*pStr != 0 )
	{
		switch(*pStr)
		{
			case '\n':
        		break;
			default:
				Buffer_PutASCII(x, y, *pStr, 0xffff, buffer);
				x += 8;
				break;
		}
		pStr++;
        i++;		
        if( i > length ) break;
	}
}

//----------------------
void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...)
{
    char __LCD_Printf_Buf[256];
    va_list ap;
	unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;

    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
	 
    while(*pStr != 0 )
	{
		switch(*pStr)
		{
			case '\n' :
				{
			
                    break;
				}

			default:
				{
					if( *pStr > 0xA0 & *(pStr+1) > 0xA0 )  //中文输出
                    {
                        Lcd_PutHZ( x , y , (*pStr - 0xA0)*0x0100 + *(pStr+1) - 0xA0 , c , bk_c , st);

                        pStr++;
                        i++;

                        x += 16;
                    }else               //英文输出
                    {
                        Lcd_PutASCII( x , y , *pStr , c , bk_c , st );

                        x += 8;

                    }

                    break;
				}
		}
		
		pStr++;
        i++;		

        if( i > 256 ) break;
	}
   
}
#endif

void Lcd_Present(uint16 * lcdbuffer)
{
	uint32 stat;
	//rLCDCON1 = rLCDCON1 & 0x3fffe; // ENVID Off	//disable video
	//while(rLCDCON1 < 0x00100000);
	stat = rLCDCON1>>18;
	while(stat < 1) {
		stat = rLCDCON1>>18;
	}
	_LCD_BUFFER = lcdbuffer;
	//change display buffer
    rLCDSADDR1 = (((uint32)lcdbuffer >> 22) << 21) | ((M5D((uint32)lcdbuffer >> 1)) <<  0);
    rLCDSADDR2 = M5D(((uint32)lcdbuffer + LCD_WIDTH * LCD_HEIGHT * 2) >> 1);
    rLCDSADDR3 = LCD_WIDTH;
	//rLCDCON1 |= 1; // ENVID=ON
}

/*void Buffer_Copy_Key(uint16 key, int16 startx, uint16 marginx, uint16 * buffer, uint16 * image, size_t size)
{
	uint16 c;
	while(size>0)
	{
		c = *(image + marginx++);
		if ((c ^ key) != 0) {
			*(buffer + startx) = c ;
		}
		startx++;
		size--;
	}
}*/
	

