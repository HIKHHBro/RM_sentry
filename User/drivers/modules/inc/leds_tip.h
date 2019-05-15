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
#include "baseclass.h "
#define TIP_BASE_TIME    10//时间片10ms
	void FlashingLed(GPIO_TypeDef *GPIO, uint16_t ledx, uint8_t times, uint32_t lag);
	void ProgressBarLed(GPIO_TypeDef *GPIO, uint32_t lag);
   void WarningLed(uint8_t times,uint16_t lag);
#endif // __LEDS_TIP_H

/*------------------------------------file of end-------------------------------*/


