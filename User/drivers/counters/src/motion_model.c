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
	|--FileName    : motion_model.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-02-17             
	|--Libsupports : 
	|--Description :各种运动模型算法                                                     
	|--FunctionList                                                     
	|-------1. ....                                                     
	|          <version>:                                                     
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-----------------------------declaration of end-----------------------------|
 **/
#include "motion_model.h" 
#include "Math.h"
/* ======================== 全向轮三轮运动模型 of begin ======================= */
#define COS_THETA 				 0
#define SIN_THETA 			   1
#define COS_60_MIN_THETA 	 2
#define SIN_60_MIN_THETA 	 3
#define COS_60_ADD_THETA 	 4
#define SIN_60_ADD_THETA	 5
#define RADIUS             6
float value[7];
	/**
	* @Data    2019-02-17 21:24
	* @brief  获取轮子半径长度，场地坐标和机器人坐标角度误差,计算运动模型相关参数
	* @param   uint8_t radius  轮子为圆的半径
	* @param   char theta 场地坐标和机器人坐标角度误差 
	* @retval  void
	*/
	void GetThreeMotionModeData(uint8_t radius,float theta)
	{
    //cos等运算不准确
//		value[COS_THETA] = cos(theta);
//	 	value[SIN_THETA] = sin(theta);
//	 	value[COS_60_MIN_THETA] = -cos((60 - theta));
//	 	value[SIN_60_MIN_THETA] = sin((60 - theta));
//	 	value[COS_60_ADD_THETA] = -cos((60 + theta));
//	 	value[SIN_60_ADD_THETA] = -sin((60 + theta));
//	 	value[RADIUS] = radius;
    value[COS_THETA] = 1;
	 	value[SIN_THETA] = 0;
	 	value[COS_60_MIN_THETA] = -0.5;
	 	value[SIN_60_MIN_THETA] = 1.732050;
	 	value[COS_60_ADD_THETA] = -0.5;
	 	value[SIN_60_ADD_THETA] = -1.732050;
	 	value[RADIUS] = radius;
	}
	/**
	* @Data    2019-02-17 20:17
	* @brief  全向轮三轮底盘运动模型
	* @param   void
	* @retval  void
	*/
	void ThreeWheelMotionModel(int16_t *motorspeed,const int16_t vx,const \
																								int16_t vy,const int16_t w)
	{
  	*motorspeed = (int16_t)(value[COS_THETA]*vx + value[SIN_THETA]*vy + \
																									value[RADIUS]*w);
		*(motorspeed+1) = (int16_t)(value[COS_60_MIN_THETA]*vx + \
																value[SIN_60_MIN_THETA]*vy + value[RADIUS]*w);
		*(motorspeed+2) = (int16_t)(value[COS_60_ADD_THETA]*vx + \
																value[SIN_60_ADD_THETA]*vy + value[RADIUS]*w);
	}
/* ======================== 全向轮三轮运动模型 of end ======================== */
/*-----------------------------------file of end------------------------------*/


