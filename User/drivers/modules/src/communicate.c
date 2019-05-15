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
	|--FileName    : communicate.c                                              
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
#include "communicate.h" 
#define COMMUN_RX_BUFF 200
#define _SOF       0xA5 
#define ROBOT_START    0x201
#define THE_POWER_OF_HEAT   0x202
#define  ROBOT_POSITION     0x203
#define ROBOT_GAIN        0x204
#define THE_DAMAGE_STATE 0x206
#define REAL_TIME_SHOOTING_INFORMATION   0x207
#define AIR_PLANE_DATA  0x301
#define GAME_STATE     0x001

//整个数据帧的长度，
//FrameHeader(5-Byte)+
//CmdID(2-Byte)+Data(n-Byte)+
//FrameTail(2-Byte, CRC16, 整包校验)
#define REF_HEADER_CRC_CMDID_LEN  9
#define ROBOT_START_LEN    (REF_HEADER_CRC_CMDID_LEN + 15)
#define THE_POWER_OF_HEAT_LEN   (REF_HEADER_CRC_CMDID_LEN + 14)
#define  ROBOT_POSITION_LEN     (REF_HEADER_CRC_CMDID_LEN + 16)
#define ROBOT_GAIN_LEN        (REF_HEADER_CRC_CMDID_LEN + 1)
#define THE_DAMAGE_STATE_LEN   (REF_HEADER_CRC_CMDID_LEN + 1)
#define REAL_TIME_SHOOTING_INFORMATION_LEN   (REF_HEADER_CRC_CMDID_LEN + 6)
#define AIR_PLANE_DATA_LEN   (REF_HEADER_CRC_CMDID_LEN + 6+1)
#define GAME_STATE_DATA_LEN    (REF_HEADER_CRC_CMDID_LEN + 3)
refereeSystemStruct ext_refereeSystem_t;
ext_game_robot_state_t robot_state_t;
ext_power_heat_data_t power_heat_data_t;
ext_game_robot_pos_t  game_robot_pos_t;
ext_buff_musk_t  buff_musk_t;
ext_robot_hurt_t robot_hurt_t;
ext_shoot_data_t shoot_data_t;
ext_student_interactive_header_data_t air_plane_data_t;
 ext_game_state_t game_state_t;
uint8_t reebuff[COMMUN_RX_BUFF]  = {0};
uint16_t cmd_id =0;
#if RED_GAME
RobotId_t seftRobotID_t = {
  .Hero=0x01,
	.Engineer=0x02,
	.Standard_3=0x03,
	.Standard_4=0x04,
	.Standard_5=0x05,
	.Aerial=0x06,
	.Sentry=0x07,
};
#elif BLUE_GAME
RobotId_t seftRobotID_t = {
	.Hero=0x0011,
	.Engineer=0x0012,
	.Standard_3=0x0013,
	.Standard_4=0x0014,
	.Standard_5=0x0015,
	.Aerial=0x0016,
	.Sentry=0x0017,
  };
#endif
//client_id_t client_id = {
//	.Red_Hero=0x0101,
//	.Red_Engineer=0x0102,
//	.Red_Standard_3=0x0103,
//	.Red_Standard_4=0x0104,
//	.Red_Standard_5=0x0105,
//	.Red_Aerial=0x0106,
//	
//	.Blue_Hero=0x0111,
//	.Blue_Engineer=0x0112,
//	.Blue_Standard_3=0x0113,
//	.Blue_Standard_4=0x0114,
//	.Blue_Standard_5=0x0115,
//	.Blue_Aerial=0x0116,
//};
  /**
  * @Data    2019-03-23 23:25
  * @brief   裁判系统初始化
  * @param   void
  * @retval  void
  */
