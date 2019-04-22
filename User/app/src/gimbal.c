/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,海康平头哥,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : gimbal.c                                              
	|--Version     : v2.0                                                          
	|--Author      : 海康平头哥                                                     
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
/* -------------- 结构体声明 ----------------- */
	gimbalStruct gimbal_t;//云台结构体
  RM6623Struct yaw_t;//yaw轴电机
	postionPidStruct yawOuterLoopPid_t;//yaw 电机外环pid
	speedPidStruct yawInnerLoopPid_t;//yaw 电机内环pid
	GM6020Struct pitch_t;//pitch轴电机
	postionPidStruct pitchOuterLoopPid_t;//pitch 电机外环pid
	speedPidStruct pitchInnerLoopPid_t;//pitch 电机内环pid
  
  static gy955Struct gimbalGyroByCan_t;
/* -------------- 任务句柄 ----------------- */
		osThreadId startRammerTaskHandle; 
/* -------------- 外部链接 ----------------- */
		extern osThreadId startGimbalTaskHandle;
/* -------------- 发送队列 ----------------- */
  xQueueHandle gimbal_queue;
/* ----------------- 任务钩子函数 -------------------- */
	void StartRammerTask(void const *argument);
        
 /* -------------- 软件定时器 ----------------- */       
osTimerId gimbalGetStatusTimerHandle;
void GimbalGetStatusCallback(void const * argument);
/* ------------ 记录云台历史状态队列 ---------------- */
Int16Queue yawStatusQu;
Int16Queue pitchStatusQu;
/* ----------------- 临时变量 -------------------- */
    int16_t taddd =0;
    int16_t xianfuweizhi =3000;
    int16_t xianfusudu = 5000;//STUCK_BULLET_THRE
    int16_t yawxianfu = 1900;
		int16_t pitchxianfu =10000;
		int16_t sudupitchxianfu =10000;
	/**
	* @Data    2019-01-27 17:09
	* @brief   云台结构体初始化
	* @param  CAN_HandleTypeDef* hcanx（x=1,2）
	* @retval  void
	*/
	void GimbalStructInit(const dbusStruct* pRc_t,const pcDataStruct* pPc_t)
	{
		gimbal_t.pRc_t = pRc_t;//遥控数据地址
    gimbal_t.pPc_t = pPc_t;//小电脑数据地址
    gimbal_t.status = MOD_READ;
    gimbal_t.prammer_t =RammerInit();
		gimbal_t.pYaw_t = YawInit();
		gimbal_t.pPitch_t = PitchInit();
    gimbal_t.pGyro_t = &gimbalGyroByCan_t;
  /* ------ 开启摩擦轮 ------- */
		BrushlessMotorInit();
//    UserCanConfig(GIMBAL_CAN);
  /* ------ 创建云台发送队列 ------- */
	  gimbal_queue	= xQueueCreate(QUEUE_LEN,QUEUE_SIZE);//一定要在用之前创建队列
  /* ------ 创建云台历史状态队列 ------- */
    Int16QueueCreate(&yawStatusQu,QUEUE_YAW_STATUS_SIZE);
    Int16QueueCreate(&pitchStatusQu,QUEUE_PITCH_STATUS_SIZE);
    
    //测试----------------------
   // UserCanConfig(GY955_CAN);
    //--------------------
    
    
 	/* ------ 设置初始化标志位 ------- */
		SET_BIT(gimbal_t.status,INIT_OK);
	/* ------ 挂起任务，等待初始化 ------- */
		vTaskSuspend(startGimbalTaskHandle);
	/* ------ 设置机器人初始化状态 ------- */
		SetGimBalInitStatus();
 /* ------ 创建拨弹任务 ------- */
    osThreadDef(startRammerTask,StartRammerTask,osPriorityNormal,0,RAMMER_HEAP_SIZE);
    startRammerTaskHandle = osThreadCreate(osThread(startRammerTask),NULL);
    osStatus timerresult = osOK;
    osTimerDef(GimbalGetStatusTimer, GimbalGetStatusCallback);
    gimbalGetStatusTimerHandle = osTimerCreate(osTimer(GimbalGetStatusTimer), osTimerPeriodic, NULL);
    timerresult = osTimerStart(gimbalGetStatusTimerHandle,MEMORY_SPAN);
    if(timerresult !=osOK)
    {
      //添加报错机制
    }
 		//vTaskSuspend(startGimbalTaskHandle);
	}
