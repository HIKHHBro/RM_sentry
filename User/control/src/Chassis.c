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
	RM3508Struct wheel1_t;//����1
	RM3508Struct wheel2_t;//����2
	extern CAN_HandleTypeDef hcan1;
	#define WHEEL1_ID 0x205
	#define WHEEL2_ID 0x206
/**
	* @Data    2019-01-19 11:45
	* @brief   �������ݽ���
	* @param   chassisStruct* css ���̽ṹ��ָ��
	* @retval  void
	*/
	void ChassisInit(chassisStruct* css)
	{
		css->pwheel1_t = &wheel1_t;
		css->pwheel2_t = &wheel2_t;
		RM3508StructInit(css->pwheel1_t,&hcan1);
		RM3508StructInit(css->pwheel2_t,&hcan1);
	}
/**
	* @Data    2019-01-19 12:01
	* @brief   �������ݽ���
	* @param   Peripheral type��������,����CAN_HandleTypeDef,can��CAN_HandleTypeDef
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
/*----------------------------------file of end-------------------------------*/
