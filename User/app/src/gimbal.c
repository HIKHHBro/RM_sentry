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
/* -------------- 私有宏定义 ----------------- */
	#define RAMMER_RX_ID	  0x207
	#define YAW_RX_ID 			0
	#define PITCH_RX_ID 		0
	#define GIMBAL_CAN_TX_ID 0x1ff
/* -------------- 结构体声明 ----------------- */
	gimbalStruct gimbal_t;//云台结构体
//	M2006Struct rammer_t;//拨弹电机结构体
//	postionPidStruct rammerOuterLoopPid_t;//拨弹电机外环pid
//	speedPidStruct rammerInnerLoopPid_t;//拨弹电机内环pid
//	xQueueHandle gimbal_queue;
	/**
	* @Data    2019-01-27 17:09
	* @brief   云台结构体初始化
	* @param  CAN_HandleTypeDef* hcanx（x=1,2）
	* @retval  void
	*/
	void GimbalStructInit(CAN_HandleTypeDef *hcanx)
	{
		gimbal_t.hcanx = hcanx;
		/* ------ 拨弹电机初始化 ------- */
//			gimbal_t.prammer_t = &rammer_t;
//			rammer_t.id = RAMMER_RX_ID;//电机can的 ip
//		  rammer_t.target = 0; //目标值
//		  rammer_t.real_current = 0;//真实电流
//		  rammer_t.real_angle = 0;//真实角度
//		  rammer_t.real_speed = 0;//真实速度
//			rammer_t.error = 0;
//			/* ------ 外环pid参数 ------- */
//				rammer_t.ppostionPid_t = &rammerOuterLoopPid_t;
//				rammerOuterLoopPid_t.kp = 0;
//				rammerOuterLoopPid_t.kd = 0;
//				rammerOuterLoopPid_t.ki = 0;
//				rammerOuterLoopPid_t.error = 0;
//				rammerOuterLoopPid_t.last_error = 0;//上次误差
//				rammerOuterLoopPid_t.integral_er = 0;//误差积分
//				rammerOuterLoopPid_t.pout = 0;//p输出
//				rammerOuterLoopPid_t.iout = 0;//i输出
//				rammerOuterLoopPid_t.dout = 0;//k输出
//				rammerOuterLoopPid_t.pid_out = 0;//pid输出
//			/* ------ 内环pid参数 ------- */
//				rammer_t.pspeedPid_t = &rammerInnerLoopPid_t;
//				rammerInnerLoopPid_t.kp = 0;
//				rammerInnerLoopPid_t.kd = 0;
//				rammerInnerLoopPid_t.ki = 0;
//				rammerInnerLoopPid_t.error = 0;
//				rammerInnerLoopPid_t.last_error = 0;//上次误差
//				rammerInnerLoopPid_t.before_last_error = 0;//上上次误差
//				rammerInnerLoopPid_t.integral_er = 0;//误差积分
//				rammerInnerLoopPid_t.pout = 0;//p输出
//				rammerInnerLoopPid_t.iout = 0;//i输出
//				rammerInnerLoopPid_t.dout = 0;//k输出
//				rammerInnerLoopPid_t.pid_out = 0;//pid输出
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
//				RM2006ParseData(&rammer_t,data);
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
	HAL_StatusTypeDef GimbalCanTx(int16_t w1,int16_t w2)
	{
		uint8_t s[8]={0};
		s[4] = (uint8_t)(w1>>8);
		s[5] = (uint8_t)w1;
		s[6] = (uint8_t)(w2>>8);
		s[7] = (uint8_t)w2;
		return(CanTxMsg(gimbal_t.hcanx,GIMBAL_CAN_TX_ID,s));
	}
/*-----------------------------------file of end------------------------------*/
