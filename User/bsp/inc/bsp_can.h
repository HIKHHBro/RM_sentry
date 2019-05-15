/**
	|-------------------------------- Copyright ---------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,海康平头哥,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                          All Rights Reserved                               |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                     https://github.com/GCUWildwolfteam                     |
	|----------------------------------------------------------------------------|
	|--FileName    : bsp_can.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-18               
	|--Libsupports : 
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-------------------------------declaration of end---------------------------|
 **/
#ifndef __BSP_CAN_H 
#define __BSP_CAN_H 
#include "baseclass.h " 
#include "fps.h" 
typedef struct canDataStrcut
{
	CAN_TxHeaderTypeDef txMsg;
	CAN_RxHeaderTypeDef rxMsg;
	CAN_FilterTypeDef filter;
	uint8_t rxdata[8];
} canDataStrcut;
HAL_StatusTypeDef UserCanConfig(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanFilterInit(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanTxInit(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanRxInit(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef AllocateCanxSpace(CAN_HandleTypeDef *hcanx);
canDataStrcut* GetCanAddr(CAN_HandleTypeDef *hcanx);
HAL_StatusTypeDef CanQueueCreate(canDataStrcut *canx,uint8_t len,uint8_t deep);
HAL_StatusTypeDef UserCanQueueRX(CAN_HandleTypeDef *hcanx,\
																		void* const pvBuffer);
HAL_StatusTypeDef CanTxMsg(CAN_HandleTypeDef* hcanx,int id,uint8_t *message);		
__weak void can_rx(uint32_t id,uint8_t *data);
void can2_rx(uint32_t id,uint8_t *data);
#endif // __BSP_CAN_H

/*-----------------------------------file of end------------------------------*/


