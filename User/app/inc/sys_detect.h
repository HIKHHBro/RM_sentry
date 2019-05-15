/**
  |-------------------------------- Copyright -----------------------------------|
  |                                                                              |
  |                        (C) Copyright 2019,����ƽͷ��,                         |
  |            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
  |                            All Rights Reserved                               |
  |                                                                              |
  |            By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
  |                     https://github.com/GCUWildwolfteam                       |
  |------------------------------------------------------------------------------|
  |--FileName    : sys_detect.h                                                
  |--Version     : v1.0                                                            
  |--Author      : ����ƽͷ��                                                       
  |--Date        : 2019-03-13               
  |--Libsupports : STM32CubeF1 Firmware Package V1.6.0(�ñ�Ŀ�����������)
  |--Description : ϵͳ�Լ죬����У׼                                                     
  |--FunctionList                                                       
  |-------1. ....                                                       
  |          <version>:                                                       
  |     <modify staff>:                                                       
  |             <data>:                                                       
  |      <description>:                                                        
  |-------2. ...                                                       
  |---------------------------------declaration of end----------------------------|
 **/
#ifndef __SYS_DETECT_H 
#define __SYS_DETECT_H 
#include "parse.h" 
#include "leds_tip.h" 
#include "user_tx.h" 
typedef struct sysDetectStruct 
{
  chassisStruct* psys_chassis_t;
  gimbalStruct* psys_gimbal_t;
}sysDetectStruct; 
void SystemSelfChecking(void);
MOD_Status CheckTackData(sysDetectStruct* sds);
  void OffLineTip(void);
#endif	// __SYS_DETECT_H
  
 /*------------------------------------file of end-------------------------------*/


