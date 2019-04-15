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
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	incrementalEnconderStruct* pchassisEnconder_t;
	CAN_HandleTypeDef *hcanx;
  powerBufferPoolStruct*ppowerBufferPool_t;
  refereeSystemStruct* p_refereeSystem_t;
	gy955Struct* pgyroByCan_t;
	 const pcDataStruct* pPc_t;
	struct State
	{
		int16_t  r_dire;
		uint8_t r_area;//��ǰ��������
		uint8_t last_area;//�ϴ���������//
		uint8_t hurt;//�����������
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
	#define TURNING_ANGLE             20//����ǶȲ�
 #define  SONIC_QU_SIZE              10
	void ChassisInit(CAN_HandleTypeDef *hcan,const dbusStruct*rc,const pcDataStruct* pPc_t);
	void ChassisParseDate(uint16_t id,uint8_t *data);
//	void ChassisUserCanTx(int16_t w1,int16_t w2);
	HAL_StatusTypeDef ChassisCanTx(int16_t w1,int16_t w2);
	void ChassisControl(void);
const chassisStruct* GetChassisStructAddr(void);
 chassisStruct *RWGetChassisStructAddr(void);
 uint32_t GetChassisStatus(void);
  void SetMotorTarget(int16_t w1,int16_t w2);
   void ChassisRcControlMode(void);
	 powerBufferPoolStruct* PowerBufferPoolInit(void);
     RM3508Struct* wheel2Init(void);
       RM3508Struct* wheel1Init(void);
        	void SetSetInitStatus(void);
          uint8_t GetHurtStatus(void);
				void ChassisCruiseModeInit(void);
        void ChassisEludeControlMode(void);
				void ChassisControlSwitch(uint32_t commot);
				  uint32_t ChassisControlDecision(void);
					void ChassisDeinit(void);
						int16_t GetGyroDire(void);
							uint8_t GetOrgans(void);
								void SetArea(void);//�����������
									int16_t Go(int16_t target,int16_t speed);
										void ChassisPcShootModeInit(void);
											void ChassisEludeControlModeInit(void);
                      	void ChassisPcShootMode(void);
  void ChassisSensorParse(uint8_t *data);
				//	void SetArea(void);
void SetUltrasonic(void);
int16_t jiujimoshi(void);
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


