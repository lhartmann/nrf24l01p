/* 
 * File:   nrf24l01.h
 * Author: lucas.hartmann
 *
 * Created on 18 de Junho de 2014, 11:21
 */

#ifndef NRF24L01_H
#define	NRF24L01_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////
// Select proper Hardware Abstraction Layer:                              //
// Using compile-time defines to avoid unnecessary library edits.         //
////////////////////////////////////////////////////////////////////////////

// Arduino GPIO bitbang
#ifdef NRF_HAL_ARDUINO_BITBANG
#include "nrf24l01p_hal_arduino_bitbang.h"
#endif

// MSP430 GPIO bitbang
#ifdef NRF_HAL_MSP430_BITBANG
#include "nrf24l01p_hal_msp430_bitbang.h"
#endif

// EFM8BB1 GPIO bitbang
#ifdef NRF_HAL_EFM8BB1_BITBANG
#include "nrf24l01p_hal_efm8bb1_bitbang.h"
#endif

// TM4C1294 GPIO bitbang
#ifdef NRF_HAL_TM4C1294_BITBANG
#include "nrf24l01p_hal_tm4c1294_bitbang.h"
#endif

// BitBanging SPI byte exchange, if required.
#ifdef NRF_SPI_IS_BITBANG
inline uint8_t nrfSpiExchange_hal (uint8_t x) {
    uint8_t y = 0x00, i;
    for (i=0; i<8; ++i) {
        nrfSpiMosi (x&0x80);
        x = (x<<1);
        nrfSpiClock(1);
        y = ((y<<1) | nrfSpiMiso());
        nrfSpiClock(0);
    }
    return y;
}
#endif // NRF_SPI_IS_BITBANG

////////////////////////////////////////////////////////////////////////////
// SPI commands                                                           //
////////////////////////////////////////////////////////////////////////////
extern void nrfReadRegister(uint8_t reg, void *buf, uint8_t nbyte);
extern void nrfWriteRegister(uint8_t reg, const void *buf, uint8_t nbyte);
extern void nrfRead(void *buf, uint8_t nbyte);
extern void nrfWrite(void *buf, uint8_t nbyte);
extern void nrfFlush_Tx();
extern void nrfFlush_Rx();
extern void nrfReuse_Tx_Pl();
extern uint8_t nrfReadRxPayloadWidth();
extern void nrfWriteAckPayload(uint8_t pipe, void *buf, uint8_t nbyte);
extern void nrfWrite_NoAck(void *buf, uint8_t nbyte);
extern void nrfNOP();

// User-targeted functions
extern void nrfInit();

////////////////////////////////////////////////////////////////////////////
// Register description structures                                        //
//   For use with nrfReadRegister() and nrfWriteRegister                  //
////////////////////////////////////////////////////////////////////////////
#define NRF_CONFIG 0x00
#define NRF_CONFIG_SIZE 1
typedef struct {
    unsigned PRIM_RX : 1;
    unsigned PWR_UP  : 1;
    unsigned CRCO : 1;
    unsigned EN_CRC : 1;
    unsigned MASK_MAX_RT : 1;
    unsigned MASK_TX_DS : 1;
    unsigned MASK_RX_DR : 1;
    unsigned   :1;
} nrf_CONFIG_t;

#define NRF_EN_AA 0X01
#define NRF_EN_AA_SIZE 1
typedef struct {
    unsigned ENAA_P0 : 1;
    unsigned ENAA_P1 : 1;
    unsigned ENAA_P2 : 1;
    unsigned ENAA_P3 : 1;
    unsigned ENAA_P4 : 1;
    unsigned ENAA_P5 : 1;
    unsigned   :2;
    
} nrf_EN_AA_t;

