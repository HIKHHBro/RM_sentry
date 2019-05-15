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
	|--FileName    : fps.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-04-23             
	|--Libsupports : 
	|--Description :  1.#define _FPS_ 开启帧率统计
  |--               2.在系统初始化时调用FpsUserTimeInit()开启帧率软定时，使能帧率统计器   
  |--               3.定义fps_t类型 在需要统计帧率模块初始化时调用SetFpsAddress(); 传入帧率类型地址
  |--               4.在接收函数调用Fps();统计帧率
	|--FunctionList                                                     
	|-------1. ....                                                     
	|          <version>:                                                     
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-----------------------------declaration of end-----------------------------|
 **/
#include "fps.h" 
#if defined(_FPS_)
#define FPS_POOL_SIZE 30
#define AR_SIZE      2
#define FPS_POOL     0
#define COUNTER_POOL 1
//static uint16_t used_counter =0;
 /* -------------- 软件定时器 ----------------- */       
osTimerId fpsTimerHandle;
void FpsCallback(void const * argument);
static uint32_t *s_fps_addr[FPS_POOL_SIZE];//
static uint32_t s_counters[FPS_POOL_SIZE];
#define OFFLINE_MAX_TIME   200 //掉线最大时间0.5s
#define FPS_BASE_TIME_CEO   (1000/OFFLINE_MAX_TIME)  //帧率时间系数 
/**
	* @Data    2019-04-23 06:00
	* @brief   帧率软定时开启
	* @param   void
	* @retval  void
	*/
	void FpsUserTimeInit(void)
	{
		 osStatus timerresult = osOK;
    osTimerDef(FpsTimer, FpsCallback);
    fpsTimerHandle = osTimerCreate(osTimer(FpsTimer), osTimerPeriodic, NULL);
    timerresult = osTimerStart(fpsTimerHandle,OFFLINE_MAX_TIME);
    if(timerresult !=osOK)
    {
      //添加报错机制
    }
	}
/**
	* @Data    2019-04-23 05:50
	* @brief  帧率统计 
	* @param   void
	* @retval  void
	*/
void Fps(uint32_t fps[])
{
   s_counters[fps[FPS_ADDR]]++;
}
/**
	* @Data    2019-04-23 06:10
	* @brief   传入帧数器地址
	* @param   void
	* @retval  void
	*/
	MOD_Status SetFpsAddress(uint32_t fps[])
	{
     for(uint8_t i=0;i<FPS_POOL_SIZE;i++)
    {
      if( s_fps_addr[i]==NULL)
      {
        s_fps_addr[i] = &fps[FPS];
         fps[FPS_ADDR] = i; 
        return MOD_OK;
      }
    }
     return MOD_ERROR;
	}

/**
	* @Data    2019-04-23 05:56
	* @brief   帧数软件定时器回调函数
	* @param   void
	* @retval  void
	*/
void FpsCallback(void const * argument)
{
  for(uint8_t i=0;i<FPS_POOL_SIZE;i++)
  {
    if(s_fps_addr[i] !=NULL)
    {
      *s_fps_addr[i]= (uint32_t)(s_counters[i] * FPS_BASE_TIME_CEO);
      s_counters[i] = 0;
    }
  }
}

#endif
/*-----------------------------------file of end------------------------------*/


