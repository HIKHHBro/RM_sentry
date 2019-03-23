/**
	|-------------------------------- Copyright |--------------------------------|
	|                                                                            |
	|                        (C) Copyright 2018,海康平头哥,                       |
	|         1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China        |
	|                         All Rights Reserved                                |
	|                                                                            |
	|          By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                   https://github.com/GCUWildwolfteam                       |
	|----------------------------------------------------------------------------|
	|--FileName    : baseclass.h
	|--Version     : v1.0
	|--Author      : 海康平头哥
	|--Date        : 2018-11-27 
	|-- Libsupports:           
	|--Description :
	|--FunctionList
	|-------1. ....
	|          <version>:
	|     <modify staff>:
	|             <data>:
	|      <description>:
	|-------2. ...
	|-----------------------------declaration of end-----------------------------|
 **/
#ifndef __BASECLASS_H 
#define __BASECLASS_H 
#include "userdriverconfig.h"
/* -------------- 调试版本和发行版本选择 ----------------- */
/*发行版请把这行宏定义注释掉*/
#define DEBUG_BY_KEIL

	#define MAX(X,Y) ((X)>(Y)?(Y):(X))  //限做大值
	#define MIN(X,Y) ((X)<(Y)?(Y):(X))  //限做小值
	#define ABS(X)  ( (X)<0?(-X):(X))    //取绝对值

/* ----------------- 标志位变量外部链接 -------------------- */
	extern unsigned int e_periphera_interface_flag; 	//外设接口使用状态变量定义
	extern unsigned int e_periphera_error_flag;		//外设接口错误状态变量定义
	extern unsigned int e_objects_flag;     //对象使用标志
/* ----------------- 运行状态位 宏 -------------------- */

/* uint32_t status(0代表最低地位，31代表最高位)（为了方便添加其他标志位）
 * -------------------------------------------------
 *任务数 |   0bit   |   1bit    |   2bit   |    3bit    | 7~3bit |
 *据初始--------------------------------------------
 *化位   | 初始化成功| 正常运行 | 接受正常 |启动设置成功| 保留  |
 * ----------------------------------------------------------------
 * 外设| 8bit | 9bit | 10bit  | 11bit  | 12bit  | 13bit | 14~15bit | 
 * 初始-------------------------------------------------------------
 * 化位| can1 | CAN2 | UASRT1 | UASRT3 | UASRT6 | UART7 |   保留    | 
 * -----------------------------------------------------------------
 * |        24~31bit      | 
 * -------------------------
 * |  保留(模块自定义位)  | 
 * -------------------------
 */
	#define MOD_READ                    0x00000000U//模块准备就绪
	#define INIT_OK                     0x00000001U//初始化成功
  #define RUNING_OK                   0x00000002U//正常运行
  #define RX_OK                       0x00000004U//接受正常
  #define START_OK                    0x00000008U//启动设置成功
  #define CAN1_INIT_OK                0x00000100U//can1正常
  #define CAN2_INIT_OK	              0x00000200U//can2正常
  #define UASRT1_INIT_OK	            0x00000400U//UASRT1正常
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
	/* --这两个宏一定要一起用，缓存之后一定要释放 -- */
	#define CACHE_ADDR(CACHES,ADDRS) 	(CACHES = ADDRS) //缓存地址
	#define FREE_ADDR(CACHES) 	(CACHES = NULL)		//释放地址  
	/* -------------- 宏 ----------------- */
	#define	CYCLE_NUMERICAL(data,max) ((data+1)%max)
/* -------------- 函数定义 ----------------- */
	UART_HandleTypeDef* RecognizeUSARTType(UART_HandleTypeDef* huartx);
  CAN_HandleTypeDef* RecognizeCanType(CAN_HandleTypeDef* hcanx);
	void SetFrameDropflag(int16_t flag);
	void SetNormalflag(int16_t flag);
	HAL_StatusTypeDef RCREncryption(uint8_t *pdata, uint8_t size);
	HAL_StatusTypeDef RCRDecryption(uint8_t *pdata, uint8_t size);
	void MultibyteToByle(uint32_t data,uint8_t *pdata);
	void ByleToMultibyte(uint8_t *pdata,uint32_t *data);
	void TwobyteToByle(int16_t data,uint8_t *pdata);
	void DataFilling(uint8_t *pdata,uint8_t s,uint8_t size);
#endif	// __BASECLASS_H
	
 /*--------------------------------file of end--------------------------------*/