/**
	* @Data    2019-01-28 11:40
	* @brief   云台数据解析
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
  #ifdef ANTI_CLOCK_WISE  //逆时针为正方向
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
////---------------------临时
//void GyroParse(uint32_t id,uint8_t *data)
//{
//  if(BIN_GE_GYRO_CAN_RX_ID == id)
//  {
//     BingeGyroByCan(gimbal_t.pGyro_t,data);
//  }   
//}
 //-----------------------临时
/**
	* @Data    2019-02-15 15:10
	* @brief   云台数据发送
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
  * @brief   获取云台状态
  * @param   void
  * @retval  void
  */
  uint32_t GetGimbalStatus(void)
  {
    return gimbal_t.status;
  }
  /**
  * @Data    2019-03-13 03:48
  * @brief   获取云台结构体地址 可读写，不能乱调用
  * @param   void
  * @retval  void
  */
  gimbalStruct *RWGetgimbalStructAddr(void)
  {
    return &gimbal_t;
  }
/**
	* @Data    2019-03-16 18:04
	* @brief   拨弹电机任务
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
	 * @brief   云台初始化状态设置
	 * @param   void
	 * @retval  void
	 */
	 void SetGimBalInitStatus(void)
	 {
		/* -------------- 拨弹参数设置 ----------------- */
		   gimbal_t.prammer_t->target =  gimbal_t.prammer_t->real_angle; //目标值
			/* ------ 外环pid参数 ------- */
				gimbal_t.prammer_t->ppostionPid_t->kp = 4;
				gimbal_t.prammer_t->ppostionPid_t->kd = 0;
				gimbal_t.prammer_t->ppostionPid_t->ki = 0;
				gimbal_t.prammer_t->ppostionPid_t->error = 0;
				gimbal_t.prammer_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.prammer_t->ppostionPid_t->integral_er = 0;//误差积分
				gimbal_t.prammer_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.prammer_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.prammer_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.prammer_t->ppostionPid_t->pid_out = 0;//pid输出
			/* ------ 内环pid参数 ------- */
				gimbal_t.prammer_t->pspeedPid_t->kp = 0.7;
				gimbal_t.prammer_t->pspeedPid_t->kd = 0.07;
				gimbal_t.prammer_t->pspeedPid_t->ki = 0.01;
				gimbal_t.prammer_t->pspeedPid_t->error = 0;
				gimbal_t.prammer_t->pspeedPid_t->last_error = 0;//上次误差
				gimbal_t.prammer_t->pspeedPid_t->before_last_error = 0;//上上次误差
				gimbal_t.prammer_t->pspeedPid_t->integral_er = 0;//误差积分
				gimbal_t.prammer_t->pspeedPid_t->pout = 0;//p输出
				gimbal_t.prammer_t->pspeedPid_t->iout = 0;//i输出
				gimbal_t.prammer_t->pspeedPid_t->dout = 0;//k输出
				gimbal_t.prammer_t->pspeedPid_t->pid_out = 0;//pid输出
        gimbal_t.prammer_t->pspeedPid_t->limiting=RAMMER_LIMIMT_CUT;
		/* -------------- yaw轴电机参数设置 ----------------- */
		gimbal_t.pYaw_t->target = gimbal_t.pYaw_t->real_angle;//设置启动目标值
						/* ------ 外环pid参数 ------- */
				gimbal_t.pYaw_t->ppostionPid_t->kp = 3.5;
				gimbal_t.pYaw_t->ppostionPid_t->kd = 16;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//误差积分
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid输出
			/* ------ 内环pid参数 ------- */
				gimbal_t.pYaw_t->pspeedPid_t->kp = -7;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = -0.035;
				gimbal_t.pYaw_t->pspeedPid_t->error = 0;
				gimbal_t.pYaw_t->pspeedPid_t->last_error = 0;//上次误差
				gimbal_t.pYaw_t->pspeedPid_t->before_last_error = 0;//上上次误差
				gimbal_t.pYaw_t->pspeedPid_t->integral_er = 0;//误差积分
				gimbal_t.pYaw_t->pspeedPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->pspeedPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->pspeedPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->pspeedPid_t->pid_out = 0;//pid输出
        gimbal_t.pYaw_t->pspeedPid_t->limiting=YAW_LIMIMT_CUT;
        gimbal_t.pYaw_t->pspeedPid_t->motor_lim = RM6623_LIM;
		/* -------------- pitch轴 ----------------- */
				gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;//设置启动目标值
						/* ------ 外环pid参数 ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 0;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 0;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//误差积分
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid输出
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//积分限幅
        gimbal_t.pPitch_t->ppostionPid_t->integral_threshold =300;
			/* ------ 内环pid参数 ------- */
                  /* ------ 内环pid参数 ------- */
        gimbal_t.pPitch_t->pspeedPid_t->kd =0;
        gimbal_t.pPitch_t->pspeedPid_t->ki =0.35;//45;
        gimbal_t.pPitch_t->pspeedPid_t->kp =236;//290;
        gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
				gimbal_t.pPitch_t->pspeedPid_t->error = 0;
				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//上次误差
				gimbal_t.pPitch_t->pspeedPid_t->before_last_error = 0;//上上次误差
				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//误差积分
				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p输出
				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i输出
				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k输出
				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid输出
        gimbal_t.pPitch_t->pspeedPid_t->motor_lim =PITCH_LIMIMT_CUT;
        gimbal_t.pPitch_t->pspeedPid_t->limiting=PITCH_LIMIMT_CUT;

//ceshi----------------
//				gimbal_t.pPitch_t->pspeedPid_t->kp = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->kd = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->ki = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->error = 0;
//				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//上次误差
//				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//误差积分
//				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p输出
//				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i输出
//				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k输出
//				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid输出
//        gimbal_t.pPitch_t->pspeedPid_t->integral_limint = LINT_LIMINT;//积分限幅
//        gimbal_t.pPitch_t->pspeedPid_t->integral_threshold =300;
//ceshi--------------
     	/* ------ 设置启动标志位 ------- */  
        SET_BIT(gimbal_t.status,START_OK);  

	 }
  /**
  * @Data    2019-03-19 23:28
  * @brief   拨弹PID控制
  * @param   void
  * @retval  void
  */
  int16_t RammerPidControl(int16_t rammer)
  {
    int16_t temp_pid_out;
    /* -------- 外环 --------- */
    gimbal_t.prammer_t->ppostionPid_t->error = GIMBAL_CAL_ERROR(rammer,gimbal_t.prammer_t->real_angle);
    gimbal_t.prammer_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.prammer_t->ppostionPid_t,gimbal_t.prammer_t->ppostionPid_t->error);
    gimbal_t.prammer_t->ppostionPid_t->pid_out = MAX(gimbal_t.prammer_t->ppostionPid_t->pid_out,xianfuweizhi); //限做大值
    gimbal_t.prammer_t->ppostionPid_t->pid_out = MIN(gimbal_t.prammer_t->ppostionPid_t->pid_out,-xianfuweizhi); //限做小值
    /* -------- 内环 --------- */
    gimbal_t.prammer_t->pspeedPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.prammer_t->ppostionPid_t->pid_out,gimbal_t.prammer_t->real_speed);
		gimbal_t.prammer_t->pspeedPid_t->pid_out = SpeedPid(gimbal_t.prammer_t->pspeedPid_t,gimbal_t.prammer_t->pspeedPid_t->error);
  	temp_pid_out = MAX(gimbal_t.prammer_t->pspeedPid_t->pid_out,xianfusudu); //限做大值
    temp_pid_out = MIN(gimbal_t.prammer_t->pspeedPid_t->pid_out,-xianfusudu); //限做小值
    return temp_pid_out;
  }
  /**
  * @Data    2019-03-21 01:39
  * @brief   yaw 轴pid控制
  * @param   void
  * @retval  void
  */
   int16_t __temp_pid_out;
   int16_t __temp_pid_out1;
  int16_t YawPidControl(int16_t yaw)
  {
     /* -------- 外环 --------- */
    gimbal_t.pYaw_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pYaw_t->ppostionPid_t,yaw);
    __temp_pid_out1 = gimbal_t.pYaw_t->ppostionPid_t->pid_out;
    __temp_pid_out = MAX(gimbal_t.pYaw_t->ppostionPid_t->pid_out,yawxianfu);
    __temp_pid_out = MIN( gimbal_t.pYaw_t->ppostionPid_t->pid_out,-yawxianfu);
      /* -------- 内环 --------- */
     gimbal_t.pYaw_t->pspeedPid_t->error= PostionOutToSpeedOut(gimbal_t.pYaw_t->ppostionPid_t->pid_out) - gimbal_t.pGyro_t->Gyrz;
    __temp_pid_out = SpeedPid(gimbal_t.pYaw_t->pspeedPid_t, gimbal_t.pYaw_t->pspeedPid_t->error);
    return __temp_pid_out;
  }
  /**
  * @Data    2019-03-21 01:40
  * @brief   pitch 轴pid控制
  * @param   void
  * @retval  void
  */
  int16_t ssssssssssssssssssssss;
  int16_t PitchPidControl(int16_t pitch)
  {
    int16_t temp_pid_out;
       /* -------- 外环 --------- */
    gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pPitch_t->ppostionPid_t,pitch);
    temp_pid_out = MAX(gimbal_t.pPitch_t->ppostionPid_t->pid_out,pitchxianfu);
    temp_pid_out = MIN( gimbal_t.pPitch_t->ppostionPid_t->pid_out,-pitchxianfu);
       /* -------- 内环 --------- */
		gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionOutToSpeedOut(gimbal_t.pPitch_t->ppostionPid_t->pid_out);
    gimbal_t.pPitch_t->pspeedPid_t->error = gimbal_t.pPitch_t->ppostionPid_t->pid_out - gimbal_t.pPitch_t->real_speed;//GIMBAL_CAL_ERROR(gimbal_t.pPitch_t->ppostionPid_t->pid_out,gimbal_t.pPitch_t->real_speed);
		temp_pid_out = SpeedPid(gimbal_t.pPitch_t->pspeedPid_t,gimbal_t.pPitch_t->pspeedPid_t->error);
  //	temp_pid_out = MAX(gimbal_t.pPitch_t->pspeedPid_t->pid_out,sudupitchxianfu); //限做大值
   // temp_pid_out = MIN(gimbal_t.pPitch_t->pspeedPid_t->pid_out,-sudupitchxianfu); //限做小值
    return temp_pid_out;
  }