void CommunicateInit(void)
{
  UsartAndDMAInit(COMMUNICAT,COMMUN_RX_BUFF,ENABLE);
  ext_refereeSystem_t. p_robot_state_t  = &robot_state_t;
  SetFpsAddress(robot_state_t.fps);
  ext_refereeSystem_t. p_power_heat_data_t  = &power_heat_data_t;
  SetFpsAddress(power_heat_data_t.fps);
  ext_refereeSystem_t. p_game_robot_pos_t  = &game_robot_pos_t;
  SetFpsAddress(game_robot_pos_t.fps);
  ext_refereeSystem_t. p_buff_musk_t  = &buff_musk_t;
  SetFpsAddress(buff_musk_t.fps);
  ext_refereeSystem_t. p_robot_hurt_t  = &robot_hurt_t;
  robot_hurt_t.hurt_type = 5;
  SetFpsAddress(robot_hurt_t.fps);
  ext_refereeSystem_t. p_shoot_data_t  = &shoot_data_t;
  SetFpsAddress(shoot_data_t.fps);
   ext_refereeSystem_t.p_air_plane_data_t= &air_plane_data_t;
ext_refereeSystem_t.p_ext_game_state_t = &game_state_t;
  ext_refereeSystem_t.pseftRobotID_t = &seftRobotID_t;
}
  /**
  * @Data    2019-03-23 23:52
  * @brief   裁判系统数据解析
  * @param   void
  * @retval  void
  */
  void CommunicateParse(void)
  {
    uint16_t datalen;
    floatToUnion p;
   if(UserUsartQueueRX(COMMUNICAT,reebuff) ==HAL_OK)
   {
     p.u_8[DATA_LEN_BYTE_HIGH_8] = reebuff[DATA_LEN_BYTE_HIGH_8];
     p.u_8[DATA_LEN_BYTE_LOW_8] =  reebuff[DATA_LEN_BYTE_LOW_8];
     datalen =  p.u_16[0];
     for(int i=0;i< datalen;)
     {
       if(reebuff[i +DATA_LEN_BYTE_LEN] ==_SOF)
       {
				  cmd_id = GetCmdId(&reebuff[i+5+DATA_LEN_BYTE_LEN]);
         switch (cmd_id) 
				 {
           case GAME_STATE:
					 if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],GAME_STATE_DATA_LEN))
					 {
							GameData(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
							i +=GAME_STATE_DATA_LEN;
					 }
					 else i++;
             break;
					 case ROBOT_START:
					 if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],ROBOT_START_LEN))
					 {
							Robotstateparse(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
							i +=ROBOT_START_LEN;
             Fps(robot_state_t.fps);
					 }
					 else i++;
						 break;
					 case THE_POWER_OF_HEAT:
				 if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],THE_POWER_OF_HEAT_LEN))
					 {
						  PowerHeatDataParse(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
							i += THE_POWER_OF_HEAT_LEN;
             Fps(power_heat_data_t.fps);
					 }
					 	 else i++;
						 break;
					 case ROBOT_POSITION:
					 if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],ROBOT_POSITION_LEN))
					 {
						  GameRobotPosParse(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
							i += ROBOT_POSITION_LEN;
             Fps(game_robot_pos_t.fps);
					 }
					 	 else i++;
						 break;
				  case ROBOT_GAIN:
					 if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],ROBOT_GAIN_LEN))
					 {
						 BuffMusk(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
						i += ROBOT_GAIN_LEN;
             Fps(buff_musk_t.fps);
					 }
					 	 else i++;
					 	 break;
					 case THE_DAMAGE_STATE:
					 if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],THE_DAMAGE_STATE_LEN))
					 {
						RobotHurt(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
						i += THE_DAMAGE_STATE_LEN;
             Fps(robot_hurt_t.fps);
					 }
					 	 else i++;
					 	 break;
				  case REAL_TIME_SHOOTING_INFORMATION:
					 if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],REAL_TIME_SHOOTING_INFORMATION_LEN))
					 {
						ShootData(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
						i += REAL_TIME_SHOOTING_INFORMATION_LEN;
             Fps(shoot_data_t.fps);
					 }
					 	 else i++;
							 break;
         case AIR_PLANE_DATA:
         if(ref_verify_crc16(&reebuff[i+DATA_LEN_BYTE_LEN],AIR_PLANE_DATA_LEN))
					 {
						AirPlane(&reebuff[i + 7+DATA_LEN_BYTE_LEN]);
						i += AIR_PLANE_DATA_LEN;
           //  Fps(shoot_data_t.fps);
					 }  
            else i++;
             break;
					 default:
              i++;
						 break;
				 }    
       }
			 else i++;
     }
   }
  }
