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
  |--FileName    : sys_detect.c                                                
  |--Version     : v1.0                                                            
  |--Author      : 海康平头哥                                                       
  |--Date        : 2019-03-13               
  |--Libsupports : STM32CubeF4 Firmware Package V1.24.0 (用别的库出问题别问我)
  |--Description : 系统自检，数据校准                                                    
  |--FunctionList                                                       
  |-------1. ....                                                       
  |          <version>:                                                       
  |     <modify staff>:                                                       
  |             <data>:                                                       
  |      <description>:                                                        
  |-------2. ...                                                       
  |---------------------------------declaration of end----------------------------|
 **/
#include "sys_detect.h" 
 sysDetectStruct sysDetect_t;
/* ----------------- 任务句柄 -------------------- */
osThreadId startSysDetectTaskHandle;//数据校准任务
  void StartSysDetectTask(void const *argument);
/* ----------------- 外部链接 -------------------- */
extern	osThreadId startParseTaskHandle;
//extern	osThreadId startLedTaskHandle;
extern	osThreadId startChassisTaskHandle;
extern osThreadId startGimbalTaskHandle;
extern	osThreadId startTxTaskHandle;//发送任务
/* ----------------- 私有函数 -------------------- */ 
//  static void DetectControlMode(void);
  static void RcCalibratorMode(sysDetectStruct* sds);
  static void AutoCalibratorMode(sysDetectStruct* sds);
  static void SysDetectControl(sysDetectStruct* sds);
  static void SysDetectInit(sysDetectStruct* sds);
  
//  static void DetectControlMode(void);
/* -------------- 临时变量 ----------------- */
	// uint32_t temp11 = 0;
  /**
  * @Data    2019-03-13 02:44
  * @brief   系统自检任务初始化
  * @param   void
  * @retval  void
  */
  void SysDetectInit(sysDetectStruct* sds)
  {
    sds->psys_chassis_t = RWGetChassisStructAddr();
    sds->psys_gimbal_t = RWGetgimbalStructAddr();
    
     // 		HAL_TIM_PWM_Start(FRICTIONGEAR,1000);
  }
 /**
 * @Data    2019-03-15 00:49
 * @brief   系统自检
 * @param   void
 * @retval  void
 */
 void SystemSelfChecking(void)
 {
   uint32_t temp1,temp2,temp3,temp4,temp5;
  /* -------- 各任务初始化判断 --------- */
  temp1 = GetRcStatus();
  temp2 = GetChassisStatus();
  temp3 = GetGimbalStatus();
  temp4	= GetUserTxStatus();
  temp5 = GetPcDataStatus();
  while((temp1 & temp2 & temp3 & temp4 & temp5 & INIT_OK) != INIT_OK)//加云台
  {
      //添加警报机制
//    temp1 = GetRcStatus();
//    temp2 = GetChassisStatus();
//    //temp3 = GetGimbalStatus();
//    //增加等待时间超时进入报错模式，蜂鸣器长响，
      osDelay(5);
  }
  /* -------- 重启解析任务 --------- */
  vTaskResume(startParseTaskHandle);
	  /* -------- 重启发送任务 --------- */
  vTaskResume(startTxTaskHandle);

//  temp1 = GetRcStatus();
//  temp2 = GetChassisStatus();
//  //temp3 = GetGimbalStatus();
//  while((temp1&temp2&RX_OK) != RX_OK)
//  {
//      //添加警报机制
//    temp1 = GetRcStatus();
//    temp2 = GetChassisStatus();
//    //temp3 = GetGimbalStatus();
//    //判断是什么模块离线
//      osDelay(5);
//  } 
  /* ------ 数据校准任务 ------- */
osThreadDef(sysDetectTask, StartSysDetectTask, osPriorityAboveNormal, 0, SYS_DETEC_HEAP_SIZE);
startSysDetectTaskHandle = osThreadCreate(osThread(sysDetectTask), NULL);
 }
