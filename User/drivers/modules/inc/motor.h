/**
	|-------------------------------- Copyright -----------------------------------|
	|                                                                              |
	|                        (C) Copyright 2018,海康平头哥,                         |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                               |
	|                                                                              |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                     https://github.com/GCUWildwolfteam                       |
	|------------------------------------------------------------------------------|
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
	|---------------------------------declaration of end----------------------------|
 **/
#ifndef __MOTOR_H 
#define __MOTOR_H
	#include "baseclass.h"
	#include "bsp_can.h" 
///* ----------------- maxion电机 -------------------- */
//  /* -------- ID号 --------- */
//	typedef enum
//	{
//		openloop_mode_id_e                       = 0x01,//开环模式id号
//		current_mode_id_e                        = 0x02,//电流模式id号
//		velocity_mode_id_e                       = 0x03,//速度模式id号
//		position_mode_id_e                       = 0x04,//位置模式id号
//		velocity_position_mode_id_e              = 0x05,//速度位置模式id号
//		current_velocity_mode_id_e               = 0x06,//电流速度模式id号
//		current_position_mode_id_e               = 0x07,//电流位置模式id号
//		current_velocity_position_mode_id_e      = 0x08 //电流位置速度模式id号 
//	}motorIDEnum;
//	/* -------- 控制模式 --------- */
//	typedef enum
//	{
//		openloop_mode_e                       = 0x01,//开环模式
//		current_mode_e                        = 0x02,//电流模式
//		velocity_mode_e                       = 0x03,//速度模式
//		position_mode_e                       = 0x04,//位置模式
//		velocity_position_mode_e              = 0x05,//速度位置模式
//		current_velocity_mode_e               = 0x06,//电流速度模式
//		current_position_mode_e               = 0x07,//电流位置模式
//		current_velocity_position_mode_e      = 0x08//电流位置速度模式
//	}motorModeEnum;
////	typedef struct 
////	{
////		
////	}maxionStruct;

///* ----------------- 函数定义 -------------------- */
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Reset(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Mode_Choice(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,unsigned char Mode);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_OpenLoop_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_PWM);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Velocity_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Position_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Velocity_Position_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Velocity_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Position_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Velocity_Position_Mode(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,long Temp_Position);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Config(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2);
//	HAL_StatusTypeDef CAN_RoboModule_DRV_Online_Check(CAN_HandleTypeDef* hcan,\
//		unsigned char Group,unsigned char Number);
/* ================================== RM6623 of begin ================================= */
	typedef struct RM6623Struct
	{
		uint16_t id;//电机can的ip
		int16_t target;		 //目标值
		int16_t tem_target;//临时目标值
		int16_t real_current; //真实电流
		int16_t real_angle;//真实角度
		int16_t tem_angle;//临时角度
		int16_t zero;			 //电机零点
		int16_t Percentage;//转换比例（减速前角度:减速后的角度 = x:1
		int16_t thresholds; //电机反转阀值
		CAN_HandleTypeDef *hcanx;
	} RM6623Struct;
	void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx);
	void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data);
	/* ================================== RM6623 of end =================================== */
	/* ================================== Rm3508 of begin ================================= */
	typedef struct RM3508Struct
	{
		uint16_t id;//电机can的ip
		int16_t target; //目标值
		int16_t real_current;//真实电流
		int16_t real_angle;//真实角度
		int16_t real_speed;//真实速度
		CAN_HandleTypeDef *hcanx;
	}RM3508Struct;
	void RM3508StructInit(RM3508Struct *RM3508,CAN_HandleTypeDef *hcanx);
	void RM3508ParseData(RM3508Struct *RM3508,uint8_t *data);
/* ================================== Rm3508 of end =================================== */
int16_t RatiometricConversion(int16_t real,int16_t threshold,int16_t percentage);
int16_t zeroArgument(int16_t real,int16_t threshold);
#endif	// __MOTOR_H
	
 /*------------------------------------file of end-------------------------------*/


