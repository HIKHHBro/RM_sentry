/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,海康平头哥,                       |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
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
#include "communicate.h"
#include "gyro.h" 
#include "pc_data.h" 
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	incrementalEnconderStruct* pchassisEnconder_t;
	CAN_HandleTypeDef *hcanx;
  powerBufferPoolStruct*ppowerBufferPool_t;
  refereeSystemStruct* p_refereeSystem_t;
	gy955Struct* pgyroByCan_t;
	 const pcDataStruct* pPc_t;
	struct State
	{
		int16_t  r_dire;
		uint8_t r_area;//当前所处区域
		uint8_t last_area;//上次所处区域//
		uint8_t hurt;//被攻击的情况
	}State;//解析机器人所有状态
	struct Sensor
  {
    uint8_t  lim_sw_left;
    uint8_t  lim_sw_right;
    uint8_t  laser_sw_left;
    uint8_t  laser_sw_right;
    uint8_t  sonic_left;
    uint8_t  sonic_right;
    int32_t  encoder;
  }Sensor;
  uint32_t status;
int16_t left_dis_son;
  int16_t right_dis_son;
}chassisStruct;
/* -------------- 宏 ----------------- */
#define CAL_ERROR(target,real) (CalculatePationError((target),(real)))

	#define WHEEL1_RX_ID      			 0x201
	#define WHEEL2_RX_ID       			 0x202
	#define CURRENT_METER_RX_ID      0x401//电流计接收id
  #define CHASSIS_SENSOR_RX_ID     0x402//底盘传感器数据接收id
	#define CHASSIS_CAN_TX_ID  			 0x200
	#define W1_LIMIT_SPEED    			 8000  //轮子1速度限幅
	#define W2_LIMIT_SPEED    			 8000  //轮子2速度限幅
	#define EN_RADIUS            			 30    //编码器轮子半径单位 mm
	#define ENCONDER_POLES    			 500 
  #define  AHEAD_OF_ARMOR          0//前装甲0
	#define  BACK_OF_ARMOR           1 //前装甲1
	#define UP_ROAD                 0//上路靠启动区
	#define MID_ROAD                 1//直道中路
	#define DOWN_ROAD                 2//下路靠桥
	#define TURNING_ANGLE             20//拐弯角度差
 #define  SONIC_QU_SIZE              10
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc,const pcDataStruct* pPc_t);
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
          uint8_t GetHurtStatus(void);
				void ChassisCruiseModeInit(void);
        void ChassisEludeControlMode(void);
				void ChassisControlSwitch(uint32_t commot);
				  uint32_t ChassisControlDecision(void);
					void ChassisDeinit(void);
						int16_t GetGyroDire(void);
							uint8_t GetOrgans(void);
								void SetArea(void);//激光和陀螺仪
									int16_t Go(int16_t target,int16_t speed);
										void ChassisPcShootModeInit(void);
											void ChassisEludeControlModeInit(void);
                      	void ChassisPcShootMode(void);
  void ChassisSensorParse(uint8_t *data);
				//	void SetArea(void);
void SetUltrasonic(void);
int16_t jiujimoshi(void);
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


