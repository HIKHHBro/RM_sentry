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
	|--FileName    : wholepositioning.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-29               
	|-- Libsupports: STM32CubeF1 Firmware Package V1.6.0 / 17-May-2017(用别的库出问题别问我)           
	|--Description : 1、调用WholePositioningInit申请内存和初始化
	|								 2、调用WholePositioningStart开启接收并解析数据                                                     
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end---------------------------------|
 **/
#ifndef __WHOLEPOSITIONING_H 
#define __WHOLEPOSITIONING_H 
		#include "bsp_usart.h"

		typedef struct wholePositionStruct
		{
			float z_angle;							//z轴角度															
			float x_angle;							//x轴角度
			float y_angle;							//y轴角度
			float x_coordinate; 				//x轴角度
			float y_coordinate; 				//y轴角度
			float angular_acceleration;	//角加速度

			float	C_X_Coordinate;
			float	C_Y_Coordinate;
			float C_Z_Angle;

			uint8_t *p_beffur;    //缓存数据指针
		  UART_HandleTypeDef* p_get_usartx;//得到串口状态指针
			 uint8_t status;  		//开启状态 //0代表开启成功 非零代表开启失败
			 int16_t rx_flag;     //接收标记位  1代表正常，-x代表掉帧 (x=1~10)
		/* -------- 保存全场定位所有数据现场结构体指针 --------- */
			 struct wholePositionStruct* p_lastdata_t; 
		}wholePositionStruct;
		#define WHOLE_POSION_LENGTH 28   				//全场定位一帧数据 28个字节
		#define WHOLE_POSION_BACK_LENFTH 1 			//防溢出
		HAL_StatusTypeDef  WholePositioningSetConfig(UART_HandleTypeDef* huart);
	  HAL_StatusTypeDef WholePositioningInit(wholePositionStruct* wps,UART_HandleTypeDef* huart);
		HAL_StatusTypeDef ParseWholePositioningInfo(wholePositionStruct* wps);
    HAL_StatusTypeDef WholePositioningStart(wholePositionStruct* wps,UART_HandleTypeDef* huart);
#endif	// __WHOLEPOSITIONING_H
	
 /*------------------------------------file of end------------------------------------*/


