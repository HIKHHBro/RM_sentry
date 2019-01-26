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
	#include "baseclass.h"
	#include "bsp_can.h" 
	#include "counters.h" 
/* ============================= RM6623 of begin ============================ */
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
		postionPidStruct *ppostionPidStruct;
		speedPidStruct *speedPidStruct;
	} RM6623Struct;
	void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx);
	void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data);
/* ============================= RM6623 of end ============================== */
/* ============================ RM3508 of begin ============================= */
	typedef struct RM3508Struct
	{
		uint16_t id;//电机can的ip
		int16_t target; //目标值
		int16_t real_current;//真实电流
		int16_t real_angle;//真实角度
		int16_t real_speed;//真实速度
		CAN_HandleTypeDef *hcanx;
		postionPidStruct *ppostionPidStruct;
		speedPidStruct *speedPidStruct;
	}RM3508Struct;
/* ============================== Rm3508 of end ============================= */
/* =========================== M6620 of begin =========================== */
	typedef struct M2006Struct
	{
		uint16_t id;//电机can的ip
		int16_t target; //目标值
		int16_t real_current;//真实电流
		int16_t real_angle;//真实角度
		int16_t real_speed;//真实速度
		CAN_HandleTypeDef *hcanx;
		postionPidStruct *ppostionPidStruct;
		speedPidStruct *speedPidStruct;
	}M2006Struct;
	void RM2006ParseData(M2006Struct *M2006, uint8_t *data);
/* =========================== M6620 of end =========================== */
	int16_t RatiometricConversion(int16_t real, int16_t threshold, int16_t perce);
	int16_t zeroArgument(int16_t real, int16_t threshold);
#endif	// __MOTOR_H
/*---------------------------------file of end--------------------------------*/


