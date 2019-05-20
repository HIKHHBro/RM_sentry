/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                      (C) Copyright 2018,海康平头哥,                         |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                         All Rights Reserved                            -   |
	|                                                                            |
	|          By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)       -  |
	|                     https://github.com/GCUWildwolfteam                     |
	|----------------------------------------------------------------------------|
	|--FileName    : motor.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-31               
	|--Libsupports : 标准库和HAL库
	|--Description : 1、maxion电机+RoboModule驱动 
	|								 2、3508电机+c610电调
	|								 3、6623电机
	|                4、6002电机                                  
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#ifndef __MOTOR_H 
#define __MOTOR_H
	#include "baseclass.h "
	#include "bsp_can.h" 
	#include "counters.h" 
/* =========================== common of begin =========================== */
	int16_t RatiometricConversion(int16_t real,int16_t threshold,int16_t perce,int32_t* last_real,int16_t* coefficient,uint32_t status);
  	int16_t NoRatiometricConversion(int16_t real,int16_t threshold,int16_t perce,int32_t* last_real,int16_t* coefficient,uint32_t status);
	int16_t zeroArgument(int16_t real, int16_t threshold);
/* =========================== common of end =========================== */
/* =========================== maxion of begin =========================== */
 /* -------- ID号 --------- */
	// typedef enum
	// {
	// 	openloop_mode_id_e                       = 0x01,//开环模式id号
	// 	current_mode_id_e                        = 0x02,//电流模式id号
	// 	velocity_mode_id_e                       = 0x03,//速度模式id号
	// 	position_mode_id_e                       = 0x04,//位置模式id号
	// 	velocity_position_mode_id_e              = 0x05,//速度位置模式id号
	// 	current_velocity_mode_id_e               = 0x06,//电流速度模式id号
	// 	current_position_mode_id_e               = 0x07,//电流位置模式id号
	// 	current_velocity_position_mode_id_e      = 0x08 //电流位置速度模式id号 
	// }motorIDEnum;

		#define openloop_mode_id_e                       0x01//开环模式id号
		#define current_mode_id_e                        0x02//电流模式id号
		#define velocity_mode_id_e                       0x03//速度模式id号
		#define position_mode_id_e                       0x04//位置模式id号
		#define velocity_position_mode_id_e              0x05//速度位置模式id号
		#define current_velocity_mode_id_e               0x06//电流速度模式id号
		#define current_position_mode_id_e               0x07//电流位置模式id号
		#define current_velocity_position_mode_id_e      0x08//电流位置速度模式id号 

		/* -------- 控制模式 --------- */
	// typedef enum
	// {
	// 	openloop_mode_e                       = 0x01,//开环模式
	// 	current_mode_e                        = 0x02,//电流模式
	// 	velocity_mode_e                       = 0x03,//速度模式
	// 	position_mode_e                       = 0x04,//位置模式
	// 	velocity_position_mode_e              = 0x05,//速度位置模式
	// 	current_velocity_mode_e               = 0x06,//电流速度模式
	// 	current_position_mode_e               = 0x07,//电流位置模式
	// 	current_velocity_position_mode_e      = 0x08//电流位置速度模式
	// }motorModeEnum;

	#define	openloop_mode_e                       0x01//开环模式
	#define	current_mode_e                        0x02//电流模式
	#define	velocity_mode_e                       0x03//速度模式
	#define	position_mode_e                       0x04//位置模式
	#define	velocity_position_mode_e              0x05//速度位置模式
	#define	current_velocity_mode_e               0x06//电流速度模式
	#define	current_position_mode_e               0x07//电流位置模式
	#define	current_velocity_position_mode_e      0x08//电流位置速度模式
	/* ------ 功能序号 ------- */
		// typedef enum
		// {
		// 	reset_fun_e                          = 0x00,//复位指令
		// 	mode_selection_fun_e                 = 0x01,//模式选择
		// 	openloop_fun_e                       = 0x02,//开环模式
		// 	current_fun_e                        = 0x03,//电流模式
		// 	velocity_fun_e                       = 0x04,//速度模式
		// 	position_fun_e                       = 0x05,//位置模式
		// 	velocity_position_fun_e              = 0x06,//速度位置模式
		// 	current_velocity_fun_e               = 0x07,//电流速度模式
		// 	current_position_fun_e               = 0x08,//电流位置模式
		// 	current_velocity_position_fun_e      = 0x09,//电流位置速度模式
		// 	config_fun_e 												 = 0x0A,//配置指令
		// 	data_feedback_fun_e 								 = 0x0B,//数据反馈
		// 	left_and_right_limit_feedback_e 		 = 0x0C,//左右限位反馈
		// 	online_check_fun_e 		 							 = 0x0F//在线检测
		// }functionNumEnum;

		#define	reset_fun_e                          0x00//复位指令
		#define	mode_selection_fun_e                 0x01//模式选择
		#define	openloop_fun_e                       0x02//开环模式
		#define	current_fun_e                        0x03//电流模式
		#define	velocity_fun_e                       0x04//速度模式
		#define	position_fun_e                       0x05//位置模式
		#define	velocity_position_fun_e              0x06//速度位置模式
		#define	current_velocity_fun_e               0x07//电流速度模式
		#define	current_position_fun_e               0x08//电流位置模式
		#define	current_velocity_position_fun_e      0x09//电流位置速度模式
		#define	config_fun_e 												 0x0A//配置指令
		#define	data_feedback_fun_e 								 0x0B//数据反馈
		#define	left_and_right_limit_feedback_e 		 0x0C//左右限位反馈
		#define	online_check_fun_e 		 							 0x0F//在线检测
	typedef struct maxionStruct
	{
		int16_t real_current; //真实电流
		int16_t real_position;//真实角度
		int16_t real_velocity;//真实速度
		uint32_t module_id;
    uint32_t module_rx_id;//电机数据接收id
		CAN_HandleTypeDef* hcanx;
	}maxionStruct;
	/* -------------- 公有宏 ----------------- */
	#define MOTOR_CAN_ID_CAL(group,number) \
						((((group)<<8)|((number)<<4)))	//电机can 发送id计算
	void ResetMode(CAN_HandleTypeDef *hcanx,uint32_t can_rx_id);
	void ModeSelectionMode(CAN_HandleTypeDef *hcanx,uint32_t rx_id,uint8_t mode);
	void OpenLoopMode(int16_t pwm,uint8_t *data);
  void SpeedLoopMode(int16_t pwm,int16_t Speed,uint8_t *data);
	void ConfigMode(CAN_HandleTypeDef *hcanx,uint32_t rx_id,uint8_t Time,\
																												uint8_t Ctl1_Ctl2);
	void MotorInit(CAN_HandleTypeDef *hcanx,uint8_t id,uint8_t mode);
  void MaxionParseData(maxionStruct*maxion,uint8_t *data);
  int16_t CalculateError(int16_t target,int16_t real,int16_t max_speed,int16_t linesnumb);
  int16_t CalculatePationError(int16_t target,int16_t real);