#define NRF_EN_RXADDR 0x02
#define NRF_EN_RXADDR_SIZE 1
typedef struct {
    unsigned ERX_P0 : 1;
    unsigned ERX_P1 : 1;
    unsigned ERX_P2 : 1;
    unsigned ERX_P3 : 1;
    unsigned ERX_P4 : 1;
    unsigned ERX_P5 : 1;
    unsigned   : 2;

} nrf_EN_RXADDR_t;

#define NRF_SETUP_AW 0x03
#define NRF_SETUP_AW_SIZE 0x03
typedef struct {
    unsigned AW : 2;
    unsigned rsvd : 6;
} nrf_SETUP_AW_t;

#define NRF_SETUP_RETR 0X04
#define NRF_SETUP_RETR_SIZE 0X04
typedef struct {
    unsigned ARC : 4;
    unsigned ARD : 4;
} nrf_SETUP_RETR_t;

#define NRF_RF_CH 0x05
#define NRF_RF_CH_SIZE 1
typedef struct {
    unsigned rf_ch : 7;
    unsigned rsvd : 1;
} nrf_RF_CH_t;

#define NRF_RF_SETUP 0x06
#define NRF_RF_SETUP_SIZE 1
typedef struct {
    unsigned            : 1;
    unsigned RF_PWR     : 2;
    unsigned RF_DR_HIGH : 1;
    unsigned PLL_LOCK   : 1;
    unsigned RF_DR_LOW  : 1;
    unsigned            : 1;
    unsigned CONT_WAVE  : 1;
} nrf_RF_SETUP_t;

#define NRF_STATUS 0x07
#define NRF_STATUS_SIZE 1
typedef struct {
    unsigned TX_FULL : 1;
    unsigned RX_P_NO : 3;
    unsigned MAX_RT : 1;
    unsigned TX_DS : 1;
    unsigned RX_DR : 1;
    unsigned   : 1;
} nrf_STATUS_t;

#define NRF_OBSERVE_TX 0X08
#define NRF_OBSERVE_TX_SIZE 1
typedef struct {
    unsigned ARC_CNT : 4;
    unsigned PLOS_CNT : 4;
} nrf_OBSERVE_TX_t;

#define NRF_RPD 0X09
#define NRF_RPD_SIZE 1
typedef struct {
    unsigned rpd : 1;
    unsigned   : 8;
} nrf_RPD_t;

#define NRF_RX_ADDR_P0 0x0A
#define NRF_RX_ADDR_P0_SIZE 5
typedef uint8_t nrf_RX_ADDR_P0_t[5];

#define NRF_RX_ADDR_P1 0x0B
#define NRF_RX_ADDR_P1_SIZE 5
typedef uint8_t nrf_RX_ADDR_P1_t[5];

#define NRF_RX_ADDR_P2 0x0C
#define NRF_RX_ADDR_P2_SIZE 1
typedef uint8_t nrf_RX_ADDR_P2_t;

#define NRF_RX_ADDR_P3 0x0D
#define NRF_RX_ADDR_P3_SIZE 1
typedef uint8_t nrf_RX_ADDR_P3_t;

#define NRF_RX_ADDR_P4 0x0E
#define NRF_RX_ADDR_P4_SIZE 1
typedef uint8_t nrf_RX_ADDR_P4_t;

#define NRF_RX_ADDR_P5 0x0F
#define NRF_RX_ADDR_P5_SIZE 1
typedef uint8_t nrf_RX_ADDR_P5_t;

#define NRF_TX_ADDR 0x10
#define NRF_TX_ADDR_SIZE 5
typedef uint8_t nrf_TX_ADDR_t[5];

#define NRF_RX_PW_P0 0X11
#define NRF_RX_PW_P0_SIZE 1
typedef struct {
    unsigned rx_pw_p0 : 6;
    unsigned   : 2;
} nrf_RX_PW_P0_t;

#define NRF_RX_PW_P1 0X12
#define NRF_RX_PW_P1_SIZE 1
typedef struct {
    unsigned rx_pw_p1 : 6;
    unsigned   : 2;
} nrf_RX_PW_P1_t;

