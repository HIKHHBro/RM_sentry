/**
	|------------------------------ Copyright -----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,海康平头哥,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                         -                                  |
	|          By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : bsp_usart.c                                                
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
	|------------------------------declaration of end----------------------------|
 **/
#include "bsp_usart.h"
#define A_FRAME_CHECK_LEN 3//一帧接收缓存长度
/* ----------------- 结构体地址列表 -------------------- */
usartDataStrcut *pusart1_t = NULL;
usartDataStrcut *pusart2_t = NULL;
usartDataStrcut *pusart3_t = NULL;
usartDataStrcut *pusart6_t = NULL;
// xQueueHandle usart1_queue = NULL;
// xQueueHandle usart2_queue = NULL;
// xQueueHandle usart3_queue = NULL;
// xQueueHandle usart6_queue = NULL;
/**
	* @Data    2019-01-15 15:31
	* @brief   串口初始化
	* @param	 usartx(x=1~6)
	* @param   uint8_t frame_size 一帧的长度
	* @param   on_off   DISABLE = 0U, ENABLE = 1U
	* @retval  HAL Status
	*/
HAL_StatusTypeDef UsartAndDMAInit(UART_HandleTypeDef *huartx,uint8_t frame_size\
																															,uint8_t on_off)
{
	usartDataStrcut *addr;
	/* -------- 对外接口数据获取 --------- */
	if(AllocateUsartxSpace(huartx) != HAL_OK)//用户串口结构体空间分配
	{
		free(addr);
		return HAL_ERROR;
	}
	addr = GetUsartAddr(huartx); //获取相应用户串口结构体地址
	addr->usart_queue = NULL; 
	addr->rx_buff_size = frame_size; //得到一帧的长度
	addr->rx_on_off = on_off;//接收开关
	/* ------ 分配接收缓存地址空间 ------- */
	addr->rx_buff_data = (uint8_t *)malloc(addr->rx_buff_size * sizeof(uint8_t));
	/* -------- 使能 --------- */
	__HAL_UART_ENABLE_IT(huartx, UART_IT_IDLE);					 //使能串口中断
	if(UsartQueueCreate(addr,5,addr->rx_buff_size) != HAL_OK) //创建队列
	{
		//添加错误机制
		free(addr->rx_buff_data);
		free(addr);
		return HAL_ERROR;
	}
	return HAL_OK;
}
/**
	* @Data    2019-01-15 18:58
	* @brief   串口用户中断回调函数
	* @param   huartx(x=1~6) 用户串口结构体地址
	* @retval  void
	*/
	void UserUsartCallback(UART_HandleTypeDef *huartx)
	{
		NoLenRXforUsart(huartx);
	}
/**
	* @Data    2019-01-15 19:07
	* @brief   任意字节接收
	* @param   usartx(x=1~6) 用户串口结构体地址
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef NoLenRXforUsart(UART_HandleTypeDef *huartx)
	{
		usartDataStrcut *addr = NULL;
		addr = GetUsartAddr(huartx); //获取相应用户串口结构体地址
		if(addr == NULL)
			return HAL_ERROR;
		if(addr->rx_on_off != ENABLE)
			return HAL_ERROR;
		__HAL_UART_CLEAR_IDLEFLAG(huartx);
//       HAL_UART_DMAStop(huartx);
		/* -------- 接收rx_buff_len个字节 --------- */
		if(huartx->hdmarx->Instance->NDTR== A_FRAME_CHECK_LEN)
		{
			/* -------- 设置校验值 --------- */
			if(RCREncryption(addr->rx_buff_data, addr->rx_buff_size) != HAL_OK)
				return HAL_ERROR;
			xQueueSendToBackFromISR(addr->usart_queue, addr->rx_buff_data,0);
		}
    /* ------ 设置接收地址和数据长度 ------- */
    HAL_UART_Receive_DMA(huartx,addr->rx_buff_data,addr->rx_buff_size);
    return HAL_OK;
   }
/**
	* @Data    2019-01-16 10:24
	* @brief   队列创建
	* @param   void
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef UsartQueueCreate(usartDataStrcut *usartx,uint8_t len,\
																														uint8_t deep)
	{
		usartx->usart_queue = xQueueCreate(len,deep);//创建深度len长度21队列
		if(usartx->usart_queue == NULL)
			return HAL_ERROR;
		return HAL_OK;
	}
/**
	* @Data    2019-01-16 10:54
	* @brief   分配相应串口类型数据的空间
	* @param   huartx （x=1,2,3,6）
	* @retval  HAL Status
	*/
HAL_StatusTypeDef AllocateUsartxSpace(UART_HandleTypeDef *huartx)
{
	if (huartx->Instance == USART1) 
	{
		pusart1_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart1_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
	} 
	else if (huartx->Instance == USART2) 
	{
		pusart2_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart2_t== NULL)
		return HAL_ERROR;
		return HAL_OK;
	} 
	else if (huartx->Instance == USART3) 
	{
		pusart3_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart3_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
	} 
	else if (huartx->Instance == USART6) 
	{
		pusart6_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart6_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
	}
	else	return HAL_ERROR;
}
/**
	* @Data    2019-01-16 11:08
	* @brief   自动判别串口类型获取相应用户串口结构体地址
	* @param   huartx （1,2,3,6）
	* @retval  usartDataStrcut* 用户串口结构体指针
	*/
	usartDataStrcut* GetUsartAddr(UART_HandleTypeDef *huartx)
	{
		if (huartx->Instance == USART1) 
		{
			return pusart1_t;
		} 
		else if (huartx->Instance == USART2) 
		{
			return pusart2_t;
		} 
		else if (huartx->Instance == USART3) 
		{
			return pusart3_t;
		} 
		else if (huartx->Instance == USART6) 
		{
			return pusart6_t;
		}
		else	return NULL;
	}

/**
	* @Data    2019-01-16 15:22
	* @brief   队列接收
	* @param   huartx（1,2,3,6）
	* @param 	 pvBuffer 接收数据地址
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef UserUsartQueueRX(UART_HandleTypeDef *huartx,\
																		 void* const pvBuffer)
	{
//				portBASE_TYPE xStatus;
		usartDataStrcut *addr;
		addr = GetUsartAddr(huartx);
//				xStatus = 
		xQueueReceive(addr->usart_queue, pvBuffer, 1);
//				if(pdPASS != xStatus)
//				{
//					return HAL_ERROR;
//				}
		return HAL_OK;
	}
/*-----------------------------------file of end------------------------------*/
