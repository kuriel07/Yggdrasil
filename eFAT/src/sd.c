#include "defs.h"
#include "2440addr.h"
#include "sd.h"
#include "ioman.h"
#include "2440lib.h"

static uint16 card_out()
{
	return	(rGPGDAT & 0x0100);
}

void init_sd_card(HW_Interface * interface) {
	interface->init = sd_init;
	interface->readblock = sd_readblock;
	interface->writeblock = sd_writeblock;
}

uchar sd_init(HW_Interface * interface) {
	SD_Command scmd;
	uint16 i;
	uchar ret;
	sd_init_hw(interface);
	scmd.res = CMD_LONGRES;
	scmd.arg = 0;
	scmd.cmd = IO_SEND_OP_COND;
	scmd.response[0] = 0;
	scmd.response[1] = 0;
	scmd.response[2] = 0;
	scmd.response[3] = 0;
	if(SDSendCommand(&scmd, 0, 0, interface) == SCMD_TIMEOUT) {
		//goto init memory
		goto sd_init_memory;
	} 
	scmd.res = CMD_SHORTRES;
	scmd.arg = 0x00ff8000;	
	scmd.cmd = IO_SEND_OP_COND;
	if(SDSendCommand(&scmd, 0, 0, interface) == SCMD_SUCCESS) {
		Uart_Printf("response : %x\n", scmd.response[0]);
	}
	sd_init_memory:
	scmd.res = CMD_NORES;
	scmd.arg = 0;	
	scmd.cmd = GO_IDLE_STATE;
	SDSendCommand(&scmd, 0, 0, interface);
	for(i=0;i<15;i++)	//deteksi mmc
    {
		scmd.res = CMD_SHORTRES;
		scmd.arg = 0;	
		scmd.cmd = SEND_OP_COND;
		ret = SDSendCommand(&scmd, 0, 0, interface);
		if(ret == SCMD_SUCCESS) {
			Uart_Printf("mmc detected!\n");
			Uart_Printf("response : %x\n", scmd.response[0]);
			return 0;
		}
	}
	//deteksi sd
	negotiate_voltage:
	scmd.res = CMD_SHORTRES;
	scmd.arg = 0;	
	scmd.cmd = APP_CMD;
	ret = SDSendCommand(&scmd, 0, 0, interface);
	//negotiate_voltage:
	scmd.res = CMD_SHORTRES;
	scmd.arg = 0xff8000;	
	scmd.cmd = SD_APP_OP_COND;
    ret = SDSendCommand(&scmd, 0, 0, interface);
    if(ret != SCMD_TIMEOUT) { 
		if(scmd.response[0] != 0x80ff8000) {
			goto negotiate_voltage;
		} else {
			Uart_Printf("sd detected!\n");
			Uart_Printf("response : %x\n", scmd.response[0]);
			//initialize sd card	cmd2, cmd3
			scmd.res = CMD_LONGRES;		//command 2
			scmd.arg = 0;	
			scmd.cmd = ALL_SEND_CID;
			ret = SDSendCommand(&scmd, 0, 0, interface);
			if(ret == SCMD_TIMEOUT) {
				Uart_Printf("cmd2 timeout\n");
			}
			scmd.res = CMD_SHORTRES;		//command 3
			scmd.arg = (interface->rca<<16);	
			scmd.cmd = SEND_RELATIVE_ADDR;
			ret = SDSendCommand(&scmd, 0, 0, interface);
			if(ret == SCMD_TIMEOUT) {
				Uart_Printf("cmd3 timeout\n");
			}
			if((rSDICSTA & 0x7f) == SEND_RELATIVE_ADDR) {
				interface->rca = (scmd.response[0]>>16);
				Uart_Printf("RCA : %d\n", interface->rca);
			}
			Uart_Printf("response[0] : %x\n", scmd.response[0]);
			Uart_Printf("response[1] : %x\n", scmd.response[1]);
			Uart_Printf("response[2] : %x\n", scmd.response[2]);
			Uart_Printf("response[3] : %x\n", scmd.response[3]);
		}
		return 1;
	}
	return 0;
}


