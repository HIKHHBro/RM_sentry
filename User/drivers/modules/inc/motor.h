/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                      (C) Copyright 2018,����ƽͷ��,                         |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                         All Rights Reserved                            -   |
	|                                                                            |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)       -  |
	|                     https://github.com/GCUWildwolfteam                     |
	|----------------------------------------------------------------------------|
	|--FileName    : motor.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2018-12-31               
	|--Libsupports : ��׼���HAL��
	|--Description : 1��maxion���+RoboModule���� 
	|								 2��3508���+c610���
	|								 3��6623���
	|                4��6002���                                  
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#ifndef __MOTOR_H 
#define __MOTOR_H
	#include "baseclass.h"
	#include "bsp_can.h" 
	#include "counters.h" 
/* =========================== common of begin =========================== */
	int16_t RatiometricConversion(int16_t real, int16_t threshold, int16_t perce);
	int16_t zeroArgument(int16_t real, int16_t threshold);
/* =========================== common of end =========================== */
/* =========================== maxion of begin =========================== */
 /* -------- ID�� --------- */
	// typedef enum
	// {
	// 	openloop_mode_id_e                       = 0x01,//����ģʽid��
	// 	current_mode_id_e                        = 0x02,//����ģʽid��
	// 	velocity_mode_id_e                       = 0x03,//�ٶ�ģʽid��
	// 	position_mode_id_e                       = 0x04,//λ��ģʽid��
	// 	velocity_position_mode_id_e              = 0x05,//�ٶ�λ��ģʽid��
	// 	current_velocity_mode_id_e               = 0x06,//�����ٶ�ģʽid��
	// 	current_position_mode_id_e               = 0x07,//����λ��ģʽid��
	// 	current_velocity_position_mode_id_e      = 0x08 //����λ���ٶ�ģʽid�� 
	// }motorIDEnum;

		#define openloop_mode_id_e                       0x01//����ģʽid��
		#define current_mode_id_e                        0x02//����ģʽid��
		#define velocity_mode_id_e                       0x03//�ٶ�ģʽid��
		#define position_mode_id_e                       0x04//λ��ģʽid��
		#define velocity_position_mode_id_e              0x05//�ٶ�λ��ģʽid��
		#define current_velocity_mode_id_e               0x06//�����ٶ�ģʽid��
		#define current_position_mode_id_e               0x07//����λ��ģʽid��
		#define current_velocity_position_mode_id_e      0x08//����λ���ٶ�ģʽid�� 

		/* -------- ����ģʽ --------- */
	// typedef enum
	// {
	// 	openloop_mode_e                       = 0x01,//����ģʽ
	// 	current_mode_e                        = 0x02,//����ģʽ
	// 	velocity_mode_e                       = 0x03,//�ٶ�ģʽ
	// 	position_mode_e                       = 0x04,//λ��ģʽ
	// 	velocity_position_mode_e              = 0x05,//�ٶ�λ��ģʽ
	// 	current_velocity_mode_e               = 0x06,//�����ٶ�ģʽ
	// 	current_position_mode_e               = 0x07,//����λ��ģʽ
	// 	current_velocity_position_mode_e      = 0x08//����λ���ٶ�ģʽ
	// }motorModeEnum;

	#define	openloop_mode_e                       0x01//����ģʽ
	#define	current_mode_e                        0x02//����ģʽ
	#define	velocity_mode_e                       0x03//�ٶ�ģʽ
	#define	position_mode_e                       0x04//λ��ģʽ
	#define	velocity_position_mode_e              0x05//�ٶ�λ��ģʽ
	#define	current_velocity_mode_e               0x06//�����ٶ�ģʽ
	#define	current_position_mode_e               0x07//����λ��ģʽ
	#define	current_velocity_position_mode_e      0x08//����λ���ٶ�ģʽ
	/* ------ ������� ------- */
		// typedef enum
		// {
		// 	reset_fun_e                          = 0x00,//��λָ��
		// 	mode_selection_fun_e                 = 0x01,//ģʽѡ��
		// 	openloop_fun_e                       = 0x02,//����ģʽ
		// 	current_fun_e                        = 0x03,//����ģʽ
		// 	velocity_fun_e                       = 0x04,//�ٶ�ģʽ
		// 	position_fun_e                       = 0x05,//λ��ģʽ
		// 	velocity_position_fun_e              = 0x06,//�ٶ�λ��ģʽ
		// 	current_velocity_fun_e               = 0x07,//�����ٶ�ģʽ
		// 	current_position_fun_e               = 0x08,//����λ��ģʽ
		// 	current_velocity_position_fun_e      = 0x09,//����λ���ٶ�ģʽ
		// 	config_fun_e 												 = 0x0A,//����ָ��
		// 	data_feedback_fun_e 								 = 0x0B,//���ݷ���
		// 	left_and_right_limit_feedback_e 		 = 0x0C,//������λ����
		// 	online_check_fun_e 		 							 = 0x0F//���߼��
		// }functionNumEnum;

		#define	reset_fun_e                          0x00//��λָ��
		#define	mode_selection_fun_e                 0x01//ģʽѡ��
		#define	openloop_fun_e                       0x02//����ģʽ
		#define	current_fun_e                        0x03//����ģʽ
		#define	velocity_fun_e                       0x04//�ٶ�ģʽ
		#define	position_fun_e                       0x05//λ��ģʽ
		#define	velocity_position_fun_e              0x06//�ٶ�λ��ģʽ
		#define	current_velocity_fun_e               0x07//�����ٶ�ģʽ
		#define	current_position_fun_e               0x08//����λ��ģʽ
		#define	current_velocity_position_fun_e      0x09//����λ���ٶ�ģʽ
		#define	config_fun_e 												 0x0A//����ָ��
		#define	data_feedback_fun_e 								 0x0B//���ݷ���
		#define	left_and_right_limit_feedback_e 		 0x0C//������λ����
		#define	online_check_fun_e 		 							 0x0F//���߼��
	typedef struct maxionStruct
	{
		int16_t real_current; //��ʵ����
		int16_t real_position;//��ʵ�Ƕ�
		int16_t real_velocity;//��ʵ�ٶ�
		uint32_t module_id;
    uint32_t module_rx_id;//������ݽ���id
		CAN_HandleTypeDef* hcanx;
	}maxionStruct;
	/* -------------- ���к� ----------------- */
	#define MOTOR_CAN_ID_CAL(group,number) \
						((((group)<<8)|((number)<<4)))	//���can ����id����
	void ResetMode(CAN_HandleTypeDef *hcanx,uint32_t can_rx_id);
	void ModeSelectionMode(CAN_HandleTypeDef *hcanx,uint32_t rx_id,uint8_t mode);
	void OpenLoopMode(int16_t pwm,uint8_t *data);
  void SpeedLoopMode(int16_t pwm,int16_t Speed,uint8_t *data);
	void ConfigMode(CAN_HandleTypeDef *hcanx,uint32_t rx_id,uint8_t Time,\
																												uint8_t Ctl1_Ctl2);
	void MotorInit(CAN_HandleTypeDef *hcanx,uint8_t id,uint8_t mode);
  void MaxionParseData(maxionStruct*maxion,uint8_t *data);
