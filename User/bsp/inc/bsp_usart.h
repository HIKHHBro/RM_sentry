/**
	|------------------------------ Copyright -----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,海康平头哥,                       |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : bsp_usart.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-15               
	|--Libsupports : 
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#ifndef __BSP_USART_H 
#define __BSP_USART_H 
#include "baseclass.h "
#include "fps.h" 
typedef struct usartDataStrcut
{
	uint8_t a_frame_len;//一帧的长度
	uint8_t rx_buff_size;//接收校验长度
	uint8_t *rx_buff_data; //接收缓存空间地址
	uint8_t rx_on_off;   //数据接收开关
  uint16_t datalen;//一帧数据长度
	xQueueHandle usart_queue; //队列句柄
}usartDataStrcut;
HAL_StatusTypeDef UsartAndDMAInit(UART_HandleTypeDef *huartx,\
																	uint8_t frame_len,\
																	uint8_t on_off);
void UserUsartCallback(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef NoLenRXforUsart(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef UsartQueueCreate(usartDataStrcut *usartx, uint8_t len,
																	 uint8_t deep);
HAL_StatusTypeDef AllocateUsartxSpace(UART_HandleTypeDef *huartx);
usartDataStrcut *GetUsartAddr(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef UserUsartQueueRX(UART_HandleTypeDef *huartx, \
																	 void *const pvBuffer);

void RxCcomuu(UART_HandleTypeDef *huartx);
#define A_FRAME_CHECK_LEN 3//一帧接收缓存长度
#define DATA_LEN_BYTE_LEN 2//数据长度
#define DATA_LEN_BYTE_HIGH_8 0//数据长度高八位
#define DATA_LEN_BYTE_LOW_8  1//数据长度低八位
#define HEAD_FRAME_LEN (A_FRAME_CHECK_LEN + DATA_LEN_BYTE_LEN)
#define RX_HEAD_ADDR   2//接收数据的首地址

#endif // __BSP_USART_H

/*----------------------------------file of end-------------------------------*/
