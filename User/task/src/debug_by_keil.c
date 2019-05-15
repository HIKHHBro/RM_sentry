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
	|--FileName    : debug_by_keil.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-02-23             
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
#include "debug_by_keil.h" 
#ifdef DEBUG_BY_KEIL
/* -------- 打印任务状态 --------- */
//#define PRINTFTASKSTATUS 
 extern UART_HandleTypeDef huart2;//串口1
 extern UART_HandleTypeDef huart7;
extern TIM_HandleTypeDef htim2;
extern	 pcDataStruct pc_t;//小电脑数据;
 uint32_t runtimeCounter;
debugByKeilStruct pdebug_t;
osThreadId startDebugByKeilTaskHandle; 
void StartDebugByKeilTask(void const *argument);
/* ======================== 临时测试代码变量 of begin ======================== */
	uint8_t aaa=0;
/* ======================== 临时测试代码变量 of end ======================== */



/**
  * @Data    2019-02-23 15:46
  * @brief   获取局部变量的地址，用于keil硬件仿真
  * @param   void
  * @retval  void
  */
  void DebugByKeilInit(void)
  {
    /* -------- 测量运行时间定时器 --------- */
//    HAL_TIM_Base_Start_IT(&htim10);
    /* -------- keil硬件仿真任务示任务 --------- */
     osThreadDef(debugByKeilTask, StartDebugByKeilTask, osPriorityHigh, 0,DEBUG_HEAP_SIZE);
     startDebugByKeilTaskHandle = osThreadCreate(osThread(debugByKeilTask), NULL); 
//    pdebug_t.d_rc= GetRcStructAddr();
   pdebug_t.d_pchassis_t = GetChassisStructAddr();

//			 DebugClassInit();   

    
  }
/**
  * @Data    2019-02-23 15:46
  * @brief   keil硬件仿真任务钩子函数
  * @param   void
  * @retval  void
  */
  int16_t gggfffdg =0;
void StartDebugByKeilTask(void const *argument)
{
#ifdef PRINTFTASKSTATUS
     uint8_t pcWriteBuffer[500];
#endif
		for(;;)
		{
#ifdef PRINTFTASKSTATUS
      taskENTER_CRITICAL();
        memset(pcWriteBuffer, 0, 500);
        vTaskList((char *)&pcWriteBuffer);
        printf("任务总内存大小 = %d\r\n",configTOTAL_HEAP_SIZE);
        printf("任务名称     运行状态  优先级  剩余堆栈  任务序号\r\n");
        printf("-------------------------------------------------\r\n");
        printf("%s  \r\n", pcWriteBuffer); 
        printf("-------------------------------------------------\r\n");
        printf("B:阻塞    R:就绪    D:删除    S:暂停\r\n");
        printf("-------------------------------------------------\r\n");
        memset(pcWriteBuffer, 0, 500);
        printf("任务名称\t运行计数\t使用率\r\n");
        printf("-------------------------------------------------\r\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
        printf("-------------------------------------------------\r\n");
     taskEXIT_CRITICAL();
        osDelay(100);      
#endif
//     if(GetFps(pc_t.fps) ==0)
//   {
//       __HAL_TIM_SetCompare(BUZZER_TIM,FRICTIONGEAR_1,10000);
//     }  
//    __HAL_TIM_SetCompare(BUZZER_TIM,FRICTIONGEAR_1,0);
 osDelay(100); 
		}
}
/**
* @Data    2019-02-23 15:46
* @brief   用户定时器回调函数
* @param   void
* @retval  void
*/
void USER_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM10)
  {
    runtimeCounter++;
  }
}
void configureTimerForRunTimeStats(void)
{
  runtimeCounter = 0;
}

unsigned long getRunTimeCounterValue(void)
{
return runtimeCounter;
}
/* =========================== 测试代码区 of begin =========================== */

/* =========================== 测试代码区 of end =========================== */
#endif
/*-----------------------------------file of end------------------------------*/


