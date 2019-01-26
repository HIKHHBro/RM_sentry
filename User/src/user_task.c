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
	|--Version     : v1.0                                                            
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
dbusStruct dbus_t; //��ң��
extern UART_HandleTypeDef huart1;//����1
/* ----------------- ������ -------------------- */
osThreadId startSysInitTaskHandle; 
osThreadId startParseTaskHandle;
osThreadId startLedTaskHandle;
/* ----------------- �����Ӻ��� -------------------- */
void StartSysInitTask(void const *argument);
void StartParseTask(void const *argument);
void StartLedTask(void const *argument);
/* ----------------- �����ź��� -------------------- */
static uint8_t task_on_off = 0; //�������񿪹�
static uint8_t parse_task_status = 0;//���ݽ���������״̬��־
/**
	* @Data    2019-01-16 18:30
	* @brief   ϵͳ��ʼ������
	* @param   void
	* @retval  void
	*/
void SysInitCreate(void)
{
	/* -------- ϵͳ��ʼ�����񴴽� --------- */
	task_on_off = DISABLE;//�ر���������
	osThreadDef(sysInitTask, StartSysInitTask, osPriorityNormal, 0, 400);
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
    /* -------- ģ���ʼ�� --------- */
      DJIDbusInit(&dbus_t, &huart1);//��ң�س�ʼ��
    /* ----------------- ���񴴽� -------------------- */
			/* -------- ���ݷ������� --------- */
      osThreadDef(parseTask, StartParseTask, osPriorityRealtime, 0, 400);
      startParseTaskHandle = osThreadCreate(osThread(parseTask), NULL);	
			/* -------- led����ʾ���� --------- */
			osThreadDef(ledTask, StartLedTask, osPriorityNormal, 0, 100);
      startLedTaskHandle = osThreadCreate(osThread(ledTask), NULL);
			ProgressBarLed(LED_GPIO, 500);
			task_on_off = ENABLE;//������������
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
/*----------------------------------file of end-------------------------------*/
  
