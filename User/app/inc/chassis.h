/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,娴峰悍骞冲ご鍝?,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                          All Rights Reserved                               |
	|                                                                            |
	|           By(GCU The wold of team | 鍗庡崡鐞嗗伐澶у?﹀箍宸炲?﹂櫌鏈哄櫒浜洪噹鐙奸槦)         |
	|                   https://github.com/GCUWildwolfteam                       |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 娴峰悍骞冲ご鍝?                                                       
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
#include "enconder.h" 
#include "ultrasonic.h" 
#include "currentmeter.h" 
#include "power_buffer_pool.h"
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	incrementalEnconderStruct* pchassisEnconder_t;
	CAN_HandleTypeDef *hcanx;
  powerBufferPoolStruct*ppowerBufferPool_t;
  uint32_t status;

}chassisStruct;
/* -------------- 宏 ----------------- */
#define CAL_ERROR(target,real) (CalculatePationError((target),(real)))

	#define WHEEL1_RX_ID      			 0x201
	#define WHEEL2_RX_ID       			 0x202
	#define CURRENT_METER_RX_ID      0x401//电流计接收id
	#define CHASSIS_CAN_TX_ID  			 0x200
	#define W1_LIMIT_SPEED    			 6000  //轮子1速度限幅
	#define W2_LIMIT_SPEED    			 6000  //轮子2速度限幅
	#define RADIUS            			 30    //编码器轮子半径单位 mm
	#define ENCONDER_POLES    			 500 
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc);
	void ChassisParseDate(uint16_t id,uint8_t *data);
//	void ChassisUserCanTx(int16_t w1,int16_t w2);
	HAL_StatusTypeDef ChassisCanTx(int16_t w1,int16_t w2);
	void ChassisControl(void);
const chassisStruct* GetChassisStructAddr(void);
 chassisStruct *RWGetChassisStructAddr(void);
 uint32_t GetChassisStatus(void);
  void SetMotorTarget(int16_t w1,int16_t w2);
   void ChassisRcControlMode(void);
	 powerBufferPoolStruct* PowerBufferPoolInit(void);
     RM3508Struct* wheel2Init(void);
       RM3508Struct* wheel1Init(void);
        	void SetSetInitStatus(void);
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


