/*
 * nrf24l01p_hal_msp430.h
 *
 *  Created on: 28/09/2015
 *      Author: lucas.hartmann
 */

#ifndef NRF24L01P_HAL_EFM8BB1_BITBANG_H_
#define NRF24L01P_HAL_EFM8BB_BITBANG_H_

#include <SI_EFM8BB1_Register_Enums.h>
#include <SI_EFM8BB1_Defs.h>

////////////////////////////
// SPI Bitbanging Signals //
////////////////////////////
#define NRF_SPI_IS_BITBANG

#define NRF_SPI_MOSI_PORT P0
#define NRF_SPI_MOSI_PIN  (1<<2)
#define NRF_SPI_MISO_PORT P0
#define NRF_SPI_MISO_PIN  (1<<1)
#define NRF_SPI_CLK_PORT  P0
#define NRF_SPI_CLK_PIN   (1<<0)
#define NRF_SPI_CS_PORT   P0
#define NRF_SPI_CS_PIN    (1<<5)

#define nrfDigitalWrite(PORT, PIN, VALUE) { \
	if (VALUE) (PORT) |= (PIN); \
	else       (PORT) &= ~(PIN); \
}

#define nrfSpiMiso()          (NRF_SPI_MISO_PORT & NRF_SPI_MISO_PIN ? 1 : 0)
#define nrfSpiMosi(val)       nrfDigitalWrite(NRF_SPI_MOSI_PORT, NRF_SPI_MOSI_PIN, val)
#define nrfSpiClock(val)      nrfDigitalWrite(NRF_SPI_MCLK_PORT, NRF_SPI_CLK_PIN,  val)
#define nrfSpiChipSelect(val) nrfDigitalWrite(NRF_SPI_MCS_PORT, NRF_SPI_CS_PIN, !(val))

///////////////////////////////
// NRF24L01+ Control Signals //
///////////////////////////////
#define NRF_CE_PORT  P0
#define NRF_CE_PIN   (1<<6)
#define NRF_IRQ_PORT P0
#define NRF_IRQ_PIN  (1<<7)

#define nrfChipEnable(val) nrfDigitalWrite(NRF_CE_PORT, NRF_CE_PIN, val)
#ifdef NRF_IRQ_PIN
#define nrfIRQ()           (NRF_IRQ_PORT & NRF_IRQ_PIN ? 1 : 0)
#endif

// Hardware Initialization, called from nrfInit()
inline void nrfInit_hal() {
	// Nothing to do here, Should set everything from IDE's configurator.
}

#endif /* NRF24L01P_HAL_MSP430_H_ */
