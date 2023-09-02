#include "nrf24l01.h"

void nrf24_write(uint8_t reg, uint8_t data)
{
    csn_low();

    spi_transceive(reg | W_REGISTER);
    spi_transceive(data);

    csn_high();
}

uint8_t nrf24_read(uint8_t reg)
{
    csn_low();

    spi_transceive(reg | R_REGISTER);
    uint8_t received = spi_transceive(NOP);

    csn_high();

    return received;
}

void nrf24_write_mb(uint8_t reg, uint8_t *data, uint16_t size)
{
    csn_low();

    spi_transceive(reg | W_REGISTER);
    spi_transmit(data, size);

    csn_high();
}

void nrf24_read_mb(uint8_t reg, uint8_t *data, uint16_t size)
{
    csn_low();

    spi_transceive(reg | R_REGISTER);
    spi_receive(data, size);

    csn_high();
}

void nrf24_reset(uint8_t reg)
{
    switch (reg)
    {
        case STATUS:
            nrf24_write(STATUS, 0x00);
            break;
        case FIFO_STATUS:
            nrf24_write(FIFO_STATUS, 0x11);
            break;
        default:
            nrf24_write(CONFIG, 0x08);
            nrf24_write(EN_AA, 0x3F);
            nrf24_write(EN_RXADDR, 0x03);
            nrf24_write(SETUP_AW, 0x03);
            nrf24_write(SETUP_RETR, 0x03);
            nrf24_write(RF_CH, 0x02);
            nrf24_write(RF_SETUP, 0x0E);
            nrf24_write(STATUS, 0x00);
            nrf24_write(OBSERVE_TX, 0x00);
            nrf24_write(CD, 0x00);
            uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
            nrf24_write_mb(RX_ADDR_P0, rx_addr_p0_def, 5);
            uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
            nrf24_write_mb(RX_ADDR_P1, rx_addr_p1_def, 5);
            nrf24_write(RX_ADDR_P2, 0xC3);
            nrf24_write(RX_ADDR_P3, 0xC4);
            nrf24_write(RX_ADDR_P4, 0xC5);
            nrf24_write(RX_ADDR_P5, 0xC6);
            uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
            nrf24_write_mb(TX_ADDR, tx_addr_def, 5);
            nrf24_write(RX_PW_P0, 0);
            nrf24_write(RX_PW_P1, 0);
            nrf24_write(RX_PW_P2, 0);
            nrf24_write(RX_PW_P3, 0);
            nrf24_write(RX_PW_P4, 0);
            nrf24_write(RX_PW_P5, 0);
            nrf24_write(FIFO_STATUS, 0x11);
            nrf24_write(DYNPD, 0);
            nrf24_write(FEATURE, 0);
            break;
	}
}

void nrf24_init(void)
{
    DDR_SPI = DDR_SPI
        | _BV(NRF24_CE_DD)
        | _BV(NRF24_CSN_DD);
    
    ce_low();

    nrf24_reset(0);
    nrf24_write(EN_AA, 0);          // No Auto Ack
    nrf24_write(SETUP_RETR, 0);     // No Retransmission
    nrf24_write(CONFIG, 0);         // Will set later
    nrf24_write(EN_RXADDR, 0);      // Will set later
    nrf24_set_address_width(5);
    nrf24_rf_channel(10);
    nrf24_set_rf_data_rate(_2Mbps);
    nrf24_set_rf_power(_0dBm);

    ce_high();
}

