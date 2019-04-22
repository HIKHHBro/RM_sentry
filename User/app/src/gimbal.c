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
  RM6623Struct yaw_t;//yaw����
	postionPidStruct yawOuterLoopPid_t;//yaw ����⻷pid
	speedPidStruct yawInnerLoopPid_t;//yaw ����ڻ�pid
	GM6020Struct pitch_t;//pitch����
	postionPidStruct pitchOuterLoopPid_t;//pitch ����⻷pid
	speedPidStruct pitchInnerLoopPid_t;//pitch ����ڻ�pid
  
  static gy955Struct gimbalGyroByCan_t;
/* -------------- ������ ----------------- */
		osThreadId startRammerTaskHandle; 
/* -------------- �ⲿ���� ----------------- */
		extern osThreadId startGimbalTaskHandle;
/* -------------- ���Ͷ��� ----------------- */
  xQueueHandle gimbal_queue;
/* ----------------- �����Ӻ��� -------------------- */
	void StartRammerTask(void const *argument);
        
 /* -------------- �����ʱ�� ----------------- */       
osTimerId gimbalGetStatusTimerHandle;
void GimbalGetStatusCallback(void const * argument);
/* ------------ ��¼��̨��ʷ״̬���� ---------------- */
Int16Queue yawStatusQu;
Int16Queue pitchStatusQu;
/* ----------------- ��ʱ���� -------------------- */
    int16_t taddd =0;
    int16_t xianfuweizhi =3000;
    int16_t xianfusudu = 5000;//STUCK_BULLET_THRE
    int16_t yawxianfu = 1900;
		int16_t pitchxianfu =10000;
		int16_t sudupitchxianfu =10000;
	/**
	* @Data    2019-01-27 17:09
	* @brief   ��̨�ṹ���ʼ��
	* @param  CAN_HandleTypeDef* hcanx��x=1,2��
	* @retval  void
	*/
	void GimbalStructInit(const dbusStruct* pRc_t,const pcDataStruct* pPc_t)
	{
		gimbal_t.pRc_t = pRc_t;//ң�����ݵ�ַ
    gimbal_t.pPc_t = pPc_t;//С�������ݵ�ַ
    gimbal_t.status = MOD_READ;
    gimbal_t.prammer_t =RammerInit();
		gimbal_t.pYaw_t = YawInit();
		gimbal_t.pPitch_t = PitchInit();
    gimbal_t.pGyro_t = &gimbalGyroByCan_t;
  /* ------ ����Ħ���� ------- */
		BrushlessMotorInit();
//    UserCanConfig(GIMBAL_CAN);
  /* ------ ������̨���Ͷ��� ------- */
	  gimbal_queue	= xQueueCreate(QUEUE_LEN,QUEUE_SIZE);//һ��Ҫ����֮ǰ��������
  /* ------ ������̨��ʷ״̬���� ------- */
    Int16QueueCreate(&yawStatusQu,QUEUE_YAW_STATUS_SIZE);
    Int16QueueCreate(&pitchStatusQu,QUEUE_PITCH_STATUS_SIZE);
    
    //����----------------------
   // UserCanConfig(GY955_CAN);
    //--------------------
    
    
 	/* ------ ���ó�ʼ����־λ ------- */
		SET_BIT(gimbal_t.status,INIT_OK);
	/* ------ �������񣬵ȴ���ʼ�� ------- */
		vTaskSuspend(startGimbalTaskHandle);
	/* ------ ���û����˳�ʼ��״̬ ------- */
		SetGimBalInitStatus();
 /* ------ ������������ ------- */
    osThreadDef(startRammerTask,StartRammerTask,osPriorityNormal,0,RAMMER_HEAP_SIZE);
    startRammerTaskHandle = osThreadCreate(osThread(startRammerTask),NULL);
    osStatus timerresult = osOK;
    osTimerDef(GimbalGetStatusTimer, GimbalGetStatusCallback);
    gimbalGetStatusTimerHandle = osTimerCreate(osTimer(GimbalGetStatusTimer), osTimerPeriodic, NULL);
    timerresult = osTimerStart(gimbalGetStatusTimerHandle,MEMORY_SPAN);
    if(timerresult !=osOK)
    {
      //��ӱ������
    }
 		//vTaskSuspend(startGimbalTaskHandle);
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
     gimbal_t.prammer_t->real_angle = RatiometricConversion(gimbal_t.prammer_t->tem_angle,7000,36,&(gimbal_t.prammer_t->last_real),&(gimbal_t.prammer_t->coefficient),gimbal_t.status);
  #ifdef ANTI_CLOCK_WISE  //��ʱ��Ϊ������
        AntiRM2006ParseData(gimbal_t.prammer_t,data);
  #endif
				break;
			case YAW_RX_ID:
				RM6623ParseData(gimbal_t.pYaw_t,data);
				gimbal_t.pYaw_t->real_angle = NoRatiometricConversion(gimbal_t.pYaw_t->tem_angle,5000,5,&(gimbal_t.pYaw_t->last_real),&(gimbal_t.pYaw_t->coefficient),gimbal_t.status);
				break;
			case PITCH_RX_ID:
        GM6020ParseData(gimbal_t.pPitch_t,data);
				break;
     case BIN_GE_GYRO_CAN_RX_ID:
        BingeGyroByCan(gimbal_t.pGyro_t,data);
      break;
			default:
				break;
		}
    SET_BIT(gimbal_t.status,RX_OK);
	}
