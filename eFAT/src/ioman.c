/***************************************************************/
/*eFAT : elvish FAT Library                                    */
/*Filename : ioman.c                                           */
/*Copyright (C) 2011 Agus Purwanto                             */
/*http://www.my.opera.com/kuriel                               */
/***************************************************************/
/*****************************************************************************/
//#include "interfaces/sd.h"
#include "ioman.h"
/*****************************************************************************/

uchar ioman_init(HW_Interface * interface) {
	return interface->init(interface);
	//Uart_Printf("no card detected\n");
}

/*****************************************************************************/

/* ****************************************************************************
 * WAIT ?? -- FIXME
 * CMDWRITE
 * WAIT
 * CARD RESP
 * WAIT
 * DATA BLOCK OUT
 *      START BLOCK
 *      DATA
 *      CHKS (2B)
 * BUSY...
 */
  
void ioman_writeblock(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size)
{
	interface->writeblock(interface, buffer, address, size);
	//return(0);
}
/*****************************************************************************/

/* ****************************************************************************
 * WAIT ?? -- FIXME
 * CMDCMD
 * WAIT
 * CARD RESP
 * WAIT
 * DATA BLOCK IN
 * 		START BLOCK
 * 		DATA
 * 		CHKS (2B)
 */

void ioman_readblock(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size)
{
	interface->readblock(interface, buffer, address, size);
	//Uart_Printf("read_sector\n");
	//return(0);
}



