/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,����ƽͷ��,                       |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.c                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2019-01-19               
	|--Libsupports : 
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-------------------------------declaration of end---------------------------|
 **/
#include "chassis.h"
/* -------------- ģ���־λ�궨�� ----------------- */
#define   CHASSIS_RC_MODE_READ                 (0x80000000U)//ң��ģʽ 
 #define  CHASSIS_ELUDE_MODE_READ              (0x40000000U) //���ģʽ     
 #define  CHASSIS_PC_SHOOT_MODE_READ           (0x20000000U) //������ģʽ׼������
 #define  CHASSIS_CRUISE_MODE_READ             (0x10000000U)//Ѳ��ģʽģʽ׼������
 #define  DISABLE_CHA_MOD_READ			           (0xF0000000U)//ʧ�ܾ�����־λ
 #define  CHASSIS_RC_MODE_RUNING               (0x08000000U)//ң��ģʽִ�� 
 #define  CHASSIS_ELUDE_MODE_RUNING            (0x04000000U) //���ģʽ 
 #define  CHASSIS_PC_SHOOT_MODE_RUNING         (0x02000000U) //������ģʽִ��   
 #define  CHASSIS_CRUISE_MODE_RUNING           (0x01000000U) //Ѳ��ģʽִ��
#define   DISABLE_CHA_MOD_RUNNING			         (0x0F000000U)//ʧ�ܾ�����־λ
#define   DISABLE_CHASSIS                      (0xFF000000U)//ʧ�ܵ���
#define   CHASSIS_JUDGE_READ                   (0xF0000000U)//�жϾ���
// #define  DISABLE_MOD                        (0xF0000000U)//ʹ������ģ��

// #define  RC_MODE_READ               (0x02000000U)//ң��ģʽ׼������
// #define  JUDGE_READ                 (0x0F000000U)//�жϾ���
// #define  DELEC_USER_MODE            (0x00FFFFFFU)//����û��Զ����־λ


#define SET_CHA_RUNING_STATUS(_status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(chassis_t.status,DISABLE_CHA_MOD_RUNNING);	\
					SET_BIT(chassis_t.status,_status_);							\
				}while(0)	
#define SET_CHA_READ_STATUS(__status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(chassis_t.status,DISABLE_CHA_MOD_READ);	\
					SET_BIT(chassis_t.status,__status_);							\
				}while(0)																					

/* -------------- �ⲿ���� ----------------- */
extern TIM_HandleTypeDef htim5;
extern	osThreadId startChassisTaskHandle;
/* -------------- ���Ͷ��� ----------------- */
  xQueueHandle chassis_queue;
/* -------------- ��̬���� ----------------- */
	 chassisStruct chassis_t;
    static RM3508Struct wheel1_t;         //����1
      static speedPidStruct wheel1Speed_t;
    static RM3508Struct wheel2_t;         //����2
      static speedPidStruct wheel2Speed_t;
     static powerBufferPoolStruct powerBufferPool_t;
			static currentMeterStruct currtenMeter_t;
       static gy955Struct gyroByCan_t;
/*-------------------------------------------------------
  |       �ͺ�     |  ���� |  brown | bule  |black|while|
  -------------------------------------------------------
  |ŷķ�� E6A2-CW3C|500P/R |5to12VDC|0V(GND)|OUT A|OUT B|
 -------------------------------------------------------*/
		static incrementalEnconderStruct chassisEnconder_t; //�������ṹ��
