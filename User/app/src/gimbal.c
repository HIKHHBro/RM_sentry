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
/* -------------- 模块自定义标志位宏 ----------------- */
#define  RC_MODE_READ               (0x80000000U) //遥控模式准备就绪
#define  GIMBAL_PC_SHOOT_MODE_READ  (0x40000000U) //自瞄打击模式准备就绪
#define  FRAME_DROP_BUFFER_READ     (0x20000000U) //掉帧缓冲准备就绪
#define  SCAN_MODE_READ             (0x10000000U) //扫描模式准备就绪
#define  DISABLE_MOD_READ           (0xF0000000U)//使能运行模块
#define  JUDGE_READ                 (0xF0000000U) //判断决策

#define  RC_MODE_RUNING             (0x08000000U)//遥控模式执行
#define  PC_SHOOT_MODE_RUNING       (0x04000000U)//自瞄打击模式执行
#define  FRAME_DROP_BUFFER_RUNING   (0x02000000U)//掉帧缓冲执行
#define  SCAN_MODE_RUNING           (0x01000000U)//扫描模式执行
#define  GIMBAL_MOD_RUNNING         (0x0F000000U)//运行模块位

#define  DISABLE_GIMBAL             (0xFF000000U)//失能云台
#define  DELEC_USER_MODE            (0x00000000U)//清除用户自定义标志位
#define SET_RUNING_STATUS(_status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,GIMBAL_MOD_RUNNING);	\
					SET_BIT(gimbal_t.status,_status_);							\
				}while(0)	
#define SET_READ_STATUS(__status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,DISABLE_MOD_READ);	\
					SET_BIT(gimbal_t.status,__status_);							\
				}while(0)																					
/* -------------- 私有宏 ----------------- */
	#define QUEUE_LEN      5U//深度为5
	#define  QUEUE_SIZE    8U//长度为5;
  #define  MEMORY_SPAN   (50)          //   1000ms = 50*20
  #define  QUEUE_YAW_STATUS_SIZE      20//则记忆长度为1s 精度为50
 #define   QUEUE_PITCH_STATUS_SIZE    20//长度10，则记忆长度为1s
/* -------------- 结构体声明 ----------------- */
	gimbalStruct gimbal_t;//云台结构体
  RM6623Struct yaw_t;//yaw轴电机
	postionPidStruct yawOuterLoopPid_t;//yaw 电机外环pid
	speedPidStruct yawInnerLoopPid_t;//yaw 电机内环pid
	GM6020Struct pitch_t;//pitch轴电机
	postionPidStruct pitchOuterLoopPid_t;//pitch 电机外环pid
	speedPidStruct pitchInnerLoopPid_t;//pitch 电机内环pid
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
  /* ------ 开启摩擦轮 ------- */
		BrushlessMotorInit();