/**
	* @Data    2019-03-24 14:03
	* @brief   比赛机器人状态解析
	* @param   void
	* @retval  void
	*/
	void Robotstateparse(uint8_t *data)
	{
		robot_state_t.robot_level = *(data+1);
		robot_state_t.remain_HP =                   (uint16_t)((*(data+3))<<8 |(*(data+2)));
		robot_state_t.max_HP =                      (uint16_t)((*(data+5))<<8 |(*(data+4)));
		robot_state_t.shooter_heat0_cooling_rate =  (uint16_t)((*(data+7))<<8 |(*(data+6)));
		robot_state_t.shooter_heat0_cooling_limit = (uint16_t)((*(data+9))<<8 |(*(data+8)));
	}
/**
	* @Data    2019-03-24 14:18
	* @brief   实时功率热量数据解析
	* @param   void
	* @retval  void
	*/
	void PowerHeatDataParse(uint8_t *data)
	{
		power_heat_data_t.chassis_volt =          	(uint16_t)((*(data+1))<<8 |(*(data+0)));
		power_heat_data_t.chassis_current =	        (uint16_t)((*(data+3))<<8 |(*(data+2)));
		power_heat_data_t.chassis_power =  				  (uint16_t)((*(data+7))<<24 |(*(data+6))<<16 | (*(data+5))<<8 | (*(data+4)));
		power_heat_data_t.chassis_power_buffer = 		(uint16_t)((*(data+9))<<8 |(*(data+8)));
		power_heat_data_t.shooter_heat0 = 					(uint16_t)((*(data+11))<<8 |(*(data+10)));
	}
/**
	* @Data    2019-03-24 14:02
	* @brief   机器人位置//看真实裁判系统数据对不对
	* @param   void
	* @retval  void
	*/ 
	void GameRobotPosParse(uint8_t *data)
	{
    floatToUnion p;
    p.u_8[0] = *(data+0);//如果不对，则是3210
    p.u_8[1] = *(data+1);
    p.u_8[2] = *(data+2);
    p.u_8[3] = *(data+3);
		game_robot_pos_t.x = p.f;
    p.f = 0;
    p.u_8[0] = *(data+4);
    p.u_8[1] = *(data+5);
    p.u_8[2] = *(data+6);
    p.u_8[3] = *(data+7);
		game_robot_pos_t.y =  p.f;
    p.f = 0;
    p.u_8[0] = *(data+8);
    p.u_8[1] = *(data+9);
    p.u_8[2] = *(data+10);
    p.u_8[3] = *(data+11);
		game_robot_pos_t.yaw =  p.f;
		game_robot_pos_t.z   = (float)((*(data+12))<<8 |(*(data+13)));
	}
/**
	* @Data    2019-03-24 14:42
	* @brief   机器人增益
	* @param   void
	* @retval  void
	*/
	void BuffMusk(uint8_t *data)
	{
		buff_musk_t.power_rune_buff = *data;
	}
/**
	* @Data    2019-03-24 14:49
	* @brief   伤害状态
	* @param   void
	* @retval  void
	*/
	void RobotHurt(uint8_t *data)
	{
		robot_hurt_t.armor_id =  (*data) &0x0f;
		robot_hurt_t.hurt_type = (*data) >>4 &0x0f; 
	}
