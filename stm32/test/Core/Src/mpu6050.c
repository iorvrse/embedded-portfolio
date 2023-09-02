#include "mpu6050.h"

void mpu6050_write(uint8_t reg, uint8_t data)
{
  I2C_Start(MPU6050_I2C);
  I2C_SendAddress(MPU6050_I2C, MPU6050_ADDRESS);
  I2C_TransmitByte(MPU6050_I2C, reg);
  I2C_TransmitByte(MPU6050_I2C, data);
  I2C_Stop(MPU6050_I2C);
}

void mpu6050_init(void)
{
	mpu6050_write(MPU6050_PWR_MGMT_1, 0x00);
	mpu6050_write(MPU6050_SMPLRT_DIV, 0x07);
	mpu6050_write(MPU6050_CONFIG, 0x00);
	mpu6050_write(MPU6050_GYRO_CONFIG, 0x00);
	mpu6050_write(MPU6050_ACCEL_CONFIG, 0x00);
}

void mpu6050_read_raw(mpu6050 *mpu)
{
	I2C_Start(MPU6050_I2C);
	I2C_SendAddress(MPU6050_I2C, MPU6050_ADDRESS);
	I2C_TransmitByte(MPU6050_I2C, MPU6050_ACCEL_XOUT_H);
	
	I2C_Start(MPU6050_I2C);
	I2C_SendAddress(MPU6050_I2C, MPU6050_ADDRESS | 0x01);
	mpu->accel_x = (int16_t)(I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK) << 8 | I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK));
	mpu->accel_y = (int16_t)(I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK) << 8 | I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK));
	mpu->accel_z = (int16_t)(I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK) << 8 | I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK));
	mpu->temperature = (int16_t)(I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK) << 8 | I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK));
	mpu->gyro_x = (int16_t)(I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK) << 8 | I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK));
	mpu->gyro_y = (int16_t)(I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK) << 8 | I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK));
	mpu->gyro_z = (int16_t)(I2C_ReceiveByte(MPU6050_I2C, LL_I2C_ACK) << 8 | I2C_ReceiveByte(MPU6050_I2C, LL_I2C_NACK));
	I2C_Stop(MPU6050_I2C);
}

void mpu6050_calc_raw(mpu6050* mpu)
{
	mpu->accel_x = mpu->accel_x / 16384.0;
	mpu->accel_y = mpu->accel_y / 16384.0;
	mpu->accel_z = mpu->accel_z / 16384.0;
	mpu->temperature = mpu->temperature / 340.0 + 36.53;
	mpu->gyro_x = mpu->gyro_x / 131.0;
	mpu->gyro_y = mpu->gyro_y / 131.0;
	mpu->gyro_z = mpu->gyro_z / 131.0;
}

void mpu6050_accel_angle(mpu6050* mpu)
{
	mpu->roll = atan2(mpu->accel_y, mpu->accel_z) * 180 / M_PI;
	mpu->pitch = atan2(-mpu->accel_x, sqrt((mpu->accel_y * mpu->accel_y) + (mpu->accel_z * mpu->accel_z))) * 180 / M_PI;
}