/* -------------- ˽�к� ----------------- */
	#define CHASSIS_QUEUE_LEN      5U//���Ϊ5
	#define CHASSIS_QUEUE_SIZE    8U//����Ϊ5;
	/**
	* @Data    2019-01-27 17:09
	* @brief   ���̽ṹ�����ݳ�ʼ��
	* @param  CAN_HandleTypeDef* hcanx��x=1,2��
	* @retval  void
	*/
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc,const pcDataStruct* pPc_t)
	{
		chassis_t.hcanx = hcan;
    chassis_t.rc_t =  rc;
		chassis_t.pPc_t = pPc_t;
    chassis_t.status = 0;
    chassis_t.p_refereeSystem_t = &ext_refereeSystem_t;
    chassis_t.pgyroByCan_t  = &gyroByCan_t;
    chassis_t.ppowerBufferPool_t = PowerBufferPoolInit();
		/* ------ ����1�ṹ�����ݳ�ʼ�� ------- */
		chassis_t.pwheel1_t = wheel1Init();
		/* ------ ����2�ṹ�����ݳ�ʼ�� ------- */
		chassis_t.pwheel2_t = wheel2Init();
  /* ------ ������̨���Ͷ��� ------- */
	  chassis_queue	= xQueueCreate(CHASSIS_QUEUE_LEN,CHASSIS_QUEUE_SIZE);//һ��Ҫ����֮ǰ��������
    /* -------- can���� --------- */
      if(UserCanConfig(hcan)!= HAL_OK)
        while(1){}//����ӷ��ظ����ù���
		/* ------ ��������ʼ�� ------- */
	 	chassis_t.pchassisEnconder_t = &chassisEnconder_t;
		  if(EnconderInit(&chassisEnconder_t,RADIUS,ENCONDER_POLES) !=HAL_OK)
        while(1){}
		/* ------ ��������ʼ�� ------- */
	  	HCSR04Init();
    /* -------- ����ģ���ʼ���ж� --------- */
      SET_BIT(chassis_t.status,INIT_OK);//��ʼ���ɹ�
	/* ------ �������񣬵ȴ���ʼ�� ------- */
    	vTaskSuspend(startChassisTaskHandle);
	/* ------ ���û����˳�ʼ��״̬ ------- */
     SetSetInitStatus();
	}
/**
	* @Data    2019-01-28 11:40
	* @brief   �������ݽ���
	* @param   void
	* @retval  void
	*/
	void ChassisParseDate(uint16_t id,uint8_t *data)
	{
		switch (id)
		{
			case WHEEL1_RX_ID:
				RM3508ParseData(&wheel1_t,data);
				break;
			case WHEEL2_RX_ID:
				RM3508ParseData(&wheel2_t,data);
				break;
	  	case CURRENT_METER_RX_ID:
				 CurrentMeterAnalysis(&currtenMeter_t,data);
				break;
      case BIN_GE_GYRO_CAN_RX_ID:
        BingeGyroByCan(&gyroByCan_t,data);
        break;
			default:
				break;
		}
    SET_BIT(chassis_t.status,RX_OK);//���ܳɹ�
	}
///**
//	* @Data    2019-02-15 15:10
//	* @brief   �������ݷ���
//	* @param   void
//	* @retval  void
//	*/
//	void ChassisUserCanTx(int16_t w1,int16_t w2)
//	{
//		uint8_t s[8]={0};
//		s[0] = (uint8_t)(w1<<8);
//		s[1] = (uint8_t)w1;
//		s[2] = (uint8_t)(w2<<8);
//		s[3] = (uint8_t)w2;
//		CanTxMsg(chassis_t.hcanx,CHASSIS_CAN_TX_ID,s);
//	}
/**
	* @Data    2019-03-12 16:25
	* @brief   ���̿��ƺ���
	* @param   void
	* @retval  void
	*/
  uint32_t chassiscommot =0;
	void ChassisControl(void)
	{
		int16_t pid_out[2];
    Inject(&powerBufferPool_t);//���¹��ʻ����״̬
		SetArea();//������
 chassiscommot	= ChassisControlDecision();
	ChassisControlSwitch(chassiscommot);
    chassis_t.pwheel1_t->error = chassis_t.pwheel1_t->target - chassis_t.pwheel1_t->real_speed;
     chassis_t.pwheel2_t->error = chassis_t.pwheel2_t->target - chassis_t.pwheel2_t->real_speed;
		pid_out[0] = SpeedPid(chassis_t.pwheel1_t->pspeedPid_t,chassis_t.pwheel1_t->error);
  	pid_out[1] = SpeedPid(chassis_t.pwheel2_t->pspeedPid_t,chassis_t.pwheel2_t->error);
    SetInPut(&powerBufferPool_t,pid_out,2);//�����ʻ����
		ChassisCanTx(pid_out[0],pid_out[1]);
    SET_BIT(chassis_t.status,RUNING_OK);
	}
 /*
* @Data    2019-02-24 11:59
* @brief   ��ȡ���̽ṹ���ַ
* @param   void
* @retval  void
*/
const chassisStruct* GetChassisStructAddr(void)
{
  return &chassis_t;
}
  /**
  * @Data    2019-03-13 03:48
  * @brief   ��ȡ���̽ṹ���ַ �ɶ�д�������ҵ���
  * @param   void
  * @retval  void
  */
  chassisStruct *RWGetChassisStructAddr(void)
  {
    return &chassis_t;
  }
  /**
  * @Data    2019-03-13 01:55
  * @brief   ��ȡ����״̬
  * @param   void
  * @retval  void
  */
  uint32_t GetChassisStatus(void)
  {
    return chassis_t.status;
  }
  /**
  * @Data    2019-03-13 02:56
  * @brief   ���õ��Ŀ���ٶ�
  * @param   void
  * @retval  void
  */
  void SetMotorTarget(int16_t w1,int16_t w2)
  {
    	wheel1_t.target = w1;
      wheel2_t.target = w2;
  }
  /**
  * @Data    2019-03-13 03:19
  * @brief   ң��ģʽ
  * @param   void
  * @retval  void
  */
