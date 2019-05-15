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
	|--FileName    : sys_task.c                                                
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
#include "sys_task.h"

/* ----------------- ģ��������� -------------------- */
extern UART_HandleTypeDef huart1;//����1
extern UART_HandleTypeDef huart2;//����1
extern CAN_HandleTypeDef hcan1;
/* ----------------- ������ -------------------- */
	osThreadId startSysInitTaskHandle; 
	osThreadId startParseTaskHandle;
  osThreadId startFaulTaskHandle;
	osThreadId startChassisTaskHandle;
	osThreadId startGimbalTaskHandle;
	osThreadId startTxTaskHandle;//��������
/* ----------------- �����Ӻ��� -------------------- */
	void StartSysInitTask(void const *argument);
	void StartParseTask(void const *argument);
	void StartFaulTask(void const *argument);
	void StartChassisTask(void const *argument);
	void StartGimbalTask(void const *argument);
	void StartTxTask(void const *argument);
/* ----------------- �����ź��� -------------------- */
//static uint8_t parse_task_status = 0;//���ݽ���������״̬��־
uint8_t enble_s = 0;
/**
	* @Data    2019-01-16 18:30
	* @brief   ϵͳ��ʼ��
	* @param   void
	* @retval  void
	*/
	void SysInitCreate(void)
	{
		/* -------- ϵͳ��ʼ�����񴴽� --------- */
		osThreadDef(sysInitTask, StartSysInitTask, osPriorityRealtime, 0, SYS_INIT_HEAP_SIZE);
		startSysInitTaskHandle = osThreadCreate(osThread(sysInitTask), NULL);
    /* -------- ֡����ʱ���� --------- */
    	FpsUserTimeInit();
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
			/* -------- ���ݷ������� --------- */
      osThreadDef(parseTask, StartParseTask, osPriorityHigh, 0, PARSE_HEAP_SIZE);
      startParseTaskHandle = osThreadCreate(osThread(parseTask), NULL);	
			/* ------ ��̨���� ------- */
			osThreadDef(gimbalTask, StartGimbalTask, osPriorityNormal, 0, GIMBAL_HEAP_SIZE);
      startGimbalTaskHandle = osThreadCreate(osThread(gimbalTask), NULL);
      		osDelay(10);
			/* ------ �������� ------- */
			osThreadDef(chassisTask, StartChassisTask, osPriorityNormal, 0, CHASSIS_HEAP_SIZE);
      startChassisTaskHandle = osThreadCreate(osThread(chassisTask), NULL);
      /* ------ ���ݷ����������� ------- */
			osThreadDef(txTask, StartTxTask, osPriorityHigh, 0, TX_HEAP_SIZE);
      startTxTaskHandle = osThreadCreate(osThread(txTask), NULL);
       ProgressBarLed(LED_GPIO, 500);
      enble_s =1;
      			/* -------- ����ʾ���� --------- */
			osThreadDef(FaulTask, StartFaulTask, osPriorityNormal, 0,LED_TIP_HEAP_SIZE);
      startFaulTaskHandle = osThreadCreate(osThread(FaulTask), NULL);
      /* -------- ϵͳģ���Լ� --------- */
        SystemSelfChecking();
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
				ParseData();
				osDelay(2);
		}
	}
	/**
	* @Data    2019-01-16 18:27
	* @brief   ����ʾ����
	* @param   argument: Not used
	* @retval  void
	*/
	void StartFaulTask(void const *argument)
	{
        		HAL_TIM_PWM_Start(BUZZER_TIM,FRICTIONGEAR_1);
		for(;;)
		{
      if(enble_s ==1)
      {
        OffLineTip();
      }
			osDelay(TIP_BASE_TIME);
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
    const dbusStruct* pRc_t;
		const pcDataStruct* pPc_t;
    pRc_t = GetRcStructAddr();
    pPc_t = GetPcDataStructAddr();//С��������
    ChassisInit(CHASSIS_CAN,pRc_t,pPc_t);
		for (;;)
		{
        ChassisControl();
			  osDelay(5);
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
    const dbusStruct* pRc_t;
    const pcDataStruct* pPc_t;
    pRc_t = GetRcStructAddr();//ң������
    pPc_t = GetPcDataStructAddr();//С��������
    GimbalStructInit(pRc_t,pPc_t); 
		for (;;)
		{  
				GimbalAutoControl();
				osDelay(5);
		}
	}
/**
	* @Data    2019-03-20 20:01
	* @brief   �û����ݷ�������
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
  
