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
	|--FileName    : photoelectric_sensor.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-12               
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
#include "photoelectric_sensor.h" 
 /*---------------------------------80字符限制-----------------------------------*/
	 /**
	 * @Data    2019-01-12 13:24
	 * @brief   光电初始化
	 * @param   光电结构体
	 * @retval  HAL_StatusTypeDef
	 */
	 HAL_StatusTypeDef PhotoelectricInit(photoelectricStruct* ps,CAN_HandleTypeDef* hcan)
	 {
		 if(ps == NULL)
		 {
			 	return HAL_ERROR;
		 }
		 
		 ps->back = 0;
		 ps->front = 0;
		 ps->right = 0;
		 ps->left = 0;
		 ps->hcanx = hcan;
		 ps->p_addr_cache = NULL;
		 return HAL_OK;
	 }

	/*---------------------------------80字符限制-----------------------------------*/
		/**
		* @Data    2019-01-12 13:15
		* @brief   光电数据解析
		* @param   void
		* @retval  void
		*/
		HAL_StatusTypeDef PhotoelectricGetInfo(photoelectricStruct* ps)
		{
				//要加判断帧率
				CanRxMsgTypeDef* rx;
				CACHE_ADDR(rx,ps->hcanx->pRxMsg); //得到can接收结构体地址
			switch (rx->StdId)
			{
				case FRONT:
					ps->front = (int16_t)(rx->Data[0]<<8)| (rx->Data[1]);
					break;
				case BLACK:
					ps->back = (int16_t)(rx->Data[0]<<8)| (rx->Data[1]);
					break;
				case RIGHT:
					ps->right = (int16_t)(rx->Data[0]<<8)| (rx->Data[1]);
					break;
			 case LEFT:
					ps->left = (int16_t)(rx->Data[0]<<8)| (rx->Data[1]);
					break;
				default:
					break;
			}
			FREE_ADDR(rx); //释放地址
			return HAL_OK;
		}
/*------------------------------------file of end-------------------------------*/


