#include "mpu6050.h"
#include "twi.h"
#include <math.h>

void mpu6050_write(uint8_t addr, uint8_t reg, uint8_t data)
{
	twi_start(addr);
	twi_write(reg);
	twi_write(data);
	twi_stop();
}

void mpu6050_init(void)
{
	mpu6050_write(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);
	mpu6050_write(MPU6050_ADDRESS, SMPLRT_DIV, 0x07);
	mpu6050_write(MPU6050_ADDRESS, CONFIG, 0x00);
	mpu6050_write(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);
	mpu6050_write(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00);
}

void mpu6050_read_raw(mpu6050 *mpu)
{
	twi_start(MPU6050_ADDRESS);
	twi_write(ACCEL_XOUT_H);

	twi_start(MPU6050_ADDRESS | 0x01);
	mpu->accel_x = twi_read(ACK) << 8 | twi_read(ACK);
	mpu->accel_y = twi_read(ACK) << 8 | twi_read(ACK);
	mpu->accel_z = twi_read(ACK) << 8 | twi_read(ACK);
	mpu->temperature = twi_read(ACK) << 8 | twi_read(ACK);
	mpu->gyro_x = twi_read(ACK) << 8 | twi_read(ACK);
	mpu->gyro_y = twi_read(ACK) << 8 | twi_read(ACK);
	mpu->gyro_z = twi_read(ACK) << 8 | twi_read(NACK);
	twi_stop();
}

void mpu6050_calc_raw(mpu6050* mpu)
{
	mpu->accel_x = mpu->accel_x / 16384.0;
	mpu->accel_y = mpu->accel_y / 16384.0;
	mpu->accel_z = mpu->accel_z / 16384.0;
	mpu->temperature = mpu->temperature/ 340.0 + 36.53;
	mpu->gyro_x = mpu->gyro_x / 131.0;
	mpu->gyro_y = mpu->gyro_y / 131.0;
	mpu->gyro_z = mpu->gyro_z / 131.0;
}

void mpu6050_accel_angle(mpu6050* mpu)
{
	mpu->roll = atan2(mpu->accel_y, mpu->accel_z) * 180 / M_PI;
	mpu->pitch = atan2(-mpu->accel_x, sqrt((mpu->accel_y * mpu->accel_y) + (mpu->accel_z * mpu->accel_z))) * 180 / M_PI;
}