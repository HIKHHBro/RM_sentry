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
#ifdef ENCOER_TIM
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
	HAL_StatusTypeDef EnconderInit(incrementalEnconderStruct* ies,uint16_t radius, int16_t poles)
	{
		float temp1,tem2p;
		temp1 = (float)(poles * 4);
	  tem2p = (float)(radius * 2 * 3.14F);
		ies->coefficient = tem2p/temp1;
    ies->counter = 0;
    ies->last_data = 0;
		__HAL_TIM_SET_AUTORELOAD(ENCOER_TIM,(USE_ARR-1));
	/* ------   编码器初始化及使能编码器模式   ------- */
    HAL_TIM_Encoder_Start(ENCOER_TIM, TIM_CHANNEL_ALL);
		return HAL_OK;
	}
	/**
		* @Data    2019-03-14 19:09
		* @brief   获取当前位置// 待增加过零之后变负数，增加冗余，要不过零点在返回会改变零点
		* @param   incrementalEnconderStruct* ies
		* @retval  uint32_t 当前位置，单位mm
		*/
		int32_t GetPosition(incrementalEnconderStruct* ies)
		{
			uint32_t temp =0;
			temp = ENCOER_TIM->Instance->CNT;//(__HAL_TIM_GET_COUNTER(ies->htim));
			if(((int32_t)(temp - ies->last_data)) < -THRESHILD)
			{
        ies->counter++;
				if(ies->counter > 254)
				{
					ies->counter = 0;
					__HAL_TIM_SET_COUNTER(ENCOER_TIM,0);
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
					__HAL_TIM_SET_COUNTER(ENCOER_TIM,0);
          ies->last_data = 0;
					return 0 ;
				}
			}
      ies->last_data = temp;
   return ((temp+(USE_ARR*ies->counter)) * ies->coefficient);
		}
    int16_t exti_z =0;
    int32_t last_temp_h = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
   case ENCODE_Z:
    {
      if((ENCOER_TIM->Instance->CNT -last_temp_h) > 0)
      exti_z++;
      else exti_z--;
      last_temp_h = ENCOER_TIM->Instance->CNT;
    }break;
  }
}
 /**
	* @Data    2019-03-30 22:13
	* @brief   编码器校准
	* @param   int32_t distanct 校准位置距离
	* @retval  void
	*/ 
MOD_Status CalibratingEncoder(incrementalEnconderStruct* ies,int32_t distanct)
{
  int32_t temp_dis,tem_int_a;
  if(distanct <0)
    return MOD_ERROR;
  if(distanct == 0)
  {
    ies->counter =0;
    __HAL_TIM_SET_COUNTER(ENCOER_TIM,0);
  }
  else 
  {
       temp_dis =  (int32_t)((float)distanct / ies->coefficient);
   tem_int_a = (int32_t)temp_dis/0xFFFF;
    ies->counter = tem_int_a;
  __HAL_TIM_SET_COUNTER(ENCOER_TIM,(uint32_t)(temp_dis%0xFFFF));
  }
  return MOD_OK;
}
#endif  
/*-----------------------------------file of end------------------------------*/


