/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2018,海康平头哥,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                           All Rights Reserved                              |
	|                                                                            |
	|          By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                   https://github.com/GCUWildwolfteam                       |
	|----------------------------------------------------------------------------|
	|--FileName    : motor.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-31               
	|--Libsupports : 标准库和HAL库
	|--Description : 1、maxion电机+RoboModule驱动 
	|								 2、3508电机+c610电调
	|								 3、6623电机
	|                4、6002电机
	|								 5、                                                        
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#include "motor.h" 
/* ==============================common of begin ============================ */
	static int16_t s_max_motor_lines = 8192 ;//电机一圈最大线数
	/**
	* @Data    2019-01-18 20:14
	* @brief   比例转换  
	* @param   real 真实值
	* @param   threshold 一圈最大线数的阀值
	* @param   perce //转换比例（减速前角度:减速后的角度 = x:1
  * @param   uint32_t status 模块的状态
	* @retval  int16_t 换算后的目标值
	*/
	int16_t RatiometricConversion(int16_t real,int16_t threshold,int16_t perce,int32_t* last_real,int16_t* coefficient,uint32_t status)
	{
		int32_t tem=0;
    if((status&START_OK) ==START_OK)
    {
       if ((real - *last_real) < -threshold)
      {
        /*角度系数循环自增,下面转36圈归零系数,设置范围[0,perce] */
        *coefficient =(*coefficient+1)%(perce);
      }
     else if((real -*last_real) > threshold)
      {
        *coefficient = (perce-1)-((((perce-1) - *coefficient)+1)%(perce));
      }
      *last_real = real;//缓存现在值
      tem = real + (s_max_motor_lines* (*coefficient)); //转换总角度
      return ((int16_t)((float)tem/((float)perce)));//换算成上面转一圈
    }
    else
    {
      *last_real = real;//缓存现在值
      return ((int16_t)((float)real/((float)perce)));//换算成上面转一圈
    }
	}
	/**
	* @Data    2019-01-18 20:14
	* @brief    圈数累计转换  
	* @param   real 真实值
	* @param   threshold 一圈最大线数的阀值
	* @param   perce //转换比例（减速前角度:减速后的角度 = x:1
  * @param   uint32_t status 模块的状态
	* @retval  int16_t 换算后的目标值
	*/
	int16_t NoRatiometricConversion(int16_t real,int16_t threshold,int16_t perce,int32_t* last_real,int16_t* coefficient,uint32_t status)
	{
		int32_t tem=0;
    if((status&START_OK) ==START_OK)
    {
       if ((real - *last_real) < -threshold)
      {
        /*角度系数循环自增,下面转36圈归零系数,设置范围[0,perce] */
        *coefficient =(*coefficient+1)%(perce);
      }
     else if((real -*last_real) > threshold)
      {
        *coefficient = (perce-1)-((((perce-1) - *coefficient)+1)%(perce));
      }
      *last_real = real;//缓存现在值
      tem = real + (s_max_motor_lines* (*coefficient)); //转换总角度
      return ((int16_t)(tem *(float)0.5));//换算成上面转一圈
    }
    else
    {
      *last_real = real;//缓存现在值
      return ((int16_t)(real *(float)0.5));//换算成上面转一圈
    }
	}
	/**
	* @Data    2019-01-18 20:48
	* @brief   零点处理
	* @param   real 真实值
	* @param   threshold 一圈最大线数的阀值
	* @retval  int16_t 换算之后的目标值
	*/
	int16_t zeroArgument(int16_t real,int16_t threshold)
	{
		static int16_t last_realdata = 0,counter = 0;
		if(real - last_realdata < threshold) 
			counter = 1;
		if(last_realdata - real <threshold)
			counter = -1;
			else counter = 0;
		last_realdata = real;
		return (real + (s_max_motor_lines * counter));
	}
  /**
  * @Data    2019-03-13 03:55
  * @brief   计算PID控制系统误差
  * @param    int16_t target 目标值
  * @param   int16_t real 真实值
  * @param    int16_t max_speed 一个计算周期内的最大速度
  * @retval  当前误差
  */
  int16_t CalculateError(int16_t target,int16_t real,int16_t max_speed,int16_t linesnumb)
  {
    int16_t error;
    error = target - real;
    if(ABS(error) <= max_speed)
    {
      return error;
    }
    else
    {
      if(error < (-max_speed))
      {
        error = target - (real - linesnumb);
      }
      if(error > max_speed)
      {
        error = (target -linesnumb) - real;
      }
      return error;
    }
  }
   /**
  * @Data    2019-03-13 03:55
  * @brief   速度PID控制系统误差
  * @param    int16_t target 目标值
  * @param   int16_t real 真实值
  * @retval  当前误差
  */
  int16_t CalculatePationError(int16_t target,int16_t real)
  {
    return (target - real);
  }
