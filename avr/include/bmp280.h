#ifndef _BMP280_H_
#define _BMP280_H_

#define SDO 0

#if SDO
#define BMP280_ADDRESS  0xEE    /* 0x77 << 1 */
#else
#define BMP280_ADDRESS  0xEC    /* 0x76 << 1 */
#endif

#define BMP280_ID           0xD0
#define BMP280_RESET        0xE0
#define BMP280_STATUS       0xF3
#define BMP280_CTRL_MEAS    0xF4
#define BMP280_CONFIG       0xF5
#define PRESS_MSB           0xF7
#define PRESS_LSB           0xF8
#define PRESS_XLSB          0xF9
#define TEMP_MSB            0xFA
#define TEMP_LSB            0xFB
#define TEMP_XLSB           0xFC

#endif  /* _BMP280_H_ */