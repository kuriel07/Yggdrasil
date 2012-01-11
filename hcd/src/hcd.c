#include "defs.h"
#include "host.h"
#include "host-alloc.h"
#include "hcd.h"
#include "2440addr.h"
#include "2440lib.h"

static hcd_core _hcd_core;		//hcd engine core

void hcd_wait_us(uint32 dly) {
	uint32 count = (dly * 320);
	for(dly=0;dly<count;dly++);
}

void hcd_wait_ms(uint32 dly) {
	uint32 count = dly;
	for(dly=0;dly<count;dly++) {
		hcd_wait_us(1);
	}
}

void hcd_core_push(uchar address) {
	_hcd_core.address_stack[_hcd_core.stack_index++] = address;
}

uchar hcd_core_pop() {
	return _hcd_core.address_stack[--_hcd_core.stack_index];
}

uchar hcd_core_stack() {
	return _hcd_core.stack_index;
}

uchar hcd_core_next_address() {
	if(hcd_core_stack()) {
		return hcd_core_pop();					//pop stacked address
	} else {
		return ++_hcd_core.next_address;		//start address begin at 1 (zero must not used)
	}
}

void usb_lowlevel_init(void) {
	//rUPLLCON = 0x78023;		//upll value
	//rCLKSLOW &= ~(1<<7);		//enable upll
	//rCLKCON |= (1<<6);		//enable usb clock
}

void usb_lowlevel_stop(void) {
	//rCLKCON &= ~(1<<6);		//enable usb clock
}

void debug_error_code(uchar cc) {				//for debugging purpose
	uint32 condition_code = cc;
	condition_code = (condition_code << 28);
	switch(condition_code) {
		case CC_ERROR_CRC:
			Uart_Printf("transmission error : CRC\n");
			break;
		case CC_BIT_STUFFING:
			Uart_Printf("transmission error : bit stuffing\n");
			break;
		case CC_DTOGL_MIS:
			Uart_Printf("sequence error : data toggle mismatch\n");
			break;
		case CC_STALL:
			Uart_Printf("transmission error : bit stuffing\n");
			break;
		case CC_DEV_NO_RES:
			Uart_Printf("transmission error : device not responding\n");
			break;
		case CC_PID_FAIL:
			Uart_Printf("transmission error : PID check failed\n");
			break;
		case CC_ERROR_PID:
			Uart_Printf("transmission error : unexpected PID\n");
			break;
		case CC_DAT_OVRRUN:
			Uart_Printf("time error : data overrun\n");
			break;
		case CC_DAT_UNDRUN:
			Uart_Printf("time error : data underrun\n");
			break;
		case CC_BUF_OVRRUN:
			Uart_Printf("system error : buffer overrun\n");
			break;
		case CC_BUF_UNDRUN:
			Uart_Printf("system error : buffer underrun\n");
			break;
		case CC_ACCESS_FAILED:
		case CC_NOT_ACCESSED:
			Uart_Printf("time error : not accessed\n");
			break;
		default:
			break;
	}
}

void hcd_init(void) {
	_hcd_core.next_address = 0;
	_hcd_core.stack_index = 0;
	_hcd_core.hc_list = NULL;
	_hcd_core.driver_list = NULL;
	_hcd_core.device_list = NULL;
}

void hcd_start(void) {
	usb_lowlevel_init();
}

void hcd_stop(void) {
	usb_lowlevel_stop();
}

//register driver to hcd
void hcd_register_driver(usb_driver * driver) {
	usb_driver * prev_driver;
	prev_driver = _hcd_core.driver_list;	//iterate all driverlist and add new driver
	if(prev_driver == NULL) {
		_hcd_core.driver_list = driver;
	} else {
		while(prev_driver->next != NULL) {
			prev_driver = prev_driver->next;
		}
		prev_driver->next = driver;
	}
}

//unregister driver prior to hcd
void hcd_unregister_driver(usb_driver * driver) {
	usb_driver * prev_driver;
	prev_driver = _hcd_core.driver_list;	//iterate all driverlist and add new driver
	if(prev_driver == driver) {
		prev_driver->next = driver->next;
	} else {
		while(prev_driver != NULL) {
			if(prev_driver->next == driver) {
				prev_driver->next = driver->next;
				return;
			}
			prev_driver = prev_driver->next;
		}		
	}
}

