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
/* -------------- ģ���Զ����־λ�� ----------------- */
#define  SCAN_MODE_RUNING           (0x80000000U)//ɨ��ģʽִ��
#define  PC_SHOOT_MODE_RUNING       (0x40000000U)//������ģʽִ��
#define  RC_MODE_RUNING             (0x20000000U)//ң��ģʽִ��
#define  FRAME_DROP_BUFFER_RUNING   (0x10000000U)//��֡����ִ��
#define  DISABLE_MOD_RUNNING        (0xF0000000U)//ʹ������ģ��
#define  SCAN_MODE_READ             (0x08000000U)//ɨ��ģʽ׼������
#define  GIMBAL_PC_SHOOT_MODE_READ  (0x04000000U)//������ģʽ׼������
#define  RC_MODE_READ               (0x02000000U)//ң��ģʽ׼������
#define  FRAME_DROP_BUFFER_READ     (0x01000000U)//��֡����׼������
#define  JUDGE_READ                 (0x0F000000U)//�жϾ���
#define  DISABLE_MOD_READ           (0x0F000000U)//ʹ������ģ��
#define  DISABLE_GIMBAL             (0xFF000000U)//ʧ����̨
#define  DELEC_USER_MODE            (0x00FFFFFFU)//����û��Զ����־λ
#define SET_RUNING_STATUS(_status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,DISABLE_MOD_RUNNING);	\
					SET_BIT(gimbal_t.status,_status_);							\
				}while(0)	
#define SET_READ_STATUS(__status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,DISABLE_MOD_READ);	\
					SET_BIT(gimbal_t.status,__status_);							\
				}while(0)																					\
/* -------------- ˽�к� ----------------- */
	#define QUEUE_LEN      5U//���Ϊ5
	#define  QUEUE_SIZE    8U//����Ϊ5;
  #define  MEMORY_SPAN   (50)          //   1000ms = 50*20
  #define  QUEUE_YAW_STATUS_SIZE      20//����䳤��Ϊ1s ����Ϊ50
 #define   QUEUE_PITCH_STATUS_SIZE    20//����10������䳤��Ϊ1s
