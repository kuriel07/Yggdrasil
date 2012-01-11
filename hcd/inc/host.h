#include "defs.h"
#include "midgard.h"
#ifndef __HOST_H

#define ED_NEW 		0x00
#define ED_UNLINK 	0x01
#define ED_OPER		0x02
#define ED_DEL		0x04
#define ED_URB_DEL  	0x08

/* TD info field */
#define TD_CC       0xf0000000
#define TD_CC_GET(td_p) ((td_p >>28) & 0x0f)
#define TD_CC_SET(td_p, cc) (td_p) = ((td_p) & 0x0fffffff) | (((cc) & 0x0f) << 28)
#define TD_EC       0x0C000000
#define TD_T        0x03000000
#define TD_T_DATA0  0x02000000
#define TD_T_DATA1  0x03000000
#define TD_T_TOGGLE 0x00000000
#define TD_R        0x00040000
#define TD_DI       0x00E00000
#define TD_DI_SET(X) (((X) & 0x07)<< 21)
#define TD_DP       0x00180000
#define TD_DP_SETUP 0x00000000
#define TD_DP_IN    0x00100000
#define TD_DP_OUT   0x00080000

#define TD_ISO	    0x00010000
#define TD_DEL      0x00020000

/* CC Codes */
#define TD_CC_NOERROR      0x00
#define TD_CC_CRC          0x01
#define TD_CC_BITSTUFFING  0x02
#define TD_CC_DATATOGGLEM  0x03
#define TD_CC_STALL        0x04
#define TD_DEVNOTRESP      0x05
#define TD_PIDCHECKFAIL    0x06
#define TD_UNEXPECTEDPID   0x07
#define TD_DATAOVERRUN     0x08
#define TD_DATAUNDERRUN    0x09
#define TD_BUFFEROVERRUN   0x0C
#define TD_BUFFERUNDERRUN  0x0D
#define TD_NOTACCESSED     0x0F

/* OHCI CONTROL AND STATUS REGISTER MASKS */

/*
 * HcControl (control) register masks
 */
#define OHCI_CTRL_CBSR	(3 << 0)	/* control/bulk service ratio */
#define OHCI_CTRL_PLE	(1 << 2)	/* periodic list enable */
#define OHCI_CTRL_IE	(1 << 3)	/* isochronous enable */
#define OHCI_CTRL_CLE	(1 << 4)	/* control list enable */
#define OHCI_CTRL_BLE	(1 << 5)	/* bulk list enable */
#define OHCI_CTRL_HCFS	(3 << 6)	/* host controller functional state */
#define OHCI_CTRL_IR	(1 << 8)	/* interrupt routing */
#define OHCI_CTRL_RWC	(1 << 9)	/* remote wakeup connected */
#define OHCI_CTRL_RWE	(1 << 10)	/* remote wakeup enable */

/* pre-shifted values for HCFS */
#define OHCI_USB_RESET	(0 << 6)
#define OHCI_USB_RESUME	(1 << 6)
#define OHCI_USB_OPER	(2 << 6)
#define OHCI_USB_SUSPEND	(3 << 6)

/*
 * HcCommandStatus (cmdstatus) register masks
 */
#define OHCI_HCR	(1 << 0)	/* host controller reset */
#define OHCI_CLF  	(1 << 1)	/* control list filled */
#define OHCI_BLF  	(1 << 2)	/* bulk list filled */
#define OHCI_OCR  	(1 << 3)	/* ownership change request */
#define OHCI_SOC  	(3 << 16)	/* scheduling overrun count */

/*
 * masks used with interrupt registers:
 * HcInterruptStatus (intrstatus)
 * HcInterruptEnable (intrenable)
 * HcInterruptDisable (intrdisable)
 */
#define OHCI_INTR_SO	(1 << 0)	/* scheduling overrun */
#define OHCI_INTR_WDH	(1 << 1)	/* writeback of done_head */
#define OHCI_INTR_SF	(1 << 2)	/* start frame */
#define OHCI_INTR_RD	(1 << 3)	/* resume detect */
#define OHCI_INTR_UE	(1 << 4)	/* unrecoverable error */
#define OHCI_INTR_FNO	(1 << 5)	/* frame number overflow */
#define OHCI_INTR_RHSC	(1 << 6)	/* root hub status change */
#define OHCI_INTR_OC	(1 << 30)	/* ownership change */
#define OHCI_INTR_MIE	(1UL << 31)	/* master interrupt enable */

/* For initializing controller (mask in an HCFS mode too) */
#define	OHCI_CONTROL_INIT \
	(OHCI_CTRL_CBSR & 0x3) | OHCI_CTRL_IE | OHCI_CTRL_PLE


#define EP_BULK				2
#define EP_CONTROL			1
#define EP_INTERRUPT		8
#define EP_ISOCHRONOUS		4

#define EP_CTRL_SKIP 	(1 << 13)

typedef struct host_controller host_controller;
typedef struct ohci_hcd_regs ohci_hcd_regs;
typedef struct host_hcca host_hcca;

struct host_hcca {
	uint32 interrupt_table[32];
	uint16 frame_number;
	uint16 pad1;
	uint32 done_head;
	uint32 reserved[29];
};

#define RHPORT_MAX_DEVICES		5
#define RHPORT_CCS				0x01
#define RHPORT_PES				0x02
#define RHPORT_PSS				0x04
#define RHPORT_POCI				0x08
#define RHPORT_PRS				0x10
#define RHPORT_PPS				0x100
#define RHPORT_LSDA				0x200

struct ohci_hcd_regs {
	/* control and status registers */
	uint32	revision;			//HcRevision
	uint32	control;			//HcControl
	uint32	cmdstatus;			//HcCommandStatus
	uint32	intrstatus;			//HcInterruptStatus
	uint32	intrenable;			//HcInterruptEnable
	uint32	intrdisable;		//HcInterruptDisable
	/* memory pointers */
	uint32	hcca;				//HcHCCA
	uint32	ed_periodcurrent;	//HcPeriodCurrentED
	uint32	ed_controlhead;		//HcControlHeadED
	uint32	ed_controlcurrent;	//HcControlCurrentED
	uint32	ed_bulkhead;		//HcBulkHeadED
	uint32	ed_bulkcurrent;		//HcBulkCurrentED
	uint32	donehead;			//HcDoneHead
	/* frame counters */
	uint32	fminterval;			//HcFmInterval
	uint32	fmremaining;		//HcFmRemaining
	uint32	fmnumber;			//HcFmNumber
	uint32	periodicstart;		//HcPeriodicStart
	uint32	lsthresh;			//HcLSThreshold
	/* Root hub ports */
	uint32  rhdesc_a;			//HcRhDescriptorA
	uint32  rhdesc_b;			//HcRhDescriptorB
	uint32  rhstatus;			//HcRhStatus
	uint32  rhportstatus[RHPORT_MAX_DEVICES];	//HcRhPortStatus
};

#define HC_PORT_OCCUPIED		0x01
struct host_controller {
	uint32 _ports[RHPORT_MAX_DEVICES];
	ohci_hcd_regs * _regs;
	Heap_Manager * _dev_heap;
	host_hcca * hcca;
	host_controller * next;
};

void host_suspend(host_controller * hc);		//disable host operation for changing purpose
void host_resume(host_controller * hc);			//enable host operation

#define __HOST_H
#endif