////---------------------��ʱ
//void GyroParse(uint32_t id,uint8_t *data)
//{
//  if(BIN_GE_GYRO_CAN_RX_ID == id)
//  {
//     BingeGyroByCan(gimbal_t.pGyro_t,data);
//  }   
//}
 //-----------------------��ʱ
/**
	* @Data    2019-02-15 15:10
	* @brief   ��̨���ݷ���
	* @param   void
	* @retval  void
	*/
	HAL_StatusTypeDef GimbalCanTx(int16_t yaw,int16_t pitch,int16_t rammer)
	{
		uint8_t s[8];
    memset(s,0,8);
    s[0] = (uint8_t)(yaw>>8) & 0xFF;
    s[1] = (uint8_t)yaw;
    s[2] = (uint8_t)(pitch>>8) & 0xFF;
    s[3] = (uint8_t)pitch;
		s[4] = (uint8_t)(rammer>>8) & 0xFF;
		s[5] = (uint8_t)rammer;
   	xQueueSendToBack(gimbal_queue,s,0);
		return HAL_OK;
	}
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
		/* -------------- ������������ ----------------- */
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
        gimbal_t.prammer_t->pspeedPid_t->limiting=RAMMER_LIMIMT_CUT;
		/* -------------- yaw������������ ----------------- */
		gimbal_t.pYaw_t->target = gimbal_t.pYaw_t->real_angle;//��������Ŀ��ֵ
						/* ------ �⻷pid���� ------- */
				gimbal_t.pYaw_t->ppostionPid_t->kp = 3.5;
				gimbal_t.pYaw_t->ppostionPid_t->kd = 16;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//������
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid���
			/* ------ �ڻ�pid���� ------- */
				gimbal_t.pYaw_t->pspeedPid_t->kp = -7;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = -0.035;
				gimbal_t.pYaw_t->pspeedPid_t->error = 0;
				gimbal_t.pYaw_t->pspeedPid_t->last_error = 0;//�ϴ����
				gimbal_t.pYaw_t->pspeedPid_t->before_last_error = 0;//���ϴ����
				gimbal_t.pYaw_t->pspeedPid_t->integral_er = 0;//������
				gimbal_t.pYaw_t->pspeedPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->pspeedPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->pspeedPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->pspeedPid_t->pid_out = 0;//pid���
        gimbal_t.pYaw_t->pspeedPid_t->limiting=YAW_LIMIMT_CUT;
        gimbal_t.pYaw_t->pspeedPid_t->motor_lim = RM6623_LIM;
		/* -------------- pitch�� ----------------- */
				gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;//��������Ŀ��ֵ
						/* ------ �⻷pid���� ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 0;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 0;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//�����޷�
        gimbal_t.pPitch_t->ppostionPid_t->integral_threshold =300;
			/* ------ �ڻ�pid���� ------- */
                  /* ------ �ڻ�pid���� ------- */
        gimbal_t.pPitch_t->pspeedPid_t->kd =0;
        gimbal_t.pPitch_t->pspeedPid_t->ki =0.35;//45;
        gimbal_t.pPitch_t->pspeedPid_t->kp =236;//290;
        gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
				gimbal_t.pPitch_t->pspeedPid_t->error = 0;
				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//�ϴ����
				gimbal_t.pPitch_t->pspeedPid_t->before_last_error = 0;//���ϴ����
				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->pspeedPid_t->motor_lim =PITCH_LIMIMT_CUT;
        gimbal_t.pPitch_t->pspeedPid_t->limiting=PITCH_LIMIMT_CUT;