/* -------------- �ṹ������ ----------------- */
	gimbalStruct gimbal_t;//��̨�ṹ��
  RM6623Struct yaw_t;//yaw����
	postionPidStruct yawOuterLoopPid_t;//yaw ����⻷pid
	speedPidStruct yawInnerLoopPid_t;//yaw ����ڻ�pid
	GM6020Struct pitch_t;//pitch����
	postionPidStruct pitchOuterLoopPid_t;//pitch ����⻷pid
	speedPidStruct pitchInnerLoopPid_t;//pitch ����ڻ�pid
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
  /* ------ ����Ħ���� ------- */
		BrushlessMotorInit();
  /* ------ ������̨���Ͷ��� ------- */
	  gimbal_queue	= xQueueCreate(QUEUE_LEN,QUEUE_SIZE);//һ��Ҫ����֮ǰ��������
  /* ------ ������̨��ʷ״̬���� ------- */
    Int16QueueCreate(&yawStatusQu,QUEUE_YAW_STATUS_SIZE);
    Int16QueueCreate(&pitchStatusQu,QUEUE_PITCH_STATUS_SIZE);
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
  		int16_t yaw=0;
    int16_t pitch=0;
    int16_t rammer=0;
    uint32_t commot =0;
	void GimbalAutoControl(void)
	{
    SET_BIT(gimbal_t.status,RUNING_OK);
    commot = ControlDecision();//����ģʽ
    ControlSwitch(commot);//����ģʽ�л�
    yaw = YawPidControl(gimbal_t.pYaw_t->ppostionPid_t->error);
    pitch = PitchPidControl(gimbal_t.pPitch_t->ppostionPid_t->error);
    rammer = RammerPidControl(gimbal_t.prammer_t->target);
    GimbalCanTx(yaw,pitch,rammer);
	}
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
				gimbal_t.pYaw_t->ppostionPid_t->kp = -0.3;
				gimbal_t.pYaw_t->ppostionPid_t->kd = -1;
				gimbal_t.pYaw_t->ppostionPid_t->ki = -0.02;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//������
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid���
			/* ------ �ڻ�pid���� ------- */
				gimbal_t.pYaw_t->pspeedPid_t->kp = 0;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = 0;
				gimbal_t.pYaw_t->pspeedPid_t->error = 0;
				gimbal_t.pYaw_t->pspeedPid_t->last_error = 0;//�ϴ����
				gimbal_t.pYaw_t->pspeedPid_t->before_last_error = 0;//���ϴ����
				gimbal_t.pYaw_t->pspeedPid_t->integral_er = 0;//������
				gimbal_t.pYaw_t->pspeedPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->pspeedPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->pspeedPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->pspeedPid_t->pid_out = 0;//pid���
        gimbal_t.pYaw_t->pspeedPid_t->limiting=YAW_LIMIMT_CUT;
		/* -------------- pitch�� ----------------- */
				gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;//��������Ŀ��ֵ
						/* ------ �⻷pid���� ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 10;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 10;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0.1;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//�����޷�
			/* ------ �ڻ�pid���� ------- */
				gimbal_t.pPitch_t->pspeedPid_t->kp = 9;
				gimbal_t.pPitch_t->pspeedPid_t->kd = 30;
				gimbal_t.pPitch_t->pspeedPid_t->ki = 0.2;
				gimbal_t.pPitch_t->pspeedPid_t->error = 0;
				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//�ϴ����
				gimbal_t.pPitch_t->pspeedPid_t->before_last_error = 0;//���ϴ����
				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->pspeedPid_t->limiting=PITCH_LIMIMT_CUT;
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
//  /**
//  * @Data    2019-03-21 01:39
//  * @brief   yaw��pid����
//  * @param   void
//  * @retval  void
//  */
//  int16_t YawPidControl(int16_t yaw)
//  {
//    int16_t temp_pid_out;
//    gimbal_t.pYaw_t->ppostionPid_t->error = CalculateError((yaw),(gimbal_t.pYaw_t->real_angle),15000,(20480));//YAW_CAL_ERROR(yaw,gimbal_t.pYaw_t->real_angle);
//    gimbal_t.pYaw_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pYaw_t->ppostionPid_t,gimbal_t.pYaw_t->ppostionPid_t->error);
//    temp_pid_out = MAX(gimbal_t.pYaw_t->ppostionPid_t->pid_out,yawxianfu);
//    temp_pid_out = MIN( gimbal_t.pYaw_t->ppostionPid_t->pid_out,-yawxianfu);
//    return temp_pid_out;
//  }
//  /**
//  * @Data    2019-03-21 01:40
//  * @brief   pitch��pid����
//  * @param   void
//  * @retval  void
//  */
//  int16_t PitchPidControl(int16_t pitch)
//  {
//    int16_t temp_pid_out;
//       /* -------- �⻷ --------- */
//    gimbal_t.pPitch_t->ppostionPid_t->error = CalculateError((pitch),( gimbal_t.pPitch_t->real_angle),5500,(8192));//GIMBAL_CAL_ERROR(pitch,gimbal_t.pPitch_t->real_angle);
//    gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pPitch_t->ppostionPid_t,gimbal_t.pPitch_t->ppostionPid_t->error);
//    temp_pid_out = MAX(gimbal_t.pPitch_t->ppostionPid_t->pid_out,pitchxianfu);
//    temp_pid_out = MIN( gimbal_t.pPitch_t->ppostionPid_t->pid_out,-pitchxianfu);
////       /* -------- �ڻ� --------- */
////    gimbal_t.pPitch_t->pspeedPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.pPitch_t->ppostionPid_t->pid_out,gimbal_t.pPitch_t->real_speed);
////		gimbal_t.pPitch_t->pspeedPid_t->pid_out = SpeedPid(gimbal_t.pPitch_t->pspeedPid_t,gimbal_t.pPitch_t->pspeedPid_t->error);
////  	temp_pid_out = MAX(gimbal_t.pPitch_t->pspeedPid_t->pid_out,sudupitchxianfu); //������ֵ
////    temp_pid_out = MIN(gimbal_t.pPitch_t->pspeedPid_t->pid_out,-sudupitchxianfu); //����Сֵ
//    return temp_pid_out;
//  }

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
   
    gimbal_t.pYaw_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pYaw_t->ppostionPid_t,yaw);
    __temp_pid_out1 = gimbal_t.pYaw_t->ppostionPid_t->pid_out;
    __temp_pid_out = MAX(gimbal_t.pYaw_t->ppostionPid_t->pid_out,yawxianfu);
    __temp_pid_out = MIN( gimbal_t.pYaw_t->ppostionPid_t->pid_out,-yawxianfu);
    return __temp_pid_out;
  }
  /**
  * @Data    2019-03-21 01:40
  * @brief   pitch ��pid����
  * @param   void
  * @retval  void
  */
  int16_t PitchPidControl(int16_t pitch)
  {
    int16_t temp_pid_out;
       /* -------- �⻷ --------- */
    gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pPitch_t->ppostionPid_t,pitch);
    temp_pid_out = MAX(gimbal_t.pPitch_t->ppostionPid_t->pid_out,pitchxianfu);
    temp_pid_out = MIN( gimbal_t.pPitch_t->ppostionPid_t->pid_out,-pitchxianfu);
