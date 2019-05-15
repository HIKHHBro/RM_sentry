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
//#include "SEGGER_SYSVIEW_FreeRTOS.h"
//#define INCLUDE_xTaskGetIdleTaskHandle  1
//#define INCLUDE_pxTaskGetStackStart     1
/* -------------- 软件定时器 ----------------- */
//是否编译定时器相关代码， 如需要使用定时器， 设置为 1 
#define configUSE_TIMERS 1
//设置定时器Daemon 任务优先级， 如果优先级太低， 可能导致定时器无法及时执行 
#define configTIMER_TASK_PRIORITY  (2)
//设置定时器Daemon 任务的命令队列深度， 设置定时器都是通过发送消息到该队列实现的。 
#define configTIMER_QUEUE_LENGTH   10
//设置定时器Daemon 任务的栈大小
#define configTIMER_TASK_STACK_DEPTH  256
	/* ----------------- 任务堆栈大小设置 -------------------- */
   #define SYS_INIT_HEAP_SIZE    512//系统初始任务
   #define PARSE_HEAP_SIZE       1024//数据解析任务
   #define CHASSIS_HEAP_SIZE     1024//底盘任务
   #define GIMBAL_HEAP_SIZE      1024//云台任务
   #define LED_TIP_HEAP_SIZE     0//led提示任务
   #define SYS_DETEC_HEAP_SIZE   1024//系统自检和数据校准任务
//   #define DEBUG_HEAP_SIZE       1024//硬件仿真任务提示任务
	 #define RAMMER_HEAP_SIZE      512//拨弹电机任务
	 #define TX_HEAP_SIZE          512//用户发送任务
   #define QUEUE_HEAP_SIZE       512//队列
	/* -------------- 队列堆栈大小设置 ----------------- */   
	/* ----------------- 任务内存大小设置-------------------- */
   #define TOTAL_HEAP_SIZE 								\
	         ((size_t)((										\
						 SYS_INIT_HEAP_SIZE + 				\
					   PARSE_HEAP_SIZE +						\
					   CHASSIS_HEAP_SIZE +					\
						 GIMBAL_HEAP_SIZE +						\
						 LED_TIP_HEAP_SIZE + 					\
						 SYS_DETEC_HEAP_SIZE+					\
						 RAMMER_HEAP_SIZE +          \
						 TX_HEAP_SIZE +               \
             QUEUE_HEAP_SIZE +             \
						 configTIMER_TASK_STACK_DEPTH   \
																			 ) * 7))
                                       
                                       //	 DEBUG_HEAP_SIZE +
		

/* -------------- 重定义freertos任务内存大小 ----------------- */
  #ifdef configTOTAL_HEAP_SIZE
    #undef configTOTAL_HEAP_SIZE
  #endif
  #ifndef configTOTAL_HEAP_SIZE
    #define configTOTAL_HEAP_SIZE   TOTAL_HEAP_SIZE//重新定义总任务内存大小，防止cubemx生成之后被修改
  #endif
#endif	// __USERFREERTOSCONFIG_H
/*-----------------------------------file of end------------------------------*/


