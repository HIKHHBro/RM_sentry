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
  |--FileName    : pc_data.h                                                
  |--Version     : v1.0                                                            
  |--Author      : 海康平头哥                                                       
  |--Date        : 2019-03-21               
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
#ifndef __PC_DATA_H 
#define __PC_DATA_H 
#include "bsp_usart.h"
#include "DataStructure.h" 
typedef struct pcDataStruct
{
  uint32_t status;
  int16_t yaw_target_angle;
  int16_t pitch_target_angle;
  uint8_t commot;
  uint8_t shoot_commot;
  int16_t fps;
  int16_t distance;//单位cm
}pcDataStruct;
     void Pc_ParseData(pcDataStruct* pc);
     HAL_StatusTypeDef PcDataRxInit(pcDataStruct* pc);

int16_t YawDataConversion(int16_t yaw);
int16_t PitchDataConversion(int16_t pitch);
#endif	// __PC_DATA_H
  
 /*------------------------------------file of end-------------------------------*/


