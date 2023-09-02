#include "twi.h"

void twi_master_init(uint32_t scl_freq)
{
	TWSR = 0;
	TWBR = (F_CPU / scl_freq - 16) / 2;
}

void twi_slave_init(uint8_t addr)
{
	TWAR = addr;
	TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN);
}

void twi_start(uint8_t addr)
{
	TWCR = _BV(TWSTA) | _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
	
	TWDR = (addr << 1);
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
}

void twi_write(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
}

uint8_t twi_read(uint8_t ack)
{
	switch (ack)
	{
	case ACK:
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
		break;

	case NACK:
		TWCR = _BV(TWINT) | _BV(TWEN);
		break;
	}

	loop_until_bit_is_set(TWCR, TWINT);
	return TWDR;
}

void twi_stop(void)
{
	TWCR = _BV(TWSTO) | _BV(TWINT) | _BV(TWEN);
}

uint8_t twi_listen(void)
{
	loop_until_bit_is_set(TWCR, TWINT);
	uint8_t status = TW_STATUS;

	if (status == TW_ST_SLA_ACK || status == TW_ST_ARB_LOST_SLA_ACK)
	{
		return TW_READ;
	}
	else if (status == TW_SR_SLA_ACK || status == TW_SR_ARB_LOST_SLA_ACK)
	{
		return TW_WRITE;
	}
	else if (status == TW_SR_GCALL_ACK || status == TW_SR_ARB_LOST_GCALL_ACK)
	{
		return 2;
	}

	return 0;
}