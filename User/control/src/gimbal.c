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
extern UART_HandleTypeDef huart2;//串口1
/* -------------- 私有宏定义 ----------------- */
	#define RAMMER_RX_ID	  0x207
	#define YAW_RX_ID 			0x205
	#define PITCH_RX_ID 		0x206
	#define GIMBAL_CAN_TX_ID 0x1ff
/* -------------- 结构体声明 ----------------- */
	gimbalStruct gimbal_t;                     //云台结构体
	M2006Struct rammer_t;                       //拨弹电机结构体
	  postionPidStruct rammerOuterLoopPid_t;      //拨弹电机外环pid
	  speedPidStruct rammerInnerLoopPid_t;        //拨弹电机内环pid
  RM6623Struct yaw_t;                         //yaw轴(横轴)电机初始化
	  postionPidStruct yawOuterLoopPid_t;         //yaw电机外环pid
	  speedPidStruct yawInnerLoopPid_t;           //yaw电机内环pid
  RM6623Struct pitch_t;                       //pitch轴（纵轴）电机初始化
  	postionPidStruct pitchOuterLoopPid_t;       //pitch电机外环pid
	  speedPidStruct pitchInnerLoopPid_t;         //pitch电机内环pid
	xQueueHandle gimbal_queue;
  /* ----------------- 全局变量 -------------------- */
    uint8_t pc_data[3] ={0};
   float pamamer__p =0.02;
   float pamamer__y =0.04;
	/**
	* @Data    2019-01-27 17:09
	* @brief   云台结构体初始化
	* @param  CAN_HandleTypeDef* hcanx（x=1,2）
	* @retval  void
	*/
	void GimbalStructInit(CAN_HandleTypeDef *hcanx)
	{
		gimbal_t.hcanx = hcanx;
    gimbal_t.huartx = &huart2;
		/* ---------------- 拨弹电机初始化 ----------------- */
			gimbal_t.prammer_t = &rammer_t;
			rammer_t.rx_id = RAMMER_RX_ID;//电机can的 ip
		  rammer_t.target = 0; //目标值
		  rammer_t.real_current = 0;//真实电流
		  rammer_t.real_angle = 0;//真实角度
		  rammer_t.real_speed = 0;//真实速度
			rammer_t.error[0] = 0;
			rammer_t.error[1] = 0;
			rammer_t.error[2] = 0;
			/* ------ 外环pid参数 ------- */
				rammer_t.pOuterLoop_t = &rammerOuterLoopPid_t;
				rammerOuterLoopPid_t.kp = 0;
				rammerOuterLoopPid_t.kd = 0;
				rammerOuterLoopPid_t.ki = 0;
				rammerOuterLoopPid_t.error = 0;
				rammerOuterLoopPid_t.last_error = 0;//上次误差
				rammerOuterLoopPid_t.integral_er = 0;//误差积分
				rammerOuterLoopPid_t.pout = 0;//p输出
				rammerOuterLoopPid_t.iout = 0;//i输出
				rammerOuterLoopPid_t.dout = 0;//k输出
				rammerOuterLoopPid_t.pid_out = 0;//pid输出
			/* ------ 内环pid参数 ------- */
				rammer_t.pInnerLoop_t = &rammerInnerLoopPid_t;
				rammerInnerLoopPid_t.kp = 0;
				rammerInnerLoopPid_t.kd = 0;
				rammerInnerLoopPid_t.ki = 0;
				rammerInnerLoopPid_t.error = 0;
				rammerInnerLoopPid_t.last_error = 0;//上次误差
				rammerInnerLoopPid_t.before_last_error = 0;//上上次误差
				rammerInnerLoopPid_t.integral_er = 0;//误差积分
				rammerInnerLoopPid_t.pout = 0;//p输出
				rammerInnerLoopPid_t.iout = 0;//i输出
				rammerInnerLoopPid_t.dout = 0;//k输出
				rammerInnerLoopPid_t.pid_out = 0;//pid输出
   /* ---------------- YAW轴电机初始化 ----------------- */
    gimbal_t.pYaw_t = &yaw_t;
     yaw_t.id = YAW_RX_ID;             //电机can的 ip
		 yaw_t.target = 0;          //目标值
		 yaw_t.tem_target = 0;        //临时目标值
		 yaw_t.real_current = 0;        //真实电流
		 yaw_t.real_angle = 0;         //真实角度
		 yaw_t.tem_angle = 0;          //临时角度
		 yaw_t.zero = 0;             //电机零点
		 yaw_t.Percentage = 0;           //转换比例（减速前角度:减速后的角度 = x:1
     yaw_t.thresholds = 0;        //电机反转阀值
    	/* ------ 外环pid参数 ------- */
				yawOuterLoopPid_t.kp = 0;
				yawOuterLoopPid_t.kd = 0;
				yawOuterLoopPid_t.ki = 0;
				yawOuterLoopPid_t.error = 0;
				yawOuterLoopPid_t.last_error = 0;//上次误差
				yawOuterLoopPid_t.integral_er = 0;//误差积分
				yawOuterLoopPid_t.pout = 0;//p输出
				yawOuterLoopPid_t.iout = 0;//i输出
				yawOuterLoopPid_t.dout = 0;//k输出
				yawOuterLoopPid_t.pid_out = 0;//pid输出
    yaw_t.ppostionPidStruct = &yawOuterLoopPid_t;
    	/* ------ 内环pid参数 ------- */
				yawInnerLoopPid_t.kp = 0;
				yawInnerLoopPid_t.kd = 0;
				yawInnerLoopPid_t.ki = 0;
				yawInnerLoopPid_t.error = 0;
				yawInnerLoopPid_t.last_error = 0;//上次误差
				yawInnerLoopPid_t.before_last_error = 0;//上上次误差
				yawInnerLoopPid_t.integral_er = 0;//误差积分
				yawInnerLoopPid_t.pout = 0;//p输出
				yawInnerLoopPid_t.iout = 0;//i输出
				yawInnerLoopPid_t.dout = 0;//k输出
				yawInnerLoopPid_t.pid_out = 0;//pid输出
	   yaw_t.pspeedPidStruct = &yawInnerLoopPid_t;
   /* ---------------- pitch轴电机初始化 ----------------- */
    gimbal_t.pPitch_t = &pitch_t;
     pitch_t.id = PITCH_RX_ID;             //电机can的 ip
		 pitch_t.target = 0;          //目标值
		 pitch_t.tem_target = 0;        //临时目标值
		 pitch_t.real_current = 0;        //真实电流
		 pitch_t.real_angle = 0;         //真实角度
		 pitch_t.tem_angle = 0;          //临时角度
		 pitch_t.zero = 0;             //电机零点
		 pitch_t.Percentage = 0;           //转换比例（减速前角度:减速后的角度 = x:1
     pitch_t.thresholds = 0;        //电机反转阀值
    	/* ------ 外环pid参数 ------- */
				pitchOuterLoopPid_t.kp = 0;
				pitchOuterLoopPid_t.kd = 0;
				pitchOuterLoopPid_t.ki = 0;
				pitchOuterLoopPid_t.error = 0;
				pitchOuterLoopPid_t.last_error = 0;//上次误差
				pitchOuterLoopPid_t.integral_er = 0;//误差积分
				pitchOuterLoopPid_t.pout = 0;//p输出
				pitchOuterLoopPid_t.iout = 0;//i输出
				pitchOuterLoopPid_t.dout = 0;//k输出
				pitchOuterLoopPid_t.pid_out = 0;//pid输出
    pitch_t.ppostionPidStruct = &pitchOuterLoopPid_t;
    	/* ------ 内环pid参数 ------- */
				pitchInnerLoopPid_t.kp = 0;
				pitchInnerLoopPid_t.kd = 0;
				pitchInnerLoopPid_t.ki = 0;
				pitchInnerLoopPid_t.error = 0;
				pitchInnerLoopPid_t.last_error = 0;//上次误差
				pitchInnerLoopPid_t.before_last_error = 0;//上上次误差
				pitchInnerLoopPid_t.integral_er = 0;//误差积分
				pitchInnerLoopPid_t.pout = 0;//p输出
				pitchInnerLoopPid_t.iout = 0;//i输出
				pitchInnerLoopPid_t.dout = 0;//k输出
				pitchInnerLoopPid_t.pid_out = 0;//pid输出
	   pitch_t.pspeedPidStruct = &pitchInnerLoopPid_t;

				// /* ------ 云台消息队列创建 ------- */
				// gimbal_queue = xQueueCreate(5,3);
        UserCanConfig(hcanx);

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
				RM2006ParseData(&rammer_t,data);
				break;
			case YAW_RX_ID:
				RM6623ParseData(gimbal_t.pYaw_t,data);
        /* -------- 比例转换 --------- */
        gimbal_t.pYaw_t->real_angle = RatiometricConversion  \
        (gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds,gimbal_t.pYaw_t->Percentage);
        /* -------- 过零处理 --------- */
         zeroArgument(gimbal_t.pYaw_t->real_angle,gimbal_t.pYaw_t->thresholds); 
				break;
			case PITCH_RX_ID:
				RM6623ParseData(gimbal_t.pPitch_t,data);
        /* -------- 过零处理 --------- */
         zeroArgument(gimbal_t.pPitch_t->real_angle,gimbal_t.pPitch_t->thresholds); 
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
	void GimbalControl(const dbusStruct* dbus)
	{
    int16_t p,y;
    if(dbus->switch_left == 1)//遥控模式
    {
      y= DbusAntiShake(dbus->ch1,20);
      gimbal_t.pYaw_t->target += (int16_t)(y*pamamer__y);
      gimbal_t.pPitch_t->target += (int16_t)(p*pamamer__p);
  
        // GimbalCanTx();
    }
	
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
		return(CanTxMsg(gimbal_t.hcanx,GIMBAL_CAN_TX_ID,s));
	}
/*---------------------------------80字符限制-----------------------------------*/
  /**
  * @Data    2019-02-24 23:55
  * @brief   小电脑数据接收
  * @param   void
  * @retval  void
  */
  HAL_StatusTypeDef RxPCMsg(void)
  {
    return (HAL_UART_Receive(gimbal_t.huartx,pc_data,3,1));
  }
/*-----------------------------------file of end------------------------------*/
