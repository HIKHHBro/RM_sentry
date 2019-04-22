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
#include "DataStructure.h" 
#include "gyro.h" 
/* -------------- 模块自定义标志位宏 ----------------- */
#define  RC_MODE_READ               (0x80000000U) //遥控模式准备就绪
#define  GIMBAL_PC_SHOOT_MODE_READ  (0x40000000U) //自瞄打击模式准备就绪
#define  FRAME_DROP_BUFFER_READ     (0x20000000U) //掉帧缓冲准备就绪
#define  SCAN_MODE_READ             (0x10000000U) //扫描模式准备就绪
#define  DISABLE_MOD_READ           (0xF0000000U)//使能运行模块
#define  JUDGE_READ                 (0xF0000000U) //判断决策

#define  RC_MODE_RUNING             (0x08000000U)//遥控模式执行
#define  PC_SHOOT_MODE_RUNING       (0x04000000U)//自瞄打击模式执行
#define  FRAME_DROP_BUFFER_RUNING   (0x02000000U)//掉帧缓冲执行
#define  SCAN_MODE_RUNING           (0x01000000U)//扫描模式执行
#define  GIMBAL_MOD_RUNNING         (0x0F000000U)//运行模块位

#define  DISABLE_GIMBAL             (0xFF000000U)//失能云台
#define  DELEC_USER_MODE            (0x00000000U)//清除用户自定义标志位
#define SET_RUNING_STATUS(_status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,GIMBAL_MOD_RUNNING);	\
					SET_BIT(gimbal_t.status,_status_);							\
				}while(0)	
#define SET_READ_STATUS(__status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,DISABLE_MOD_READ);	\
					SET_BIT(gimbal_t.status,__status_);							\
				}while(0)																					
/* -------------- 私有宏 ----------------- */
	#define QUEUE_LEN      5U//深度为5
	#define  QUEUE_SIZE    8U//长度为5;
  #define  MEMORY_SPAN   (50)          //   1000ms = 50*20
  #define  QUEUE_YAW_STATUS_SIZE      20//则记忆长度为1s 精度为50
 #define   QUEUE_PITCH_STATUS_SIZE    20//长度10，则记忆长度为1s
/* -------------- 宏定义 ----------------- */
#define GIMBAL_CAN_ID_L 0x204
#define GIMBAL_CAN_ID_H 0x208
	#define YAW_RX_ID 								0x205//YAW轴电机接收 id
	#define PITCH_RX_ID 							0x206//PITCH轴电机接收 id
	#define GIMBAL_CAN_TX_ID 					0x1ff//云台电机发送id
 #define YAW_LIMIMT_CUT                 (6000)//电流限幅
  #define PITCH_LIMIMT_CUT                 (29000)//电流限幅
  #define LINT_LIMINT                    (6000)
	#define PASS_STATUS_LEN                 10//历史状态记忆长度
  #define DOWN_BUFF_POSI                     6000//仰角位置限制值
  #define UP_BUFF_POSI                     7000//俯角位置限制值
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
	gy955Struct *pGyro_t;
	// struct frameDropBuffer_t
	// {
	// 	int16_t yaw_pass_position[PASS_STATUS_LEN];
	// 	int16_t pitch_pass_position[PASS_STATUS_LEN];
	// }frameDropBuffer_t;
	
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
void SetGeneralMode(void);
void SetPcControlPID(void);
void GimbalDeinit(void);
		void SetFrameDropBufferStatus(void);
	void GetTrend(int16_t* yaw_tr,int16_t* pitch_tr);
		void FrameDropBufferMode(void);
int16_t SetLock(int16_t r,int16_t t);
 void GimbalWeakDeinit(void);

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





	int16_t PostionOutToSpeedOut(int16_t pid_out);															
  //void GyroParse(uint32_t id,uint8_t *data);
#endif // __GIMBAL_H
/*-----------------------------------file of end------------------------------*/






