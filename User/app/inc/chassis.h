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
#include "input_sw.h" 
#include "sentry_state_config.h"
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
#define  IS_CHASSIS_STATUS(_status_)                (GET_BYTE(chassis_t.status,_status_) == _status_)
/* -------------- 宏 ----------------- */
#define CAL_ERROR(target,real) (CalculatePationError((target),(real)))

	#define WHEEL1_RX_ID      			 0x201
	#define WHEEL2_RX_ID       			 0x202
	#define CURRENT_METER_RX_ID      0x401//电流计接收id
  #define CHASSIS_SENSOR_RX_ID     0x402//底盘传感器数据接收id
  #define  CHASSIS_RX_ID   0x413
	#define CHASSIS_CAN_TX_ID  			 0x200
	#define W1_LIMIT_SPEED    			 8000  //轮子1速度限幅
	#define W2_LIMIT_SPEED    			 8000  //轮子2速度限幅
	#define EN_RADIUS            			 25    //编码器轮子半径单位 mm
	#define ENCONDER_POLES    			 3600 
  #define  DIRE_SU                      200 //底盘位置角度改变的过零  360-底盘位置角度变化最大角度
/* ----轨道参数，上场一定要改好---- */
//  #define TRACK_TOLAT            1650//轨道总长度//正式比赛轨道长度为5436  //最好上场测量一下，第一场允许校准//写整体轨道校准模式
//	#define UP_ROAD_LEN               610//临时   611//上路靠启动区
//  #define HALF_ORBITAL                 825//临时                         //1400//实验室轨道//比赛轨道                                               
//	#define MID_ROAD_LEN               1000//临时  2017//直道中路
//	#define DOWN_ROAD_LEN               1650 //临时 2806//下路靠桥
//	#define TURNING_ANGLE             55//拐弯角度差//待测试角度差
  #define  OUT_OF_CONTROL_MID_FLAG     100//100*5ms  经过中间轨道时间最短应该比这个大
// #define  TRACK_END             2806      //轨道中点
// #define TRACK_START            200 //轨道起点
// #define ORBITAL_ANGLE       60//轨道角度 
// #define HALF_ORBITAL_ANGLE   30//角度一半
 #define MOV_RAN          300//击打模式走动范围
 #define  CHASSIS_MOTOR_DIRE_LEFT    -1//左边是反转
 #define  CHASSIS_MOTOR_DIRE_RIGHT    1//右边是正转 
 
 #define  SONIC_QU_SIZE              10
 
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	incrementalEnconderStruct* pchassisEnconder_t;
	CAN_HandleTypeDef *hcanx;
  powerBufferPoolStruct*ppowerBufferPool_t;
  refereeSystemStruct* p_refereeSystem_t;
	gyroStruct* pGyro_t;
	 const pcDataStruct* pPc_t;
	int16_t  r_dire;
  uint32_t gimbal_state;//云台命令
	// uint32_t state;//解析机器人所有状态
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
  struct track
  {
    int16_t total_len;
    int16_t up_turn_len;
    int16_t down_turn_len;
    int16_t mid_road_len;
    int16_t up_road_len;
    int16_t dowm_road_len;
    int16_t turning_len;
    int16_t turning_angle;
    int16_t angle;
    int16_t end;//终点死区
    int16_t start;//起点死去
    int16_t half_angle;
    int16_t half_len;
    
  }track;
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
  void ChassisDeinit(void);
  int16_t GetGyroDire(void);
  uint8_t GetOrgans(void);
  void SetArea(void);//激光和陀螺仪
  void ChassisSensorParse(void);
  //	void SetArea(void);
  void SetUltrasonic(void);
  int16_t jiujimoshi(void);
  int16_t GoToPiont(int16_t piont,int16_t *pid_out);
  void EncodeDetector(void);
  //----------临时增加
  int16_t Toback(int16_t speed);
  uint32_t SetAByEncoder(void);
//  void GetState(void);
  void GetHurted(void);
  MOD_Status AutoCalibrationData(void);
void GetGimblaCommot(uint32_t commot);
void TrackparseInit(int16_t tatol_len,int16_t angle);
void TrackparseInit(int16_t tatol_len,int16_t angle);
//-----------------
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


