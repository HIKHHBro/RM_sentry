/**
	|-------------------------------- Copyright -----------------------------------|
	|                                                                              |
	|                        (C) Copyright 2019,海康平头哥,                         |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                               |
	|                                                                              |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                     https://github.com/GCUWildwolfteam                       |
	|------------------------------------------------------------------------------|
	|--FileName    : leds_tip.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-18               
	|--Libsupports : STM32CubeF1 Firmware Package V1.6.0(用别的库出问题别问我)
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end----------------------------|
 **/
#include "leds_tip.h" 
	/*---------------------------------80字符限制-----------------------------------*/
		/**
		* @Data    2019-01-18 11:38
		* @brief   闪烁灯
		* @param   GPIO LED_GPIO (详细说明见.h文件)
		* @param   ledx led引脚宏定义   (详细说明见.h文件)
		* @param   times 闪烁次数
		* @param   lag  闪烁时间间隔 单位ms
		* @retval  void
		*/
		void FlashingLed(GPIO_TypeDef* GPIO,uint16_t ledx,uint8_t times,uint32_t lag)
		{
			uint8_t i = 0;
			for (i = 0; i < times;i++)
			{
					HAL_GPIO_TogglePin(GPIO, ledx);
          osDelay(lag);
					HAL_GPIO_WritePin(GPIO,ledx,GPIO_PIN_SET);
			}
		}
	/*---------------------------------80字符限制-----------------------------------*/
		/**
		* @Data    2019-01-18 14:16
		* @brief   跳动LED灯效 (适合LED引脚相连的板子)
		* @param   GPIO LED_GPIO (详细说明见.h文件)
		* @param 	 speed 跳动最大速度
		* @retval  void
		*/
		// void PulsatileLed(GPIO_TypeDef* GPIO,uint8_t speed)
		// {
		// 	uint8_t pulsatiles = 8;//跳跃LED个数
		// 	uint8_t i = 0;
		// 	for (i = 0; i < pulsatiles;i++)
		// 	{

		// 	}
		// }
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-18 14:25
	* @brief   进度条等效
	* @param   GPIO LED_GPIO (详细说明见.h文件)
	* @param   lag  闪烁时间间隔 单位ms
	* @retval  void
	*/
	void ProgressBarLed(GPIO_TypeDef* GPIO,uint32_t lag)
	{
		uint8_t i = 0;
		uint16_t tem_pin=0;
		for (i = 0; i < LED_TOTAL; i++)
		{
			tem_pin |= 1 << (i+LED_ORIGIN_PIN);
			HAL_GPIO_WritePin(GPIO, tem_pin, GPIO_PIN_RESET);
			osDelay(lag);
		}
		HAL_GPIO_WritePin(GPIO, tem_pin, GPIO_PIN_SET);
	}
/*------------------------------------file of end-------------------------------*/


