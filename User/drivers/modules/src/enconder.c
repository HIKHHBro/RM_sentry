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
	|--FileName    : enconder.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-03-14             
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
#include "enconder.h" 
#define USE_ARR   0xFFFF//重装值
#define THRESHILD 50000//((uint32_t)(USE_ARR*0.8))//越界阀值
/**
	* @Data    2019-03-14 16:02
	* @brief   编码器的定时器使能和初始化
	* @param   TIM_HandleTypeDef* htim 
	* @param   int16_t poles 编码器线数  单位P/R
	* @param   int16_t radius 装在编码器上的轮子的半径 单位mm
	* @retval  void
	*/
	HAL_StatusTypeDef EnconderInit(incrementalEnconderStruct* ies, \
											TIM_HandleTypeDef* htim, uint16_t radius, int16_t poles)
	{
		if(htim==NULL)
		return HAL_ERROR;
		float temp1,tem2p;
		temp1 = (float)(poles * 4);
	  tem2p = (float)(radius * 2 * 3.14F);
		ies->coefficient = tem2p/temp1;
    ies->counter = 0;
    ies->htim = htim;
    ies->last_data = 0;
		__HAL_TIM_SET_AUTORELOAD(htim,(USE_ARR-1));
	/* ------   编码器初始化及使能编码器模式   ------- */
    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
		return HAL_OK;
	}
	/**
		* @Data    2019-03-14 19:09
		* @brief   获取当前位置
		* @param   incrementalEnconderStruct* ies
		* @retval  uint32_t 当前位置，单位mm
		*/
		uint32_t GetPosition(incrementalEnconderStruct* ies)
		{
			uint32_t temp =0;
			temp = ies->htim->Instance->CNT;//(__HAL_TIM_GET_COUNTER(ies->htim));
			if(((int32_t)(temp - ies->last_data)) < -THRESHILD)
			{
        ies->counter++;
				if(ies->counter > 254)
				{
					ies->counter = 0;
					__HAL_TIM_SET_COUNTER(ies->htim,0);
          ies->last_data = 0;
					return 0;
				}
			}
			else if(((int32_t)(temp - ies->last_data)) > THRESHILD)
			{
        	ies->counter--;
				if(ies->counter <0)
				{
					ies->counter =0;
					__HAL_TIM_SET_COUNTER(ies->htim,0);
          ies->last_data = 0;
					return 0 ;
				}
			}
      ies->last_data = temp;
   return (uint32_t)((temp+(USE_ARR*ies->counter)) * ies->coefficient);
		}
/*-----------------------------------file of end------------------------------*/


