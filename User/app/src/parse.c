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
 pcDataStruct pc_t;//小电脑数据
/* -------------- 外部链接 ----------------- */
extern osThreadId startParseTaskHandle;
extern UART_HandleTypeDef huart1;//串口1
extern uint8_t can1_rx[12];
extern uint32_t lenjuu;
	/**
	* @Data    2019-02-14 14:36
	* @brief   数据解析任务初始化
	* @param   void
	* @retval  void
	*/
void ParseInit(void)
{
  CommunicateInit();
    DJIDbusInit(&dbus_t,&huart1);//大疆遥控初始化
  PcDataRxInit(&pc_t);//小电脑数据接收初始化
  /* -------- 挂起等待任务系统初始化 --------- */
	vTaskSuspend(startParseTaskHandle);
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
//    taskENTER_CRITICAL();
//		ByleToMultibyte(&can1_rx[8],&can_id);
//		ChassisParseDate(can_id,can1_rx);
//    GimbalParseDate(can_id,can1_rx);
//    taskEXIT_CRITICAL();
        DbusParseData(&dbus_t);//
    Pc_ParseData(&pc_t);//小电脑数据解析
    		HCSR04RxMsg();//超声波数据接收
  CommunicateParse(lenjuu);

	}
 /*
	* @Data    2019-02-24 11:59
	* @brief   获取遥控构体地址
	* @param   void
	* @retval  void
	*/
	const dbusStruct* GetRcStructAddr(void)
	{
		return &dbus_t;
	}
	/**
	* @Data    2019-03-13 15:30
	* @brief   
	* @param   void
	* @retval  void
	*/
	uint32_t GetRcStatus(void)
	{
		return (dbus_t.status);
	}
  /**
	* @Data    2019-03-13 15:30
	* @brief   
	* @param   void
	* @retval  void
	*/
	uint32_t GetPcDataStatus(void)
	{
		return (pc_t.status);
	}
  /**
  * @Data    2019-03-21 01:09
  * @brief   获取小电脑构体地址
  * @param   void
  * @retval  void
  */
  const pcDataStruct* GetPcDataStructAddr(void)
  {
    return (&pc_t);
  }
//void can_rx(uint32_t id,uint8_t *data)
//  {
//    ChassisParseDate(id,data);
//    GimbalParseDate(id,data);
//  }
  void can_rx(uint32_t id,uint8_t *data)
  {

     GimbalParseDate(id,data);
  }
void can2_rx(uint32_t id,uint8_t *data)
{
         ChassisParseDate(id,data);
}
/*-----------------------------------file of end------------------------------*/


