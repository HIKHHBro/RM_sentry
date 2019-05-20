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
#define JUN_ZHUN_VERSION 1
#define ZI_KAI_VERSION 0
#define PC_CHECK_BYTE (0x55)//校验位
#if JUN_ZHUN_VERSION
#define PC_DATA_LEN (12 + HEAD_FRAME_LEN)//接收数据长度
#elif ZI_KAI_VERSION
#define PC_DATA_LEN (20 + HEAD_FRAME_LEN)//接收数据长度
#endif 
#define  PC_DATA_LEN_BSP (PC_DATA_LEN+3)
uint8_t pc_databuff[PC_DATA_LEN_BSP];
static pcDataStruct lastpc;
SqQueue pc_yaw_queue;
SqQueue pc_pitch_queue;
fps_t tem_fps,tem_fps1;
int16_t fx_[4],x_[4];
int16_t low_cont,max_low_cont=500;
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
      GyinitQueue(&pc_yaw_queue);
      GyinitQueue(&pc_pitch_queue);
      if(UsartAndDMAInit(PC_DATA_UASRT,PC_DATA_LEN_BSP,ENABLE) != HAL_OK)
      {
        //报错机制
        return HAL_ERROR;
      }
      SetFpsAddress(pc->fps);
      SetFpsAddress(tem_fps);
      SetFpsAddress(tem_fps1);
      SET_BIT(pc->status,INIT_OK);
      return HAL_OK;
    }