#define NRF_RX_PW_P2 0X13
#define NRF_RX_PW_P2_SIZE 1
typedef struct {
    unsigned rx_pw_p2 : 6;
    unsigned   : 2;
} nrf_RX_PW_P2_t;

#define NRF_RX_PW_P3 0X14
#define NRF_RX_PW_P3_SIZE 1
typedef struct {
    unsigned rx_pw_p3 : 6;
    unsigned   : 2;
} nrf_RX_PW_P3_t;

#define NRF_RX_PW_P4 0X15
#define NRF_RX_PW_P4_SIZE 1
typedef struct {
    unsigned rx_pw_p4 : 6;
    unsigned   : 2;
} nrf_RX_PW_P4_t;

#define NRF_RX_PW_P5 0X16
#define NRF_RX_PW_P5_SIZE 1
typedef struct {
    unsigned rx_pw_p5 : 6;
    unsigned   : 2;
} nrf_RX_PW_P5_t;

#define NRF_FIFO_STATUS 0X17
#define NRF_FIFO_STATUS_SIZE 1
typedef struct {
    unsigned RX_EMPTY : 1;
    unsigned RX_FULL : 1;
    unsigned   : 2;
    unsigned TX_EMPTY : 1;
    unsigned TX_FULL : 1;
    unsigned TX_REUSE : 1;
    unsigned   : 1;
} nrf_FIFO_STATUS_t;

#define NRF_DYNPD 0X1C
#define NRF_DYNPD_SIZE 1
typedef struct {
    unsigned DPL_P0 : 1;
    unsigned DPL_P1 : 1;
    unsigned DPL_P2 : 1;
    unsigned DPL_P3 : 1;
    unsigned DPL_P4 : 1;
    unsigned DPL_P5 : 1;
    unsigned   : 2;
} nrf_DYNPD_t;

#define NRF_FEATURE 0X1D
#define NRF_FEATURE_SIZE 1
typedef struct {
    unsigned EN_DYN_ACK : 1;
    unsigned EN_ACK_PAY : 1;
    unsigned EN_DPL : 1;
    unsigned   : 5;
} nrf_FEATURE_t;

