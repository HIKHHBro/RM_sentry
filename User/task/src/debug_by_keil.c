/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,����ƽͷ��,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : debug_by_keil.c                                              
	|--Version     : v1.0                                                          
	|--Author      : ����ƽͷ��                                                     
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
/* -------- ��ӡ����״̬ --------- */
//#define PRINTFTASKSTATUS 
 extern UART_HandleTypeDef huart2;//����1
 extern UART_HandleTypeDef huart7;
 extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim2;
 uint32_t runtimeCounter;
debugByKeilStruct pdebug_t;
osThreadId startDebugByKeilTaskHandle; 
void StartDebugByKeilTask(void const *argument);
/* ======================== ��ʱ���Դ������ of begin ======================== */
	uint8_t aaa=0;
/* ======================== ��ʱ���Դ������ of end ======================== */



/**
  * @Data    2019-02-23 15:46
  * @brief   ��ȡ�ֲ������ĵ�ַ������keilӲ������
  * @param   void
  * @retval  void
  */
  void DebugByKeilInit(void)
  {
    /* -------- ��������ʱ�䶨ʱ�� --------- */
    HAL_TIM_Base_Start_IT(&htim10);
    /* -------- keilӲ����������ʾ���� --------- */
     osThreadDef(debugByKeilTask, StartDebugByKeilTask, osPriorityLow, 0,DEBUG_HEAP_SIZE);
     startDebugByKeilTaskHandle = osThreadCreate(osThread(debugByKeilTask), NULL); 
//    pdebug_t.d_rc= GetRcStructAddr();
//    pdebug_t.d_pchassis_t = GetChassisStructAddr();
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
    	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
//			 DebugClassInit();      
    
  }
/**
  * @Data    2019-02-23 15:46
  * @brief   keilӲ�����������Ӻ���
  * @param   void
  * @retval  void
  */
void StartDebugByKeilTask(void const *argument)
{
#ifdef PRINTFTASKSTATUS
     uint8_t pcWriteBuffer[500];
#endif
		for(;;)
		{
#ifdef PRINTFTASKSTATUS
      taskENTER_CRITICAL();
        vTaskList((char *)&pcWriteBuffer);
        printf("��������     ����״̬  ���ȼ�  ʣ���ջ  �������\r\n");
        printf("-------------------------------------------------\r\n");
        printf("%s  \r\n", pcWriteBuffer); 
        printf("-------------------------------------------------\r\n");
        printf("B:����    R:����    D:ɾ��    S:��ͣ\r\n");
        printf("-------------------------------------------------\r\n");
        memset(pcWriteBuffer, 0, 500);
        printf("��������\t���м���\tʹ����\r\n");
        printf("-------------------------------------------------\r\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
        printf("-------------------------------------------------\r\n");
        taskEXIT_CRITICAL();
        osDelay(500);      
#endif
      if(aaa == 0)
			{
				aaa =1;
        HAL_GPIO_TogglePin(SONIC_GPIO,SONIC);
       osDelay(1);
			}
			else
			{
				aaa =0;
				osDelay(10);
			}
      
		}
}
/**
* @Data    2019-02-23 15:46
* @brief   �û���ʱ���ص�����
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
/* =========================== ���Դ����� of begin =========================== */
uint16_t 	Channel3HighTime, Channel4HighTime;
uint16_t Channel3Period, Channel4Period;
uint8_t   Channel3Edge = 0, Channel4Edge = 0;
uint32_t  Channel3Percent, Channel4Percent;
uint16_t 	Channel3RisingTimeLast=0, Channel3RisingTimeNow, Channel3FallingTime;
uint16_t 	Channel4RisingTimeLast=0, Channel4RisingTimeNow, Channel4FallingTime;
	void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
	{
		
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if(Channel3Edge == 0)
		{
			Channel3RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_3);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
			Channel3Edge = 1;
			if(Channel3RisingTimeLast == 0)
			{
				Channel3Period = 0;
			}
			else
			{
				if(Channel3RisingTimeNow > Channel3RisingTimeLast)
				{
					Channel3Period = Channel3RisingTimeNow - Channel3RisingTimeLast;
				}
				else
				{
					Channel3Period = Channel3RisingTimeNow + 0xffff - Channel3RisingTimeLast + 1;
				}
			}
			Channel3RisingTimeLast = Channel3RisingTimeNow;
		}
		else if(Channel3Edge == 1)
		{
			Channel3FallingTime = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_3);	
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
			
			if(Channel3FallingTime < Channel3RisingTimeNow)
			{
				Channel3HighTime = Channel3FallingTime + 0xffff - Channel3RisingTimeNow + 1;
			}
			else
			{
				Channel3HighTime = Channel3FallingTime - Channel3RisingTimeNow;
			}
			if(Channel3Period != 0)
			{
				Channel3Percent = Channel3HighTime*0.017;
				// Channel3Percent = (uint32_t )(float)(Channel3HighTime);
				// Channel3Percent = (uint8_t)(((float)Channel3HighTime / Channel3Period) * 1000);
			}
			Channel3Edge = 0;
		}
	}
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if(Channel4Edge == 0)
		{
			Channel4RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_4);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
			Channel4Edge = 1;
			if(Channel4RisingTimeLast == 0)
			{
				Channel4Period = 0;
			}
			else
			{
				if(Channel4RisingTimeNow > Channel4RisingTimeLast)
				{
					Channel4Period = Channel4RisingTimeNow - Channel4RisingTimeLast;
				}
				else
				{
					Channel4Period = Channel4RisingTimeNow + 0xffff - Channel4RisingTimeLast + 1;
				}
			}
			Channel4RisingTimeLast = Channel4RisingTimeNow;
		}
		else if(Channel4Edge == 1)
		{
			Channel4FallingTime = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_4);	
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
			
			if(Channel4FallingTime < Channel4RisingTimeNow)
			{
				Channel4HighTime = Channel4FallingTime + 0xffff - Channel4RisingTimeNow + 1;
			}
			else
			{
				Channel4HighTime = Channel4FallingTime - Channel4RisingTimeNow;
			}
			if(Channel4Period != 0)
			{
        Channel4Percent = Channel4HighTime*0.017;
				// Channel4Percent = 
				// Channel4Percent = (uint8_t)(((float)Channel4HighTime / Channel4Period) * 1000);
			}
			Channel4Edge = 0;
		}
	}

	}
/* =========================== ���Դ����� of end =========================== */
#endif
/*-----------------------------------file of end------------------------------*/


