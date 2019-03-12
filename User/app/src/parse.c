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
  dbusStruct dbus_t; //大疆遥控
/* -------------- 外部链接 ----------------- */
extern UART_HandleTypeDef huart1;//串口1
extern uint8_t can1_rx[12];
	/**
	* @Data    2019-02-14 14:36
	* @brief   数据解析任务初始化
	* @param   void
	* @retval  void
	*/
void ParseInit(void)
{
  DJIDbusInit(&dbus_t,&huart1);//大疆遥控初始化
}

	/**
	* @Data    2019-02-14 14:36
	* @brief   数据解析
	* @param   void
	* @retval  void
	*/
	void ParseData(void)
	{
//		uint32_t can_id = 0;
//		ByleToMultibyte(&can1_rx[8],&can_id);
//		ChassisParseDate(can_id,can1_rx);
    DbusParseData(&dbus_t);//遥控解析
	}
 /*
	* @Data    2019-02-24 11:59
	* @brief   获取遥控构体地址
	* @param   void
	* @retval  void
	*/
	const dbusStruct* GetRcStructAdd(void)
	{
		return &dbus_t;
	}
/*-----------------------------------file of end------------------------------*/