uchar SDSendCommand(struct SD_Command * scmd, void * buffer, uint32 size, HW_Interface * interface)
{
	uchar res = scmd->res; 
	uchar cmd = scmd->cmd; 
	uint32 arg = scmd->arg;
	uint32 cmd_config = 0x40;	//initialize host command
	uint32 dcon_config = 0x00;	//clear
	uint32 * w_buffer;
	uint16 * hw_buffer;
	uint8  * b_buffer;
	uint32 count = 0;
	uint32 c, w, i;
	//deteksi card
	if(card_out()) return SCMD_NODEV;
	//set argument register
	rSDICARG = arg;
	//response settings
	if(res & CMD_NORES) {
		cmd_config |= cmd ;
	} else {
		//wait response
		if(res & CMD_LONGRES) {
			//long response
			cmd_config |= ((cmd | SDICCON_WAITRES) | SDICCON_LONGRES);
		} else {
			//short response
			cmd_config |= (cmd | SDICCON_WAITRES);
		}
	}
	//data settings
	if(res & CMD_DATA) {
		//command sequence with data
		cmd_config |= SDICCON_WITHDAT;
		if(res & CMD_DATXMIT) {
			//set transmit configuration
			dcon_config = (((3<<SDIDCON_DATMODE) | ((res & 0xfc)<<SDIDCON_DATASIZE)) | SDIDCON_TARSP);
		} else {
			//set receive configuration
			dcon_config = (((2<<SDIDCON_DATMODE) | ((res & 0xfc)<<SDIDCON_DATASIZE)) | SDIDCON_RACMD);
		}
		//set bus width
		if(res & CMD_DATWIDE) {
			dcon_config |= SDIDCON_WIDEBUS;	
		}
		//block data transfer
		if(res & CMD_DATBLOCK) {
			dcon_config |= SDIDCON_BLOCKMODE;
		}
		//set dcon register
		rSDIFSTA |= SDIFSTA_FRST;
		rSDIBSIZE = 512;
		rSDIDCON = ((dcon_config | SDIDCON_START) | 1);
	}
	//clear command status register
	rSDICSTA |= 0x1e00;
	//set cmdcon register
	rSDICCON = cmd_config | (SDICCON_START);
	//Uart_Printf("command : %x\n", cmd_config);
	
	if(res & CMD_DATA) {
		count = 0;
		if(res & CMD_DATXMIT) {
			if((res & 0x3) == 2) {	//word transfer
				b_buffer = (uchar *)buffer;
				while(count < size) {
					//while TFDET is empty
					while((rSDIFSTA & (1<<10)) != (1<<10));
					for(i=0; i<16; i++) {
						for(w=0; w<4; w++) {
							c <<= 8;
							c |= *(b_buffer++);
						}
						rSDIDAT = c;
					}
					count += 16;
				}
				while((rSDIFSTA & (1<<10)) != (1<<10));
				while((rSDIDSTA & (1<<1)));
			} else if((res & 0x3) == 1) {	//half word transfer
				hw_buffer = (uint16 *)buffer;
				while(count++ < size) {
					//while TFDET is empty
					while((rSDIFSTA & SDIFSTA_TFDET) == 0x0000);
					for(w=0;w<2;w++) {
						c <<= 16;
						c = *(hw_buffer++);
					}
					rSDIDAT = c;
				}
			} else {		//byte transfer
				b_buffer = (uchar *)buffer;
				while(count++ < size) {
					//while TFDET is empty
					while((rSDIFSTA & SDIFSTA_TFDET) == 0x0000);
					rSDIDAT = *(b_buffer++);
				}
			}
		} else {
			if((res & 0x3) == 2) {	//word transfer
				b_buffer = (uchar *)buffer;
				while(count < size) {
					//while RFDET is empty
					while((rSDIFSTA & (1<<8)) != (1<<8));
					c = rSDIFSTA;
					for(i=0; i<16; i++) {
						c = rSDIDAT;
						*(b_buffer++) = (uchar)(c >>  24);
						*(b_buffer++) = (uchar)(c >>  16);
						*(b_buffer++) = (uchar)(c >>   8);
						*(b_buffer++) = (uchar)(c & 0xff);
					}
					count += 16;
				}
			} else if((res & 0x3) == 1) {	//half word transfer
				hw_buffer = (uint16 *)buffer;
				while(count++ < size) {
					//while RFDET is empty
					while((rSDIFSTA & SDIFSTA_RFDET) == 0x0000);
					c = rSDIDAT;
					*(hw_buffer++) = (uint16)(c >>  16);
					*(hw_buffer++) = (uint16)(c & 0x0f);
				}
			} else {		//byte transfer
				b_buffer = (uchar *)buffer;
				while(count++ < size) {
					//while RFDET is empty
					while((rSDIFSTA & SDIFSTA_RFDET) == 0x0000);	// { Uart_Printf("%x\n", rSDIFSTA); }
					c = rSDIDAT;
					*(b_buffer++) = (uchar)c;
				}
			}
		}
		//clear data status register
		rSDIDSTA |= 0xf8;
		return SCMD_SUCCESS;
	}
	
	//wait response here
	if(res & CMD_NORES) {
		//wait till command sent
		while ((rSDICSTA & 0x1e00) == 0x0000) {  }
		if(rSDICSTA & SDICSTA_CRCFAIL) return SCMD_FAIL;
		if(rSDICSTA & SDICSTA_TIMEOUT) return SCMD_TIMEOUT;
		if(rSDICSTA & SDICSTA_CMDSENT) return SCMD_SUCCESS;
		return SCMD_FAIL;
	} else {
		//wait till response received
		while ((rSDICSTA & 0x600) == 0x0000) {  }
		//check for response
		if(rSDICSTA & SDICSTA_RESPFIN) {
			if(res & CMD_LONGRES) {
				scmd->response[0] = rSDIRSP0;
				scmd->response[1] = rSDIRSP1;
				scmd->response[2] = rSDIRSP2;
				scmd->response[3] = rSDIRSP2; 
			} else {
				scmd->response[0] = rSDIRSP0;
			}
			return SCMD_SUCCESS;
		}
		if(rSDICSTA & SDICSTA_TIMEOUT) return SCMD_TIMEOUT;
		return SCMD_FAIL;
	}
}

