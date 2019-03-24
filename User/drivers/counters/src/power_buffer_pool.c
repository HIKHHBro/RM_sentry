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
	|--FileName    : power_buffer_pool.c                                              
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
#include "power_buffer_pool.h"
/* 
	* @Data    2019-03-24 19:42
	* @brief   功率缓冲池
	* @param   void
	* @retval  void
	*/
	int16_t PowerBufferPool(powerBufferPoolStruct* pbs,int16_t input)
	{
		 if(pbs->r_w > pbs->high_water_level)
		 {
			 	
		  	pbs->buf_t++;
		 }

	}
/**
	* @Data    2019-03-24 19:59
	* @brief   pid计算初始值映射电流
	* @param   void
	* @retval  void 单位mA
	*/
int16_t OutMapCurrent(int16_t coe,int16_t input)
{
	//-16384 ~ 0 ~ 16384,3508
	return (int16_t)(coe*input);
}
/**
	* @Data    2019-03-24 21:14
	* @brief   电流映射pid输出值
	* @param   void
	* @retval  void
	*/
	int16_t CurrentMapOut(int16_t coe,int16_t current)
	{
		return (int16_t)(current / (float)(coe));
	}
/**
	* @Data    2019-03-24 20:18
	* @brief   缓存池出水口 //待完善
	* @param   input 输出量
	* @retval  void  
	*/
float time_coe=500;//单位ms
int16_t WaterOutlet(powerBufferPoolStruct* pbs,int16_t input)
{
	int16_t cur;//单位mN
	
	if(pbs->r_w > pbs->high_water_level)
	{
		cur = CurrentMapping(pbs->current_mapp_coe,input);
		if(cur > pbs->current_threshold)
		{
			return CurrentMapOut(pbs->current_mapp_coe,pbs->current_threshold);
		}
		else return input;
	}
	if(pbs->r_w < pbs->low_water_level)
	{

	}
}
// /**
// 	* @Data    2019-03-24 20:20
// 	* @brief   功率计算
// 	* @param   void
// 	* @retval  void 
// 	*/
// 	void CalculatedPower(powerBufferPoolStruct* pbs,int16_t input)
// 	{
// 		int16_t p;
// 		p = CurrentMapping(pbs->current_mapp_coe,input);
// 		return (int16_t)( p * pbs->period);
// 	}
// /**
// 	* @Data    2019-03-24 21:04
// 	* @brief   计算阀值电流
// 	* @param   void
// 	* @retval  void
// 	*/
// 	void name(void)
// 	{
		
// 	}
/*-----------------------------------file of end------------------------------*/


