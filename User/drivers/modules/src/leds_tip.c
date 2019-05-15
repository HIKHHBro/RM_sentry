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
#if defined (LED_GPIO)
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
		#if RM_OLD_BOARD
    	UNUSED(GPIO);
		#elif (BINGE_BOARD|RM_NEW_BOARD)
			uint8_t i = 0;
			for (i = 0; i < times*2;i++)
			{
					HAL_GPIO_TogglePin(GPIO, ledx);
          osDelay(lag);
			}
  	HAL_GPIO_WritePin(GPIO,ledx,GPIO_PIN_SET);
		#endif
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
	/**
	* @Data    2019-01-18 14:25
	* @brief   进度条等效
	* @param   GPIO LED_GPIO (详细说明见baseclass.h文件)
	* @param   lag  闪烁时间间隔 单位ms
	* @retval  void
	*/
	void ProgressBarLed(GPIO_TypeDef* GPIO,uint32_t lag)
	{
		#if RM_OLD_BOARD
		  UNUSED(GPIO);
		#elif (BINGE_BOARD|RM_NEW_BOARD)
		uint8_t i = 0;
		uint16_t tem_pin=0;
		for (i = 0; i < LED_TOTAL; i++)
		{
			tem_pin |= 1 << (i+LED_ORIGIN_PIN);
			HAL_GPIO_WritePin(GPIO, tem_pin, GPIO_PIN_RESET);
			osDelay(lag);
		}
		HAL_GPIO_WritePin(GPIO, tem_pin, GPIO_PIN_SET);
	#endif
	}
  /**
  * @Data    2019-03-16 01:26
  * @brief   流水往返灯效
  * @param   void 
  * @retval  void
  */
//   void WaterfallLight(uint32_t times,uint8_t speed)
//   {
// #if RM_OLD_BOARD
// 		  UNUSED(GPIO);
// #elif (BINGE_BOARD|RM_NEW_BOARD)
// 		uint8_t i = 0;
// 		uint32_t tem_pin=0;
//     uint16_t tem_pin1;
//     uint16_t x ;
   
//     for( x= (uint16_t)(times/speed);x<1;x--)
//     {
// 	  	for (i = 0; i < LED_TOTAL; i++)
//       {
//         tem_pin = 7 << (i+LED_ORIGIN_PIN+7);
//         tem_pin1 = (uint16_t)(tem_pin >>8);
//         HAL_GPIO_WritePin(LED_GPIO, tem_pin1, GPIO_PIN_RESET);
//         osDelay(speed);
//       }
//     }

// 		HAL_GPIO_WritePin(LED_GPIO, tem_pin, GPIO_PIN_SET);
// #endif
//   }
  /**
  * @Data    2019-03-16 02:07
  * @brief   闪烁警告灯
  * @param   void
  * @retval  void
  */
  void WarningLed(uint8_t times,uint16_t lag)
  {
    uint16_t temp;
    temp = (LED_1|LED_2 |LED_3 |LED_4 |LED_5 |LED_6 |LED_7);
    FlashingLed(LED_GPIO,temp,times,lag);
  }
    /**
    * @Data    2019-03-16 02:24
    * @brief   递增灯效
    * @param   void
    * @retval  void
    */
    void SetpUpLED(void)
    {
      uint8_t i;
      HAL_GPIO_WritePin(LED_GPIO, i, GPIO_PIN_RESET);
    }
#endif
/*------------------------------------file of end-------------------------------*/


