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
  |--FileName    : gyro.c                                                
  |--Version     : v1.0                                                            
  |--Author      : 海康平头哥                                                       
  |--Date        : 2019-03-25               
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
#include "gyro.h" 
#if  defined (GYRO_CAN) || defined (GYRO_UART)

#define GY955BACKLEN   3
#define GY955LEN   (18+GY955BACKLEN)
#define EULERANGLE 0x08
#define GYRO       0x04
  /**
  * @Data    2019-03-25 01:32
  * @brief   gy955初始化
  * @param   void
  * @retval  void
  */
uint8_t GB955Buffer[18];//数据接收
void Gy955Init(UART_HandleTypeDef *huartx)
{
  UsartAndDMAInit(huartx,GY955LEN,ENABLE);
}
  /**
  * @Data    2019-03-25 01:32
  * @brief   gy955数据解析
  * @param   void
  * @retval  void
  */
void AnalysisGyro(gy955Struct* Gyc)
{
  uint8_t sum,i;
	if ((GB955Buffer[0]==0x5A)&&(GB955Buffer[1]==0x5A))
	 {
	 
		for (sum=0,i = 0; i < GB955Buffer[3]+4; i++)  sum+=GB955Buffer[i];
	
		if(GB955Buffer[i]==sum)//校验和判断
		{
			if((GB955Buffer[2]&GYRO)==GYRO)
			 {
				Gyc->Gyr_X=((GB955Buffer[4]<<8)|GB955Buffer[5]);
				Gyc->Gyr_y=((GB955Buffer[6]<<8)|GB955Buffer[7]);
				Gyc->Gyr_z=((GB955Buffer[8]<<8)|GB955Buffer[9]);
			 }
			if ((GB955Buffer[2]&EULERANGLE)==EULERANGLE)
			 {
				Gyc->Yaw=(uint16_t)((GB955Buffer[10]<<8)|GB955Buffer[11])/100.0f;
				Gyc->Roll=(int16_t)((GB955Buffer[12]<<8)|GB955Buffer[13])/100.0f;
				Gyc->Pitch=(int16_t)((GB955Buffer[14]<<8)|GB955Buffer[15])/100.0f;
			 }
		
			Gyc->counter++;
		}
	 }
}
void BingeGyroByCan(gyroStruct* Gyc,uint8_t *data)
{
  floatToUnion y,p;
  uint8_t  i;
  for(i=0;i<4;i++)
  {
   y.u_8[i] = data[i];
   p.u_8[i] = data[i+4];
  }
  Gyc->Yaw   = (int16_t)(y.s_16[0]/100.f);
  Gyc->Gyrz  = (int16_t)(y.s_16[1]*0.1);
  Gyc->Pitch = (int16_t)(p.s_16[0]/100.f);
  Gyc->Gyrx  = (int16_t)(p.s_16[1]*0.1);
}
#endif
/*------------------------------------file of end-------------------------------*/


