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
	|--FileName    : motor.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-12-31               
	|--Libsupports : 标准库和HAL库
	|--Description : 1、maxion电机+RoboModule驱动 
	|								 2、3508电机+c610电调
	|								 3、6623电机
	|                4、6002电机
	|								 5、                                                        
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end----------------------------|
 **/
#include "motor.h" 
///* ----------------- maxion -------------------- */
//	/*---------------------------------80字符限制-----------------------------------*/
//		/**
//		* @Data    2019-01-04 12:45
//		* @brief  复位指令 
//		* @param   Group   取值范围 0-7
//		* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//		* @retval  HAL_StatusTypeDef
//		*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Reset(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number)
//{
//    unsigned short can_id = 0x000;
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
//    
//    hcan->pTxMsg->Data[0] = 0x55;
//    hcan->pTxMsg->Data[1] = 0x55;
//    hcan->pTxMsg->Data[2] = 0x55;
//    hcan->pTxMsg->Data[3] = 0x55;
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//  
//    return (HAL_CAN_Transmit(hcan,200));
////    while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:15
//	* @brief   模式选择指令
//	* @param   Group   取值范围 0-7
//	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	 Mode   见motor  motorModeEnum枚举
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Mode_Choice(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,unsigned char Mode)
//{
//    unsigned short can_id = 0x001;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
//    
//    hcan->pTxMsg->Data[0] = Mode;
//    hcan->pTxMsg->Data[1] = 0x55;
//    hcan->pTxMsg->Data[2] = 0x55;
//    hcan->pTxMsg->Data[3] = 0x55;
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:24
//	* @brief   开环模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	 temp_pwm的取值范围如下：
//						-5000 ~ +5000，满值5000，其中temp_pwm = ±5000时，最大输出电压为电源电压
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_OpenLoop_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_PWM)
//{
//    unsigned short can_id = 0x002;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

//    if(Temp_PWM > 5000)
//    {
//        Temp_PWM = 5000;
//    }
//    else if(Temp_PWM < -5000)
//    {
//        Temp_PWM = -5000;
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
//    hcan->pTxMsg->Data[2] = 0x55;
//    hcan->pTxMsg->Data[3] = 0x55;
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:25
//	* @brief   电流模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param 	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param 	 temp_pwm的取值范围如下：
//							0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压
//	* @param 	 temp_current的取值范围如下：
//							-32768 ~ +32767，单位mA
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current)
//{
//    unsigned short can_id = 0x003;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

//    if(Temp_PWM > 5000)
//    {
//        Temp_PWM = 5000;
//    }
//    else if(Temp_PWM < -5000)
//    {
//        Temp_PWM = -5000;
//    }
//    
//    if(Temp_PWM < 0)
//    {
//        Temp_PWM = abs(Temp_PWM);
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
//    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Current>>8)&0xff);
//    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Current&0xff);
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:27
//	* @brief   速度模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	 temp_pwm的取值范围如下：
//							 0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压
//	* @param	 temp_velocity的取值范围如下：
//							 -32768 ~ +32767，单位RPM
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Velocity_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity)
//{
//    unsigned short can_id = 0x004;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

//    if(Temp_PWM > 5000)
//    {
//        Temp_PWM = 5000;
//    }
//    else if(Temp_PWM < -5000)
//    {
//        Temp_PWM = -5000;
//    }
//    
//    if(Temp_PWM < 0)
//    {
//        Temp_PWM = abs(Temp_PWM);
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
//    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
//    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:28
//	* @brief   位置模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param	Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	temp_pwm的取值范围如下：
//							0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压
//	* @param	temp_position的取值范围如下：
//							-2147483648~+2147483647，单位qc
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Position_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position)
//{
//    unsigned short can_id = 0x005;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

//    if(Temp_PWM > 5000)
//    {
//        Temp_PWM = 5000;
//    }
//    else if(Temp_PWM < -5000)
//    {
//        Temp_PWM = -5000;
//    }
//    
//    if(Temp_PWM < 0)
//    {
//        Temp_PWM = abs(Temp_PWM);
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
//    hcan->pTxMsg->Data[2] = 0x55;
//    hcan->pTxMsg->Data[3] = 0x55;
//    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
//    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
//    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
//    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:30
//	* @brief   速度位置模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param	Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	temp_pwm的取值范围如下：
//							0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压
//	* @param	temp_velocity的取值范围如下：
//								0 ~ +32767，单位RPM
//	* @param	temp_position的取值范围如下：
//							-2147483648~+2147483647，单位qc
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Velocity_Position_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position)
//{
//    unsigned short can_id = 0x006;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