esint8 sd_init_hw(HW_Interface * interface)
{
	esint16 i;
	euint8 resp;
	// Type B, FIFO reset, SD clock enable 
	rSDICON = (((SDICON_TYPEB | SDICON_CLKEN) | SDICON_MMCCLK) | SDICON_RESET);	//reset
	while(rSDICON & SDICON_RESET);
	// Set block size to 512 bytes 
	rSDIBSIZE = 0x200;
	// Set timeout count 
	rSDIDTIMER = 0x7fffff;
	// Disable SDI interrupt 
	rSDIIMSK = 0x0;
	rSDICON = (SDICON_TYPEB | SDICON_CLKEN);
	rSDIFSTA = rSDIFSTA | SDIFSTA_FRST;
	while(rSDIFSTA & SDIFSTA_FRST);
	rSDIPRE = PCLK / (100000) - 1;
	Delay(2000);
	//SD_Initialize(iface);
	return (0);
}

void sd_Resp8bError(HW_Interface *iface, euint8 value)
{
	switch(value)
	{
		case 0x40:
			Uart_Printf("Argument out of bounds.\n");
			break;
		case 0x20:
			Uart_Printf("Address out of bounds.\n");
			break;
		case 0x10:
			Uart_Printf("Error during erase sequence.\n");
			break;
		case 0x08:
			Uart_Printf("CRC failed.\n");
			break;
		case 0x04:
			Uart_Printf("Illegal command.\n");
			break;
		case 0x02:
			Uart_Printf("Erase reset (see SanDisk docs p5-13).\n");
			break;
		case 0x01:
			Uart_Printf("Card is initialising.\n");
			break;
		default:
			Uart_Printf("Unknown error 0x%x (see SanDisk docs p5-13).\n",value);
			break;
	}
}
/*****************************************************************************/

