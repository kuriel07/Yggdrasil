#include "host.h"

void host_init() {

}


void host_suspend(host_controller * hc) {		//disable host operation for changing purpose
	hc->_regs->control &= ~(OHCI_CTRL_CLE | OHCI_CTRL_BLE);
}

void host_resume(host_controller * hc) {		//enable host operation
	hc->_regs->control |= (OHCI_CTRL_CLE | OHCI_CTRL_BLE);
}