/**
	* @Data    2019-04-16 22:29
	* @brief   位置pid输出映射成速度
	* @param   void
	* @retval  void
	*/
	int16_t PostionOutToSpeedOut(int16_t pid_out)
	{
		return (int16_t)(pid_out *0.1);
	}
/**
	* @Data    2019-03-20 21:27
	* @brief   yaw轴电机初始化
	* @param   void
	* @retval  void
	*/
	RM6623Struct* YawInit(void)
	{
	yaw_t.id = YAW_RX_ID ;//电机can的 ip
	yaw_t.target = 0 ;		 //目标值
	yaw_t.tem_target = 0 ;//临时目标值
	yaw_t.real_current = 0 ; //真实电流
	yaw_t.real_angle = 0 ;//真实角度
	yaw_t.tem_angle = 0 ;//临时角度
	yaw_t.zero = 0 ;			 //电机零点
	yaw_t.Percentage = 0 ;//转换比例（减速前角度:减速后的角度 = x:1
	yaw_t.thresholds = 0 ; //电机反转阀值
  yaw_t.error = 0 ;//当前误差
  yaw_t.last_real = 0 ;
  yaw_t.coefficient = 0 ;
		/* ------ 外环pid地址 ------- */
  yaw_t.ppostionPid_t = &yawOuterLoopPid_t;
    yawOuterLoopPid_t.motor_lim = RM6623_LIM;//6623最大电流范围
			/* ------ 内环pid地址 ------- */
	yaw_t.pspeedPid_t = &yawInnerLoopPid_t ;
    yawInnerLoopPid_t.motor_lim = RM6623_LIM;//6623最大电流范围
			return &yaw_t;
	}
	/**
	* @Data    2019-03-20 21:27
	* @brief   pitch轴电机初始化
	* @param   void
	* @retval  void
	*/
	GM6020Struct* PitchInit(void)
	{
	pitch_t.id = YAW_RX_ID ;//电机can的 ip
	pitch_t.target = 0 ;		 //目标值
	pitch_t.tem_target = 0 ;//临时目标值
	pitch_t.real_current = 0 ; //真实电流
	pitch_t.real_angle = 0 ;//真实角度
	pitch_t.tem_angle = 0 ;//临时角度
	pitch_t.zero = 0 ;			 //电机零点
	pitch_t.Percentage = 0 ;//转换比例（减速前角度:减速后的角度 = x:1
	pitch_t.thresholds = 0 ; //电机反转阀值
  pitch_t.error = 0 ;//当前误差
  pitch_t.last_real = 0 ;
  pitch_t.coefficient = 0 ;
		/* ------ 外环pid地址 ------- */
  pitch_t.ppostionPid_t = &pitchOuterLoopPid_t ;
    pitchOuterLoopPid_t.motor_lim = RM6020_LIM;//6020最大电流范围
			/* ------ 内环pid地址 ------- */
	pitch_t.pspeedPid_t = &pitchInnerLoopPid_t ;
    pitchInnerLoopPid_t.motor_lim = RM6020_LIM;//6020最大电流范围
    
//            /* ------ 内环pid参数 ------- */
//        gimbal_t.pPitch_t->pspeedPid_t->kd =0;
//        gimbal_t.pPitch_t->pspeedPid_t->ki =0.35;//45;
//        gimbal_t.pPitch_t->pspeedPid_t->kp =236;//290;
//        gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
			return &pitch_t;
	}


