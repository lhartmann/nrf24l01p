/*
 * nrf24l01p_hal_msp430.h
 *
 *  Created on: 20/08/2015
 *      Author: lucas.hartmann
 */

#ifndef NRF24L01P_HAL_MSP430_H_
#define NRF24L01P_HAL_MSP430_H_

#include "msp430.h"

////////////////////////////
// SPI Bitbanging Signals //
////////////////////////////
#define NRF_SPI_IS_BITBANG

#define NRF_SPI_MOSI_PIN (1<<0)
#define NRF_SPI_MISO_PIN (1<<3)
#define NRF_SPI_CLK_PIN  (1<<4)
#define NRF_SPI_CS_PIN   (1<<5)

#define nrfDigitalWrite(bit, value) { \
	if(value) P1OUT |= bit;           \
	else P1OUT &= ~(bit);             \
}

#define nrfSpiMiso()          (P1IN & NRF_SPI_MISO_PIN ? 1 : 0)
#define nrfSpiMosi(val)       nrfDigitalWrite(NRF_SPI_MOSI_PIN, val)
#define nrfSpiClock(val)      nrfDigitalWrite(NRF_SPI_CLK_PIN,  val)
#define nrfSpiChipSelect(val) nrfDigitalWrite(NRF_SPI_CS_PIN, !(val))

///////////////////////////////
// NRF24L01+ Control Signals //
///////////////////////////////
#define NRF_CE_PIN  (1<<6)
#define NRF_IRQ_PIN (1<<7)

#define nrfChipEnable(val) nrfDigitalWrite(NRF_CE_PIN, val)
#ifdef NRF_IRQ_PIN
#define nrfIRQ()           (P1IN & NRF_IRQ_PIN ? 1 : 0)
#endif

// Hardware Initialization, called from nrfInit()
inline void nrfInit_hal() {
	// Spi bitbang
    P1SEL  &= ~(NRF_SPI_CLK_PIN | NRF_SPI_CS_PIN | NRF_SPI_MISO_PIN | NRF_SPI_MOSI_PIN);
    P1SEL2 &= ~(NRF_SPI_CLK_PIN | NRF_SPI_CS_PIN | NRF_SPI_MISO_PIN | NRF_SPI_MOSI_PIN);
    P1DIR  &= ~(NRF_SPI_MISO_PIN);
    P1DIR  |= NRF_SPI_CLK_PIN | NRF_SPI_CS_PIN | NRF_SPI_MOSI_PIN;
	spiChipSelect(0);
	spiClock(0);

    // NRF control pins
#ifdef NRF_IRQ_PIN
    P1SEL  &= ~(NRF_CE_PIN | NRF_IRQ_PIN);
	P1SEL2 &= ~(NRF_CE_PIN | NRF_IRQ_PIN);
	P1DIR  &= ~(             NRF_IRQ_PIN);
#else
    P1SEL  &= ~(NRF_CE_PIN);
	P1SEL2 &= ~(NRF_CE_PIN);
#endif
	P1DIR  |= NRF_CE_PIN;
}

#endif /* NRF24L01P_HAL_MSP430_H_ */
