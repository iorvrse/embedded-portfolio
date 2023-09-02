#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "main.h"
#include "spi.h"
#include "usart.h"
#include <string.h>

/* Instruction Mnemonics */
#define R_REGISTER          0b00000000
#define W_REGISTER          0b00100000
#define R_RX_PAYLOAD        0b01100001
#define W_TX_PAYLOAD        0b10100000
#define FLUSH_TX            0b11100001
#define FLUSH_RX            0b11100010
#define REUSE_TX_PL         0b11100011
#define R_RX_PL_WID         0b01100000
#define W_ACK_PAYLOAD       0b10101000
#define W_TX_PAYLOAD_NOACK  0b10110000
#define NOP                 0b11111111

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define RPD         0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

#define NRF24_SPI       SPI2
#define NRF24_CE_PORT   NRF24_CE_GPIO_Port
#define NRF24_CE_PIN    NRF24_CE_Pin

#define NRF24_CSN_PORT  SPI2_CS_GPIO_Port
#define NRF24_CSN_PIN   SPI2_CS_Pin

#ifndef PAYLOAD_LENGTH
    #define PAYLOAD_LENGTH   32
#endif

typedef enum
{
    _250kbps = 2,
    _1Mbps   = 0,
    _2Mbps   = 1
} data_rate;

typedef enum
{
    _0dBm  = 3,
    _6dBm  = 2,
    _12dBm = 1,
    _18dBm = 0
} power;

typedef enum
{
    EN_DPL  = 2,
    EN_ACK_PAY = 1,
    EN_DYN_ACK = 0
} feature;

static inline void csn_high(void)
{
    LL_GPIO_SetOutputPin(NRF24_CSN_PORT, NRF24_CSN_PIN);
}

static inline void csn_low(void)
{
    LL_GPIO_ResetOutputPin(NRF24_CSN_PORT, NRF24_CSN_PIN);
}

static inline void ce_high(void)
{
    LL_GPIO_SetOutputPin(NRF24_CE_PORT, NRF24_CE_PIN);
}

static inline void ce_low(void)
{
    LL_GPIO_ResetOutputPin(NRF24_CE_PORT, NRF24_CE_PIN);
}

void nrf24_write(uint8_t reg, uint8_t data);

uint8_t nrf24_read(uint8_t reg);

void nrf24_write_mb(uint8_t reg, uint8_t *data, uint16_t size);

void nrf24_read_mb(uint8_t reg, uint8_t *data, uint16_t size);

void nrf24_reset(uint8_t reg);

void nrf24_init(void);

void nrf24_tx(uint8_t *address);

uint8_t nrf24_transmit(uint8_t *data);

void nrf24_rx(uint8_t pipe, uint8_t *address);

void nrf24_receive(uint8_t *data);

void nrf24_print_register_value(void);

uint8_t nrf24_available(uint8_t pipenum);

void nrf24_prx_mode(void);

void nrf24_ptx_mode(void);

void nrf24_enable_autoack(uint8_t pipe);

void nrf24_enable_dynamic_payload(uint8_t pipe);

void nrf24_feature(feature ftr);

void nrf24_flush_rx_fifo(void);

void nrf24_flush_tx_fifo(void);

uint8_t nrf24_get_status(void);

uint8_t nrf24_get_fifo_status(void);

void nrf24_rx_set_payload_widths(uint8_t pipe, uint8_t bytes);

void nrf24_clear_rx_dr(void);

void nrf24_clear_tx_ds(void);

void nrf24_clear_max_rt(void);

void nrf24_power_up(void);

void nrf24_power_down(void);

void nrf24_set_crc_length(uint8_t bytes);

void nrf24_set_address_width(uint8_t bytes);

void nrf24_set_retransmit_count(uint8_t cnt);

void nrf24_set_retransmit_delay(uint16_t us);

void nrf24_rf_channel(uint8_t channel);

void nrf24_set_rf_power(power dBm);

void nrf24_set_rf_data_rate(data_rate bps);

#endif