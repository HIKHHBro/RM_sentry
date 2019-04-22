/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,����ƽͷ��,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : gimbal.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2019-01-26               
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
#ifndef __GIMBAL_H 
#define __GIMBAL_H 
#include "rammer.h" 
#include "DataStructure.h" 
#include "gyro.h" 
/* -------------- ģ���Զ����־λ�� ----------------- */
#define  RC_MODE_READ               (0x80000000U) //ң��ģʽ׼������
#define  GIMBAL_PC_SHOOT_MODE_READ  (0x40000000U) //������ģʽ׼������
#define  FRAME_DROP_BUFFER_READ     (0x20000000U) //��֡����׼������
#define  SCAN_MODE_READ             (0x10000000U) //ɨ��ģʽ׼������
#define  DISABLE_MOD_READ           (0xF0000000U)//ʹ������ģ��
#define  JUDGE_READ                 (0xF0000000U) //�жϾ���

#define  RC_MODE_RUNING             (0x08000000U)//ң��ģʽִ��
#define  PC_SHOOT_MODE_RUNING       (0x04000000U)//������ģʽִ��
#define  FRAME_DROP_BUFFER_RUNING   (0x02000000U)//��֡����ִ��
#define  SCAN_MODE_RUNING           (0x01000000U)//ɨ��ģʽִ��
#define  GIMBAL_MOD_RUNNING         (0x0F000000U)//����ģ��λ

#define  DISABLE_GIMBAL             (0xFF000000U)//ʧ����̨
#define  DELEC_USER_MODE            (0x00000000U)//����û��Զ����־λ
#define SET_RUNING_STATUS(_status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,GIMBAL_MOD_RUNNING);	\
					SET_BIT(gimbal_t.status,_status_);							\
				}while(0)	
#define SET_READ_STATUS(__status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(gimbal_t.status,DISABLE_MOD_READ);	\
					SET_BIT(gimbal_t.status,__status_);							\
				}while(0)																					
/* -------------- ˽�к� ----------------- */
	#define QUEUE_LEN      5U//���Ϊ5
	#define  QUEUE_SIZE    8U//����Ϊ5;
  #define  MEMORY_SPAN   (50)          //   1000ms = 50*20
  #define  QUEUE_YAW_STATUS_SIZE      20//����䳤��Ϊ1s ����Ϊ50
 #define   QUEUE_PITCH_STATUS_SIZE    20//����10������䳤��Ϊ1s
/* -------------- �궨�� ----------------- */
#define GIMBAL_CAN_ID_L 0x204
#define GIMBAL_CAN_ID_H 0x208
	#define YAW_RX_ID 								0x205//YAW�������� id
	#define PITCH_RX_ID 							0x206//PITCH�������� id
	#define GIMBAL_CAN_TX_ID 					0x1ff//��̨�������id
 #define YAW_LIMIMT_CUT                 (6000)//�����޷�
  #define PITCH_LIMIMT_CUT                 (29000)//�����޷�
  #define LINT_LIMINT                    (6000)
	#define PASS_STATUS_LEN                 10//��ʷ״̬���䳤��
  #define DOWN_BUFF_POSI                     6000//����λ������ֵ
  #define UP_BUFF_POSI                     7000//����λ������ֵ
typedef struct gimbalStruct
{
  uint32_t status;
	M2006Struct *prammer_t;
  RM6623Struct *pYaw_t;
  GM6020Struct *pPitch_t;
	const dbusStruct* pRc_t;
  const pcDataStruct* pPc_t;
  int16_t yaw_scan_target;
  int16_t pitch_scan_target;
	gy955Struct *pGyro_t;
	// struct frameDropBuffer_t
	// {
	// 	int16_t yaw_pass_position[PASS_STATUS_LEN];
	// 	int16_t pitch_pass_position[PASS_STATUS_LEN];
	// }frameDropBuffer_t;
	
} gimbalStruct;
	void GimbalStructInit(const dbusStruct* pRc_t,const pcDataStruct* pPc_t);
void GimbalParseDate(uint32_t id,uint8_t *data);
HAL_StatusTypeDef GimbalCanTx(int16_t yaw,int16_t pitch,int16_t rammer);
void GimbalAutoControl(void);
 HAL_StatusTypeDef RxPCMsg(void);
 uint32_t GetGimbalStatus(void);
gimbalStruct *RWGetgimbalStructAddr(void);
	 void SetGimBalInitStatus(void);
  int16_t RammerPidControl(int16_t rammer);
RM6623Struct* YawInit(void);
  int16_t PitchPidControl(int16_t pitch);
  int16_t YawPidControl(int16_t yaw);
		GM6020Struct* PitchInit(void);
void ScanningToExplore(void);
void ControlSwitch(uint32_t commot);
void PcControlMode(void);
void GimbalRcControlMode(void);
  uint32_t ControlDecision(void);//�����Դ���
void SetGeneralMode(void);
void SetPcControlPID(void);
void GimbalDeinit(void);
		void SetFrameDropBufferStatus(void);
	void GetTrend(int16_t* yaw_tr,int16_t* pitch_tr);
		void FrameDropBufferMode(void);
int16_t SetLock(int16_t r,int16_t t);
 void GimbalWeakDeinit(void);

#define GIMBAL_CAL_ERROR(target,real) (CalculateError((target),(real),5500,(8192)))
//#define YAW_CAL_ERROR(target,real) (CalculateError((target),(real),15000,(20480)))
#define GET_RAMMER_ANGLE(_A_,_LAST_,_COE_)     (RatiometricConversion(\
                                    _A_,\
                                    M2006_THRESHOLD,\
                                    RAMMER_REDUCTION,\
                                    _LAST_,\
                                    _COE_\
                                  )\
                              )





	int16_t PostionOutToSpeedOut(int16_t pid_out);															
  //void GyroParse(uint32_t id,uint8_t *data);
#endif // __GIMBAL_H
/*-----------------------------------file of end------------------------------*/






