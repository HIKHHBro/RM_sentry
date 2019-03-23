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
 #define YAW_LIMIMT_CUT                 (5000)//电流限幅
  #define PITCH_LIMIMT_CUT                 (29000)//电流限幅
  #define LINT_LIMINT                    (5000)
typedef struct gimbalStruct
{
  uint32_t status;
	M2006Struct *prammer_t;
  RM6623Struct *pYaw_t;
  GM6020Struct *pPitch_t;
	const dbusStruct* pRc_t;
  const pcDataStruct* pPc_t;
  int16_t yaw_scan_target;
  int16_t pitch_scan_target;
} gimbalStruct;
	void GimbalStructInit(const dbusStruct* pRc_t,const pcDataStruct* pPc_t);
void GimbalParseDate(uint32_t id,uint8_t *data);
HAL_StatusTypeDef GimbalCanTx(int16_t yaw,int16_t pitch,int16_t rammer);
void GimbalAutoControl(void);
 HAL_StatusTypeDef RxPCMsg(void);
 uint32_t GetGimbalStatus(void);
gimbalStruct *RWGetgimbalStructAddr(void);
	 void SetGimBalInitStatus(void);
  int16_t RammerPidControl(int16_t rammer);
RM6623Struct* YawInit(void);
  int16_t PitchPidControl(int16_t pitch);
  int16_t YawPidControl(int16_t yaw);
		GM6020Struct* PitchInit(void);
void ScanningToExplore(void);
void ControlSwitch(uint32_t commot);
void PcControlMode(void);
void GimbalRcControlMode(void);
  uint32_t ControlDecision(void);//待测试代码

#define GIMBAL_CAL_ERROR(target,real) (CalculateError((target),(real),5500,(8192)))
//#define YAW_CAL_ERROR(target,real) (CalculateError((target),(real),15000,(20480)))
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






