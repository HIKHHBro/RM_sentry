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

typedef void (*ref_send_handler_t)(uint8_t* buf, uint16_t len);


// #define REF_PROTOCOL_HEADER_SIZE            sizeof(frame_header_t)
// #define REF_PROTOCOL_CMD_SIZE               2
// #define REF_PROTOCOL_CRC16_SIZE             2
// #define REF_HEADER_CRC_LEN                  (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE)
// #define REF_HEADER_CRC_CMDID_LEN            (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE + sizeof(uint16_t))
// #define REF_HEADER_CMDID_LEN                (REF_PROTOCOL_HEADER_SIZE + sizeof(uint16_t))

// #define REF_PROTOCOL_FRAME_MAX_SIZE         128
// #define REF_PROTOCOL_CMD_MAX_NUM            20

// #define REF_USER_TO_SERVER_MAX_DATA_LEN     64
// #define REF_SERVER_TO_USER_MAX_DATA_LEN     32
void CommunicateInit(void);
  void CommunicateParse(void);
	uint16_t GetCmdId(uint8_t *data);
	void Robotstateparse(uint8_t *data);
	void PowerHeatDataParse(uint8_t *data);
		void GameRobotPosParse(uint8_t *data);
			void BuffMusk(uint8_t *data);
				void RobotHurt(uint8_t *data);
					void ShootData(uint8_t *data);



// #pragma pack(push,1)

// typedef struct
// {
//   uint8_t  sof;
//   uint16_t data_length;
//   uint8_t  seq;
//   uint8_t  crc8;
// } frame_header_t;



// void referee_param_init(void);
// void referee_unpack_fifo_data(void);
// uint32_t referee_uart_rx_data_handle(uint8_t *data, uint32_t len);
// uint32_t referee_send_data_register(ref_send_handler_t send_t);
// void referee_protocol_tansmit(uint16_t cmd_id, void* p_buf, uint16_t len);
	
// uint8_t     ref_get_crc8(uint8_t *p_msg, uint32_t len, uint8_t crc8) ;
// uint32_t    ref_verify_crc8(uint8_t *p_msg, uint32_t len);
// void        ref_append_crc8(uint8_t *p_msg, uint32_t len);
uint16_t    ref_get_crc16(uint8_t *p_msg, uint16_t len, uint16_t crc16);
uint32_t    ref_verify_crc16(uint8_t *p_msg, uint16_t len);
void        ref_append_crc16(uint8_t* p_msg, uint32_t len) ;

// #define SOF   
// #define ROBOT_STATE
// #define REAL_POWER  0x202//实时功率热量数据，50Hz周期发送
// #define GAIN_STATE 0x204//增益值
// #define  DAMAGESTATE 0x206 //伤害状态
// #define  FIRING_DATA 0x207//实时射击数据
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
} ext_game_robot_state_t;
typedef  struct ext_power_heat_data_t
{ 
  uint16_t chassis_volt; 
  uint16_t chassis_current; 
  float chassis_power; 
  uint16_t chassis_power_buffer; 
  uint16_t shooter_heat0;
  uint16_t shooter_heat1; 
} ext_power_heat_data_t;
typedef  struct ext_game_robot_pos_t
{
  float x; 
  float y; 
  float z; 
  float yaw; 
} ext_game_robot_pos_t;
typedef  struct ext_buff_musk_t
{ 
  uint8_t power_rune_buff;
}ext_buff_musk_t;
typedef  struct ext_robot_hurt_t
{ 
  uint8_t armor_id; 
  uint8_t hurt_type; 
} ext_robot_hurt_t;
typedef  struct shoot_data_t
{ 
  uint8_t bullet_type; 
  uint8_t bullet_freq; 
  float bullet_speed; 
} ext_shoot_data_t;
typedef  struct refereeSystemStruct
{
 ext_game_robot_state_t* p_robot_state_t;
 ext_power_heat_data_t *p_power_heat_data_t;
 ext_game_robot_pos_t*  p_game_robot_pos_t;
 ext_buff_musk_t * p_buff_musk_t;
 ext_robot_hurt_t* p_robot_hurt_t;
 ext_shoot_data_t* p_shoot_data_t;
 uint16_t datalen;
}refereeSystemStruct;
extern refereeSystemStruct ext_refereeSystem_t;
//extern ext_game_robot_state_t robot_state_t;
//extern ext_power_heat_data_t power_heat_data_t;
//extern ext_game_robot_pos_t  game_robot_pos_t;
//extern ext_buff_musk_t  buff_musk_t;
//extern ext_robot_hurt_t robot_hurt_t;
//extern ext_shoot_data_t shoot_data_t;
#endif	// __COMMUNICATE_H
/*-----------------------------------file of end------------------------------*/


