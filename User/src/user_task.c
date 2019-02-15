/**
	|------------------------------ Copyright -----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,����ƽͷ��,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
	|                     https://github.com/GCUWildwolfteam                     |
	|----------------------------------------------------------------------------|
	|--FileName    : user_task.c                                                
	|--Version     : v2.0                                                            
	|--Author      : ����ƽͷ��                                                       
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
#include "user_task.h"
/* ----------------- ģ��������� -------------------- */

extern UART_HandleTypeDef huart1;//����1
extern CAN_HandleTypeDef hcan1;
extern dbusStruct dbus_t; //��ң��
/* ----------------- ������ -------------------- */
	osThreadId startSysInitTaskHandle; 
	osThreadId startParseTaskHandle;
	osThreadId startLedTaskHandle;
	osThreadId startChassisTaskHandle;
	osThreadId startGimbalTaskHandle;
/* ----------------- �����Ӻ��� -------------------- */
	void StartSysInitTask(void const *argument);
	void StartParseTask(void const *argument);
	void StartLedTask(void const *argument);
	void StartChassisTask(void const *argument);
	void StartGimbalTask(void const *argument);
/* ----------------- �����ź��� -------------------- */
static uint8_t parse_task_status = 0;//���ݽ���������״̬��־
uint8_t task_on_off = 0;
/**
	* @Data    2019-01-16 18:30
	* @brief   ϵͳ��ʼ������
	* @param   void
	* @retval  void
	*/
	void SysInitCreate(void)
	{
		/* -------- ϵͳ��ʼ�����񴴽� --------- */
		osThreadDef(sysInitTask, StartSysInitTask, osPriorityNormal, 0, 512);
		startSysInitTaskHandle = osThreadCreate(osThread(sysInitTask), NULL);
	}
/**
	* @Data    2019-01-16 18:27
	* @brief   ϵͳ��ʼ�����Ӻ���
	* @param   argument: Not used 
	* @retval  void
	*/
	void StartSysInitTask(void const * argument)
	{
    for(;;)
    {
      task_on_off = DISABLE;
			/* -------- ���ݷ������� --------- */
      osThreadDef(parseTask, StartParseTask, osPriorityRealtime, 0, 512);
      startParseTaskHandle = osThreadCreate(osThread(parseTask), NULL);	
			/* -------- led����ʾ���� --------- */
			osThreadDef(ledTask, StartLedTask, osPriorityNormal, 0,128);
      startLedTaskHandle = osThreadCreate(osThread(ledTask), NULL);
//			/* ------ �������� ------- */
//			osThreadDef(chassisTask, StartChassisTask, osPriorityNormal, 0, 100);
//      startChassisTaskHandle = osThreadCreate(osThread(chassisTask), NULL);
			/* ------ ��̨���� ------- */
			osThreadDef(gimbalTask, StartGimbalTask, osPriorityNormal, 0, 640);
      startGimbalTaskHandle = osThreadCreate(osThread(gimbalTask), NULL);
			ProgressBarLed(LED_GPIO, 500);
      task_on_off = ENABLE;
			/* -------- ɾ��ϵͳ���� --------- */
			vTaskDelete(startSysInitTaskHandle);
    }
	}
	/**
	* @Data    2019-01-16 18:27
	* @brief   ��������
	* @param   argument: Not used
	* @retval  void
	*/
	void StartParseTask(void const *argument)
	{
    ParseInit();
		for(;;)
		{
      if(task_on_off == ENABLE)
      {
				ParseData();
				parse_task_status = ENABLE;
				osDelay(1);
      }
      else osDelay(1);
		}
	}
	/**
	* @Data    2019-01-18 11:31
	* @brief   led����ʾ�����Ӻ���
	* @param   argument: Not used
	* @retval  void
	*/
	void StartLedTask(void const *argument)
	{
		for(;;)
		{
      if(task_on_off == ENABLE)
      {
         FlashingLed(LED_GPIO,LED_7,2,100);
        if(parse_task_status == ENABLE)
        {
          FlashingLed(LED_GPIO, LED_1, 2, 100);
        }
        else osDelay(1);
      }
      else osDelay(1);
		}
    
	}
/**
	* @Data    2019-01-27 17:54
	* @brief   ���������Ӻ���
	* @param   argument: Not used
	* @retval  void
	*/
	void StartChassisTask(void const *argument)
	{
		for (;;)
		{
			osDelay(1);
		}
	}
/**
	* @Data    2019-01-27 17:55
	* @brief   ��̨�����Ӻ���
	* @param   argument: Not used
	* @retval  void
	*/
	void StartGimbalTask(void const *argument)
	{
    GimbalStructInit(&hcan1); 
		for (;;)
		{  
			if(task_on_off == ENABLE)
      {
				GimbalControl(&dbus_t);
				osDelay(2);
			}
			else osDelay(1);
		}
	}
/*----------------------------------file of end-------------------------------*/
  
