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
	|---------------------------------declaration of end---------------------------------|
 **/
#ifndef __COUNTERS_H 
#define __COUNTERS_H 
	#include "baseclass.h"

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
#endif	// __COUNTERS_H
	
 /*------------------------------------file of end------------------------------------*/