/* =========================== maxion of end =========================== */

/* ============================= RM6623 of begin ============================ */
  #define RM6623_LIM    13000//6623最大电流值 -13000~13000
	/* -------------- 结构体 ----------------- */	
	typedef struct RM6623Struct
	{
		uint16_t id;//电机can的 ip
		int16_t target;		 //目标值
		int16_t tem_target;//临时目标值
		int16_t real_current; //真实电流
		int16_t real_angle;//真实角度
		int16_t tem_angle;//临时角度
		int16_t zero;			 //电机零点
		int16_t Percentage;//转换比例（减速前角度:减速后的角度 = x:1
		int16_t thresholds; //电机反转阀值
    int16_t error;//当前误差
    int32_t last_real;
    int16_t coefficient;
    fps_t fps;//帧率
    postionPidStruct *ppostionPid_t;
    postionPidStruct* InnerLoopPid_t;
		//speedPidStruct *pspeedPid_t;
    
	} RM6623Struct;
	/* -------------- 公有函数 ----------------- */
	void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx);
	void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data);
/* ============================= RM6623 of end ============================== */
/* ============================ RM3508 of begin ============================= */
  #define RM3508_LIM    16384//RM3508最大电流值 -16384~16384
	typedef struct RM3508Struct
	{
		uint16_t id;//电机can的 ip
		int16_t target;		 //目标值
		int16_t tem_target;//临时目标值
		int16_t real_current; //真实电流
		int16_t real_angle;//真实角度
		int16_t tem_angle;//临时角度
    int16_t real_speed;//真实速度
    int16_t tem_speed;//真实速度
		int16_t zero;			 //电机零点
		int16_t Percentage;//转换比例（减速前角度:减速后的角度 = x:1
		int16_t thresholds; //电机反转阀值
    int16_t error;//当前误差
    fps_t fps;//帧率
    postionPidStruct *ppostionPid_t;
		speedPidStruct *pspeedPid_t;
	}RM3508Struct;