/* =========================== maxion of end =========================== */

/* ============================= RM6623 of begin ============================ */
	/* -------------- �ṹ�� ----------------- */	
	typedef struct RM6623Struct
	{
		uint16_t id;//���can�� ip
		int16_t target;		 //Ŀ��ֵ
		int16_t tem_target;//��ʱĿ��ֵ
		int16_t real_current; //��ʵ����
		int16_t real_angle;//��ʵ�Ƕ�
		int16_t tem_angle;//��ʱ�Ƕ�
		int16_t zero;			 //������
		int16_t Percentage;//ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
		int16_t thresholds; //�����ת��ֵ
		postionPidStruct *ppostionPidStruct;
		speedPidStruct *speedPidStruct;
	} RM6623Struct;
	/* -------------- ���к��� ----------------- */
	void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx);
	void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data);
/* ============================= RM6623 of end ============================== */
/* ============================ RM3508 of begin ============================= */
	typedef struct RM3508Struct
	{
		uint16_t id;//���can�� ip
		int16_t target; //Ŀ��ֵ
		int16_t real_current;//��ʵ����
		int16_t real_angle;//��ʵ�Ƕ�
		int16_t real_speed;//��ʵ�ٶ�
	}RM3508Struct;
/* ============================== Rm3508 of end ============================= */
/* =========================== M6620 of begin =========================== */
	typedef struct M2006Struct
	{
		uint16_t rx_id;//���can�� ip
		int16_t target; //Ŀ��ֵ
		int16_t real_current;//��ʵ����
		int16_t real_angle;//��ʵ�Ƕ�
		int16_t real_speed;//��ʵ�ٶ�
		int16_t error[3];//��ǰ���
		postionPidStruct *pOuterLoop_t;
		speedPidStruct *pInnerLoop_t;
	}M2006Struct;
	void RM2006ParseData(M2006Struct *M2006, uint8_t *data);
/* =========================== M6620 of end =========================== */
#endif	// __MOTOR_H
/*---------------------------------file of end--------------------------------*/


