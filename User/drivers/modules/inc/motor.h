/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                      (C) Copyright 2018,����ƽͷ��,                         |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                         All Rights Reserved                            -   |
	|                                                                            |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)       -  |
	|                     https://github.com/GCUWildwolfteam                     |
	|----------------------------------------------------------------------------|
	|--FileName    : motor.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2018-12-31               
	|--Libsupports : ��׼���HAL��
	|--Description : 1��maxion���+RoboModule���� 
	|								 2��3508���+c610���
	|								 3��6623���
	|                4��6002���                                  
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#ifndef __MOTOR_H 
#define __MOTOR_H
	#include "baseclass.h"
	#include "bsp_can.h" 
/* ============================= RM6623 of begin ============================ */
	typedef struct RM6623Struct
	{
		uint16_t id;//���can��ip
		int16_t target;		 //Ŀ��ֵ
		int16_t tem_target;//��ʱĿ��ֵ
		int16_t real_current; //��ʵ����
		int16_t real_angle;//��ʵ�Ƕ�
		int16_t tem_angle;//��ʱ�Ƕ�
		int16_t zero;			 //������
		int16_t Percentage;//ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
		int16_t thresholds; //�����ת��ֵ
		CAN_HandleTypeDef *hcanx;
	} RM6623Struct;
	void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx);
	void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data);
/* ============================= RM6623 of end ============================== */
/* ============================ RM3508 of begin ============================= */
	typedef struct RM3508Struct
	{
		uint16_t id;//���can��ip
		int16_t target; //Ŀ��ֵ
		int16_t real_current;//��ʵ����
		int16_t real_angle;//��ʵ�Ƕ�
		int16_t real_speed;//��ʵ�ٶ�
		CAN_HandleTypeDef *hcanx;
	}RM3508Struct;
	void RM3508StructInit(RM3508Struct *RM3508,CAN_HandleTypeDef *hcanx);
	void RM3508ParseData(RM3508Struct *RM3508,uint8_t *data);
/* ============================== Rm3508 of end ============================= */
int16_t RatiometricConversion(int16_t real,int16_t threshold,int16_t perce);
int16_t zeroArgument(int16_t real,int16_t threshold);
#endif	// __MOTOR_H
/*---------------------------------file of end--------------------------------*/


