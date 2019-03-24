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
/* -------------- 模块标志位宏定义 ----------------- */
#define   RC_MODE_READ                 (0x80000000U)//遥控模式 
 #define  ELUDE_MODE_READ              (0x40000000U) //躲避模式     
 #define  PC_SHOOT_MODE_READ           (0x20000000U) //自瞄打击模式准备就绪
 #define  CRUISE_MODE_READ             (0x10000000U)//巡航模式模式准备就绪
 #define  RC_MODE_RUNING               (0x08000000U)//遥控模式执行 
 #define  ELUDE_MODE_RUNING            (0x04000000U) //躲避模式 
 #define  PC_SHOOT_MODE_RUNING         (0x02000000U) //自瞄打击模式执行   
 #define  CRUISE_MODE_RUNING           (0x01000000U) //巡航模式执行
// #define  DISABLE_MOD                (0xF0000000U)//使能运行模块

// #define  RC_MODE_READ               (0x02000000U)//遥控模式准备就绪
// #define  JUDGE_READ                 (0x0F000000U)//判断决策
// #define  DELEC_USER_MODE            (0x00FFFFFFU)//清除用户自定义标志位
/* -------------- 外部链接 ----------------- */
extern TIM_HandleTypeDef htim5;
extern	osThreadId startChassisTaskHandle;
/* -------------- 静态变量 ----------------- */
	 chassisStruct chassis_t;
    static RM3508Struct wheel1_t;         //轮子1
      static speedPidStruct wheel1Speed_t;
    static RM3508Struct wheel2_t;         //轮子2
      static speedPidStruct wheel2Speed_t;
     static powerBufferPoolStruct powerBufferPool_t;
			static currentMeterStruct currtenMeter_t;
/*-------------------------------------------------------
  |       型号     |  线数 |  brown | bule  |black|while|
  -------------------------------------------------------
  |欧姆龙 E6A2-CW3C|500P/R |5to12VDC|0V(GND)|OUT A|OUT B|
 -------------------------------------------------------*/
		static incrementalEnconderStruct chassisEnconder_t; //编码器结构体
/* -------------- 私有宏 ----------------- */
	#define WHEEL1_RX_ID      			 0x201
	#define WHEEL2_RX_ID       			 0x202
	#define CURRENT_METER_RX_ID      0x401//电流计接收id
	#define CHASSIS_CAN_TX_ID  			 0x200
	#define W1_LIMIT_SPEED    			 3000  //轮子1速度限幅
	#define W2_LIMIT_SPEED    			 3000  //轮子2速度限幅
	#define RADIUS            			 30    //编码器轮子半径单位 mm
	#define ENCONDER_POLES    			 500 

	/**
	* @Data    2019-01-27 17:09
	* @brief   底盘结构体数据初始化
	* @param  CAN_HandleTypeDef* hcanx（x=1,2）
	* @retval  void
	*/
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc)
	{
		chassis_t.hcanx = hcan;
    chassis_t.rc_t =  rc;
    chassis_t.status = 0;
    chassis_t.ppowerBufferPool_t = PowerBufferPoolInit();
		/* ------ 轮子1结构体数据初始化 ------- */
		chassis_t.pwheel1_t = wheel1Init();
		/* ------ 轮子2结构体数据初始化 ------- */
		chassis_t.pwheel2_t = wheel2Init();
    /* -------- can配置 --------- */
      if(UserCanConfig(hcan)!= HAL_OK)
        while(1){}//待添加防重复配置功能
		/* ------ 编码器初始化 ------- */
	 	chassis_t.pchassisEnconder_t = &chassisEnconder_t;
		  if(EnconderInit(&chassisEnconder_t,RADIUS,ENCONDER_POLES) !=HAL_OK)
        while(1){}
		/* ------ 超声波初始化 ------- */
	  	HCSR04Init();
    /* -------- 底盘模块初始化判断 --------- */
      SET_BIT(chassis_t.status,INIT_OK);//初始化成功
    	vTaskSuspend(startChassisTaskHandle);
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
    SET_BIT(chassis_t.status,RX_OK);//接受成功
	}
/**
	* @Data    2019-02-15 15:10
	* @brief   底盘数据发送
	* @param   void
	* @retval  void
	*/
	void ChassisCanTx(int16_t w1,int16_t w2)
	{
		uint8_t s[8]={0};
		s[0] = (uint8_t)(w1<<8);
		s[1] = (uint8_t)w1;
		s[2] = (uint8_t)(w2<<8);
		s[3] = (uint8_t)w2;
		CanTxMsg(chassis_t.hcanx,CHASSIS_CAN_TX_ID,s);
	}
