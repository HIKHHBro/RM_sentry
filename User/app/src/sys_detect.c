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
 sysDetectStruct sysDetect_t;
/* ----------------- ������ -------------------- */
osThreadId startSysDetectTaskHandle;//����У׼����
  void StartSysDetectTask(void const *argument);
/* ----------------- �ⲿ���� -------------------- */
extern	osThreadId startParseTaskHandle;
//extern	osThreadId startLedTaskHandle;
extern	osThreadId startChassisTaskHandle;
extern osThreadId startGimbalTaskHandle;
/* ----------------- ˽�к��� -------------------- */ 
//  static void DetectControlMode(void);
  static void RcCalibratorMode(sysDetectStruct* sds);
  static void SysDetectControl(sysDetectStruct* sds);
  static void SysDetectInit(sysDetectStruct* sds);
//  static void DetectControlMode(void);
/* -------------- ��ʱ���� ----------------- */
	// uint32_t temp11 = 0;
  /**
  * @Data    2019-03-13 02:44
  * @brief   ϵͳ�Լ������ʼ��
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
 * @brief   ϵͳ�Լ�
 * @param   void
 * @retval  void
 */
 void SystemSelfChecking(void)
 {
   uint32_t temp1,temp2,temp3;
  /* -------- �������ʼ���ж� --------- */
  temp1 = GetRcStatus();
  temp2 = GetChassisStatus();
  temp3 = GetGimbalStatus();
  while((temp1&temp2&temp3&INIT_OK) != INIT_OK)//����̨
  {
      //��Ӿ�������
//    temp1 = GetRcStatus();
//    temp2 = GetChassisStatus();
//    //temp3 = GetGimbalStatus();
//    //���ӵȴ�ʱ�䳬ʱ���뱨��ģʽ�����������죬
//      osDelay(5);
  }
  /* -------- ������������ --------- */
  vTaskResume(startParseTaskHandle);
//  temp1 = GetRcStatus();
//  temp2 = GetChassisStatus();
//  //temp3 = GetGimbalStatus();
//  while((temp1&temp2&RX_OK) != RX_OK)
//  {
//      //��Ӿ�������
//    temp1 = GetRcStatus();
//    temp2 = GetChassisStatus();
//    //temp3 = GetGimbalStatus();
//    //�ж���ʲôģ������
//      osDelay(5);
//  } 
  /* ------ ����У׼���� ------- */
osThreadDef(sysDetectTask, StartSysDetectTask, osPriorityAboveNormal, 0, SYS_DETEC_HEAP_SIZE);
startSysDetectTaskHandle = osThreadCreate(osThread(sysDetectTask), NULL);
 }
/**
* @Data    2019-03-13 02:45
* @brief   ϵͳ�Լ�������ƺ���
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
  * @brief   �ֶ�ң��У׼ģʽ
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
        (sds->psys_chassis_t->rc_t->ch4 <-600) )//�ڰ����ñ��������
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
      (sds->psys_chassis_t->rc_t->ch4 == 0) )//�������
      {
  /* -------- �������̺���̨���� --------- */
   vTaskResume(startChassisTaskHandle);
   vTaskResume(startGimbalTaskHandle);
//   temp1 = GetChassisStatus();
   //temp3 = GetGimbalStatus();
//   while((temp1&RUNING_OK) != RUNING_OK)
//   {
//      //��һ���жϴ���
//      temp1 = GetChassisStatus();
//      //temp3 = GetGimbalStatus();
//       osDelay(5);
//   }
        WarningLed(5,100);
     /* -------- ɾ��У׼���� --------- */
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
  * @brief   У������ģʽ
  * @param   void
  * @retval  void
  */
// void DetectControlMode(void)
// {
// // if(GetChassisStatus() == (RX_OK|INIT_OK))
// // {
// //   //if()//�����жϼ���С�������ƶ�����Ե������ң���ƶ���
// //   //�����ᴥ���أ�������У׼��
// //   //�������˶���У�鳬�����Լ������࣬�ж����ĸ�

// //   // ProgressBarLed(LED_GPIO, 500);
// //   // task_on_off = ENABLE;
// // }
// }
  /**
  * @Data    2019-03-13 01:46
  * @brief   ����У׼����
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


