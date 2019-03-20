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
	|--FileName    : gimbal.c                                              
	|--Version     : v2.0                                                          
	|--Author      : ����ƽͷ��                                                     
	|--Date        : 2019-01-26             
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
#include "gimbal.h" 
/* -------------- �ṹ������ ----------------- */
	gimbalStruct gimbal_t;//��̨�ṹ��
/* -------------- ������ ----------------- */
		osThreadId startRammerTaskHandle; 
/* -------------- �ⲿ���� ----------------- */
		extern osThreadId startGimbalTaskHandle;
/* ----------------- �����Ӻ��� -------------------- */
/* ----------------- ��ʱ���� -------------------- */
		int16_t pid_out = 0;
    int16_t taddd =0;
    int16_t xianfuweizhi =3000;
    int16_t xianfusudu = 5000;//STUCK_BULLET_THRE
	void StartRammerTask(void const *argument);
	/**
	* @Data    2019-01-27 17:09
	* @brief   ��̨�ṹ���ʼ��
	* @param  CAN_HandleTypeDef* hcanx��x=1,2��
	* @retval  void
	*/
	void GimbalStructInit(const dbusStruct* pRc_t)
	{
		gimbal_t.pRc_t = pRc_t;
    gimbal_t.status = 0;
    gimbal_t.prammer_t =RammerInit();
  /* ------ ����Ħ���� ------- */
		BrushlessMotorInit();
 	/* ------ ���ó�ʼ����־λ ------- */
		SET_BIT(gimbal_t.status,INIT_OK);
	/* ------ �������񣬵ȴ���ʼ�� ------- */
		vTaskSuspend(startGimbalTaskHandle);
	/* ------ ���û����˳�ʼ��״̬ ------- */
		SetGimBalInitStatus();
 /* ------ ������������ ------- */
	osThreadDef(startRammerTask,StartRammerTask,osPriorityNormal,0,RAMMER_HEAP_SIZE);
	startRammerTaskHandle = osThreadCreate(osThread(startRammerTask),NULL);
	}
/**
	* @Data    2019-01-28 11:40
	* @brief   ��̨���ݽ���
	* @param   void
	* @retval  void
	*/
	void GimbalParseDate(uint32_t id,uint8_t *data)
	{
		switch (id)
		{
			case RAMMER_RX_ID:
				RM2006ParseData(gimbal_t.prammer_t,data);
     gimbal_t.prammer_t->real_angle = RatiometricConversion(gimbal_t.prammer_t->tem_angle,7000,36,gimbal_t.status);
  #ifdef ANTI_CLOCK_WISE  //��ʱ��Ϊ������
        AntiRM2006ParseData(gimbal_t.prammer_t,data);
  #endif
				break;
			case YAW_RX_ID:
				// RM6623ParseData(gimbal_t.pYaw_t,data);
        // /* -------- ����ת�� --------- */
        // gimbal_t.pYaw_t->real_angle = RatiometricConversion  \
        // (gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds,gimbal_t.pYaw_t->Percentage);
        /* -------- ���㴦�� --------- */
        //  zeroArgument(gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds); 
				break;
			case PITCH_RX_ID:
				// RM6623ParseData(gimbal_t.pPitch_t,data);
        /* -------- ���㴦�� --------- */
        //  zeroArgument(gimbal_t.pPitch_t->real_angle,gimbal_t.pPitch_t->thresholds); 
				break;
		
			default:
				break;
		}
    SET_BIT(gimbal_t.status,RX_OK);
	}
/**
	* @Data    2019-02-14 21:01
	* @brief   ��̨����
	* @param   void
	* @retval  void
	*/
	void GimbalControl(void)
	{

    pid_out = RammerPidControl();
    GimbalCanTx(0,0,pid_out);

//		GimbalCanTx(pid_out,0);
	}
/**
	* @Data    2019-02-15 15:10
	* @brief   ��̨���ݷ���
	* @param   void
	* @retval  void
	*/
	HAL_StatusTypeDef GimbalCanTx(int16_t yaw,int16_t pitch,int16_t rammer)
	{
		uint8_t s[8]={0};
    s[0] = (uint8_t)(yaw>>8);
    s[1] = (uint8_t)yaw;
    s[2] = (uint8_t)(pitch>>8);
    s[3] = (uint8_t)pitch;
		s[4] = (uint8_t)(rammer>>8);
		s[5] = (uint8_t)rammer;
		return(CanTxMsg(GIMBAL_CAN,GIMBAL_CAN_TX_ID,s));
	}