//       /* -------- �ڻ� --------- */
//    gimbal_t.pPitch_t->pspeedPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.pPitch_t->ppostionPid_t->pid_out,gimbal_t.pPitch_t->real_speed);
//		gimbal_t.pPitch_t->pspeedPid_t->pid_out = SpeedPid(gimbal_t.pPitch_t->pspeedPid_t,gimbal_t.pPitch_t->pspeedPid_t->error);
//  	temp_pid_out = MAX(gimbal_t.pPitch_t->pspeedPid_t->pid_out,sudupitchxianfu); //������ֵ
//    temp_pid_out = MIN(gimbal_t.pPitch_t->pspeedPid_t->pid_out,-sudupitchxianfu); //����Сֵ
    return temp_pid_out;
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
  yaw_t.ppostionPid_t = &yawOuterLoopPid_t ;
			/* ------ �ڻ�pid��ַ ------- */
	yaw_t.pspeedPid_t = &yawInnerLoopPid_t ;

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
			/* ------ �ڻ�pid��ַ ------- */
	pitch_t.pspeedPid_t = &pitchInnerLoopPid_t ;
			return &pitch_t;
	}
/**
* @Data    2019-03-20 21:27
* @brief   ��̨ɨ��̽��ģʽĿ��ֵ
* @param   void
* @retval  void
*/
uint8_t scflag = 0;
  int16_t pitchspeed_= 6;
    int16_t yawspeed_= 15;
  int16_t scok_up = 400;
  int16_t scok_down = 2000;