void RM3508ParseData(RM3508Struct *RM3508,uint8_t *data);
/* ============================== Rm3508 of end ============================= */
/* =========================== M2006 of begin =========================== */
  #define RM2006_LIM    10000//M2006最大电流值 -10000~10000
	typedef struct M2006Struct
	{
		uint16_t id;//电机can的 ip
		int16_t target;		 //目标值
		int16_t tem_target;//临时目标值
		int16_t real_current; //真实电流
		int16_t real_angle;//真实角度
		int16_t tem_angle;//临时角度
		int16_t zero;			 //电机零点
		int16_t Percentage;//转换比例（减速前角度:减速后的角度 = x:1
		int16_t thresholds; //电机反转阀值
    int16_t error;//当前误差
		int16_t real_speed;//真实速度
    int16_t tem_speed;//真实速度
    int32_t last_real;
    int16_t coefficient;
    fps_t fps;//帧率
    postionPidStruct *ppostionPid_t;
		speedPidStruct *pspeedPid_t;
	}M2006Struct;
	void RM2006ParseData(M2006Struct *M2006, uint8_t *data);
  void AntiRM2006ParseData(M2006Struct *M2006,uint8_t *data);
	#define M2006_THRESHOLD 6000
	#define M2006_POLES  8192
/* =========================== M2006 of end =========================== */
/* =========================== PWM控制的电机 of begin ======================== */
	void BrushlessMotorInit(void);
	 #define SET_FRICTIONGEAR_SPEED(__V_)															 \
				do{                                                  				\
						__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,__V_);  \
						__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,__V_);  \
					} while(0U)//设置摩擦轮速度
/* =========================== PWM控制的电机 of end ========================== */
/* =========================== M6020 of begin =========================== */
  #define RM6020_LIM    30000//M6020最大电流值 -30000~30000
  #define RM6020_LINES  8192//最大线数
	typedef struct GM6020Struct
{
		uint16_t id;//电机can的 ip
		int16_t target;		 //目标值
		int16_t tem_target;//临时目标值
		int16_t real_current; //真实电流
		int16_t real_angle;//真实角度
		int16_t tem_angle;//临时角度
		int16_t zero;			 //电机零点
		int16_t Percentage;//转换比例（减速前角度:减速后的角度 = x:1
		int16_t thresholds; //电机反转阀值
    int16_t error;//当前误差
		int16_t real_speed;//真实速度
    int16_t tem_speed;//真实速度
    int32_t last_real;
    int16_t coefficient;
    fps_t fps;//帧率
    postionPidStruct *ppostionPid_t;
//		speedPidStruct *pspeedPid_t;
  postionPidStruct *pspeedPid_t;
}GM6020Struct;
 void GM6020ParseData(GM6020Struct* GM6020,uint8_t *data);
/* =========================== M6020 of end =========================== */


#endif	// __MOTOR_H
/*---------------------------------file of end--------------------------------*/


