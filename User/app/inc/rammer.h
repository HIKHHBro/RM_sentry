/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,����ƽͷ��,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : rammer.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
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
/* ----------------- �����궨�� -------------------- */
 	#define RAMMER_RX_ID	 					  0x207//�����������id
	#define RAMMER_ANGER_COE         (0.0012F)//��������Ƕ�ת��ϵ��
	#define RAMMER_REDUCTION         (36)
	#define STUCK_BULLET_THRE        (5000)//����������ֵ
//	#define RAMMER_TIME             (200)  // 1s �������  ʱ�����Ϊ10ms
//  #define LOCK_ROTOT_TIME         (3 * RAMMER_TIME) //3s ��תʱ��
  #define CLOCK_WISE             //˳ʱ��Ϊ��
  #define PARTITION_NUMB           (1366)//ÿ��ת��ֵPARTITION_NUMB = M2006_POLES/x
  #define SHAKE_VAULE               (600)//������Χ
  #define RAMMER_LIMIMT_CUT                 (3000)//�����޷�
  // #define ANTI_CLOCK_WISE	        //��ʱ��Ϊ������
  #define FRICTIONGEAR_SPEED 				(1400)	//Ħ�����ٶ�
/*
 * ��Ƶӳ������ matlab ������ߺ�������������polynomial, Degree=2
 */
  #define S_P1_                      (0.327)//p1ϵ��  
  #define S_P2_                      (-23.55)//p2ϵ��
  #define S_P3_                      (520.3)//p3
  #define CAL_SHOOT_SPEED(_x)  ((int16_t)((S_P1_ * _x * _x) + (S_P2_ * _x) + S_P3_))
/*
 * ����ӳ����Ƶ matlab ������ߺ�������������polynomial, Degree=1
 * _s ÿ����Ƶ _s = (80cm~250cm) ������
 */
  #define F_P1_                      (-0.088)//p1ϵ��  
  #define F_P2_                      (23.04)//p2ϵ��
  #define CAL_SHOOT_FRE(_s)   ((uint8_t)((F_P1_ * _s) + F_P2_))
  // #define IS_PARAM_X(x)  do{                      \
  //                            x = x > 16 ? 16 : x; \
  //                            x = x < 1 ? 1:x;     \
  //                          }while(0)���Ӳ������

	M2006Struct* RammerInit(void);
	void RammerControl(void);
		int16_t PCycleNumerical(int16_t data);
		int16_t MCycleNumerical(int16_t data);
    	void SetRammerPID(uint8_t speed);
      void Shoot(uint8_t speed,uint8_t buffer_on);
#endif	// __RAMMER_H
/*-----------------------------------file of end------------------------------*/


