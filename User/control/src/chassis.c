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
	chassisStruct chassis_t;
	RM3508Struct wheel1_t;//轮子1
	RM3508Struct wheel2_t;//轮子2
	#define WHEEL1_ID 0x205
	#define WHEEL2_ID 0x206
	#define CHASSIS_CAN_TX_ID 0x200
/**
	* @Data    2019-01-19 11:45
	* @brief   底盘数据解析
	* @param   chassisStruct* css 底盘结构体指针
	* @retval  void
	*/
	void ChassisInit(CAN_HandleTypeDef *hcan,chassisStruct *css)
	{
		css->hcanx = hcan;
		/* ------ 轮子1结构体初始化 ------- */
		css->pwheel1_t = &wheel1_t;
		wheel1_t.id = 0;
		wheel1_t.target = 0;
		wheel1_t.real_current = 0;
		wheel1_t.real_angle = 0;
		wheel1_t.real_speed = 0;
		/* ------ 轮子2结构体初始化 ------- */
		css->pwheel2_t = &wheel2_t;
		wheel2_t.id = 0;
		wheel2_t.target = 0;
		wheel2_t.real_current = 0;
		wheel2_t.real_angle = 0;
		wheel2_t.real_speed = 0;
	}
/**
	* @Data    2019-01-19 12:01
	* @brief   底盘数据解析
	* @param   Peripheral type外设类型,串口CAN_HandleTypeDef,can 的CAN_HandleTypeDef
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
/**
	* @Data    2019-02-15 14:15
	* @brief   底盘can数据发送
	* @param   void
	* @retval  void
	*/
	void ChassisCanTx(int16_t w1,int16_t w2)
	{
		uint8_t s[8]={0};
		s[0] = (uint8_t)(w1<<8);
 		s[1] = (uint8_t)w1;
		s[2] = (uint8_t)(w2<<8);
		s[3] = (uint8_t)w2;
		CanTxMsg(chassis_t.hcanx,CHASSIS_CAN_TX_ID,s);
	}
/*----------------------------------file of end-------------------------------*/