//    UserCanConfig(GIMBAL_CAN);
  /* ------ 创建云台发送队列 ------- */
	  gimbal_queue	= xQueueCreate(QUEUE_LEN,QUEUE_SIZE);//一定要在用之前创建队列
  /* ------ 创建云台历史状态队列 ------- */
    Int16QueueCreate(&yawStatusQu,QUEUE_YAW_STATUS_SIZE);
    Int16QueueCreate(&pitchStatusQu,QUEUE_PITCH_STATUS_SIZE);
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
  		int16_t yaw=0;
    int16_t pitch=0;
    int16_t rammer=0;
    uint32_t commot =0;
	void GimbalAutoControl(void)
	{
    SET_BIT(gimbal_t.status,RUNING_OK);
    commot = ControlDecision();//决策模式
    ControlSwitch(commot);//控制模式切换
    yaw = YawPidControl(gimbal_t.pYaw_t->ppostionPid_t->error);
    pitch = PitchPidControl(gimbal_t.pPitch_t->ppostionPid_t->error);
    rammer = RammerPidControl(gimbal_t.prammer_t->target);
    GimbalCanTx(yaw,pitch,rammer);
	}
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
				gimbal_t.pYaw_t->ppostionPid_t->kp = -0.3;
				gimbal_t.pYaw_t->ppostionPid_t->kd = -1;
				gimbal_t.pYaw_t->ppostionPid_t->ki = -0.02;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//误差积分
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
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
		/* -------------- pitch轴 ----------------- */
				gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;//设置启动目标值
						/* ------ 外环pid参数 ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 10;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 10;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0.1;
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
				gimbal_t.pPitch_t->pspeedPid_t->kp = 9;
				gimbal_t.pPitch_t->pspeedPid_t->kd = 30;
				gimbal_t.pPitch_t->pspeedPid_t->ki = 0.2;
				gimbal_t.pPitch_t->pspeedPid_t->error = 0;
				gimbal_t.pPitch_t->pspeedPid_t->last_error = 0;//上次误差
				gimbal_t.pPitch_t->pspeedPid_t->before_last_error = 0;//上上次误差
				gimbal_t.pPitch_t->pspeedPid_t->integral_er = 0;//误差积分
				gimbal_t.pPitch_t->pspeedPid_t->pout = 0;//p输出
				gimbal_t.pPitch_t->pspeedPid_t->iout = 0;//i输出
				gimbal_t.pPitch_t->pspeedPid_t->dout = 0;//k输出
				gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;//pid输出
        gimbal_t.pPitch_t->pspeedPid_t->limiting=PITCH_LIMIMT_CUT;
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
//  /**
//  * @Data    2019-03-21 01:39
//  * @brief   yaw轴pid控制
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
//  * @brief   pitch轴pid控制
//  * @param   void
//  * @retval  void
//  */
//  int16_t PitchPidControl(int16_t pitch)
//  {
//    int16_t temp_pid_out;
//       /* -------- 外环 --------- */
//    gimbal_t.pPitch_t->ppostionPid_t->error = CalculateError((pitch),( gimbal_t.pPitch_t->real_angle),5500,(8192));//GIMBAL_CAL_ERROR(pitch,gimbal_t.pPitch_t->real_angle);
//    gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pPitch_t->ppostionPid_t,gimbal_t.pPitch_t->ppostionPid_t->error);
//    temp_pid_out = MAX(gimbal_t.pPitch_t->ppostionPid_t->pid_out,pitchxianfu);
//    temp_pid_out = MIN( gimbal_t.pPitch_t->ppostionPid_t->pid_out,-pitchxianfu);
////       /* -------- 内环 --------- */
////    gimbal_t.pPitch_t->pspeedPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.pPitch_t->ppostionPid_t->pid_out,gimbal_t.pPitch_t->real_speed);
////		gimbal_t.pPitch_t->pspeedPid_t->pid_out = SpeedPid(gimbal_t.pPitch_t->pspeedPid_t,gimbal_t.pPitch_t->pspeedPid_t->error);
////  	temp_pid_out = MAX(gimbal_t.pPitch_t->pspeedPid_t->pid_out,sudupitchxianfu); //限做大值
////    temp_pid_out = MIN(gimbal_t.pPitch_t->pspeedPid_t->pid_out,-sudupitchxianfu); //限做小值
//    return temp_pid_out;
//  }

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
   
    gimbal_t.pYaw_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pYaw_t->ppostionPid_t,yaw);
    __temp_pid_out1 = gimbal_t.pYaw_t->ppostionPid_t->pid_out;
    __temp_pid_out = MAX(gimbal_t.pYaw_t->ppostionPid_t->pid_out,yawxianfu);
    __temp_pid_out = MIN( gimbal_t.pYaw_t->ppostionPid_t->pid_out,-yawxianfu);
    return __temp_pid_out;
  }
  /**
  * @Data    2019-03-21 01:40
  * @brief   pitch 轴pid控制
  * @param   void
  * @retval  void
  */
  int16_t PitchPidControl(int16_t pitch)
  {
    int16_t temp_pid_out;
       /* -------- 外环 --------- */
    gimbal_t.pPitch_t->ppostionPid_t->pid_out = PostionPid(gimbal_t.pPitch_t->ppostionPid_t,pitch);
    temp_pid_out = MAX(gimbal_t.pPitch_t->ppostionPid_t->pid_out,pitchxianfu);
    temp_pid_out = MIN( gimbal_t.pPitch_t->ppostionPid_t->pid_out,-pitchxianfu);
//       /* -------- 内环 --------- */
//    gimbal_t.pPitch_t->pspeedPid_t->error = GIMBAL_CAL_ERROR(gimbal_t.pPitch_t->ppostionPid_t->pid_out,gimbal_t.pPitch_t->real_speed);
//		gimbal_t.pPitch_t->pspeedPid_t->pid_out = SpeedPid(gimbal_t.pPitch_t->pspeedPid_t,gimbal_t.pPitch_t->pspeedPid_t->error);
//  	temp_pid_out = MAX(gimbal_t.pPitch_t->pspeedPid_t->pid_out,sudupitchxianfu); //限做大值
//    temp_pid_out = MIN(gimbal_t.pPitch_t->pspeedPid_t->pid_out,-sudupitchxianfu); //限做小值
    return temp_pid_out;
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
    pitchOuterLoopPid_t.motor_lim = RM6020_LIM;//6020最大电流范围
			return &pitch_t;
	}
