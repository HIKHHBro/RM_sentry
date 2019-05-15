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
	|--FileName    : sentry_state.c                                              
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
#include "sentry_state.h" 
uint32_t sentry_state=0;
	extern chassisStruct chassis_t;
  extern	gimbalStruct gimbal_t;//云台结构体
   /**
	* @Data    2019-03-30 22:13
	* @brief   获取运行中所有状态
	* @param   void
	* @retval  void
	*/ 
  int16_t lefts,rights;
void GetState(void)
{
  if(chassis_t.p_refereeSystem_t->p_ext_game_state_t->game_progress == FIGHTING)
  {
    CLEAR_STATE(MOD_BEGIN_CHECK);
    SET_STATE(GAMING_BEGIN);
  }
#ifdef DEBUG_BY
      CLEAR_STATE(MOD_BEGIN_CHECK);
    SET_STATE(DEBUG_BEGIN);//比赛时把这个注释掉
  #endif
  SetArea();//获取机器人位置
  GetHurted();//获取机器人被攻击状态
    lefts = (int16_t)GetDistance(3);
  rights = (int16_t)GetDistance(4);
  if(gimbal_t.pPc_t->commot ==1)
  {
    SET_STATE(FINE_ENEMY|HAVE_ENEMY);
  }
  else CLEAR_STATE(FINE_ENEMY);
}
/**
	* @Data    2019-03-25 19:16
	* @brief   获取机器人被攻击的状态
	* @param   void
	* @retval  void
	*/
void GetHurted(void)
{
  if(IS_CHASSIS_STATUS(CHASSIS_PC_SHOOT_MODE_RUNING) && \
    IS_BIMBAL_STATUS(SCAN_MODE_RUNING) &&\
  ext_refereeSystem_t.p_robot_hurt_t->hurt_type == HURT_RX_CLEAR)
  {
    CLEAR_STATE(HAVE_ENEMY);
  }
	if(ext_refereeSystem_t.p_robot_hurt_t->hurt_type ==0)
	{
    SET_STATE(HAVE_ENEMY|HURTED);
    GetEnemy();
    ext_refereeSystem_t.p_robot_hurt_t->hurt_type =HURT_RX_CLEAR;
	}
}
/**
	* @Data    2019-03-25 19:16
	* @brief   获取敌人的位置
	* @param   void
	* @retval  void
	*/
void GetEnemy(void)
{
 if(ext_refereeSystem_t.p_robot_hurt_t->armor_id ==AHEAD_OF_ARMOR)
 {
   switch(GET_CHASSIS_SELF_POS(REAL_AREA))
   {
      case UP_ROAD:
       SET_ENMET_POS(IN_WILD_AREA);
      break;
      case MID_ROAD:
      SET_ENMET_POS(IN_BLOCK_HOUSE);
      break;
      case DOWN_ROAD:
        SET_ENMET_POS(IN_BLOCK_HOUSE);
      break;
   }
 }
 else if(ext_refereeSystem_t.p_robot_hurt_t->armor_id ==BACK_OF_ARMOR)
 {
   switch(GET_CHASSIS_SELF_POS(REAL_AREA))
   {
      case UP_ROAD:
       SET_ENMET_POS(IN_ROAD);
      break;
      case MID_ROAD:
      SET_ENMET_POS(IN_ROAD);
      break;
      case DOWN_ROAD:
       SET_ENMET_POS(IN_BASE);
      break;
   }
 }
}
/*-----------------------------------file of end------------------------------*/


