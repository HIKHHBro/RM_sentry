/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,海康平头哥,                       |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-19               
	|--Libsupports : 
	|--Description :  3508 顺时针为正                                                     
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
/* -------- 设置失常常和掉线模式 --------- */
  
/* -------------- 外部链接 ----------------- */
extern	osThreadId startChassisTaskHandle;
/* -------------- 发送队列 ----------------- */
  xQueueHandle chassis_queue;
/* -------------- 静态变量 ----------------- */
	 chassisStruct chassis_t;
    static RM3508Struct wheel1_t;         //轮子1
      static speedPidStruct wheel1Speed_t;
      static postionPidStruct wheel1postion_t;
    static RM3508Struct wheel2_t;         //轮子2
      static speedPidStruct wheel2Speed_t;
       static postionPidStruct wheel2postion_t;
     static powerBufferPoolStruct powerBufferPool_t;
			static currentMeterStruct currtenMeter_t;
      static gyroStruct ChassisGyro_t;
      static incrementalEnconderStruct chassisEnconder_t;
        
  //  Int16Queue leftSonicQu_t;
  //   Int16Queue rightSonicQu_t;
// /*-------------------------------------------------------
//   |       型号     |  线数 |  brown | bule  |black|while|
//   -------------------------------------------------------
//   |欧姆龙 E6A2-CW3C|500P/R |5to12VDC|0V(GND)|OUT A|OUT B|
//  -------------------------------------------------------*/
// 		static incrementalEnconderStruct chassisEnconder_t; //编码器结构体
/* -------------- 私有宏 ----------------- */
	#define CHASSIS_QUEUE_LEN      5U//深度为5
	#define CHASSIS_QUEUE_SIZE    8U//长度为5;
	/**
	* @Data    2019-01-27 17:09
	* @brief   底盘结构体数据初始化
	* @param  CAN_HandleTypeDef* hcanx（x=1,2）
	* @retval  void
	*/
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc,const pcDataStruct* pPc_t)
	{
		chassis_t.hcanx = hcan;
    chassis_t.rc_t =  rc;
		chassis_t.pPc_t = pPc_t;
    chassis_t.status = 0;
    chassis_t.p_refereeSystem_t = &ext_refereeSystem_t;
    chassis_t.pGyro_t  = &ChassisGyro_t;
    SetFpsAddress(chassis_t.pGyro_t->fps);
    chassis_t.ppowerBufferPool_t = PowerBufferPoolInit();
		/* ------ 轮子1结构体数据初始化 ------- */
		chassis_t.pwheel1_t = wheel1Init();
		/* ------ 轮子2结构体数据初始化 ------- */
		chassis_t.pwheel2_t = wheel2Init();
  /* ------ 创建云台发送队列 ------- */
	  chassis_queue	= xQueueCreate(CHASSIS_QUEUE_LEN,CHASSIS_QUEUE_SIZE);//一定要在用之前创建队列
    /* -------- can配置 --------- */
     if(UserCanConfig(hcan)!= HAL_OK)
     {
      while(1)
      {
//        HAL_GPIO_WritePin(COLOR_LED_GPIO,RED_LED,GPIO_PIN_RESET);
//       HAL_GPIO_WritePin(COLOR_LED_GPIO,GREEN_LED,GPIO_PIN_RESET);
      }//待添加防重复配置功能
    }
		 ///* ------ 编码器初始化 ------- */
	 	 chassis_t.pchassisEnconder_t = &chassisEnconder_t;
		 if(EnconderInit(&chassisEnconder_t,EN_RADIUS,ENCONDER_POLES) !=HAL_OK)
     {
//            while(1){
//              HAL_GPIO_WritePin(COLOR_LED_GPIO,RED_LED,GPIO_PIN_RESET);
//              HAL_GPIO_WritePin(COLOR_LED_GPIO,GREEN_LED,GPIO_PIN_RESET);
//                    }
     }
      //TrackparseInit(5436,60);
		/* ------ 超声波初始化 ------- */
	  	HCSR04Init();
      // Int16QueueCreate(&leftSonicQu_t,SONIC_QU_SIZE);
      // Int16QueueCreate(&rightSonicQu_t,SONIC_QU_SIZE);
    /* -------- 底盘模块初始化判断 --------- */
      SET_BIT(chassis_t.status,INIT_OK);//初始化成功
	/* ------ 挂起任务，等待初始化 ------- */
    	vTaskSuspend(startChassisTaskHandle);
	/* ------ 设置机器人初始化状态 ------- */
     SetSetInitStatus();
//    vTaskSuspend(startChassisTaskHandle);
	}
