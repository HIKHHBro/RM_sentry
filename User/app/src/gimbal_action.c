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
	|--FileName    : gimbal_action.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-04-16             
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
#include "gimbal_action.h" 
extern 	gimbalStruct gimbal_t;//云台结构体
/**
	* @Data    2019-02-14 21:01
	* @brief   云台控制
	* @param   void
	* @retval  void
	*/
  		int16_t yaw=0;
    int16_t pitch=0;
    int16_t rammer=0;
    uint32_t commot =0;
	void GimbalAutoControl(void)
	{
    SET_BIT(gimbal_t.status,RUNING_OK);
    commot = ControlDecision();//决策模式
    ControlSwitch(commot);//控制模式切换
    yaw = YawPidControl(gimbal_t.pYaw_t->ppostionPid_t->error);
    pitch = PitchPidControl(gimbal_t.pPitch_t->ppostionPid_t->error);
    rammer = RammerPidControl(gimbal_t.prammer_t->target);
    GimbalCanTx(yaw,pitch,rammer);
	}
/**
* @Data    2019-03-20 21:27
* @brief   云台扫描探索模式目标值
* @param   void
* @retval  void
*/
uint8_t scflag = 0;
  int16_t pitchspeed_= 15;
    int16_t yawspeed_= 8000;
  int16_t scok_up = 1500;
  int16_t scok_down = 2000;
