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
	|--FileName    : bsp_can.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-18               
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
#include "bsp_can.h" 
/* ----------------- 结构体地址列表 -------------------- */
canDataStrcut *pcan1_t = NULL;
canDataStrcut *pcan2_t = NULL;
/* -------------------------------- begin -------------------------------- */
/**
  * @brief  can配置初始化
  * @param  hcanx (x=1,2)
  * @retval HAL Status
 **/
/* -------------------------------- end -------------------------------- */
HAL_StatusTypeDef UserCanConfig(CAN_HandleTypeDef* hcanx)   
{
	canDataStrcut *addr;
	addr = GetCantAddr(hcanx);
	if(AllocateCanxSpace(hcanx) != HAL_OK)//用户can结构体空间分配
	{
		free(addr);
		return HAL_ERROR;
	}
	addr->can_queue = NULL;
	if(CanFilterInit(hcanx) != HAL_OK)
		return HAL_ERROR;
	if(CanTxInit(hcanx) != HAL_OK)
		return HAL_ERROR;
	if(CanRxInit(hcanx) != HAL_OK)
		return HAL_ERROR;
	if(CanQueueCreate(addr,5,16) != HAL_OK) //创建队列
	{
		//添加错误机制
		free(addr);
		return HAL_ERROR;
	}
	 HAL_CAN_Start(hcanx);
	 HAL_CAN_ActivateNotification(hcanx, CAN_IT_RX_FIFO0_MSG_PENDING); //开启中断
	 return HAL_OK;
}

/* -------------------------------- begin -------------------------------- */
/**
  * @brief  can邮箱配置
  * @param CAN_HandleTypeDef *hcanx(x=1,2)
  * @retval HAL Status
 **/
/* -------------------------------- end -------------------------------- */
HAL_StatusTypeDef CanFilterInit(CAN_HandleTypeDef* hcanx)
{
	canDataStrcut *addr;
	addr = GetCantAddr(hcanx);
	addr->filter.FilterIdHigh = 0x0000;									 //要过滤的ID高位
	addr->filter.FilterIdLow = 0x0000;										 //要过滤的ID低位
	addr->filter.FilterMaskIdHigh = 0x0000;							 //过滤器高16位每位必须匹配
	addr->filter.FilterMaskIdLow = 0x0000;								 //过滤器低16位每位必须匹配
	addr->filter.FilterFIFOAssignment = CAN_FILTER_FIFO0; //过滤器被关联到FIFO 0
	addr->filter.FilterBank = 0;
	addr->filter.FilterScale = CAN_FILTERSCALE_32BIT;
	addr->filter.FilterMode = CAN_FILTERMODE_IDMASK;
	addr->filter.FilterActivation = ENABLE; //使能过滤器
	addr->filter.SlaveStartFilterBank = 14;
	return (HAL_CAN_ConfigFilter(hcanx, &addr->filter)); //使能筛选器
}


/* -------------------------------- begin -------------------------------- */
/**
  * @brief  can 发送结构体初始化
  * @param  CanTxMsgTypeDef* TxMsg
  * @retval HAL Status
 **/
/* -------------------------------- end -------------------------------- */
HAL_StatusTypeDef CanTxInit(CAN_HandleTypeDef* hcanx)
{
	canDataStrcut *addr;
	addr = GetCantAddr(hcanx);//获取相应用户can结构体地址
	if(addr == NULL)
	{
		return HAL_ERROR;
	}
	addr->txMsg.StdId = 0x000; //设置id
	addr->txMsg.IDE = CAN_ID_STD; //选择标准id
	addr->txMsg.RTR = CAN_RTR_DATA; //0为数据帧，1为远程帧
	addr->txMsg.DLC = 8; //设置数据长度为8个字节
	addr->txdata[0] = 0x00; //发送数据位都清零
	addr->txdata[1] = 0x00;
	addr->txdata[2] = 0x00;
	addr->txdata[3] = 0x00;
	addr->txdata[4] = 0x00;
	addr->txdata[5] = 0x00;
	addr->txdata[6] = 0x00;
	addr->txdata[7] = 0x00;
	return HAL_OK;
}


