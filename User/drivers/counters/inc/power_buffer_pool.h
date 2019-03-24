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
	uint32_t max_p;
	uint32_t max_w;//功率单位mW
	uint32_t r_p;
	uint32_t r_w;//功率单位mW
  uint32_t buf_t;
	uint32_t rec_t;
	uint32_t current_mapp_coe;//电流映射系数
	uint32_t high_water_level;
	uint32_t low_water_level;
	uint32_t mid_water_level;
	uint32_t base_v;//基准电压 单位V
	uint32_t period;//运行周期，单位/ms
	uint32_t current_threshold;//mA
}powerBufferPoolStruct;
int16_t CurrentMapping(int16_t coe,int16_t input);
int16_t WaterOutlet(powerBufferPoolStruct* pbs,int16_t input);
	void CalculatedPower(powerBufferPoolStruct* pbs,int16_t input);
int16_t OutMapCurrent(int16_t coe,int16_t input);
	int16_t CurrentMapOut(int16_t coe,int16_t current);
#endif	// __POWER_BUFFER_POOL_H
/*-----------------------------------file of end------------------------------*/