// For debug purpose
void nrf24_print_register_value(void)
{
    uint8_t data[9];
    uint8_t addr[5];
    data[0] = nrf24_read(CONFIG);
    data[1] = nrf24_read(EN_AA);
    data[2] = nrf24_read(EN_RXADDR);
    data[3] = nrf24_read(SETUP_AW);
    data[4] = nrf24_read(SETUP_RETR);
    data[5] = nrf24_read(RF_CH);
    data[6] = nrf24_read(RF_SETUP);
    data[7] = nrf24_read(STATUS);
    data[8] = nrf24_read(FIFO_STATUS);

    usart_putstring((uint8_t *)"CONFIG: ", sizeof("CONFIG: "));
    usart_uint8(&data[0]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"EN_AA: ", sizeof("EN_AA: "));
    usart_uint8(&data[1]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"EN_RXADDR: ", sizeof("EN_RXADDR: "));
    usart_uint8(&data[2]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"SETUP_AW: ", sizeof("SETUP_AW: "));
    usart_uint8(&data[3]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"SETUP_RETR: ", sizeof("SETUP_RETR: "));
    usart_uint8(&data[4]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"RF_CH: ", sizeof("RF_CH: "));
    usart_uint8(&data[5]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"RF_SETUP: ", sizeof("RF_SETUP: "));
    usart_uint8(&data[6]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"STATUS: ", sizeof("STATUS: "));
    usart_uint8(&data[7]);
    usart_putchar('\r');
    usart_putchar('\n');

    usart_putstring((uint8_t *)"FIFO_STATUS: ", sizeof("FIFO_STATUS: "));
    usart_uint8(&data[8]);
    usart_putchar('\r');
    usart_putchar('\n');
    
    nrf24_read_mb(TX_ADDR, addr, 5);
    usart_putstring((uint8_t *)"TX_ADDR: ", sizeof("TX_ADDR: "));
    for (uint8_t i = 0; i < 5; i++)
    {
        usart_uint8(&addr[i]);
        usart_putchar(' ');
    }
    usart_putchar('\r');
    usart_putchar('\n');

    nrf24_read_mb(RX_ADDR_P0, addr, 5);
    usart_putstring((uint8_t *)"RX_ADDR_P0: ", sizeof("RX_ADDR_P0: "));
    for (uint8_t i = 0; i < 5; i++)
    {
        usart_uint8(&addr[i]);
        usart_putchar(' ');
    }
    usart_putchar('\r');
    usart_putchar('\n');
    nrf24_read_mb(RX_ADDR_P1, addr, 5);
    
    nrf24_read_mb(RX_ADDR_P1, addr, 5);
    usart_putstring((uint8_t *)"RX_ADDR_P1: ", sizeof("RX_ADDR_P1: "));
    for (uint8_t i = 0; i < 5; i++)
    {
        usart_uint8(&addr[i]);
        usart_putchar(' ');
    }
    usart_putchar('\r');
    usart_putchar('\n');

    addr[0] = nrf24_read(RX_ADDR_P2);
    usart_putstring((uint8_t *)"RX_ADDR_P2: ", sizeof("RX_ADDR_P2: "));
    usart_uint8(&addr[0]);
    usart_putchar('\r');
    usart_putchar('\n');

    addr[0] = nrf24_read(RX_ADDR_P3);
    usart_putstring((uint8_t *)"RX_ADDR_P3: ", sizeof("RX_ADDR_P3: "));
    usart_uint8(&addr[0]);
    usart_putchar('\r');
    usart_putchar('\n');

    addr[0] = nrf24_read(RX_ADDR_P4);
    usart_putstring((uint8_t *)"RX_ADDR_P4: ", sizeof("RX_ADDR_P4: "));
    usart_uint8(&addr[0]);
    usart_putchar('\r');
    usart_putchar('\n');

    addr[0] = nrf24_read(RX_ADDR_P5);
    usart_putstring((uint8_t *)"RX_ADDR_P5: ", sizeof("RX_ADDR_P5: "));
    usart_uint8(&addr[0]);
    usart_putchar('\r');
    usart_putchar('\n');
}

void nrf24_tx(uint8_t *address)
{
    ce_low();

    nrf24_write_mb(TX_ADDR, address, 5);

    nrf24_ptx_mode();
    nrf24_power_up();

    ce_high();
}

uint8_t nrf24_transmit(uint8_t *data)
{
    csn_low();

    spi_transceive(W_TX_PAYLOAD);
    spi_transmit(data, PAYLOAD_LENGTH);

    csn_high();

    uint8_t fifostatus = nrf24_read(FIFO_STATUS);

	if ((fifostatus & _BV(4)) && !(fifostatus & _BV(3)))
	{
		nrf24_flush_tx_fifo();
		nrf24_reset (FIFO_STATUS);

		return 1;
	}

	return 0;
}

void nrf24_rx(uint8_t pipe, uint8_t *address)
{
    ce_low();

    uint8_t en_rxaddr = nrf24_read(EN_RXADDR);
    nrf24_write(EN_RXADDR, en_rxaddr | _BV(pipe));

    switch (pipe)
    {
        case 0:
            nrf24_write_mb(RX_ADDR_P0, address, 5);
            nrf24_write(RX_PW_P0, PAYLOAD_LENGTH);
            break;

        case 1:
            nrf24_write_mb(RX_ADDR_P1, address, 5);
            nrf24_write(RX_PW_P1, PAYLOAD_LENGTH);
            break;

        case 2:
            nrf24_write(RX_ADDR_P2, *address);
            nrf24_write(RX_PW_P2, PAYLOAD_LENGTH);
            break;
        case 3:
            nrf24_write(RX_ADDR_P3, *address);
            nrf24_write(RX_PW_P3, PAYLOAD_LENGTH);
            break;
        case 4:
            nrf24_write(RX_ADDR_P4, *address);
            nrf24_write(RX_PW_P4, PAYLOAD_LENGTH);
            break;
        case 5:
            nrf24_write(RX_ADDR_P5, *address);
            nrf24_write(RX_PW_P5, PAYLOAD_LENGTH);
            break;
    }

    nrf24_prx_mode();
    nrf24_power_up();

    ce_high();
}

void nrf24_receive(uint8_t *data)
{
    csn_low();

    spi_transceive(R_RX_PAYLOAD);
    spi_receive(data, PAYLOAD_LENGTH);

    csn_high();

    nrf24_flush_rx_fifo();
}

uint8_t nrf24_available(uint8_t pipenum)
{
	uint8_t status = nrf24_read(STATUS);

	if ((status & _BV(6)) && (status & (pipenum << 1)))
	{
		nrf24_write(STATUS, _BV(6));
		return 1;
	}

	return 0;
}

