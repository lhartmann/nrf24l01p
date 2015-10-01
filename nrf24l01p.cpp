#include "nrf24l01p.h"

// import HAL code here
uint8_t nrfSpiExchange(uint8_t x) {
	return nrfSpiExchange_hal(x);
}

// Initialization
void nrfInit() {
	nrfInit_hal();

	nrfChipEnable(0);
}

void nrfReadRegister(uint8_t reg, void *buf, uint8_t nbyte) {

    uint8_t *bbuf = (uint8_t *)buf;
    uint8_t cmd = (reg & 0x1F) | 0x00;
    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);

    while (nbyte--) 
        *bbuf++ = nrfSpiExchange(0);
    nrfSpiChipSelect (0);
}

void nrfWriteRegister(uint8_t reg, const void *buf, uint8_t nbyte) {

    uint8_t *bbuf = (uint8_t *)buf;
    uint8_t cmd = (reg & 0x1F) | 0x20;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    while (nbyte--)
        nrfSpiExchange(*bbuf++);

    nrfSpiChipSelect (0);
}

void nrfRead(void *buf, uint8_t nbyte) {
    uint8_t *bbuf = (uint8_t *)buf;
    uint8_t cmd = 0x61;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    while (nbyte--)
        *bbuf++ = nrfSpiExchange(0);

    nrfSpiChipSelect (0);
}

void nrfWrite(void *buf, uint8_t nbyte) {
    uint8_t *bbuf = (uint8_t *)buf;
    uint8_t cmd = 0xA0;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    while (nbyte--)
        nrfSpiExchange(*bbuf++);

    nrfSpiChipSelect (0);
}

void nrfFlush_Tx() {
    uint8_t cmd = 0xE1;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    nrfSpiChipSelect (0);
}

void nrfFlush_Rx() {
    uint8_t cmd = 0xE2;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    nrfSpiChipSelect (0);
}

void nrfReuse_Tx_Pl() {
    uint8_t cmd = 0xE3;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    nrfSpiChipSelect (0);
}

uint8_t nrfReadRxPayloadWidth() {
    uint8_t cmd = 0x60;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    cmd = nrfSpiExchange(0);

    nrfSpiChipSelect (0);

    return cmd;
}

void nrfWriteAckPayload(uint8_t pipe, void *buf, uint8_t nbyte) {
    uint8_t *bbuf = (uint8_t *)buf;
    uint8_t cmd = (pipe & 0x07) | 0xA8;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    while (nbyte--)
        nrfSpiExchange(*bbuf++);

    nrfSpiChipSelect (0);
}

void nrfWrite_NoAck(void *buf, uint8_t nbyte) {
    uint8_t *bbuf = (uint8_t *)buf;
    uint8_t cmd = 0xB0;

    nrfSpiChipSelect (1);
    nrfSpiExchange(cmd);
    while (nbyte--)
        nrfSpiExchange(*bbuf++);

    nrfSpiChipSelect (0);
}

void nrfNOP() {
    uint8_t cmd = 0xFF;

    nrfSpiChipSelect(1);
    nrfSpiExchange(cmd);
    nrfSpiChipSelect(0);
}

void nrfSetupPipe(uint8_t pipe, const void *address, bool rxenable, bool autoack, uint8_t payload) {
	if (pipe>5) return;
	uint8_t m = 1<<pipe;

	// Enable / Disable channel
	nrf_EN_RXADDR_t EN_RXADDR;
	nrfReadRegister(EN_RXADDR);
	*(uint8_t*)&EN_RXADDR &= ~m;
	if (rxenable) *(uint8_t*)&EN_RXADDR |= m;
	nrfWriteRegister(EN_RXADDR);

	// AutoAck
	nrf_EN_AA_t EN_AA;
	nrfReadRegister(EN_AA);
	*(uint8_t*)&EN_AA &= ~m;
	if (autoack) *(uint8_t*)&EN_AA |= m;
	nrfWriteRegister(EN_AA);

	// RX payload size
	nrf_DYNPD_t DYNPD;
	nrfReadRegister(DYNPD);
	if (payload==0 || payload>32) {
		// Enable dynamic payload size
		*(uint8_t*)&DYNPD |= m;
	} else {
		// Set to fixed payload size
		*(uint8_t*)&DYNPD &= ~m;
		nrfWriteRegister(NRF_RX_PW_P0+pipe, &payload, sizeof(payload));
	}
	nrfWriteRegister(DYNPD);

	// Address
	if (pipe <= 1) nrfWriteRegister(NRF_RX_ADDR_P0+pipe, address, 5); // Pipes 0-1, full address
	else           nrfWriteRegister(NRF_RX_ADDR_P0+pipe, address, 1); // Pipes 2-5, LSByte only
}
