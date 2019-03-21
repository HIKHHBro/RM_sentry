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
	|--FileName    : userFreeRTOSConfig.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-03-16               
	|--Libsupports : 
	|--Description : 配置FreeRTOS接口
  |               用户FreeRTOS设置，此头文件放在FreeRTOSConfig.h最下面的用户代码段                                                      
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-----------------------------declaration of end-----------------------------|
 **/
#ifndef __USERFREERTOSCONFIG_H 
#define __USERFREERTOSCONFIG_H 
	/* ----------------- 任务堆栈大小设置 -------------------- */
   #define SYS_INIT_HEAP_SIZE    512//系统初始任务
   #define PARSE_HEAP_SIZE       1024//数据解析任务
   #define CHASSIS_HEAP_SIZE     512//底盘任务
   #define GIMBAL_HEAP_SIZE      512//云台任务
   #define LED_TIP_HEAP_SIZE     512//led提示任务
   #define SYS_DETEC_HEAP_SIZE   1024//系统自检和数据校准任务
   #define DEBUG_HEAP_SIZE       1024//硬件仿真任务提示任务
	 #define RAMMER_HEAP_SIZE      512//拨弹电机任务
	 #define TX_HEAP_SIZE          512//用户发送任务
	/* -------------- 队列堆栈大小设置 ----------------- */   
	/* ----------------- 任务内存大小设置-------------------- */
   #define TOTAL_HEAP_SIZE 								\
	         ((size_t)((										\
						 SYS_INIT_HEAP_SIZE + 				\
					   PARSE_HEAP_SIZE +						\
					   CHASSIS_HEAP_SIZE +					\
						 GIMBAL_HEAP_SIZE +						\
						 LED_TIP_HEAP_SIZE + 					\
						 DEBUG_HEAP_SIZE + 						\
						 SYS_DETEC_HEAP_SIZE+					\
						 RAMMER_HEAP_SIZE +          \
						 TX_HEAP_SIZE                \
																			 ) * 7))
		

/* -------------- 重定义freertos任务内存大小 ----------------- */
  #ifdef configTOTAL_HEAP_SIZE
    #undef configTOTAL_HEAP_SIZE
  #endif
  #ifndef configTOTAL_HEAP_SIZE
    #define configTOTAL_HEAP_SIZE   TOTAL_HEAP_SIZE//重新定义总任务内存大小，防止cubemx生成之后被修改
  #endif
#endif	// __USERFREERTOSCONFIG_H
/*-----------------------------------file of end------------------------------*/