void nrf24_prx_mode(void)
{
    uint8_t config = nrf24_read(CONFIG);
    config |= _BV(0);

    nrf24_write(CONFIG, config);
}

void nrf24_ptx_mode(void)
{
    uint8_t config = nrf24_read(CONFIG);
    config &= ~_BV(0);

    nrf24_write(CONFIG, config);
}

void nrf24_enable_autoack(uint8_t pipe)
{
    uint8_t en_aa = nrf24_read(EN_AA) | _BV(pipe);
    nrf24_write(EN_AA, en_aa);
}

void nrf24_enable_dynamic_payload(uint8_t pipe)
{
    uint8_t dynpd = nrf24_read(DYNPD) | _BV(pipe);
    nrf24_write(DYNPD, dynpd);
}

void nrf24_feature(feature ftr)
{
    uint8_t read_feature = nrf24_read(FEATURE);
    switch (ftr)
    {
        case EN_DPL:
            nrf24_write(FEATURE, read_feature | _BV(2));
            break;
        case EN_ACK_PAY:
            nrf24_write(FEATURE, read_feature | _BV(1));
            break;
        case EN_DYN_ACK:
            nrf24_write(FEATURE, read_feature | _BV(0));
            break;
    }
}

void nrf24_flush_rx_fifo(void)
{
    csn_low();
    spi_transceive(FLUSH_RX);
    csn_high();
}

void nrf24_flush_tx_fifo(void)
{
    csn_low();
    spi_transceive(FLUSH_TX);
    csn_high();
}

uint8_t nrf24_get_status(void)
{
    uint8_t status;

    csn_low();
    status = spi_transceive(NOP);
    csn_high(); 

    return status;
}

uint8_t nrf24_get_fifo_status(void)
{
    return nrf24_read(FIFO_STATUS);
}

void nrf24_rx_set_payload_widths(uint8_t pipe, uint8_t bytes)
{
    switch (pipe)
    {
        case 0:
            nrf24_write(RX_PW_P0, bytes);
            break;
        case 1:
            nrf24_write(RX_PW_P1, bytes);
            break;
        case 2:
            nrf24_write(RX_PW_P2, bytes);
            break;
        case 3:
            nrf24_write(RX_PW_P3, bytes);
            break;
        case 4:
            nrf24_write(RX_PW_P4, bytes);
            break;
        case 5:
            nrf24_write(RX_PW_P5, bytes);
            break;
    }
}

void nrf24_clear_rx_dr(void)
{
    uint8_t status = nrf24_get_status() | 0x40;

    nrf24_write(STATUS, status);
}

void nrf24_clear_tx_ds(void)
{
    uint8_t status = nrf24_get_status() | 0x20;

    nrf24_write(STATUS, status);     
}

void nrf24_clear_max_rt(void)
{
    uint8_t status = nrf24_get_status() | 0x10;
    nrf24_write(STATUS, status); 
}

void nrf24_power_up(void)
{
    uint8_t config = nrf24_read(CONFIG);
    config |= _BV(1);

    nrf24_write(CONFIG, config);
}

void nrf24_power_down(void)
{
    uint8_t config = nrf24_read(CONFIG);
    config &= ~_BV(1);

    nrf24_write(CONFIG, config);
}

void nrf24_set_crc_length(uint8_t bytes)
{
    uint8_t config = nrf24_read(CONFIG);
    
    switch(bytes)
    {
        // CRCO bit in CONFIG resiger set 0
        case 1:
            config &= ~_BV(2);
            break;
        // CRCO bit in CONFIG resiger set 1
        case 2:
            config |= _BV(2);
            break;
    }

    nrf24_write(CONFIG, config);
}

void nrf24_set_address_width(uint8_t bytes)
{
    nrf24_write(SETUP_AW, bytes - 2);
}

void nrf24_set_retransmit_count(uint8_t cnt)
{
    uint8_t setup_retr = nrf24_read(SETUP_RETR);
    
    setup_retr &= 0xF0;
    setup_retr |= cnt;
    nrf24_write(SETUP_RETR, setup_retr);
}

void nrf24_set_retransmit_delay(uint16_t us)
{
    uint8_t setup_retr = nrf24_read(SETUP_RETR);

    setup_retr &= 0x0F;
    setup_retr |= ((us / 250) - 1) << 4;
    nrf24_write(SETUP_RETR, setup_retr);
}

void nrf24_rf_channel(uint8_t channel)
{
    nrf24_write(RF_CH, channel);
}

void nrf24_set_rf_power(uint8_t dBm)
{
    uint8_t rf_setup = (nrf24_read(RF_SETUP) & 0xF9) | (dBm << 1);
    nrf24_write(RF_SETUP, rf_setup);
}

void nrf24_set_rf_data_rate(data_rate bps)
{
    uint8_t rf_setup = nrf24_read(RF_SETUP) & 0xD7;
    
    switch(bps)
    {
        case _1Mbps:
            break;
        case _2Mbps:
            rf_setup |= _BV(3);
            break;
        case _250kbps:
            rf_setup |= _BV(5);
            break;
    }
    nrf24_write(RF_SETUP, rf_setup);
}