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
	|--FileName    : power_buffer_pool.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-03-24               
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
#ifndef __POWER_BUFFER_POOL_H 
#define __POWER_BUFFER_POOL_H 
#include "baseclass.h "
#include "currentmeter.h"
#include "communicate.h"
typedef struct powerBufferPoolStruct
{
	currentMeterStruct* pcurrentMeter_t;
    refereeSystemStruct* p_refereeSystem_t;
	float max_p;
	float max_w;//功率单位W
	float r_w;//功率单位W
  float r_p;//功率单位W
	float current_mapp_coe;//电流映射系数
	float high_water_level;
	float low_water_level;
	float mid_water_level;
	float period;//运行周期，单位/s
	float high_current_threshold;//A
  float mid_current_threshold;//A
  float low_current_threshold;//A
  float safe_current_threshold;//A
}powerBufferPoolStruct;
float OutMapCurrent(float coe,int16_t input);
	int16_t CurrentMapOut(float coe,float current);
    uint8_t Inject(powerBufferPoolStruct* pbs);
int16_t GetOutlet(powerBufferPoolStruct* pbs,int16_t input);
  uint8_t GetPowerPoolState(powerBufferPoolStruct* pbs);
void SetInPut(powerBufferPoolStruct* pbs,int16_t *input,uint8_t len);
#endif	// __POWER_BUFFER_POOL_H
/*-----------------------------------file of end------------------------------*/


