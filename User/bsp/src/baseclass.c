/**
	|-------------------------------- Copyright ----------------------------------------|
	|                                                                                   |
	|                        (C) Copyright 2018,海康平头哥,                              |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China            |
	|                            All Rights Reserved                                    |
	|                                                                                   |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)               |
	|                     https://github.com/GCUWildwolfteam                            |
	|-----------------------------------------------------------------------------------|
	|--FileName    : baseclass.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-30               
	|--Libsupports : STM32CubeF1 Firmware Package V1.6.0 / 17-May-2017(用别的库出问题别问我)
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end---------------------------------|
 **/
#include "baseclass.h" 
/* ----------------- 标志位变量定义 -------------------- */
	unsigned int e_periphera_interface_flag = 0; 	//外设接口状态变量定义
	unsigned int e_periphera_error_flag = 0;		//外设接口错误状态变量定义
	unsigned int e_objects_flag = 0;     //对象使用标志
	/**
	* @Data    2018-12-30 17:23
	* @brief   获取外设类型指针
	* @param   void* 
	* @retval  void*
	*/
	void* Get_Peripheral_type(void* Peripheral)
	{
		return Peripheral;
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2018-12-30 21:14
	* @brief   识别串口类型,设置相应串口标志状态位
	* @param   串口类型指针huartx(x=1、2、3、4、5、6)
	* @retval  USART_TypeDef 
	*/
	UART_HandleTypeDef* RecognizeUSARTType(UART_HandleTypeDef* huartx)
	{
		if (huartx->Instance == USART1) 
		{
			SET_BIT(e_periphera_interface_flag,USART1_BY_USED);
			return huartx;
		}
		else if (huartx->Instance == USART2) 
		{
			SET_BIT(e_periphera_interface_flag,USART2_BY_USED);
			return huartx;
		}
		else if (huartx->Instance == USART3) 
		{
			SET_BIT(e_periphera_interface_flag,USART3_BY_USED);
			return huartx;
		}
		else if(huartx->Instance == UART4)
		{
			SET_BIT(e_periphera_interface_flag,UART4_BY_USED);
			return huartx;
		}
		else if (huartx->Instance == UART5)
		{
			SET_BIT(e_periphera_interface_flag,UART5_BY_USED);
			return huartx;
		}
		else return NULL;

	}
	/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2018-12-30 21:14
	* @brief   识别can类型,设置相应can标志状态位
	* @param   串口类型指针hcanx(x=1、2)
	* @retval  CAN_HandleTypeDef 
	*/
CAN_HandleTypeDef* RecognizeCanType(CAN_HandleTypeDef* hcanx)
{
	if (hcanx->Instance == CAN1) 
	{
		SET_BIT(e_periphera_interface_flag,CAN1_BY_USED);
		return hcanx;
	} 
#if HAL_F4 
	else if (hcanx->Instance == CAN2) 
	{
		SET_BIT(e_periphera_interface_flag,CAN2_BY_USED);
		return hcanx;
	} 
#endif 
	else return NULL;
}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2018-12-30 22:16
	* @brief   掉帧标志位设置
	* @param   int16_t flag
	* @retval  void
	*/
	void SetFrameDropflag(int16_t flag)
	{
		flag = 0;
		flag--;
		if(flag < -10)
		{
			SET_BIT(e_periphera_error_flag,RX_ERROR);
			flag = 0;
		}
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2018-12-30 22:16
	* @brief   正常标志位设置
	* @param   int16_t flag
	* @retval  void
	*/
	void SetNormalflag(int16_t flag)
	{
		if(flag > 0)
		flag =1;
	}
/*---------------------------------80字符限制-----------------------------------*/
/**
* @Data    2019-01-16 16:01
* @brief   RCR校验加密
* @param   uint8_t* pdata 数据首地址  倒数一位是低八位 倒数第二位是高八位
* @param   uint8_t size 数据长度
* @retval  HAL Status
*/
HAL_StatusTypeDef RCREncryption(uint8_t* pdata,uint8_t size)
{
	if(size<4)
		return HAL_ERROR;
	uint8_t i;
	uint16_t sum;
	unsigned char *p;
	pdata[size-3] = 0x55;
	for (i = 0; i < (size-2); i++)
	{
		sum += pdata[i];
	}
	p=(unsigned char *)&sum;
	pdata[size-1] = (unsigned char)(*p);//低八位
	pdata[size-2] = (unsigned char)(*p+1);//高八位
	return HAL_OK;
}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-16 16:38
	* @brief   RCR校验加密
	* @param   uint8_t* pdata 数据首地址
	* @param   uint8_t size 数据长度
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef RCRDecryption(uint8_t* pdata,uint8_t size)
	{
		uint8_t i;
		uint16_t sumen;//加密前得值
		uint16_t sumde;//解密后的值
		for (i = 0; i < (size-2); i++)
		{
			sumde += pdata[i];
		}
		sumen = (uint16_t)(pdata[size - 2] << 8 | pdata[size - 1]);
		if (pdata[size - 3] == 0x55 && sumen == sumde)
			return HAL_OK;
		return HAL_ERROR;
	}
	/*------------------------------------file of end------------------------------------*/
