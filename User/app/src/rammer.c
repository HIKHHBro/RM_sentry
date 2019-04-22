/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,海康平头哥,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : rammer.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-03-16             
	|--Libsupports : 
	|--Description :                                                     
	|--FunctionList                                                     
	|-------1. ....                                                     
	|          <version>:                                                     
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-----------------------------declaration of end-----------------------------|
 **/
#include "rammer.h" 
	M2006Struct rammer_t;//拨弹电机结构体
	postionPidStruct rammerOuterLoopPid_t;//拨弹电机外环pid
	speedPidStruct rammerInnerLoopPid_t;//拨弹电机内环pid
/* -------------- 变量声明 ----------------- */
	 int16_t stuct_lock_count = 0;
   int16_t stuct_count = 0;
  int16_t currer = 0;
  
  
  	int16_t RAMMER_TIME    =         100 ;// 1s 拨弹间隔  时间基数为10ms
  int16_t LOCK_ROTOT_TIME ; // =     3 * RAMMER_TIME; //3s 堵转时间
	M2006Struct* RammerInit(void)
	{
				/* ------ 拨弹电机初始化 ------- */
			rammer_t.id = RAMMER_RX_ID;//电机can的 ip
		  rammer_t.target = 0; //目标值
		  rammer_t.real_current = 0;//真实电流
		  rammer_t.real_angle = 0;//真实角度
		  rammer_t.real_speed = 0;//真实速度
			rammer_t.error = 0;
    rammer_t.coefficient =0;
    rammer_t.last_real =0;
			/* ------ 外环pid参数 ------- */
				rammer_t.ppostionPid_t = &rammerOuterLoopPid_t;
				rammerOuterLoopPid_t.kp = 0;
				rammerOuterLoopPid_t.kd = 0;
				rammerOuterLoopPid_t.ki = 0;
				rammerOuterLoopPid_t.error = 0;
				rammerOuterLoopPid_t.last_error = 0;//上次误差
				rammerOuterLoopPid_t.integral_er = 0;//误差积分
				rammerOuterLoopPid_t.pout = 0;//p输出
				rammerOuterLoopPid_t.iout = 0;//i输出
				rammerOuterLoopPid_t.dout = 0;//k输出
				rammerOuterLoopPid_t.pid_out = 0;//pid输出
        rammerOuterLoopPid_t.motor_lim = RM2006_LIM;//2006最大电流范围
			/* ------ 内环pid参数 ------- */
				rammer_t.pspeedPid_t = &rammerInnerLoopPid_t;
				rammerInnerLoopPid_t.kp = 0;
				rammerInnerLoopPid_t.kd = 0;
				rammerInnerLoopPid_t.ki = 0;
				rammerInnerLoopPid_t.error = 0;
				rammerInnerLoopPid_t.last_error = 0;//上次误差
				rammerInnerLoopPid_t.before_last_error = 0;//上上次误差
				rammerInnerLoopPid_t.integral_er = 0;//误差积分
				rammerInnerLoopPid_t.pout = 0;//p输出
				rammerInnerLoopPid_t.iout = 0;//i输出
				rammerInnerLoopPid_t.dout = 0;//k输出
				rammerInnerLoopPid_t.pid_out = 0;//pid输出
        rammerInnerLoopPid_t.motor_lim = RM2006_LIM;//2006最大电流范围
				return &rammer_t;
	}
	/**
	* @Data    2019-02-14 21:01
	* @brief   拨弹控制
	* @param   void
	* @retval  void
	*/
	void RammerControl(void)
	{
    LOCK_ROTOT_TIME  =     3 * RAMMER_TIME; //3s 堵转时间
     if((rammer_t.real_current < STUCK_BULLET_THRE) &&((rammer_t.real_current > (-STUCK_BULLET_THRE))||(rammer_t.real_current == (-STUCK_BULLET_THRE))))
      {
        if(stuct_count < RAMMER_TIME)//500ms
        stuct_count ++;
        stuct_lock_count =-1;
      } 
     else if(rammer_t.real_current > STUCK_BULLET_THRE )
      {
        if(stuct_lock_count > -LOCK_ROTOT_TIME)//500ms
        stuct_lock_count --;
      }
     else if(rammer_t.real_current < -STUCK_BULLET_THRE)
     {
//       RammerShake();
     }
    if(stuct_count >= RAMMER_TIME)
      {
        rammer_t.target = PCycleNumerical( rammer_t.target);
        stuct_count =0;
      }
      else if(stuct_lock_count <= (-LOCK_ROTOT_TIME))
      {
        rammer_t.target = MCycleNumerical( rammer_t.target);
        stuct_lock_count =-1;
      }
	}
	/**
		* @Data    2019-03-16 19:43
		* @brief   正数数值正向循环，软件重转载
		* @param   void
		* @retval  void
		*/
		int16_t PCycleNumerical(int16_t data)
		{
			return ((data+PARTITION_NUMB) % M2006_POLES);
		}
	/**
		* @Data    2019-03-16 19:43
		* @brief   正数数值反向循环，软件重转载//看M2006_POLES应该取什么
		* @param   void
		* @retval  void
		*/
		int16_t MCycleNumerical(int16_t data)
		{
			return ((M2006_POLES-1) - (((M2006_POLES-1)-data) + PARTITION_NUMB) % M2006_POLES);
		}
  // /**
  // * @Data    2019-03-17 00:49
  // * @brief   拨弹大力气抖动
  // * @param   void
  // * @retval  void
  // */
  // void RammerShake(void)
  // {
  //   static int16_t icr=0,i=0;
  //   icr = icr*10;
  //   while (i) 
  //   {
      
  //   }
  //   if((rammer_t.real_current >STUCK_BULLET_THRE) || (rammer_t.real_current < -STUCK_BULLET_THRE))
  //   {
  //     if(i == 0)
  //      rammer_t.real_angle = PCycleNumerical(rammer_t.real_angle);
  //     else
  //     {
  //       rammer_t.real_angle = MCycleNumerical(rammer_t.real_angle);
  //     }
      
  //   }
  // }
