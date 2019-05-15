/**
  |-------------------------------- Copyright -----------------------------------|
  |                                                                              |
  |                        (C) Copyright 2019,����ƽͷ��,                         |
  |            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
  |                            All Rights Reserved                               |
  |                                                                              |
  |            By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
  |                     https://github.com/GCUWildwolfteam                       |
  |------------------------------------------------------------------------------|
  |--FileName    : chassis_action.c                                                
  |--Version     : v1.0                                                            
  |--Author      : ����ƽͷ��                                                       
  |--Date        : 2019-04-15               
  |--Libsupports : STM32CubeF1 Firmware Package V1.6.0(�ñ�Ŀ�����������)
  |--Description :                                                       
  |--FunctionList                                                       
  |-------1. ....                                                       
  |          <version>:                                                       
  |     <modify staff>:                                                       
  |             <data>:                                                       
  |      <description>:                                                        
  |-------2. ...                                                       
  |---------------------------------declaration of end----------------------------|
 **/
#include "chassis_action.h" 
	extern  chassisStruct chassis_t;
/**
	* @Data    2019-03-12 16:25
	* @brief   ���̿��ƺ���
	* @param   void
	* @retval  void
	*/
  int16_t pid_out[2];
  uint32_t tsoncin3;
  uint32_t tsoncin4;
  int16_t saerr=0;
  uint32_t ssssssss;
  int16_t ceshi_rw;
  int16_t ceshi_rw_r;
  uint8_t onoff_temp=0;
  int32_t temp_dis_a=0;
	void ChassisControl(void)
	{
    Inject(chassis_t.ppowerBufferPool_t);//���¹��ʻ����״̬
    GetState();
      EncodeDetector();
    //��ȡ�ܱ����
    //ceshiqu-------------------------
    //ceshiqu-------------------------
	  ChassisControlSwitch(ChassisControlDecision());
    chassis_t.pwheel1_t->error = chassis_t.pwheel1_t->target - chassis_t.pwheel1_t->real_speed;
    chassis_t.pwheel2_t->error = chassis_t.pwheel2_t->target - chassis_t.pwheel2_t->real_speed;
		pid_out[0] = SpeedPid(chassis_t.pwheel1_t->pspeedPid_t,chassis_t.pwheel1_t->error);
  	pid_out[1] = SpeedPid(chassis_t.pwheel2_t->pspeedPid_t,chassis_t.pwheel2_t->error);
    SetInPut(chassis_t.ppowerBufferPool_t,pid_out,2);//�����ʻ����
    ceshi_rw = (int16_t)chassis_t.ppowerBufferPool_t->r_w;
    ceshi_rw_r = (int16_t)chassis_t.p_refereeSystem_t->p_power_heat_data_t->chassis_power_buffer;
		ChassisCanTx(pid_out[0],pid_out[1]);
    SET_BIT(chassis_t.status,RUNING_OK);
	}
/**
* @Data    2019-03-20 21:27
* @brief   ���߿����ж�
* @param   void
* @retval  void
*/
  uint32_t ChassisControlDecision(void)
  {
    if(chassis_t.rc_t ->switch_left ==2)
    {
      CLEAR_BIT(chassis_t.status,DISABLE_CHASSIS);
     return (chassis_t.status & CHASSIS_JUDGE_READ);
    }
    else if(chassis_t.rc_t->switch_right ==1)
    {
      if(IS_STATE(OUT_OF_CONTROL))
        SET_CHA_READ_STATUS(CHASSIS_OUT_OF_CONTROL_READ);
      else if(IS_STATE(HURTED))
        SET_CHA_READ_STATUS(CHASSIS_ELUDE_MODE_READ);
      else if(IS_STATE(HAVE_ENEMY))
        SET_CHA_READ_STATUS(CHASSIS_PC_SHOOT_MODE_READ);
      else SET_CHA_READ_STATUS(CHASSIS_CRUISE_MODE_READ);
    }
    else if(chassis_t.rc_t->switch_right ==2)
    {
			 SET_CHA_READ_STATUS(CHASSIS_RC_MODE_READ);
    } 
    
    return (chassis_t.status & CHASSIS_JUDGE_READ);
  }
/**
* @Data    2019-03-20 21:27
* @brief   ����Ȩ�л�
* @param   void
* @retval  void
*/
void ChassisControlSwitch(uint32_t commot)
{
 switch (commot)
 {
    case CHASSIS_OUT_OF_CONTROL_READ:
      ChassisOutOfControlMode();
    break;
   case CHASSIS_RC_MODE_READ:
       ChassisRcControlMode();
     break;
  case CHASSIS_ELUDE_MODE_READ://����ע�͵�
   		ChassisEludeControlMode();
		  break;
	case CHASSIS_PC_SHOOT_MODE_READ:
	    ChassisPcShootMode();
    break;
      case CHASSIS_CRUISE_MODE_READ:
       ChassisCruiseModeControl();
     break;
   default:
    ChassisDeinit();
	    break;
 }
}
/**
  * @Data    2019-03-13 03:19
  * @brief   ң��ģʽ
  * @param   void
  * @retval  void
  */
