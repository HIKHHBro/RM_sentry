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
  |--FileName    : usart_debug.c                                                
  |--Version     : v1.0                                                            
  |--Author      : 海康平头哥                                                       
  |--Date        : 2019-02-21               
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
#include "usart_debug.h" 
#ifdef DEBUG_BY_KEIL
#define USED_FLOAT 0x01 
#define USED_INT16 0x02
#define USED_UINT32 0x04 
#define IS_REUSED(_param) ( ((_param) == (USED_FLOAT)) || \
                            ((_param) == (USED_INT16)) || \
                            ((_param) == (USED_UINT32))  )
void usart2_send_char(uint8_t c,UART_HandleTypeDef* huart);
void usart2_niming_report(uint8_t fun,uint8_t*data,uint8_t len,\
													UART_HandleTypeDef* huart);
void PID_Debugfloat(float Target,float Real,UART_HandleTypeDef* huart);
void PID_Debugint16_t(int16_t Target,int16_t Real,UART_HandleTypeDef* huart);
void DebugUint32_t(UART_HandleTypeDef* huart,uint32_t data);
void NimingClassInit(void);
/* ----------------- 外部链接 -------------------- */
  extern UART_HandleTypeDef huart1;//串口1
  extern UART_HandleTypeDef huart2;//串口1
  extern UART_HandleTypeDef huart6;//串口1
Niming_Class  Debug_t;
/* -------------------------------- begin 1 -------------------------------- */
	/**
	* @brief  串口发送
	* @param  
	* @retval 
	**/
/* -------------------------------- end -------------------------------- */
void usart2_send_char(uint8_t c,UART_HandleTypeDef* huart)
{
    while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TXE)==0);//等待上一次发送完毕  
    huart->Instance->DR=c;   
}

/*---------------------------------80字符限制-----------------------------------*/
  /**
  * @Data    2019-02-21 23:32
  * @brief   重定向C库函数 printf 到USART2
  * @param   void
  * @retval  void
  */
	int fputc(int ch,FILE *f)  
  {   
    usart2_send_char((unsigned char)ch,&huart2);
    return (ch);    
  }
/* -------------------------------- begin 2 -------------------------------- */
	/**
	* @brief  匿名结构体初始化
	* @param  
	* @retval 
	**/
/* -------------------------------- end -------------------------------- */
void DebugClassInit(void)
{
	Debug_t.f=PID_Debugfloat;
	Debug_t.i16=PID_Debugint16_t;
	Debug_t.uint32 = DebugUint32_t;
	Debug_t.flag = 0;
}

/* -------------------------------- begin 3 -------------------------------- */
	/**
	* @brief     下位机发送数据给匿名上位机
    * @param / 1、uint8_t 功能字   0xA0~0xAF
    *        / 2、发送数据数组指针 最多24个字节
    *        / 3、发送数组有效区的数据长度 
	* @retval 
	**/
/* -------------------------------- end -------------------------------- */
void usart2_niming_report(uint8_t fun,uint8_t*data,uint8_t len,UART_HandleTypeDef* huart)
{
    uint8_t send_buf[32];
    uint8_t i;
    if(len>28)return;                                   //最多28字节数据
    send_buf[len+3]=0;                                  //校验数置零
    send_buf[0]=0X88;                                   //帧头
    send_buf[1]=fun;                                    //功能字
    send_buf[2]=len;                                    //数据长度
    for(i=0;i<len;i++)send_buf[3+i]=data[i];            //复制数据
    for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];   //计算校验和
    for(i=0;i<len+4;i++)usart2_send_char(send_buf[i],huart);  //发送数据到串口1
}

/* -------------------------------- begin 4 -------------------------------- */
   /**
    * @brief  在匿名上位机调试PID函数
    * @param / float Target_Angle目标角度
    *        / float Real_Angle实际角度
    * @retval void
    **/
/* -------------------------------- end -------------------------------- */
void PID_Debugfloat(float Target,float Real,UART_HandleTypeDef* huart)
{
    uint8_t tbuf[8];
    unsigned char *p;
    /* -------- 使用标志位 --------- */   
      SET_BIT(Debug_t.flag,USED_FLOAT);
    /* -------- 检测是否充分调用，防止打印数据混乱 --------- */
     assert_param(IS_REUSED(Debug_t.flag));
     p=(unsigned char *)&Target;
     tbuf[0]=(unsigned char)(*(p+3));
     tbuf[1]=(unsigned char)(*(p+2));
	 tbuf[2]=(unsigned char)(*(p+1));
	 tbuf[3]=(unsigned char)(*(p+0));

    p=(unsigned char *)&Real;
     tbuf[4]=(unsigned char)(*(p+3));
     tbuf[5]=(unsigned char)(*(p+2));
	 tbuf[6]=(unsigned char)(*(p+1));
	 tbuf[7]=(unsigned char)(*(p+0));
	  p=NULL;
    usart2_niming_report(0XA1,tbuf,8,huart);//自定义帧,0XA2
}
/* -------------------------------- begin 5 -------------------------------- */
   /**
    * @brief  在匿名上位机调试PID函数
    * @param / int16_t Target_Angle目标角度
    *        / int16_t Real_Angle实际角度
    * @retval void
    **/
/* -------------------------------- end -------------------------------- */
void PID_Debugint16_t(int16_t Target,int16_t Real,UART_HandleTypeDef* huart)
{
  uint8_t tbuf[4];
  unsigned char *p;
  /* -------- 使用标志位 --------- */   
  SET_BIT(Debug_t.flag,USED_INT16);
  /* -------- 检测是否充分调用，防止打印数据混乱 --------- */
  assert_param(IS_REUSED(Debug_t.flag));
  p=(unsigned char *)&Target;
  tbuf[0]=(unsigned char)(*(p+1));
  tbuf[1]=(unsigned char)(*(p+0));

  p=(unsigned char *)&Real;
  tbuf[2]=(unsigned char)(*(p+1));
  tbuf[3]=(unsigned char)(*(p+0));
  p=NULL;
  Debug_t.flag = USED_INT16;
  usart2_niming_report(0XA1,tbuf,4,huart);//自定义帧,0XA2
}
/**
* @Data    2019-02-21 18:15
* @brief   发送uint32_t类型数据给匿名上位机
* @param   void
* @retval  void
*/
void DebugUint32_t(UART_HandleTypeDef* huart,uint32_t data)
{
    uint8_t tbuf[4];
    unsigned char *p;
    /* -------- 使用标志位 --------- */   
    SET_BIT(Debug_t.flag,USED_UINT32);
    /* -------- 检测是否充分调用，防止打印数据混乱 --------- */
    assert_param(IS_REUSED(Debug_t.flag));
    p=(unsigned char *)&data;
    tbuf[0]=(unsigned char)(*(p+3));
    tbuf[1]=(unsigned char)(*(p+2));
    tbuf[2]=(unsigned char)(*(p+1));
    tbuf[3]=(unsigned char)(*(p+0));
    p=NULL;
    Debug_t.flag = USED_UINT32;
    usart2_niming_report(0XA1,tbuf,4,huart);//自定义帧,0XA2
}
#endif
/*------------------------------------file of end-------------------------------*/



