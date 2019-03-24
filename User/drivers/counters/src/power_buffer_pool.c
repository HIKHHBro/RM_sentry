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
	|--FileName    : power_buffer_pool.c                                              
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
#include "power_buffer_pool.h"
/* 
	* @Data    2019-03-24 19:42
	* @brief   ���ʻ����
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
	* @brief   pid�����ʼֵӳ�����
	* @param   void
	* @retval  void ��λmA
	*/
int16_t OutMapCurrent(int16_t coe,int16_t input)
{
	//-16384 ~ 0 ~ 16384,3508
	return (int16_t)(coe*input);
}
/**
	* @Data    2019-03-24 21:14
	* @brief   ����ӳ��pid���ֵ
	* @param   void
	* @retval  void
	*/
	int16_t CurrentMapOut(int16_t coe,int16_t current)
	{
		return (int16_t)(current / (float)(coe));
	}
/**
	* @Data    2019-03-24 20:18
	* @brief   ����س�ˮ�� //������
	* @param   input �����
	* @retval  void  
	*/
float time_coe=500;//��λms
int16_t WaterOutlet(powerBufferPoolStruct* pbs,int16_t input)
{
	int16_t cur;//��λmN
	
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
// 	* @brief   ���ʼ���
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
// 	* @brief   ���㷧ֵ����
// 	* @param   void
// 	* @retval  void
// 	*/
// 	void name(void)
// 	{
		
// 	}
/*-----------------------------------file of end------------------------------*/


