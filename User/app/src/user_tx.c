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
extern  xQueueHandle chassis_queue;
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
    int16_t ssssss =0;
  uint8_t pc_data[8]={0};
void UserTxControl(void)
{
  memset(pc_data,0,8);
  uint32_t chassis_status;
  uint32_t gimbal_status;
  portBASE_TYPE xStatus;
 gimbal_status = GetGimbalStatus();
 chassis_status =  GetChassisStatus();
  EscPc(userTx_t.rc->switch_left,userTx_t.rc->ch1,userTx_t.rc->ch2,userTx_t.rc->ch3,userTx_t.rc->ch4,userTx_t.rc->thumbwheel,userTx_t.rc->switch_right);
  if((gimbal_status & RUNING_OK) ==RUNING_OK)
  {
    if(userTx_t.rc->switch_left !=DISABLE_MOD)
    {
      xStatus = xQueueReceive(gimbal_queue,pc_data,0);
      if(xStatus == pdPASS)
      {
        taskENTER_CRITICAL();
       CanTxMsg(GIMBAL_CAN,GIMBAL_CAN_TX_ID,pc_data);//ע����Ϊ��̨�����ʱʧ��
        taskEXIT_CRITICAL();
        	//	osDelay(1);
      }
    }
   else
   {
      memset(pc_data,0,8);
      CanTxMsg(GIMBAL_CAN,GIMBAL_CAN_TX_ID,pc_data);
   }
  }
if(((chassis_status & RUNING_OK) ==RUNING_OK) || IS_BYTE(chassis_status,DHECK_DATA_RUNING))
{
   if(userTx_t.rc->switch_left !=DISABLE_MOD)
   {
    memset(pc_data,0,8);
    xStatus = xQueueReceive(chassis_queue,pc_data,0);
    if(xStatus == pdPASS)
    {
      taskENTER_CRITICAL();
//      if(userTx_t.rc->switch_right ==3)
//      {
//        memset(pc_data,0,8);
//        CanTxMsg(CHASSIS_CAN,CHASSIS_CAN_TX_ID,pc_data);
//      }
   CanTxMsg(CHASSIS_CAN,CHASSIS_CAN_TX_ID,pc_data);//ע����Ϊ���̵����ʱʧ��
      taskEXIT_CRITICAL();
    }
  }
   else
   {
      memset(pc_data,0,8);
     CanTxMsg(CHASSIS_CAN,CHASSIS_CAN_TX_ID,pc_data);
   }
}
else  memset(pc_data,0,8);
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