/**
	* @Data    2019-01-28 11:40
	* @brief   底盘数据解析
	* @param   void
	* @retval  void
	*/
	void ChassisParseDate(uint16_t id,uint8_t *data)
	{
		switch (id)
		{
			case WHEEL1_RX_ID:
				RM3508ParseData(&wheel1_t,data);
        Fps(wheel1_t.fps);
				break;
			case WHEEL2_RX_ID:
				RM3508ParseData(&wheel2_t,data);
        Fps(wheel2_t.fps);
				break;
	  	case CURRENT_METER_RX_ID:
				 CurrentMeterAnalysis(&currtenMeter_t,data);
         Fps(currtenMeter_t.fps);
				break;
      case CHASSIS_RX_ID:
       BingeGyroByCan(chassis_t.pGyro_t,data);
       Fps(chassis_t.pGyro_t->fps);
        break;
			default:
				break;
		}
    
    SET_BIT(chassis_t.status,RX_OK);//接受成功
	}
///**
//	* @Data    2019-02-15 15:10
//	* @brief   底盘数据发送
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
* @brief   获取底盘结构体地址
* @param   void
* @retval  void
*/
const chassisStruct* GetChassisStructAddr(void)
{
  return &chassis_t;
}
  /**
  * @Data    2019-03-13 03:48
  * @brief   获取底盘结构体地址 可读写，不能乱调用
  * @param   void
  * @retval  void
  */
  chassisStruct *RWGetChassisStructAddr(void)
  {
    return &chassis_t;
  }
  /**
  * @Data    2019-03-13 01:55
  * @brief   获取底盘状态
  * @param   void
  * @retval  void
  */
  uint32_t GetChassisStatus(void)
  {
    return chassis_t.status;
  }
  /**
  * @Data    2019-03-13 02:56
  * @brief   设置电机目标速度
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
  * @brief   控制优先权决策
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
	* @brief   云台数据发送
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
* @Data    2019-03-21 00:46
* @brief   底盘模块失能//待完善
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
  * @brief   初始化功率缓存池
  * @param   void
  * @retval  void
  */
  powerBufferPoolStruct* PowerBufferPoolInit(void)
  {
    powerBufferPool_t.pcurrentMeter_t = &currtenMeter_t;
     powerBufferPool_t.p_refereeSystem_t = &ext_refereeSystem_t;
    SetFpsAddress(powerBufferPool_t.pcurrentMeter_t->fps);
    powerBufferPool_t.max_p = 20.0;
    powerBufferPool_t.max_w = 200.0;//功率单位mW
    powerBufferPool_t.r_p = 0.0;
    powerBufferPool_t.r_w = 200.0;//功率单位mW
    powerBufferPool_t.current_mapp_coe = 0.00122;//电流映射系数
    powerBufferPool_t.high_water_level = 100.0;
    powerBufferPool_t.low_water_level = 30.0;
    powerBufferPool_t.mid_water_level = 50.0;
    powerBufferPool_t.period = 0.006;//运行周期，单位/s
    powerBufferPool_t.high_current_threshold = 6.0;//A
    powerBufferPool_t.mid_current_threshold = 4.0;//A
    powerBufferPool_t.low_current_threshold = 1.5;//A
    powerBufferPool_t.safe_current_threshold = 0.65;//A
    return &powerBufferPool_t;
  }
  /**
  * @Data    2019-03-25 00:12
  * @brief   初始化轮子1参数
  * @param   void
  * @retval  void
  */
  RM3508Struct* wheel1Init(void)
  {
      wheel1_t.id = WHEEL1_RX_ID;//电机can的 ip
//			wheel1_t.target = 0;		 //目标值
			wheel1_t.tem_target = 0;//临时目标值
			wheel1_t.real_current = 0; //真实电流
			wheel1_t.real_angle = 0;//真实角度
			wheel1_t.tem_angle = 0;//临时角度
  	  wheel1_t.real_speed = 0;//真实速度
  	  wheel1_t.tem_speed = 0;//真实速度
			wheel1_t.zero = 0;			 //电机零点
			wheel1_t.Percentage = 0;//转换比例（减速前角度:减速后的角度 = x:1
			wheel1_t.thresholds = 0; //电机反转阀值
  	  wheel1_t.error = 0;//当前误差
      SetFpsAddress(wheel1_t.fps); 
    
      wheel1_t.ppostionPid_t = &wheel1postion_t;
      wheel1_t.pspeedPid_t = &wheel1Speed_t;
//				wheel1Speed_t.kp = 1;
//				wheel1Speed_t.kd = 0;
//				wheel1Speed_t.ki = 0;
				wheel1Speed_t.error = 0;
				wheel1Speed_t.last_error = 0;//上次误差
				wheel1Speed_t.before_last_error = 0;//上上次误差
				wheel1Speed_t.integral_er = 0;//误差积分
				wheel1Speed_t.pout = 0;//p输出
				wheel1Speed_t.iout = 0;//i输出
				wheel1Speed_t.dout = 0;//k输出
				wheel1Speed_t.pid_out = 0;//pid输出
        wheel1Speed_t.motor_lim = RM3508_LIM;//3508最大电流范围
        	wheel1_t.target = 0;		 //目标值
    		wheel1Speed_t.kp = 4;
				wheel1Speed_t.kd = 0;
				wheel1Speed_t.ki = 0.2;
     	wheel1Speed_t.limiting = W1_LIMIT_SPEED;//轮子1速度限幅
//				wheel1Speed_t.limiting = W1_LIMIT_SPEED;//轮子1速度限幅
/* -------- 外环 --------- */
        wheel1postion_t.integral_limint = 2000;
        wheel1postion_t.integral_threshold =0;
        wheel1postion_t.kp_separatecmd = 0;
        wheel1postion_t.motor_lim = RM3508_LIM;
        wheel1postion_t.kd = 0;
        wheel1postion_t.ki = 0;
        wheel1postion_t.kp =-4;
        return &wheel1_t;
  }
  /**
  * @Data    2019-03-25 00:15
  * @brief   轮子2参数初始化
  * @param   void
  * @retval  void
  */
  RM3508Struct* wheel2Init(void)
  {
			wheel2_t.id = WHEEL2_RX_ID;//电机can的 ip
//			wheel2_t.target = 0;		 //目标值
			wheel2_t.tem_target = 0;//临时目标值
			wheel2_t.real_current = 0; //真实电流
			wheel2_t.real_angle = 0;//真实角度
			wheel2_t.tem_angle = 0;//临时角度
			wheel2_t.real_speed = 0;//真实速度
			wheel2_t.tem_speed = 0;//真实速度
			wheel2_t.zero = 0;			 //电机零点
			wheel2_t.Percentage = 0;//转换比例（减速前角度:减速后的角度 = x:1
			wheel2_t.thresholds = 0; //电机反转阀值
			wheel2_t.error = 0;//当前误差
    
    SetFpsAddress(wheel2_t.fps); 
    
 			wheel2_t.ppostionPid_t = &wheel2postion_t;
      wheel2_t.pspeedPid_t = &wheel2Speed_t;
//				wheel2Speed_t.kp = 1;
//				wheel2Speed_t.kd = 0;
//				wheel2Speed_t.ki = 0;
				wheel2Speed_t.error = 0;
				wheel2Speed_t.last_error = 0;//上次误差
				wheel2Speed_t.before_last_error = 0;//上上次误差
				wheel2Speed_t.integral_er = 0;//误差积分
				wheel2Speed_t.pout = 0;//p输出
				wheel2Speed_t.iout = 0;//i输出
				wheel2Speed_t.dout = 0;//k输出
				wheel2Speed_t.pid_out = 0;//pid输出
        wheel2Speed_t.motor_lim = RM3508_LIM;//3508最大电流范围
//				wheel2Speed_t.limiting = W2_LIMIT_SPEED;//轮子2速度限幅
    	wheel2_t.target = 0;		 //目标值
    	wheel2Speed_t.kp = 4;
			wheel2Speed_t.kd = 0;
			wheel2Speed_t.ki = 0.2;
    	wheel2Speed_t.limiting = W2_LIMIT_SPEED;//轮子2速度限幅
/* -------- 外环 --------- */
        wheel2postion_t.integral_limint = 2000;
        wheel2postion_t.integral_threshold =0;
        wheel2postion_t.kp_separatecmd = 0;
        wheel2postion_t.motor_lim = RM3508_LIM;
        wheel2postion_t.kd = 0;
        wheel2postion_t.ki = 0;
        wheel2postion_t.kp =-4;
      return &wheel2_t;
  }
 /**
	 * @Data    2019-03-19 17:58
	 * @brief   底盘初始化状态设置
	 * @param   void
	 * @retval  void
	 */
 	void SetSetInitStatus(void)
  {
    	wheel1_t.target = 0;		 //目标值
    		wheel1Speed_t.kp = 9;
				wheel1Speed_t.kd = 1.5;
				wheel1Speed_t.ki = 0.9;
     	wheel1Speed_t.limiting = W1_LIMIT_SPEED;//轮子1速度限幅
     wheel1postion_t.kd = -5;
    wheel1postion_t.ki = 0;
    wheel1postion_t.kp = -18;
    wheel1postion_t.motor_lim = 5000;

    
    	wheel2_t.target = 0;		 //目标值
    	wheel2Speed_t.kp = 9;
			wheel2Speed_t.kd = 1.5;
			wheel2Speed_t.ki = 0.9;
    	wheel2Speed_t.limiting = W2_LIMIT_SPEED;//轮子2速度限幅

    wheel2postion_t.kd = -5;
    wheel2postion_t.ki = 0;
    wheel2postion_t.kp = -18;
    wheel2postion_t.motor_lim = 5000;

		/* ------ 设置初始化区域位置 ------- */
     /* ------ 设置启动标志位 ------- */  
        SET_BIT(chassis_t.status,START_OK);  
  }
	/**
		* @Data    2019-03-30 17:16
		* @brief   机器人所处区域获取
		* @param   void
		* @retval  void
		*/
  uint32_t temp_are =0,temp_are1=0,temp_are2=0;
	void SetArea(void)
	{
    uint32_t state;
    state = SetAByEncoder();
    if(state == UP_ROAD)
      {
        if(!IS_ROAD(UP_ROAD))
        {
           temp_are = CLEAR_BIT(GET_ALL_STATE,LAST_AREA);
          SET_CHASSIS_LAST_SELF_POS(GET_CHASSIS_SELF_POS(REAL_AREA));
        }
        SET_CHASSIS_REAL_SELF_POS(UP_ROAD);
      }
    else if(state == MID_ROAD)
		{
       if(!IS_ROAD(MID_ROAD))
       {
           temp_are1 = CLEAR_BIT(GET_ALL_STATE,LAST_AREA);
          SET_CHASSIS_LAST_SELF_POS(GET_CHASSIS_SELF_POS(REAL_AREA));
       }
        SET_CHASSIS_REAL_SELF_POS(MID_ROAD);
		}
    else if(state == DOWN_ROAD)
    {
        if(!IS_ROAD(DOWN_ROAD))
        {
          temp_are2 = CLEAR_BIT(GET_ALL_STATE,LAST_AREA);
          SET_CHASSIS_LAST_SELF_POS(GET_CHASSIS_SELF_POS(REAL_AREA));
        }
        SET_CHASSIS_REAL_SELF_POS(DOWN_ROAD);
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
		return ABS(CalculateError(chassis_t.r_dire,\
                             (int16_t)chassis_t.pGyro_t->Yaw,\
                              DIRE_SU,GYRO_MAX_ANGLE));
	}
int16_t organs_flag =1;//初始化为1，既是初始化为上路
int16_t organs = 1;
 int16_t organssss = 0; 
int16_t temp_organs =0;
  int16_t temp_organssss =0;
 	uint8_t GetOrgans(void)
	{
    int16_t temp_dire;
    temp_dire = GetGyroDire();
     if(LaserSwBef() ==1 && temp_dire >10 && temp_dire< 40)
       return DOWN_ROAD;
     else if(LaserSwBack() ==1 && temp_dire >10 && temp_dire< 40)
       return UP_ROAD;
     else 
     {
       if(temp_dire >45)
       return MID_ROAD;
       return 5;//错误
     }
	}


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
int16_t jiujimoshi(void)//暂时问题，修改一下dddddd的大小
{
  //可用版本会     在转弯的时候不能停掉如果停，会跑飞
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
   //临时修改，待测试
    temp_area5 = GetGyroDire();
 // temp_area1 = HAL_GPIO_ReadPin(LASER_SWITCH_GPIO,LASER_SWITCH);//读io口
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
		* @brief   底盘传感器数据解析
		* @param   void
		* @retval  void
		*/
void ChassisSensorParse(void)
{
  uint8_t data[1];
   data[0] = LimSw();
  if((data[0]>>4 & 0x0f) ==0 && data[0] !=0)
  chassis_t.Sensor.lim_sw_left = 1;
  else chassis_t.Sensor.lim_sw_left = 0;
   if((data[0] & 0x0f) ==0 && data[0] !=0)
  chassis_t.Sensor.lim_sw_right = 1;
   else   chassis_t.Sensor.lim_sw_right = 0;
  if(IS_STATE(CORRET_ENCODE) && data[0]>0)
     SET_STATE(OUT_OF_CONTROL);
//  chassis_t.Sensor.laser_sw_left = data[1]>>4 & 0x0f;
//  chassis_t.Sensor.laser_sw_left = data[1] & 0x0f;
//  
//   chassis_t.Sensor.sonic_left = data[2];
//   chassis_t.Sensor.sonic_right = data[3];
//  
//  p.u_8[0] = data[4];
//  p.u_8[1] = data[5];
//  p.u_8[2] = data[6];
//  p.u_8[3] = data[7];
//  
  chassis_t.Sensor.encoder = GetPosition(chassis_t.pchassisEnconder_t);  
}
/**
	* @Data    2019-03-30 22:13
	* @brief   跑到指定目的地
	* @param   void
	* @retval  void
	*/
	int16_t Go(int16_t target,int16_t speed)
	{
		int16_t dire;
		dire =GET_CHASSIS_SELF_POS(REAL_AREA)  - target;
			if(ABS(dire) >1)
			dire =(int16_t)( dire *0.5);
			speed = dire * speed;
			SetMotorTarget(speed,speed); 
			return dire;
	}
  int16_t flag_toback,flag_max=150,dire_temp=1;
  
int16_t Toback(int16_t speed)
  {
    if(flag_toback>flag_max)
    {
      dire_temp = -dire_temp;
      flag_toback=0;
    } 
    else flag_toback++;
    return speed*dire_temp;
  }
/**
	* @Data    2019-03-30 22:13
	* @brief   编码器跑到指定位置
	* @param   void
	* @retval  void
	*/
  int16_t GoToPiont(int16_t piont,int16_t *pid_out)
  {
    *(pid_out+0)= PostionPid(chassis_t.pwheel1_t->ppostionPid_t, CalculateError(piont,chassis_t.Sensor.encoder,4000,5000));
    *(pid_out+1) = PostionPid(chassis_t.pwheel2_t->ppostionPid_t, CalculateError(piont,chassis_t.Sensor.encoder,4000,5000));
    return ABS(chassis_t.pwheel1_t->ppostionPid_t->error);
  }
 /**
	* @Data    2019-03-30 22:13
	* @brief   编码器值确定区域
	* @param   void
	* @retval  void
	*/ 
uint32_t SetAByEncoder(void)
{
  if(chassis_t.Sensor.encoder <=chassis_t.track.up_turn_len)
    return UP_ROAD;
  else if(chassis_t.Sensor.encoder > chassis_t.track.up_turn_len &&chassis_t.Sensor.encoder <chassis_t.track.down_turn_len)
    return MID_ROAD;
  else return DOWN_ROAD;
}
 /**
	* @Data    2019-03-30 22:13
	* @brief   判断编码器值是否偏差过大，并校准
	* @param   void
	* @retval  void
	*/ 
int16_t u_flag =0,u_flag4=0,max_u_flag = 100;
void EncodeDetector(void)
{
if(IS_OPEN_INT(GetGyroDire(),(chassis_t.track.half_angle-3),(chassis_t.track.half_angle+3)))
{
  if(chassis_t.Sensor.encoder <  chassis_t.track.half_len-100)
     CalibratingEncoder(chassis_t.pchassisEnconder_t,chassis_t.track.up_turn_len);
  else if(chassis_t.Sensor.encoder > chassis_t.track.half_len +100)
    CalibratingEncoder(chassis_t.pchassisEnconder_t,chassis_t.track.down_turn_len);
}
else if(chassis_t.Sensor.lim_sw_left ==1)
{
   CalibratingEncoder(chassis_t.pchassisEnconder_t,chassis_t.track.total_len);
}
else if(chassis_t.Sensor.lim_sw_right ==1)
{
   CalibratingEncoder(chassis_t.pchassisEnconder_t,0);
}
//待测试
else if(GetDistance(3) > 100)
{
  u_flag++;
  if(u_flag >max_u_flag)
  {
    CalibratingEncoder(chassis_t.pchassisEnconder_t,chassis_t.track.total_len - (int32_t)GetDistance(3));
    u_flag =0;
  }
}
else if(GetDistance(4) >100)
{
  u_flag4++;
  if(u_flag4 >max_u_flag)
  {
    CalibratingEncoder(chassis_t.pchassisEnconder_t,(int32_t)GetDistance(4));
    u_flag4 =0;
  }
}
  else 
  {
    if(u_flag <0)
     u_flag = 0;
    u_flag--;
    if(u_flag4 <0)
     u_flag4 = 0;
    u_flag4--;
  }
  //    lefts = (int16_t)GetDistance(3);
  //rights = (int16_t)GetDistance(4);
}

 /**
	* @Data    2019-03-30 22:13
	* @brief   自动校准所有数据
	* @param   void
	* @retval  void
	*/ 
  MOD_Status AutoCalibrationData(void)
{
  if(chassis_t.Sensor.lim_sw_left ==1)
  {
     CalibratingEncoder(chassis_t.pchassisEnconder_t,chassis_t.track.total_len);
     chassis_t.pwheel1_t->target =chassis_t.pwheel2_t->target = 0;
      SET_CHASSIS_LAST_SELF_POS(MID_ROAD);
    SET_CHASSIS_REAL_SELF_POS(DOWN_ROAD);
    return MOD_OK;
  }
  else if(chassis_t.Sensor.lim_sw_right ==1)
  {
     CalibratingEncoder(chassis_t.pchassisEnconder_t,0);
     chassis_t.r_dire = chassis_t.pGyro_t->Yaw;
    chassis_t.pwheel1_t->target =chassis_t.pwheel2_t->target = 0;
    SET_CHASSIS_LAST_SELF_POS(MID_ROAD);
    SET_CHASSIS_REAL_SELF_POS(UP_ROAD);
       return MOD_OK;
  }
  else 
//    chassis_t.pwheel1_t->target =chassis_t.pwheel2_t->target = 1000;
//    chassis_t.pwheel1_t->error = chassis_t.pwheel1_t->target - chassis_t.pwheel1_t->real_speed;
//    chassis_t.pwheel2_t->error = chassis_t.pwheel2_t->target - chassis_t.pwheel2_t->real_speed;
//    SpeedPid(chassis_t.pwheel1_t->pspeedPid_t,chassis_t.pwheel1_t->error);
//    SpeedPid(chassis_t.pwheel2_t->pspeedPid_t,chassis_t.pwheel2_t->error);
//   chassis_t.pwheel1_t->pspeedPid_t->pid_out= MAX(chassis_t.pwheel1_t->pspeedPid_t->pid_out,4000);
//   chassis_t.pwheel1_t->pspeedPid_t->pid_out= MIN(chassis_t.pwheel1_t->pspeedPid_t->pid_out,-4000);
//    chassis_t.pwheel2_t->pspeedPid_t->pid_out =MAX(chassis_t.pwheel2_t->pspeedPid_t->pid_out,4000);
//    chassis_t.pwheel2_t->pspeedPid_t->pid_out = MIN(chassis_t.pwheel2_t->pspeedPid_t->pid_out,-4000);
//		ChassisCanTx(chassis_t.pwheel1_t->pspeedPid_t->pid_out,chassis_t.pwheel2_t->pspeedPid_t->pid_out);
    return MOD_ERROR;
}
 /**
	* @Data    2019-03-30 22:13
	* @brief   轨道数据初始化
	* @param   void
	* @retval  void
	*/ 
void TrackparseInit(int16_t tatol_len,int16_t angle)
{
     chassis_t.track.total_len =1670 ;//tatol_len;//总长度
     chassis_t.track.angle = angle;//轨道角度 4
     chassis_t.track.up_road_len =520-300; //(int16_t)(0.246 *tatol_len);//中路长度1
     chassis_t.track.mid_road_len = 1000-300; //(int16_t)(0.317 *tatol_len);//中路长度
     chassis_t.track.dowm_road_len = 400-300;//chassis_t.track.up_road_len;//下路长度
     chassis_t.track.turning_len =523;//拐弯长度//实验室423 3

     chassis_t.track.half_angle = (int16_t)(chassis_t.track.angle*0.5);//轨道半角度 5
     chassis_t.track.turning_angle = chassis_t.track.half_angle;//拐弯角度 6
     chassis_t.track.half_len = (int16_t)(tatol_len*0.5);//轨道半长度

     chassis_t.track.up_turn_len = 500+ (int16_t)(chassis_t.track. turning_len*0.5)-350;//实验室//chassis_t.track.up_road_len + (int16_t)(chassis_t.track. turning_len*0.5);//上路拐弯距离比赛//实验室274  7
     chassis_t.track.down_turn_len = chassis_t.track.up_turn_len + (int16_t)(chassis_t.track. turning_len*0.5) + 1000-300-150;//下路拐弯距离//chassis_t.track.up_turn_len + (int16_t)(chassis_t.track. turning_len*0.5) +  chassis_t.track.mid_road_len-300;//下路拐弯距离//实验室1512
     chassis_t.track.end = chassis_t.track.total_len - 600;//终点死区
     chassis_t.track.start = 300;//起点死区 2
}
/*----------------------------------file of end-------------------------------*/
//测试区