//create new host controller and add it to the hc_list
host_controller * hcd_create_host_controller(uint32 address, uint32 heap_size) {
	uchar hcfs;
	uint16 fminterval = 0x2edf;
	uint16 timeout = 30;
	uint16 smm_timeout = 50; /* 0,5 sec */
	host_controller * hc;
	host_controller * prev_hc;
	hc = m_alloc(sizeof(host_controller));		//allocate memory for host controller
	hc->_regs = (ohci_hcd_regs *)address;		//current host controller registers
	//if((hc->_regs->revision & 0xff) > HCD_REVISION) 		//check for hc revision (omitted)
	hcfs = (hc->_regs->control >> 6) & 0x03;
	//check for previous condition
	switch(hcfs) {
		case USBRESET:		//cold boot
			hc->_regs->control |= OHCI_CTRL_RWC;	//set remote wakeup connected
			if(hc->_regs->control & OHCI_CTRL_IR) {
				hc->_regs->cmdstatus = OHCI_OCR;
			}
			while(hc->_regs->control & OHCI_CTRL_IR) {
				hcd_wait_ms(10);
				if (--smm_timeout == 0) {
					return NULL;
				}
			}
			hc->_regs->intrdisable 	= OHCI_INTR_MIE;
			hc->_regs->control 		= 0;   	
			hc->_regs->rhdesc_a	= hc->_regs->rhdesc_a | (1 << 8) | (2 << 24);
			hc->_regs->rhdesc_a	= hc->_regs->rhdesc_a & ~(1 << 11);
			hc->_regs->rhdesc_b	= 1;
			/* HC Reset requires max 10 ms delay */
			hc->_regs->cmdstatus 	= OHCI_HCR;
			while ((hc->_regs->cmdstatus & OHCI_HCR) != 0) {
				if (--timeout == 0) {
					return NULL;
				}	
				hcd_wait_us(1);
			}
			break;
		case USBOPERATIONAL:
			//goto hc setup
			break;
		default:
			//set to usb resume
			hcfs &= 0xffffff3f;
			hc->_regs->control = hcfs & (USBRESUME << 6);
			//wait 
			while (--timeout != 0) {
				hcd_wait_us(1);
			}
			break;
	}
	hcd_wait_us(10);
	hc->_dev_heap = m_create_heap(heap_size);	//current host controller device heap
	
	create_endpoint_control(ED_DEFAULT_ADDRESS, ED_ENDPOINT0, ED_LOWSPEED, 8, hc);	//create default endpoint 0 at default address
	//create_endpoint_bulk(ED_DEFAULT_ADDRESS, ED_ENDPOINT0, ED_LOWSPEED, 8, hc);
	//setup, derived from linux ohci setup
	//hc->_regs->ed_controlhead 	= 0;
	hc->_regs->ed_bulkhead 		= 0;
	//hc->_regs->fminterval 		= (fminterval | ((((fminterval - 210) * 6) / 7) << 16));
	//hc->_regs->periodicstart 	= (hc->_regs->fminterval * 5) / 10;
	hc->_regs->fminterval		= fminterval;
	hc->_regs->periodicstart	= (fminterval * 5) / 10;
	Uart_Printf("interval : %x\n", hc->_regs->fminterval);
	hc->_regs->lsthresh 		= 0x628;
	hc->_regs->control 			= (OHCI_CONTROL_INIT | OHCI_USB_OPER);
	hc->_regs->intrenable 		= (OHCI_INTR_MIE | OHCI_INTR_UE | OHCI_INTR_WDH | OHCI_INTR_SO);
	hc->_regs->intrstatus 		= (OHCI_INTR_MIE | OHCI_INTR_UE | OHCI_INTR_WDH | OHCI_INTR_SO);
	hc->_regs->rhdesc_a			= hc->_regs->rhdesc_a | (1 << 8) | (2 << 24);
	hc->_regs->rhdesc_a			= hc->_regs->rhdesc_a & ~(1 << 11);
	hc->_regs->rhdesc_b			= 1;
	hc->_regs->rhstatus			= (1<<16);
	hc->_regs->rhportstatus[0] 	= (1<<1) | (1<<8);
	hc->_regs->rhportstatus[1] 	= (1<<1) | (1<<8);
	initialize_hcca(hc);							//allocate/initialize hcca interrupt table, etc
	hc->next = NULL;							//initialize next hc
	prev_hc = _hcd_core.hc_list;				//add hc to hc_list
	if(prev_hc == NULL) {
		_hcd_core.hc_list = hc;
	} else {
		while(prev_hc->next != NULL) {
			prev_hc = prev_hc->next;
		}
		prev_hc->next = hc;
	}
	hc->_regs->control |= (OHCI_CTRL_CLE | OHCI_CTRL_BLE);
	Uart_Printf("hc created\n");
	return hc;
}