////////////////////////////////////////////////////////////////////////////
// Register access shortcuts for C++                                      //
////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
inline void nrfWriteRegister(nrf_CONFIG_t       reg) { nrfWriteRegister(NRF_CONFIG,      &reg, NRF_CONFIG_SIZE); }
inline void nrfReadRegister (nrf_CONFIG_t      &reg) { nrfReadRegister (NRF_CONFIG,      &reg, NRF_CONFIG_SIZE); }
inline void nrfWriteRegister(nrf_EN_AA_t        reg) { nrfWriteRegister(NRF_EN_AA,       &reg, NRF_EN_AA_SIZE); }
inline void nrfReadRegister (nrf_EN_AA_t       &reg) { nrfReadRegister (NRF_EN_AA,       &reg, NRF_EN_AA_SIZE); }
inline void nrfWriteRegister(nrf_EN_RXADDR_t    reg) { nrfWriteRegister(NRF_EN_RXADDR,   &reg, NRF_EN_RXADDR_SIZE); }
inline void nrfReadRegister (nrf_EN_RXADDR_t   &reg) { nrfReadRegister (NRF_EN_RXADDR,   &reg, NRF_EN_RXADDR_SIZE); }
inline void nrfWriteRegister(nrf_SETUP_AW_t     reg) { nrfWriteRegister(NRF_SETUP_AW,    &reg, NRF_SETUP_AW_SIZE); }
inline void nrfReadRegister (nrf_SETUP_AW_t    &reg) { nrfReadRegister (NRF_SETUP_AW,    &reg, NRF_SETUP_AW_SIZE); }
inline void nrfWriteRegister(nrf_SETUP_RETR_t   reg) { nrfWriteRegister(NRF_SETUP_RETR,  &reg, NRF_SETUP_RETR_SIZE); }
inline void nrfReadRegister (nrf_SETUP_RETR_t  &reg) { nrfReadRegister (NRF_SETUP_RETR,  &reg, NRF_SETUP_RETR_SIZE); }
inline void nrfWriteRegister(nrf_RF_CH_t        reg) { nrfWriteRegister(NRF_RF_CH,       &reg, NRF_RF_CH_SIZE); }
inline void nrfReadRegister (nrf_RF_CH_t       &reg) { nrfReadRegister (NRF_RF_CH,       &reg, NRF_RF_CH_SIZE); }
inline void nrfWriteRegister(nrf_RF_SETUP_t     reg) { nrfWriteRegister(NRF_RF_SETUP,    &reg, NRF_RF_SETUP_SIZE); }
inline void nrfReadRegister (nrf_RF_SETUP_t    &reg) { nrfReadRegister (NRF_RF_SETUP,    &reg, NRF_RF_SETUP_SIZE); }
inline void nrfWriteRegister(nrf_STATUS_t       reg) { nrfWriteRegister(NRF_STATUS,      &reg, NRF_STATUS_SIZE); }
inline void nrfReadRegister (nrf_STATUS_t      &reg) { nrfReadRegister (NRF_STATUS,      &reg, NRF_STATUS_SIZE); }
inline void nrfWriteRegister(nrf_OBSERVE_TX_t   reg) { nrfWriteRegister(NRF_OBSERVE_TX,  &reg, NRF_OBSERVE_TX_SIZE); }
inline void nrfReadRegister (nrf_OBSERVE_TX_t  &reg) { nrfReadRegister (NRF_OBSERVE_TX,  &reg, NRF_OBSERVE_TX_SIZE); }
inline void nrfWriteRegister(nrf_RPD_t          reg) { nrfWriteRegister(NRF_RPD,         &reg, NRF_RPD_SIZE); }
inline void nrfReadRegister (nrf_RPD_t         &reg) { nrfReadRegister (NRF_RPD,         &reg, NRF_RPD_SIZE); }
inline void nrfWriteRegister(nrf_TX_ADDR_t      reg) { nrfWriteRegister(NRF_TX_ADDR,     &reg, NRF_TX_ADDR_SIZE); }
inline void nrfReadRegister (nrf_TX_ADDR_t     &reg) { nrfReadRegister (NRF_TX_ADDR,     &reg, NRF_TX_ADDR_SIZE); }
inline void nrfWriteRegister(nrf_RX_PW_P0_t     reg) { nrfWriteRegister(NRF_RX_PW_P0,    &reg, NRF_RX_ADDR_P0_SIZE); }
inline void nrfReadRegister (nrf_RX_PW_P0_t    &reg) { nrfReadRegister (NRF_RX_PW_P0,    &reg, NRF_RX_ADDR_P0_SIZE); }
inline void nrfWriteRegister(nrf_RX_PW_P1_t     reg) { nrfWriteRegister(NRF_RX_PW_P1,    &reg, NRF_RX_ADDR_P1_SIZE); }
inline void nrfReadRegister (nrf_RX_PW_P1_t    &reg) { nrfReadRegister (NRF_RX_PW_P1,    &reg, NRF_RX_ADDR_P1_SIZE); }
inline void nrfWriteRegister(nrf_RX_PW_P2_t     reg) { nrfWriteRegister(NRF_RX_PW_P2,    &reg, NRF_RX_ADDR_P2_SIZE); }
inline void nrfReadRegister (nrf_RX_PW_P2_t    &reg) { nrfReadRegister (NRF_RX_PW_P2,    &reg, NRF_RX_ADDR_P2_SIZE); }
inline void nrfWriteRegister(nrf_RX_PW_P3_t     reg) { nrfWriteRegister(NRF_RX_PW_P3,    &reg, NRF_RX_ADDR_P3_SIZE); }
inline void nrfReadRegister (nrf_RX_PW_P3_t    &reg) { nrfReadRegister (NRF_RX_PW_P3,    &reg, NRF_RX_ADDR_P3_SIZE); }
inline void nrfWriteRegister(nrf_RX_PW_P4_t     reg) { nrfWriteRegister(NRF_RX_PW_P4,    &reg, NRF_RX_ADDR_P4_SIZE); }
inline void nrfReadRegister (nrf_RX_PW_P4_t    &reg) { nrfReadRegister (NRF_RX_PW_P4,    &reg, NRF_RX_ADDR_P4_SIZE); }
inline void nrfWriteRegister(nrf_RX_PW_P5_t     reg) { nrfWriteRegister(NRF_RX_PW_P5,    &reg, NRF_RX_ADDR_P5_SIZE); }
inline void nrfReadRegister (nrf_RX_PW_P5_t    &reg) { nrfReadRegister (NRF_RX_PW_P5,    &reg, NRF_RX_ADDR_P5_SIZE); }
inline void nrfWriteRegister(nrf_FIFO_STATUS_t  reg) { nrfWriteRegister(NRF_FIFO_STATUS, &reg, NRF_FIFO_STATUS_SIZE); }
inline void nrfReadRegister (nrf_FIFO_STATUS_t &reg) { nrfReadRegister (NRF_FIFO_STATUS, &reg, NRF_FIFO_STATUS_SIZE); }
inline void nrfWriteRegister(nrf_DYNPD_t        reg) { nrfWriteRegister(NRF_DYNPD,       &reg, NRF_DYNPD_SIZE); }
inline void nrfReadRegister (nrf_DYNPD_t       &reg) { nrfReadRegister (NRF_DYNPD,       &reg, NRF_DYNPD_SIZE); }
inline void nrfWriteRegister(nrf_FEATURE_t      reg) { nrfWriteRegister(NRF_FEATURE,     &reg, NRF_FEATURE_SIZE); }
inline void nrfReadRegister (nrf_FEATURE_t     &reg) { nrfReadRegister (NRF_FEATURE,     &reg, NRF_FEATURE_SIZE); }
#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////
// Enums for use with coded registers                                     //
////////////////////////////////////////////////////////////////////////////

