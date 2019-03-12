/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,海康平头哥,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : gimbal.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-26               
	|--Libsupports : 
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
#ifndef __GIMBAL_H 
#define __GIMBAL_H 
#include "motor.h"
#include "DJI_dbus.h" 
#define GIMBAL_CAN_ID_L 0x204
#define GIMBAL_CAN_ID_H 0x208
typedef struct gimbalStruct
{
	M2006Struct *prammer_t;
  RM6623Struct *pYaw_t;
    RM6623Struct *pPitch_t;
	CAN_HandleTypeDef *hcanx;
  UART_HandleTypeDef *huartx;
} gimbalStruct;
void GimbalStructInit(CAN_HandleTypeDef *hcanx);
void GimbalParseDate(uint32_t id,uint8_t *data);
HAL_StatusTypeDef GimbalCanTx(int16_t yaw,int16_t pitch,int16_t rammer);
void GimbalControl(const dbusStruct* dbus);
 HAL_StatusTypeDef RxPCMsg(void);
#endif // __GIMBAL_H
/*-----------------------------------file of end------------------------------*/






