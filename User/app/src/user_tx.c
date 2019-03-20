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
	|--FileName    : user_tx.c                                              
	|--Version     : v1.0                                                          
	|--Author      : ����ƽͷ��                                                     
	|--Date        : 2019-03-20             
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
#include "user_tx.h" 
extern osThreadId startTxTaskHandle;//��������
extern xQueueHandle gimbal_queue;
userTxStruct userTx_t;
	/**
		* @Data    2019-03-20 20:06
		* @brief   �û����������ʼ��
		* @param   void
		* @retval  void
		*/
		void UserTxInit(const dbusStruct* pRc_t)
		{
			userTx_t.status = MOD_READ;
			userTx_t.rc = pRc_t;
			SET_BIT(userTx_t.status,INIT_OK);
		/* ------ �������񣬵ȴ���ʼ�� ------- */
	  	vTaskSuspend(startTxTaskHandle);
			GetUserTxStatus();

		}
	/**
		* @Data    2019-03-20 20:36
		* @brief   �û������������
		* @param   void
		* @retval  void
		*/
		void UserTxControl(void)
		{
			if(userTx_t.rc->switch_left ==1)
			{
				int16_t data[8]={0};
				portBASE_TYPE xStatus;
			 xStatus = xQueueReceive(gimbal_queue,data,0);
				if(xStatus == pdPASS)
				{
						GimbalCanTx(data[0],data[1],data[2]);
				}
			}
   else
	 {
		 GimbalCanTx(0,0,0);
	 }

		}
	/**
		* @Data    2019-03-20 20:12
		* @brief   �û�����������������
		* @param   void
		* @retval  void
		*/
		void SetUserTxStatus(void)
		{
			SET_BIT(userTx_t.status,START_OK);
		}
  /**
  * @Data    2019-03-15 23:14
  * @brief   ��ȡ��������״̬״̬
  * @param   void
  * @retval  void
  */
  uint32_t GetUserTxStatus(void)
  {
    return userTx_t.status;
  }
/*-----------------------------------file of end------------------------------*/


