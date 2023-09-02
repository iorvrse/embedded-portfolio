#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_CLOCK_DIV4      0x00
#define SPI_CLOCK_DIV16     0x01
#define SPI_CLOCK_DIV64     0x02
#define SPI_CLOCK_DIV128    0x03
#define SPI_CLOCK_DIV2      0x04
#define SPI_CLOCK_DIV8      0x05
#define SPI_CLOCK_DIV32     0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK       0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK      0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK    0x01  // SPI2X = bit 0 on SPSR

#define DDR_SPI DDRB

#define DD_SS   DDB4
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK  DDB7

void spi_master_init(void);

void spi_set_bitorder(uint8_t bitorder);

void spi_set_clockdiv(uint8_t clkdiv);

void spi_set_datamode(uint8_t datamode);

uint8_t spi_transceive(uint8_t data);

void spi_transmit(uint8_t *pdata, uint16_t size);

void spi_receive(uint8_t *pdata, uint16_t size);

void spi_transmit_receive(uint8_t *ptxdata, uint8_t *prxdata, uint16_t size);

void spi_enable_interrupt(void);

void spi_disable_interrupt(void);

#endif /* _SPI_H_ */