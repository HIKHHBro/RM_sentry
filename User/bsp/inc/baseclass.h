/**
	|-------------------------------- Copyright ----------------------------------------|
	|                                                                                   |
	|                        (C) Copyright 2018,海康平头哥,                              |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China            |
	|                            All Rights Reserved                                    |
	|                                                                                   |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)               |
	|                     https://github.com/GCUWildwolfteam                            |
	|-----------------------------------------------------------------------------------|
	|--FileName    : baseclass.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-11-27 
	|-- Libsupports: STM32CubeF1 Firmware Package V1.6.0 / 17-May-2017(用别的库出问题别问我)              
	|--Description : 1、支持板子f103vetx                                                     
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end---------------------------------|
 **/
#ifndef __BASECLASS_H 
#define __BASECLASS_H 
/* -------- stm32 库选择 1是开启，0是开闭--------- */
#define HAL_F1 0
#define HAL_F4 1
#if HAL_F1 
	// #include "stm32f1xx_hal.h"
	// #include <stdlib.h>
#elif HAL_F4
		#include "stm32f4xx_hal.h"
		#include "cmsis_os.h"
		#include <stdlib.h>
    #include <string.h>
#endif 
	#define MAX(X,Y) (X)>(Y)?(Y):(X)  //限做大值
	#define MIN(X,Y) (X)<(Y)?(Y):(X)  //限做小值
	#define ABS(X) (X)<0?(-X):(X)    //取绝对值

/* ----------------- 标志位变量外部链接 -------------------- */
	extern unsigned int e_periphera_interface_flag; 	//外设接口使用状态变量定义
	extern unsigned int e_periphera_error_flag;		//外设接口错误状态变量定义
	extern unsigned int e_objects_flag;     //对象使用标志
/* ----------------- 外设使用标志位表  -------------------- */
	#define USART1_BY_USED    						0x0001U//串口1被使用
	#define USART2_BY_USED    						0x0002U//串口2被使用
	#define USART3_BY_USED    						0x0004U//串口3被使用
	#define UART4_BY_USED    							0x0008U//串口4被使用
	#define UART5_BY_USED    							0x0010U//串口5被使用
	#define UART6_BY_USED    							0x0020U//串口6被使用
	#define USART1_DMA_BY_USED    						0x0040U//串口1被使用
	#define USART2_DMA_BY_USED    						0x0080U//串口2被使用
	#define USART3_DMA_BY_USED    						0x0100U//串口3被使用
	#define UART4_DMA_BY_USED    							0x0200U//串口4被使用
	#define UART5_DMA_BY_USED    							0x0400U//串口5被使用
	#define UART6_DMA_BY_USED    							0x0800U//串口6被使用
	#define CAN1_BY_USED      						0x01000U//can1被使用
	#define CAN2_BY_USED      						0x02000U//can2被使用
	#define WHOLEPOSITOM_CLEAN							0x04000U//全场定位清理成功
/* ----------------- 外设接口错误状态变量定义 -------------------- */
	#define NO_ERROR   														 0x0000U
	#define FRAME_DROP  													 0x0001U
	#define RX_ERROR    													0x0002U
	#define GET_USART_ADDR_FAIL													0x0004U
	#define GET_CAN_ADDR_FAIL													0x0004U
/* ----------------- 对象使用标志表 -------------------- */
	#define NO_CREATE_OBJECTS_OF_WHOLEPOSITION        0x0000U //申请全场定位的对象
	#define CREATE_OBJECTS_OF_WHOLEPOSITION        0x0001U //申请全场定位的对象
	#define CREATE_OBJECTS_OF_CHASSIS 						0x0002U //申请底盘的对象
	// #define (x,y)
	/* --这两个宏一定要一起用，缓存之后一定要释放 -- */
	#define CACHE_ADDR(CACHES,ADDRS) 	(CACHES = ADDRS) //缓存地址
	#define FREE_ADDR(CACHES) 	(CACHES = NULL)		//释放地址  	
	UART_HandleTypeDef* RecognizeUSARTType(UART_HandleTypeDef* huartx);//识别串口类型
  CAN_HandleTypeDef* RecognizeCanType(CAN_HandleTypeDef* hcanx);//识别can类型
	void SetFrameDropflag(int16_t flag);
	void SetNormalflag(int16_t flag);
	HAL_StatusTypeDef RCREncryption(uint8_t *pdata, uint8_t size);
	HAL_StatusTypeDef RCRDecryption(uint8_t *pdata, uint8_t size);

#endif	// __BASECLASS_H
	
 /*------------------------------------file of end------------------------------------*/


