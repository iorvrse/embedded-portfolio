#ifndef _TWI_H_
#define _TWI_H_

#ifndef F_CPU
#define F_CPU   8000000UL
#endif /* F_CPU */

#include <avr/io.h>
#include <util/twi.h>

#define ACK		0
#define NACK	1

void twi_master_init(uint32_t scl_freq);
void twi_slave_init(uint8_t addr);
void twi_start(uint8_t addr);
void twi_write(uint8_t data);
uint8_t twi_read(uint8_t ack);
void twi_stop(void);
uint8_t twi_listen(void);

#endif