/**
	* @Data    2019-03-24 14:54
	* @brief   实时射击信
	* @param   void
	* @retval  void
	*/ 
	void ShootData(uint8_t *data)
	{
    floatToUnion m1;
    m1.u_8[0] = *(data+2);
    m1.u_8[1] = *(data+3);
    m1.u_8[2] = *(data+4);
    m1.u_8[3] = *(data+5);
		shoot_data_t.bullet_type = *(data) ;
		shoot_data_t.bullet_freq =  *(data+1);
		shoot_data_t.bullet_speed =  m1.f;
	}
/**
	* @Data    2019-03-24 14:54
	* @brief   无人机数据
	* @param   void
	* @retval  void
	*/ 
void AirPlane(uint8_t *data)
{
  floatToUnion m1;
    m1.u_8[0] = *(data);
    m1.u_8[1] = *(data+1);
    m1.u_8[2] = *(data+2);
    m1.u_8[3] = *(data+3);
  air_plane_data_t.data_cmd_id = m1.u_16[0];
  air_plane_data_t.send_ID = m1.u_16[1];
    m1.u_32=0;
    m1.u_8[0] = *(data+4);
    m1.u_8[1] = *(data+5);
    m1.u_8[2] = *(data+6);
  air_plane_data_t.receiver_ID = m1.u_16[0];
  if(air_plane_data_t.data_cmd_id ==ext_refereeSystem_t.pseftRobotID_t->Aerial \
      && air_plane_data_t.receiver_ID == ext_refereeSystem_t.pseftRobotID_t->Sentry)
  {
      air_plane_data_t.commot =  m1.u_8[2];
  }

}
/**
	* @Data    2019-03-24 14:54
	* @brief   比赛状态
	* @param   void
	* @retval  void
	*/ 
	void GameData(uint8_t *data)
	{
    game_state_t.game_progress =  RIGHT_SH(*data,4) & 0x0f;
	}
/**
	* @Data    2019-03-24 13:38
	* @brief   获取数据id
	* @param   void
	* @retval  void
	*/
	uint16_t GetCmdId(uint8_t *data)
	{
		return (uint16_t)((*(data+1))<<8 |(*data));
	}
static const uint8_t ref_crc8_init = 0xff;
static const uint8_t ref_crc8_tab[256] =
{
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};


uint8_t ref_get_crc8(uint8_t *p_msg, unsigned int len, uint8_t crc8)
{
    uint8_t uc_index;

    while (len--)
    {
        uc_index = crc8^(*p_msg++);
        crc8  = ref_crc8_tab[uc_index];
    }

    return(crc8);
}


/*
**  Descriptions: CRC8 Verify function
**  Input:        Data to Verify,Stream length = Data + checksum
**  Output:       True or False (CRC Verify Result)
*/
unsigned int ref_verify_crc8(uint8_t *p_msg, unsigned int len)
{
    uint8_t uc_expected = 0;

    if ((p_msg == 0) || (len <= 2)) return 0;

    uc_expected = ref_get_crc8 (p_msg, len-1, ref_crc8_init);

    return ( uc_expected == p_msg[len-1] );
}


/*
**  Descriptions: append CRC8 to the end of data
**  Input:        Data to CRC and append,Stream length = Data + checksum
**  Output:       True or False (CRC Verify Result)
*/
void ref_append_crc8(uint8_t *p_msg, unsigned int len)
{
    uint8_t crc8 = 0;

    if ((p_msg == 0) || (len <= 2)) return;

    crc8 = ref_get_crc8 ( (uint8_t *)p_msg, len-1, ref_crc8_init);

    p_msg[len-1] = crc8;
}


static uint16_t ref_crc16_init = 0xffff;
static const uint16_t ref_crc16_tab[256] =
    {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};



/*
**  Descriptions: CRC16 checksum function
**  Input:        Data to check,Stream length, initialized checksum
**  Output:       CRC checksum
*/
uint16_t ref_get_crc16(uint8_t *p_msg, uint16_t len, uint16_t crc16)
{
    uint8_t data;
    
    if (p_msg == NULL)
    {
        return 0xFFFF;
    }

    while(len--)
    {
        data = *p_msg++;
        (crc16) = ((uint16_t)(crc16) >> 8)  ^ ref_crc16_tab[((uint16_t)(crc16) ^ (uint16_t)(data)) & 0x00ff];
    }

    return crc16;
}

