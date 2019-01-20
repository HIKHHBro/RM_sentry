/**
	|-------------------------------- Copyright ----------------------------------------|
	|                                                                                   |
	|                        (C) Copyright 2018,海康平头哥,                              |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China            |
	|                            All Rights Reserved                                    |
	|                                                                                   |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)               |
	|                     https://github.com/GCUWildwolfteam                            |
	|-----------------------------------------------------------------------------------|
	|--FileName    : Chassis.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-11               
	|--Description : 1、三轮底盘 THIRD_OMNIDIRECTIONAL_WHEEL_CHASSIS 置1开启，0关闭
	|								 2、普通四轮麦克轮底盘	MCUNAMM_WHEEL_CHASSIS 置1开启，0关闭
	|								    （注意，不能 同时置1开启）
	|								 2、用底盘文件，一定要can.c、can.h和motor.h 这三个文件                                                 
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end---------------------------------|
 **/
#ifndef __CHASSIS_H
	#include "motor.h" 
	#include "photoelectric_sensor.h" 
	/* ----------------- 底盘类型选择(不能全部置1) ---------------- */
		#define THIRD_OMNIDIRECTIONAL_WHEEL_CHASSIS			 1
		#define MCUNAMM_WHEEL_CHASSIS              			 0
  /* ----------------- 三轮底盘结构体 -------------------- */
		typedef struct 
		{
				int16_t v_x;																			  //Vx速度
				int16_t v_y;																				//Vy速度
				int16_t v_w;																				//自旋速度

				int16_t motorspeed1;																//电机1分配速度
				int16_t motorspeed2;																//电机2分配速度
				int16_t motorspeed3;																//电机3分配速度

		}thirdWheelChassisStruct;
	/* -------- 底盘电机结构体 --------- */	
		typedef struct
		{ 
	#if THIRD_OMNIDIRECTIONAL_WHEEL_CHASSIS 
		/* -------- 全向三轮底盘 --------- */
			thirdWheelChassisStruct thirdWheelChassis_t;//三轮底盘结构体声明
	#elif MCUNAMM_WHEEL_CHASSIS
	/* -------- 麦克纳姆轮底盘 --------- */
	#endif 
		/* -------- 继承 --------- */
			photoelectricStruct* photoelectric_t;
			CAN_HandleTypeDef* p_canx;												//获取can类型
			uint8_t status;                               //0代表开启成功 非零代表开启失败
			void* p_addr_cache  ;                          //临时地址保存，用完一定要指空（危险）
		}chassiStruct;	
  /* -------- robomoter电机控制模式 --------- */
		HAL_StatusTypeDef ChassisClassInit(chassiStruct * cC,CAN_HandleTypeDef* hcan\
																			,photoelectricStruct* plt);
		void ChassisMotorModeInit(uint8_t mode);


		
#endif	// __CHASSIS_H
	
 /*------------------------------------file of end------------------------------------*/


