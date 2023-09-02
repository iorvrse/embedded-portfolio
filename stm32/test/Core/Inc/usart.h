/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

__STATIC_INLINE void USART_TransmitChar(USART_TypeDef *USARTx, uint8_t c)
{
  LL_USART_TransmitData8(USARTx, c);
  while(!LL_USART_IsActiveFlag_TC(USARTx))
    ;
  LL_USART_ClearFlag_TC(USARTx);
}

__STATIC_INLINE char USART_ReceiveChar(USART_TypeDef *USARTx)
{
  while(!LL_USART_IsActiveFlag_RXNE(USARTx));
  return USARTx->DR;
}

void USART_TransmitString(USART_TypeDef *USARTx, const uint8_t *str, uint16_t size);

void USART_TransmitUint8(USART_TypeDef *USARTx, uint8_t *str);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

