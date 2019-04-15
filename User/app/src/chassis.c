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
	|--Description :  3508 ˳ʱ��Ϊ��                                                     
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
/* -------- ����ʧ�����͵���ģʽ --------- */
  
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
        
  //  Int16Queue leftSonicQu_t;
  //   Int16Queue rightSonicQu_t;
// /*-------------------------------------------------------
//   |       �ͺ�     |  ���� |  brown | bule  |black|while|
//   -------------------------------------------------------
//   |ŷķ�� E6A2-CW3C|500P/R |5to12VDC|0V(GND)|OUT A|OUT B|
//  -------------------------------------------------------*/
// 		static incrementalEnconderStruct chassisEnconder_t; //�������ṹ��
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
		// /* ------ ��������ʼ�� ------- */
	 	// chassis_t.pchassisEnconder_t = &chassisEnconder_t;
		  // if(EnconderInit(&chassisEnconder_t,EN_RADIUS,ENCONDER_POLES) !=HAL_OK)
      //   while(1){}
		/* ------ ��������ʼ�� ------- */
	  	// HCSR04Init();
      // Int16QueueCreate(&leftSonicQu_t,SONIC_QU_SIZE);
      // Int16QueueCreate(&rightSonicQu_t,SONIC_QU_SIZE);
    /* -------- ����ģ���ʼ���ж� --------- */
      SET_BIT(chassis_t.status,INIT_OK);//��ʼ���ɹ�
	/* ------ �������񣬵ȴ���ʼ�� ------- */
    	vTaskSuspend(startChassisTaskHandle);
	/* ------ ���û����˳�ʼ��״̬ ------- */
     SetSetInitStatus();
//    vTaskSuspend(startChassisTaskHandle);
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
      case CHASSIS_SENSOR_RX_ID:
        ChassisSensorParse(data);
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


/**
	* @Data    2019-03-25 19:16
	* @brief   ��ȡ�����˱�������״̬
	* @param   void
	* @retval  void
	*/
int16_t f_hurt_flag=0;
int16_t f_hurt_fre=0;
int16_t b_hurt_flag=0;
int16_t b_hurt_fre=0;
uint8_t GetHurtStatus(void)
{
	if(chassis_t.p_refereeSystem_t->p_robot_hurt_t->hurt_type ==0)
	{
		f_hurt_flag =1000;
	}
	else
	{
    
		if(f_hurt_flag >0)
    {
      f_hurt_flag --;
     return (chassis_t.p_refereeSystem_t->p_robot_hurt_t->armor_id);
    }
      else f_hurt_flag =-1;
	}
  	 return 5;
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
    powerBufferPool_t.high_water_level = 100.0;
    powerBufferPool_t.low_water_level = 30.0;
    powerBufferPool_t.mid_water_level = 50.0;
    powerBufferPool_t.period = 0.01;//�������ڣ���λ/s
    powerBufferPool_t.high_current_threshold = 7.0;//A
    powerBufferPool_t.mid_current_threshold = 2.0;//A
    powerBufferPool_t.low_current_threshold = 0.9;//A
    powerBufferPool_t.safe_current_threshold = 0.7;//A
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
        wheel1Speed_t.motor_lim = RM3508_LIM;//3508��������Χ
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
        wheel2Speed_t.motor_lim = RM3508_LIM;//3508��������Χ
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
			chassis_t.State.last_area = MID_ROAD;//��ӳ���������Ƿ�λ����ȷ
      chassis_t.State.out_of_flag =AUTO_CONTROL_OK;
     /* ------ ����������־λ ------- */  
        SET_BIT(chassis_t.status,START_OK);  
  }
	/**
		* @Data    2019-03-30 17:16
		* @brief   ���������������ȡ
		* @param   void
		* @retval  void
		*/