//ceshi----------------
//				gimbal_t.pPitch_t->pspeedPid_t->kp = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->kd = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->ki = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->error = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//�ϴ����
//				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//������
//				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p���
//				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i���
//				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k���
//				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid���
//        gimbal_t.pPitch_t->pspeedPid_t->integral_limint = LINT_LIMINT;//�����޷�
//        gimbal_t.pPitch_t->pspeedPid_t->integral_threshold =300;
//ceshi--------------
     	/* ------ ����������־λ ------- */  
        SET_BIT(gimbal_t.status,START_OK);  

	 }
  /**
  * @Data    2019-03-19 23:28
  * @brief   ����PID����
  * @param   void
  * @retval  void
  */
  int16_t RammerPidControl(int16_t rammer)
  {
    int16_t temp_pid_out;
    /* -------- �⻷ --------- */
    gimbal_t.prammer_t->ppostionPid_t->error = GIMBAL_CAL_ERROR(rammer,gimbal_t.prammer_t->real_angle);
    gimbal_t.prammer_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.prammer_t->ppostionPid_t,gimbal_t.prammer_t->ppostionPid_t->error);
    gimbal_t.prammer_t->ppostionPid_t->pid_out = MAX(gimbal_t.prammer_t->ppostionPid_t->pid_out,xianfuweizhi); //������ֵ
    gimbal_t.prammer_t->ppostionPid_t->pid_out = MIN(gimbal_t.prammer_t->ppostionPid_t->pid_out,-xianfuweizhi); //����Сֵ
    /* -------- �ڻ� --------- */
    gimbal_t.prammer_t->pspeedPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.prammer_t->ppostionPid_t->pid_out,gimbal_t.prammer_t->real_speed);
		gimbal_t.prammer_t->pspeedPid_t->pid_out = SpeedPid(gimbal_t.prammer_t->pspeedPid_t,gimbal_t.prammer_t->pspeedPid_t->error);
  	temp_pid_out = MAX(gimbal_t.prammer_t->pspeedPid_t->pid_out,xianfusudu); //������ֵ
    temp_pid_out = MIN(gimbal_t.prammer_t->pspeedPid_t->pid_out,-xianfusudu); //����Сֵ
    return temp_pid_out;
  }
  /**
  * @Data    2019-03-21 01:39
  * @brief   yaw ��pid����
  * @param   void
  * @retval  void
  */
   int16_t __temp_pid_out;
   int16_t __temp_pid_out1;
  int16_t YawPidControl(int16_t yaw)
  {
     /* -------- �⻷ --------- */
    gimbal_t.pYaw_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pYaw_t->ppostionPid_t,yaw);
    __temp_pid_out1 = gimbal_t.pYaw_t->ppostionPid_t->pid_out;
    __temp_pid_out = MAX(gimbal_t.pYaw_t->ppostionPid_t->pid_out,yawxianfu);
    __temp_pid_out = MIN( gimbal_t.pYaw_t->ppostionPid_t->pid_out,-yawxianfu);
      /* -------- �ڻ� --------- */
     gimbal_t.pYaw_t->pspeedPid_t->error= PostionOutToSpeedOut(gimbal_t.pYaw_t->ppostionPid_t->pid_out) - gimbal_t.pGyro_t->Gyrz;
    __temp_pid_out = SpeedPid(gimbal_t.pYaw_t->pspeedPid_t, gimbal_t.pYaw_t->pspeedPid_t->error);
    return __temp_pid_out;
  }
  /**
  * @Data    2019-03-21 01:40
  * @brief   pitch ��pid����
  * @param   void
  * @retval  void
  */
  int16_t ssssssssssssssssssssss;
  int16_t PitchPidControl(int16_t pitch)
  {
    int16_t temp_pid_out;
       /* -------- �⻷ --------- */
    gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pPitch_t->ppostionPid_t,pitch);
    temp_pid_out = MAX(gimbal_t.pPitch_t->ppostionPid_t->pid_out,pitchxianfu);
    temp_pid_out = MIN( gimbal_t.pPitch_t->ppostionPid_t->pid_out,-pitchxianfu);
       /* -------- �ڻ� --------- */
		gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionOutToSpeedOut(gimbal_t.pPitch_t->ppostionPid_t->pid_out);
    gimbal_t.pPitch_t->pspeedPid_t->error = gimbal_t.pPitch_t->ppostionPid_t->pid_out - gimbal_t.pPitch_t->real_speed;//GIMBAL_CAL_ERROR(gimbal_t.pPitch_t->ppostionPid_t->pid_out,gimbal_t.pPitch_t->real_speed);
		temp_pid_out = SpeedPid(gimbal_t.pPitch_t->pspeedPid_t,gimbal_t.pPitch_t->pspeedPid_t->error);
  //	temp_pid_out = MAX(gimbal_t.pPitch_t->pspeedPid_t->pid_out,sudupitchxianfu); //������ֵ
   // temp_pid_out = MIN(gimbal_t.pPitch_t->pspeedPid_t->pid_out,-sudupitchxianfu); //����Сֵ
    return temp_pid_out;
  }