int16_t rc_coe=7;
  void ChassisRcControlMode(void)
  {
		if((chassis_t.status & CHASSIS_RC_MODE_RUNING)!= CHASSIS_RC_MODE_RUNING)
		{
			chassis_t.pwheel1_t->target = 0;
			chassis_t.pwheel2_t->target = 0;
			SET_BIT(chassis_t.status,CHASSIS_RC_MODE_RUNING);
		}
		chassis_t.pwheel1_t->target = chassis_t.rc_t->ch1 *rc_coe;
		chassis_t.pwheel2_t->target = chassis_t.rc_t->ch1 *rc_coe;
  }
  /**
  * @Data    2019-03-25 00:28
  * @brief   ��������Ȩ����
  * @param   void
  * @retval  void
  */
  // void ChassisControlPriorityDecision(void)
  // {
  //   if(chassis_t.rc_t->switch_right ==1)
	// 	{
	// 		switch ()
	// 	}
	// 	else if(chassis_t.rc_t->switch_right ==2)
	// 	{
	// 	 	SET_BIT(chassis_t.status,RC_MODE_READ);
  //     CLEAR_BIT(chassis_t.status,ELUDE_MODE_READ);
  //     CLEAR_BIT(chassis_t.status,PC_SHOOT_MODE_READ);
	// 		CLEAR_BIT(chassis_t.status,CRUISE_MODE_READ);
	// 	}

  //     // SET_BIT(chassis_t.status,RC_MODE_RUNING);
  //     // CLEAR_BIT(chassis_t.status,ELUDE_MODE_RUNING);
  //     // CLEAR_BIT(chassis_t.status,PC_SHOOT_MODE_RUNING);
  //     // CLEAR_BIT(chassis_t.status,CRUISE_MODE_RUNING);
  // }
/**
	* @Data    2019-02-15 15:10
	* @brief   ��̨���ݷ���
	* @param   void
	* @retval  void
	*/
	HAL_StatusTypeDef ChassisCanTx(int16_t w1,int16_t w2)
	{
		uint8_t s[8];
    memset(s,0,8);
    s[0] = (uint8_t)(w1>>8) & 0xFF;
    s[1] = (uint8_t)w1;
    s[2] = (uint8_t)(w2>>8) & 0xFF;
    s[3] = (uint8_t)w2;
   	xQueueSendToBack(chassis_queue,s,0);
		return HAL_OK;
	}
// /**
// 	* @Data    2019-03-25 19:05
// 	* @brief   ��ȡ��̨����
// 	* @param   void
// 	* @retval  void
// 	*/
// 	void GetGimbalCom(void)
// 	{
		
// 	}

// /**
// 	* @Data    2019-03-25 19:11
// 	* @brief   ��ȡ������״̬
// 	* @param   void
// 	* @retval  void
// 	*/
// 	void GetRotbStatus(void)
// 	{
// 		if()
// 	}
/**
	* @Data    2019-03-25 19:13
	* @brief   Ѳ��ģʽ
	* @param   void
	* @retval  void
	*/
		 int16_t zhongd = 1500;
		 int16_t cru_speed = 4000;
		 int16_t direction =0;
	void ChassisCruiseModeControl(void)
	{
//			uint32_t position;
		if((chassis_t.status & CHASSIS_CRUISE_MODE_READ) != CHASSIS_CRUISE_MODE_READ)
		{
			ChassisCruiseModeInit();
		}
		if(chassis_t.State.r_area == MID_ROAD)
		{
			 direction = chassis_t.State.r_area - chassis_t.State.last_area;//��3508�ķ���
			 chassis_t.pwheel1_t->target = direction*chassis_t.pwheel1_t->target;
			 chassis_t.pwheel2_t->target = chassis_t.pwheel1_t->target;
		}
		else if(chassis_t.State.r_area != MID_ROAD)
		{
			 direction = -(chassis_t.State.r_area - chassis_t.State.last_area);//��3508�ķ���
			 chassis_t.pwheel1_t->target = direction*chassis_t.pwheel1_t->target;
			 chassis_t.pwheel2_t->target = chassis_t.pwheel1_t->target;
		}
	}