/**
	* @Data    2019-03-26 15:34
	* @brief   拨弹pid设置
	* @param   uint8_t speed 拨弹速度一秒x发
	* @retval  void
	*/
	void SetRammerPID(uint8_t speed)
	{
    if(speed == 0)
    {
      rammer_t.target = rammer_t.real_angle;
    }
    else 
         RAMMER_TIME = (int16_t)(100/(float)(speed));
//				rammerOuterLoopPid_t.kp = 0;
//				rammerOuterLoopPid_t.kd = 0;
//				rammerOuterLoopPid_t.ki = 0;
//				rammerInnerLoopPid_t.kp = 0;
//				rammerInnerLoopPid_t.kd = 0;
//				rammerInnerLoopPid_t.ki = 0;
	}
  
int16_t hot_flag=0;
  int16_t speed_fri =1330;
void Shoot(uint8_t speed,uint8_t buffer_on)
{
  if(speed ==0)
  {
    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_SPEED-100);
    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_SPEED -100);
    SetRammerPID(0);
    HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);

  }
  else if(speed >0)
  {
    if(speed >8&& buffer_on ==1)
    {
      if(hot_flag > 100)
     buffer_on =0;
      hot_flag++;
    }
    else if(speed >12&& buffer_on ==0)
    {
      speed =12;
    }
    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,speed_fri);
    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,speed_fri);
         HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
    SetRammerPID(speed);
  }
}

//测试版加减速
//int16_t line_d =100;
//float line_k =0.02;
//int16_t Addmspeed(int16_t speed,int16_t k)
//{
//  int16_t y;
//  if(k >0)
//  {
//    if(speed <300)
//    y = speed +20;
//  }
//}
/*-----------------------------------file of end------------------------------*/


