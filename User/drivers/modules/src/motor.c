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
/* ============================= RM6623 of begin ============================ */
	static int16_t s_max_motor_lines = 8192 ;//电机一圈最大线数
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
		RM6623->hcanx = hcanx;
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
		int16_t tem_angle = 0;
		RM6623->real_current = ((int16_t)(data[4] << 8) | data[5]);
		tem_angle = ((int16_t)(data[0] << 8) | data[1]);
		tem_angle = RatiometricConversion(tem_angle, RM6623->thresholds,\
																								 RM6623->Percentage);
		RM6623->real_angle = zeroArgument(tem_angle, RM6623->thresholds);
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
		* @brief   M2006数据解析
		* @param   M2006truct 2006结构体指针
		* @param   *data  接收的数据指针
		* @retval  void
		*/
		void RM2006ParseData(M2006Struct *M2006,uint8_t *data)
		{
			M2006->real_angle  = ((int16_t)(data[0]<<8)|data[1]);
    	M2006->real_speed = ((int16_t)(data[2]<<8)|data[3]);
			M2006->real_current=((int16_t)(data[4]<<8)|data[5]);
		}
/* ============================== M2006 of end ============================== */
/* ==============================common of begin ============================ */
	/**
	* @Data    2019-01-18 20:14
	* @brief   比例转换  
	* @param   real 真实值
	* @param   threshold 一圈最大线数的阀值
	* @param   perce //转换比例（减速前角度:减速后的角度 = x:1
	* @retval  int16_t 换算后的目标值
	*/
	int16_t RatiometricConversion(int16_t real,int16_t threshold,int16_t perce)
	{
		static int32_t last_real,tem=0;
		static uint16_t  coefficient=0;
		if (real - last_real < threshold)
		{
			/*角度系数循环自增,下面转36圈归零系数,设置范围[0,perce] */
			coefficient =(coefficient+1)%(perce);
		}
		else if(last_real - real < threshold)
		{
			coefficient = (perce-1)-((((perce-1) - coefficient)+1)%(perce));
		}
		last_real = real;//缓存现在值
		tem = real + (s_max_motor_lines* coefficient); //转换总角度
		return ((int16_t)(tem/(perce)));//换算成上面转一圈
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
/* ============================ common of end =============================== */
/*---------------------------------file of end--------------------------------*/