//initialize hcca on host controller creation
void initialize_hcca(host_controller * hc) {
	uint32 i;
	host_hcca * hcca = dev_alloc(hc->_dev_heap, sizeof(host_hcca), 256);	//256 byte boundary
	//initialize interrupt table
	Uart_Printf("hcca : %x\n", hcca);
	for(i=0;i<32;i++) {
		hcca->interrupt_table[i] = NULL;		//initialize with null
	}
	hc->hcca = hcca;
	hc->_regs->hcca = (uint32)hcca;
}

endpoint_desc * create_endpoint_bulk(uchar f_address, uchar en_number, uchar speed, uint16 mps, host_controller * hc) {
	uint32 head_ptr;
	endpoint_desc * ed;
	endpoint_desc * prev_ed;
	ed = dev_alloc(hc->_dev_heap, sizeof(endpoint_desc), 16);		//16byte boundary
	ed->type = EP_BULK;
	ed->control = (f_address & 0x7f) | ((en_number & 0x0f) << 7) | ((speed & 0x01) << 13) | ((mps & 0x3ff) << 16);
	//create TD to fill in
	ed->head_p = dev_alloc(hc->_dev_heap, sizeof(transfer_desc), 16);
	ed->tail_p = ed->head_p;
	ed->next_ed = NULL;
	head_ptr = hc->_regs->ed_bulkhead;
	if(head_ptr == NULL) {
		//create new endpoint for this hcca
		hc->_regs->ed_bulkhead = (uint32)ed;
	} else {
		while(head_ptr != NULL) {
			prev_ed = (endpoint_desc *)head_ptr;
			head_ptr = prev_ed->next_ed;
		}
		prev_ed->next_ed = (uint32)ed;
	}
	return ed;
}

endpoint_desc * create_endpoint_control(uchar f_address, uchar en_number, uchar speed, uint16 mps, host_controller * hc) {
	uint32 head_ptr;
	endpoint_desc * ed;
	endpoint_desc * prev_ed;
	ed = dev_alloc(hc->_dev_heap, sizeof(endpoint_desc), 16);		//16 byte boundary
	ed->type = EP_CONTROL;
	ed->control = (f_address & 0x7f) | ((en_number & 0x0f) << 7) | ((speed & 0x01) << 13) | ((mps & 0x3ff) << 16);
	//create TD to fill in
	ed->head_p = dev_alloc(hc->_dev_heap, sizeof(transfer_desc), 16);
	ed->tail_p = ed->head_p;
	ed->next_ed = NULL;
	head_ptr = hc->_regs->ed_controlhead;
	if(head_ptr == NULL) {
		//create new endpoint for this hcca
		hc->_regs->ed_controlhead = (uint32)ed;
		Uart_Printf("ed control head : %x %x %x\n", hc->_regs->ed_controlhead, HcControlHeadED, ed->control);
	} else {
		while(head_ptr != NULL) {
			prev_ed = (endpoint_desc *)head_ptr;
			head_ptr = prev_ed->next_ed;
		}
		prev_ed->next_ed = (uint32)ed;
	}
	return ed;
}

endpoint_desc * create_endpoint_interrupt(uchar no, uchar f_address, uchar en_number, uchar speed, uint16 mps, host_controller * hc) {
	uint32 head_ptr;
	endpoint_desc * ed;
	endpoint_desc * prev_ed;
	host_hcca * hcca = (host_hcca *)hc->_regs->hcca;
	ed = dev_alloc(hc->_dev_heap, sizeof(endpoint_desc), 16);		//16 byte boundary
	ed->type = EP_INTERRUPT;
	//function address | endpoint number | speed | maximum packet size | direction depend on td
	ed->control = (f_address & 0x7f) | ((en_number & 0x0f) << 7) | ((speed & 0x01) << 13) | ((mps & 0x3ff) << 16);
	//create TD to fill in
	ed->head_p = dev_alloc(hc->_dev_heap, sizeof(transfer_desc), 16);
	ed->tail_p = ed->head_p;
	ed->next_ed = NULL;
	head_ptr = hcca->interrupt_table[no];
	if(head_ptr == NULL) {
		//create new endpoint for this hcca
		hcca->interrupt_table[no] = (uint32)ed;
	} else {
		while(head_ptr != NULL) {
			prev_ed = (endpoint_desc *)head_ptr;
			head_ptr = prev_ed->next_ed;
		}
		prev_ed->next_ed = (uint32)ed;
	}
	return ed;
}

