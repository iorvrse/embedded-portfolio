#include "spi.h"

void spi_master_init(void)
{
    DDR_SPI &= ~(1 << DD_MISO);
    DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK);      // set MOSI and SCK pin as output
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPI2X); // enable SPI, master mode
    spi_set_bitorder(MSBFIRST);
}

void spi_set_bitorder(uint8_t bitorder) {
    switch (bitorder)
    {
        case LSBFIRST:
            SPCR |= _BV(DORD);
            break;
        case MSBFIRST:
            SPCR &= ~(_BV(DORD));
            break;
    }
}

void spi_set_clockdiv(uint8_t clkdiv)
{
    SPCR = (SPCR & ~SPI_CLOCK_MASK) | (clkdiv & SPI_CLOCK_MASK);
    SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((clkdiv >> 2) & SPI_2XCLOCK_MASK);
}

void spi_set_datamode(uint8_t datamode) {
    SPCR = (SPCR & ~SPI_MODE_MASK) | datamode;
}

uint8_t spi_transceive(uint8_t data)
{
    SPDR = data;
    loop_until_bit_is_set(SPSR, SPIF);
    return SPDR;
}

void spi_transmit(uint8_t *pdata, uint16_t size)
{
    while (size--)
    {
        spi_transceive(*pdata++);
    }
}

void spi_receive(uint8_t *pdata, uint16_t size)
{
    while (size)
    {
        *pdata++ = spi_transceive(0xFF);
    }
}

void spi_transmit_receive(uint8_t *ptxdata, uint8_t *prxdata, uint16_t size)
{
    while (size)
    {
        spi_transceive(*ptxdata++);
        *prxdata++ = spi_transceive(0xFF);
    }
}

void spi_enable_interrupt(void)
{
    SPCR |= _BV(SPIE);
}

void spi_disable_interrupt(void)
{
    SPCR |= _BV(SPIE);
}