/*
**  Descriptions: CRC16 Verify function
**  Input:        Data to Verify,Stream length = Data + checksum
**  Output:       True or False (CRC Verify Result)
*/
uint32_t ref_verify_crc16(uint8_t *p_msg, uint16_t len)
{
    uint16_t w_expected = 0;

    if ((p_msg == NULL) || (len <= 2))
    {
        return 1;
    }
    w_expected = ref_get_crc16 ( p_msg, len - 2, ref_crc16_init);

    return ((w_expected & 0xff) == p_msg[len - 2] && ((w_expected >> 8) & 0xff) == p_msg[len - 1]);
}

/*
**  Descriptions: append CRC16 to the end of data
**  Input:        Data to CRC and append,Stream length = Data + checksum
**  Output:       True or False (CRC Verify Result)
*/
void ref_append_crc16(uint8_t* p_msg, uint32_t len)
{
    uint16_t crc16 = 0;

    if ((p_msg == NULL) || (len <= 2))
    {
        return;
    }
    crc16 = ref_get_crc16 ( (uint8_t *)p_msg, len-2, ref_crc16_init );

    p_msg[len-2] = (uint8_t)(crc16 & 0x00ff);
    p_msg[len-1] = (uint8_t)((crc16 >> 8)& 0x00ff);
}




  //==-------------------------------------------
//  #define REFEREE_FIFO_BUFLEN 200

//extern osThreadId communicate_task_t;

//static unpack_data_t referee_unpack_obj;
//static uint8_t   referee_rxdata_buf[REFEREE_FIFO_BUFLEN];

//static ref_send_handler_t ref_protocol_send;
//static uint8_t ref_seq_num;

//void referee_param_init(void)
//{

//  /* initial judge data unpack object */
//  referee_unpack_obj.p_header = (frame_header_t *)referee_unpack_obj.protocol_packet;
//  referee_unpack_obj.index = 0;
//  referee_unpack_obj.data_len = 0;
//  referee_unpack_obj.unpack_step = STEP_HEADER_SOF;
//}

//static uint16_t referee_protocol_pack(uint8_t *p_out, uint8_t *p_in, uint16_t len, uint16_t cmd_id)
//{
//	uint16_t headSize = REF_PROTOCOL_HEADER_SIZE;
//	uint16_t frameSize = len + REF_HEADER_CRC_CMDID_LEN;

//	memcpy(p_out + headSize, &cmd_id, sizeof(cmd_id));
//	ref_append_crc8(p_out, headSize);
//	memcpy(p_out + headSize + sizeof(cmd_id), p_in, len);
//	ref_append_crc16(p_out, frameSize);

//	return frameSize;
//}

///**
//*@func:     ExtProtocolTransmit
//*@brief:    pc send protocol to main control
//*@param:    cmd_id: the protocol cmd id
//*           pdata: the point about data buffer
//*           len: the length of the data buffer
//*/
//void referee_protocol_tansmit(uint16_t cmd_id, void* p_buf, uint16_t len)
//{
//	uint8_t txBuf[REF_PROTOCOL_FRAME_MAX_SIZE]={0};
//	frame_header_t *pHeader = (frame_header_t *)txBuf;
//	uint16_t frameSize;

//	pHeader->sof                    = REF_PROTOCOL_HEADER;
//	pHeader->data_length            = len;
//	pHeader->seq                    = ref_seq_num++;

//	frameSize = referee_protocol_pack(txBuf, p_buf, len, cmd_id);
//	if(ref_protocol_send != NULL)
//	{
//		ref_protocol_send(txBuf, frameSize);
//	}
//}

//uint32_t referee_send_data_register(ref_send_handler_t send_t)
//{
//	ref_protocol_send = send_t;
//	return 0;
//}


