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
#include "rammer.h" 
/* -------------- 宏定义 ----------------- */
#define GIMBAL_CAN_ID_L 0x204
#define GIMBAL_CAN_ID_H 0x208
	#define YAW_RX_ID 								0x205//YAW轴电机接收 id
	#define PITCH_RX_ID 							0x206//PITCH轴电机接收 id
	#define GIMBAL_CAN_TX_ID 					0x1ff//云台电机发送id
	#define FRICTIONGEAR_SPEED 				(17)	//摩擦轮速度
typedef struct gimbalStruct
{
  uint32_t status;
	M2006Struct *prammer_t;
  RM6623Struct *pYaw_t;
    RM6623Struct *pPitch_t;
	const dbusStruct* pRc_t;
} gimbalStruct;
	void GimbalStructInit(const dbusStruct* pRc_t);
void GimbalParseDate(uint32_t id,uint8_t *data);
HAL_StatusTypeDef GimbalCanTx(int16_t yaw,int16_t pitch,int16_t rammer);
void GimbalControl(void);
 HAL_StatusTypeDef RxPCMsg(void);
 uint32_t GetGimbalStatus(void);
gimbalStruct *RWGetgimbalStructAddr(void);
	 void SetGimBalInitStatus(void);
  int16_t RammerPidControl(void);

#define GIMBAL_CAL_ERROR(target,real) (CalculateError((target),(real),6000,(8192)))
#define GET_RAMMER_ANGLE(_A_,_LAST_,_COE_)     (RatiometricConversion(\
                                    _A_,\
                                    M2006_THRESHOLD,\
                                    RAMMER_REDUCTION,\
                                    _LAST_,\
                                    _COE_\
                                  )\
                              )
#endif // __GIMBAL_H
/*-----------------------------------file of end------------------------------*/






