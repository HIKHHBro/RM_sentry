/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,����ƽͷ��,                       |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : chassis.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
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
	|------------------------------declaration of end----------------------------|
 **/
#ifndef __CHASSIS_H 
#define __CHASSIS_H 
#include "motor.h" 
#include "DJI_dbus.h" 
#include "usart_debug.h" 
#include "enconder.h" 
#include "ultrasonic.h" 
#include "currentmeter.h" 
#include "power_buffer_pool.h"
#include "communicate.h"
#include "gyro.h" 
#include "pc_data.h" 

/* -------------- ģ���־λ�궨�� ----------------- */
#define   CHASSIS_RC_MODE_READ                 (0x80000000U)//ң��ģʽ 
 #define  CHASSIS_ELUDE_MODE_READ              (0x40000000U) //���ģʽ     
 #define  CHASSIS_PC_SHOOT_MODE_READ           (0x20000000U) //������ģʽ׼������
 #define  CHASSIS_CRUISE_MODE_READ             (0x10000000U)//Ѳ��ģʽģʽ׼������
 #define  DISABLE_CHA_MOD_READ			           (0xF0F00000U)//ʧ�ܾ�����־λ
 #define  CHASSIS_RC_MODE_RUNING               (0x08000000U)//ң��ģʽִ�� 
 #define  CHASSIS_ELUDE_MODE_RUNING            (0x04000000U) //���ģʽ 
 #define  CHASSIS_PC_SHOOT_MODE_RUNING         (0x02000000U) //������ģʽִ��   
 #define  CHASSIS_CRUISE_MODE_RUNING           (0x01000000U) //Ѳ��ģʽִ��
 #define   DISABLE_CHA_MOD_RUNNING			       (0x0F0F0000U)//ʧ�ܾ�����־λ
 #define   DISABLE_CHASSIS                     (0xFFFF0000U)//ʧ�ܵ���
 #define   CHASSIS_JUDGE_READ                  (0xF0F00000U)//�жϾ���

 #define CHASSIS_OUT_OF_CONTROL_READ           (0x00800000U)//��̨ʧ��ģʽ���� 
 #define CHASSIS_OUT_OF_CONTROL_RUNNING        (0x00080000U)//��̨ʧ��ģʽִ��  
///#define   CHASSIS_
// #define  DISABLE_MOD                        (0xF0000000U)//ʹ������ģ��

// #define  RC_MODE_READ               (0x02000000U)//ң��ģʽ׼������
// #define  JUDGE_READ                 (0x0F000000U)//�жϾ���
// #define  DELEC_USER_MODE            (0x00FFFFFFU)//����û��Զ����־λ

/* -------- ��������ģʽ��־λ --------- */
#define SET_CHA_RUNING_STATUS(_status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(chassis_t.status,DISABLE_CHA_MOD_RUNNING);	\
					SET_BIT(chassis_t.status,_status_);							\
				}while(0)	
#define SET_CHA_READ_STATUS(__status_)  											\
				do																								\
				{																									\
					CLEAR_BIT(chassis_t.status,DISABLE_CHA_MOD_READ);	\
					SET_BIT(chassis_t.status,__status_);							\
				}while(0)		
/* -------------- �� ----------------- */
#define CAL_ERROR(target,real) (CalculatePationError((target),(real)))

	#define WHEEL1_RX_ID      			 0x201
	#define WHEEL2_RX_ID       			 0x202
	#define CURRENT_METER_RX_ID      0x401//�����ƽ���id
  #define CHASSIS_SENSOR_RX_ID     0x402//���̴��������ݽ���id
	#define CHASSIS_CAN_TX_ID  			 0x200
	#define W1_LIMIT_SPEED    			 8000  //����1�ٶ��޷�
	#define W2_LIMIT_SPEED    			 8000  //����2�ٶ��޷�
	#define EN_RADIUS            			 30    //���������Ӱ뾶��λ mm
	#define ENCONDER_POLES    			 500 
  #define  AHEAD_OF_ARMOR          0//ǰװ��0
	#define  BACK_OF_ARMOR           1 //ǰװ��1
	#define UP_ROAD                 0//��·��������
	#define MID_ROAD                 1//ֱ����·
	#define DOWN_ROAD                 2//��·����
	#define TURNING_ANGLE             20//����ǶȲ�//�����ԽǶȲ�
 #define  SONIC_QU_SIZE              10
 #define  CHASSIS_MOTOR_DIRE_LEFT    -1//����Ƿ�ת
 #define  CHASSIS_MOTOR_DIRE_RIGHT    1//�ұ�����ת
 #define  OUT_OF_CONTROL_MID_FLAG     1000//1000*5ms  �����м���ʱ�����Ӧ�ñ������
 #define  AUTO_CONTROL_OK              (uint8_t)0//�Զ�ģʽ����
  #define OUT_OF_CONTROL            (uint8_t)(~(AUTO_CONTROL_OK))//�Զ�ģʽʧ��
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	incrementalEnconderStruct* pchassisEnconder_t;
	CAN_HandleTypeDef *hcanx;
  powerBufferPoolStruct*ppowerBufferPool_t;
  refereeSystemStruct* p_refereeSystem_t;
	//gy955Struct* pgyroByCan_t;
	 const pcDataStruct* pPc_t;
	struct State
	{
		int16_t  r_dire;
		uint8_t r_area;//��ǰ��������
		uint8_t last_area;//�ϴ���������//
		uint8_t hurt;//�����������
    uint8_t  out_of_flag;
	}State;//��������������״̬
	struct Sensor
  {
    uint8_t  lim_sw_left;
    uint8_t  lim_sw_right;
    uint8_t  laser_sw_left;
    uint8_t  laser_sw_right;
    uint8_t  sonic_left;
    uint8_t  sonic_right;
    int32_t  encoder;
  }Sensor;
  uint32_t status;
int16_t left_dis_son;
  int16_t right_dis_son;
}chassisStruct;
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc,const pcDataStruct* pPc_t);
	void ChassisParseDate(uint16_t id,uint8_t *data);
	void ChassisControl(void);
//	void ChassisUserCanTx(int16_t w1,int16_t w2);
	HAL_StatusTypeDef ChassisCanTx(int16_t w1,int16_t w2);
const chassisStruct* GetChassisStructAddr(void);
 chassisStruct *RWGetChassisStructAddr(void);
 uint32_t GetChassisStatus(void);
  void SetMotorTarget(int16_t w1,int16_t w2);

	 powerBufferPoolStruct* PowerBufferPoolInit(void);
     RM3508Struct* wheel2Init(void);
       RM3508Struct* wheel1Init(void);
        	void SetSetInitStatus(void);
          uint8_t GetHurtStatus(void);
					void ChassisDeinit(void);
						int16_t GetGyroDire(void);
							uint8_t GetOrgans(void);
								void SetArea(void);//�����������
  void ChassisSensorParse(uint8_t *data);
				//	void SetArea(void);
void SetUltrasonic(void);
int16_t jiujimoshi(void);
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