/**
* @Data    2019-03-13 02:45
* @brief   系统自检任务控制函数
* @param   void
* @retval  void
*/
void SysDetectControl(sysDetectStruct* sds)
{
  switch (sds->psys_chassis_t->rc_t->switch_left)
  {
    case 1:
      CLEAR_BIT(sds->psys_chassis_t->status,DHECK_DATA_RUNING);
   // __HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,2000);//ces
     RcCalibratorMode(sds);
      break;
    case 3:
         SET_BIT(sds->psys_chassis_t->status,DHECK_DATA_RUNING);
   //   		__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,1000);//ces
     AutoCalibratorMode(sds);
      break;
//   case 2: 
//    	__HAL_TIM_SetCompare(FRICTIONGEAR,FRICTIONGEAR_1,1200);//ces
//     break;
    default:
      break;
  }
    sds->psys_chassis_t->pwheel1_t->error = sds->psys_chassis_t->pwheel1_t->target - sds->psys_chassis_t->pwheel1_t->real_speed;
    sds->psys_chassis_t->pwheel2_t->error =sds->psys_chassis_t->pwheel2_t->target - sds->psys_chassis_t->pwheel2_t->real_speed;
    SpeedPid(sds->psys_chassis_t->pwheel1_t->pspeedPid_t,sds->psys_chassis_t->pwheel1_t->error);
    SpeedPid(sds->psys_chassis_t->pwheel2_t->pspeedPid_t,sds->psys_chassis_t->pwheel2_t->error);
   sds->psys_chassis_t->pwheel1_t->pspeedPid_t->pid_out= MAX(sds->psys_chassis_t->pwheel1_t->pspeedPid_t->pid_out,4000);
   sds->psys_chassis_t->pwheel1_t->pspeedPid_t->pid_out= MIN(sds->psys_chassis_t->pwheel1_t->pspeedPid_t->pid_out,-4000);
    sds->psys_chassis_t->pwheel2_t->pspeedPid_t->pid_out =MAX(sds->psys_chassis_t->pwheel2_t->pspeedPid_t->pid_out,4000);
    sds->psys_chassis_t->pwheel2_t->pspeedPid_t->pid_out = MIN(sds->psys_chassis_t->pwheel2_t->pspeedPid_t->pid_out,-4000);
		ChassisCanTx(sds->psys_chassis_t->pwheel1_t->pspeedPid_t->pid_out,sds->psys_chassis_t->pwheel2_t->pspeedPid_t->pid_out);
 
}

  /**
  * @Data    2019-03-16 00:09
  * @brief   手动遥控校准模式
  * @param   void
  * @retval  void
  */
  void RcCalibratorMode(sysDetectStruct* sds)
  {
//    uint32_t temp1;
    static uint8_t zero_init_flag = 0;
    static uint8_t crtl_mode_flag = 0;
    if( (sds->psys_chassis_t->rc_t->ch1 < -600) && \
        (sds->psys_chassis_t->rc_t->ch2 <-600) && \
        (sds->psys_chassis_t->rc_t->ch3 >600) && \
        (sds->psys_chassis_t->rc_t->ch4 <-600) )//内八设置编码器零点
    {
       crtl_mode_flag =1;
      if(zero_init_flag == 30)
      {
        zero_init_flag =30;
       //  SET_ENCODER_ZERO(0);
        ProgressBarLed(LED_GPIO, 200);
      }
      zero_init_flag++;
    }
    if(zero_init_flag == 31)
    {
      if( (sds->psys_chassis_t->rc_t->ch1 == 0) && \
      (sds->psys_chassis_t->rc_t->ch2 == 0) && \
      (sds->psys_chassis_t->rc_t->ch3 == 0) && \
      (sds->psys_chassis_t->rc_t->ch4 == 0) )//回零防抖
      {
  /* -------- 重启底盘和云台任务 --------- */
   vTaskResume(startChassisTaskHandle);
   vTaskResume(startGimbalTaskHandle);
//   temp1 = GetChassisStatus();
   //temp3 = GetGimbalStatus();
//   while((temp1&RUNING_OK) != RUNING_OK)
//   {
//      //进一步判断错误
//      temp1 = GetChassisStatus();
//      //temp3 = GetGimbalStatus();
//       osDelay(5);
//   }
        WarningLed(10,100);
//   vTaskResume(startChassisTaskHandle);
//   vTaskResume(startGimbalTaskHandle);
     /* -------- 删除校准任务 --------- */
			vTaskDelete(startSysDetectTaskHandle);
      }
      zero_init_flag =30;
    }
    if(crtl_mode_flag==0)
      crtl_mode_flag = 0;
//       RcControlMode();
    else crtl_mode_flag = 0;
  }
  
  /**
  * @Data    2019-03-16 00:09
  * @brief   自动校准遥控校准模式
  * @param   void
  * @retval  void
  */
 void AutoCalibratorMode(sysDetectStruct* sds)
 {
   if(CheckTackData(sds) ==MOD_OK)
   {
     CLEAR_BIT(sds->psys_chassis_t->status,DHECK_DATA_RUNING);
     vTaskResume(startChassisTaskHandle);
     vTaskResume(startGimbalTaskHandle);
     ProgressBarLed(LED_GPIO, 200);
    WarningLed(10,100);
   	vTaskDelete(startSysDetectTaskHandle);
   }
 }
  /**
  * @Data    2019-03-13 03:21
  * @brief   校验数据模式
  * @param   void
  * @retval  void
  */
