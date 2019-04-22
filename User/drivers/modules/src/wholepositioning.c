/**
	|-------------------------------- Copyright -----------------------------------|
	|                                                                              |
	|                        (C) Copyright 2018,海康平头哥,                         |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                               |
	|                                                                              |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
	|                     https://github.com/GCUWildwolfteam                       |
	|------------------------------------------------------------------------------|
	|--FileName    : wholepositioning.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-29    
	|-- Libsupports: STM32CubeF1 Firmware Package V1.6.0 / 17-May-2017(用别的库出问
	|								 题别问我)           
	|--Description : 1、采用串口空闲中断+DMA接收不定长度数据
	|								 2、调用WholePositioningInit申请内存和初始化
	|								 3、调用WholePositioningStart开启接收并解析数据                                                     
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end----------------------------|
 **/
#include "wholepositioning.h" 
#if defined(WHOLE_POSITION_UART)
/* -------- 变量声明 --------- */
	  uint8_t buffer[31]={0};	
	 static wholePositionStruct last_wholeositiontruct_t; //缓存数据 
	/**
	* @Data    2018-12-29 11:11
	* @brief   全场定位初始化 
	* @param   1.全场定位结构体
	* @param   2.串口类型
	* @retval  HAL_StatusTypeDef
	*/
	HAL_StatusTypeDef WholePositioningInit(wholePositionStruct* wps,UART_HandleTypeDef* huart)
	{

	/* -------- 检测缓存空间是否分配成功 --------- */		
		if(wps == NULL)
		{
			SET_BIT(e_objects_flag,NO_CREATE_OBJECTS_OF_WHOLEPOSITION); //获取标志
			return HAL_ERROR;
		}
        SET_BIT(e_objects_flag,CREATE_OBJECTS_OF_WHOLEPOSITION);
		wps->p_get_usartx = huart; //识别串口类型，获取串口类型地址
		/* -------- 检测传值 --------- */
		if(wps->p_get_usartx != huart&&wps->p_get_usartx ==NULL)
		{
			SET_BIT(e_periphera_error_flag,GET_USART_ADDR_FAIL);
			return HAL_ERROR;
		}
		wps->z_angle = 0;							//z轴角度															
		wps->x_angle = 0;							//x轴角度
		wps->y_angle = 0;							//y轴角度
		wps->x_coordinate = 0; 				//x轴角度
		wps->y_coordinate = 0; 				//y轴角度
		wps->angular_acceleration = 0;	//
		wps->C_X_Coordinate = 0;
		wps->C_Y_Coordinate = 0;
		wps->C_Z_Angle = 0;
		wps->p_beffur = buffer;
		wps->p_lastdata_t = &last_wholeositiontruct_t;
		wps->rx_flag = 0;
		wps->status = 0; //0代表开启成功 非零代表开启失败
	
		 if(WholePositioningSetConfig(huart) != HAL_OK)
		 {
			 	return HAL_ERROR;
		 }
		if( UsartAndDMAInit(huart,31,ENABLE)!=HAL_OK)
		 	{
				return HAL_ERROR;
			}
		 return HAL_OK;
	}
  /**
	* @Data    2018-12-29 10:36
	* @brief   全场定位启动初始化
	* @param   UART_HandleTypeDef* huart 串口指针
	* @retval  void
	*/
	HAL_StatusTypeDef WholePositioningSetConfig(UART_HandleTypeDef* huart)
	{
		unsigned char text[4] = "ACT0";
		uint8_t i = 0;
		for(i=0;i<101;i++)
			 HAL_UART_Transmit(huart,text,4,20);
			 SET_BIT(e_periphera_interface_flag,WHOLEPOSITOM_CLEAN);
			return HAL_OK;
	}
	/* -------- 变量定义 --------- */
	union
	{
		uint8_t data_u8[24];
		float data_f[6];
	}data_u; //全场定位数据类型转换联合体
  /**
	* @Data    2018-12-29 10:50
	* @brief   解析全场定位数据
	* @param   wholePositionStruct_t* wp
	* @retval  void
	*/
	HAL_StatusTypeDef ParseWholePositioningInfo(wholePositionStruct* wps)
	{
		/* -------- 变量声明 --------- */
			uint8_t *data;  
			uint8_t *status;
			float *data_f[6];
			uint8_t i=0;
		/* -------- 结构体指针传递 --------- */
			data = buffer;
			data_f[0] = &(wps->z_angle);
			data_f[1] = &(wps->x_angle);							
		  data_f[2] = &(wps->y_angle);							
		  data_f[3] = &(wps->x_coordinate); 				
		  data_f[4] = &(wps->y_coordinate); 				
		  data_f[5] = &(wps->angular_acceleration);
			status = &wps->status;
			if(UserUsartQueueRX(wps->p_get_usartx,buffer) == HAL_OK)
			{
				if(data[0]==0x0D&&data[1]==0x0A&&data[26]==0x0A&&data[27]==0x0D)
				{
					for(i = 0;i < 24;i++)
					{
						data_u.data_u8[i] = data[i+2];
					}
					*data_f[0] = data_u.data_f[0];
					*data_f[1] = data_u.data_f[1]; 
					*data_f[2] = data_u.data_f[2];  
					*data_f[3] = data_u.data_f[3];  
					*data_f[4] = data_u.data_f[4];  
					*data_f[5] = data_u.data_f[5]; 
					*status = HAL_OK;
					return HAL_OK;
				}
				else
				{
						*status = HAL_ERROR;
						return HAL_ERROR;
				}
			}
      return HAL_ERROR;
    }