//this function used to skip endpoint on td creation/deletion
void endpoint_skip(uchar set, endpoint_desc * endpoint) {
	(set)? (endpoint->control |= EP_CTRL_SKIP): (endpoint->control &= ~EP_CTRL_SKIP);
}

//create general transfer descriptor
transfer_desc * create_transfer_descriptor(uchar togl, uchar buffer_rounding, uchar direction, uint16 size, void * buffer, endpoint_desc * endpoint, usbd_request * request) {
	transfer_desc * td;								//for control/bulk/interrupt
	transfer_desc * prev_td;
	//iso_transfer_desc * itd;						//for isonchronous
	//iso_transfer_desc * prev_itd;
	host_controller * hc = request->device->hc;		//get device hc
	if(endpoint == NULL) return 0;
	//endpoint_skip(1, endpoint);							//prevent access to endpoint
	//use td to fill in
	td = (transfer_desc *)endpoint->tail_p;
	//buffer_rounding | direction (SETUP, IN, OUT) | no interupt
	td->control = ((buffer_rounding & 0x01) << 18) | ((direction & 0x03) << 19) | ((0x07) << 21) | ((togl & 0x03) << 24);
	td->control = td->control | CC_NOT_ACCESSED;
	td->endpoint = (uint32)endpoint;				//set current endpoint
	td->request = (uint32)request;					//set current request
	if(buffer) {									//check if data exist
		td->cbp = (uint32)buffer;
		td->buf_end = ((uint32)buffer + size - 1);
	} else {
		td->buf_end = 0;
	}
	if(request != NULL) {
		request->td_count += 1;				//add request count
		td->next = (uint32)request->td_list;		//set td->next to request->td_list(default NULL)
		request->td_list = td;				//set request->td_list to new td
	}
	//previous transfer descriptor = current transfer descriptor
	prev_td = td;
	//create new td to fill in
	td = dev_alloc(hc->_dev_heap, sizeof(transfer_desc), 16);		//16 byte boundary
	td->cbp = 0;							//zero pointer value (all bytes has been transfered)
	td->next_td = NULL;
	//set previous transfer descriptor next_td to new td
	prev_td->next_td = (uint32)td;
	endpoint->tail_p = (uint32)td;	//set endpoint tail pointer
	//Uart_Printf("current : %x, next : %x\n", (uint32)prev_td, *(uint32 *)((uint32)prev_td + (2 * sizeof(uint32))));
	//endpoint_skip(0, endpoint);					//allow access to endpoint
	
	/*switch(endpoint->type) {				//set HcCommandStatus according to endpoint type (bulk/control)
		case EP_BULK:
			hc->_regs->cmdstatus |= OHCI_BLF;
			break;
		case EP_CONTROL: 
			hc->_regs->cmdstatus |= OHCI_CLF;
			break;
		default:
			break;
	}*/
	Uart_Printf("td created at : %x, next %x\n", prev_td, prev_td->next_td);
	return (transfer_desc *)prev_td;
}

usbd_request * create_request(uint32 length, uchar setup[8], uchar * buffer, usb_device * dev) {
	uchar i;
	usbd_request * request;
	request = dev_alloc(dev->hc->_dev_heap, sizeof(usbd_request), 4);
	request->length = length;				//data length
	request->status = 0x00;					//completion status from HCD to USBD
	request->td_count = 0;					//td counter
	for(i=0;i<8;i++) {
		request->setup[i] = setup[i];		//initialize setup data
	}
	request->td_list = NULL;				//default td_list (no td on list)
	request->buffer = buffer;				//pointer to data
	request->device = dev;					//current request host controller
	return request;
}

