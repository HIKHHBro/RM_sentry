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
	|--FileName    : leds_tip.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-18               
	|--Libsupports : STM32CubeF4 Firmware Package V1.6.0(用别的库出问题别问我)
	|--Description : 修改引脚的宏,不同板的引脚不一样
	|--							 1、RM_NEW_BOARD
	|--							 2、RM_OLD_BOARD                                                      
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end----------------------------|
 **/
#ifndef __LEDS_TIP_H 
#define __LEDS_TIP_H 
#include "baseclass.h"
/* ----------------- 开发板的选择 -------------------- */
#define RM_NEW_BOARD 1
#define RM_OLD_BOARD 0
/* ----------------- led引脚宏定义定义 -------------------- */
#if RM_NEW_BOARD //新板引脚宏定义
	#define LED_1 GPIO_PIN_1
	#define LED_2 GPIO_PIN_2
	#define LED_3 GPIO_PIN_3
	#define LED_4 GPIO_PIN_4
	#define LED_5 GPIO_PIN_5
	#define LED_6 GPIO_PIN_6
	#define LED_7 GPIO_PIN_7
	#define LED_8 GPIO_PIN_8
	#define LED_GPIO GPIOG
#elif RM_OLD_BOARD //旧板引脚宏定义
	#define LED_1 GPIO_PIN_1
	#define LED_2 GPIO_PIN_2
	#define LED_3 GPIO_PIN_3
	#define LED_4 GPIO_PIN_4
	#define LED_5 GPIO_PIN_5
	#define LED_6 GPIO_PIN_6
	#define LED_7 GPIO_PIN_7
	#define LED_8 GPIO_PIN_8
#endif
	void FlashingLed(GPIO_TypeDef *GPIO, uint16_t ledx, uint8_t times, uint32_t lag);
	void ProgressBarLed(GPIO_TypeDef *GPIO, uint32_t lag);
#define LED_TOTAL 8 //led等个数
#endif // __LEDS_TIP_H

/*------------------------------------file of end-------------------------------*/