void ScanningToExplore(void)
{
  if((gimbal_t.status&SCAN_MODE_RUNING) != SCAN_MODE_RUNING)
  {
    //Ŀ��ֵ�л���״̬�л�
    gimbal_t.pitch_scan_target = gimbal_t.pPitch_t->real_angle;
    gimbal_t.yaw_scan_target = gimbal_t.pYaw_t->real_angle;
    gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;
    if((gimbal_t.pitch_scan_target) > 2000)
    {
      gimbal_t.pitch_scan_target = scok_down;
    }
    else if ((gimbal_t.pitch_scan_target )< 400) 
    {
      gimbal_t.pitch_scan_target = scok_up;
    }
    //���pc���Ʊ�־λ�����ñ�־λ
    CLEAR_BIT(gimbal_t.status,RC_MODE_RUNING);
     CLEAR_BIT(gimbal_t.status,PC_SHOOT_MODE_RUNING);
     SET_BIT(gimbal_t.status,SCAN_MODE_RUNING);
    SetGeneralMode();
    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_SPEED);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_SPEED);
  }
    if(((gimbal_t.pitch_scan_target < scok_down) || (gimbal_t.pitch_scan_target == scok_down)) && (scflag ==0) )
    {
      gimbal_t.pitch_scan_target -= pitchspeed_;
      if(gimbal_t.pitch_scan_target <= scok_up)
      {
        scflag =1;
        gimbal_t.pitch_scan_target = scok_up+1;
      }
    }
    else if(((gimbal_t.pitch_scan_target > scok_up) || (gimbal_t.pitch_scan_target == scok_up)) && (scflag ==1) )
    {
       gimbal_t.pitch_scan_target += pitchspeed_;
     if(gimbal_t.pitch_scan_target >= scok_down)
      {
        scflag =0;
         gimbal_t.pitch_scan_target = scok_down-1;
      }
    }
    gimbal_t.yaw_scan_target =  ((gimbal_t.yaw_scan_target+yawspeed_) % 20480);
     gimbal_t.pPitch_t->ppostionPid_t->error = CalculateError(gimbal_t.pitch_scan_target,( gimbal_t.pPitch_t->real_angle),5500,(8192));//������
     gimbal_t.pYaw_t->ppostionPid_t->error = CalculateError(( gimbal_t.yaw_scan_target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//������
    
}
/**
* @Data    2019-03-20 21:27
* @brief   ��̨����pc����ģʽĿ��ֵ
* @param   void
* @retval  void
*/
float pitchcin = 1;
float  yawcin = 1;
void PcControlMode(void)
{
  if((gimbal_t.status&PC_SHOOT_MODE_RUNING) != PC_SHOOT_MODE_RUNING)
  {
    //Ŀ��ֵ�л���״̬�л�
     CLEAR_BIT(gimbal_t.status,SCAN_MODE_RUNING);//���pc���Ʊ�־λ
     CLEAR_BIT(gimbal_t.status,RC_MODE_RUNING);
     SET_BIT(gimbal_t.status,PC_SHOOT_MODE_RUNING);
     gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;
    SetPcControlPID();
      __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_SPEED);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_SPEED);
//    if(gimbal_t.pPitch_t->ppostionPid_t->error - gimbal_t.pPc_t->pitch_target_angle)
  }
   gimbal_t.pPitch_t->ppostionPid_t->error  = (int16_t)(gimbal_t.pPc_t->pitch_target_angle * pitchcin);
  gimbal_t.pYaw_t->ppostionPid_t->error = (int16_t)(gimbal_t.pPc_t->yaw_target_angle * pitchcin);
}
/**
* @Data    2019-03-20 21:27
* @brief   ����Ȩ�л�
* @param   void
* @retval  void
*/
void ControlSwitch(uint32_t commot)
{
 switch (commot)
 {
   case SCAN_MODE_READ:
     ScanningToExplore();
     break;
   case GIMBAL_PC_SHOOT_MODE_READ:
     PcControlMode();
		  break;
	case FRAME_DROP_BUFFER_READ:
	   FrameDropBufferMode();
    break;
      case RC_MODE_READ:
        GimbalRcControlMode();
     break;
   default:
     GimbalDeinit();
	    break;
 }
}
/**
* @Data    2019-03-20 21:27
* @brief   ���߿����ж�
* @param   void
* @retval  void
*/
  uint32_t ControlDecision(void)
  {
    if(gimbal_t.pRc_t->switch_left ==2)
    {
      CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);
     return (gimbal_t.status & JUDGE_READ);
    }
    else if(gimbal_t.pRc_t->switch_right ==1)
    {
       switch (gimbal_t.pPc_t->commot)
       {
         case 0:
				 SET_READ_STATUS(FRAME_DROP_BUFFER_READ);
            //  SET_BIT(gimbal_t.status,SCAN_MODE_READ);
            //  CLEAR_BIT(gimbal_t.status,GIMBAL_PC_SHOOT_MODE_READ);
            //  CLEAR_BIT(gimbal_t.status,RC_MODE_READ);
           break;
         case 1:
				  SET_READ_STATUS(GIMBAL_PC_SHOOT_MODE_READ);
          //   SET_BIT(gimbal_t.status,GIMBAL_PC_SHOOT_MODE_READ);
          //  CLEAR_BIT(gimbal_t.status,SCAN_MODE_READ);
          //    CLEAR_BIT(gimbal_t.status,RC_MODE_READ);
           break;
					
         default:
            break;
       }
    }
    else if(gimbal_t.pRc_t->switch_right ==2)
    {
			 SET_READ_STATUS(RC_MODE_READ);
      // SET_BIT(gimbal_t.status,RC_MODE_READ);
      // CLEAR_BIT(gimbal_t.status,SCAN_MODE_READ);
      // CLEAR_BIT(gimbal_t.status,GIMBAL_PC_SHOOT_MODE_READ);
    }   
    return (gimbal_t.status & JUDGE_READ);
  }
 /**
* @Data    2019-03-20 21:27
* @brief   ң��ģʽ
* @param   void
* @retval  void
*/
    int16_t i = 0;
  float iii = 10;
   float yyyy = 0.1;
    int16_t rcscok_up = 400;
  int16_t rcscok_down = 3000;
  int16_t rctemp;
  int16_t seepdd =7;