/**
* @Data    2019-03-21 00:46
* @brief   数据解析
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
        int16_t last_yaaw_get;
    int16_t e_pitch_temp =IND_PITCH,e_yaw_temp=IND_YAW;//英特尔  490//普通   270  290
void Pc_ParseData(pcDataStruct* pc)
{
  if(UserUsartQueueRX(PC_DATA_UASRT,pc_databuff) == HAL_OK)
  {
    if(pc_databuff[0+DATA_LEN_BYTE_LEN] == PC_CHECK_BYTE)
    {
    tem_yaw = (int16_t)((pc_databuff[1+DATA_LEN_BYTE_LEN]<<8) | pc_databuff[2+DATA_LEN_BYTE_LEN]);
      __tem_yaw = (tem_yaw-e_yaw_temp);//英特尔  490//普通   390
    pc->yaw_target_angle = YawDataConversion(__tem_yaw);
    tem_pitch= (int16_t)((pc_databuff[3+DATA_LEN_BYTE_LEN]<<8) | pc_databuff[4+DATA_LEN_BYTE_LEN]);
      __tem_pitch = (tem_pitch-e_pitch_temp);
    pc->pitch_target_angle = PitchDataConversion(__tem_pitch);
     pc->commot =pc_databuff[5+DATA_LEN_BYTE_LEN];
      pc->shoot_commot = pc_databuff[6+DATA_LEN_BYTE_LEN];
      pc->tem_fps = (pc_databuff[8+DATA_LEN_BYTE_LEN]<<8)|pc_databuff[9+DATA_LEN_BYTE_LEN];
      pc->distance = (pc_databuff[10+DATA_LEN_BYTE_LEN]<<8)|pc_databuff[11+DATA_LEN_BYTE_LEN];
    //  Fps(pc->fps);
             pc->yaw_target_angle = (int16_t)( 0.1*last_yaaw_get  + (1-0.1)*pc->yaw_target_angle);
      last_yaaw_get = pc->yaw_target_angle;
      low_cont =0;
//        fx_[0] =  fx_[1];
//       fx_[1] =  fx_[2];
//      fx_[2] =  fx_[3];
//      fx_[3] =  pc->yaw_target_angle;
//      low_cont = 0;
//      enQueue(&pc_yaw_queue,pc->yaw_target_angle,MAXSIZE);
//      deQueue(&pc_yaw_queue,&temmmm,MAXSIZE);
//      enQueue(&pc_pitch_queue,pc->pitch_target_angle,MAXSIZE);
//      deQueue(&pc_pitch_queue,&temmmm,MAXSIZE);
//      for(uint8_t i=0;i<MAXSIZE;i++)
//      {
//        pc->yaw_target_angle +=pc_yaw_queue.data[i];
////         pc->pitch_target_angle += pc_pitch_queue.data[i];
//      }
//      pc->yaw_target_angle = (int16_t)(pc->yaw_target_angle * 0.2);
//      pc->pitch_target_angle  = (int16_t)(pc->pitch_target_angle* 0.2);
//      lastpc  = *pc;
//      if(flagdf <-20)
//      {
//        flagdf =0;
//      }
//      else   flagdf--;
     
    }
    else if(pc_databuff[0 + DATA_LEN_BYTE_LEN] == 0xA5)
    {
//      if(low_cont < max_low_cont)
//      {
//      pc->pitch_target_angle = 0;
//      pc->yaw_target_angle = 0;
//        pc->commot = 1;//pc_databuff[5];
//       pc->shoot_commot = 0;
//        low_cont ++;
//      }
//      else 
//      {
        pc->pitch_target_angle = 0;
        pc->yaw_target_angle = 0;
        pc->commot = 0;
        pc->status = 0;
        pc->distance = 0;
//      }
//      if(flagdf > 20)
//      {
//       pc->commot = 0;//pc_databuff[5];
//       pc->shoot_commot = 0;
//      }
//      else 
//      {
//        flagdf++;
//         *pc =  lastpc;
//      }
    }  
     SET_BIT(pc->status,RX_OK);    
     Fps(pc->fps);
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
  else if(GetFps(pc->fps) ==0)
  {
      pc->pitch_target_angle = 0;
      pc->yaw_target_angle = 0;
      pc->commot = 0;
      pc->status = 0;
     pc->distance = 0;
    pc->shoot_commot=0;
       CLEAR_BIT(pc->status,RX_OK);
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
float zhengzhongcen = 1;
int16_t zhengxiao = 2;
float zhengxixiaoen = -1;
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
     return (int16_t)(yaw*(zhengxixiaoen));
//  }
}
int16_t pitch_coe =-1;
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
     return pitch *pitch_coe;
}

/**
* @Data    2019-03-21 00:46
* @brief   小电脑正常退出
* @param   void
* @retval  void
*/
uint8_t esc;
//int16_t pc_red =0;
void EscPc(int16_t key,int16_t ch1,int16_t ch2,int16_t ch3,int16_t ch4,int16_t thumbwheel,int16_t key1)
{
  if(key ==2 && ch1 < -600 && ch2 <-600 && ch3 >600 && ch4 <-600)
  {
//    if( (ch1 < -600) && \
//        (ch2 <-600) && \
//        (ch3 >600) && \
//        (ch4 <-600) )//内八设置编码器零点
    esc= 'q';
  }
   // printf("q");
  else if(key1 ==2&&thumbwheel >600)
  {
        esc= 'r';
  }
  else if(key1 ==2&& thumbwheel <-600)
  {
    esc= 'b';
  }
  else 
  {
    if(esc == 'q')
    esc = 'r';
  }
  if(HAL_UART_Transmit(PC_DATA_UASRT,&esc,1,1) ==HAL_OK)
      Fps(tem_fps1);
   // HAL_UART_Transmit(PC_DATA_UASRT,'r',1,0);
  //printf("r");

}
/**
* @Data    2019-03-21 00:46
* @brief   获取陀螺仪角度数据
* @param   void
* @retval  void
*/
//void GetGyroAngle(int16_t yaw_angle,int16_t pitch_angle)
//{
//    floatToUnion p;
//  p.s_16[0] = yaw_angle;
//  p.s_16[1] = pitch_angle;
//  esc[1] = p.u_8[0];
//  esc[2] = p.u_8[1];
//  esc[3] = p.u_8[2];
//  esc[4] = p.u_8[3];
//  Fps(tem_fps);
//}

/*------------------------------------file of end-------------------------------*/