void request_cleanup(usbd_request * request) {
	transfer_desc * td;
	usb_device * dev = request->device;
	while(request->td_list != NULL) {		//freed all td list
		td = request->td_list;
		request->td_list = (transfer_desc *)td->next;
		dev_free(dev->hc->_dev_heap, td);	//freed memory
	}
	if(request->status & REQ_ERROR) {		//debug for error
		debug_error_code(request->status);
	} else {
		//Uart_Printf("success\n");			//on success
	}
	Uart_Printf("finished\n");	
	//dev_free(dev->hc->_dev_heap, request);	//request from memory
}

usb_device * usb_open(uint32 vendor_id, uint32 product_id) {		//iterate all device list and return device pointer
	usb_device * dev = _hcd_core.device_list;
	while(dev != NULL) {
		if(dev->VID == vendor_id) {
			if(dev->PID == product_id) {
				if(dev->status == DEV_CLOSED) {
					dev->status = DEV_OPENED;		//set device status
					return dev;
				}
			}
		}
		dev = dev->next;
	}
	return NULL;
}

void usb_close(usb_device * device) {				//only set status, never remove device from device list
	device->status = DEV_CLOSED;
}

void endpoint_set_mps(endpoint_desc * ed, uint16 mps) {
	ed->control &= 0xf800ffff;				//clear mps
	ed->control |= ((mps & 0x3ff) << 16);	//mask mps
}

void usb_control_msg(uchar requesttype, uchar request, uint16 value, uint16 index, uint16 size, uchar * buffer, usb_device * dev) {
	uchar direction = 0;
	uchar togl;
	uint16 i;
	uint16 length;
	uint32 head_ptr;
	uint32 endpoint_address;
	uchar setup[8];
	volatile usbd_request * req;
	endpoint_desc * ed;
	transfer_desc * td;
	host_controller * hc = dev->hc;
	host_hcca * hcca = (host_hcca *)hc->hcca;
	if(requesttype & USB_ENDPOINT_IN) {
		direction = HCD_PID_IN;				//from endpoint
	} else {
		direction = HCD_PID_OUT;			//to endpoint
	}
	setup[0]=(uchar)requesttype;
  	setup[1]=(uchar)request;	   
  	setup[2]=(uchar)(value >> 8);
  	setup[3]=(uchar)(value);	  
  	setup[4]=(uchar)(index >> 8);
  	setup[5]=(uchar)(index);	
  	// lenght buf are the only where the order is inverted
  	setup[6]=(uchar)(size);
  	setup[7]=(uchar)(size >> 8);
	//endpoint number : 0 for control message
	endpoint_address = ((0 << 7) | dev->address) & 0x7ff;
	//ed = CreateEndpointControl(dev->address, 0, uchar speed, 4096, dev->hc);
	head_ptr = hc->_regs->ed_controlhead;
	//iterate all endpoint until specific endpoint found (endpoint created on driver creation)
	if(head_ptr == NULL) {
		//no endpoint found
		return;		//endpoint descriptor not found
	} else {
		while(head_ptr != NULL) {
			ed = (endpoint_desc *)head_ptr;
			if((ed->control & 0x7ff) == endpoint_address) {		//if en_number and f_address equal
				endpoint_set_mps(ed, dev->c_mps);		//endpoint found, set mps to control mps
				break;
			}
			head_ptr = ed->next_ed;
		}
	}
	if(head_ptr == NULL) return;
	req = create_request(size, setup, buffer, dev);
	if((ed->head_p & 0x01) == 1) {
	//	ed->head_p = (ed->head_p & 0xfffffff0);		//clear halted bit
		td = (transfer_desc *)hcca->done_head;
	} 
	//Uart_Printf("1st head_p : %x\n", ed->head_p);
	//send setup request, always 8 bytes (togl = 10)
	togl = 0;
	endpoint_skip(1, ed);
	td = create_transfer_descriptor(2 | togl, 1, HCD_PID_SETUP, 64, setup, ed, req);
	i = 0;
	//Uart_Printf("td : %x, next td : %x\n", td, td->next_td);
	while(i < size) {
		length = (size - i);
		(togl)?(togl=0):(togl=1);
		//Uart_Printf("togl : %x\n", togl);
		if(length > dev->c_mps) {
			length = dev->c_mps;
			td = create_transfer_descriptor(togl, 1, direction, length, (void *)((uint32)req->buffer + i), ed, req);
		} else {
			//last transfer descriptor
			td = create_transfer_descriptor(togl, 1, direction, length, (void *)((uint32)req->buffer + i), ed, req);
		}
		i += dev->c_mps;
	}
	//status packet (acknowledgement)
	if(requesttype & USB_ENDPOINT_IN) {
		direction = HCD_PID_OUT;				//to endpoint
	} else {
		direction = HCD_PID_IN;					//from endpoint
	}
	td = create_transfer_descriptor(3, 1, direction, 0, NULL, ed, req);
	switch(ed->type) {				//set HcCommonStatus according to endpoint type (bulk/control)
		case EP_BULK:
			hc->_regs->cmdstatus |= OHCI_BLF;
			break;
		case EP_CONTROL: 
			hc->_regs->cmdstatus |= OHCI_CLF;
			break;
		default:
			break;
	}
	endpoint_skip(0, ed);
	
	//check for req_complete and halt bit
	while((req->status & REQ_COMPLETED) == 0) {		//wait till request completed; ProcessDoneQueue
		if((ed->head_p & 0x01) == 0) {
			ProcessDoneQueue(hc);
			Uart_Printf("done head : %x\n", req->td_count);
		} else {
			Uart_Printf("halted : %x\n", ed->head_p);
			//ed->head_p = ed->head_p & 0xfffffff0;
			//if(ed->head_p == 0) break;
			//break;
		}
	}
	//Uart_Printf("tail_p : %x\n", ed->tail_p);
	//Uart_Printf("head_p : %x\n", ed->head_p);
	request_cleanup(req);
}

