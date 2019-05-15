/**
	|----------------------------- Copyright ------------------------------------|
	|                                                                            |
	|                     (C) Copyright 2018,海康平头哥,       --                 |
	|        1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China         |
	|                            All Rights Reserved                             |
	|                                                                            |
	|        By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)            |
	|                  https://github.com/GCUWildwolfteam                        |
	|----------------------------------------------------------------------------|
	|--FileName    : counters.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-11-27               
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#ifndef __COUNTERS_H 
#define __COUNTERS_H 
	#include "baseclass.h "

  #define LOCATION_MODE        0  //位置式
  #define INCREMENTAL_MODE     1  //增量式
  #define ER    										0x00U  		//误差
  #define LAST_ER 									0x01U  		//上次误差
  #define BEFORE_LAST_ER   					0x02U  		//上上次误差
	#define INTEGRAL_ER								0x03U     //积分
  #define KP  											0x04U  		//参数p
  #define KI 												0x05U  		//参数i
  #define KD  											0x06U  		//参数d
	#define TARGET 										0x07U			//目标值
	#define REAL											0x08U			//真实值
	#define P_OUT											0x09U			//p输出
	#define I_OUT											0x0AU			//i输出
	#define D_OUT											0x0BU			//d输出
	#define PID_OUT										0x0CU			//pid输出
	#define LIMIT_MAX									0x0DU     //最大限幅
	#define LIMIT_MIN									0x0EU     //最小限幅
	float Bezier(int x,int y);
	int16_t GetLengthForTwopoint(const int16_t *Q1,const int16_t *Q2);
/* -------------- 位置式pid ----------------- */
	typedef struct postionPidStruct
	{
		float kp;
		float kd;
		float ki;
		int16_t error;
		int16_t last_error;//上次误差
		int16_t integral_er;//误差积分
    int16_t integral_limint;
    int16_t integral_threshold;
    int32_t motor_lim;
		float pout;//p输出
		float iout;//i输出
		float dout;//k输出
		int32_t pid_out;//pid输出
    uint8_t kp_separatecmd;
	}postionPidStruct;
	int16_t PostionPid(postionPidStruct *pps, int16_t error);
   int16_t IntegralSeparationCallback(postionPidStruct *pps);
  int16_t KpSeparationCallback(postionPidStruct *pps);
/* -------------- 速度式pid ----------------- */
	typedef struct speedPidStruct
	{
		float kp;
		float kd;
		float ki;
		int16_t error;
		int16_t last_error;//上次误差
		int16_t before_last_error;//上上次误差
		int16_t integral_er;//误差积分
		int32_t pout;//p输出
		int32_t iout;//i输出
		int32_t dout;//k输出
		int32_t pid_out;//pid输出
		int16_t limiting;
    int32_t motor_lim;
}speedPidStruct;
int16_t SpeedPid(speedPidStruct *sps, int16_t error);
/* =========================== 贝塞尔曲线 of begin =========================== */
	typedef struct BezierStruct
	{
		uint32_t *coordinate_x;
		uint32_t *coordinate_y;
		uint8_t n;
		uint32_t point_x[8];
		uint32_t point_y[8];
		uint32_t precision;
		// void(*SetPoint)(uint32_t x,uint32_t y);//设置控制点
		// void(*SetPrecision)(uint32_t precision);//设置曲线精度
		// void(*SetPow)(uint8_t n);//设置贝塞尔阶数
	}bezierStruct;
	HAL_StatusTypeDef CreateDataSpace(bezierStruct* bs);
	uint32_t QuadTo(const uint8_t n,const uint32_t *point,float t);
/* =========================== 贝塞尔曲线 of end =========================== */
void Insert(int16_t fx[], int16_t x[]);
int16_t NewtonInterpolation(int16_t fx[], int16_t x[], int16_t a);
#endif	// __COUNTERS_H
	
/*--------------------------------file of end---------------------------------*/


