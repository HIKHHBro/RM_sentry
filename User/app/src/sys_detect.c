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
/* ----------------- ������ -------------------- */
extern	osThreadId startSysInitTaskHandle; 
extern	osThreadId startParseTaskHandle;
//extern	osThreadId startLedTaskHandle;
extern	osThreadId startChassisTaskHandle;
	extern osThreadId startGimbalTaskHandle;
 extern osThreadId startSysDetectTaskHandle;//ϵͳ�Լ죬����У׼����
  static void RcControlMode(const dbusStruct* rc);
  static void DetectControlMode(void);
static chassisStruct *pchassis_t = NULL;
static uint8_t detect_flag =0;
/* -------------- ��ʱ���� ----------------- */
	uint32_t temp11 = 0;
  /**
  * @Data    2019-03-13 02:44
  * @brief   ϵͳ�Լ������ʼ��
  * @param   void
  * @retval  void
  */
  void SysDetectInit(void)
  {
    pchassis_t = RWGetChassisStructAddr();
		detect_flag = ENABLE;
  }
/**
* @Data    2019-03-13 02:45
* @brief   ϵͳ�Լ�������ƺ���
* @param   void
* @retval  void
*/
void SysDetectControl(const dbusStruct* rc)
{
	if((GetRcStatus() == INIT_OK)&&detect_flag)
	{
		vTaskResume(startParseTaskHandle);
	}
	// if((GetChassisStatus() == (RX_OK|INIT_OK))&&detect_flag)
	// {
	// 	detect_flag = DISABLE;
	// }
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
    temp11 = GetPosition(pchassis_t->pchassisEnconder_t);
		pchassis_t->pwheel1_t->error = CAL_ERROR(rc->ch1,pchassis_t->pwheel1_t->real_speed);
		pchassis_t->pwheel2_t->error = CAL_ERROR(rc->ch2,pchassis_t->pwheel2_t->real_speed);
    SpeedPid(pchassis_t->pwheel1_t->pspeedPid_t,pchassis_t->pwheel1_t->error);
		SpeedPid(pchassis_t->pwheel2_t->pspeedPid_t,pchassis_t->pwheel2_t->error);
		ChassisCanTx(pchassis_t->pwheel1_t->pspeedPid_t->pid_out,pchassis_t->pwheel2_t->pspeedPid_t->pid_out);
  }
  /**
  * @Data    2019-03-13 03:21
  * @brief   �Լ��У������ģʽ
  * @param   void
  * @retval  void
  */
//  void DetectControlMode(void)
//  {
//  //       if(GetChassisStatus() == (RX_OK|INIT_OK))
//  // {

//  //   //if()//�����жϼ���С�������ƶ�����Ե������ң���ƶ���
//  //   //�����ᴥ���أ�������У׼��
//  //   //�������˶���У�鳬�����Լ������࣬�ж����ĸ�

//  //   // ProgressBarLed(LED_GPIO, 500);
//  //   // task_on_off = ENABLE;
//  // }
//  }
/*------------------------------------file of end-------------------------------*/


