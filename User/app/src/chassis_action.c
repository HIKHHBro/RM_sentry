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
  uint32_t chassiscommot =0;
  int16_t pid_out[2];
  uint32_t tsoncin3;
  uint32_t tsoncin4;
  int16_t saerr=0;
  uint32_t ssssssss;
	void ChassisControl(void)
	{
		//ssssssss = GetPosition(&chassisEnconder_t);
    Inject(chassis_t.ppowerBufferPool_t);//���¹��ʻ����״̬
	  SetArea();//������
    saerr =  jiujimoshi();
    chassiscommot	= ChassisControlDecision();
	  ChassisControlSwitch(chassiscommot);
    chassis_t.pwheel1_t->error = chassis_t.pwheel1_t->target - chassis_t.pwheel1_t->real_speed;
    chassis_t.pwheel2_t->error = chassis_t.pwheel2_t->target - chassis_t.pwheel2_t->real_speed;
		pid_out[0] = SpeedPid(chassis_t.pwheel1_t->pspeedPid_t,chassis_t.pwheel1_t->error);
  	pid_out[1] = SpeedPid(chassis_t.pwheel2_t->pspeedPid_t,chassis_t.pwheel2_t->error);
      SetInPut(chassis_t.ppowerBufferPool_t,pid_out,2);//�����ʻ����
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
      if(chassis_t.State.out_of_flag == OUT_OF_CONTROL)
      {
        SET_CHA_READ_STATUS(CHASSIS_OUT_OF_CONTROL_READ);
      }
//		 if(GetHurtStatus() !=5)
//      {
//        SET_CHA_READ_STATUS(CHASSIS_ELUDE_MODE_READ);
//      }
     else if(chassis_t.pPc_t->commot ==1 &&GetGyroDire()>=28)// else if(chassis_t.pPc_t->commot ==1)
      {
        SET_CHA_READ_STATUS(CHASSIS_PC_SHOOT_MODE_READ);
      }
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
		if(chassis_t.State.r_area == MID_ROAD)
		{
			 direction = chassis_t.State.last_area -chassis_t.State.r_area;//��3508�ķ���
			 chassis_t.pwheel1_t->target = direction*cru_speed;//�޸�Ŀ��ֵ���������仯��bug
			 chassis_t.pwheel2_t->target = chassis_t.pwheel1_t->target;
		}
		else if(chassis_t.State.r_area != MID_ROAD)
		{
			 direction = -(chassis_t.State.last_area -chassis_t.State.r_area);//��3508�ķ���
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
* @Data    2019-03-21 00:46
* @brief   ���ģʽ//������
* @param   void
* @retval  void
*/
uint8_t tempwwwwaaa;
void ChassisEludeControlMode(void)
{
	ChassisEludeControlModeInit();
//  tempwwwwaaa = GetHurtStatus();
//	if( tempwwwwaaa== AHEAD_OF_ARMOR)
//	{
		switch (chassis_t.State.r_area) 
		{
			case UP_ROAD:
			  Go(DOWN_ROAD,2000);
				break;
			case MID_ROAD:
		  	Go(UP_ROAD,2000);
				break;
			case DOWN_ROAD:
		  	Go(UP_ROAD,2000);
				break;
			default:
				break;
		}
//	}
}
/**
	* @Data    2019-03-31 12:06
	* @brief   ���ģʽ��ʼ��
	* @param   void
	* @retval  void
	*/
	void ChassisEludeControlModeInit(void)
	{
	if((chassis_t.status & CHASSIS_ELUDE_MODE_RUNING) != CHASSIS_ELUDE_MODE_RUNING)
		{
			SET_CHA_RUNING_STATUS(CHASSIS_ELUDE_MODE_RUNING);
		}
	}

/**
	* @Data    2019-03-31 12:02
	* @brief   ����ģʽ����
	* @param   void
	* @retval  void
	*/
	void ChassisPcShootMode(void)
	{
		ChassisPcShootModeInit();
		SetMotorTarget(0,0);
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
			SET_CHA_RUNING_STATUS(CHASSIS_PC_SHOOT_MODE_RUNING);
		}
	}


/**
* @Data    2019-04-15 23:24
* @brief   ʧ��ģʽ//������
* @param   void
* @retval  void
*/
int16_t mid_road_flag =0;
void ChassisOutOfControlMode(void)
{
  ChassisOutOfControlInit();
  if(GetGyroDire() > TURNING_ANGLE)
  {
    mid_road_flag++;
  }
  else if(GetGyroDire() <TURNING_ANGLE && mid_road_flag > OUT_OF_CONTROL_MID_FLAG)
  {
    chassis_t.pwheel1_t->target = chassis_t.pwheel2_t->target = -chassis_t.pwheel2_t->target;
    mid_road_flag =0;
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
  int16_t speed = 4000;
  if((chassis_t.status & CHASSIS_OUT_OF_CONTROL_RUNNING) != CHASSIS_OUT_OF_CONTROL_RUNNING)
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

/*------------------------------------file of end-------------------------------*/


