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
#define   RC_MODE_READ                 (0x80000000U)//ң��ģʽ 
 #define  ELUDE_MODE_READ              (0x40000000U) //���ģʽ     
 #define  PC_SHOOT_MODE_READ           (0x20000000U) //������ģʽ׼������
 #define  CRUISE_MODE_READ             (0x10000000U)//Ѳ��ģʽģʽ׼������
 #define  RC_MODE_RUNING               (0x08000000U)//ң��ģʽִ�� 
 #define  ELUDE_MODE_RUNING            (0x04000000U) //���ģʽ 
 #define  PC_SHOOT_MODE_RUNING         (0x02000000U) //������ģʽִ��   
 #define  CRUISE_MODE_RUNING           (0x01000000U) //Ѳ��ģʽִ��
// #define  DISABLE_MOD                (0xF0000000U)//ʹ������ģ��

// #define  RC_MODE_READ               (0x02000000U)//ң��ģʽ׼������
// #define  JUDGE_READ                 (0x0F000000U)//�жϾ���
// #define  DELEC_USER_MODE            (0x00FFFFFFU)//����û��Զ����־λ
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
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc)
	{
		chassis_t.hcanx = hcan;
    chassis_t.rc_t =  rc;
    chassis_t.status = 0;
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
	void ChassisControl(void)
	{
		int16_t pid_out[2];
    Inject(&powerBufferPool_t);//���¹��ʻ����״̬
    ChassisRcControlMode();
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
		if((chassis_t.status & RC_MODE_RUNING)!= RC_MODE_RUNING)
		{
			chassis_t.pwheel1_t->target = 0;
			chassis_t.pwheel2_t->target = 0;
			SET_BIT(chassis_t.status,RC_MODE_RUNING);
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
	// 	 int16_t zhongd = 1500;
	// void ChassisCruiseModeControl(void)
	// {
	// 		uint32_t position;
	// 	if((chassis_t.status & CRUISE_MODE_READ) != CRUISE_MODE_READ)
	// 	{
	// 		CLEAR_BIT(chassis_t.status,ELUDE_MODE_RUNING);
	// 		CLEAR_BIT(chassis_t.status,RC_MODE_RUNING);
	// 		CLEAR_BIT(chassis_t.status,PC_SHOOT_MODE_RUNING);
	// 		SET_BIT(chassis_t.status,CRUISE_MODE_RUNING);
	// 		// 
	// 	}
	// 		position	=GetPosition(chassis_t.pchassisEnconder_t);
	// 		if(position > 2600)
	// 		{
	// 			if(GetDistance(3) <600)
	// 			{
	// 				chassis_t
	// 			}
	// 		}
	// }

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
     /* ------ ����������־λ ------- */  
        SET_BIT(chassis_t.status,START_OK);  
  }
/*----------------------------------file of end-------------------------------*/