int16_t rc_coe=10;
  void ChassisRcControlMode(void)
  {
		if((chassis_t.status & CHASSIS_RC_MODE_RUNING)!= CHASSIS_RC_MODE_RUNING)
		{
			chassis_t.pwheel1_t->target = 0;
			chassis_t.pwheel2_t->target = 0;
      OUT_OF_CONTROL_COUNTER(0);
			SET_CHA_RUNING_STATUS(CHASSIS_RC_MODE_RUNING);
		}
		chassis_t.pwheel1_t->target = chassis_t.rc_t->ch1 *rc_coe;
		chassis_t.pwheel2_t->target = chassis_t.rc_t->ch1 *rc_coe;
  }
/**
	* @Data    2019-03-25 19:13
	* @brief   Ѳ��ģʽ  3508��·����·Ŀ��ֵΪ��ֵ����֮Ϊ��ֵ
	* @param   void
	* @retval  void
	*/
		 int16_t zhongd = 1500;
		 int16_t cru_speed = 5000;
		 int16_t direction =0;
	void ChassisCruiseModeControl(void)
	{
//			uint32_t position;
		if((chassis_t.status & CHASSIS_CRUISE_MODE_RUNING) != CHASSIS_CRUISE_MODE_RUNING)
		{
			ChassisCruiseModeInit();
		}
    //ԭ����-------------------------------------
		if(GET_CHASSIS_SELF_POS(REAL_AREA) == MID_ROAD)
		{
			 direction = GET_CHASSIS_STLE_DIR;//��3508�ķ���
			 chassis_t.pwheel1_t->target = direction*cru_speed;//�޸�Ŀ��ֵ���������仯��bug
			 chassis_t.pwheel2_t->target = chassis_t.pwheel1_t->target;
		}
		else if(GET_CHASSIS_SELF_POS(REAL_AREA) != MID_ROAD)
		{
			 direction = -GET_CHASSIS_STLE_DIR;//��3508�ķ���
			 chassis_t.pwheel1_t->target = direction*cru_speed;
			 chassis_t.pwheel2_t->target = chassis_t.pwheel1_t->target;
		}
    //----------------------ԭ����----------------------------
    // //��ʱ�޸�------------------------------
    // if(saerr ==0)
    // {
    //   direction =1;

    // }
    // else if(saerr ==1)
    // {
    //    direction =-1;
    // }
    // else direction=0;
    //   chassis_t.pwheel1_t->target = direction*cru_speed;
    //  chassis_t.pwheel2_t->target = chassis_t.pwheel1_t->target;
    // //-----------��ʱ�޸�--------------------
	}
/**
	* @Data    2019-03-30 15:50
	* @brief   ��������ģʽ��ʼ��
	* @param   void
	* @retval  void
	*/
	void ChassisCruiseModeInit(void)
	{
		SET_CHA_RUNING_STATUS(CHASSIS_CRUISE_MODE_RUNING);
	}
/**
	* @Data    2019-03-31 12:06
	* @brief   ���ģʽ��ʼ��
	* @param   void
	* @retval  void
	*/
uint8_t r_area_buffer;
uint8_t hurt_buffer;
	void ChassisEludeControlModeInit(void)
	{
	if((chassis_t.status & CHASSIS_ELUDE_MODE_RUNING) != CHASSIS_ELUDE_MODE_RUNING)
		{
      r_area_buffer = GET_CHASSIS_SELF_POS(REAL_AREA);
			SET_CHA_RUNING_STATUS(CHASSIS_ELUDE_MODE_RUNING);
		}
	}
/**
* @Data    2019-03-21 00:46
* @brief   ���ģʽ//������
* @param   void
* @retval  void
*/
uint8_t tempwwwwaaa;
void ChassisEludeControlMode(void)
{
   int16_t temp__target =0;
int16_t pid_out[2];
	ChassisEludeControlModeInit();
  if(chassis_t.p_refereeSystem_t->p_robot_hurt_t->armor_id == AHEAD_OF_ARMOR)
  {
		switch (r_area_buffer) 
		{
			case UP_ROAD:
       temp__target= GoToPiont(chassis_t.track.end-200,pid_out);
      SetMotorTarget(pid_out[0],pid_out[1]);
      
    // temp__target = GoToPiont(600,pid_out);
				break;
			case MID_ROAD:
       temp__target =  GoToPiont(chassis_t.track.start + 100,pid_out);
       SetMotorTarget(pid_out[0],pid_out[1]);
				break;
			case DOWN_ROAD:
      temp__target =  GoToPiont(chassis_t.track.start +100,pid_out);
       SetMotorTarget(pid_out[0],pid_out[1]);
				break;
			default:
     
				break;
		}
    if(temp__target <50)
    {
      chassis_t.p_refereeSystem_t->p_robot_hurt_t->hurt_type =HURT_RX_CLEAR;
      chassis_t.p_refereeSystem_t->p_robot_hurt_t->armor_id =HURT_RX_CLEAR;
      CLEAR_BIT(GET_ALL_STATE,HURTED);
    }
  }
}

