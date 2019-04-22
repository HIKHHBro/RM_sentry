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

/* -------------- 模块标志位宏定义 ----------------- */
#define   CHASSIS_RC_MODE_READ                 (0x80000000U)//遥控模式 
 #define  CHASSIS_ELUDE_MODE_READ              (0x40000000U) //躲避模式     
 #define  CHASSIS_PC_SHOOT_MODE_READ           (0x20000000U) //自瞄打击模式准备就绪
 #define  CHASSIS_CRUISE_MODE_READ             (0x10000000U)//巡航模式模式准备就绪
 #define  DISABLE_CHA_MOD_READ			           (0xF0F00000U)//失能就绪标志位
 #define  CHASSIS_RC_MODE_RUNING               (0x08000000U)//遥控模式执行 
 #define  CHASSIS_ELUDE_MODE_RUNING            (0x04000000U) //躲避模式 
 #define  CHASSIS_PC_SHOOT_MODE_RUNING         (0x02000000U) //自瞄打击模式执行   
 #define  CHASSIS_CRUISE_MODE_RUNING           (0x01000000U) //巡航模式执行
 #define   DISABLE_CHA_MOD_RUNNING			       (0x0F0F0000U)//失能就绪标志位
 #define   DISABLE_CHASSIS                     (0xFFFF0000U)//失能底盘
 #define   CHASSIS_JUDGE_READ                  (0xF0F00000U)//判断决策

 #define CHASSIS_OUT_OF_CONTROL_READ           (0x00800000U)//云台失控模式就绪 
 #define CHASSIS_OUT_OF_CONTROL_RUNNING        (0x00080000U)//云台失控模式执行  
///#define   CHASSIS_
// #define  DISABLE_MOD                        (0xF0000000U)//使能运行模块

// #define  RC_MODE_READ               (0x02000000U)//遥控模式准备就绪
// #define  JUDGE_READ                 (0x0F000000U)//判断决策
// #define  DELEC_USER_MODE            (0x00FFFFFFU)//清除用户自定义标志位

/* -------- 设置正常模式标志位 --------- */
#define SET_CHA_RUNING_STATUS(_status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(chassis_t.status,DISABLE_CHA_MOD_RUNNING);	\
					SET_BIT(chassis_t.status,_status_);							\
				}while(0)	
#define SET_CHA_READ_STATUS(__status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(chassis_t.status,DISABLE_CHA_MOD_READ);	\
					SET_BIT(chassis_t.status,__status_);							\
				}while(0)		
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
	#define TURNING_ANGLE             20//拐弯角度差//待测试角度差
 #define  SONIC_QU_SIZE              10
 #define  CHASSIS_MOTOR_DIRE_LEFT    -1//左边是反转
 #define  CHASSIS_MOTOR_DIRE_RIGHT    1//右边是正转
 #define  OUT_OF_CONTROL_MID_FLAG     1000//1000*5ms  经过中间轨道时间最短应该比这个大
 #define  AUTO_CONTROL_OK              (uint8_t)0//自动模式正常
  #define OUT_OF_CONTROL            (uint8_t)(~(AUTO_CONTROL_OK))//自动模式失控
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	incrementalEnconderStruct* pchassisEnconder_t;
	CAN_HandleTypeDef *hcanx;
  powerBufferPoolStruct*ppowerBufferPool_t;
  refereeSystemStruct* p_refereeSystem_t;
	//gy955Struct* pgyroByCan_t;
	 const pcDataStruct* pPc_t;
	struct State
	{
		int16_t  r_dire;
		uint8_t r_area;//当前所处区域
		uint8_t last_area;//上次所处区域//
		uint8_t hurt;//被攻击的情况
    uint8_t  out_of_flag;
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
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc,const pcDataStruct* pPc_t);
	void ChassisParseDate(uint16_t id,uint8_t *data);
	void ChassisControl(void);
//	void ChassisUserCanTx(int16_t w1,int16_t w2);
	HAL_StatusTypeDef ChassisCanTx(int16_t w1,int16_t w2);
const chassisStruct* GetChassisStructAddr(void);
 chassisStruct *RWGetChassisStructAddr(void);
 uint32_t GetChassisStatus(void);
  void SetMotorTarget(int16_t w1,int16_t w2);

	 powerBufferPoolStruct* PowerBufferPoolInit(void);
     RM3508Struct* wheel2Init(void);
       RM3508Struct* wheel1Init(void);
        	void SetSetInitStatus(void);
          uint8_t GetHurtStatus(void);
					void ChassisDeinit(void);
						int16_t GetGyroDire(void);
							uint8_t GetOrgans(void);
								void SetArea(void);//激光和陀螺仪
  void ChassisSensorParse(uint8_t *data);
				//	void SetArea(void);
void SetUltrasonic(void);
int16_t jiujimoshi(void);
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


