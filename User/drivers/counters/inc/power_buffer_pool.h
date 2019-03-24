/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,����ƽͷ��,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : power_buffer_pool.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
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
	float max_w;//���ʵ�λmW
	float r_w;//���ʵ�λmW
	float current_mapp_coe;//����ӳ��ϵ��
	float high_water_level;
	float low_water_level;
	float mid_water_level;
	float period;//�������ڣ���λ/s
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


