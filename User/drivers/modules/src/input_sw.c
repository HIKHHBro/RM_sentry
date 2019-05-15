/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2018,海康平头哥,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : "input_sw.c                                             
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2018-11-27             
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
#include "input_sw.h" 
 /**
	* @Data    2019-04-16 10:17
	* @brief  限位开关读取
	* @param   void
	* @retval  void
	*/
  int32_t lim_sw_flag=0;
uint8_t LimSw(void)
{
  uint8_t sw_left=0,sw_right=0,sw;
  sw_right = HAL_GPIO_ReadPin(LIM_SWITCH_GPIO,LIM_SW_RIGHT);
  sw_left = HAL_GPIO_ReadPin(LIM_SWITCH_GPIO,LIM_SW_LEFT);
  sw = (uint8_t)(sw_left <<4) | (sw_right);
  if(sw ==0x11)
    HAL_GPIO_WritePin(LED_GPIO,LED_7,GPIO_PIN_RESET);
  if(sw !=0x11)
  {
    lim_sw_flag++;
    if(lim_sw_flag >0xFFFF-2)
    {
      lim_sw_flag =0xFFFF-2;
    }
  }
  else 
  {
    if(lim_sw_flag<0)
      lim_sw_flag =0;
   lim_sw_flag = lim_sw_flag-4;
  }
  if(lim_sw_flag < LIM_SW_TIME_OF_DURATION)
  {
    return 0;
  }
  else return sw;
}

 /**
	* @Data    2019-04-16 10:17
	* @brief  激光右开关读取
	* @param   void
	* @retval  void
	*/
  int32_t laser_sw_r_flag=0;
 	uint8_t LaserSwBack(void)
  {
     uint8_t sw_right;
     sw_right = HAL_GPIO_ReadPin(LASER_SWITCH_GPIO,LASER_SWITCH_BACK);
    if(sw_right ==1)
    {
      laser_sw_r_flag++;
    }
    else 
    {
      if(laser_sw_r_flag<0)
        laser_sw_r_flag =0;
      laser_sw_r_flag =laser_sw_r_flag-10;
    }
    if(laser_sw_r_flag < LASER_SW_TIME_OF_DURATION)
    {
      return 0;
    }
    else return 1;
  }
 /**
	* @Data    2019-04-16 10:17
	* @brief  激光左开关读取
	* @param   void
	* @retval  void
	*/
 int32_t laser_sw_l_flag=0;
 	uint8_t LaserSwBef(void)
  {
    uint8_t sw_left=0;
    sw_left = HAL_GPIO_ReadPin(LASER_SWITCH_GPIO,LASER_SWITCH_BEF);
   if(sw_left ==0)
    {
      laser_sw_l_flag++;
    }
    else 
    {
      if(laser_sw_l_flag<0)
        laser_sw_l_flag =0;
      laser_sw_l_flag = laser_sw_l_flag -10;
    }
    if(laser_sw_l_flag < LASER_SW_TIME_OF_DURATION)
    {
      return 0;
    }
    else return 1;
  }

