#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "nrf24l01p.h"

const uint64_t NRF_PIPE0 = 0xF0F0F0F0D1;
const uint64_t NRF_PIPE1 = 0xF0F0F0F0E1;
uint64_t t64 = 0;

int main(void) {
	nrfInit();
	if (1) {
		nrf_FEATURE_t FEATURE;
		nrfReadRegister(FEATURE);
		FEATURE.EN_ACK_PAY = 1;
		FEATURE.EN_DPL     = 1;
		FEATURE.EN_DYN_ACK = 1;
//		nrfWriteRegister(FEATURE);
	}
	nrfSetFrequency(2468);
	nrfSetAutoRetransmit(nrfAutoRetransmitDelay_1500us, 15);
	nrfSetupRadio(nrfDataRate_1Mbps, nrfTransmitPower_0dBm);
	nrfSetCrc(nrfCrc_16bit);
	nrfSetupPipe(0, &NRF_PIPE0, true, true, 0x20);
	nrfSetupPipe(1, &NRF_PIPE1, true, true, 0x20);
	if (1) {
		nrf_STATUS_t STATUS;
		STATUS.MAX_RT = 1;
		STATUS.RX_DR  = 1;
		STATUS.TX_DS  = 1;
		nrfWriteRegister(STATUS);
	}
	nrfFlush_Rx();
	nrfFlush_Tx();
	nrfListen();

	while (true) {
		if (1) {
			// Read several registers to memory, for debugger access.
			nrf_FEATURE_t     FEATURE;     nrfReadRegister(FEATURE);
			nrf_CONFIG_t      CONFIG;      nrfReadRegister(CONFIG);
			nrf_EN_AA_t       EN_AA;       nrfReadRegister(EN_AA);
			nrf_EN_RXADDR_t   EN_RXADDR;   nrfReadRegister(EN_RXADDR);
			nrf_SETUP_AW_t    SETUP_AW;    nrfReadRegister(SETUP_AW);
			nrf_SETUP_RETR_t  SETUP_RETR;  nrfReadRegister(SETUP_RETR);
			nrf_RF_CH_t       RF_CH;       nrfReadRegister(RF_CH);
			nrf_RF_SETUP_t    RF_SETUP;    nrfReadRegister(RF_SETUP);
			nrf_STATUS_t      STATUS;      nrfReadRegister(STATUS);
			nrf_OBSERVE_TX_t  OBSERVE_TX;  nrfReadRegister(OBSERVE_TX);
//			nrf_RPD
			nrf_RX_PW_P0_t    RX_PW_P0;    nrfReadRegister(RX_PW_P0);
			nrf_RX_PW_P1_t    RX_PW_P1;    nrfReadRegister(RX_PW_P1);
			nrf_RX_PW_P2_t    RX_PW_P2;    nrfReadRegister(RX_PW_P2);
			nrf_RX_PW_P3_t    RX_PW_P3;    nrfReadRegister(RX_PW_P3);
			nrf_RX_PW_P4_t    RX_PW_P4;    nrfReadRegister(RX_PW_P4);
			nrf_RX_PW_P5_t    RX_PW_P5;    nrfReadRegister(RX_PW_P5);
			nrf_FIFO_STATUS_t FIFO_STATUS; nrfReadRegister(FIFO_STATUS);
			nrf_DYNPD_t       DYNPD;       nrfReadRegister(DYNPD);
			asm(" nop");
		}

		// Wait for IRQ pin to go high
//		while(nrfIRQ());

		// Figure out hhy is IRQ high
		nrf_STATUS_t STATUS;
		nrfReadRegister(STATUS);

		// Ignone anything but data ready.
		if (!STATUS.RX_DR) {
			continue;
		}

		uint8_t n = nrfReadRxPayloadWidth();
		uint8_t data[32];
		nrfRead(data, n);

		nrfWriteRegister(STATUS);
		asm(" nop");
	}
}

// Error function for TI's TIVA-C driver library.
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {
	// Do nothing, just ignore.
}
#endif
