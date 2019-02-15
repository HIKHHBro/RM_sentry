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
	|--FileName    : parse.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-02-02             
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
#include "parse.h" 
/* -------------- 结构体 ----------------- */
extern UART_HandleTypeDef huart1;//串口1
extern CAN_HandleTypeDef hcan1;
dbusStruct dbus_t; //大疆遥控
extern uint8_t can1_rx[12];
//static uint8_t can1_rx_buffer[12]={0};
	/**
		* @Data    2019-02-02 12:59
		* @brief   解析数据初始化
		* @param   void
		* @retval  void
		*/
		void ParseInit(void)
		{
      DJIDbusInit(&dbus_t, &huart1);//大疆遥控初始化
		}
	/**
		* @Data    2019-02-14 14:36
		* @brief   数据解析
		* @param   void
		* @retval  void
		*/
		void ParseData(void)
		{
			uint32_t can_id = 0;
//			UserCanQueueRX(&hcan1,can1_rx);//cam1接收
			ByleToMultibyte(&can1_rx[8],&can_id);
			if(can_id > GIMBAL_CAN_ID_L && can_id < GIMBAL_CAN_ID_H)
			{
				GimbalParseDate(can_id,can1_rx);
			}
      DbusParseData(&dbus_t);
		}
/*-----------------------------------file of end------------------------------*/


