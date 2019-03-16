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
	|--FileName    : ultrasonic.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-03-15             
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
#include "ultrasonic.h" 
/* ===========================  HCSR-04 of begin =========================== */
uint32_t 	Channel3HighTime, Channel4HighTime;
uint32_t Channel3Period, Channel4Period;
uint8_t   Channel3Edge = 0, Channel4Edge = 0;
uint32_t  Channel3Percent, Channel4Percent;
uint32_t 	Channel3RisingTimeLast=0, Channel3RisingTimeNow, Channel3FallingTime;
uint32_t 	Channel4RisingTimeLast=0, Channel4RisingTimeNow, Channel4FallingTime;
/**
	* @Data    2019-03-16 10:28
	* @brief   HCSR 04初始化
	* @param   void
	* @retval  void
	*/
	void  HCSR04Init(void)
	{
			HAL_TIM_IC_Start_IT(HCSR04_TIM, TIM_CHANNEL_3);
    	HAL_TIM_IC_Start_IT(HCSR04_TIM, TIM_CHANNEL_4);
	}

	void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
	{
		
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if(Channel3Edge == 0)
		{
			Channel3RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_3);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
			Channel3Edge = 1;
			if(Channel3RisingTimeLast == 0)
			{
				Channel3Period = 0;
			}
			else
			{
				if(Channel3RisingTimeNow > Channel3RisingTimeLast)
				{
					Channel3Period = Channel3RisingTimeNow - Channel3RisingTimeLast;
				}
				else
				{
					Channel3Period = Channel3RisingTimeNow + 0xffff - Channel3RisingTimeLast + 1;
				}
			}
			Channel3RisingTimeLast = Channel3RisingTimeNow;
		}
		else if(Channel3Edge == 1)
		{
			Channel3FallingTime = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_3);	
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
			
			if(Channel3FallingTime < Channel3RisingTimeNow)
			{
				Channel3HighTime = Channel3FallingTime + 0xffff - Channel3RisingTimeNow + 1;
			}
			else
			{
				Channel3HighTime = Channel3FallingTime - Channel3RisingTimeNow;
			}
			if(Channel3Period != 0)
			{
				Channel3Percent = Channel3HighTime;
				// Channel3Percent = (uint32_t )(float)(Channel3HighTime);
				// Channel3Percent = (uint8_t)(((float)Channel3HighTime / Channel3Period) * 1000);
			}
			Channel3Edge = 0;
		}
	}
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if(Channel4Edge == 0)
		{
			Channel4RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_4);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
			Channel4Edge = 1;
			if(Channel4RisingTimeLast == 0)
			{
				Channel4Period = 0;
			}
			else
			{
				if(Channel4RisingTimeNow > Channel4RisingTimeLast)
				{
					Channel4Period = Channel4RisingTimeNow - Channel4RisingTimeLast;
				}
				else
				{
					Channel4Period = Channel4RisingTimeNow + 0xffff - Channel4RisingTimeLast + 1;
				}
			}
			Channel4RisingTimeLast = Channel4RisingTimeNow;
		}
		else if(Channel4Edge == 1)
		{
			Channel4FallingTime = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_4);	
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
			
			if(Channel4FallingTime < Channel4RisingTimeNow)
			{
				Channel4HighTime = Channel4FallingTime + 0xffff - Channel4RisingTimeNow + 1;
			}
			else
			{
				Channel4HighTime = Channel4FallingTime - Channel4RisingTimeNow;
			}
			if(Channel4Period != 0)
			{
        Channel4Percent = Channel4HighTime;
				// Channel4Percent = 
				// Channel4Percent = (uint8_t)(((float)Channel4HighTime / Channel4Period) * 1000);
			}
			Channel4Edge = 0;
		}
	}
	}
/**
  * @Data    2019-03-16 10:35
  * @brief   获取距离
  * @param   void
  * @retval  void
  */
  uint32_t GetDistance(uint8_t Channel)
  {
    static uint32_t lastdata3;
    static uint32_t lastdata4;
    static uint8_t i=0;
    uint32_t tem;
    int32_t tem1;
    if(i ==0)
    {
      lastdata3 = (uint32_t)(Channel3Percent*0.017);
      lastdata4 = (uint32_t)(Channel4Percent*0.017);
    }
       i=1;
    if(Channel ==3)
    {
      tem =(uint32_t)(Channel3Percent*0.017);
      tem1 = tem - lastdata3;
      if((tem1 <60) || (tem1> -60))
      lastdata3 = tem;
       return lastdata3;
    }
    if(Channel ==4)
    {
       tem =(uint32_t)(Channel4Percent*0.017);
       tem1 = tem - lastdata4;
     if((tem1 <60) || (tem1> -60))
      lastdata4 = tem;
       return lastdata4;
    }
    return 0;
  }
/**
	* @Data    2019-03-16 14:11
	* @brief   HCSR04数据接收
	* @param   void
	* @retval  void
	*/
	void HCSR04RxMsg(void)
	{
       HAL_GPIO_TogglePin(SONIC_GPIO,SONIC);
	}
/* ===========================  HCSR-04 of end =========================== */	
/*-----------------------------------file of end------------------------------*/


