/*
 * nrf24l01p_hal_arduino_bitbang.h
 *
 *  Created on: 20/08/2015
 *      Author: lucas.hartmann
 */

#ifndef TM4C1294_NRF_NRF24L01P_HAL_ARDUINO_BITBANG_H_
#define TM4C1294_NRF_NRF24L01P_HAL_ARDUINO_BITBANG_H_

#include "Arduino.h"

////////////////////////////
// SPI Bitbanging Signals //
////////////////////////////
#define NRF_SPI_IS_BITBANG

#define SPI_MISO_PIN 12
#define SPI_MOSI_PIN 11
#define SPI_CLK_PIN 13
#define SPI_CS_PIN 7

#define spiMiso()          digitalRead(SPI_MISO_PIN)
#define spiMosi(val)       digitalWrite(SPI_MOSI_PIN, (val) ? HIGH : LOW)
#define spiClock(val)      digitalWrite(SPI_CLK_PIN,  (val) ? HIGH : LOW)
#define spiChipSelect(val) digitalWrite(SPI_CS_PIN,   (val) ? HIGH : LOW)

///////////////////////////////
// NRF24L01+ Control Signals //
///////////////////////////////
#define NRF_CE_PIN  8
//#define NRF_IRQ_PIN -1 // Not connected

#define nrfChipEnable(val) digitalWrite(NRF_CE_PIN, val)
#ifdef NRF_IRQ_PIN
#define nrfIRQ()           digitalRead(NRF_IRQ_PIN)
#endif

inline void nrfInit_hal() {
	pinMode(SPI_MISO_PIN, INPUT);
	pinMode(SPI_MOSI_PIN, OUTPUT);
	pinMode(SPI_CLK_PIN,  OUTPUT);
	pinMode(SPI_CS_PIN,   OUTPUT);
	pinMode(NRF_CE_PIN,   OUTPUT);
#ifdef NRF_IRQ_PIN
	pinMode(NRF_IRQ_PIN,  INPUT);
#endif
	spiChipSelect(0);
	spiClock(0);
}

#endif