//			*wps->p_lastdata_t = *wps;//保存数据现场

  // /**
	// * @Data    2018-12-30 14:32
	// * @brief   全场定位接收开启
	// * @param   全场定位数据结构指针
	// * @retval  HAL status
	// */
	// HAL_StatusTypeDef WholePositioningStart(wholePositionStruct* wps,UART_HandleTypeDef* huart)
	// {
	// 	if(wps->p_get_usartx != huart)
	// 	{
  //          return HAL_BUSY;
	// 	}
	// 	__HAL_UART_CLEAR_IDLEFLAG(wps->p_get_usartx);
  //       HAL_UART_DMAStop(huart);
	// 		if(wps->p_get_usartx->hdmarx->Instance->NDTR == WHOLE_POSION_BACK_LENFTH)//接收28字节正确
	// 		{
	// 					if(ParseWholePositioningInfo(wps) != HAL_OK) //解码
	// 					{
	// 						SetFrameDropflag(wps->rx_flag); 
	// 					}
	// 		}
	// 		else SetFrameDropflag(wps->rx_flag);
	// 		SetNormalflag(wps->rx_flag);
	// 		HAL_UART_Receive_DMA(wps->p_get_usartx,wps->p_beffur,WHOLE_POSION_LENGTH + WHOLE_POSION_BACK_LENFTH);
	// 		return HAL_OK;


		// if (huart->Instance == USART1) 
		// {
		// 	__HAL_UART_CLEAR_IDLEFLAG(huart);
		// 	if(huart->hdmarx->Instance->CNDTR == WHOLE_POSION_BACK_LENFTH)//接收28字节正确
		// 	{
		// 				ParseWholePositioningInfo(&wps); //解码
		// 	}
		// 	HAL_UART_Receive_DMA(huart,wps->p_beffur,WHOLE_POSION_LENGTH + WHOLE_POSION_LENGTH);
		// 	SET_BIT(e_periphera_interface_flag,USART1_RX_BY_USED);//使用标志位
		// 	return HAL_OK;
		// } 
		// else if (huart->Instance == USART2) 
		// {
		// 	__HAL_UART_CLEAR_IDLEFLAG(huart);
		// 	if(huart->hdmarx->Instance->CNDTR == WHOLE_POSION_BACK_LENFTH)//接收28字节正确
		// 	{
		// 				ParseWholePositioningInfo(&wps); //解码
		// 	}
		// 	HAL_UART_Receive_DMA(huart,wps->p_beffur,WHOLE_POSION_LENGTH + WHOLE_POSION_LENGTH);
		// 	SET_BIT(e_periphera_interface_flag,USART2_RX_BY_USED);//使用标志位
		// 	return HAL_OK;
		// } 
		// else if (huart->Instance == USART3) 
		// {
		// 	__HAL_UART_CLEAR_IDLEFLAG(huart);
		// 	if(huart->hdmarx->Instance->CNDTR == WHOLE_POSION_BACK_LENFTH)//接收28字节正确
		// 	{
		// 				ParseWholePositioningInfo(&wps); //解码
		// 	}
		// 	HAL_UART_Receive_DMA(huart,wps->p_beffur,WHOLE_POSION_LENGTH + WHOLE_POSION_LENGTH);
		// 	SET_BIT(e_periphera_interface_flag,USART3_RX_BY_USED);//使用标志位
		// 	return HAL_OK;
		// } 
		// else if(huart->Instance == UART4)
		// {
		// 	__HAL_UART_CLEAR_IDLEFLAG(huart);
		// 	if(huart->hdmarx->Instance->CNDTR == WHOLE_POSION_BACK_LENFTH)//接收28字节正确
		// 	{
		// 				ParseWholePositioningInfo(&wps); //解码
		// 	}
		// 	HAL_UART_Receive_DMA(huart,wps->p_beffur,WHOLE_POSION_LENGTH + WHOLE_POSION_LENGTH);
		// 	SET_BIT(e_periphera_interface_flag,UART4_RX_BY_USED);//使用标志位
		// 	return HAL_OK;
		// }
		
		// else if (huart->Instance == UART5)
		// {
		// 	__HAL_UART_CLEAR_IDLEFLAG(huart);
		// 	if(huart->hdmarx->Instance->CNDTR == WHOLE_POSION_BACK_LENFTH)//接收28字节正确
		// 	{
		// 				ParseWholePositioningInfo(&wps); //解码
		// 	}
		// 	HAL_UART_Receive_DMA(huart,wps->p_beffur,WHOLE_POSION_LENGTH + WHOLE_POSION_LENGTH);
		// 	SET_BIT(e_periphera_interface_flag,UART5_RX_BY_USED);//使用标志位
		// 	return HAL_OK;
		// }
		// else return HAL_ERROR;
		