///**
//* @Data    2019-03-21 00:46
//* @brief   自瞄数据转换//待测试
//* @param   void
//* @retval  void
//*/
/**
* @Data    2019-03-21 00:46
* @brief   自瞄pid参数
* @param   void
* @retval  void
*/
/**
* @Data    2019-03-21 00:46
* @brief   云台预判//待测试
* @param   void
* @retval  void
*/
/**
* @Data    2019-03-21 00:46
* @brief   自瞄模式pid参数
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
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//误差积分
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid输出
       gimbal_t.pYaw_t->ppostionPid_t->integral_threshold =15;
  
  
  				gimbal_t.pYaw_t->pspeedPid_t->kp = -7;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = -0.035;
  						/* ------ 外环pid参数 ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = kp_test;
				gimbal_t.pPitch_t->ppostionPid_t->kd = kd_test;
				gimbal_t.pPitch_t->ppostionPid_t->ki = ki_test;
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//误差积分
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid输出
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//积分限幅
        gimbal_t.pPitch_t->ppostionPid_t->integral_threshold =300;
        
         gimbal_t.pPitch_t->pspeedPid_t->kd =0;
        gimbal_t.pPitch_t->pspeedPid_t->ki =0.35;//45;
        gimbal_t.pPitch_t->pspeedPid_t->kp =236;//290
         //gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
}
/**
* @Data    2019-03-21 00:46
* @brief   下位pid参数
* @param   void
* @retval  void
*/
void SetGeneralMode(void)
{
    		gimbal_t.pYaw_t->ppostionPid_t->kp = 1.85;
				gimbal_t.pYaw_t->ppostionPid_t->kd = 2;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//误差积分
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid输出
  
    		gimbal_t.pYaw_t->pspeedPid_t->kp = -7;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = -0.035;
  
  						/* ------ 外环pid参数 ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 3;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 3;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//误差积分
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid输出
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//积分限幅
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
//				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//上次误差
//				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//误差积分
//				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p输出
//				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i输出
//				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k输出
//				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid输出
//        gimbal_t.pPitch_t->pspeedPid_t->integral_limint = LINT_LIMINT;//积分限幅
//        gimbal_t.pPitch_t->pspeedPid_t->integral_threshold = 500;
//        gimbal_t.pPitch_t->pspeedPid_t->kp_separatecmd =1;
        //ceshi-------------
        
 }
/**
	* @Data    2019-03-27 15:16
	* @brief   掉出视野趋势计算
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
  * @brief   云台软件定时器钩子函数
  * @param   void
  * @retval  void
  */
 void GimbalGetStatusCallback(void const * argument)
  {
    yawStatusQu.enQueue(&yawStatusQu,gimbal_t.pYaw_t->real_angle);
    pitchStatusQu.enQueue(&pitchStatusQu,gimbal_t.pPitch_t->real_angle);
  }


  
	
/*-----------------------------------file of end------------------------------*/
