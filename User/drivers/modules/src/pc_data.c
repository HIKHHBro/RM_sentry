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
uint8_t pc_databuff[8];
static pcDataStruct lastpc;
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
      pc->commot = 0;
      pc->status = 0;
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
      int16_t tem_yaw;
  int16_t tem_pitch;
     int16_t __tem_yaw;
  int16_t __tem_pitch;
    float yawscinf =-1;
    float pitchddd =1;
void Pc_ParseData(pcDataStruct* pc)
{
  if(UserUsartQueueRX(PC_DATA_UASRT,pc_databuff) == HAL_OK)
  {
    if(pc_databuff[0] == PC_CHECK_BYTE)
    {
    tem_yaw = (int16_t)((pc_databuff[1]<<8) | pc_databuff[2]);
      __tem_yaw = (tem_yaw-220);
    pc->yaw_target_angle = YawDataConversion(__tem_yaw);
    tem_pitch= (int16_t)((pc_databuff[3]<<8) | pc_databuff[4]);
      __tem_pitch = (tem_pitch-223);
    pc->pitch_target_angle = PitchDataConversion(__tem_pitch);
     pc->commot = pc_databuff[5];
      lastpc  = *pc;
    }
    
//    
//    else if((pc->yaw_target_angle > 320)||(pc->yaw_target_angle <-320))
//    {
//      pc->yaw_target_angle = 0;
//    }
//   else if((pc->pitch_target_angle > 180)||(pc->pitch_target_angle <-180))
//    {
//      pc->pitch_target_angle = 0;
//    }
    
  }
  else
  {
     *pc = lastpc;
  }
  
}
/**
* @Data    2019-03-21 00:46
* @brief   自瞄数据转换//待测试
* @param   void
* @retval  void
*/
int16_t lockqu =340;
int16_t zhengda = 140;
float zhengdacen = 11;
int16_t zhengzhong = 70;
float zhengzhongcen = 8;
int16_t zhengxiao = 2;
float zhengxixiaoen = 3;
int16_t plockqu =200;
int16_t pzhengda = 100;
float pzhengdacen = 0;
int16_t pzhengzhong = 0;
float pzhengzhongcen = 0;
int16_t pzhengxiao = 0;
float pzhengxixiaoen = 0;
int16_t YawDataConversion(int16_t yaw)
{
   if((yaw > lockqu)||(yaw <-lockqu))
    {
     return 0;
    }
//  else if((yaw > zhengda) || (yaw < -zhengda))
//  {
//    return (int16_t)(yaw*(-zhengdacen));
//  }
//  else if((yaw > zhengzhong) || (yaw< -zhengzhong))
//  {
//    return (int16_t)(yaw*(-zhengzhongcen));
//  }
  else
  {
     return (int16_t)(yaw*(-zhengxixiaoen));
  }
}

int16_t PitchDataConversion(int16_t pitch)
{
   if((pitch > plockqu)||(pitch <-plockqu))
    {
     pitch = 0;
    }
//  if((pitch >pzhengda) || (pitch < -pzhengda))
//  {
//   
//    return (int16_t)(pitch * pzhengdacen);
//  }
//  else if((pitch > pzhengzhong) || (pitch< -pzhengzhong))
//  {
//    return (int16_t)(pitch*pzhengzhongcen);
//  }
  else
  {
     return pitch;
  }
}

/*------------------------------------file of end-------------------------------*/