/* ============================ common of end =============================== */
/* ============================= RM6623 of begin ============================ */
	/**
		* @Data    2019-01-18 19:59
		* @brief   RM6623结构体初始化
		* @param   RM6623电机结构体地址
		* @retval  void
		*/
	void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx)
	{
		RM6623->id = 0;
		RM6623->target = 0;				//目标值
		RM6623->tem_target = 0;		//临时目标值
		RM6623->real_current = 0; //真实电流
		RM6623->real_angle = 0;		//真实角度
		RM6623->tem_angle = 0;		//临时角度
		RM6623->zero = 0;					//电机零点
		RM6623->Percentage = 0;		//转换比例（减速前角度:减速后的角度 = x:1）
		/*根据读取时间间隔，估算最大转过线数p，thresholds = p -s_max_motor_lines */
		RM6623->thresholds = 1200 - s_max_motor_lines;//1200是瞎给的，别计较
	}
		/**
		* @Data    2019-01-18 20:03
		* @brief   6623数据解析
		* @param   RM6623电机结构体地址
		* @retval  HAL Status
		*/
	void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data)
	{
		RM6623->real_current = ((int16_t)(data[4] << 8) | data[5]);
		RM6623->tem_angle = ((int16_t)(data[0] << 8) | data[1]);
	}

/* ============================= RM6623 of end ============================== */
/* ============================= 3508 of begin ============================== */
	/**
	* @Data    2019-01-19 00:42
	* @brief   3508数据解析
	* @param   RM3508Struct 3508结构体指针
	* @param   *data  接收的数据指针
	* @retval  void
	*/
	void RM3508ParseData(RM3508Struct *RM3508,uint8_t *data)
	{
		RM3508->real_angle=((int16_t)(data[0]<<8)|data[1]);
		RM3508->real_speed=((int16_t)(data[2]<<8)|data[3]);
		RM3508->real_current=((int16_t)(data[4]<<8)|data[5]);
	}
/* ============================== 3508 of end =============================== */
/* ============================= M2006 of begin ============================= */
	/**
		* @Data    2019-01-26 18:21
		* @brief   基准方向顺时针M2006数据解析
		* @param   M2006struct 2006结构体指针
		* @param   *data  接收的数据指针
		* @retval  void
		*/
		void RM2006ParseData(M2006Struct *M2006,uint8_t *data)
		{
			M2006->tem_angle  = ((int16_t)(data[0]<<8)|data[1]);
    	M2006->real_speed = ((int16_t)(data[2]<<8)|data[3]);
			M2006->real_current=((int16_t)(data[4]<<8)|data[5]);
		}
 /**
  * @Data    2019-01-26 18:21
  * @brief   基准方向逆时针M2006数据解析
  * @param   M2006struct 2006结构体指针
  * @param   *data  接收的数据指针
  * @retval  void
  */
  void AntiRM2006ParseData(M2006Struct *M2006,uint8_t *data)
  {
    M2006->real_angle  = M2006_POLES - ((int16_t)(data[0]<<8)|data[1]);
    M2006->real_speed = -((int16_t)(data[2]<<8)|data[3]);
    M2006->real_current= - ((int16_t)(data[4]<<8)|data[5]);
    if(M2006->real_angle == 8192)
    M2006->real_angle = 0;
  }

/* ============================== M2006 of end ============================== */
/* =========================== M6020 of begin =========================== */
	/**
	* @Data    2019-01-19 00:42
	* @brief   6020数据解析
	* @param   RM3508Struct 3508结构体指针
	* @param   *data  接收的数据指针
	* @retval  void
	*/
	void GM6020ParseData(GM6020Struct* GM6020,uint8_t *data)
    {
      GM6020->real_angle  = ((int16_t)(data[0]<<8)|data[1]);
    	GM6020->real_speed = ((int16_t)(data[2]<<8)|data[3]);
			GM6020->real_current=((int16_t)(data[4]<<8)|data[5]);
    }
/* =========================== M6020 of end =========================== */