void ProcessDoneQueue(host_controller * hc) {
	uint32 cc;								//condition code for error
	transfer_desc * td;						//for control/bulk/interrupt
	transfer_desc * prev_td = NULL;
	endpoint_desc * ed;
	usbd_request * req;
	host_hcca * hcca = (host_hcca *)hc->_regs->hcca;
	if (hcca->done_head == 0) {
		//clear WD bit in HCInterruptStatus in order for HC to write new done queue
		hc->_regs->intrstatus = hc->_regs->intrstatus & ~OHCI_INTR_WDH;
		return;
	}						
	do {									//reverse queue
		td = (transfer_desc *)hcca->done_head;
		hcca->done_head = td->next_td;
		ed = (endpoint_desc *)td->endpoint;
		//Uart_Printf("ed->head_p : %x\n", ed->head_p);
		td->next_td = prev_td;
		prev_td = td;
		Uart_Printf("dequeuing : %x\n", (uint32)td);
	} while(hcca->done_head != 0);
	
	while (prev_td != NULL) {
		td = prev_td;
		prev_td = (transfer_desc *) (td->next_td);
		ed = (endpoint_desc *)td->endpoint;
		req = (usbd_request *)td->request;
		
		Uart_Printf("head_p : %x\n", ed->head_p);
		//Uart_Printf("processing : %x\n", (uint32)td);
		/*if (td->status == TD_CANCELED) {
			FreeTransferDescriptor(TD);
		}*/
		if(ed->type != EP_ISOCHRONOUS) {
			//control, interrupt, bulk
			cc = (td->control >> 28);
			if(cc == 0x00) {
				ed->head_p = td->next_td;
				td->status |= TD_COMPLETED;
				//set usbd request
				req->td_count--;
				if(req->td_count == 0x01) {
					req->status = REQ_COMPLETED;	//set request status
				}
				//dev_free(hc->_dev_heap, td);		//freed memory
			} else {
				//error handling here
				req->status = REQ_COMPLETED | REQ_ERROR;		//request set completed and error
				req->status = req->status | cc;					//request set error status
				//dev_free(hc->_dev_heap, td);					//freed memory
			}
		} else {
			//for isochronous
		}
	}
}

