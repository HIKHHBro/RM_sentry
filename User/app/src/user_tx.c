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
	|--FileName    : user_tx.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
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
extern osThreadId startTxTaskHandle;//发送任务
extern xQueueHandle gimbal_queue;
userTxStruct userTx_t;
	/**
		* @Data    2019-03-20 20:06
		* @brief   用户发送任务初始化
		* @param   void
		* @retval  void
		*/
		void UserTxInit(const dbusStruct* pRc_t)
		{
			userTx_t.status = MOD_READ;
			userTx_t.rc = pRc_t;
			SET_BIT(userTx_t.status,INIT_OK);
		/* ------ 挂起任务，等待初始化 ------- */
	  	vTaskSuspend(startTxTaskHandle);
			GetUserTxStatus();

		}
	/**
		* @Data    2019-03-20 20:36
		* @brief   用户发送任务控制
		* @param   void
		* @retval  void
		*/
    int16_t ssssss =0;
  uint8_t pc_data[8]={0};
		void UserTxControl(void)
    {
      memset(pc_data,0,8);
			if(userTx_t.rc->switch_left ==1)
			{
				portBASE_TYPE xStatus;
			 xStatus = xQueueReceive(gimbal_queue,pc_data,0);
				if(xStatus == pdPASS)
				{
           taskENTER_CRITICAL();
					CanTxMsg(GIMBAL_CAN,GIMBAL_CAN_TX_ID,pc_data);
            taskEXIT_CRITICAL();
				}
			}
   else
	 {
     memset(pc_data,0,8);
		 	CanTxMsg(GIMBAL_CAN,GIMBAL_CAN_TX_ID,pc_data);
	 }

		}
	/**
		* @Data    2019-03-20 20:12
		* @brief   用户发送任务启动设置
		* @param   void
		* @retval  void
		*/
		void SetUserTxStatus(void)
		{
			SET_BIT(userTx_t.status,START_OK);
		}
  /**
  * @Data    2019-03-15 23:14
  * @brief   获取发送任务状态状态
  * @param   void
  * @retval  void
  */
  uint32_t GetUserTxStatus(void)
  {
    return userTx_t.status;
  }
/*-----------------------------------file of end------------------------------*/