// For SETUP_AW.AW
enum nrfAddressWidth_e {
	nrfAddressWidth_3_bytes = 1,
	nrfAddressWidth_4_bytes = 2,
	nrfAddressWidth_5_bytes = 3
};

inline void nrfSetAddressWidth(nrfAddressWidth_e opt) {
	nrf_SETUP_AW_t reg;
	reg.AW = opt;
	nrfWriteRegister(reg);
}

// For SETUP_RETR
enum nrfAutoRetransmitDelay_e {
	nrfAutoRetransmitDelay_250us  =  0,
	nrfAutoRetransmitDelay_500us  =  1,
	nrfAutoRetransmitDelay_750us  =  2,
	nrfAutoRetransmitDelay_1000us =  3,
	nrfAutoRetransmitDelay_1250us =  4,
	nrfAutoRetransmitDelay_1500us =  5,
	nrfAutoRetransmitDelay_1750us =  6,
	nrfAutoRetransmitDelay_2000us =  7,
	nrfAutoRetransmitDelay_2250us =  8,
	nrfAutoRetransmitDelay_2500us =  9,
	nrfAutoRetransmitDelay_2750us = 10,
	nrfAutoRetransmitDelay_3000us = 11,
	nrfAutoRetransmitDelay_3250us = 12,
	nrfAutoRetransmitDelay_3500us = 13,
	nrfAutoRetransmitDelay_3750us = 14,
	nrfAutoRetransmitDelay_4000us = 15
};

