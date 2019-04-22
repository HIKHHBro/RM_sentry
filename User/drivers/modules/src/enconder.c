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
	|--FileName    : enconder.c                                              
	|--Version     : v1.0                                                          
	|--Author      : ����ƽͷ��                                                     
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
#define USE_ARR   0xFFFF//��װֵ
#define THRESHILD 50000//((uint32_t)(USE_ARR*0.8))//Խ�緧ֵ
/**
	* @Data    2019-03-14 16:02
	* @brief   �������Ķ�ʱ��ʹ�ܺͳ�ʼ��
	* @param   TIM_HandleTypeDef* htim 
	* @param   int16_t poles ����������  ��λP/R
	* @param   int16_t radius װ�ڱ������ϵ����ӵİ뾶 ��λmm
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
	/* ------   ��������ʼ����ʹ�ܱ�����ģʽ   ------- */
    HAL_TIM_Encoder_Start(ENCOER_TIM, TIM_CHANNEL_ALL);
		return HAL_OK;
	}
	/**
		* @Data    2019-03-14 19:09
		* @brief   ��ȡ��ǰλ��// �����ӹ���֮��为�����������࣬Ҫ��������ڷ��ػ�ı����
		* @param   incrementalEnconderStruct* ies
		* @retval  uint32_t ��ǰλ�ã���λmm
		*/
		uint32_t GetPosition(incrementalEnconderStruct* ies)
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
   return (uint32_t)((temp+(USE_ARR*ies->counter)) * ies->coefficient);
		}
#endif  
/*-----------------------------------file of end------------------------------*/


