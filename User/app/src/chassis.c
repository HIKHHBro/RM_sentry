/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,海康平头哥,                       |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-01-19               
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
#include "chassis.h"
	static chassisStruct chassis_t;
    static RM3508Struct wheel1_t;         //轮子1
      static speedPidStruct wheel1Speed_t;
    static RM3508Struct wheel2_t;         //轮子2
      static speedPidStruct wheel2Speed_t;
	#define WHEEL1_RX_ID 0x201
	#define WHEEL2_RX_ID 0x202
	#define CHASSIS_CAN_TX_ID 0x200
	/**
	* @Data    2019-01-27 17:09
	* @brief   底盘结构体数据初始化
	* @param  CAN_HandleTypeDef* hcanx（x=1,2）
	* @retval  void
	*/
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc)
	{
		chassis_t.hcanx = hcan;
    chassis_t.rc_t =  rc;
    chassis_t.status = 0;
		/* ------ 轮子1结构体数据初始化 ------- */
		chassis_t.pwheel1_t = &wheel1_t;
			wheel1_t.id = WHEEL1_RX_ID;//电机can的 ip
			wheel1_t.target = 0;		 //目标值
			wheel1_t.tem_target = 0;//临时目标值
			wheel1_t.real_current = 0; //真实电流
			wheel1_t.real_angle = 0;//真实角度
			wheel1_t.tem_angle = 0;//临时角度
  	  wheel1_t.real_speed = 0;//真实速度
  	  wheel1_t.tem_speed = 0;//真实速度
			wheel1_t.zero = 0;			 //电机零点
			wheel1_t.Percentage = 0;//转换比例（减速前角度:减速后的角度 = x:1
			wheel1_t.thresholds = 0; //电机反转阀值
  	  wheel1_t.error = 0;//当前误差
      wheel1_t.ppostionPid_t = NULL;
      wheel1_t.pspeedPid_t = &wheel1Speed_t;
				wheel1Speed_t.kp = 0;
				wheel1Speed_t.kd = 0;
				wheel1Speed_t.ki = 0;
				wheel1Speed_t.error = 0;
				wheel1Speed_t.last_error = 0;//上次误差
				wheel1Speed_t.before_last_error = 0;//上上次误差
				wheel1Speed_t.integral_er = 0;//误差积分
				wheel1Speed_t.pout = 0;//p输出
				wheel1Speed_t.iout = 0;//i输出
				wheel1Speed_t.dout = 0;//k输出
				wheel1Speed_t.pid_out = 0;//pid输出
		/* ------ 轮子2结构体数据初始化 ------- */
		chassis_t.pwheel2_t = &wheel2_t;
			wheel2_t.id = WHEEL2_RX_ID;//电机can的 ip
			wheel2_t.target = 0;		 //目标值
			wheel2_t.tem_target = 0;//临时目标值
			wheel2_t.real_current = 0; //真实电流
			wheel2_t.real_angle = 0;//真实角度
			wheel2_t.tem_angle = 0;//临时角度
			wheel2_t.real_speed = 0;//真实速度
			wheel2_t.tem_speed = 0;//真实速度
			wheel2_t.zero = 0;			 //电机零点
			wheel2_t.Percentage = 0;//转换比例（减速前角度:减速后的角度 = x:1
			wheel2_t.thresholds = 0; //电机反转阀值
			wheel2_t.error = 0;//当前误差
 			wheel2_t.ppostionPid_t = NULL;
      wheel2_t.pspeedPid_t = &wheel2Speed_t;
				wheel2Speed_t.kp = 0;
				wheel2Speed_t.kd = 0;
				wheel2Speed_t.ki = 0;
				wheel2Speed_t.error = 0;
				wheel2Speed_t.last_error = 0;//上次误差
				wheel2Speed_t.before_last_error = 0;//上上次误差
				wheel2Speed_t.integral_er = 0;//误差积分
				wheel2Speed_t.pout = 0;//p输出
				wheel2Speed_t.iout = 0;//i输出
				wheel2Speed_t.dout = 0;//k输出
				wheel2Speed_t.pid_out = 0;//pid输出

        SET_BIT(chassis_t.status,INIT_OK);//初始化成功
	}
/**
	* @Data    2019-01-28 11:40
	* @brief   底盘数据解析
	* @param   void
	* @retval  void
	*/
	void ChassisParseDate(uint16_t id,uint8_t *data)
	{
		
		switch (id)
		{
			case WHEEL1_RX_ID:
				RM3508ParseData(&wheel1_t,data);
				break;
			case WHEEL2_RX_ID:
				RM3508ParseData(&wheel2_t,data);
				break;
			default:
				break;
		}
    SET_BIT(chassis_t.status,RX_OK);//接受成功
	}
/**
	* @Data    2019-02-15 15:10
	* @brief   底盘数据发送
	* @param   void
	* @retval  void
	*/
	void ChassisCanTx(int16_t w1,int16_t w2)
	{
		uint8_t s[8]={0};
		s[0] = (uint8_t)(w1<<8);
		s[1] = (uint8_t)w1;
		s[2] = (uint8_t)(w2<<8);
		s[3] = (uint8_t)w2;
		CanTxMsg(chassis_t.hcanx,CHASSIS_CAN_TX_ID,s);
	}
/**
	* @Data    2019-03-12 16:25
	* @brief   底盘控制函数
	* @param   void
	* @retval  void
	*/
	void ChassisControl(void)
	{
		
	}
 /*
* @Data    2019-02-24 11:59
* @brief   获取底盘结构体地址
* @param   void
* @retval  void
*/
const chassisStruct* GetChassisStructAddr(void)
{
  return &chassis_t;
}
  /**
  * @Data    2019-03-13 03:48
  * @brief   获取底盘结构体地址 可读写，不能乱调用
  * @param   void
  * @retval  void
  */
  chassisStruct *RWGetChassisStructAddr(void)
  {
    return &chassis_t;
  }
  /**
  * @Data    2019-03-13 01:55
  * @brief   获取底盘状态
  * @param   void
  * @retval  void
  */
  uint8_t GetChassisStatus(void)
  {
    return chassis_t.status;
  }
  /**
  * @Data    2019-03-13 02:56
  * @brief   设置电机目标速度
  * @param   void
  * @retval  void
  */
  void SetMotorTarget(int16_t w1,int16_t w2)
  {
    	wheel1_t.target = w1;
      wheel2_t.target = w2;
  }
/*----------------------------------file of end-------------------------------*/
