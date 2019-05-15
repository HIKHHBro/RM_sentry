/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,海康平头哥,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|          By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                     https://github.com/GCUWildwolfteam                     |
	|----------------------------------------------------------------------------|
	|--FileName    : DJI_dbus.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-15               
	|--Libsupports : 
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-------------------------------declaration of end---------------------------|
 **/
#include "DJI_dbus.h" 
#ifdef  RC_UART            
#define RC_DATA_RX_LEN   (18 + HEAD_FRAME_LEN)
uint8_t databuff[RC_DATA_RX_LEN];//数据接收
static dbusStruct lostdata;
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
		dbs->a_frame_len = RC_DATA_RX_LEN;//一帧数据长度  18（一帧数据）+3（校验）
		dbs->check_byte = 0x55;//帧尾校验
		dbs->huartx = huartx;
		/* -------- 初始化开启串口数据接收 --------- */
		if(UsartAndDMAInit(huartx, dbs->a_frame_len,ENABLE) != HAL_OK)
		{
			dbs->status = 0;
			return HAL_ERROR;
		}
    dbs->fps[FPS_ADDR]= 1;
    SetFpsAddress(dbs->fps);
		SET_BIT(dbs->status,INIT_OK);
		return HAL_OK;
	}
/**
	* @Data    2019-01-16 15:07
	* @brief   数据解析
	* @param   void
	* @retval  void
	*/
	void DbusParseData(dbusStruct* dbs)
	{
//    &&RCRDecryption\
//		(databuff,dbs->a_frame_len) == HAL_OK
//          taskENTER_CRITICAL();
		if(UserUsartQueueRX(dbs->huartx,databuff) == HAL_OK)
		{
		dbs->ch1 = (databuff[0+DATA_LEN_BYTE_LEN] | databuff[1+DATA_LEN_BYTE_LEN]<<8) & 0x07FF;
		dbs->ch1 -= 1024;
    dbs->ch1 = DbusAntiShake(20,dbs->ch1);
		dbs->ch2 = (databuff[1+DATA_LEN_BYTE_LEN]>>3 | databuff[2+DATA_LEN_BYTE_LEN]<<5 ) & 0x07FF;
		dbs->ch2 -= 1024;
    dbs->ch2 = DbusAntiShake(20,dbs->ch2);
		dbs->ch3 = (databuff[2+DATA_LEN_BYTE_LEN]>>6 | databuff[3+DATA_LEN_BYTE_LEN]<<2 | databuff[4+DATA_LEN_BYTE_LEN]<<10) & 0x07FF;
		dbs->ch3 -= 1024;
    dbs->ch3 = DbusAntiShake(20,dbs->ch3);
		dbs->ch4 = (databuff[4+DATA_LEN_BYTE_LEN]>>1 | databuff[5+DATA_LEN_BYTE_LEN]<<7) & 0x07FF;		
		dbs->ch4 -= 1024;
    dbs->ch4 = DbusAntiShake(20,dbs->ch4);
		
		dbs->switch_left = ( (databuff[5+DATA_LEN_BYTE_LEN] >> 4)& 0x000C ) >> 2;
		dbs->switch_right =  (databuff[5+DATA_LEN_BYTE_LEN] >> 4)& 0x0003 ;
		
		dbs->mouse.x = databuff[6+DATA_LEN_BYTE_LEN] | (databuff[7+DATA_LEN_BYTE_LEN] << 8);	//x axis
		dbs->mouse.y = databuff[8+DATA_LEN_BYTE_LEN] | (databuff[9+DATA_LEN_BYTE_LEN] << 8);
		dbs->mouse.z = databuff[10+DATA_LEN_BYTE_LEN]| (databuff[11+DATA_LEN_BYTE_LEN] << 8);
		
		dbs->mouse.press_left 	= databuff[12+DATA_LEN_BYTE_LEN];	// is pressed?
		dbs->mouse.press_right 	= databuff[13+DATA_LEN_BYTE_LEN];
		
		dbs->keyBoard.key_code 	= databuff[14+DATA_LEN_BYTE_LEN] | databuff[15+DATA_LEN_BYTE_LEN] << 8; //key broad code
    dbs->thumbwheel =((int16_t)databuff[16 +DATA_LEN_BYTE_LEN] | ((int16_t)databuff[17 +DATA_LEN_BYTE_LEN] << 8)) & 0x07FF;
     dbs->thumbwheel -= 1024;
     Fps(dbs->fps);
    SET_BIT(dbs->status,RX_OK);
     lostdata = *dbs;
		}
		else
		{
      if(GetFps(dbs->fps) ==0)
      {
        dbs->ch1 = 0;
        dbs->ch2 = 0;
        dbs->ch3 = 0;
        dbs->ch4 = 0;
       CLEAR_BIT(dbs->status,RX_OK);
      }
      else *dbs = lostdata;
//    
		}
//        taskEXIT_CRITICAL()	;
	}
	/**
		* @Data    2019-02-14 21:06
		* @brief   遥控防抖
		* @param   int16_t range 防抖范围
		* @param 	 int16_t *data 指向数据地址的指针
		* @retval  void
		*/
		int16_t DbusAntiShake(int16_t range,int16_t data)
		{
			if(data > -(ABS(range)) && data < (ABS(range)))
				data = 0;
			return data;
		} 
#endif
	/*----------------------------------file of end-----------------------------*/
