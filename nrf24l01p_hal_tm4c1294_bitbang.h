/*
 * nrf24l01p_hal_msp430.h
 *
 *  Created on: 20/08/2015
 *      Author: lucas.hartmann
 */

#ifndef NRF24L01P_HAL_MSP430_H_
#define NRF24L01P_HAL_MSP430_H_

#define PART_TM4C1294NCPDT 1
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#define nrfDelay_us(us) (SysCtlDelay((us) * 120/3))

/////////////////////////
// SPI Bitbanging Layer//
/////////////////////////
#define NRF_SPI_MOSI_PORT (GPIO_PORTD_BASE)
#define NRF_SPI_MOSI_PIN  (GPIO_PIN_1)
#define NRF_SPI_MISO_PORT (GPIO_PORTD_BASE)
#define NRF_SPI_MISO_PIN  (GPIO_PIN_0)
#define NRF_SPI_CLK_PORT  (GPIO_PORTD_BASE)
#define NRF_SPI_CLK_PIN   (GPIO_PIN_3)
#define NRF_SPI_CS_PORT   (GPIO_PORTN_BASE) //cs-other
#define NRF_SPI_CS_PIN    (GPIO_PIN_3)

#define nrfSpiMiso()          (GPIOPinRead(NRF_SPI_MISO_PORT, NRF_SPI_MISO_PIN) ? 1 : 0)
#define nrfSpiMosi(val)       GPIOPinWrite(NRF_SPI_MOSI_PORT, NRF_SPI_MOSI_PIN, (val) ? NRF_SPI_MOSI_PIN : 0)
#define nrfSpiClock(val)      GPIOPinWrite(NRF_SPI_CLK_PORT,  NRF_SPI_CLK_PIN,  (val) ? NRF_SPI_CLK_PIN  : 0)
#define nrfSpiChipSelect(val) GPIOPinWrite(NRF_SPI_CS_PORT,   NRF_SPI_CS_PIN,   (val) ? 0 : NRF_SPI_CS_PIN)


///////////////////////////////
// NRF24L01+ Control Signals //
///////////////////////////////
#define NRF_CE_PORT  (GPIO_PORTH_BASE)
#define NRF_CE_PIN   (GPIO_PIN_3)
#define NRF_IRQ_PORT (GPIO_PORTP_BASE)
#define NRF_IRQ_PIN  (GPIO_PIN_2)

#define nrfIRQ()           (GPIOPinRead(NRF_IRQ_PORT, NRF_IRQ_PIN) ? 1 : 0)
#define nrfChipEnable(val) GPIOPinWrite(NRF_CE_PORT,  NRF_CE_PIN,  (val) ? NRF_CE_PIN  : 0)

// Hardware Initialization, called from nrfInit()
inline void nrfInit_hal() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);

	// SPI bitbang hal init
    GPIOPadConfigSet(NRF_SPI_CLK_PORT,  NRF_SPI_CLK_PIN,  GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(NRF_SPI_MOSI_PORT, NRF_SPI_MOSI_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(NRF_SPI_MISO_PORT, NRF_SPI_MISO_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(NRF_SPI_CS_PORT,   NRF_SPI_CS_PIN,   GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIODirModeSet(NRF_SPI_CLK_PORT,  NRF_SPI_CLK_PIN,  GPIO_DIR_MODE_OUT);
	GPIODirModeSet(NRF_SPI_MISO_PORT, NRF_SPI_MISO_PIN, GPIO_DIR_MODE_IN);
	GPIODirModeSet(NRF_SPI_MOSI_PORT, NRF_SPI_MOSI_PIN, GPIO_DIR_MODE_OUT);
	GPIODirModeSet(NRF_SPI_CS_PORT,   NRF_SPI_CS_PIN,   GPIO_DIR_MODE_OUT);
	nrfSpiChipSelect(0);
	nrfSpiClock(0);

    // NRF control pins
    GPIOPadConfigSet(NRF_CE_PORT,  NRF_CE_PIN,  GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIODirModeSet(NRF_CE_PORT,  NRF_CE_PIN,  GPIO_DIR_MODE_OUT);
	GPIODirModeSet(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_DIR_MODE_IN);
}

#endif
