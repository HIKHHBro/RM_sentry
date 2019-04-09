/**
	|------------------------------ Copyright -----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,海康平头哥,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|          By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                     https://github.com/GCUWildwolfteam                     |
	|----------------------------------------------------------------------------|
	|--FileName    : sys_task.c                                                
	|--Version     : v2.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-15               
	|--Libsupports : 
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#include "sys_task.h"

/* ----------------- 模块对象声明 -------------------- */
extern UART_HandleTypeDef huart1;//串口1
extern UART_HandleTypeDef huart2;//串口1
extern CAN_HandleTypeDef hcan1;
/* ----------------- 任务句柄 -------------------- */
	osThreadId startSysInitTaskHandle; 
	osThreadId startParseTaskHandle;
//	osThreadId startLedTaskHandle;
	osThreadId startChassisTaskHandle;
	osThreadId startGimbalTaskHandle;
	osThreadId startTxTaskHandle;//发送任务
/* ----------------- 任务钩子函数 -------------------- */
	void StartSysInitTask(void const *argument);
	void StartParseTask(void const *argument);
//	void StartLedTask(void const *argument);
	void StartChassisTask(void const *argument);
	void StartGimbalTask(void const *argument);
	void StartTxTask(void const *argument);
/* ----------------- 任务信号量 -------------------- */
//static uint8_t parse_task_status = 0;//数据解析任务工作状态标志
/**
	* @Data    2019-01-16 18:30
	* @brief   系统初始化任务
	* @param   void
	* @retval  void
	*/
	void SysInitCreate(void)
	{
		/* -------- 系统初始化任务创建 --------- */
		osThreadDef(sysInitTask, StartSysInitTask, osPriorityRealtime, 0, SYS_INIT_HEAP_SIZE);
		startSysInitTaskHandle = osThreadCreate(osThread(sysInitTask), NULL);
	}
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
			/* -------- 数据分析任务 --------- */
      osThreadDef(parseTask, StartParseTask, osPriorityHigh, 0, PARSE_HEAP_SIZE);
      startParseTaskHandle = osThreadCreate(osThread(parseTask), NULL);	
//			/* -------- led灯提示任务 --------- */
//			osThreadDef(ledTask, StartLedTask, osPriorityNormal, 0,128);
//      startLedTaskHandle = osThreadCreate(osThread(ledTask), NULL);
			/* ------ 底盘任务 ------- */
			osThreadDef(chassisTask, StartChassisTask, osPriorityNormal, 0, CHASSIS_HEAP_SIZE);
      startChassisTaskHandle = osThreadCreate(osThread(chassisTask), NULL);
			/* ------ 云台任务 ------- */
			osThreadDef(gimbalTask, StartGimbalTask, osPriorityNormal, 0, GIMBAL_HEAP_SIZE);
      startGimbalTaskHandle = osThreadCreate(osThread(gimbalTask), NULL);
      /* ------ 数据发送任务任务 ------- */
			osThreadDef(txTask, StartTxTask, osPriorityHigh, 0, TX_HEAP_SIZE);
      startTxTaskHandle = osThreadCreate(osThread(txTask), NULL);
       ProgressBarLed(LED_GPIO, 500);
      /* -------- 系统模块自检 --------- */
        SystemSelfChecking();
			/* -------- 删除系统任务 --------- */
			vTaskDelete(startSysInitTaskHandle);
    }
	}
	/**
	* @Data    2019-01-16 18:27
	* @brief   解析任务
	* @param   argument: Not used
	* @retval  void
	*/
	void StartParseTask(void const *argument)
	{
    ParseInit();
		for(;;)
		{
				ParseData();
				osDelay(PARSE_DELAY);
		}
	}

/**
	* @Data    2019-01-27 17:54
	* @brief   底盘任务钩子函数
	* @param   argument: Not used
	* @retval  void
	*/
	void StartChassisTask(void const *argument)
	{
    const dbusStruct* pRc_t;
		const pcDataStruct* pPc_t;
    pRc_t = GetRcStructAddr();
    pPc_t = GetPcDataStructAddr();//小电脑数据
    ChassisInit(CHASSIS_CAN,pRc_t,pPc_t);
		for (;;)
		{
        ChassisControl();
			  osDelay(5);
		}
	}
/**
	* @Data    2019-01-27 17:55
	* @brief   云台任务钩子函数
	* @param   argument: Not used
	* @retval  void
	*/
	void StartGimbalTask(void const *argument)
	{
    const dbusStruct* pRc_t;
    const pcDataStruct* pPc_t;
    pRc_t = GetRcStructAddr();//遥控数据
    pPc_t = GetPcDataStructAddr();//小电脑数据
    GimbalStructInit(pRc_t,pPc_t); 
		for (;;)
		{  
				GimbalAutoControl();
				osDelay(5);
		}
	}
/**
	* @Data    2019-03-20 20:01
	* @brief   用户数据发送任务
	* @param   void
	* @retval  void
	*/
	void StartTxTask(void const *argument)
	{
		const dbusStruct* pRc_t;
    pRc_t = GetRcStructAddr();
    UserTxInit(pRc_t);
		for (;;)
		{ 
			UserTxControl();
				osDelay(4);
		}
	}
/*----------------------------------file of end-------------------------------*/
  
