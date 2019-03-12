/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,海康平头�?,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                          All Rights Reserved                               |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大�?�广州�?�院机器人野狼队)         |
	|                   https://github.com/GCUWildwolfteam                       |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头�?                                                       
	|--Date        : 2019-01-19               
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
#ifndef __CHASSIS_H 
#define __CHASSIS_H 
#include "motor.h" 
#include "DJI_dbus.h" 
#include "usart_debug.h" 
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	CAN_HandleTypeDef *hcanx;
  uint8_t status;
}chassisStruct;
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc);
	void ChassisParseDate(uint16_t id,uint8_t *data);
	void ChassisCanTx(int16_t w1,int16_t w2);
	void ChassisControl(void);
const chassisStruct* GetChassisStructAddr(void);
 chassisStruct *RWGetChassisStructAddr(void);
 uint8_t GetChassisStatus(void);
   void SetMotorTarget(int16_t w1,int16_t w2);
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