/**
	* @Data    2019-04-16 22:29
	* @brief   λ��pid���ӳ����ٶ�
	* @param   void
	* @retval  void
	*/
	int16_t PostionOutToSpeedOut(int16_t pid_out)
	{
		return (int16_t)(pid_out *0.1);
	}
/**
	* @Data    2019-03-20 21:27
	* @brief   yaw������ʼ��
	* @param   void
	* @retval  void
	*/
	RM6623Struct* YawInit(void)
	{
	yaw_t.id = YAW_RX_ID ;//���can�� ip
	yaw_t.target = 0 ;		 //Ŀ��ֵ
	yaw_t.tem_target = 0 ;//��ʱĿ��ֵ
	yaw_t.real_current = 0 ; //��ʵ����
	yaw_t.real_angle = 0 ;//��ʵ�Ƕ�
	yaw_t.tem_angle = 0 ;//��ʱ�Ƕ�
	yaw_t.zero = 0 ;			 //������
	yaw_t.Percentage = 0 ;//ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
	yaw_t.thresholds = 0 ; //�����ת��ֵ
  yaw_t.error = 0 ;//��ǰ���
  yaw_t.last_real = 0 ;
  yaw_t.coefficient = 0 ;
		/* ------ �⻷pid��ַ ------- */
  yaw_t.ppostionPid_t = &yawOuterLoopPid_t;
    yawOuterLoopPid_t.motor_lim = RM6623_LIM;//6623��������Χ
			/* ------ �ڻ�pid��ַ ------- */
	yaw_t.pspeedPid_t = &yawInnerLoopPid_t ;
    yawInnerLoopPid_t.motor_lim = RM6623_LIM;//6623��������Χ
			return &yaw_t;
	}
	/**
	* @Data    2019-03-20 21:27
	* @brief   pitch������ʼ��
	* @param   void
	* @retval  void
	*/
	GM6020Struct* PitchInit(void)
	{
	pitch_t.id = YAW_RX_ID ;//���can�� ip
	pitch_t.target = 0 ;		 //Ŀ��ֵ
	pitch_t.tem_target = 0 ;//��ʱĿ��ֵ
	pitch_t.real_current = 0 ; //��ʵ����
	pitch_t.real_angle = 0 ;//��ʵ�Ƕ�
	pitch_t.tem_angle = 0 ;//��ʱ�Ƕ�
	pitch_t.zero = 0 ;			 //������
	pitch_t.Percentage = 0 ;//ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
	pitch_t.thresholds = 0 ; //�����ת��ֵ
  pitch_t.error = 0 ;//��ǰ���
  pitch_t.last_real = 0 ;
  pitch_t.coefficient = 0 ;
		/* ------ �⻷pid��ַ ------- */
  pitch_t.ppostionPid_t = &pitchOuterLoopPid_t ;
    pitchOuterLoopPid_t.motor_lim = RM6020_LIM;//6020��������Χ
			/* ------ �ڻ�pid��ַ ------- */
	pitch_t.pspeedPid_t = &pitchInnerLoopPid_t ;
    pitchInnerLoopPid_t.motor_lim = RM6020_LIM;//6020��������Χ
    
//            /* ------ �ڻ�pid���� ------- */
//        gimbal_t.pPitch_t->pspeedPid_t->kd =0;
//        gimbal_t.pPitch_t->pspeedPid_t->ki =0.35;//45;
//        gimbal_t.pPitch_t->pspeedPid_t->kp =236;//290;
//        gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
			return &pitch_t;
	}


