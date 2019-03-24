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
typedef struct powerBufferPoolStruct
{
	currentMeterStruct* pcurrentMeter_t;
	float max_p;
	float max_w;//功率单位mW
	float r_w;//功率单位mW
	float current_mapp_coe;//电流映射系数
	float high_water_level;
	float low_water_level;
	float mid_water_level;
	float period;//运行周期，单位/s
	float high_current_threshold;//mA
  float mid_current_threshold;//mA
  float low_current_threshold;//mA
  float safe_current_threshold;//mA
}powerBufferPoolStruct;
int16_t WaterOutlet(powerBufferPoolStruct* pbs,int16_t input);
float OutMapCurrent(int16_t coe,int16_t input);
	int16_t CurrentMapOut(int16_t coe,int16_t current);
    uint8_t Inject(powerBufferPoolStruct* pbs);
  uint8_t GetPowerPoolState(powerBufferPoolStruct* pbs);
#endif	// __POWER_BUFFER_POOL_H
/*-----------------------------------file of end------------------------------*/