//    if(Temp_PWM > 5000)
//    {
//        Temp_PWM = 5000;
//    }
//    else if(Temp_PWM < -5000)
//    {
//        Temp_PWM = -5000;
//    }
//    
//    if(Temp_PWM < 0)
//    {
//        Temp_PWM = abs(Temp_PWM);
//    }
//    
//    if(Temp_Velocity < 0)
//    {
//        Temp_Velocity = abs(Temp_Velocity);
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
//    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
//    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
//    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
//    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
//    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
//    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}

///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:31
//	* @brief   电流速度模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	 temp_current的取值范围如下：
//							 0 ~ +32767，单位mA
//	* @param	 temp_velocity的取值范围如下：
//							 -32768 ~ +32767，单位RPM
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Velocity_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity)
//{
//    unsigned short can_id = 0x007;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
//    
//    if(Temp_Current < 0)
//    {
//        Temp_Current = abs(Temp_Current);
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
//    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
//    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:32
//	* @brief   电流位置模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	 temp_current的取值范围如下：
//							 0 ~ +32767，单位mA
//	* @param	 temp_position的取值范围如下：
//							 -2147483648~+2147483647，单位qc
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Position_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position)
//{
//    unsigned short can_id = 0x008;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

//    
//    if(Temp_Current < 0)
//    {
//        Temp_Current = abs(Temp_Current);
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
//    hcan->pTxMsg->Data[2] = 0x55;
//    hcan->pTxMsg->Data[3] = 0x55;
//    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
//    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
//    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
//    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:33
//	* @brief   电流速度位置模式下的数据指令
//	* @param   Group   取值范围 0-7
//	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @param	 temp_current的取值范围如下：
//						 	0 ~ +32767，单位mA
//	* @param	 temp_velocity的取值范围如下：
//						 	0 ~ +32767，单位RPM
//	* @param	 temp_position的取值范围如下：
//						 	-2147483648~+2147483647，单位qc
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Current_Velocity_Position_Mode(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,long Temp_Position)
//{
//    unsigned short can_id = 0x009;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
//    
//    if(Temp_Current < 0)
//    {
//        Temp_Current = abs(Temp_Current);
//    }
//    
//    if(Temp_Velocity < 0)
//    {
//        Temp_Velocity = abs(Temp_Velocity);
//    }
//    
//    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
//    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
//    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
//    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
//    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
//    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
//    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
//    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:35
//	* @brief   配置指令
//	* @param   Temp_Time的取值范围: 0 ~ 255，为0时候，为关闭电流速度位置反馈功能
//	* @param	 Ctl1_Ctl2的取值范围：0 or 1 ，当不为0 or 1，则认为是0，为关闭左右限位检测功能
//						 特别提示：Ctl1，Ctl2的功能仅存在于102 301，其余版本驱动器，Ctl1_Ctl2 = 0 即可
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Config(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2)
//{
//    unsigned short can_id = 0x00A;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    if((Ctl1_Ctl2 != 0x00)&&(Ctl1_Ctl2 != 0x01))
//    {
//        Ctl1_Ctl2 = 0x00;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
//    
//    hcan->pTxMsg->Data[0] = Temp_Time;
//    hcan->pTxMsg->Data[1] = Ctl1_Ctl2;
//    hcan->pTxMsg->Data[2] = 0x55;
//    hcan->pTxMsg->Data[3] = 0x55;
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
///*---------------------------------80字符限制-----------------------------------*/
//	/**
//	* @Data    2019-01-04 13:36
//	* @brief   在线检测
//	* @param   Group   取值范围 0-7
//	* @param	 Number  取值范围 0-15，其中Number==0时，为广播发送
//	* @retval  HAL_StatusTypeDef
//	*/
//HAL_StatusTypeDef CAN_RoboModule_DRV_Online_Check(CAN_HandleTypeDef* hcan,\
//unsigned char Group,unsigned char Number)
//{
//    unsigned short can_id = 0x00F;
//    
//    hcan->pTxMsg->IDE = CAN_ID_STD;    //标准帧
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
//    hcan->pTxMsg->DLC = 0x08;          //帧长度为8
//    
//    if((Group<=7)&&(Number<=15))
//    {
//        can_id |= Group<<8;
//        can_id |= Number<<4;
//    }
//    else
//    {
//        return HAL_ERROR;
//    }
//    
//    hcan->pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
//    
//    hcan->pTxMsg->Data[0] = 0x55;
//    hcan->pTxMsg->Data[1] = 0x55;
//    hcan->pTxMsg->Data[2] = 0x55;
//    hcan->pTxMsg->Data[3] = 0x55;
//    hcan->pTxMsg->Data[4] = 0x55;
//    hcan->pTxMsg->Data[5] = 0x55;
//    hcan->pTxMsg->Data[6] = 0x55;
//    hcan->pTxMsg->Data[7] = 0x55;
//    
//    return (HAL_CAN_Transmit(hcan,200));
//    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
//}
/* ================================== RM6623 of begin ================================= */
static int16_t s_max_motor_lines = 8192 ;//电机一圈最大线数
/*---------------------------------80字符限制-----------------------------------*/
/**
	* @Data    2019-01-18 19:59
	* @brief   RM6623结构体初始化
	* @param   RM6623电机结构体地址
	* @retval  void
	*/