void ScanningToExplore(void)
{
  if((gimbal_t.status&SCAN_MODE_RUNING) != SCAN_MODE_RUNING)
  {
    //目标值切换，状态切换
    gimbal_t.pitch_scan_target = gimbal_t.pPitch_t->real_angle;
    gimbal_t.yaw_scan_target = gimbal_t.pYaw_t->real_angle;
    gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;

    if((gimbal_t.pitch_scan_target) > scok_down && gimbal_t.pPitch_t->target< DOWN_BUFF_POSI) 
    {
      gimbal_t.pitch_scan_target = scok_down;
    }
    else if ((gimbal_t.pitch_scan_target )< scok_up || gimbal_t.pPitch_t->target > UP_BUFF_POSI) 
    {
      gimbal_t.pitch_scan_target = scok_up;
    }
    //清除pc控制标志位，设置标志位
    SET_RUNING_STATUS(SCAN_MODE_RUNING);
    SetGeneralMode();
     Shoot(0,0);
  }
    if(((gimbal_t.pitch_scan_target < scok_down) || (gimbal_t.pitch_scan_target == scok_down)) && (scflag ==0) )
    {
      gimbal_t.pitch_scan_target -= pitchspeed_;
      if(gimbal_t.pitch_scan_target <= scok_up)
      {
        scflag =1;
        gimbal_t.pitch_scan_target = scok_up+1;
      }
    }
    else if(((gimbal_t.pitch_scan_target > scok_up) || (gimbal_t.pitch_scan_target == scok_up)) && (scflag ==1) )
    {
       gimbal_t.pitch_scan_target += pitchspeed_;
     if(gimbal_t.pitch_scan_target >= scok_down)
      {
        scflag =0;
         gimbal_t.pitch_scan_target = scok_down-1;
      }
    }
    //gimbal_t.yaw_scan_target =  ((gimbal_t.yaw_scan_target+yawspeed_) % 20480);
     gimbal_t.pPitch_t->ppostionPid_t->error = CalculateError(gimbal_t.pitch_scan_target,( gimbal_t.pPitch_t->real_angle),6500,(8192));//待测试
     gimbal_t.pYaw_t->ppostionPid_t->error = yawspeed_;//CalculateError(( gimbal_t.yaw_scan_target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//待测试
    
         Shoot(0,0);

}
/**
* @Data    2019-03-20 21:27
* @brief   云台自瞄pc控制模式目标值
* @param   void
* @retval  void
*/
int16_t fri_fre = 12;
float pitchcin = 1;
float  yawcin = 1;
void PcControlMode(void)
{
  if((gimbal_t.status&PC_SHOOT_MODE_RUNING) != PC_SHOOT_MODE_RUNING)
  {
    //目标值切换，状态切换
    SET_RUNING_STATUS(PC_SHOOT_MODE_RUNING);
//    //测试区
//  gimbal_t.pPitch_t->ppostionPid_t->error  = (int16_t)(gimbal_t.pPc_t->pitch_target_angle * pitchcin);
//  gimbal_t.pYaw_t->ppostionPid_t->error = (int16_t)(gimbal_t.pPc_t->yaw_target_angle * pitchcin);
//    if(gimbal_t.pPitch_t->ppostionPid_t->pid_out )
     gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;
    SetPcControlPID();
  //  HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
//        __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_1_START_V+120);
//    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_2_START_V +120);
//    if(gimbal_t.pPitch_t->ppostionPid_t->error - gimbal_t.pPc_t->pitch_target_angle)
  }
  if(gimbal_t.pPc_t->distance < 300)
  Shoot(fri_fre,0);
  else Shoot(0,0);
   gimbal_t.pPitch_t->ppostionPid_t->error  = (int16_t)(gimbal_t.pPc_t->pitch_target_angle * pitchcin);
  gimbal_t.pYaw_t->ppostionPid_t->error = (int16_t)(gimbal_t.pPc_t->yaw_target_angle * pitchcin);
}
/**
* @Data    2019-03-20 21:27
* @brief   控制权切换
* @param   void
* @retval  void
*/
void ControlSwitch(uint32_t commot)
{
 switch (commot)
 {
   case SCAN_MODE_READ:
     ScanningToExplore();
     break;
   case GIMBAL_PC_SHOOT_MODE_READ:
     PcControlMode();
		  break;
	case FRAME_DROP_BUFFER_READ:
	   FrameDropBufferMode();
    break;
      case RC_MODE_READ:
        GimbalRcControlMode();
     break;
      case DISABLE_MOD_READ:
        GimbalWeakDeinit();
     break;
   default:
     GimbalDeinit();
	    break;
 }
}
/**
* @Data    2019-03-20 21:27
* @brief   决策控制判断
* @param   void
* @retval  void
*/
int16_t conflag =0;
  uint32_t ControlDecision(void)
  {
    switch (gimbal_t.pRc_t->switch_left)
    {
      case 1:
        if(gimbal_t.pRc_t->switch_right ==1)
        {
         switch (gimbal_t.pPc_t->commot)
         {
           case 0:
           if(conflag > 100)
           {
             SET_READ_STATUS(SCAN_MODE_READ);
           }
           else 
           {
                conflag ++;
              SET_READ_STATUS(FRAME_DROP_BUFFER_READ);
           }
             break;
           case 1:
            SET_READ_STATUS(GIMBAL_PC_SHOOT_MODE_READ);
           conflag =0;
             break;
           default:
             SET_READ_STATUS(SCAN_MODE_READ);
              break;
         }
        }
        else if(gimbal_t.pRc_t->switch_right ==2)
        {
           SET_READ_STATUS(RC_MODE_READ);
        } 
        break;
      case 2:
        CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);//完全开闭
        break;
      default:
        SET_READ_STATUS(DISABLE_MOD_READ);//失能
        break;      
    } 
//    if(gimbal_t.pRc_t->switch_left ==2)
//    {
//      CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);
//    }
//    else if(gimbal_t.pRc_t->switch_left ==2)
//    {
//      
//    }
//    else if(gimbal_t.pRc_t->switch_right ==1)
//    {
//       switch (gimbal_t.pPc_t->commot)
//       {
//         case 0:
//         if(conflag > 100)
//         {
//           SET_READ_STATUS(SCAN_MODE_READ);
//         }
//         else 
//         {
//              conflag ++;
//           	SET_READ_STATUS(FRAME_DROP_BUFFER_READ);
//         }
//           break;
//         case 1:
//				  SET_READ_STATUS(GIMBAL_PC_SHOOT_MODE_READ);
//         conflag =0;
//           break;
//         default:
//            break;
//       }
//    }
//    else if(gimbal_t.pRc_t->switch_right ==2)
//    {
//			 SET_READ_STATUS(RC_MODE_READ);
//    } 
//else  CLEAR_BIT(gimbal_t.status,GIMBAL_MOD_RUNNING);   
    return (gimbal_t.status & JUDGE_READ);//与出就绪位，用以判断不影响转态值
  }
 /**
* @Data    2019-03-20 21:27
* @brief   遥控模式
* @param   void
* @retval  void
*/
    int16_t i = 0;
  float iii = 0.03;
   float yyyy = 0.01;
    int16_t rcscok_up = 300;
  int16_t rcscok_down = 2400;
  int16_t rctemp;
  int16_t seepdd =7;
  int16_t shesu =0;
  int16_t flaaaa =0;
