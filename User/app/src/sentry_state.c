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
	|--FileName    : sentry_state.c                                              
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
#include "sentry_state.h" 
uint32_t sentry_state=0;
	extern chassisStruct chassis_t;
  extern	gimbalStruct gimbal_t;//��̨�ṹ��
   /**
	* @Data    2019-03-30 22:13
	* @brief   ��ȡ����������״̬
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
    SET_STATE(DEBUG_BEGIN);//����ʱ�����ע�͵�
  #endif
  SetArea();//��ȡ������λ��
  GetHurted();//��ȡ�����˱�����״̬
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
	* @brief   ��ȡ�����˱�������״̬
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
	* @brief   ��ȡ���˵�λ��
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


