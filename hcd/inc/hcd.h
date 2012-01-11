#include "defs.h"
#include "host.h"
#include "host-alloc.h"
#ifndef _HCD_H
//Host Controller Driver revision
#define HCD_REVISION 	11

#define USBRESET		0x00
#define USBRESUME		0x01
#define USBOPERATIONAL	0x02
#define USBSUSPEND		0x03

#define HC_CTRL_IR		(1 << 8)		//interrupt routing smi
#define HC_CTRL_RWC		(1 << 9)		//remote wakeup connected
#define HC_CTRL_RWE		(1 << 10)		//remote wakeup enable

/*------------------------------------------
 * transfer types 
 * ------------------------------------------*/

#define USB_CTRL		0x00
#define USB_ISOC		0x01
#define USB_BULK		0x02
#define USB_INTR		0x03

/*-------------------------------------------
 * device descriptor types 
 * ------------------------------------------*/
#define DO_STANDARD   0x00
#define DO_CLASS      0x20
#define DO_VENDOR     0x40

/*-------------------------------------------
 * standard requests for ep0 
 * ------------------------------------------*/

#define GET_STATUS	  	0x00 
#define CLR_FEATURE	  	0x01
#define SET_FEATURE	  	0x03
#define SET_ADDRESS	  	0x05
#define GET_DESCRIPTOR	  	0x06
#define SET_DESCRIPTOR	  	0x07
#define GET_CONFIGURATION 	0x08
#define SET_CONFIGURATION 	0x09
#define GET_INTERFACE	  	0x0A
#define SET_INTERFACE	  	0x0B

/*-------------------------------------------
 * descriptor types 
 * ------------------------------------------*/

#define DEVICE			0x01
#define CONFIGURATION		0x02
#define STRING			0x03
#define INTERFACE		0x04
#define ENDPOINT		0x05
#define DEVICEQUALIFIER		0x06 // only usb2.0
#define OTHERSPEEDCONFIGURATION 0x07 // only usb2.0


/*-------------------------------------------
 * pid fields 
 * ------------------------------------------*/

#define USB_PID_SOF     0x05
#define USB_PID_SETUP   0x0D
#define USB_PID_IN      0x09
#define USB_PID_OUT     0x01
#define USB_PID_DATA0   0x03
#define USB_PID_DATA1   0x0B
#define USB_PID_ACK     0x02
#define USB_PID_NACK    0x0A
#define USB_PID_STALL   0x0E
#define USB_PID_PRE     0x0C


#define MASS_STORAGE_CLASSCODE	0x08
#define HUB_CLASSCODE		0x09

//
// The different ED lists are as follows.
//
#define ED_INTERRUPT_1ms 	0
#define ED_INTERRUPT_2ms 	1
#define ED_INTERRUPT_4ms 	3
#define ED_INTERRUPT_8ms 	7
#define ED_INTERRUPT_16ms 	15
#define ED_INTERRUPT_32ms 	31
#define ED_CONTROL 			63
#define ED_BULK 			64
#define ED_ISOCHRONOUS 		0 		// same as 1ms interrupt queue
#define NO_ED_LISTS 		65
#define ED_EOF 				0xff

#define TD_COMPLETED		0x80000000
#define TD_ERROR			0x40000000

/*-------------------------------------------
 * condition code error
 * ------------------------------------------*/
#define CC_NO_ERROR			(0x00UL << 28)
#define CC_ERROR_CRC		(0x01UL << 28)
#define CC_BIT_STUFFING		(0x02UL << 28)
#define CC_DTOGL_MIS		(0x03UL << 28)
#define CC_STALL			(0x04UL << 28)
#define CC_DEV_NO_RES		(0x05UL << 28)
#define CC_PID_FAIL			(0x06UL << 28)
#define CC_ERROR_PID		(0x07UL << 28)
#define CC_DAT_OVRRUN		(0x09UL << 28)
#define CC_DAT_UNDRUN		(0x0aUL << 28)
#define CC_BUF_OVRRUN		(0x0cUL << 28)
#define CC_BUF_UNDRUN		(0x0dUL << 28)
#define CC_ACCESS_FAILED	(0x0eUL << 28)
#define CC_NOT_ACCESSED		(0x0fUL << 28)

typedef struct usbd_request usbd_request;
typedef struct usb_device usb_device;
typedef struct usb_driver usb_driver;
typedef struct hcd_core hcd_core;
typedef struct endpoint_desc endpoint_desc;
typedef struct transfer_desc transfer_desc;
typedef struct iso_transfer_desc iso_transfer_desc;

