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
	|--FileName    : user_task.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-15               
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
#include "user_task.h"
/* ----------------- 模块对象声明 -------------------- */
dbusStruct dbus_t; //大疆遥控
extern UART_HandleTypeDef huart1;//串口1
/* ----------------- 任务句柄 -------------------- */
osThreadId startSysInitTaskHandle; 
osThreadId startParseTaskHandle;
osThreadId startLedTaskHandle;
/* ----------------- 任务钩子函数 -------------------- */
void StartSysInitTask(void const *argument);
void StartParseTask(void const *argument);
void StartLedTask(void const *argument);
/* ----------------- 任务信号量 -------------------- */
static uint8_t task_on_off = 0; //功能任务开关
static uint8_t parse_task_status = 0;//数据解析任务工作状态标志
/*---------------------------------80字符限制-----------------------------------*/
/**
	* @Data    2019-01-16 18:30
	* @brief   系统初始化任务
	* @param   void
	* @retval  void
	*/
void SysInitCreate(void)
{
	/* -------- 系统初始化任务创建 --------- */
	task_on_off = DISABLE;//关闭所有任务
	osThreadDef(sysInitTask, StartSysInitTask, osPriorityNormal, 0, 400);
	startSysInitTaskHandle = osThreadCreate(osThread(sysInitTask), NULL);
   
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-16 18:27
	* @brief   系统初始化钩子函数
	* @param   argument: Not used 
	* @retval  void
	*/
	void StartSysInitTask(void const * argument)
	{
    for(;;)
    {
    /* -------- 模块初始化 --------- */
      DJIDbusInit(&dbus_t, &huart1);//大疆遥控初始化
    /* ----------------- 任务创建 -------------------- */
			/* -------- 数据分析任务 --------- */
      osThreadDef(parseTask, StartParseTask, osPriorityRealtime, 0, 400);
      startParseTaskHandle = osThreadCreate(osThread(parseTask), NULL);	
			/* -------- led灯提示任务 --------- */
			osThreadDef(ledTask, StartLedTask, osPriorityNormal, 0, 100);
      startLedTaskHandle = osThreadCreate(osThread(ledTask), NULL);
			ProgressBarLed(LED_GPIO, 500);
			task_on_off = ENABLE;//开启所有任务
			/* -------- 删除系统任务 --------- */
			vTaskDelete(startSysInitTaskHandle);
    }
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-16 18:27
	* @brief   解析任务
	* @param   argument: Not used
	* @retval  void
	*/
void StartParseTask(void const *argument)
{
	for(;;)
	{
		if(task_on_off==ENABLE)
		{
			DbusParseData(&dbus_t);
			
			parse_task_status = ENABLE;
      osDelay(1);
		}
		else
			parse_task_status = DISABLE;
			osDelay(1);
	}
}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-18 11:31
	* @brief   led灯提示任务钩子函数
	* @param   argument: Not used
	* @retval  void
	*/
	void StartLedTask(void const *argument)
	{
		for(;;)
		{
			if(task_on_off==ENABLE)
			{
					FlashingLed(LED_GPIO,LED_8,2,500);
			}
			if(parse_task_status == ENABLE)
			{
					FlashingLed(LED_GPIO, LED_1, 2, 500);
			}
			else osDelay(1);
		}
	}
/*------------------------------------file of end-------------------------------*/
  