//void referee_data_handler(uint8_t *p_frame)
//{
//  frame_header_t *p_header = (frame_header_t*)p_frame;
//  memcpy(p_header, p_frame, REF_PROTOCOL_HEADER_SIZE);

//  uint16_t data_length = p_header->data_length;
//  uint16_t cmd_id      = *(uint16_t *)(p_frame + REF_PROTOCOL_HEADER_SIZE);
//  uint8_t *data_addr   = p_frame + REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CMD_SIZE;
//  
//  protocol_send(MANIFOLD2_ADDRESS, cmd_id + 0x4000, data_addr, data_length);
//}

//void referee_unpack_fifo_data(void)
//{
//  uint8_t byte = 0;
//  uint8_t sof = REF_PROTOCOL_HEADER;
//  unpack_data_t *p_obj = &referee_unpack_obj;

//  while ( fifo_s_used(p_obj->data_fifo) )
//  {
//    byte = fifo_s_get(p_obj->data_fifo);
//    switch(p_obj->unpack_step)
//    {
//      case STEP_HEADER_SOF:
//      {
//        if(byte == sof)
//        {
//          p_obj->unpack_step = STEP_LENGTH_LOW;
//          p_obj->protocol_packet[p_obj->index++] = byte;
//        }
//        else
//        {
//          p_obj->index = 0;
//        }
//      }break;
//      
//      case STEP_LENGTH_LOW:
//      {
//        p_obj->data_len = byte;
//        p_obj->protocol_packet[p_obj->index++] = byte;
//        p_obj->unpack_step = STEP_LENGTH_HIGH;
//      }break;
//      
//      case STEP_LENGTH_HIGH:
//      {
//        p_obj->data_len |= (byte << 8);
//        p_obj->protocol_packet[p_obj->index++] = byte;

//        if(p_obj->data_len < (REF_PROTOCOL_FRAME_MAX_SIZE - REF_HEADER_CRC_CMDID_LEN))
//        {
//          p_obj->unpack_step = STEP_FRAME_SEQ;
//        }
//        else
//        {
//          p_obj->unpack_step = STEP_HEADER_SOF;
//          p_obj->index = 0;
//        }
//      }break;
//    
//      case STEP_FRAME_SEQ:
//      {
//        p_obj->protocol_packet[p_obj->index++] = byte;
//        p_obj->unpack_step = STEP_HEADER_CRC8;
//      }break;

//      case STEP_HEADER_CRC8:
//      {
//        p_obj->protocol_packet[p_obj->index++] = byte;

//        if (p_obj->index == REF_PROTOCOL_HEADER_SIZE)
//        {
//          if ( ref_verify_crc8(p_obj->protocol_packet, REF_PROTOCOL_HEADER_SIZE) )
//          {
//            p_obj->unpack_step = STEP_DATA_CRC16;
//          }
//          else
//          {
//            p_obj->unpack_step = STEP_HEADER_SOF;
//            p_obj->index = 0;
//          }
//        }
//      }break;  

//      case STEP_DATA_CRC16:
//      {
//        if (p_obj->index < (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
//        {
//           p_obj->protocol_packet[p_obj->index++] = byte;  
//        }
//        if (p_obj->index >= (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
//        {
//          p_obj->unpack_step = STEP_HEADER_SOF;
//          p_obj->index = 0;

//          if ( ref_verify_crc16(p_obj->protocol_packet, REF_HEADER_CRC_CMDID_LEN + p_obj->data_len) )
//          {
//            referee_data_handler(p_obj->protocol_packet);
//          }
//        }
//      }break;

//      default:
//      {
//        p_obj->unpack_step = STEP_HEADER_SOF;
//        p_obj->index = 0;
//      }break;
//    }
//  }
//}

//static const uint8_t ref_crc8_init = 0xff;
//static const uint8_t ref_crc8_tab[256] =
//{
//    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
//    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
//    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
//    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
//    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
//    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
//    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
//    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
//    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
//    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
//    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
//    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
//    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
//    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
//    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
//    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
//};