/**
	* @Data    2019-03-31 12:02
	* @brief   ����ģʽ����
	* @param   void
	* @retval  void
	*/
  int16_t speed_temp_ =4000;
  int16_t target_posion = 0;
  int16_t postion_buff;
  int16_t postion_i =MOV_RAN;
  int16_t pid_chassis_pos[2];
  int16_t temp_outtarget;
  int16_t error_offset =100;
	void ChassisPcShootMode(void)
	{
		ChassisPcShootModeInit();
    if(ABS(chassis_t.pwheel1_t->ppostionPid_t->error) < error_offset)
    {
      postion_i = -postion_i;
    }
    temp_outtarget = postion_i+postion_buff;
   if(temp_outtarget>chassis_t.track.end)
     postion_buff -= (temp_outtarget - chassis_t.track.end);
     
     GoToPiont((postion_i+postion_buff),pid_chassis_pos);
    
 //  target_posion = Toback(speed_temp_);
    SetMotorTarget(pid_chassis_pos[0],pid_chassis_pos[1]);
	}
/**
	* @Data    2019-03-31 12:03
	* @brief   ����ģʽ��ʼ��
	* @param   void
	* @retval  void
	*/
	void ChassisPcShootModeInit(void)
	{
	 if((chassis_t.status & CHASSIS_PC_SHOOT_MODE_RUNING) != CHASSIS_PC_SHOOT_MODE_RUNING)
		{
      if((chassis_t.Sensor.encoder + MOV_RAN) > chassis_t.track.end)
        postion_buff = chassis_t.Sensor.encoder -300;
     if((chassis_t.Sensor.encoder - MOV_RAN) < chassis_t.track.start)
        postion_buff = chassis_t.Sensor.encoder +300;
      else postion_buff = chassis_t.Sensor.encoder;
			SET_CHA_RUNING_STATUS(CHASSIS_PC_SHOOT_MODE_RUNING);
		}
	}


/**
* @Data    2019-04-15 23:24
* @brief   ʧ��ģʽ//������
* @param   void
* @retval  void
*/
uint32_t mid_road_flag =0;
  uint8_t out_of_counter = 0,buffer_lim_sw=0;
  int16_t temp_error;
 
void ChassisOutOfControlMode(void)
{
  int16_t pidout[2];
  ChassisOutOfControlInit();
  if(out_of_counter >=12)
  {
     if(GetGyroDire() > chassis_t.track.angle-5)
    {
      mid_road_flag++;
      if(mid_road_flag > 0xFFFF)
       mid_road_flag =0xFFFF;
    }
    else if(GetGyroDire() <(chassis_t.track.angle-5) && mid_road_flag > OUT_OF_CONTROL_MID_FLAG)
    {
      chassis_t.pwheel1_t->target = chassis_t.pwheel2_t->target = -chassis_t.pwheel2_t->target;
      mid_road_flag =0;
    }
  }
  else 
  {
    if(buffer_lim_sw ==1)
    {
      temp_error = GoToPiont(100,pidout);
      SetMotorTarget(pidout[0],pidout[1]);
    }
    else if(buffer_lim_sw ==2)
    {
      temp_error = GoToPiont(chassis_t.track.end-100,pidout);
      SetMotorTarget(pidout[0],pidout[1]);
    }
    if(temp_error <60)
      CLEAR_STATE(OUT_OF_CONTROL);
  }
}
/**
* @Data    2019-04-15 23:24
* @brief   ʧ��ģʽ��ʼ��//������
* @param   void
* @retval  void
*/
void ChassisOutOfControlInit(void)
{
    int16_t speed = 7000;//������ʧ��ʱ���ٶ�
  if((chassis_t.status & CHASSIS_OUT_OF_CONTROL_RUNNING) != CHASSIS_OUT_OF_CONTROL_RUNNING)
  {
    out_of_counter ++;
    if(out_of_counter <12)
    {
      AutoCalibrationData();
       SET_CHA_RUNING_STATUS(CHASSIS_OUT_OF_CONTROL_RUNNING);
      if(chassis_t.Sensor.lim_sw_right ==1)
        buffer_lim_sw = 1; 
      if(chassis_t.Sensor.lim_sw_left ==1)
        buffer_lim_sw = 2; 
    }
    else
    {
       SET_CHA_RUNING_STATUS(CHASSIS_OUT_OF_CONTROL_RUNNING);
      if(chassis_t.Sensor.lim_sw_left ==1)
      {
        speed = speed*CHASSIS_MOTOR_DIRE_RIGHT;
      }
      else if(chassis_t.Sensor.lim_sw_right ==1)
      {
        speed = speed*CHASSIS_MOTOR_DIRE_LEFT;
      }
      chassis_t.pwheel1_t->target = chassis_t.pwheel2_t->target = speed;
      mid_road_flag=0;
    }

  }
}

/*------------------------------------file of end-------------------------------*/