esint8 sd_State(HW_Interface *iface)
{
	SD_Command scmd;
	uint16 i;
	eint16 value;
	scmd.res = CMD_SHORTRES;
	scmd.arg = 0;
	scmd.cmd = SD_READ_STATUS;
	scmd.response[0] = 0;
	scmd.response[1] = 0;
	scmd.response[2] = 0;
	scmd.response[3] = 0;
	SDSendCommand(&scmd, 0, 0, 0);
	value = scmd.response[0];

	switch(value)
	{
		case 0x000:
			return(1);
			break;
		case 0x0001:
			Uart_Printf("Card is Locked.\n");
			break;
		case 0x0002:
			Uart_Printf("WP Erase Skip, Lock/Unlock Cmd Failed.\n");
			break;
		case 0x0004:
			Uart_Printf("General / Unknown error -- card broken?.\n");
			break;
		case 0x0008:
			Uart_Printf("Internal card controller error.\n");
			break;
		case 0x0010:
			Uart_Printf("Card internal ECC was applied, but failed to correct the data.\n");
			break;
		case 0x0020:
			Uart_Printf("Write protect violation.\n");
			break;
		case 0x0040:
			Uart_Printf("An invalid selection, sectors for erase.\n");
			break;
		case 0x0080:
			Uart_Printf("Out of Range, CSD_Overwrite.\n");
			break;
		default:
			if(value>0x00FF)
				sd_Resp8bError(iface,(euint8) (value>>8));
			else
				Uart_Printf("Unknown error: 0x%x (see SanDisk docs p5-14).\n",value);
			break;
	}
	return(-1);
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
  
void sd_writeblock(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size)
{
	SD_Command scmd;
	uint16 i;
	uchar ret;
	scmd.response[0] = 0;
	scmd.response[1] = 0;
	scmd.response[2] = 0;
	scmd.response[3] = 0;
	
	scmd.res = CMD_SHORTRES;
	scmd.arg = (interface->rca<<16);			//select 0
	scmd.cmd = SELECT_CARD;
	ret = SDSendCommand(&scmd, 0, 0, interface);
	if(ret == SCMD_TIMEOUT) {
		Uart_Printf("cmd7 timeout\n");
	}
	scmd.res = ((((CMD_SHORTRES | CMD_DATA) | CMD_DATWORD) | CMD_DATBLOCK) | CMD_DATXMIT);
	scmd.arg = (512 * address);
	scmd.cmd = WRITE_SINGLE_BLOCK;
	if(SDSendCommand(&scmd, buffer, 128, interface) == SCMD_TIMEOUT) {
		//Uart_Printf("timeout\n");
	}
	//Uart_Printf("success write single block\n");
	scmd.res = CMD_SHORTRES;
	scmd.arg = 0;							//deselect
	scmd.cmd = SELECT_CARD;
	ret = SDSendCommand(&scmd, 0, 0, interface);
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

void sd_readblock(HW_Interface * interface, uchar * buffer, uint32 address, uint32 size)
{
	SD_Command scmd;
	uint16 i;
	uchar ret;
	scmd.response[0] = 0;
	scmd.response[1] = 0;
	scmd.response[2] = 0;
	scmd.response[3] = 0;
	
	scmd.res = CMD_SHORTRES;
	scmd.arg = (interface->rca << 16);			//select 0
	scmd.cmd = SELECT_CARD;
	//Uart_Printf("rca : %x\n", interface->rca);
	ret = SDSendCommand(&scmd, 0, 0, interface);
	if(ret == SCMD_TIMEOUT) {
		//Uart_Printf("cmd7 timeout\n");
	}
	scmd.res = (((CMD_SHORTRES | CMD_DATA) | CMD_DATWORD) | CMD_DATBLOCK);
	scmd.arg = (512 * address);
	scmd.cmd = READ_SINGLE_BLOCK;
	if(SDSendCommand(&scmd, buffer, 128, interface) == SCMD_TIMEOUT) {
		//Uart_Printf("timeout\n");
	}
	/*for(i=0;i<size;i++) {
		Uart_Printf("%d\n", buffer[i]);
	}*/
	scmd.res = CMD_SHORTRES;
	scmd.arg = 0;							//deselect 
	scmd.cmd = SELECT_CARD;
	ret = SDSendCommand(&scmd, 0, 0, interface);
	//Uart_Printf("read_sector\n");
	//return(0);
}
/*****************************************************************************/

/* ****************************************************************************
 calculates size of card from CSD
 (extension by Martin Thomas, inspired by code from Holger Klabunde)
 */
esint8 sd_getDriveSize(HW_Interface *iface, euint32* drive_size )
{
	SD_Command scmd;
	euint8 cardresp, by;
	euint8 *ioc = scmd.response;
	euint16 c_size, c_size_mult, read_bl_len;
	uint16 i;
	uchar ret;

	scmd.res = CMD_LONGRES;
	scmd.arg = 0;
	scmd.cmd = SD_READ_CSD;
	scmd.response[0] = 0;
	scmd.response[1] = 0;
	scmd.response[2] = 0;
	scmd.response[3] = 0;
	SDSendCommand(&scmd, 0, 0, 0);
	
	c_size = ioc[6] & 0x03; // bits 1..0
	c_size <<= 10;
	c_size += (euint16)ioc[7]<<2;
	c_size += ioc[8]>>6;

	by= ioc[5] & 0x0F;
	read_bl_len = 1;
	read_bl_len <<= by;

	by=ioc[9] & 0x03;
	by <<= 1;
	by += ioc[10] >> 7;

	c_size_mult = 1;
	c_size_mult <<= (2+by);

	*drive_size = (euint32)(c_size+1) * (euint32)c_size_mult * (euint32)read_bl_len;

	return 0;
}