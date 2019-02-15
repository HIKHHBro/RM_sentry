/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,����ƽͷ��,                       |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.c                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2019-01-19               
	|--Libsupports : 
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-------------------------------declaration of end---------------------------|
 **/
#include "chassis.h"
	chassisStruct chassis_t;
	RM3508Struct wheel1_t;//����1
	RM3508Struct wheel2_t;//����2
	#define WHEEL1_ID 0x205
	#define WHEEL2_ID 0x206
	#define CHASSIS_CAN_TX_ID 0x200
/**
	* @Data    2019-01-19 11:45
	* @brief   �������ݽ���
	* @param   chassisStruct* css ���̽ṹ��ָ��
	* @retval  void
	*/
	void ChassisInit(CAN_HandleTypeDef *hcan,chassisStruct *css)
	{
		css->hcanx = hcan;
		/* ------ ����1�ṹ���ʼ�� ------- */
		css->pwheel1_t = &wheel1_t;
		wheel1_t.id = 0;
		wheel1_t.target = 0;
		wheel1_t.real_current = 0;
		wheel1_t.real_angle = 0;
		wheel1_t.real_speed = 0;
		/* ------ ����2�ṹ���ʼ�� ------- */
		css->pwheel2_t = &wheel2_t;
		wheel2_t.id = 0;
		wheel2_t.target = 0;
		wheel2_t.real_current = 0;
		wheel2_t.real_angle = 0;
		wheel2_t.real_speed = 0;
	}
/**
	* @Data    2019-01-19 12:01
	* @brief   �������ݽ���
	* @param   Peripheral type��������,����CAN_HandleTypeDef,can ��CAN_HandleTypeDef
	* @retval  void
	*/
	void ChassisParseDate(uint16_t id)
	{
		
		switch (id)
		{
			case WHEEL1_ID:
				// RM3508ParseData();
				break;
		
			default:
				break;
		}
	}
/**
	* @Data    2019-02-15 14:15
	* @brief   ����can���ݷ���
	* @param   void
	* @retval  void
	*/
	void ChassisCanTx(int16_t w1,int16_t w2)
	{
		uint8_t s[8]={0};
		s[0] = (uint8_t)(w1<<8);
		s[1] = (uint8_t)w1;
		s[2] = (uint8_t)(w2<<8);
		s[3] = (uint8_t)w2;
		CanTxMsg(chassis_t.hcanx,CHASSIS_CAN_TX_ID,s);
	}
/*----------------------------------file of end-------------------------------*/
