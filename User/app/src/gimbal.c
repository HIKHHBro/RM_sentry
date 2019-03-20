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
/* -------------- 私有宏 ----------------- */
	#define QUEUE_LEN      5U//深度为5
	#define  QUEUE_SIZE    8U//长度为5;
/* -------------- 结构体声明 ----------------- */
	gimbalStruct gimbal_t;//云台结构体
  RM6623Struct yaw_t;//yaw轴电机
	postionPidStruct yawOuterLoopPid_t;//yaw 电机外环pid
	speedPidStruct yawInnerLoopPid_t;//yaw 电机内环pid
/* -------------- 任务句柄 ----------------- */
		osThreadId startRammerTaskHandle; 
/* -------------- 外部链接 ----------------- */
		extern osThreadId startGimbalTaskHandle;
/* -------------- 发送队列 ----------------- */
  xQueueHandle gimbal_queue;
/* ----------------- 任务钩子函数 -------------------- */
	void StartRammerTask(void const *argument);
/* -------------- 私有函数 ----------------- */
	static void GimbalTx(int16_t yaw,int16_t pitch,int16_t rammer);
/* ----------------- 临时变量 -------------------- */
    int16_t taddd =0;
    int16_t xianfuweizhi =3000;
    int16_t xianfusudu = 5000;//STUCK_BULLET_THRE
    int16_t yawxianfu = 5000;
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
  /* ------ 开启摩擦轮 ------- */
		BrushlessMotorInit();
  /* ------ 创建云台发送队列 ------- */
	  gimbal_queue	= xQueueCreate(QUEUE_LEN,QUEUE_SIZE);//一定要在用之前创建队列
 	/* ------ 设置初始化标志位 ------- */
		SET_BIT(gimbal_t.status,INIT_OK);
	/* ------ 挂起任务，等待初始化 ------- */
		vTaskSuspend(startGimbalTaskHandle);
	/* ------ 设置机器人初始化状态 ------- */
		SetGimBalInitStatus();
 /* ------ 创建拨弹任务 ------- */
	osThreadDef(startRammerTask,StartRammerTask,osPriorityNormal,0,RAMMER_HEAP_SIZE);
	startRammerTaskHandle = osThreadCreate(osThread(startRammerTask),NULL);
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
				gimbal_t.pYaw_t->real_angle = RatiometricConversion(gimbal_t.pYaw_t->tem_angle,7000,5,&(gimbal_t.pYaw_t->last_real),&(gimbal_t.pYaw_t->coefficient),gimbal_t.status);
				break;
			case PITCH_RX_ID:
				// RM6623ParseData(gimbal_t.pPitch_t,data);
        /* -------- 过零处理 --------- */
        //  zeroArgument(gimbal_t.pPitch_t->real_angle,gimbal_t.pPitch_t->thresholds); 
				break;
		
			default:
				break;
		}
    SET_BIT(gimbal_t.status,RX_OK);
	}
/**
	* @Data    2019-02-14 21:01
	* @brief   云台控制
	* @param   void
	* @retval  void
	*/
	void GimbalAutoControl(void)
	{
		int16_t yaw,pitch,rammer;
    rammer = RammerPidControl(gimbal_t.prammer_t->target);
    yaw = YawPidControl(gimbal_t.pPc_t->yaw_target_angle);
    
		GimbalTx(yaw,0,rammer);
	}
/**
	* @Data    2019-02-15 15:10
	* @brief   云台数据发送
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
				gimbal_t.pYaw_t->ppostionPid_t->kp = 0;
				gimbal_t.pYaw_t->ppostionPid_t->kd = 0;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//误差积分
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid输出
			/* ------ 内环pid参数 ------- */
				gimbal_t.pYaw_t->pspeedPid_t->kp = 0;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = 0;
				gimbal_t.pYaw_t->pspeedPid_t->error = 0;
				gimbal_t.pYaw_t->pspeedPid_t->last_error = 0;//上次误差
				gimbal_t.pYaw_t->pspeedPid_t->before_last_error = 0;//上上次误差
				gimbal_t.pYaw_t->pspeedPid_t->integral_er = 0;//误差积分
				gimbal_t.pYaw_t->pspeedPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->pspeedPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->pspeedPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->pspeedPid_t->pid_out = 0;//pid输出
        gimbal_t.pYaw_t->pspeedPid_t->limiting=YAW_LIMIMT_CUT;
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
  * @brief   yaw轴pid控制
  * @param   void
  * @retval  void
  */
  int16_t YawPidControl(int16_t yaw)
  {
    int16_t temp_pid_out;
    gimbal_t.pYaw_t->ppostionPid_t->error = GIMBAL_CAL_ERROR(yaw,gimbal_t.pYaw_t->real_angle);
    gimbal_t.pYaw_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pYaw_t->ppostionPid_t,gimbal_t.pYaw_t->ppostionPid_t->error);
    gimbal_t.pYaw_t->ppostionPid_t->pid_out = MAX(gimbal_t.pYaw_t->ppostionPid_t->pid_out,yawxianfu);
    gimbal_t.pYaw_t->ppostionPid_t->pid_out = MIN( gimbal_t.pYaw_t->ppostionPid_t->pid_out,-yawxianfu);
    return temp_pid_out;
  }
  /**
  * @Data    2019-03-21 01:40
  * @brief   pitch轴pid控制
  * @param   void
  * @retval  void
  */
  int16_t PitchPidControl(int16_t yaw)
  {
    int16_t temp_pid_out;
    return temp_pid_out;
  }
/**
	* @Data    2019-03-20 20:27
	* @brief   云台队列发送函数
	* @param   void
	* @retval  void
	*/
	void GimbalTx(int16_t yaw,int16_t pitch,int16_t rammer)
	{
		int16_t data[5];
		data[0] = yaw;
		data[1] = pitch;
		data[2] = rammer;
		xQueueSendToBack(gimbal_queue,data,0);
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
  yaw_t.ppostionPid_t = &yawOuterLoopPid_t ;
			/* ------ 内环pid地址 ------- */
	yaw_t.pspeedPid_t = &yawInnerLoopPid_t ;

			return &yaw_t;
	}
/*-----------------------------------file of end------------------------------*/