/**
* @Data    2019-03-20 21:27
* @brief   云台扫描探索模式目标值
* @param   void
* @retval  void
*/
uint8_t scflag = 0;
  int16_t pitchspeed_= 25;
    int16_t yawspeed_= 20;
  int16_t scok_up = 1000;
  int16_t scok_down = 2200;
void ScanningToExplore(void)
{
  if((gimbal_t.status&SCAN_MODE_RUNING) != SCAN_MODE_RUNING)
  {
    //目标值切换，状态切换
    gimbal_t.pitch_scan_target = gimbal_t.pPitch_t->real_angle;
    gimbal_t.yaw_scan_target = gimbal_t.pYaw_t->real_angle;
    gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;

    if((gimbal_t.pitch_scan_target) > scok_down && gimbal_t.pPitch_t->target< DOWN_BUFF_POSI) 
    {
      gimbal_t.pitch_scan_target = scok_down;
    }
    else if ((gimbal_t.pitch_scan_target )< scok_up || gimbal_t.pPitch_t->target > UP_BUFF_POSI) 
    {
      gimbal_t.pitch_scan_target = scok_up;
    }
    //清除pc控制标志位，设置标志位
    SET_RUNING_STATUS(SCAN_MODE_RUNING);
    SetGeneralMode();
     Shoot(0,0);
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
     gimbal_t.pPitch_t->ppostionPid_t->error = CalculateError(gimbal_t.pitch_scan_target,( gimbal_t.pPitch_t->real_angle),6500,(8192));//待测试
     gimbal_t.pYaw_t->ppostionPid_t->error = CalculateError(( gimbal_t.yaw_scan_target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//待测试
    
}
/**
* @Data    2019-03-20 21:27
* @brief   云台自瞄pc控制模式目标值
* @param   void
* @retval  void
*/
float pitchcin = 1;
float  yawcin = 1;
void PcControlMode(void)
{
  if((gimbal_t.status&PC_SHOOT_MODE_RUNING) != PC_SHOOT_MODE_RUNING)
  {
    //目标值切换，状态切换
    SET_RUNING_STATUS(PC_SHOOT_MODE_RUNING);
     gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;
    SetPcControlPID();
    HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
      __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_SPEED);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_SPEED);
//    if(gimbal_t.pPitch_t->ppostionPid_t->error - gimbal_t.pPc_t->pitch_target_angle)
  }
   Shoot(CAL_SHOOT_FRE(gimbal_t.pPc_t->distance),0);
   gimbal_t.pPitch_t->ppostionPid_t->error  = (int16_t)(gimbal_t.pPc_t->pitch_target_angle * pitchcin);
  gimbal_t.pYaw_t->ppostionPid_t->error = (int16_t)(gimbal_t.pPc_t->yaw_target_angle * pitchcin);
}
/**
* @Data    2019-03-20 21:27
* @brief   控制权切换
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
* @brief   决策控制判断
* @param   void
* @retval  void
*/
int16_t conflag =0;
  uint32_t ControlDecision(void)
  {
    switch (gimbal_t.pRc_t->switch_left)
    {
      case 1:
        if(gimbal_t.pRc_t->switch_right ==1)
        {
         switch (gimbal_t.pPc_t->commot)
         {
           case 0:
           if(conflag > 100)
           {
             SET_READ_STATUS(SCAN_MODE_READ);
           }
           else 
           {
                conflag ++;
              SET_READ_STATUS(FRAME_DROP_BUFFER_READ);
           }
             break;
           case 1:
            SET_READ_STATUS(GIMBAL_PC_SHOOT_MODE_READ);
           conflag =0;
             break;
           default:
             SET_READ_STATUS(SCAN_MODE_READ);
              break;
         }
        }
        else if(gimbal_t.pRc_t->switch_right ==2)
        {
           SET_READ_STATUS(RC_MODE_READ);
        } 
        break;
      case 2:
        CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);
        break;
      default:
        CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);
        break;      
    } 
