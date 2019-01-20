/**
	|-------------------------------- Copyright -----------------------------------|
	|                                                                              |
	|                        (C) Copyright 2019,海康平头哥,                         |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                               |
	|                                                                              |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                     https://github.com/GCUWildwolfteam                       |
	|------------------------------------------------------------------------------|
	|--FileName    : bsp_usart.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-15               
	|--Libsupports : STM32CubeF1 Firmware Package V1.6.0(用别的库出问题别问我)
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end----------------------------|
 **/
#ifndef __BSP_USART_H 
#define __BSP_USART_H 
#include "baseclass.h"
typedef struct usartDataStrcut
{
	uint8_t a_frame_len;//一帧的长度
	uint8_t rx_buff_size;//接收校验长度
	uint8_t *rx_buff_data; //接收缓存空间地址
	uint8_t rx_on_off;   //数据接收开关
	xQueueHandle usart_queue; //队列句柄
}usartDataStrcut;
HAL_StatusTypeDef UsartAndDMAInit(UART_HandleTypeDef *huartx, uint8_t frame_len,\
																															uint8_t on_off);
void UserUsartCallback(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef NoLenRXforUsart(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef UsartQueueCreate(usartDataStrcut *usartx, uint8_t len,
																	 uint8_t deep);
HAL_StatusTypeDef AllocateUsartxSpace(UART_HandleTypeDef *huartx);
usartDataStrcut *GetUsartAddr(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef UserUsartQueueRX(UART_HandleTypeDef *huartx, void *const pvBuffer);
#endif // __BSP_USART_H

/*------------------------------------file of end-------------------------------*/