/* -------------------------------- begin -------------------------------- */
/**
  * @brief  can 接受结构体初始化
  * @param  CanRxMsgTypeDef* RxMsg
  * @retval void
 **/
/* -------------------------------- end -------------------------------- */
HAL_StatusTypeDef CanRxInit(CAN_HandleTypeDef* hcanx)
{
		canDataStrcut *addr;
	  addr = GetCantAddr(hcanx); //获取相应用户can结构体地址
		if(addr == NULL)
		{
			return HAL_ERROR;
		}
    addr->rxMsg.StdId=0x00;
    addr->rxMsg.ExtId=0x00;
    addr->rxMsg.DLC=0x00;
    addr->rxdata[0] = 0x00; //接收数据位都清零
    addr->rxdata[1] = 0x00;
    addr->rxdata[2] = 0x00;
    addr->rxdata[3] = 0x00;
    addr->rxdata[4] = 0x00;
    addr->rxdata[5] = 0x00;
    addr->rxdata[6] = 0x00;
    addr->rxdata[7] = 0x00;
		return HAL_OK;
}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-16 10:54
	* @brief   分配相应can类型数据的空间
	* @param   hcanx （x=1,2）
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef AllocateCanxSpace(CAN_HandleTypeDef *hcanx)
	{
		
			if (hcanx->Instance == CAN1) 
			{
				pcan1_t	= (struct canDataStrcut*)malloc(sizeof(struct canDataStrcut));
				if(pcan1_t == NULL)
				return HAL_ERROR;
				return HAL_OK;
			} 
			else if (hcanx->Instance == CAN2) 
			{
				pcan2_t	= (struct canDataStrcut*)malloc(sizeof(struct canDataStrcut));
				if(pcan2_t== NULL)
				return HAL_ERROR;
				return HAL_OK;
			}
			else	return HAL_ERROR;
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-16 11:08
	* @brief   自动判别can类型获取相应用户can结构体地址
	* @param   hcanx （x=1,2）
	* @retval  canDataStrcut* 用户串口结构体指针
	*/
	canDataStrcut* GetCantAddr(CAN_HandleTypeDef *hcanx)
	{
		if(hcanx->Instance == CAN1)
		{
			return pcan1_t;
		} 
		else if(hcanx->Instance == CAN2)  
		{
			return pcan2_t;
		} 
		else	return NULL;
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-19 00:58
	* @brief   can接受回调函数
	* @param   void
	* @retval  void
	*/
	void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
	{
		canDataStrcut *addr;
	  addr = GetCantAddr(hcan); //获取相应用户can结构体地址
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&addr->rxMsg,addr->rxdata);
		 memcpy(addr->queue_data,&addr->rxdata,8);
		xQueueSendToBackFromISR(addr->can_queue,addr->queue_data, 0);
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-19 00:58
	* @brief   can接受中断队列创建
	* @param   void
	* @retval  void
	*/
	HAL_StatusTypeDef CanQueueCreate(canDataStrcut *canx,uint8_t len,uint8_t deep)
	{
		canx->can_queue = xQueueCreate(len,deep);//创建深度len长度20队列
		if(canx->can_queue == NULL)
			return HAL_ERROR;
		return HAL_OK;
	}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-16 15:22
	* @brief   队列接收
	* @param   hcanx（1,2,3,6）
	* @param 	 pvBuffer 接收数据地址
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef UserCanQueueRX(CAN_HandleTypeDef *hcanx,void* const pvBuffer)
	{
//				portBASE_TYPE xStatus;
		canDataStrcut *addr;
		addr = GetCantAddr(hcanx);
//				xStatus = 
		xQueueReceive(addr->can_queue, pvBuffer, 1);
//				if(pdPASS != xStatus)
//				{
//					return HAL_ERROR;
//				}
		return HAL_OK;
	}
/*------------------------------------file of end-------------------------------*/