//    if(gimbal_t.pRc_t->switch_left ==2)
//    {
//      CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);
//    }
//    else if(gimbal_t.pRc_t->switch_left ==2)
//    {
//      
//    }
//    else if(gimbal_t.pRc_t->switch_right ==1)
//    {
//       switch (gimbal_t.pPc_t->commot)
//       {
//         case 0:
//         if(conflag > 100)
//         {
//           SET_READ_STATUS(SCAN_MODE_READ);
//         }
//         else 
//         {
//              conflag ++;
//           	SET_READ_STATUS(FRAME_DROP_BUFFER_READ);
//         }
//           break;
//         case 1:
//				  SET_READ_STATUS(GIMBAL_PC_SHOOT_MODE_READ);
//         conflag =0;
//           break;
//         default:
//            break;
//       }
//    }
//    else if(gimbal_t.pRc_t->switch_right ==2)
//    {
//			 SET_READ_STATUS(RC_MODE_READ);
//    } 
//else  CLEAR_BIT(gimbal_t.status,GIMBAL_MOD_RUNNING);   
    return (gimbal_t.status & JUDGE_READ);//与出就绪位，用以判断不影响转态值
  }
 /**
* @Data    2019-03-20 21:27
* @brief   遥控模式
* @param   void
* @retval  void
*/
    int16_t i = 0;
  float iii = 10;
   float yyyy = 0.1;
    int16_t rcscok_up = 500;
  int16_t rcscok_down = 2400;
  int16_t rctemp;
  int16_t seepdd =7;
