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
	|--FileName    : rammer.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-03-16               
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
#ifndef __RAMMER_H 
#define __RAMMER_H 
#include "motor.h"
#include "DJI_dbus.h"  
#include "pc_data.h" 
 	#define RAMMER_RX_ID	 					  0x207//拨弹电机接收id
	#define RAMMER_ANGER_COE         (0.0012F)//拨弹电机角度转换系数
	#define RAMMER_REDUCTION         (36)
	#define STUCK_BULLET_THRE        (5000)//卡弹电流阀值
//	#define RAMMER_TIME             (200)  // 1s 拨弹间隔  时间基数为10ms
//  #define LOCK_ROTOT_TIME         (3 * RAMMER_TIME) //3s 堵转时间
  #define CLOCK_WISE             //顺时针为正
  #define PARTITION_NUMB           (1366)//每格转的值PARTITION_NUMB = M2006_POLES/x
  #define SHAKE_VAULE               (600)//抖动范围
  #define RAMMER_LIMIMT_CUT                 (3000)//电流限幅
  // #define ANTI_CLOCK_WISE	        //逆时针为正方向
  	#define FRICTIONGEAR_SPEED 				(1400)	//摩擦轮速度
	M2006Struct* RammerInit(void);
	void RammerControl(void);
		int16_t PCycleNumerical(int16_t data);
		int16_t MCycleNumerical(int16_t data);
    	void SetRammerPID(uint8_t speed);
      void Shoot(uint8_t speed,uint8_t buffer_on);
#endif	// __RAMMER_H
/*-----------------------------------file of end------------------------------*/


