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
	|--FileName    : bsp_can.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-18               
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
#ifndef __BSP_CAN_H 
#define __BSP_CAN_H 
#include "baseclass.h"
typedef struct canDataStrcut
{
	CAN_TxHeaderTypeDef txMsg;
	CAN_RxHeaderTypeDef rxMsg;
	CAN_FilterTypeDef filter;
	uint8_t txdata[8];
	uint8_t rxdata[8];
	xQueueHandle can_queue; //队列句柄
	char queue_data[16];//队列数据空间
} canDataStrcut;
HAL_StatusTypeDef UserCanConfig(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanFilterInit(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanTxInit(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanRxInit(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef AllocateCanxSpace(CAN_HandleTypeDef *hcanx);
canDataStrcut* GetCantAddr(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanQueueCreate(canDataStrcut *canx,uint8_t len,uint8_t deep);
#endif // __BSP_CAN_H

/*------------------------------------file of end-------------------------------*/