void GimbalRcControlMode(void)
{
  if((gimbal_t.status&RC_MODE_RUNING) != RC_MODE_RUNING)
  {
     SET_RUNING_STATUS(RC_MODE_RUNING);
        //目标值切换，状态切换
    gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;
    gimbal_t.pYaw_t->target = gimbal_t.pYaw_t->real_angle;
    gimbal_t.prammer_t->target = gimbal_t.prammer_t->real_angle ;
    if(gimbal_t.pPitch_t->target > (rcscok_down-1) && gimbal_t.pPitch_t->target< DOWN_BUFF_POSI)
    {
      gimbal_t.pPitch_t->target =(rcscok_down-1);
    }
    else if(gimbal_t.pPitch_t->target < (rcscok_up+1) || gimbal_t.pPitch_t->target > UP_BUFF_POSI)
    {
      gimbal_t.pPitch_t->target =(rcscok_up+1);
    }
    gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;
    SetGeneralMode();
  }
  if(gimbal_t.pRc_t->ch2 >600)
  {
     Shoot(10,0);
  }
  else  Shoot(0,0);
                   HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
  rctemp = -(int16_t)(gimbal_t.pRc_t->ch3 * iii);
  
        gimbal_t.pPitch_t->target += (int16_t)(gimbal_t.pRc_t->ch4 * yyyy);
        gimbal_t.pPitch_t->target = MAX(gimbal_t.pPitch_t->target,rcscok_down);
        gimbal_t.pPitch_t->target = MIN(gimbal_t.pPitch_t->target,rcscok_up);
        
     gimbal_t.pPitch_t->ppostionPid_t->error =CalculateError(gimbal_t.pPitch_t->target,( gimbal_t.pPitch_t->real_angle),4600,(8192));//待测试
     gimbal_t.pYaw_t->ppostionPid_t->error =rctemp;// CalculateError((gimbal_t.pYaw_t->target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//待测试
}
/**
* @Data    2019-03-21 00:46
* @brief   云台预判//待测试
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
float kp_test=20;
float ki_test=0.3;
float kd_test=180;
void SetPcControlPID(void)
{
  			gimbal_t.pYaw_t->ppostionPid_t->kp = -1.8;
				gimbal_t.pYaw_t->ppostionPid_t->kd = -11;
				gimbal_t.pYaw_t->ppostionPid_t->ki = 0;
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//误差积分
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid输出
  
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
}
/**
* @Data    2019-03-21 00:46
* @brief   下位pid参数
* @param   void
* @retval  void
*/
void SetGeneralMode(void)
{
    		gimbal_t.pYaw_t->ppostionPid_t->kp = -0.3;
				gimbal_t.pYaw_t->ppostionPid_t->kd = -1;
				gimbal_t.pYaw_t->ppostionPid_t->ki = -0.02;
				gimbal_t.pYaw_t->ppostionPid_t->error = 0;
				gimbal_t.pYaw_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pYaw_t->ppostionPid_t->integral_er = 0;//误差积分
        gimbal_t.pYaw_t->ppostionPid_t->integral_limint = 3000;
				gimbal_t.pYaw_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pYaw_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pYaw_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pYaw_t->ppostionPid_t->pid_out = 0;//pid输出
  
  						/* ------ 外环pid参数 ------- */
				gimbal_t.pPitch_t->ppostionPid_t->kp = 18;
				gimbal_t.pPitch_t->ppostionPid_t->kd = 300;
				gimbal_t.pPitch_t->ppostionPid_t->ki = 0.22;
				gimbal_t.pPitch_t->ppostionPid_t->error = 0;
				gimbal_t.pPitch_t->ppostionPid_t->last_error = 0;//上次误差
				gimbal_t.pPitch_t->ppostionPid_t->integral_er = 0;//误差积分
				gimbal_t.pPitch_t->ppostionPid_t->pout = 0;//p输出
				gimbal_t.pPitch_t->ppostionPid_t->iout = 0;//i输出
				gimbal_t.pPitch_t->ppostionPid_t->dout = 0;//k输出
				gimbal_t.pPitch_t->ppostionPid_t->pid_out = 0;//pid输出
        gimbal_t.pPitch_t->ppostionPid_t->integral_limint = LINT_LIMINT;//积分限幅
        gimbal_t.pPitch_t->ppostionPid_t->integral_threshold = 500;
}
/**
* @Data    2019-03-21 00:46
* @brief   云台模块失能//待完善
* @param   void
* @retval  void
*/
void GimbalDeinit(void)
{
   Shoot(0,0);
    HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
}
	/**
		* @Data    2019-03-26 21:32
		* @brief   掉帧缓冲状态设置
		* @param   void
		* @retval  void
		*/
		void SetFrameDropBufferStatus(void)
		{
			SET_RUNING_STATUS(FRAME_DROP_BUFFER_RUNING);
     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_SPEED);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_SPEED);
     HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
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

