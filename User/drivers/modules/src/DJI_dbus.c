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
	|--FileName    : DJI_dbus.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-15               
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
#include "DJI_dbus.h" 
uint8_t databuff[21];//数据接收
	/*---------------------------------80字符限制-----------------------------------*/
		/**
		* @Data    2019-01-15 20:24
		* @brief   大疆遥控初始化
		* @param   dbusStruct* dbs 遥控结构体指针
		* @param   huartx(x=1,2,3,6)
		* @retval  HAL Status
		*/
		HAL_StatusTypeDef DJIDbusInit(dbusStruct* dbs,UART_HandleTypeDef *huartx)
		{
		// DBUSConnectStatus = Lost;
			if(dbs == NULL)
			{
				return HAL_ERROR;
				//添加huart限制判断
			}
			dbs->ch1 = 0;
			dbs->ch2 = 0;
			dbs->ch3 = 0;
			dbs->ch4 = 0;
			dbs->switch_left = 0;
			dbs->switch_right = 0;
			dbs->mouse.x = 0;
			dbs->mouse.y=0;
			dbs->mouse.z=0;
			dbs->mouse.press_left=0;
			dbs->mouse.press_right=0;
			dbs->mouse.jumppress_left=0;
			dbs->mouse.jumppress_right=0;
			dbs->keyBoard.key_code = 0;
			dbs->keyBoard.jumpkey_code = 0;
			dbs->state_flag = 0;//状态标志位
			dbs->a_frame_len = 21;//一帧数据长度  18（一帧数据）+3（校验）
			dbs->check_byte = 0x55;//帧尾校验
			dbs->huartx = huartx;
			/* -------- 初始化开启串口数据接收 --------- */
			if(UsartAndDMAInit(huartx, dbs->a_frame_len,ENABLE) != HAL_OK)
			{
				return HAL_ERROR;
			}
      return HAL_OK;
		}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-16 15:07
	* @brief   数据解析
	* @param   void
	* @retval  void
	*/
	void DbusParseData(dbusStruct* dbs)
	{
//    &&RCRDecryption\
//																						(databuff,dbs->a_frame_len) == HAL_OK
		if(UserUsartQueueRX(dbs->huartx,databuff) == HAL_OK)
		{
		dbs->ch1 = (databuff[0] | databuff[1]<<8) & 0x07FF;
		dbs->ch1 -= 1024;
		dbs->ch2 = (databuff[1]>>3 | databuff[2]<<5 ) & 0x07FF;
		dbs->ch2 -= 1024;
		dbs->ch3 = (databuff[2]>>6 | databuff[3]<<2 | databuff[4]<<10) & 0x07FF;
		dbs->ch3 -= 1024;
		dbs->ch4 = (databuff[4]>>1 | databuff[5]<<7) & 0x07FF;		
		dbs->ch4 -= 1024;
		
		dbs->switch_left = ( (databuff[5] >> 4)& 0x000C ) >> 2;
		dbs->switch_right =  (databuff[5] >> 4)& 0x0003 ;
		
		dbs->mouse.x = databuff[6] | (databuff[7] << 8);	//x axis
		dbs->mouse.y = databuff[8] | (databuff[9] << 8);
		dbs->mouse.z = databuff[10]| (databuff[11] << 8);
		
		dbs->mouse.press_left 	= databuff[12];	// is pressed?
		dbs->mouse.press_right 	= databuff[13];
		
		dbs->keyBoard.key_code 	= databuff[14] | databuff[15] << 8; //key borad code
		}
	}
	/*------------------------------------file of end-------------------------------*/