void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx)
{
	RM6623->id = 0;
	RM6623->target = 0;				//目标值
	RM6623->tem_target = 0;		//临时目标值
	RM6623->real_current = 0; //真实电流
	RM6623->real_angle = 0;		//真实角度
	RM6623->tem_angle = 0;		//临时角度
	RM6623->zero = 0;					//电机零点
	RM6623->Percentage = 0;		//转换比例（减速前角度:减速后的角度 = x:1）
	RM6623->hcanx = hcanx;
	/*根据读取时间间隔，估算最大转过线数p，thresholds = p -s_max_motor_lines */
	RM6623->thresholds = 1200 - s_max_motor_lines;//1200是瞎给的，别计较
}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-18 20:03
	* @brief   6623数据解析
	* @param   RM6623电机结构体地址
	* @retval  HAL Status
	*/
void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data)
{
	int16_t tem_angle = 0;
	RM6623->real_current = ((int16_t)(data[4] << 8) | data[5]);
	tem_angle = ((int16_t)(data[0] << 8) | data[1]);
	tem_angle = RatiometricConversion(tem_angle, RM6623->thresholds, RM6623->Percentage);
	RM6623->real_angle = zeroArgument(tem_angle, RM6623->thresholds);
}

/* ================================== RM6623 of end =================================== */
/* ================================== 3508 of begin ================================= */
/*---------------------------------80字符限制-----------------------------------*/
/**
	* @Data    2019-01-18 19:59
	* @brief   RM3508结构体初始化
	* @param   RM3508电机结构体地址
	* @retval  void
	*/
void RM3508StructInit(RM3508Struct *RM3508,CAN_HandleTypeDef *hcanx)
{
		RM3508->id = 0;
		RM3508->target = 0;
		RM3508->real_current = 0;
		RM3508->real_angle = 0;
		RM3508->real_speed = 0;
		RM3508->hcanx = hcanx;
}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-19 00:42
	* @brief   3508数据解析
	* @param   void
	* @retval  void
	*/
	void RM3508ParseData(RM3508Struct *RM3508,uint8_t *data)
	{
		RM3508->real_angle=((int16_t)(data[0]<<8)|data[1]);
    RM3508->real_speed=((int16_t)(data[2]<<8)|data[3]);
    RM3508->real_current=((int16_t)(data[4]<<8)|data[5]);
	}
/* ================================== 3508 of end =================================== */
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-18 20:14
	* @brief   比例转换  
	* @param   real 真实值
	* @param   threshold 一圈最大线数的阀值
	* @param   percentage //转换比例（减速前角度:减速后的角度 = x:1
	* @retval  int16_t 换算后的目标值
	*/
int16_t RatiometricConversion(int16_t real,int16_t threshold,int16_t percentage)
{

		static int32_t last_real,tem=0;
		static uint16_t  coefficient=0;
		if (real - last_real < threshold)
		{
			/*角度系数循环自增,下面转36圈归零系数,设置范围[0,Percentage] */
			 coefficient =(coefficient+1)%(percentage);
		}
		else if(last_real - real < threshold)
		{
			coefficient = (percentage-1)-((((percentage-1) - coefficient)+1)%(percentage));
		}
		last_real = real;//缓存现在值
		tem = real + (s_max_motor_lines* coefficient); //转换总角度
		return ((int16_t)(tem/(percentage)));//换算成上面转一圈
}
/*---------------------------------80字符限制-----------------------------------*/
	/**
	* @Data    2019-01-18 20:48
	* @brief   零点处理
	* @param   real 真实值
	* @param   threshold 一圈最大线数的阀值
	* @retval  int16_t 换算之后的目标值
	*/
	int16_t zeroArgument(int16_t real,int16_t threshold)
	{
		static int16_t last_realdata = 0,counter = 0;
		if(real - last_realdata < threshold) 
			counter = 1;
		if(last_realdata - real <threshold)
			counter = -1;
			else counter = 0;
		last_realdata = real;
		return (real + (s_max_motor_lines * counter));
	}


	/*------------------------------------file of end-------------------------------*/