// /*---------------------------------80�ַ�����-----------------------------------*/
//   /**
//   * @Data    2019-02-24 23:55
//   * @brief   С�������ݽ���
//   * @param   void
//   * @retval  void
//   */
//   HAL_StatusTypeDef RxPCMsg(void)
//   {
//     return (HAL_UART_Receive(gimbal_t.huartx,pc_data,3,1));
//   }
  /**
  * @Data    2019-03-15 23:14
  * @brief   ��ȡ��̨״̬
  * @param   void
  * @retval  void
  */
  uint32_t GetGimbalStatus(void)
  {
    return gimbal_t.status;
  }
  /**
  * @Data    2019-03-13 03:48
  * @brief   ��ȡ��̨�ṹ���ַ �ɶ�д�������ҵ���
  * @param   void
  * @retval  void
  */
  gimbalStruct *RWGetgimbalStructAddr(void)
  {
    return &gimbal_t;
  }
/**
	* @Data    2019-03-16 18:04
	* @brief   �����������
	* @param   void
	* @retval  void
	*/
	void StartRammerTask(void const *argument)
	{
		for(;;)
		{
      RammerControl();
			osDelay(10);
		}
	}
 /**
	 * @Data    2019-03-19 17:58
	 * @brief   ��̨��ʼ��״̬����
	 * @param   void
	 * @retval  void
	 */
	 void SetGimBalInitStatus(void)
	 {
		   gimbal_t.prammer_t->target =  gimbal_t.prammer_t->real_angle; //Ŀ��ֵ
			/* ------ �⻷pid���� ------- */
				gimbal_t.prammer_t->ppostionPid_t->kp = 4;
				gimbal_t.prammer_t->ppostionPid_t->kd = 0;
				gimbal_t.prammer_t->ppostionPid_t->ki = 0;
				gimbal_t.prammer_t->ppostionPid_t->error = 0;
				gimbal_t.prammer_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.prammer_t->ppostionPid_t->integral_er = 0;//������
				gimbal_t.prammer_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.prammer_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.prammer_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.prammer_t->ppostionPid_t->pid_out = 0;//pid���
			/* ------ �ڻ�pid���� ------- */
				gimbal_t.prammer_t->pspeedPid_t->kp = 0.7;
				gimbal_t.prammer_t->pspeedPid_t->kd = 0.07;
				gimbal_t.prammer_t->pspeedPid_t->ki = 0.01;
				gimbal_t.prammer_t->pspeedPid_t->error = 0;
				gimbal_t.prammer_t->pspeedPid_t->last_error = 0;//�ϴ����
				gimbal_t.prammer_t->pspeedPid_t->before_last_error = 0;//���ϴ����
				gimbal_t.prammer_t->pspeedPid_t->integral_er = 0;//������
				gimbal_t.prammer_t->pspeedPid_t->pout = 0;//p���
				gimbal_t.prammer_t->pspeedPid_t->iout = 0;//i���
				gimbal_t.prammer_t->pspeedPid_t->dout = 0;//k���
				gimbal_t.prammer_t->pspeedPid_t->pid_out = 0;//pid���
        gimbal_t.prammer_t->pspeedPid_t->limiting=LIMIMT_CUT;
     	/* ------ ����������־λ ------- */  
        SET_BIT(gimbal_t.status,START_OK);  
	 }
  /**
  * @Data    2019-03-19 23:28
  * @brief   ����PID����
  * @param   void
  * @retval  void
  */
  int16_t RammerPidControl(void)
  {
    int16_t temp_pid_out;
    gimbal_t.prammer_t->ppostionPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.prammer_t->target,gimbal_t.prammer_t->real_angle);
    gimbal_t.prammer_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.prammer_t->ppostionPid_t,gimbal_t.prammer_t->ppostionPid_t->error);
    gimbal_t.prammer_t->ppostionPid_t->pid_out = MAX(gimbal_t.prammer_t->ppostionPid_t->pid_out,xianfuweizhi); //������ֵ
    gimbal_t.prammer_t->ppostionPid_t->pid_out = MIN(gimbal_t.prammer_t->ppostionPid_t->pid_out,-xianfuweizhi); //����Сֵ
    
    gimbal_t.prammer_t->pspeedPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.prammer_t->ppostionPid_t->pid_out,gimbal_t.prammer_t->real_speed);
		gimbal_t.prammer_t->pspeedPid_t->pid_out = SpeedPid(gimbal_t.prammer_t->pspeedPid_t,gimbal_t.prammer_t->pspeedPid_t->error);
  	temp_pid_out = MAX(gimbal_t.prammer_t->pspeedPid_t->pid_out,xianfusudu); //������ֵ
    temp_pid_out = MIN(gimbal_t.prammer_t->pspeedPid_t->pid_out,-xianfusudu); //����Сֵ
    return temp_pid_out;
  }
/*-----------------------------------file of end------------------------------*/
