/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,����ƽͷ��,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : gimbal.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
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
/* -------------- �궨�� ----------------- */
#define GIMBAL_CAN_ID_L 0x204
#define GIMBAL_CAN_ID_H 0x208
	#define YAW_RX_ID 								0x205//YAW�������� id
	#define PITCH_RX_ID 							0x206//PITCH�������� id
	#define GIMBAL_CAN_TX_ID 					0x1ff//��̨�������id
	#define FRICTIONGEAR_SPEED 				(17)	//Ħ�����ٶ�
 #define YAW_LIMIMT_CUT                 (5000)//�����޷�
  #define PITCH_LIMIMT_CUT                 (29000)//�����޷�
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
  uint32_t ControlDecision(void);//�����Դ���

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