void GimbalRcControlMode(void)
{
  if((gimbal_t.status&RC_MODE_RUNING) != RC_MODE_RUNING)
  {

     CLEAR_BIT(gimbal_t.status,PC_SHOOT_MODE_RUNING);//���pc���Ʊ�־λ
     CLEAR_BIT(gimbal_t.status,SCAN_MODE_RUNING);
     SET_BIT(gimbal_t.status,RC_MODE_RUNING);
        //Ŀ��ֵ�л���״̬�л�
    gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;
    gimbal_t.pYaw_t->target = gimbal_t.pYaw_t->real_angle;
    gimbal_t.prammer_t->target = gimbal_t.prammer_t->real_angle ;
    if(gimbal_t.pPitch_t->target > (rcscok_down-1))
    {
      gimbal_t.pPitch_t->target =(rcscok_down-1);
    }
    else if(gimbal_t.pPitch_t->target < (rcscok_up+1))
    {
      gimbal_t.pPitch_t->target =(rcscok_up+1);
    }
    gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;
    SetGeneralMode();
    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_SPEED);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_SPEED);
     HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
  }
  if(gimbal_t.pRc_t->ch2 >600)
  {
    SetRammerPID(seepdd);
  }
  else SetRammerPID(0);
                   HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
  rctemp =(int16_t)(gimbal_t.pRc_t->ch3 * iii);
  
        gimbal_t.pPitch_t->target -= (int16_t)(gimbal_t.pRc_t->ch4 * yyyy);
        gimbal_t.pPitch_t->target = MAX(gimbal_t.pPitch_t->target,rcscok_down);
        gimbal_t.pPitch_t->target = MIN(gimbal_t.pPitch_t->target,rcscok_up);
        
     gimbal_t.pPitch_t->ppostionPid_t->error =CalculateError(gimbal_t.pPitch_t->target,( gimbal_t.pPitch_t->real_angle),5500,(8192));//������
     gimbal_t.pYaw_t->ppostionPid_t->error =rctemp;// CalculateError((gimbal_t.pYaw_t->target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//������
}
/**
* @Data    2019-03-21 00:46
* @brief   ��̨Ԥ��//������
* @param   void
* @retval  void
*/
void GImbalAnticipation(void)
{
  if(gimbal_t.pPc_t->commot == 0)
  {
    
  }
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
void SetPcControlPID(void)
{
  			gimbal_t.pYaw_t->ppostionPid_t->kp = -3;
				gimbal_t.pYaw_t->ppostionPid_t->kd = 0;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//������
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid���
  
  						/* ------ �⻷pid���� ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 24;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 85;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0.5;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//�����޷�
}
/**
* @Data    2019-03-21 00:46
* @brief   ��λpid����
* @param   void
* @retval  void
*/
void SetGeneralMode(void)
{
    		gimbal_t.pYaw_t->ppostionPid_t->kp = -0.3;
				gimbal_t.pYaw_t->ppostionPid_t->kd = -1;
				gimbal_t.pYaw_t->ppostionPid_t->ki = -0.02;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//������
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid���
  
  						/* ------ �⻷pid���� ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 10;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 10;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0.1;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//�ϴ����
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//������
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p���
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i���
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k���
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid���
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//�����޷�
}
/**
* @Data    2019-03-21 00:46
* @brief   ��̨ģ��ʧ��//������
* @param   void
* @retval  void
*/
void GimbalDeinit(void)
{
  	__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_1_START_V);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_2_START_V);
    HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
}
/**
	* @Data    2019-03-26 21:17
	* @brief   ��֡����ģʽ
	* @param   void
	* @retval  void
	*/
int16_t ayaw;
int16_t apitch;
int16_t f_lock_yawshijue;
int16_t lock_ptichshij;
	void FrameDropBufferMode(void)
	{
//		int16_t yaw_er;
//		int16_t pitch_er;
//		int16_t loyaw_er;
//		int16_t lopitch_er;
		if((gimbal_t.status&FRAME_DROP_BUFFER_RUNING) != FRAME_DROP_BUFFER_RUNING)
		{
			SetFrameDropBufferStatus();
			
		 gimbal_t.pPitch_t->ppostionPid_t->error = -gimbal_t.pPitch_t->ppostionPid_t->error;
     gimbal_t.pYaw_t->ppostionPid_t->error = -gimbal_t.pYaw_t->ppostionPid_t->error ;
			// lock_yawshijue = gimbal_t.pYaw_t->real_angle;
			// lock_ptichshij = gimbal_t.pPitch_t->real_angle;
			// GetTrend(yaw_er,pitch_er);
			// gimbal_t.pPitch_t->ppostionPid_t->error = pitch_er*ayaw;
			// gimbal_t.pYaw_t->ppostionPid_t->error =  yaw_er*apitch;
			// if(gimbal_t.pPitch_t->ppostionPid_t->error >0)
			// lock_ptichshij = lock_ptichshij+600;
			// else 	lock_ptichshij = lock_ptichshij-300;
			// if ( gimbal_t.pYaw_t->ppostionPid_t->error >0)
			// lock_yawshijue += 1700;
			// else 	lock_yawshijue -= 1700;
		}
		// if(gimbal_t.pYaw_t->real_angle > lock_yawshijue)
		// {

		// }
	}
	/**
		* @Data    2019-03-26 21:32
		* @brief   ��֡����״̬����
		* @param   void
		* @retval  void
		*/
		void SetFrameDropBufferStatus(void)
		{
			SET_RUNING_STATUS(FRAME_DROP_BUFFER_RUNING);
     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_SPEED);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_SPEED);
     HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
		 SetPcControlPID();
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
