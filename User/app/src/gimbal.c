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
/* -------------- 任务句柄 ----------------- */
		osThreadId startRammerTaskHandle; 
/* ----------------- 任务钩子函数 -------------------- */
	void StartRammerTask(void const *argument);
	/**
	* @Data    2019-01-27 17:09
	* @brief   云台结构体初始化
	* @param  CAN_HandleTypeDef* hcanx（x=1,2）
	* @retval  void
	*/
	void GimbalStructInit(const dbusStruct* pRc_t)
	{
		gimbal_t.pRc_t = pRc_t;
    gimbal_t.prammer_t =RammerInit();
  /* ------ 开启摩擦轮 ------- */
		BrushlessMotorInit();
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
  #ifdef ANTI_CLOCK_WISE  //逆时针为正方向
        AntiRM2006ParseData(gimbal_t.prammer_t,data);
  #endif
				break;
			case YAW_RX_ID:
				// RM6623ParseData(gimbal_t.pYaw_t,data);
        // /* -------- 比例转换 --------- */
        // gimbal_t.pYaw_t->real_angle = RatiometricConversion  \
        // (gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds,gimbal_t.pYaw_t->Percentage);
        /* -------- 过零处理 --------- */
        //  zeroArgument(gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds); 
				break;
			case PITCH_RX_ID:
				// RM6623ParseData(gimbal_t.pPitch_t,data);
        /* -------- 过零处理 --------- */
        //  zeroArgument(gimbal_t.pPitch_t->real_angle,gimbal_t.pPitch_t->thresholds); 
				break;
		
			default:
				break;
		}

	}
/**
	* @Data    2019-02-14 21:01
	* @brief   云台控制
	* @param   void
	* @retval  void
	*/
	void GimbalControl(void)
	{

//		int16_t pid_out = -500;
//		rammer_t.target = 3*(DbusAntiShake(20,dbus->ch1)); //目标值
//		rammer_t.error = rammer_t.target - rammer_t.real_speed;
//		pid_out = SpeedPid(&rammerInnerLoopPid_t,rammer_t.error);
//		pid_out = MAX(pid_out,2000); //限做大值
//	  pid_out = MIN(pid_out,-2000); //限做小值
//		GimbalCanTx(pid_out,0);
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
// /*---------------------------------80字符限制-----------------------------------*/
//   /**
//   * @Data    2019-02-24 23:55
//   * @brief   小电脑数据接收
//   * @param   void
//   * @retval  void
//   */
//   HAL_StatusTypeDef RxPCMsg(void)
//   {
//     return (HAL_UART_Receive(gimbal_t.huartx,pc_data,3,1));
//   }
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

			osDelay(10);
		}
	}
 
/*-----------------------------------file of end------------------------------*/
