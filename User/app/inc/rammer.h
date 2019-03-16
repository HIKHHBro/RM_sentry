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
	|--FileName    : rammer.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-03-16               
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
#ifndef __RAMMER_H 
#define __RAMMER_H 
#include "motor.h"
#include "DJI_dbus.h" 
	#define RAMMER_ANGER_COE         (0.0012F)//拨弹电机角度转换系数
	#define RAMMER_REDUCTION         (36U)//减速比
	#define STUCK_BULLET_THRE        3000//卡弹电流阀值
	#define RAMMER_SPEED             50//单位10ms
	M2006Struct* RammerInit(void);
	void RammerControl(void);
		int16_t PCycleNumerical(int16_t data);
		int16_t MCycleNumerical(int16_t data);
#endif	// __RAMMER_H
/*-----------------------------------file of end------------------------------*/