void GimbalRcControlMode(void)
{
  if((gimbal_t.status&RC_MODE_RUNING) != RC_MODE_RUNING)
  {
     SET_RUNING_STATUS(RC_MODE_RUNING);
    Shoot(0,0);
        //目标值切换，状态切换
    gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;
    gimbal_t.pYaw_t->target = gimbal_t.pYaw_t->real_angle;
    gimbal_t.prammer_t->target = gimbal_t.prammer_t->real_angle ;
    if(gimbal_t.pPitch_t->target > (rcscok_down-1) && gimbal_t.pPitch_t->target< DOWN_BUFF_POSI)
    {
      gimbal_t.pPitch_t->target =(rcscok_down-1);
    }
    else if(gimbal_t.pPitch_t->target < (rcscok_up+1) || gimbal_t.pPitch_t->target > UP_BUFF_POSI)
    {
      gimbal_t.pPitch_t->target =(rcscok_up+1);
    }
    gimbal_t.pPitch_t->ppostionPid_t->error =0;
    gimbal_t.pYaw_t->ppostionPid_t->error =0;
    SetGeneralMode();
  }
//  if(gimbal_t.pRc_t->ch2 >600)
//  {
     Shoot(shesu,0);
//  }
//  else  Shoot(0,0);
   //                HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
  gimbal_t.pYaw_t->target += (int16_t)(gimbal_t.pRc_t->ch3 * iii);
  
        gimbal_t.pPitch_t->target += (int16_t)(gimbal_t.pRc_t->ch4 * yyyy);
        gimbal_t.pPitch_t->target = MAX(gimbal_t.pPitch_t->target,rcscok_down);
        gimbal_t.pPitch_t->target = MIN(gimbal_t.pPitch_t->target,rcscok_up);
        
     gimbal_t.pPitch_t->ppostionPid_t->error =CalculateError(gimbal_t.pPitch_t->target,( gimbal_t.pPitch_t->real_angle),4600,(8192));//待测试
     gimbal_t.pYaw_t->ppostionPid_t->error =CalculateError((gimbal_t.pYaw_t->target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//待测试//rctemp;// 
}
/**
* @Data    2019-03-21 00:46
* @brief   云台关闭//待完善
* @param   void
* @retval  void
*/
//int16_t last_pid_pitch;
void GimbalDeinit(void)
{
   Shoot(0,0);
    				gimbal_t.pYaw_t->pspeedPid_t->kp = 0;
				gimbal_t.pYaw_t->pspeedPid_t->kd = 0;
				gimbal_t.pYaw_t->pspeedPid_t->ki = 0;
  gimbal_t.pYaw_t->pspeedPid_t->pid_out = 0;
           gimbal_t.pPitch_t->pspeedPid_t->kd =0;
        gimbal_t.pPitch_t->pspeedPid_t->ki =0;//45;
        gimbal_t.pPitch_t->pspeedPid_t->kp =0;//290
      gimbal_t.pPitch_t->pspeedPid_t->pid_out = 0;
  //   __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_1_START_V);
 //   HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
}
/**
* @Data    2019-03-21 00:46
* @brief   云台模块失能//待完善
* @param   void
* @retval  void
*/
 void GimbalWeakDeinit(void)
 {
  CLEAR_BIT(gimbal_t.status,GIMBAL_MOD_RUNNING);
    Shoot(0,0);
//    __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_1_START_V);
//    HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
 }
	/**
		* @Data    2019-03-26 21:32
		* @brief   掉帧缓冲状态设置
		* @param   void
		* @retval  void
		*/
		void SetFrameDropBufferStatus(void)
		{
			SET_RUNING_STATUS(FRAME_DROP_BUFFER_RUNING);
 //    HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);
		}
	/**
	* @Data    2019-03-26 21:17
	* @brief   掉帧缓冲模式
	* @param   void
	* @retval  void
	*/
int16_t y_lock;
int16_t p_lock;
uint8_t buffer_flag =0;
  int16_t go_back = 10;
  int16_t go_back_lock = 800;
  int16_t e_error = 260;
	void FrameDropBufferMode(void)
	{
		if((gimbal_t.status&FRAME_DROP_BUFFER_RUNING) != FRAME_DROP_BUFFER_RUNING)
		{
			
			SetFrameDropBufferStatus();
			if(gimbal_t.pYaw_t->ppostionPid_t->error > 100)///右边
			{
         y_lock = 1500;
			 gimbal_t.pYaw_t->target	= SetLock(gimbal_t.pYaw_t->real_angle,y_lock);
			}
		 if(gimbal_t.pYaw_t->ppostionPid_t->error < -100)//左边
			{
				 y_lock = -1500;
		  	gimbal_t.pYaw_t->target	= SetLock(gimbal_t.pYaw_t->real_angle,y_lock);
			}
			SetGeneralMode();
       gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle +e_error;
//      if(gimbal_t.pPitch_t->ppostionPid_t->error >450)
//      {
//        gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle +400;
//      }
//      if(gimbal_t.pPitch_t->ppostionPid_t->error <-300)
//      {
//      gimbal_t.pPitch_t->target = gimbal_t.pPitch_t->real_angle;
//      }
//      if(gimbal_t.pPitch_t->target >go_back_lock)
//      {
//        go_back = (0 -go_back);
//      }
        
		}
    Shoot(3,0);
//   gimbal_t.pPitch_t->target  +=  go_back;
		  gimbal_t.pPitch_t->ppostionPid_t->error = CalculateError(gimbal_t.pPitch_t->target,( gimbal_t.pPitch_t->real_angle),5500,(8192));//待测试
      gimbal_t.pYaw_t->ppostionPid_t->error = CalculateError((gimbal_t.pYaw_t->target),(gimbal_t.pYaw_t->real_angle),15000,(20480));//待测试
	if(ABS(gimbal_t.pYaw_t->ppostionPid_t->error) < 100)
    {
	     gimbal_t.pYaw_t->target	= SetLock(gimbal_t.pYaw_t->real_angle,-y_lock);
        buffer_flag =1;
    }
    else if((ABS(gimbal_t.pYaw_t->ppostionPid_t->error) < 100) && (buffer_flag ==1))
    {
			CLEAR_BIT(gimbal_t.status,DISABLE_GIMBAL);
    }

	}

int16_t SetLock(int16_t r,int16_t t)
{
  if(r + t > 20480)
   r = 20480 -(r + t );
   else if (r + t <0)
   r =  t+20480+r;
  else r = t+r;
	return r;
}
//发射
//  //热量
//  uint8_t hot_bool(void)
//  {
//    if(shoot_data_t.bullet_freq > 200)
//     return 2;
//   if(shoot_data_t.bullet_freq<=200)
//     return 1;
//  }
// int16_t aa=0;
// void Shoot(uint8_t speed,uint8_t e)
// {
//   if(speed ==0)
//   {
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,FRICTIONGEAR_1_START_V);
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,FRICTIONGEAR_2_START_V);
//     HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_RESET);

//   }
//   else if(speed >0)
//   {
//     if(speed >8&& e ==1)
//     {
//       if(aa > 100)
//       e =0;
//       aa++;
//     }
//     else if(speed >8&& e ==0)
//     {
//       speed =5;
//     }
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,1700);
//     __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_2,1700);
//     HAL_GPIO_WritePin(LASER_GPIO,LASER,GPIO_PIN_SET);
//     SetRammerPID(speed);
//   }
// }

// void AiShoot(uint32_t status)
// {
//   // get(dis);
//   if(dis > 300)
//   {
//     Shoot()
//   }
// }



/*-----------------------------------file of end------------------------------*/


