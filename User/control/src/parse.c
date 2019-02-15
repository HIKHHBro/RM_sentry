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
	|--FileName    : parse.c                                              
	|--Version     : v1.0                                                          
	|--Author      : ����ƽͷ��                                                     
	|--Date        : 2019-02-02             
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
#include "parse.h" 
/* -------------- �ṹ�� ----------------- */
extern UART_HandleTypeDef huart1;//����1
extern CAN_HandleTypeDef hcan1;
dbusStruct dbus_t; //��ң��
extern uint8_t can1_rx[12];
//static uint8_t can1_rx_buffer[12]={0};
	/**
		* @Data    2019-02-02 12:59
		* @brief   �������ݳ�ʼ��
		* @param   void
		* @retval  void
		*/
		void ParseInit(void)
		{
      DJIDbusInit(&dbus_t, &huart1);//��ң�س�ʼ��
		}
	/**
		* @Data    2019-02-14 14:36
		* @brief   ���ݽ���
		* @param   void
		* @retval  void
		*/
		void ParseData(void)
		{
			uint32_t can_id = 0;
//			UserCanQueueRX(&hcan1,can1_rx);//cam1����
			ByleToMultibyte(&can1_rx[8],&can_id);
			if(can_id > GIMBAL_CAN_ID_L && can_id < GIMBAL_CAN_ID_H)
			{
				GimbalParseDate(can_id,can1_rx);
			}
      DbusParseData(&dbus_t);
		}
/*-----------------------------------file of end------------------------------*/


