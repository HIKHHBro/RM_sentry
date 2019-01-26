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
	|--FileName    : chassis.c                                                
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
	|-------------------------------declaration of end---------------------------|
 **/
#include "chassis.h"
	RM3508Struct wheel1_t;//轮子1
	RM3508Struct wheel2_t;//轮子2
	extern CAN_HandleTypeDef hcan1;
	#define WHEEL1_ID 0x205
	#define WHEEL2_ID 0x206
/**
	* @Data    2019-01-19 11:45
	* @brief   底盘数据解析
	* @param   chassisStruct* css 底盘结构体指针
	* @retval  void
	*/
	void ChassisInit(chassisStruct* css)
	{
		css->pwheel1_t = &wheel1_t;
		css->pwheel2_t = &wheel2_t;
		RM3508StructInit(css->pwheel1_t,&hcan1);
		RM3508StructInit(css->pwheel2_t,&hcan1);
	}
/**
	* @Data    2019-01-19 12:01
	* @brief   底盘数据解析
	* @param   Peripheral type外设类型,串口CAN_HandleTypeDef,can的CAN_HandleTypeDef
	* @retval  void
	*/
	void ChassisParseDate(uint16_t id)
	{
		
		switch (id)
		{
			case WHEEL1_ID:
				// RM3508ParseData();
				break;
		
			default:
				break;
		}
	}
/*----------------------------------file of end-------------------------------*/
