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
  |--FileName    : pc_data.c                                                
  |--Version     : v1.0                                                            
  |--Author      : ����ƽͷ��                                                       
  |--Date        : 2019-03-21               
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
#include "pc_data.h" 
#define PC_CHECK_BYTE (0x55)//У��λ
uint8_t pc_databuff[11];
    /**
    * @Data    2019-03-21 00:22
    * @brief   С�������ݽ��ճ�ʼ��
    * @param   void
    * @retval  void
    */
    HAL_StatusTypeDef PcDataRxInit(pcDataStruct* pc)
    {
      pc->status = MOD_READ;
      if(pc == NULL)
      {
        return HAL_ERROR;
      }
      pc->pitch_target_angle = 0;
      pc->yaw_target_angle = 0;
      if(UsartAndDMAInit(PC_DATA_UASRT,11,ENABLE) != HAL_OK)
      {
        //�������
        return HAL_ERROR;
      }
      SET_BIT(pc->status,INIT_OK);
      return HAL_OK;
    }
      /**
      * @Data    2019-03-21 00:46
      * @brief   ���ݽ���
      * @param   void
      * @retval  void
      */
      void Pc_ParseData(pcDataStruct* pc)
      {
        if(UserUsartQueueRX(PC_DATA_UASRT,pc_databuff) == HAL_OK)
        {
          if(pc_databuff[0] == PC_CHECK_BYTE)
          {
           pc->yaw_target_angle = (int16_t)((uint8_t)(pc_databuff[1]) | ((pc_databuff[2]<<8)&0xff00));
           pc->yaw_target_angle = (int16_t)((uint8_t)(pc_databuff[3]) | ((pc_databuff[4]<<8)&0xff00));
          }
          
          else
          {
            /* code */
          }
          
        }
        else
        {
          /* code */
        }
        
      }
/*------------------------------------file of end-------------------------------*/