//	}


	// uint8_t wholepositioningINit(wholePositionStruct* wps,UART_HandleTypeDef* huart)
	// {
	// 	 	if(wps == NULL)
	// 	{
	// 		SET_BIT(e_objects_flag,NO_CREATE_OBJECTS_OF_WHOLEPOSITION); //获取标志
	// 		return 1;
	// 	}
	// 	wps->p_get_usartx = RecognizeUSARTType(huart); //识别串口类型，获取串口类型地址
	// 	/* -------- 检测串口是否开启 --------- */
	// 	if(wps->p_get_usartx == NULL)
	// 	{
	// 		free(wps); //回收空间
	// 		wps = NULL;
	// 		return 1;
	// 	}
	// 	wps->z_angle = 0;							//z轴角度															
	// 	wps->x_angle = 0;							//x轴角度
	// 	wps->y_angle = 0;							//y轴角度
	// 	wps->x_coordinate = 0; 				//x轴角度
	// 	wps->y_coordinate = 0; 				//y轴角度
	// 	wps->angular_acceleration = 0;	//
	// 	wps->C_X_Coordinate = 0;
	// 	wps->C_Y_Coordinate = 0;
	// 	wps->C_Z_Angle = 0;
	// 	wps->p_beffur = buffer;
	// 	wps->p_lastdata_t = NULL;
	// 	wps->rx_flag = 0;
	
	// 	wps->status = WholePositioningSetConfig(huart);
	// 	__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE );//串口空闲中断使能
	// 	return 0;
	// }
#endif
/*------------------------------------file of end------------------------------------*/


