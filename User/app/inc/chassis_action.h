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
  |--FileName    : chassis_action.h                                                
  |--Version     : v1.0                                                            
  |--Author      : ����ƽͷ��                                                       
  |--Date        : 2019-04-15               
  |--Libsupports : STM32CubeF1 Firmware Package V1.6.0(�ñ�Ŀ�����������)
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
#ifndef __CHASSIS_ACTION_H 
#define __CHASSIS_ACTION_H 
#include "chassis.h"
  void ChassisRcControlMode(void);
  void ChassisCruiseModeInit(void);
	int16_t Go(int16_t target,int16_t speed);
  void ChassisCruiseModeInit(void);
  void ChassisControlSwitch(uint32_t commot);
  uint32_t ChassisControlDecision(void);
  void ChassisPcShootModeInit(void);
  void ChassisEludeControlModeInit(void);
  void ChassisPcShootMode(void);
  void ChassisEludeControlMode(void);
  	void ChassisCruiseModeControl(void);
    void ChassisOutOfControlMode(void);
    void ChassisOutOfControlInit(void);
#endif	// __CHASSIS_ACTION_H
  
 /*------------------------------------file of end-------------------------------*/


