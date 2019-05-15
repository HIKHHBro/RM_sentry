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
	|--FileName    : communicate.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-03-23               
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
#ifndef __COMMUNICATE_H 
#define __COMMUNICATE_H 
#include "bsp_usart.h"
#define A_HOUSE 1
#define BRIGTH 2
#define ROAD   3
#define BASE    4
#define WILD    5
typedef void (*ref_send_handler_t)(uint8_t* buf, uint16_t len);

  void CommunicateInit(void);
  void CommunicateParse(void);
  uint16_t GetCmdId(uint8_t *data);
  void Robotstateparse(uint8_t *data);
  void PowerHeatDataParse(uint8_t *data);
  void GameRobotPosParse(uint8_t *data);
  void BuffMusk(uint8_t *data);
  void RobotHurt(uint8_t *data);
  void ShootData(uint8_t *data);
  	void GameData(uint8_t *data);
void AirPlane(uint8_t *data);
uint16_t    ref_get_crc16(uint8_t *p_msg, uint16_t len, uint16_t crc16);
uint32_t    ref_verify_crc16(uint8_t *p_msg, uint16_t len);
void        ref_append_crc16(uint8_t* p_msg, uint32_t len) ;


typedef struct
{
  uint8_t Hero;
	uint8_t Engineer;
	uint8_t Standard_3;
	uint8_t Standard_4;
	uint8_t Standard_5;
	uint8_t Aerial;
	uint8_t Sentry;
}RobotId_t;

typedef  struct ext_game_robot_state_t
{ 
  uint8_t robot_id; 
  uint8_t robot_level;
  uint16_t remain_HP;
  uint16_t max_HP;
  uint16_t shooter_heat0_cooling_rate;
  uint16_t shooter_heat0_cooling_limit;
  uint16_t shooter_heat1_cooling_rate;
  uint16_t shooter_heat1_cooling_limit; 
  uint8_t mains_power_gimbal_output : 1; 
  uint8_t mains_power_chassis_output : 1; 
  uint8_t mains_power_shooter_output : 1; 
  fps_t fps;
} ext_game_robot_state_t;
typedef  struct ext_power_heat_data_t
{ 
  uint16_t chassis_volt; 
  uint16_t chassis_current; 
  float chassis_power; 
  uint16_t chassis_power_buffer; 
  uint16_t shooter_heat0;
  uint16_t shooter_heat1; 
    fps_t fps;
} ext_power_heat_data_t;
typedef  struct ext_game_robot_pos_t
{
  float x; 
  float y; 
  float z; 
  float yaw; 
    fps_t fps;
} ext_game_robot_pos_t;
typedef  struct ext_buff_musk_t
{ 
  uint8_t power_rune_buff;
    fps_t fps;
}ext_buff_musk_t;
typedef  struct ext_robot_hurt_t
{ 
  uint8_t armor_id; 
  uint8_t hurt_type; 
    fps_t fps;
} ext_robot_hurt_t;
typedef  struct shoot_data_t
{ 
  uint8_t bullet_type; 
  uint8_t bullet_freq; 
  float bullet_speed; 
    fps_t fps;
} ext_shoot_data_t;
typedef __packed struct
{
uint16_t data_cmd_id;
uint16_t send_ID;
uint16_t receiver_ID;
  uint8_t commot;
}ext_student_interactive_header_data_t;
typedef __packed struct ext_game_state_t
{ 
  uint8_t game_type; 
  uint8_t game_progress;
  uint16_t stage_remain_time; 
} ext_game_state_t;
typedef  struct refereeSystemStruct
{
 ext_game_robot_state_t* p_robot_state_t;
 ext_power_heat_data_t *p_power_heat_data_t;
 ext_game_robot_pos_t*  p_game_robot_pos_t;
 ext_buff_musk_t * p_buff_musk_t;
 ext_robot_hurt_t* p_robot_hurt_t;
 ext_shoot_data_t* p_shoot_data_t;
  ext_student_interactive_header_data_t *p_air_plane_data_t;
  ext_game_state_t *p_ext_game_state_t;
 uint16_t datalen;
 RobotId_t* pseftRobotID_t;
 fps_t fps;
}refereeSystemStruct;

extern refereeSystemStruct ext_refereeSystem_t;




typedef struct
{
  uint16_t Red_Hero;
	uint16_t Red_Engineer;
	uint16_t Red_Standard_3;
	uint16_t Red_Standard_4;
	uint16_t Red_Standard_5;
	uint16_t Red_Aerial;
	
	uint16_t Blue_Hero;
	uint16_t Blue_Engineer;
	uint16_t Blue_Standard_3;
	uint16_t Blue_Standard_4;
	uint16_t Blue_Standard_5;
	uint16_t Blue_Aerial;
}client_id_t;
#endif	// __COMMUNICATE_H
/*-----------------------------------file of end------------------------------*/


