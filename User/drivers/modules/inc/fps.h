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
	|--FileName    : fps.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-04-23               
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
#ifndef __FPS_H 
#define __FPS_H 
#include "baseclass.h " 
 #if defined(_FPS_)
  typedef uint32_t fps_t[2];
#define FPS  0//帧率
#define FPS_ADDR  1//帧率地址
	MOD_Status SetFpsAddress(uint32_t fps[]);
  	void FpsUserTimeInit(void);
void Fps(uint32_t fps[]);
static inline uint32_t GetTimeMs(void)
{
  return HAL_GetTick();
}
static inline uint32_t GetFps(uint32_t fps[])
{
  return fps[FPS];
}
  #endif	// __FPS_H
#endif	// __FPS_H
/*-----------------------------------file of end------------------------------*/


