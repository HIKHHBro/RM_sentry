/**
	|-------------------------------- Copyright -----------------------------------|
	|                                                                              |
	|                        (C) Copyright 2019,海康平头哥,                         |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                               |
	|                                                                              |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                     https://github.com/GCUWildwolfteam                       |
	|------------------------------------------------------------------------------|
	|--FileName    : photoelectric_sensor.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-12               
	|--Libsupports : STM32CubeF1 Firmware Package V1.6.0(用别的库出问题别问我)
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end----------------------------|
 **/
#ifndef __PHOTOELECTRIC_SENSOR_H 
#define __PHOTOELECTRIC_SENSOR_H 
	#include "baseclass.h"
	#include "can.h"

	/* ----------------- 光电结构体 -------------------- */
		typedef struct
		{
			int16_t front; 
			int16_t back;
			int16_t right;
			int16_t left;
			CAN_HandleTypeDef* hcanx; //缓存can类型地址
			void* p_addr_cache;      //临时地址保存，用完一定要指空（危险）
			
		}photoelectricStruct;
	/* ----------------- 光电ID号 -------------------- */
		#define FRONT 1   //前
		#define LEFT  2 	//左
		#define BLACK 3 	//后
		#define RIGHT 4 	//右
	HAL_StatusTypeDef PhotoelectricInit(photoelectricStruct* ps,CAN_HandleTypeDef* hcan);
	HAL_StatusTypeDef PhotoelectricGetInfo(photoelectricStruct* ps);
#endif	// __PHOTOELECTRIC_SENSOR_H
	
 /*------------------------------------file of end-------------------------------*/


