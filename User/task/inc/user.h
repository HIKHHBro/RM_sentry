/**
	|----------------------------- Copyright ------------------------------------|
	|                                                                            |
	|                        (C) Copyright 2018,海康平头哥,                       |
	|         1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China        |
	|                         All Rights Reserved                                |
	|                                                                            |
	|        By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)            |
	|                  https://github.com/GCUWildwolfteam                        |
	|----------------------------------------------------------------------------|
	|--FileName    : user.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-11-27               
	|--Description : 此头文件放在cube生成的math.c
  |                stm32F4xx_it.c的用户代码区                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|----------------------------declaration of end------------------------------|
 **/
#ifndef __USER_H 
#define __USER_H
#include "sys_task.h"
#ifdef DEBUG_BY_KEIL //keil调试初始化
			#include "debug_by_keil.h" 
#endif

void SysInit(void);
#endif // __USER_H

/*--------------------------------file of end---------------------------------*/


