/**
  |-------------------------------- Copyright -----------------------------------|
  |                                                                              |
  |                        (C) Copyright 2019,海康平头哥,                         |
  |            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
  |                            All Rights Reserved                               |
  |                                                                              |
  |            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
  |                     https://github.com/GCUWildwolfteam                       |
  |------------------------------------------------------------------------------|
  |--FileName    : pc_data.c                                                
  |--Version     : v1.0                                                            
  |--Author      : 海康平头哥                                                       
  |--Date        : 2019-03-21               
  |--Libsupports : STM32CubeF1 Firmware Package V1.6.0(用别的库出问题别问我)
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
#define PC_CHECK_BYTE (0x55)//校验位
uint8_t pc_databuff[11];
    /**
    * @Data    2019-03-21 00:22
    * @brief   小电脑数据接收初始化
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
        //报错机制
        return HAL_ERROR;
      }
      SET_BIT(pc->status,INIT_OK);
      return HAL_OK;
    }
      /**
      * @Data    2019-03-21 00:46
      * @brief   数据解析
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


