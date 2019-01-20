/**
	|-------------------------------- Copyright ----------------------------------------|
	|                                                                                   |
	|                        (C) Copyright 2018,海康平头哥,                              |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China            |
	|                            All Rights Reserved                                    |
	|                                                                                   |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)               |
	|                     https://github.com/GCUWildwolfteam                            |
	|-----------------------------------------------------------------------------------|
	|--FileName    : Chassis.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-11               
	|--Description : 1、三轮底盘
	|								 2、用底盘文件,请包含motor.h和motor.c文件                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end---------------------------------|
 **/
#include "Chassis.h" 
	  /**
		* @Data    2018-12-11 20:25
		* @brief   底盘数据初始化话
		* @param   chassisClass_t * cC
		* @retval  void
		*/
		HAL_StatusTypeDef ChassisClassInit(chassiStruct * cc,CAN_HandleTypeDef* hcan\
																											,photoelectricStruct* ps)
		{
		/* -------- 检测缓存空间是否分配成功 --------- */		
			if(cc == NULL)
	 			return HAL_ERROR;
			SET_BIT(e_objects_flag,CREATE_OBJECTS_OF_CHASSIS); //获取标志
			cc->p_canx = RecognizeCanType(hcan); //识别can类型，获can类型地址
		/* -------- 检测传值 --------- */
			if(cc->p_canx != hcan&&cc->p_canx ==NULL)
			{
				SET_BIT(e_periphera_error_flag,GET_CAN_ADDR_FAIL);
				return HAL_ERROR;
			}
			cc->thirdWheelChassis_t.motorspeed1= 0;
			cc->thirdWheelChassis_t.motorspeed2 = 0;
			cc->thirdWheelChassis_t.motorspeed3 = 0;
			cc->thirdWheelChassis_t.v_x = 0;
			cc->thirdWheelChassis_t.v_y = 0;
			cc->thirdWheelChassis_t.v_w = 0;
			cc->status = 0;
			cc->p_addr_cache = NULL; //不能动这句话（危险）
			UserCANConfig(hcan);          //can初始化

			/* -------- 继承 --------- */
				cc->photoelectric_t = ps;
			/* -------- 模块初始化 --------- */
				PhotoelectricInit(ps,hcan);
			return HAL_OK;
		}
  /**
	* @Data    2018-12-11 20:31
	* @brief   底盘电机模式初始化
	* @param   uint8_t mode 底盘电机控制模式
	* @retval  void
	*/
	void ChassisMotorModeInit(uint8_t mode)
	{
		// CAN_TRANSMIT(&hcan,0x001,mode,0,0,0); //选择进入电流速度模式
	}
/*------------------------------------file of end------------------------------------*/