// void DetectControlMode(void)
// {
// // if(GetChassisStatus() == (RX_OK|INIT_OK))
// // {
// //   //if()//激光判断间距大小，缓慢移动至边缘，可以遥控移动，
// //   //碰到轻触开关，编码器校准，
// //   //慢反向运动，校验超声波以及激光测距，判断用哪个

// //   // ProgressBarLed(LED_GPIO, 500);
// //   // task_on_off = ENABLE;
// // }
// }
  /**
  * @Data    2019-03-13 01:46
  * @brief   数据校准任务
  * @param   void
  * @retval  void
  */
  void StartSysDetectTask(void const *argument)
  {
     SysDetectInit(&sysDetect_t);
    for(;;)
    {
      SysDetectControl(&sysDetect_t);
      osDelay(4);
    }
  }
  /**
  * @Data    2019-03-13 01:46
  * @brief   轨道数据校准
  * @param   void
  * @retval  void
  */
  int16_t check_flag=0;
  int16_t last_gyro_c,last_encoder_c;
MOD_Status CheckTackData(sysDetectStruct* sds)
{
  switch(check_flag)
  {
    case 0:
       sds->psys_chassis_t->pwheel1_t->target =sds->psys_chassis_t->pwheel2_t->target = 1000;
    if(HAL_GPIO_ReadPin(LIM_SWITCH_GPIO,LIM_SW_RIGHT) ==0)
    {
       sds->psys_chassis_t->pwheel1_t->target =sds->psys_chassis_t->pwheel2_t->target =0;
       CalibratingEncoder(sds->psys_chassis_t->pchassisEnconder_t,0);
       check_flag++;
       sds->psys_chassis_t->r_dire = sds->psys_chassis_t->pGyro_t->Yaw;
    }
      break;
    case 1:
      sds->psys_chassis_t->pwheel1_t->target =sds->psys_chassis_t->pwheel2_t->target = -4000;
    if(GetGyroDire() >3)
    {
      sds->psys_chassis_t->track.up_road_len = sds->psys_chassis_t->Sensor.encoder;
      sds->psys_chassis_t->track.start = (int16_t)(sds->psys_chassis_t->track.up_road_len *0.15f);
      sds->psys_chassis_t->track.angle = 60;
        sds->psys_chassis_t->track.half_angle = (int16_t)(sds->psys_chassis_t->track.angle*0.5);//轨道半角度 5
        sds->psys_chassis_t->track.turning_angle =  sds->psys_chassis_t->track.half_angle;//拐弯角度 6
      check_flag++;
    }
      break;
     case 2:
    if(IS_OPEN_INT(GetGyroDire(),( sds->psys_chassis_t->track.half_angle-3),( sds->psys_chassis_t->track.half_angle+3)))
    {
       sds->psys_chassis_t->track.up_turn_len =   sds->psys_chassis_t->Sensor.encoder;
      sds->psys_chassis_t->track.turning_len =  (sds->psys_chassis_t->track.up_turn_len  - sds->psys_chassis_t->track.up_road_len) *2;
      check_flag++;
    }
   sds->psys_chassis_t->pwheel1_t->target =sds->psys_chassis_t->pwheel2_t->target = -5000;
        break;
    case 3:
    if(IS_OPEN_INT(GetGyroDire(),( sds->psys_chassis_t->track.half_angle-3),( sds->psys_chassis_t->track.half_angle+3)) && sds->psys_chassis_t->Sensor.encoder > 2* sds->psys_chassis_t->track.up_turn_len)
    {
       sds->psys_chassis_t->track.down_turn_len = sds->psys_chassis_t->Sensor.encoder;//下路拐弯距离//chassis_t.track.up_turn_len + (int16_t)(chassis_t.track. turning_len*0.5) +  chassis_t.track.mid_road_len-300;//下路拐弯距离//实验室1512
       check_flag++;
    }
     sds->psys_chassis_t->pwheel1_t->target =sds->psys_chassis_t->pwheel2_t->target = -5000;
        break;
   case 4:
       sds->psys_chassis_t->pwheel1_t->target =sds->psys_chassis_t->pwheel2_t->target = -2000;
      if(HAL_GPIO_ReadPin(LIM_SWITCH_GPIO,LIM_SW_LEFT) ==0)
      {
         sds->psys_chassis_t->pwheel1_t->target =sds->psys_chassis_t->pwheel2_t->target =0;
        sds->psys_chassis_t->track.total_len = sds->psys_chassis_t->Sensor.encoder;
        sds->psys_chassis_t->track.half_len =  (int16_t)(sds->psys_chassis_t->track.total_len* 0.5f);
        sds->psys_chassis_t->track.end = sds->psys_chassis_t->track.total_len - sds->psys_chassis_t->track.start;
        check_flag =0;
         SET_STATE(CORRET_ENCODE);
      SET_CHASSIS_LAST_SELF_POS(MID_ROAD);
     SET_CHASSIS_REAL_SELF_POS(DOWN_ROAD);
        return MOD_OK;
      }
        break;
  }
  return MOD_BUSY;
}
// /**
//	* @Data    2019-03-30 22:13
//	* @brief   轨道数据记录
//	* @param   void
//	* @retval  void
//	*/ 
//uint8_t sw_li =0;
//uint8_t tack_auto_flag[10]={0};
//int16_t last_encoder =0;
//MOD_Status TrackData(void)
//{
//  if(chassis_t.Sensor.laser_sw_right ==1 &&sw_li !=1)
//  {
//        CalibratingEncoder(chassis_t.pchassisEnconder_t,0);
//     sw_li =1;
//    chassis_t.r_dire = chassis_t.pGyro_t->Yaw;
//  }
//  else if(chassis_t.Sensor.lim_sw_left ==1 &&sw_li !=2)
//  {
//    sw_li =2;
//    chassis_t.r_dire = chassis_t.pGyro_t->Yaw;
//  }
//  if(sw_li ==1)
//  {
//    chassis_t.pwheel1_t->target = chassis_t.pwheel2_t->target ==-2000;
//    if(GetGyroDire() >2 &&tack_auto_flag[0] ==0)
//    {
//      tack_auto_flag[0] =1;
//      chassis_t.track.up_road_len = chassis_t.Sensor.encoder;
//      chassis_t.track.start = 200;
//    }
//    if(GetGyroDire()>20 && (chassis_t.Sensor.encoder - last_encoder !=0) &&chassis_t.pwheel1_t->real_speed !=0 &&chassis_t.pwheel2_t->real_speed !=0  &&tack_auto_flag[1] ==0)
//    {
//      tack_auto_flag[1] =1;
//      chassis_t.track.angle = GetGyroDire();
//      chassis_t.track.turning_len = chassis_t.Sensor.encoder - chassis_t.track.up_road_len ;
//      
//    } 
//  }
//  
//  
//  last_encoder = chassis_t.Sensor.encoder;
//}
  /**
  * @Data    2019-03-13 01:46
  * @brief   掉线提示
  * @param   void
  * @retval  void
  */
  void OffLineTip(void)
  {
    HAL_GPIO_WritePin(LED_GPIO,LED_4,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GPIO,LED_5,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GPIO,LED_6,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GPIO,LED_7,GPIO_PIN_SET);
    if(!IS_RX_OK(GetRcStatus()))
    {
      __HAL_TIM_SetCompare(BUZZER_TIM,FRICTIONGEAR_1,15000);
      FlashingLed(LED_GPIO,LED_1,1,500);
      osDelay(1000);
    }
    if(!IS_RX_OK(GetPcDataStatus()))
    {
      __HAL_TIM_SetCompare(BUZZER_TIM,FRICTIONGEAR_1,15000);
      FlashingLed(LED_GPIO,LED_1,8,62);
      osDelay(1000);
    }
    if(!IS_RX_OK(GetChassisStatus()))
    {
      __HAL_TIM_SetCompare(BUZZER_TIM,FRICTIONGEAR_1,15000);
      FlashingLed(LED_GPIO,LED_2,2,250);
      osDelay(1000);
    }
   if(!IS_RX_OK(GetGimbalStatus()))
    {
      __HAL_TIM_SetCompare(BUZZER_TIM,FRICTIONGEAR_1,15000);
      FlashingLed(LED_GPIO,LED_2,1,500);
      if( GetFps(sysDetect_t.psys_gimbal_t->prammer_t->fps) ==0)
        FlashingLed(LED_GPIO,LED_3,1,500);
      if( GetFps(sysDetect_t.psys_gimbal_t->pYaw_t->fps) ==0)
        FlashingLed(LED_GPIO,LED_3,2,250);
      if( GetFps(sysDetect_t.psys_gimbal_t->pPitch_t->fps) ==0)
        FlashingLed(LED_GPIO,LED_3,3,166);
      if( GetFps(sysDetect_t.psys_gimbal_t->pGyro_t->fps) ==0)
        FlashingLed(LED_GPIO,LED_3,4,125);
      osDelay(1000);
    }
    if(IS_STATE(GAMING_BEGIN))
    {
      FlashingLed(COLOR_LED_GPIO,RED_LED,1,500);
    }
    if(IS_STATE(DEBUG_BEGIN))
    {
      FlashingLed(COLOR_LED_GPIO,GREEN_LED,1,500);
    }
    else 
    {
      __HAL_TIM_SetCompare(BUZZER_TIM,FRICTIONGEAR_1,0);
    }
  }
/*------------------------------------file of end-------------------------------*/