///**
//* @Data    2019-03-21 00:46
//* @brief   ��������ת��//������
//* @param   void
//* @retval  void
//*/
/**
* @Data    2019-03-21 00:46
* @brief   ����pid����
* @param   void
* @retval  void
*/
/**
* @Data    2019-03-21 00:46
* @brief   ��̨Ԥ��//������
* @param   void
* @retval  void
*/
/**
* @Data    2019-03-21 00:46
* @brief   ����ģʽpid����
* @param   void
* @retval  void
*/
float kp_test=2;
float ki_test=0;
float kd_test=5;
void SetPcControlPID(void)
{
				gimbal_t.pYaw_t->ppostionPid_t->kp = 3.5;
				gimbal_t.pYaw_t->ppostionPid_t->kd = 16;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//������
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid���
       gimbal_t.pYaw_t->ppostionPid_t->integral_threshold =15;
  
  
  				gimbal_t.pYaw_t->pspeedPid_t->kp = -7;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = -0.035;
  						/* ------ �⻷pid���� ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = kp_test;
				gimbal_t.pPitch_t->ppostionPid_t->kd = kd_test;
				gimbal_t.pPitch_t->ppostionPid_t->ki = ki_test;
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//�����޷�
        gimbal_t.pPitch_t->ppostionPid_t->integral_threshold =300;
        
         gimbal_t.pPitch_t->pspeedPid_t->kd =0;
        gimbal_t.pPitch_t->pspeedPid_t->ki =0.35;//45;
        gimbal_t.pPitch_t->pspeedPid_t->kp =236;//290
         //gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
}
/**
* @Data    2019-03-21 00:46
* @brief   ��λpid����
* @param   void
* @retval  void
*/
void SetGeneralMode(void)
{
    		gimbal_t.pYaw_t->ppostionPid_t->kp = 1.85;
				gimbal_t.pYaw_t->ppostionPid_t->kd = 2;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//������
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid���
  
    		gimbal_t.pYaw_t->pspeedPid_t->kp = -7;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = -0.035;
  
  						/* ------ �⻷pid���� ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 3;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 3;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//�����޷�
        gimbal_t.pPitch_t->ppostionPid_t->integral_threshold = 500;
        gimbal_t.pPitch_t->ppostionPid_t->kp_separatecmd = 0;
        
        
        gimbal_t.pPitch_t->pspeedPid_t->kd =0;
        gimbal_t.pPitch_t->pspeedPid_t->ki =0.35;//45;
        gimbal_t.pPitch_t->pspeedPid_t->kp =236;//290
  //gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
//        //ceshi----------------
//        gimbal_t.pPitch_t->pspeedPid_t->kp = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->kd = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->ki = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->error = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//�ϴ����
//				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//������
//				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p���
//				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i���
//				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k���
//				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid���
//        gimbal_t.pPitch_t->pspeedPid_t->integral_limint = LINT_LIMINT;//�����޷�
//        gimbal_t.pPitch_t->pspeedPid_t->integral_threshold = 500;
//        gimbal_t.pPitch_t->pspeedPid_t->kp_separatecmd =1;
        //ceshi-------------
        
 }
/**
	* @Data    2019-03-27 15:16
	* @brief   ������Ұ���Ƽ���
	* @param   void
	* @retval  void
	*/
	void GetTrend(int16_t* yaw_tr,int16_t* pitch_tr)
	{
		int16_t yawtrend[8];
		int16_t pitchtrend[8];
		for(uint8_t i=0;i<8;i++)
		{
	   	yawStatusQu.deQueue(&yawStatusQu,&yawtrend[i]);
	   	pitchStatusQu.deQueue(&pitchStatusQu,&pitchtrend[i]);
			*yaw_tr += yawtrend[i];
			*pitch_tr += pitchtrend[i];
		}
		*yaw_tr = (int16_t)(*yaw_tr/8.0);
		*pitch_tr = (int16_t)(*pitch_tr/8.0);
	}
/**
  * @Data    2019-03-27 10:16
  * @brief   ��̨�����ʱ�����Ӻ���
  * @param   void
  * @retval  void
  */
 void GimbalGetStatusCallback(void const * argument)
  {
    yawStatusQu.enQueue(&yawStatusQu,gimbal_t.pYaw_t->real_angle);
    pitchStatusQu.enQueue(&pitchStatusQu,gimbal_t.pPitch_t->real_angle);
  }


  
	
/*-----------------------------------file of end------------------------------*/