/**
	* @Data    2019-03-30 15:50
	* @brief   ��������ģʽ��ʼ��
	* @param   void
	* @retval  void
	*/
	void ChassisCruiseModeInit(void)
	{
		SET_CHA_RUNING_STATUS(CHASSIS_CRUISE_MODE_READ);
	}

/**
	* @Data    2019-03-25 19:16
	* @brief   ���»�����λ�ú�״̬
	* @param   void
	* @retval  void
	*/

// 	void GetRobotStatus(void)
// 	{
// 		


// 	}

/**
	* @Data    2019-03-25 19:16
	* @brief   ��ȡ�����˱�������״̬
	* @param   void
	* @retval  void
	*/
uint8_t f_hurt_flag=0;
uint8_t f_hurt_fre=0;
uint8_t b_hurt_flag=0;
uint8_t b_hurt_fre=0;
uint8_t GetHurtStatus(void)
{
	if(chassis_t.p_refereeSystem_t->p_robot_hurt_t->hurt_type ==0)
	return (chassis_t.p_refereeSystem_t->p_robot_hurt_t->armor_id);
	else return 0x55;
}

/**
* @Data    2019-03-20 21:27
* @brief   ����Ȩ�л�
* @param   void
* @retval  void
*/
void ChassisControlSwitch(uint32_t commot)
{
 switch (commot)
 {
   case CHASSIS_RC_MODE_READ:
       ChassisRcControlMode();
     break;
   case CHASSIS_ELUDE_MODE_READ:
    //  PcControlMode();
		  break;
	case CHASSIS_PC_SHOOT_MODE_READ:
	  //  FrameDropBufferMode();
    break;
      case CHASSIS_CRUISE_MODE_READ:
       ChassisCruiseModeControl();
     break;
   default:
    ChassisDeinit();
	    break;
 }
}
/**
* @Data    2019-03-20 21:27
* @brief   ���߿����ж�
* @param   void
* @retval  void
*/
  uint32_t ChassisControlDecision(void)
  {
    if(chassis_t.rc_t ->switch_left ==2)
    {
      CLEAR_BIT(chassis_t.status,DISABLE_CHASSIS);
     return (chassis_t.status & CHASSIS_JUDGE_READ);
    }
    else if(chassis_t.rc_t->switch_right ==1)
    {
//			if(GetHurtStatus() !=0x55)
//        
      if(chassis_t.pPc_t->commot ==1)
      {
        
      }
      else 	SET_CHA_READ_STATUS(CHASSIS_CRUISE_MODE_READ);
    }
    else if(chassis_t.rc_t->switch_right ==2)
    {
			 SET_CHA_READ_STATUS(CHASSIS_RC_MODE_READ);
    }   
    return (chassis_t.status & CHASSIS_JUDGE_READ);
  }
/**
* @Data    2019-03-21 00:46
* @brief   ����ģ��ʧ��//������
* @param   void
* @retval  void
*/
void ChassisDeinit(void)
{
chassis_t.pwheel1_t->target = 0;
chassis_t.pwheel2_t->target = 0;
}
/**
* @Data    2019-03-21 00:46
* @brief   ���ģʽ//������
* @param   void
* @retval  void
*/
void ChassisEludeControlMode(void)
{
	if(GetHurtStatus() == AHEAD_OF_ARMOR)
	{
		switch (chassis_t.State.r_area) 
		{
			case UP_ROAD:
			
				break;
			case MID_ROAD:
			
				break;
			case DOWN_ROAD:
			
				break;
			default:
				break;
		}
	}
}

