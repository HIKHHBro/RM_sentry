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
#include "input_sw.h" 
#include "sentry_state_config.h"
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
#define  IS_CHASSIS_STATUS(_status_)                (GET_BYTE(chassis_t.status,_status_) == _status_)
/* -------------- �� ----------------- */
#define CAL_ERROR(target,real) (CalculatePationError((target),(real)))

	#define WHEEL1_RX_ID      			 0x201
	#define WHEEL2_RX_ID       			 0x202
	#define CURRENT_METER_RX_ID      0x401//�����ƽ���id
  #define CHASSIS_SENSOR_RX_ID     0x402//���̴��������ݽ���id
  #define  CHASSIS_RX_ID   0x413
	#define CHASSIS_CAN_TX_ID  			 0x200
	#define W1_LIMIT_SPEED    			 8000  //����1�ٶ��޷�
	#define W2_LIMIT_SPEED    			 8000  //����2�ٶ��޷�
	#define EN_RADIUS            			 25    //���������Ӱ뾶��λ mm
	#define ENCONDER_POLES    			 3600 
  #define  DIRE_SU                      200 //����λ�ýǶȸı�Ĺ���  360-����λ�ýǶȱ仯���Ƕ�
/* ----����������ϳ�һ��Ҫ�ĺ�---- */
//  #define TRACK_TOLAT            1650//����ܳ���//��ʽ�����������Ϊ5436  //����ϳ�����һ�£���һ������У׼//д������У׼ģʽ
//	#define UP_ROAD_LEN               610//��ʱ   611//��·��������
//  #define HALF_ORBITAL                 825//��ʱ                         //1400//ʵ���ҹ��//�������                                               
//	#define MID_ROAD_LEN               1000//��ʱ  2017//ֱ����·
//	#define DOWN_ROAD_LEN               1650 //��ʱ 2806//��·����
//	#define TURNING_ANGLE             55//����ǶȲ�//�����ԽǶȲ�
  #define  OUT_OF_CONTROL_MID_FLAG     100//100*5ms  �����м���ʱ�����Ӧ�ñ������
// #define  TRACK_END             2806      //����е�
// #define TRACK_START            200 //������
// #define ORBITAL_ANGLE       60//����Ƕ� 
// #define HALF_ORBITAL_ANGLE   30//�Ƕ�һ��
 #define MOV_RAN          300//����ģʽ�߶���Χ
 #define  CHASSIS_MOTOR_DIRE_LEFT    -1//����Ƿ�ת
 #define  CHASSIS_MOTOR_DIRE_RIGHT    1//�ұ�����ת 
 
 #define  SONIC_QU_SIZE              10
 
typedef struct chassisStruct
{
	RM3508Struct *pwheel1_t;
	RM3508Struct *pwheel2_t;
  const dbusStruct   *rc_t;
	incrementalEnconderStruct* pchassisEnconder_t;
	CAN_HandleTypeDef *hcanx;
  powerBufferPoolStruct*ppowerBufferPool_t;
  refereeSystemStruct* p_refereeSystem_t;
	gyroStruct* pGyro_t;
	 const pcDataStruct* pPc_t;
	int16_t  r_dire;
  uint32_t gimbal_state;//��̨����
	// uint32_t state;//��������������״̬
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
  struct track
  {
    int16_t total_len;
    int16_t up_turn_len;
    int16_t down_turn_len;
    int16_t mid_road_len;
    int16_t up_road_len;
    int16_t dowm_road_len;
    int16_t turning_len;
    int16_t turning_angle;
    int16_t angle;
    int16_t end;//�յ�����
    int16_t start;//�����ȥ
    int16_t half_angle;
    int16_t half_len;
    
  }track;
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
  void ChassisDeinit(void);
  int16_t GetGyroDire(void);
  uint8_t GetOrgans(void);
  void SetArea(void);//�����������
  void ChassisSensorParse(void);
  //	void SetArea(void);
  void SetUltrasonic(void);
  int16_t jiujimoshi(void);
  int16_t GoToPiont(int16_t piont,int16_t *pid_out);
  void EncodeDetector(void);
  //----------��ʱ����
  int16_t Toback(int16_t speed);
  uint32_t SetAByEncoder(void);
//  void GetState(void);
  void GetHurted(void);
  MOD_Status AutoCalibrationData(void);
void GetGimblaCommot(uint32_t commot);
void TrackparseInit(int16_t tatol_len,int16_t angle);
void TrackparseInit(int16_t tatol_len,int16_t angle);
//-----------------
#endif	// __CHASSIS_H
	
/*-----------------------------------file of end------------------------------*/


