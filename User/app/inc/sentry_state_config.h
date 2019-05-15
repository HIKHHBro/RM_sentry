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
	|--FileName    : sentry_state_config.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2019-04-28               
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
#ifndef __SENTRY_STATE_CONFIG_H 
#define __SENTRY_STATE_CONFIG_H 
#include "baseclass.h "
extern uint32_t sentry_state;
/* -------- ����״����־λ ��Ӧ��λ��1�����У�0����--------- */
#define  CHASSIS_STATE    (0x00000000U)
 #define HAVE_ENEMY       (0x80000000U)//�е���
 #define HURTED           (0x40000000U)//������
 #define FINE_ENEMY       (0x20000000U)//���ֵ���
 #define ENEMY_POS        (0x0F000000U)//����λ��λ
 #define IN_ROAD          (0x08000000U)//�����ڹ�·
 #define IN_BASE          (0x04000000U)//�����ڻ���
 #define IN_WILD_AREA     (0x02000000U)//������Ұ��
 #define IN_BLOCK_HOUSE   (0x01000000U)//�����ڵﱤ
 #define GAMING_BEGIN     (0x00800000U)//����ģʽ��ʼ
 #define DEBUG_BEGIN      (0x00400000U)//����ģʽ��ʼ
 #define MOD_BEGIN_CHECK  (0x00F00000U)//ģʽѡ��
 #define SELF_AREA        (0x000000FFU)//����λ��״̬
 #define LAST_AREA        (0x000000F0U)//�ϴ�λ��״̬
 #define REAL_AREA        (0x0000000FU)//���λ��״̬
 #define UP_ROAD          (0x00000001U)//��·��������
 #define MID_ROAD         (0x00000002U)//��·��������//ֱ����·
 #define DOWN_ROAD        (0x00000003U)//��·����
 #define CORRET_ENCODE    (0x00000100U)//��ʼ��У׼λ
 #define OUT_OF_CONTROL   (0x00000200U)//ʧ��λ��ʼ
 #define OUT_OF__OVER     (0x0000F00U)//ʧ��λ ʧ�ش���14��֮�󣬽�����ȫʧ��ģʽ
 #define AUTO_CONTROL_OK  (0x00000000U)//��������Ϊ
 #define HURT_RX_CLEAR    5
 
 #define  AHEAD_OF_ARMOR          0//ǰװ��0
	#define  BACK_OF_ARMOR           1 //ǰװ��1
#define  SET_STATE(_state_)     (SET_BIT(sentry_state,_state_))
#define  CLEAR_STATE(_state_)   (CLEAR_BIT(sentry_state,_state_))
#define  IS_STATE(_state_)                (GET_BYTE(sentry_state,_state_) == _state_)
#define  GET_CHASSIS_SELF_POS(_s_)                 GET_BYTE(sentry_state,_s_) // _s_ ��REAL_AREA
#define  GET_CHASSIS_STLE_DIR               (RIGHT_SH(GET_CHASSIS_SELF_POS(LAST_AREA),4) - GET_CHASSIS_SELF_POS(REAL_AREA))
#define  SET_CHASSIS_REAL_SELF_POS(_state_)    do                                              \
                                                {                                      \
                                                 CLEAR_BIT(sentry_state,REAL_AREA);\
                                                 SET_BIT(sentry_state,_state_);\
                                                }while(0)    
#define  SET_CHASSIS_LAST_SELF_POS(_state_)    do                                      \
                                                {                                        \
                                                 CLEAR_BIT(sentry_state,LAST_AREA);\
                                                 SET_BIT(sentry_state,LEFT_SH(_state_,4));\
                                                }while(0)
#define  OUT_OF_CONTROL_COUNTER(_state_)    do                              \
                                                {\
                                                 CLEAR_BIT(sentry_state,OUT_OF_CONTROL);\
                                                 SET_BIT(sentry_state,LEFT_SH(_state_,8));\
                                                }while(0) 
#define SET_ENMET_POS(_pos_)               do                              \
                                                {\
                                                 CLEAR_BIT(sentry_state,ENEMY_POS);\
                                                 SET_BIT(sentry_state,_pos_);\
                                                }while(0) 
#define IS_ROAD(_road_)   (GET_BYTE(sentry_state,REAL_AREA) == _road_)
#define GET_ALL_STATE     (sentry_state)                                            
	void GetState(void);
#endif	// __SENTRY_STATE_CONFIG_H
/*-----------------------------------file of end------------------------------*/


