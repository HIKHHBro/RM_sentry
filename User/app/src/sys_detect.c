/**
  |-------------------------------- Copyright -----------------------------------|
  |                                                                              |
  |                        (C) Copyright 2019,����ƽͷ��,                         |
  |            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
  |                            All Rights Reserved                               |
  |                                                                              |
  |            By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
  |                     https://github.com/GCUWildwolfteam                       |
  |------------------------------------------------------------------------------|
  |--FileName    : sys_detect.c                                                
  |--Version     : v1.0                                                            
  |--Author      : ����ƽͷ��                                                       
  |--Date        : 2019-03-13               
  |--Libsupports : STM32CubeF4 Firmware Package V1.24.0 (�ñ�Ŀ�����������)
  |--Description : ϵͳ�Լ죬����У׼                                                    
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
  * @brief   ϵͳ�Լ������ʼ��
  * @param   void
  * @retval  void
  */
  void SysDetectInit(void)
  {
    pchassis_t = RWGetChassisStructAddr();
  }
/**
* @Data    2019-03-13 02:45
* @brief   ϵͳ�Լ�������ƺ���
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
  * @brief   ң��ģʽ
  * @param   void
  * @retval  void
  */
  void RcControlMode(const dbusStruct* rc)
  {
    
  }
  /**
  * @Data    2019-03-13 03:21
  * @brief   �Լ��У������ģʽ
  * @param   void
  * @retval  void
  */
  void DetectControlMode(void)
  {
  //       if(GetChassisStatus() == (RX_OK|INIT_OK))
  // {

  //   //if()//�����жϼ���С�������ƶ�����Ե������ң���ƶ���
  //   //�����ᴥ���أ�������У׼��
  //   //�������˶���У�鳬�����Լ������࣬�ж����ĸ�

  //   // ProgressBarLed(LED_GPIO, 500);
  //   // task_on_off = ENABLE;
  // }
  }
/*------------------------------------file of end-------------------------------*/