/**
	* @Data    2019-03-30 22:13
	* @brief   �ܵ�ָ��Ŀ�ĵ�
	* @param   void
	* @retval  void
	*/
	// void Go(int16_t target)
	// {
		
	// }






  /**
  * @Data    2019-03-25 00:08
  * @brief   ��ʼ�����ʻ����
  * @param   void
  * @retval  void
  */
  powerBufferPoolStruct* PowerBufferPoolInit(void)
  {
    powerBufferPool_t.pcurrentMeter_t = &currtenMeter_t;
    powerBufferPool_t.max_p = 20.0;
    powerBufferPool_t.max_w = 200.0;//���ʵ�λmW
    powerBufferPool_t.r_p = 0.0;
    powerBufferPool_t.r_w = 200.0;//���ʵ�λmW
    powerBufferPool_t.current_mapp_coe = 0.00122;//����ӳ��ϵ��
    powerBufferPool_t.high_water_level = 150.0;
    powerBufferPool_t.low_water_level = 30.0;
    powerBufferPool_t.mid_water_level = 80.0;
    powerBufferPool_t.period = 0.01;//�������ڣ���λ/s
    powerBufferPool_t.high_current_threshold = 5.0;//mA
    powerBufferPool_t.mid_current_threshold = 2.0;//mA
    powerBufferPool_t.low_current_threshold = 0.9;//mA
    powerBufferPool_t.safe_current_threshold = 0.7;//mA
    return &powerBufferPool_t;
  }
  /**
  * @Data    2019-03-25 00:12
  * @brief   ��ʼ�����Ӳ���
  * @param   void
  * @retval  void
  */
  RM3508Struct* wheel1Init(void)
  {
      wheel1_t.id = WHEEL1_RX_ID;//���can�� ip
//			wheel1_t.target = 0;		 //Ŀ��ֵ
			wheel1_t.tem_target = 0;//��ʱĿ��ֵ
			wheel1_t.real_current = 0; //��ʵ����
			wheel1_t.real_angle = 0;//��ʵ�Ƕ�
			wheel1_t.tem_angle = 0;//��ʱ�Ƕ�
  	  wheel1_t.real_speed = 0;//��ʵ�ٶ�
  	  wheel1_t.tem_speed = 0;//��ʵ�ٶ�
			wheel1_t.zero = 0;			 //������
			wheel1_t.Percentage = 0;//ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
			wheel1_t.thresholds = 0; //�����ת��ֵ
  	  wheel1_t.error = 0;//��ǰ���
      wheel1_t.ppostionPid_t = NULL;
      wheel1_t.pspeedPid_t = &wheel1Speed_t;
//				wheel1Speed_t.kp = 1;
//				wheel1Speed_t.kd = 0;
//				wheel1Speed_t.ki = 0;
				wheel1Speed_t.error = 0;
				wheel1Speed_t.last_error = 0;//�ϴ����
				wheel1Speed_t.before_last_error = 0;//���ϴ����
				wheel1Speed_t.integral_er = 0;//������
				wheel1Speed_t.pout = 0;//p���
				wheel1Speed_t.iout = 0;//i���
				wheel1Speed_t.dout = 0;//k���
				wheel1Speed_t.pid_out = 0;//pid���
//				wheel1Speed_t.limiting = W1_LIMIT_SPEED;//����1�ٶ��޷�
        return &wheel1_t;
  }
  /**
  * @Data    2019-03-25 00:15
  * @brief   ����2������ʼ��
  * @param   void
  * @retval  void
  */
  RM3508Struct* wheel2Init(void)
  {
			wheel2_t.id = WHEEL2_RX_ID;//���can�� ip
//			wheel2_t.target = 0;		 //Ŀ��ֵ
			wheel2_t.tem_target = 0;//��ʱĿ��ֵ
			wheel2_t.real_current = 0; //��ʵ����
			wheel2_t.real_angle = 0;//��ʵ�Ƕ�
			wheel2_t.tem_angle = 0;//��ʱ�Ƕ�
			wheel2_t.real_speed = 0;//��ʵ�ٶ�
			wheel2_t.tem_speed = 0;//��ʵ�ٶ�
			wheel2_t.zero = 0;			 //������
			wheel2_t.Percentage = 0;//ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
			wheel2_t.thresholds = 0; //�����ת��ֵ
			wheel2_t.error = 0;//��ǰ���
 			wheel2_t.ppostionPid_t = NULL;
      wheel2_t.pspeedPid_t = &wheel2Speed_t;
//				wheel2Speed_t.kp = 1;
//				wheel2Speed_t.kd = 0;
//				wheel2Speed_t.ki = 0;
				wheel2Speed_t.error = 0;
				wheel2Speed_t.last_error = 0;//�ϴ����
				wheel2Speed_t.before_last_error = 0;//���ϴ����
				wheel2Speed_t.integral_er = 0;//������
				wheel2Speed_t.pout = 0;//p���
				wheel2Speed_t.iout = 0;//i���
				wheel2Speed_t.dout = 0;//k���
				wheel2Speed_t.pid_out = 0;//pid���
//				wheel2Speed_t.limiting = W2_LIMIT_SPEED;//����2�ٶ��޷�
      return &wheel2_t;
  }
 /**
	 * @Data    2019-03-19 17:58
	 * @brief   ���̳�ʼ��״̬����
	 * @param   void
	 * @retval  void
	 */
 	void SetSetInitStatus(void)
  {
    	wheel1_t.target = 0;		 //Ŀ��ֵ
    		wheel1Speed_t.kp = 9;
				wheel1Speed_t.kd = 1.5;
				wheel1Speed_t.ki = 0.9;
     	wheel1Speed_t.limiting = W1_LIMIT_SPEED;//����1�ٶ��޷�
    
    	wheel2_t.target = 0;		 //Ŀ��ֵ
    	wheel2Speed_t.kp = 9;
			wheel2Speed_t.kd = 1.5;
			wheel2Speed_t.ki = 0.9;
    	wheel2Speed_t.limiting = W2_LIMIT_SPEED;//����2�ٶ��޷�
		/* ------ ����״̬��ʼ�� ------- */
			chassis_t.State.r_dire = (int16_t)chassis_t.pgyroByCan_t->Yaw;
		/* ------ ���ó�ʼ������λ�� ------- */
			chassis_t.State.r_area  = UP_ROAD;
			chassis_t.State.last_area = UP_ROAD;//��ӳ���������Ƿ�λ����ȷ
     /* ------ ����������־λ ------- */  
        SET_BIT(chassis_t.status,START_OK);  
  }
	/**
		* @Data    2019-03-30 17:16
		* @brief   ���������������ȡ
		* @param   void
		* @retval  void
		*/