//uint8_t ref_get_crc8(uint8_t *p_msg, unsigned int len, uint8_t crc8)
//{
//    uint8_t uc_index;

//    while (len--)
//    {
//        uc_index = crc8^(*p_msg++);
//        crc8  = ref_crc8_tab[uc_index];
//    }

//    return(crc8);
//}


///*
//**  Descriptions: CRC8 Verify function
//**  Input:        Data to Verify,Stream length = Data + checksum
//**  Output:       True or False (CRC Verify Result)
//*/
//unsigned int ref_verify_crc8(uint8_t *p_msg, unsigned int len)
//{
//    uint8_t uc_expected = 0;

//    if ((p_msg == 0) || (len <= 2)) return 0;

//    uc_expected = ref_get_crc8 (p_msg, len-1, ref_crc8_init);

//    return ( uc_expected == p_msg[len-1] );
//}


///*
//**  Descriptions: append CRC8 to the end of data
//**  Input:        Data to CRC and append,Stream length = Data + checksum
//**  Output:       True or False (CRC Verify Result)
//*/
//void ref_append_crc8(uint8_t *p_msg, unsigned int len)
//{
//    uint8_t crc8 = 0;

//    if ((p_msg == 0) || (len <= 2)) return;

//    crc8 = ref_get_crc8 ( (uint8_t *)p_msg, len-1, ref_crc8_init);

//    p_msg[len-1] = crc8;
//}


//static uint16_t ref_crc16_init = 0xffff;
//static const uint16_t ref_crc16_tab[256] =
//    {
//        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
//        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
//        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
//        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
//        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
//        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
//        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
//        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
//        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
//        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
//        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
//        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
//        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
//        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
//        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
//        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
//        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
//        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
//        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
//        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
//        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
//        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
//        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
//        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
//        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
//        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
//        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
//        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
//        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
//        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
//        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
//        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
//};



///*
//**  Descriptions: CRC16 checksum function
//**  Input:        Data to check,Stream length, initialized checksum
//**  Output:       CRC checksum
//*/
//uint16_t ref_get_crc16(uint8_t *p_msg, uint16_t len, uint16_t crc16)
//{
//    uint8_t data;
//    
//    if (p_msg == NULL)
//    {
//        return 0xFFFF;
//    }

//    while(len--)
//    {
//        data = *p_msg++;
//        (crc16) = ((uint16_t)(crc16) >> 8)  ^ ref_crc16_tab[((uint16_t)(crc16) ^ (uint16_t)(data)) & 0x00ff];
//    }

//    return crc16;
//}

///*
//**  Descriptions: CRC16 Verify function
//**  Input:        Data to Verify,Stream length = Data + checksum
//**  Output:       True or False (CRC Verify Result)
//*/
//uint32_t ref_verify_crc16(uint8_t *p_msg, uint16_t len)
//{
//    uint16_t w_expected = 0;

//    if ((p_msg == NULL) || (len <= 2))
//    {
//        return 1;
//    }
//    w_expected = ref_get_crc16 ( p_msg, len - 2, ref_crc16_init);

//    return ((w_expected & 0xff) == p_msg[len - 2] && ((w_expected >> 8) & 0xff) == p_msg[len - 1]);
//}

///*
//**  Descriptions: append CRC16 to the end of data
//**  Input:        Data to CRC and append,Stream length = Data + checksum
//**  Output:       True or False (CRC Verify Result)
//*/
//void ref_append_crc16(uint8_t* p_msg, uint32_t len)
//{
//    uint16_t crc16 = 0;

//    if ((p_msg == NULL) || (len <= 2))
//    {
//        return;
//    }
//    crc16 = ref_get_crc16 ( (uint8_t *)p_msg, len-2, ref_crc16_init );

//    p_msg[len-2] = (uint8_t)(crc16 & 0x00ff);
//    p_msg[len-1] = (uint8_t)((crc16 >> 8)& 0x00ff);
//}





/*-----------------------------------file of end------------------------------*/


