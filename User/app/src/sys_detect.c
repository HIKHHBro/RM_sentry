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
/* ----------------- 私有函数 -------------------- */ 
//  static void DetectControlMode(void);
  static void RcCalibratorMode(sysDetectStruct* sds);
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
  }
 /**
 * @Data    2019-03-15 00:49
 * @brief   系统自检
 * @param   void
 * @retval  void
 */
 void SystemSelfChecking(void)
 {
   uint32_t temp1,temp2,temp3;
  /* -------- 各任务初始化判断 --------- */
  temp1 = GetRcStatus();
  temp2 = GetChassisStatus();
  temp3 = GetGimbalStatus();
  while((temp1&temp2&temp3&INIT_OK) != INIT_OK)//加云台
  {
      //添加警报机制
//    temp1 = GetRcStatus();
//    temp2 = GetChassisStatus();
//    //temp3 = GetGimbalStatus();
//    //增加等待时间超时进入报错模式，蜂鸣器长响，
//      osDelay(5);
  }
  /* -------- 重启解析任务 --------- */
  vTaskResume(startParseTaskHandle);
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
     RcCalibratorMode(sds);
      break;
    case 3:
      break;
    default:
      break;
  }
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
         SET_ENCODER_ZERO(0);
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
        WarningLed(5,100);
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
/*------------------------------------file of end-------------------------------*/