/**
	* @Data    2019-03-12 16:25
	* @brief   底盘控制函数
	* @param   void
	* @retval  void
	*/
	void ChassisControl(void)
	{
  //  chassis_t.pwheel1_t->target = GetDistance(3);
  //  chassis_t.pwheel2_t->target = GetDistance(4);
	// 	SET_BIT(chassis_t.status,RUNING_OK);
	}
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
  * @Data    2019-03-13 03:19
  * @brief   遥控模式
  * @param   void
  * @retval  void
  */
  void ChassisRcControlMode(void)
  {
		chassis_t.pwheel1_t->error = CAL_ERROR(chassis_t.rc_t->ch1,chassis_t.pwheel1_t->real_speed);
		chassis_t.pwheel2_t->error = CAL_ERROR(chassis_t.rc_t->ch2,chassis_t.pwheel2_t->real_speed);
    SpeedPid(chassis_t.pwheel1_t->pspeedPid_t,chassis_t.pwheel1_t->error);
		SpeedPid(chassis_t.pwheel2_t->pspeedPid_t,chassis_t.pwheel2_t->error);
		ChassisCanTx(chassis_t.pwheel1_t->pspeedPid_t->pid_out,chassis_t.pwheel2_t->pspeedPid_t->pid_out);
  }
  /**
  * @Data    2019-03-25 00:28
  * @brief   控制优先权决策
  * @param   void
  * @retval  void
  */
  void ChassisControlPriorityDecision(void)
  {
    // if(chassis_t.status > RC_MODE_READ)
    // {
    //   SET_BIT(chassis_t.status,RC_MODE_RUNING);
    //   CLEAR_BIT(chassis_t.status,ELUDE_MODE_RUNING);
    //   CLEAR_BIT(chassis_t.status,PC_SHOOT_MODE_RUNING);
    //   CLEAR_BIT(chassis_t.status,CRUISE_MODE_RUNING);
    // }
    // else if(chassis_t.status > )
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
    powerBufferPool_t.max_p = 0;
    powerBufferPool_t.max_w = 0;//功率单位mW
    powerBufferPool_t.r_w = 0;//功率单位mW
    powerBufferPool_t.current_mapp_coe = 0;//电流映射系数
    powerBufferPool_t.high_water_level = 0;
    powerBufferPool_t.low_water_level = 0;
    powerBufferPool_t.mid_water_level = 0;
    powerBufferPool_t.period = 0;//运行周期，单位/s
    powerBufferPool_t.high_current_threshold = 0;//mA
    powerBufferPool_t.mid_current_threshold = 0;//mA
    powerBufferPool_t.low_current_threshold = 0;//mA
    powerBufferPool_t.safe_current_threshold = 0;//mA
    return &powerBufferPool_t;
  }
  /**
  * @Data    2019-03-25 00:12
  * @brief   初始化轮子参数
  * @param   void
  * @retval  void
  */
  RM3508Struct* wheel1Init(void)
  {
      wheel1_t.id = WHEEL1_RX_ID;//电机can的 ip
			wheel1_t.target = 0;		 //目标值
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
      wheel1_t.ppostionPid_t = NULL;
      wheel1_t.pspeedPid_t = &wheel1Speed_t;
				wheel1Speed_t.kp = 1;
				wheel1Speed_t.kd = 0;
				wheel1Speed_t.ki = 0;
				wheel1Speed_t.error = 0;
				wheel1Speed_t.last_error = 0;//上次误差
				wheel1Speed_t.before_last_error = 0;//上上次误差
				wheel1Speed_t.integral_er = 0;//误差积分
				wheel1Speed_t.pout = 0;//p输出
				wheel1Speed_t.iout = 0;//i输出
				wheel1Speed_t.dout = 0;//k输出
				wheel1Speed_t.pid_out = 0;//pid输出
				wheel1Speed_t.limiting = W1_LIMIT_SPEED;//轮子1速度限幅
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
			wheel2_t.target = 0;		 //目标值
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
 			wheel2_t.ppostionPid_t = NULL;
      wheel2_t.pspeedPid_t = &wheel2Speed_t;
				wheel2Speed_t.kp = 1;
				wheel2Speed_t.kd = 0;
				wheel2Speed_t.ki = 0;
				wheel2Speed_t.error = 0;
				wheel2Speed_t.last_error = 0;//上次误差
				wheel2Speed_t.before_last_error = 0;//上上次误差
				wheel2Speed_t.integral_er = 0;//误差积分
				wheel2Speed_t.pout = 0;//p输出
				wheel2Speed_t.iout = 0;//i输出
				wheel2Speed_t.dout = 0;//k输出
				wheel2Speed_t.pid_out = 0;//pid输出
				wheel2Speed_t.limiting = W2_LIMIT_SPEED;//轮子2速度限幅
      return &wheel2_t;
  }
/*----------------------------------file of end-------------------------------*/
