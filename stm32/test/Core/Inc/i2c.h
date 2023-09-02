/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */

__STATIC_INLINE void I2C_TransmitByte(I2C_TypeDef *I2Cx, char Data)
{
  LL_I2C_TransmitData8(I2Cx, Data);
  while (!LL_I2C_IsActiveFlag_TXE(I2Cx))
    ;
  while (!LL_I2C_IsActiveFlag_BTF(I2Cx))
    ;
}

__STATIC_INLINE void I2C_Start(I2C_TypeDef *I2Cx)
{
  LL_I2C_GenerateStartCondition(I2Cx);
  while (!LL_I2C_IsActiveFlag_SB(I2Cx))
    ;
  __IO uint32_t tmpreg;
  tmpreg = I2Cx->SR1;
  (void) tmpreg;
}

__STATIC_INLINE void I2C_SendAddress(I2C_TypeDef *I2Cx, char DevAddress)
{
  LL_I2C_TransmitData8(I2Cx, DevAddress);
  while (!LL_I2C_IsActiveFlag_ADDR(I2Cx))
    ;
  LL_I2C_ClearFlag_ADDR(I2Cx);
}

__STATIC_INLINE char I2C_ReceiveByte(I2C_TypeDef *I2Cx, uint32_t Ack)
{
  LL_I2C_AcknowledgeNextData(I2Cx, Ack);
  while (!LL_I2C_IsActiveFlag_RXNE(I2Cx))
    ;
  return LL_I2C_ReceiveData8(I2Cx);
}

__STATIC_INLINE void I2C_Stop(I2C_TypeDef *I2Cx)
{
  LL_I2C_GenerateStopCondition(I2Cx);
  while(LL_I2C_IsActiveFlag_BUSY(I2Cx))
    ;
}

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