#define REQ_COMPLETED			0x80000000
#define REQ_ERROR				0x40000000
#define REQ_TYPE_BULK			0x02
#define REQ_TYPE_CONTROL		0x01
#define REQ_TYPE_INTERRUPT		0x04
#define REQ_TYPE_ISOCHRONOUS	0x08
struct usbd_request {
	uint32 length;				//data length
	uint32 control;				//in/out for bulk and control
	volatile uint32 status;				//completion status from HCD to USBD
	uint32 td_count;			//td counter
	uchar setup[8];				//setup data
	transfer_desc * td_list;	//list of transfer descriptor
	uchar * buffer;				//pointer to data
	usb_device * device;		//current request host controller
};

#define DEV_CLOSED				0x02
#define DEV_OPENED				0x03
#define ENDPOINT_CONTROL		63
#define ENDPOINT_BULK			64
#define NO_OF_ENDPOINTS			65
struct usb_device {
	uchar address;
	uchar class;
	uchar subclass;
	uchar protocol;
	uint16 status;						//device status DEV_OPENED/DEV_CLOSED
	uint16 VID;							//vendor id
	uint16 PID;							//product id
	uint16 c_mps;						//maximum packet size for control message
	host_controller * hc;				//current device host controller
	usb_driver * driver;
	usb_device * next;
	endpoint_desc * endpoints[NO_OF_ENDPOINTS];		//device endpoints, initialize with usb_device->driver->init and destory with usb_device->driver->close
};

struct usb_driver {
	uchar name[256];					//standard long filename for FAT
	uint16 VID;
	uint16 PID;
	uint32 revision;
	void (* init)(usb_device * dev);	//contain the necessary function for device creation
	void (* close)(usb_device * dev);	//contain function for closing device
	usb_driver * next;
};

struct hcd_core {
	uchar next_address;					//next device address
	uchar address_stack[255];			//stack for removed device
	uchar stack_index;					//stack index
	host_controller * hc_list;				//list host controller
	usb_driver * driver_list;
	usb_device * device_list;
};

#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03

/*
 * Various libusb API related stuff
 */

#define USB_ENDPOINT_IN			0x80
#define USB_ENDPOINT_OUT		0x00

#define ED_FULLSPEED			0
#define ED_LOWSPEED				1
#define ED_DEFAULT_ADDRESS		0
#define ED_ENDPOINT0			0
#define ED_ENDPOINT1			1
#define ED_ENDPOINT2 			2
#define ENDPOINT_HALT 	0x00000001 //hardware stopped bit
#define ENDPOINT_CARRY	0x00000002 //hardware toggle carry bit
struct endpoint_desc {
	uint32 control;
	uint32 tail_p;		//tail pointer to the transfer descriptor
	uint32 head_p;		//head pointer to the transfer descriptor
	uint32 next_ed;		//next endpoint descriptor pointer
	//extended property
	uint32 type;
	uint32 status;
};

#define HCD_PID_SETUP	0x00
#define HCD_PID_OUT		0x01
#define HCD_PID_IN		0x02
struct transfer_desc {
	uint32 control;
	uint32 cbp;
	uint32 next_td;		//next transfer descriptor
	uint32 buf_end;
	//extended property
	uint32 status;
	uint32 next;	//per request list
	uint32 endpoint;
	uint32 request;
};

struct iso_transfer_desc {
	uint32 control;
	uint32 buf_page;
	uint32 next_td;
	uint32 buf_end;
	uint16 ops[8];
};

//hcd functions
void hcd_init(void);
void hcd_start(void);
void hcd_stop(void);
void usb_lowlevel_init(void);
void usb_lowlevel_stop(void);
host_controller * hcd_create_host_controller(uint32 address, uint32 heap_size);
void initialize_hcca(host_controller * hc);
void hcd_register_driver(usb_driver * driver);
void hcd_unregister_driver(usb_driver * driver);
endpoint_desc * create_endpoint_control(uchar f_address, uchar en_number, uchar speed, uint16 mps, host_controller * hc);
endpoint_desc * create_endpoint_interrupt(uchar no, uchar f_address, uchar en_number, uchar speed, uint16 mps, host_controller * hc);
endpoint_desc * create_endpoint_bulk(uchar f_address, uchar en_number, uchar speed, uint16 mps, host_controller * hc);
transfer_desc * create_transfer_descriptor(uchar togl, uchar buffer_rounding, uchar direction, uint16 size, void * buffer, endpoint_desc * endpoint, usbd_request * request);
usbd_request * create_request(uint32 length, uchar setup[8], uchar * buffer, usb_device * dev);

//usb host functions(name derived from libusb)
usb_device * usb_open(uint32 vendor_id, uint32 product_id);
void usb_control_msg(uchar requesttype, uchar request, uint16 value, uint16 index, uint16 size, uchar * buffer, usb_device * dev);
void usb_close(usb_device * device);

//usb host cleanup function
void usb_periodic(void);
void ProcessDoneQueue(host_controller * hc);
//void usb_init(void); 		//didn't exist instead use init_hcd followed by hcd_create_host_controller(s)
#define _HCD_H
#endif