/* =======================-===== maxion of begin ============================ */
	/**
	* @Data    2019-01-04 13:27
	* @brief   速度模式下的数据指令
	* @param   Group   取值范围 0-7
	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
	* @param	 temp_pwm的取值范围如下：
							0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压
	* @param	 temp_velocity的取值范围如下：
							-32768 ~ +32767，单位RPM
	* @retval  HAL_StatusTypeDef
	*/
	/**
		* @Data    2019-02-16 16:17
		* @brief   复位指令
		* @param   void
		* @retval  void
		*/
		void ResetMode(CAN_HandleTypeDef *hcanx,uint32_t rx_id)
		{
      uint8_t s[8];
      memset(s,0x55,8);
			rx_id |= reset_fun_e;
			CanTxMsg(hcanx,rx_id,s);
		}
	/**
		* @Data    2019-02-16 16:35
		* @brief   控制模式选择
		* @param   void
		* @retval  void
		*/
		void ModeSelectionMode(CAN_HandleTypeDef *hcanx,uint32_t rx_id,uint8_t mode)
		{
			uint8_t s[8];
       memset(s,0x55,8);
			rx_id |= mode_selection_fun_e;
			s[0] = mode;
			CanTxMsg(hcanx,rx_id,s);
		}
  /**
	* @Data    2019-02-16 13:15
	* @brief   速度模式指令
	* @param	 Speed的取值范围如下：
						-32768 ~ +32767，单位RPM
	* @retval  void
	*/
	void SpeedLoopMode(int16_t pwm,int16_t Speed,uint8_t *data)
	{
     memset(data,0x55,8);
		TwobyteToByle(pwm,(data));
		TwobyteToByle(Speed,(data+2));
	}
	 /**
		* @Data    2019-02-16 13:31
		* @brief   开环模式指令
		* @param   void
		* @retval  void
		*/
		void OpenLoopMode(int16_t pwm,uint8_t *data)
		{
       memset(data,0x55,8);
			TwobyteToByle(pwm,(data));
			DataFilling((data+2),0x55,8);
		}
	/**
	* @Data    2019-02-17 17:12
	* @brief   配置指令
	* @param   void
	* @retval  void
	*/
	void ConfigMode(CAN_HandleTypeDef *hcanx,uint32_t rx_id,uint8_t Time,\
																													uint8_t Ctl1_Ctl2)
	{
    uint8_t s[8];
		uint32_t id = config_fun_e;
		id |= rx_id;
		if((Ctl1_Ctl2 != 0x00)&&(Ctl1_Ctl2 != 0x01))
    {
        Ctl1_Ctl2 = 0x00;
    }
    memset(s,0x55,8);
		s[0] = Time;
		s[1] = Ctl1_Ctl2;
		CanTxMsg(hcanx,id,s);
	}
	/**
		* @Data    2019-02-16 14:10
		* @brief   电机can发送
		* @param   void
		* @retval  void
		*/
		void SendMotorMsg(void)
		{
			
		}
	/**
	* @Data    2019-01-23 00:00
	* @brief   maxioncan报文解析
	* @param   void
	* @retval  void
	*/
	void MaxionParseData(maxionStruct*maxion,uint8_t *data)
	{
		maxion->real_current  = (data[0]<<8)|data[1];
		maxion->real_velocity = (data[2]<<8)|data[3];
		maxion->real_position = (data[4]<<24)| (data[5]<<16)| (data[6]<<8)| data[7];
	}
	/**
		* @Data    2019-02-16 16:15
		* @brief   maxion电机初始化
		* @param   void
		* @retval  void
		*/
		void MotorInit(CAN_HandleTypeDef *hcanx,uint8_t id,uint8_t mode)
		{
//			ResetMode(hcanx,id);//发送复位指令
			osDelay(500);//等待500ms
//			ModeSelectionMode(hcanx,id,mode);
			osDelay(500);//等待500ms
		}
/* ============================= maxion of end ============================== */
/* =========================== PWM控制的电机 of begin ======================== */
/**
	* @Data    2019-03-16 16:47
	* @brief   
	* @param   void
	* @retval  void
	*/
	void BrushlessMotorInit(void)
	{
		HAL_TIM_PWM_Start(FRICTIONGEAR,FRICTIONGEAR_1);
		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_1_START_V);
	}
/* =========================== PWM控制的电机 of end ========================== */


/*---------------------------------file of end--------------------------------*/