//for plug and play support, also add device to the list
void EnumerateDevices(host_controller * hc) {
	uchar i, devdescr_size;
	uchar address;
  	char buf[64];
	usb_driver * driver;
	usb_device * prev_dev;
	usb_device * dev;
	for(i=0; i<RHPORT_MAX_DEVICES; i++) {
		if(hc->_regs->rhportstatus[i] & RHPORT_CCS) {
			if(hc->_ports[i] & HC_PORT_OCCUPIED) {
				//port has been occupied, scan next port
			} else {
				hc->_ports[i] |= HC_PORT_OCCUPIED;
				//initialize port here
				Delay(100000);													//delay 100ms(stabilize power)
				hc->_regs->rhportstatus[i] |= (RHPORT_PES | RHPORT_PRS);		//enable port + reset
				Delay(50000);													//delay 50ms(wait for reset)
				hc->_regs->rhportstatus[i] &= ~(RHPORT_PRS);					//disable reset
				hc->_regs->rhportstatus[i] |= RHPORT_PES;						//enable port
				goto start_enumerate;
			}
		} else {
			if(hc->_ports[i] & HC_PORT_OCCUPIED) {
				hc->_ports[i] &= ~HC_PORT_OCCUPIED;
				hc->_regs->rhportstatus[i] &= ~RHPORT_PES;						//disable port
			}
		}
	}
	return;	//no new device found
	start_enumerate:
	dev = (usb_device *) dev_alloc(hc->_dev_heap, sizeof(usb_device), 4);
	dev->address = ED_DEFAULT_ADDRESS;		//default address for newly added device
  	dev->c_mps = 64;     	//set control mps to 8 for default command
	dev->driver = NULL;		//null driver
	dev->next = NULL;
	dev->status = DEV_CLOSED;
	dev->hc = hc;
	for(i=0; i<NO_OF_ENDPOINTS; i++) {
		dev->endpoints[i] = NULL;		//initialize all endpoints with null, dev->endpoints[] will be initialize with usb_device->driver->init
	}
  	/* ask first 8 bytes of device descriptor with this special 
   	 * GET Descriptor Request, when device address = 0
   	 */
	usb_control_msg(USB_ENDPOINT_IN, GET_DESCRIPTOR, 1, 0, 8, buf, dev);
	dev->c_mps = (uchar)buf[7];			//actual mps size for control
  	devdescr_size = (uchar)buf[0];		//actual descriptor size

  	/* get complete device descriptor */
  	usb_control_msg(USB_ENDPOINT_IN, GET_DESCRIPTOR, 1, 0, devdescr_size, buf, dev);

  	/* save only really neccessary values for this small usbstack */
  	//dev->class = (uchar) buf[4];
  	dev->subclass = (uchar) buf[5];
  	dev->protocol = (uchar) buf[6];
  	dev->VID = (uint16) (buf[9] << 8) | (buf[8]);
  	dev->PID = (uint16) (buf[11] << 8) | (buf[10]);
  	//dev->bcdDevice = (uint16) (buf[13] << 8) | (buf[12]);

	/* check for driver for this device */
	driver = _hcd_core.driver_list;
	while(driver != NULL) {
		if(driver->VID == dev->VID) {
			if(driver->PID == dev->PID) {
				break;				
			}
		}
		driver = driver->next;
	}
	if(driver == NULL) {		//driver not found
		Uart_Printf("driver not found, VID : %x, PID : %x\n", dev->VID, dev->PID);
		//freed memory, destroy device, return;
		//hcd_core_push(dev->address);		//push address to stack for future use(only if device address has been set)
		dev_free(hc->_dev_heap, dev);
		return;
	}
	//driver found
	dev->driver = driver;
	//set to new address
	address = hcd_core_next_address();
	usb_control_msg(USB_ENDPOINT_OUT, SET_ADDRESS, (address << 8), 0, 0, buf, dev);
  	dev->address = address;
	dev->driver->init(dev);		//initialize current device with the specified driver(creating endpoints)
	//add device to the list
	prev_dev = _hcd_core.device_list;
	if(prev_dev == NULL) {
		_hcd_core.device_list = dev;
	} else {
		while(prev_dev->next != NULL) {
			prev_dev = prev_dev->next;
		}
		prev_dev->next = dev;
	}
	//unfreed device allocated memory for future use
}

void usb_periodic(void) {		//support for multi-host controller
	host_controller * hc;
	hc = _hcd_core.hc_list;
	while(hc != NULL) {			//for-each host controllers
		Uart_Printf("enumerating devices\n");
		EnumerateDevices(hc);	//check for new devices
		Uart_Printf("processing done queue\n");
		ProcessDoneQueue(hc);	//process done transfer(freed memory)
		Uart_Printf("hc->next : %x\n", hc->next);
		hc = hc->next;			//iterate next host controller (if exist)
	}
}