/**
	* @Data    2019-03-26 21:17
	* @brief   掉帧缓冲模式
	* @param   void
	* @retval  void
	*/
int16_t y_lock;
int16_t p_lock;
uint8_t buffer_flag =0;
  int16_t go_back = 10;
  int16_t go_back_lock = 800;
	void FrameDropBufferMode(void)
	{
		if((gimbal_t.status&FRAME_DROP_BUFFER_RUNING) != FRAME_DROP_BUFFER_RUNING)
		{
			
			SetFrameDropBufferStatus();
			if(gimbal_t.pYaw_t->ppostionPid_t->error > 100)///右边
			{
         y_lock = 1500;
			 gimbal_t.pYaw_t->target	= SetLock(gimbal_t.pYaw_t->real_angle,y_lock);
			}
		 if(gimbal_t.pYaw_t->ppostionPid_t->error < -100)//左边
			{
				 y_lock = -1500;
		  	gimbal_t.pYaw_t->target	= SetLock(gimbal_t.pYaw_t->real_angle,y_lock);
			}
			SetGeneralMode();
       gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle +400;
//      if(gimbal_t.pPitch_t->ppostionPid_t->error >450)
//      {
//        gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle +400;
//      }
//      if(gimbal_t.pPitch_t->ppostionPid_t->error <-300)
//      {
//      gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;
//      }
//      if(gimbal_t.pPitch_t->target >go_back_lock)
//      {
//        go_back = (0 -go_back);
//      }
        
		}
    Shoot(3,0);
//   gimbal_t.pPitch_t->target  +=  go_back;
		  gimbal_t.pPitch_t->ppostionPid_t->error = CalculateError(gimbal_t.pPitch_t->target,( gimbal_t.pPitch_t->real_angle),5500,(8192));//待测试
      gimbal_t.pYaw_t->ppostionPid_t->error = CalculateError((gimbal_t.pYaw_t->target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//待测试
	if(ABS(gimbal_t.pYaw_t->ppostionPid_t->error) < 100)
    {
	     gimbal_t.pYaw_t->target	= SetLock(gimbal_t.pYaw_t->real_angle,-y_lock);
        buffer_flag =1;
    }
    else if((ABS(gimbal_t.pYaw_t->ppostionPid_t->error) < 100) && (buffer_flag ==1))
    {
			CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);
    }

	}

int16_t SetLock(int16_t r,int16_t t)
{
  if(r + t > 20480)
   r = 20480 -(r + t );
   else if (r + t <0)
   r =  t+20480+r;
  else r = t+r;
	return r;
}
//发射
//  //热量
//  uint8_t hot_bool(void)
//  {
//    if(shoot_data_t.bullet_freq > 200)
//     return 2;
//   if(shoot_data_t.bullet_freq<=200)
//     return 1;
//  }
// int16_t aa=0;
// void Shoot(uint8_t speed,uint8_t e)
// {
//   if(speed ==0)
//   {
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_1_START_V);
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_2_START_V);
//     HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);

//   }
//   else if(speed >0)
//   {
//     if(speed >8&& e ==1)
//     {
//       if(aa > 100)
//       e =0;
//       aa++;
//     }
//     else if(speed >8&& e ==0)
//     {
//       speed =5;
//     }
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,1700);
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,1700);
//     HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
//     SetRammerPID(speed);
//   }
// }

// void AiShoot(uint32_t status)
// {
//   // get(dis);
//   if(dis > 300)
//   {
//     Shoot()
//   }
// }

	
/*-----------------------------------file of end------------------------------*/