uint8_t up_turn = 1;
 uint8_t road_flag =0;
	void SetArea(void)
	{
//	if(GetOrgans() != 0)
//	{
   up_turn =GetOrgans();
//	}
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
int16_t organs_flag =1;//��ʼ��Ϊ1�����ǳ�ʼ��Ϊ��·
int16_t organs = 1;
 int16_t organssss = 0; 
int16_t temp_organs =0;
  int16_t temp_organssss =0;
	uint8_t GetOrgans(void)
	{
    	 temp_organs = HAL_GPIO_ReadPin(LASER_SWITCH_GPIO,LASER_SWITCH);//��io��
////    if(GetGyroDire() >= TURNING_ANGLE)
////         organs_flag =1;
////    if(GetGyroDire() <TURNING_ANGLE &&organs_flag ==1)
////       organs_flag =0;
   	if(temp_organs ==0)//���⿪��
	  {
		  organs_flag ++;
	  }
    
    if(GetGyroDire() >= TURNING_ANGLE)
    {
      if(organs_flag >10)
      {
          organs_flag  =1;//����·����
        temp_organssss=0;
      }
      else if(temp_organs ==1)
      {
        temp_organssss ++;
        if(temp_organssss >5000)
        organs_flag  =0;//����·����
        else organs_flag =1;
      }
      
    }
//    else if(GetGyroDire() <= TURNING_ANGLE)
//    {
//      if(organs_flag ==1)
//      {
//        
//      }
//    }
    
      
     return  organs_flag;
    
    
//    if(GetGyroDire() >= TURNING_ANGLE  && organs_flag >10 &&organs_flag ==0)//
//    {
//      
////      if(GetGyroDire() <= 50  && temp_organs == 0) //1�Ǳ���ס��0��û��ס
////      {
//        organs_flag  =1;//����·����
//        organssss =1;
////      }
//    }
//      else if(GetGyroDire() >= TURNING_ANGLE  && organssss ==1 &&temp_organs == 1)
//      {
//        organs_flag = 0;
//        organssss =0;
//      }
//      return  organs_flag;
		// temp_organs = HAL_GPIO_ReadPin(LASER_SWITCH_GPIO,LASER_SWITCH);//��io��
//  	if(temp_organs ==0)//���⿪��
//	{
//		organs_flag ++;
//	}
//	else organs_flag --;
//	if(organs_flag <0)
//    return  0;  
//	else if(organs_flag >30)
//   	return  1; 
//    if(temp_organs !=0 )
//    {
//      organs = 0;
//    }
//    
//    return  organs;
		// uint8_t temp;
//	if(organs ==0)//���⿪��
//	{
//		organs_flag ++;
//	}
//	else organs_flag --;
//	if(organs_flag <0)
//    return  0;  
//	else if(organs_flag >30)
//   	return  1; 
//  else   return  0; 
	}
	/**
		* @Data    2019-03-30 17:16
		* @brief   �����Լ�
		* @param   void
		* @retval  void
		*/
// uint8_t AutoCalibratorMode(void)
// {
//   
// }
//	/**
//		* @Data    2019-03-30 17:16
//		* @brief   ���������ݴ���
//		* @param   void
//		* @retval  void
//		*/
//int16_t soncin3;
//int16_t soncin4;
//int16_t t_soncin3;
//int16_t t_soncin4;
//  int16_t sum3;
//  int16_t sum4;
//  int16_t s3;
//  int16_t s4;
// int16_t temp_s1;
// int16_t temp_s2;
// int16_t last_s3;
//void SetUltrasonic(void)
//{

////  rightSonicQu_t.deQueue(&rightSonicQu_t,&t_soncin4);
//  for(int i=0;i< SONIC_QU_SIZE;i++)
//  {
//     sum3+=leftSonicQu_t.data[i];
//  }
////  for(int i=0;i< SONIC_QU_SIZE;i++)
////  {
////     sum4+=rightSonicQu_t.data[i];
////  }
//  s3 = (int16_t)(sum3/10.0);
////  s4 = (int16_t)(sum4/10.0); 
//  soncin3 = (int16_t)GetDistance(3);
////  soncin4 = (int16_t)GetDistance(4);
//  temp_s1 = soncin3- s3 ;
////  temp_s2 = soncin4-s4;
//  if(ABS(temp_s1) >40)
//    soncin3 = s3;
////  if(ABS(temp_s2) >40)
////    soncin4 = s4;
//   leftSonicQu_t.enQueue(&leftSonicQu_t,soncin3);
////   rightSonicQu_t.enQueue(&rightSonicQu_t,soncin4);
//  chassis_t.left_dis_son = soncin3;
////  chassis_t.right_dis_son = soncin4;
//}

int16_t dddddd=0;
int16_t dddddd11=18;
uint8_t ddssfsflag =0;
int16_t ereeeer=0;


int16_t temp_area1=0;
int16_t temp_area2=0;
int16_t temp_area3=0;
int16_t temp_area4=0;
int16_t temp_area5=0;
int16_t jiujimoshi(void)//��ʱ���⣬�޸�һ��dddddd�Ĵ�С
{
  //���ð汾��     ��ת���ʱ����ͣ�����ͣ�����ܷ�
//  ereeeer = GetGyroDire();
//   if(ereeeer <= dddddd11 &&ddssfsflag ==0 && dddddd< 1)
//   {
//     dddddd =1000;
//      ddssfsflag =1;
//   }
//   else if(ereeeer<= dddddd11 &&ddssfsflag ==1 && dddddd< 1)
//   {
//     dddddd =1000;
//      ddssfsflag =0;
//   }
//   else 
//   {
//     if(dddddd<0)
//       dddddd=0;
//     else dddddd--;
//   }
//   return ddssfsflag;
  //-----------------------------------------------------------
   //��ʱ�޸ģ�������
    temp_area5 = GetGyroDire();
  temp_area1 = HAL_GPIO_ReadPin(LASER_SWITCH_GPIO,LASER_SWITCH);//��io��
   if(temp_area1 ==0)
   {
     temp_area2 ++;
   }
   if(temp_area2>10)
      ddssfsflag =1;
   if(temp_area5 >28 && temp_area1==1 && ddssfsflag==1)
   {
     if(temp_area3>2000)
     {
       temp_area2=0;
       temp_area3 =2000;
     }
     else temp_area3++;
   }
   if(temp_area5 <=dddddd11 && temp_area3 >1900)
   {
     ddssfsflag =0;
   }
    return ddssfsflag;
}
	/**
		* @Data    2019-04-14 16:56
		* @brief   ���̴��������ݽ���
		* @param   void
		* @retval  void
		*/
void ChassisSensorParse(uint8_t *data)
{
  floatToUnion p;
  chassis_t.Sensor.lim_sw_left = data[0]>>4 & 0x0f;
  chassis_t.Sensor.lim_sw_right = data[0] & 0x0f;
  if(data[1] >AUTO_CONTROL_OK)
  {
   chassis_t.State.out_of_flag =OUT_OF_CONTROL;
  }
  chassis_t.Sensor.laser_sw_left = data[1]>>4 & 0x0f;
  chassis_t.Sensor.laser_sw_left = data[1] & 0x0f;
  
   chassis_t.Sensor.sonic_left = data[2];
   chassis_t.Sensor.sonic_right = data[3];
  
  p.u_8[0] = data[4];
  p.u_8[1] = data[5];
  p.u_8[2] = data[6];
  p.u_8[3] = data[7];
  
  chassis_t.Sensor.encoder = p.s_32;
}
/**
	* @Data    2019-03-30 22:13
	* @brief   �ܵ�ָ��Ŀ�ĵ�
	* @param   void
	* @retval  void
	*/
	int16_t Go(int16_t target,int16_t speed)
	{
		int16_t dire;
		dire =chassis_t.State.r_area  - target;
			if(ABS(dire) >1)
			dire =(int16_t)( dire *0.5);
			speed = dire * speed;
			SetMotorTarget(speed,speed); 
			return dire;


	}
/*----------------------------------file of end-------------------------------*/
//������
//void midrun(void)
//{
//  
//}



