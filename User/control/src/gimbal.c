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
extern UART_HandleTypeDef huart2;//����1
/* -------------- ˽�к궨�� ----------------- */
	#define RAMMER_RX_ID	  0x207
	#define YAW_RX_ID 			0x205
	#define PITCH_RX_ID 		0x206
	#define GIMBAL_CAN_TX_ID 0x1ff
/* -------------- �ṹ������ ----------------- */
	gimbalStruct gimbal_t;                     //��̨�ṹ��
	M2006Struct rammer_t;                       //��������ṹ��
	  postionPidStruct rammerOuterLoopPid_t;      //��������⻷pid
	  speedPidStruct rammerInnerLoopPid_t;        //��������ڻ�pid
  RM6623Struct yaw_t;                         //yaw��(����)�����ʼ��
	  postionPidStruct yawOuterLoopPid_t;         //yaw����⻷pid
	  speedPidStruct yawInnerLoopPid_t;           //yaw����ڻ�pid
  RM6623Struct pitch_t;                       //pitch�ᣨ���ᣩ�����ʼ��
  	postionPidStruct pitchOuterLoopPid_t;       //pitch����⻷pid
	  speedPidStruct pitchInnerLoopPid_t;         //pitch����ڻ�pid
	xQueueHandle gimbal_queue;
  /* ----------------- ȫ�ֱ��� -------------------- */
    uint8_t pc_data[3] ={0};
   float pamamer__p =0.02;
   float pamamer__y =0.04;
	/**
	* @Data    2019-01-27 17:09
	* @brief   ��̨�ṹ���ʼ��
	* @param  CAN_HandleTypeDef* hcanx��x=1,2��
	* @retval  void
	*/
	void GimbalStructInit(CAN_HandleTypeDef *hcanx)
	{
		gimbal_t.hcanx = hcanx;
    gimbal_t.huartx = &huart2;
		/* ---------------- ���������ʼ�� ----------------- */
			gimbal_t.prammer_t = &rammer_t;
			rammer_t.rx_id = RAMMER_RX_ID;//���can�� ip
		  rammer_t.target = 0; //Ŀ��ֵ
		  rammer_t.real_current = 0;//��ʵ����
		  rammer_t.real_angle = 0;//��ʵ�Ƕ�
		  rammer_t.real_speed = 0;//��ʵ�ٶ�
			rammer_t.error[0] = 0;
			rammer_t.error[1] = 0;
			rammer_t.error[2] = 0;
			/* ------ �⻷pid���� ------- */
				rammer_t.pOuterLoop_t = &rammerOuterLoopPid_t;
				rammerOuterLoopPid_t.kp = 0;
				rammerOuterLoopPid_t.kd = 0;
				rammerOuterLoopPid_t.ki = 0;
				rammerOuterLoopPid_t.error = 0;
				rammerOuterLoopPid_t.last_error = 0;//�ϴ����
				rammerOuterLoopPid_t.integral_er = 0;//������
				rammerOuterLoopPid_t.pout = 0;//p���
				rammerOuterLoopPid_t.iout = 0;//i���
				rammerOuterLoopPid_t.dout = 0;//k���
				rammerOuterLoopPid_t.pid_out = 0;//pid���
			/* ------ �ڻ�pid���� ------- */
				rammer_t.pInnerLoop_t = &rammerInnerLoopPid_t;
				rammerInnerLoopPid_t.kp = 0;
				rammerInnerLoopPid_t.kd = 0;
				rammerInnerLoopPid_t.ki = 0;
				rammerInnerLoopPid_t.error = 0;
				rammerInnerLoopPid_t.last_error = 0;//�ϴ����
				rammerInnerLoopPid_t.before_last_error = 0;//���ϴ����
				rammerInnerLoopPid_t.integral_er = 0;//������
				rammerInnerLoopPid_t.pout = 0;//p���
				rammerInnerLoopPid_t.iout = 0;//i���
				rammerInnerLoopPid_t.dout = 0;//k���
				rammerInnerLoopPid_t.pid_out = 0;//pid���
   /* ---------------- YAW������ʼ�� ----------------- */
    gimbal_t.pYaw_t = &yaw_t;
     yaw_t.id = YAW_RX_ID;             //���can�� ip
		 yaw_t.target = 0;          //Ŀ��ֵ
		 yaw_t.tem_target = 0;        //��ʱĿ��ֵ
		 yaw_t.real_current = 0;        //��ʵ����
		 yaw_t.real_angle = 0;         //��ʵ�Ƕ�
		 yaw_t.tem_angle = 0;          //��ʱ�Ƕ�
		 yaw_t.zero = 0;             //������
		 yaw_t.Percentage = 0;           //ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
     yaw_t.thresholds = 0;        //�����ת��ֵ
    	/* ------ �⻷pid���� ------- */
				yawOuterLoopPid_t.kp = 0;
				yawOuterLoopPid_t.kd = 0;
				yawOuterLoopPid_t.ki = 0;
				yawOuterLoopPid_t.error = 0;
				yawOuterLoopPid_t.last_error = 0;//�ϴ����
				yawOuterLoopPid_t.integral_er = 0;//������
				yawOuterLoopPid_t.pout = 0;//p���
				yawOuterLoopPid_t.iout = 0;//i���
				yawOuterLoopPid_t.dout = 0;//k���
				yawOuterLoopPid_t.pid_out = 0;//pid���
    yaw_t.ppostionPidStruct = &yawOuterLoopPid_t;
    	/* ------ �ڻ�pid���� ------- */
				yawInnerLoopPid_t.kp = 0;
				yawInnerLoopPid_t.kd = 0;
				yawInnerLoopPid_t.ki = 0;
				yawInnerLoopPid_t.error = 0;
				yawInnerLoopPid_t.last_error = 0;//�ϴ����
				yawInnerLoopPid_t.before_last_error = 0;//���ϴ����
				yawInnerLoopPid_t.integral_er = 0;//������
				yawInnerLoopPid_t.pout = 0;//p���
				yawInnerLoopPid_t.iout = 0;//i���
				yawInnerLoopPid_t.dout = 0;//k���
				yawInnerLoopPid_t.pid_out = 0;//pid���
	   yaw_t.pspeedPidStruct = &yawInnerLoopPid_t;
   /* ---------------- pitch������ʼ�� ----------------- */
    gimbal_t.pPitch_t = &pitch_t;
     pitch_t.id = PITCH_RX_ID;             //���can�� ip
		 pitch_t.target = 0;          //Ŀ��ֵ
		 pitch_t.tem_target = 0;        //��ʱĿ��ֵ
		 pitch_t.real_current = 0;        //��ʵ����
		 pitch_t.real_angle = 0;         //��ʵ�Ƕ�
		 pitch_t.tem_angle = 0;          //��ʱ�Ƕ�
		 pitch_t.zero = 0;             //������
		 pitch_t.Percentage = 0;           //ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
     pitch_t.thresholds = 0;        //�����ת��ֵ
    	/* ------ �⻷pid���� ------- */
				pitchOuterLoopPid_t.kp = 0;
				pitchOuterLoopPid_t.kd = 0;
				pitchOuterLoopPid_t.ki = 0;
				pitchOuterLoopPid_t.error = 0;
				pitchOuterLoopPid_t.last_error = 0;//�ϴ����
				pitchOuterLoopPid_t.integral_er = 0;//������
				pitchOuterLoopPid_t.pout = 0;//p���
				pitchOuterLoopPid_t.iout = 0;//i���
				pitchOuterLoopPid_t.dout = 0;//k���
				pitchOuterLoopPid_t.pid_out = 0;//pid���
    pitch_t.ppostionPidStruct = &pitchOuterLoopPid_t;
    	/* ------ �ڻ�pid���� ------- */
				pitchInnerLoopPid_t.kp = 0;
				pitchInnerLoopPid_t.kd = 0;
				pitchInnerLoopPid_t.ki = 0;
				pitchInnerLoopPid_t.error = 0;
				pitchInnerLoopPid_t.last_error = 0;//�ϴ����
				pitchInnerLoopPid_t.before_last_error = 0;//���ϴ����
				pitchInnerLoopPid_t.integral_er = 0;//������
				pitchInnerLoopPid_t.pout = 0;//p���
				pitchInnerLoopPid_t.iout = 0;//i���
				pitchInnerLoopPid_t.dout = 0;//k���
				pitchInnerLoopPid_t.pid_out = 0;//pid���
	   pitch_t.pspeedPidStruct = &pitchInnerLoopPid_t;

				// /* ------ ��̨��Ϣ���д��� ------- */
				// gimbal_queue = xQueueCreate(5,3);
        UserCanConfig(hcanx);

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
				RM2006ParseData(&rammer_t,data);
				break;
			case YAW_RX_ID:
				RM6623ParseData(gimbal_t.pYaw_t,data);
        /* -------- ����ת�� --------- */
        gimbal_t.pYaw_t->real_angle = RatiometricConversion  \
        (gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds,gimbal_t.pYaw_t->Percentage);
        /* -------- ���㴦�� --------- */
         zeroArgument(gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds); 
				break;
			case PITCH_RX_ID:
				RM6623ParseData(gimbal_t.pPitch_t,data);
        /* -------- ���㴦�� --------- */
         zeroArgument(gimbal_t.pPitch_t->real_angle,gimbal_t.pPitch_t->thresholds); 
				break;
		
			default:
				break;
		}

	}
/**
	* @Data    2019-02-14 21:01
	* @brief   ��̨����
	* @param   void
	* @retval  void
	*/
	void GimbalControl(const dbusStruct* dbus)
	{
    int16_t p,y;
    if(dbus->switch_left == 1)//ң��ģʽ
    {
      y= DbusAntiShake(dbus->ch1,20);
      gimbal_t.pYaw_t->target += (int16_t)(y*pamamer__y);
      gimbal_t.pPitch_t->target += (int16_t)(p*pamamer__p);
  
        // GimbalCanTx();
    }
	
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
		return(CanTxMsg(gimbal_t.hcanx,GIMBAL_CAN_TX_ID,s));
	}
/*---------------------------------80�ַ�����-----------------------------------*/
  /**
  * @Data    2019-02-24 23:55
  * @brief   С�������ݽ���
  * @param   void
  * @retval  void
  */
  HAL_StatusTypeDef RxPCMsg(void)
  {
    return (HAL_UART_Receive(gimbal_t.huartx,pc_data,3,1));
  }
/*-----------------------------------file of end------------------------------*/
