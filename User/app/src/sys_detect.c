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
  static void RcControlMode(const dbusStruct* rc);
  static void DetectControlMode(void);
extern uint8_t task_on_off;
static chassisStruct *pchassis_t = NULL;
  /**
  * @Data    2019-03-13 02:44
  * @brief   系统自检任务初始化
  * @param   void
  * @retval  void
  */
  void SysDetectInit(void)
  {
    pchassis_t = RWGetChassisStructAddr();
  }
/**
* @Data    2019-03-13 02:45
* @brief   系统自检任务控制函数
* @param   void
* @retval  void
*/
void SysDetectControl(const dbusStruct* rc)
{
  
  switch (rc->switch_left)
  {
    case 1:
      RcControlMode(rc);
      break;
      case 3:
      
      break;
    default:
      break;
  }
}
  /**
  * @Data    2019-03-13 03:19
  * @brief   遥控模式
  * @param   void
  * @retval  void
  */
  void RcControlMode(const dbusStruct* rc)
  {
    
  }
  /**
  * @Data    2019-03-13 03:21
  * @brief   自检和校验数据模式
  * @param   void
  * @retval  void
  */
  void DetectControlMode(void)
  {
  //       if(GetChassisStatus() == (RX_OK|INIT_OK))
  // {

  //   //if()//激光判断间距大小，缓慢移动至边缘，可以遥控移动，
  //   //碰到轻触开关，编码器校准，
  //   //慢反向运动，校验超声波以及激光测距，判断用哪个

  //   // ProgressBarLed(LED_GPIO, 500);
  //   // task_on_off = ENABLE;
  // }
  }
/*------------------------------------file of end-------------------------------*/


