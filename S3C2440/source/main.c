#include "config.h"
//=======================================
#include "app_cfg.h"
#include "yggdrasil.h"
#include "midgard.h"
#include "efat.h"
#include "sdi.h"
#include "efat_alloc.h"

extern OS_STK ShellTaskStk[ShellTaskStkLength];
//static EmbeddedFileSystem _efs;


void Isr_Init(void);
void HaltUndef(void);
void HaltSwi(void);
void HaltPabort(void);
void HaltDabort(void);
//efat_FileSystem * fs;

int Main(int argc, char **argv)
{
	//_efs.myCard.rca = 4;
	//target board initialization (Mini2440)
	//Uart_Printf("initialize memory manager\n");
	TargetInit(); 
	Uart_Printf("target initialized\n");
	Delay(200);
	//Uart_Printf("pullup : %x\n", rGPEUP);
	//Port_Init();		//IO?????
	//Test_SDI();
	Uart_Printf("initialize memory manager\n");
	//initialize midgard memory manager
	m_init_alloc();
	Uart_Printf("initialize efat memory manager\n");
	//initialize eFAT memory allocator
	init_efat_Alloc(); 
	/*Uart_Printf("use eFAT\n");
	fs = fat_Init(0, 0);
	if(fs == NULL)
	{
		Uart_Printf("SD Card Not Initialized!..\n");
	} else {
		Uart_Printf("SD Card Initialized!..\n");
	}*/
	//Test_SDI();
	//init uC/OS 2 Kernel
	Uart_Printf("initialize operating system\n");
	OSInit();	
	Uart_Printf("operating system initialized\n");
   	//
   	OSTimeSet(0);
   	//Uart_Printf("hello world\n");
   	//create shelltask, including display task, touch task, idle task
   	OSTaskCreate (ShellTask, (void *)0, &ShellTaskStk[ShellTaskStkLength - 1], ShellTaskPrio);																										
	OSStart();
	//ShellTask(NULL);
	return 0;
}

//===================================================================
void Isr_Init(void)
{
    pISR_UNDEF  = (unsigned)HaltUndef;
    pISR_SWI    = (unsigned)HaltSwi;
    pISR_PABORT = (unsigned)HaltPabort;
    pISR_DABORT = (unsigned)HaltDabort;
    
    rINTMOD     = 0x0;                     //All=IRQ mode
//    rINTCON=0x5;                           //Non-vectored,IRQ enable,FIQ disable    
    rINTMSK     = BIT_ALLMSK;              //All interrupt is masked.
    rINTSUBMSK  = BIT_SUB_ALLMSK;          //All sub-interrupt is masked. <- April 01, 2002 SOP

//    rINTSUBMSK  = ~(BIT_SUB_RXD0);         //Enable Rx0 Default value=0x7ff
//    rINTMSK     = ~(BIT_UART0);            //Enable UART0 Default value=0xffffffff    
    
//    pISR_UART0=(unsigned)RxInt;            //pISR_FIQ,pISR_IRQ must be initialized
}

//===================================================================
void HaltUndef(void)
{
    Uart_Printf("Undefined instruction exception.\n");
    while(1);
}

//===================================================================
void HaltSwi(void)
{
    Uart_Printf("SWI exception.\n");
    while(1);
}

//===================================================================
void HaltPabort(void)
{
    Uart_Printf("Pabort exception.\n");
    while(1);
}

//===================================================================
void HaltDabort(void)
{
    Uart_Printf("Dabort exception.\n");
    while(1);
}