uint8_t up_turn = 0;
	void SetArea(void)
	{
	if(GetOrgans() != 0)
	{
   up_turn =GetOrgans();
	}
		if(GetGyroDire()<TURNING_ANGLE)//������״̬//up
		{
				// temp = UP_ROAD;
				if(up_turn ==1)
				{
					if(chassis_t.State.r_area  != UP_ROAD)
					{
						chassis_t.State.last_area = chassis_t.State.r_area;
					}
					chassis_t.State.r_area = UP_ROAD;
					up_turn = 0;
				}
			else 
			{
				if(chassis_t.State.r_area  != DOWN_ROAD)
				{
					chassis_t.State.last_area = chassis_t.State.r_area;
				}
				chassis_t.State.r_area = DOWN_ROAD;
			}

		}
		else if(GetGyroDire() >= TURNING_ANGLE)//mid
		{
			if(chassis_t.State.r_area  != MID_ROAD)
			{
				chassis_t.State.last_area = chassis_t.State.r_area;
			}
			chassis_t.State.r_area = MID_ROAD;
		}
	}
/**
	* @Data    2019-03-30 19:58
	* @brief   
	* @param   void
	* @retval  void
	*/
	int16_t  GetGyroDire(void)
	{
		int16_t dire=0;
		dire = chassis_t.State.r_dire - (int16_t)chassis_t.pgyroByCan_t->Yaw;
		return ABS(dire);
	}
int16_t organs_flag;
int16_t organs = 0;
	uint8_t GetOrgans(void)
	{
		 organs = HAL_GPIO_ReadPin(LASER_SWITCH_GPIO,LASER_SWITCH);//��io��
		// uint8_t temp;
	if(organs ==1)//���⿪��
	{
		organs_flag ++;
	}
	else organs_flag --;
	if(organs_flag <0)
    return  0;  
	else if(organs_flag >5)
   	return  1; 
  else   return  0; 
	}
/*----------------------------------file of end-------------------------------*/
