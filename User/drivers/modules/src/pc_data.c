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
#define PC_DATA_LEN 10//�������ݳ���
#define  PC_DATA_LEN_BSP (PC_DATA_LEN+3)
uint8_t pc_databuff[10];
static pcDataStruct lastpc;
SqQueue pc_yaw_queue;
SqQueue pc_pitch_queue;
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
      pc->commot = 0;
      pc->status = 0;
      GyinitQueue(&pc_yaw_queue);
      GyinitQueue(&pc_pitch_queue);
      if(UsartAndDMAInit(PC_DATA_UASRT,PC_DATA_LEN_BSP,ENABLE) != HAL_OK)
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
      int16_t flagdf=0;
    int16_t temmmm;
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
      __tem_yaw = (tem_yaw-334);
    pc->yaw_target_angle = YawDataConversion(__tem_yaw);
    tem_pitch= (int16_t)((pc_databuff[3]<<8) | pc_databuff[4]);
      __tem_pitch = (tem_pitch-365);
    pc->pitch_target_angle = PitchDataConversion(__tem_pitch);
     pc->commot =pc_databuff[5];
      pc->shoot_commot = pc_databuff[6];
      pc->fps = (pc_databuff[8]<<8)|pc_databuff[9];
      enQueue(&pc_yaw_queue,pc->yaw_target_angle,MAXSIZE);
      deQueue(&pc_yaw_queue,&temmmm,MAXSIZE);
//      enQueue(&pc_pitch_queue,pc->pitch_target_angle,MAXSIZE);
//      deQueue(&pc_pitch_queue,&temmmm,MAXSIZE);
      for(uint8_t i=0;i<MAXSIZE;i++)
      {
        pc->yaw_target_angle +=pc_yaw_queue.data[i];
//         pc->pitch_target_angle += pc_pitch_queue.data[i];
      }
      pc->yaw_target_angle = (int16_t)(pc->yaw_target_angle * 0.2);
//      pc->pitch_target_angle  = (int16_t)(pc->pitch_target_angle* 0.2);
      lastpc  = *pc;
      if(flagdf <-20)
      {
        flagdf =0;
      }
      else   flagdf--;
     
    }
    else if(pc_databuff[0] == 0xA5)
    {
      if(flagdf > 20)
      {
        pc->yaw_target_angle =0;
      pc->pitch_target_angle =0;
       pc->commot = 0;//pc_databuff[5];
       pc->shoot_commot = 0;
      }
      else 
      {
        flagdf++;
         *pc =  lastpc;
      }

      
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
     
  }
  
}
/**
* @Data    2019-03-21 00:46
* @brief   ��������ת��//������
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
//   if((yaw > lockqu)||(yaw <-lockqu))
//    {
//     return 0;
//    }
//  else if((yaw > zhengda) || (yaw < -zhengda))
//  {
//    return (int16_t)(yaw*(-zhengdacen));
//  }
//  else if((yaw > zhengzhong) || (yaw< -zhengzhong))
//  {
//    return (int16_t)(yaw*(-zhengzhongcen));
//  }
//  else
//  {
     return (int16_t)(yaw*(-zhengxixiaoen));
//  }
}
int16_t pitch_coe =1;
int16_t PitchDataConversion(int16_t pitch)
{
//   if((pitch > plockqu)||(pitch <-plockqu))
//    {
//     return 0;
//    }
//  if((pitch >pzhengda) || (pitch < -pzhengda))
//  {
//   
//    return (int16_t)(pitch * pzhengdacen);
//  }
//  else if((pitch > pzhengzhong) || (pitch< -pzhengzhong))
//  {
//    return (int16_t)(pitch*pzhengzhongcen);
//  }
//  else
//  {
//     return pitch *pitch_coe;
//  }
}


/*------------------------------------file of end-------------------------------*/


