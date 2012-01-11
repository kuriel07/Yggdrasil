/***************************************************************/
/*eFAT : elvish FAT Library                                    */
/*Filename : ioman.h                                           */
/*Copyright (C) 2011 Agus Purwanto                             */
/*http://www.my.opera.com/kuriel                               */
/***************************************************************/
#include "defs.h"
//for testing purpose use offline file, include stdio
#include <stdio.h>
#ifndef __EFAT_IOMAN_H
//standard interface to any storage hardware
typedef struct HW_Interface HW_Interface;
typedef struct HW_Interface {
	uchar id;
	uint16 rca;
	uchar (* init)(HW_Interface * interface);
	void (* readblock)(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size);
	uint32 (* writeblock)(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size);
}  HW_Interface;

uchar ioman_init(HW_Interface * interface);
void ioman_readblock(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size);
void ioman_writeblock(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size);

#define __EFAT_IOMAN_H
#endif
