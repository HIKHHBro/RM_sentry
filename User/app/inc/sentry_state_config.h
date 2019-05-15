/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,海康平头哥,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : sentry_state_config.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
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
/* -------- 敌人状况标志位 相应的位置1，既有，0则无--------- */
#define  CHASSIS_STATE    (0x00000000U)
 #define HAVE_ENEMY       (0x80000000U)//有敌人
 #define HURTED           (0x40000000U)//被攻击
 #define FINE_ENEMY       (0x20000000U)//发现敌人
 #define ENEMY_POS        (0x0F000000U)//敌人位置位
 #define IN_ROAD          (0x08000000U)//敌人在公路
 #define IN_BASE          (0x04000000U)//敌人在基地
 #define IN_WILD_AREA     (0x02000000U)//敌人在野区
 #define IN_BLOCK_HOUSE   (0x01000000U)//敌人在碉堡
 #define GAMING_BEGIN     (0x00800000U)//比赛模式开始
 #define DEBUG_BEGIN      (0x00400000U)//调试模式开始
 #define MOD_BEGIN_CHECK  (0x00F00000U)//模式选择
 #define SELF_AREA        (0x000000FFU)//自身位置状态
 #define LAST_AREA        (0x000000F0U)//上次位置状态
 #define REAL_AREA        (0x0000000FU)//这次位置状态
 #define UP_ROAD          (0x00000001U)//上路靠启动区
 #define MID_ROAD         (0x00000002U)//上路靠启动区//直道中路
 #define DOWN_ROAD        (0x00000003U)//下路靠桥
 #define CORRET_ENCODE    (0x00000100U)//初始化校准位
 #define OUT_OF_CONTROL   (0x00000200U)//失控位起始
 #define OUT_OF__OVER     (0x0000F00U)//失控位 失控次数14次之后，进入完全失控模式
 #define AUTO_CONTROL_OK  (0x00000000U)//正常运行为
 #define HURT_RX_CLEAR    5
 
 #define  AHEAD_OF_ARMOR          0//前装甲0
	#define  BACK_OF_ARMOR           1 //前装甲1
#define  SET_STATE(_state_)     (SET_BIT(sentry_state,_state_))
#define  CLEAR_STATE(_state_)   (CLEAR_BIT(sentry_state,_state_))
#define  IS_STATE(_state_)                (GET_BYTE(sentry_state,_state_) == _state_)
#define  GET_CHASSIS_SELF_POS(_s_)                 GET_BYTE(sentry_state,_s_) // _s_ 或REAL_AREA
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