inline void nrfSetAutoRetransmit(nrfAutoRetransmitDelay_e dly, uint8_t count) {
	nrf_SETUP_RETR_t SETUP_RETR;
	SETUP_RETR.ARD = dly;
	SETUP_RETR.ARC = count;
	nrfWriteRegister(SETUP_RETR);
}

inline void nrfSetAutoRetransmitDelay(nrfAutoRetransmitDelay_e dly) {
	nrf_SETUP_RETR_t reg;
	nrfReadRegister(reg);
	reg.ARD = dly;
	nrfWriteRegister(reg);
}
inline void nrfSetAutoRetransmitCount(uint8_t count) {
	nrf_SETUP_RETR_t reg;
	nrfReadRegister(reg);
	reg.ARC = count;
	nrfWriteRegister(reg);
}

// Channel/frequency setting
inline void nrfSetChannel(uint8_t ch) {
	nrf_RF_CH_t reg;
	reg.rf_ch = ch < 125 ? ch : 125;
	nrfWriteRegister(reg);
}
inline void nrfSetFrequency(uint16_t MHz) {
	if      (MHz < 2400) nrfSetChannel(2400 - 2400);
	else if (MHz > 2525) nrfSetChannel(2525 - 2400);
	else                 nrfSetChannel(MHz  - 2400);
}

// RF setup:
enum nrfDataRate_e {
	nrfDataRate_250kbps = 2,
	nrfDataRate_1Mbps   = 0,
	nrfDataRate_2Mbps   = 1
};
enum nrfTransmitPower_e {
	nrfTransmitPower_0dBm = 3,
	nrfTransmitPower_n6dBm = 2,
	nrfTransmitPower_n12dBm = 1,
	nrfTransmitPower_n18dBm = 0
};

inline void nrfSetupRadio(nrfDataRate_e dr, nrfTransmitPower_e tp) {
	nrf_RF_SETUP_t RF_SETUP;
	RF_SETUP.RF_DR_LOW  = dr & 1 ? 1 : 0;
	RF_SETUP.RF_DR_HIGH = dr & 2 ? 1 : 0;
	RF_SETUP.RF_PWR     = tp;
	RF_SETUP.PLL_LOCK   = 0;
	RF_SETUP.CONT_WAVE  = 0;
	nrfWriteRegister(RF_SETUP);
}

//
void nrfSetupPipe(uint8_t pipe, const void *address, bool rxenable, bool autoack, uint8_t payload);

inline void nrfSetPipeAddress(uint8_t pipe, const void *address) {
	nrfWriteRegister(NRF_RX_ADDR_P0+pipe, address, pipe <=1 ? 5 : 1);
}

// For CONFIG register
enum nrfCrc_e {
	nrfCrc_Disabled = 0,
	nrfCrc_8bit = 1,
	nrfCrc_16bit = 2
};

inline void nrfSetCrc(nrfCrc_e crc) {
	nrf_CONFIG_t CONFIG;
	nrfReadRegister(CONFIG);
	CONFIG.EN_CRC = (crc == nrfCrc_Disabled) ? 0 : 1;
	CONFIG.CRCO   = (crc == nrfCrc_8bit    ) ? 0 : 1;
	nrfWriteRegister(CONFIG);
}
inline nrfCrc_e nrfGetCrc() {
	nrf_CONFIG_t CONFIG;
	nrfReadRegister(CONFIG);
	if (CONFIG.EN_CRC) return nrfCrc_Disabled;
	if (CONFIG.CRCO)   return nrfCrc_8bit;
	return nrfCrc_16bit;
}

inline void nrfListen() {
	nrf_CONFIG_t CONFIG;
	nrfReadRegister(CONFIG);
	CONFIG.PWR_UP  = 1;
	CONFIG.PRIM_RX = 1;
	nrfWriteRegister(CONFIG);
	nrfDelay_us(100);
	nrfChipEnable(true);
}

inline void nrfDisable() {
	nrfChipEnable(false);
}

#endif	/* NRF24L01_H */

