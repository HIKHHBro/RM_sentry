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
	|--FileName    : rammer.c                                              
	|--Version     : v1.0                                                          
	|--Author      : ����ƽͷ��                                                     
	|--Date        : 2019-03-16             
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
#include "rammer.h" 
	M2006Struct rammer_t;//��������ṹ��
	postionPidStruct rammerOuterLoopPid_t;//��������⻷pid
	speedPidStruct rammerInnerLoopPid_t;//��������ڻ�pid
/* -------------- �������� ----------------- */
	 int16_t stuct_lock_count = 0;
   int16_t stuct_count = 0;
  int16_t currer = 0;
  
  
  	int16_t RAMMER_TIME    =         100 ;// 1s �������  ʱ�����Ϊ10ms
  int16_t LOCK_ROTOT_TIME ; // =     3 * RAMMER_TIME; //3s ��תʱ��
	M2006Struct* RammerInit(void)
	{
				/* ------ ���������ʼ�� ------- */
			rammer_t.id = RAMMER_RX_ID;//���can�� ip
		  rammer_t.target = 0; //Ŀ��ֵ
		  rammer_t.real_current = 0;//��ʵ����
		  rammer_t.real_angle = 0;//��ʵ�Ƕ�
		  rammer_t.real_speed = 0;//��ʵ�ٶ�
			rammer_t.error = 0;
    rammer_t.coefficient =0;
    rammer_t.last_real =0;
			/* ------ �⻷pid���� ------- */
				rammer_t.ppostionPid_t = &rammerOuterLoopPid_t;
				rammerOuterLoopPid_t.kp = 0;
				rammerOuterLoopPid_t.kd = 0;
				rammerOuterLoopPid_t.ki = 0;
				rammerOuterLoopPid_t.error = 0;
				rammerOuterLoopPid_t.last_error = 0;//�ϴ����
				rammerOuterLoopPid_t.integral_er = 0;//������
				rammerOuterLoopPid_t.pout = 0;//p���
				rammerOuterLoopPid_t.iout = 0;//i���
				rammerOuterLoopPid_t.dout = 0;//k���
				rammerOuterLoopPid_t.pid_out = 0;//pid���
			/* ------ �ڻ�pid���� ------- */
				rammer_t.pspeedPid_t = &rammerInnerLoopPid_t;
				rammerInnerLoopPid_t.kp = 0;
				rammerInnerLoopPid_t.kd = 0;
				rammerInnerLoopPid_t.ki = 0;
				rammerInnerLoopPid_t.error = 0;
				rammerInnerLoopPid_t.last_error = 0;//�ϴ����
				rammerInnerLoopPid_t.before_last_error = 0;//���ϴ����
				rammerInnerLoopPid_t.integral_er = 0;//������
				rammerInnerLoopPid_t.pout = 0;//p���
				rammerInnerLoopPid_t.iout = 0;//i���
				rammerInnerLoopPid_t.dout = 0;//k���
				rammerInnerLoopPid_t.pid_out = 0;//pid���
        
				return &rammer_t;
	}
	/**
	* @Data    2019-02-14 21:01
	* @brief   ��������
	* @param   void
	* @retval  void
	*/
	void RammerControl(void)
	{
    LOCK_ROTOT_TIME  =     3 * RAMMER_TIME; //3s ��תʱ��
     if((rammer_t.real_current < STUCK_BULLET_THRE) &&((rammer_t.real_current > (-STUCK_BULLET_THRE))||(rammer_t.real_current == (-STUCK_BULLET_THRE))))
      {
        if(stuct_count < RAMMER_TIME)//500ms
        stuct_count ++;
        stuct_lock_count =-1;
      } 
     else if(rammer_t.real_current > STUCK_BULLET_THRE )
      {
        if(stuct_lock_count > -LOCK_ROTOT_TIME)//500ms
        stuct_lock_count --;
      }
     else if(rammer_t.real_current < -STUCK_BULLET_THRE)
     {
//       RammerShake();
     }
    if(stuct_count >= RAMMER_TIME)
      {
        rammer_t.target = PCycleNumerical( rammer_t.target);
        stuct_count =0;
      }
      else if(stuct_lock_count <= (-LOCK_ROTOT_TIME))
      {
        rammer_t.target = MCycleNumerical( rammer_t.target);
        stuct_lock_count =-1;
      }

//		int16_t pid_out = -500;
//		rammer_t.target = 3*(DbusAntiShake(20,dbus->ch1)); //Ŀ��ֵ
//		rammer_t.error = rammer_t.target - rammer_t.real_speed;
//		pid_out = SpeedPid(&rammerInnerLoopPid_t,rammer_t.error);
//		pid_out = MAX(pid_out,2000); //������ֵ
//	  pid_out = MIN(pid_out,-2000); //����Сֵ
//		GimbalCanTx(pid_out,0);
	}
	/**
		* @Data    2019-03-16 19:43
		* @brief   ������ֵ����ѭ���������ת��
		* @param   void
		* @retval  void
		*/
		int16_t PCycleNumerical(int16_t data)
		{
			return ((data+PARTITION_NUMB) % M2006_POLES);
		}
	/**
		* @Data    2019-03-16 19:43
		* @brief   ������ֵ����ѭ���������ת��//��M2006_POLESӦ��ȡʲô
		* @param   void
		* @retval  void
		*/
		int16_t MCycleNumerical(int16_t data)
		{
			return ((M2006_POLES-1) - (((M2006_POLES-1)-data) + PARTITION_NUMB) % M2006_POLES);
		}
  // /**
  // * @Data    2019-03-17 00:49
  // * @brief   ��������������
  // * @param   void
  // * @retval  void
  // */
  // void RammerShake(void)
  // {
  //   static int16_t icr=0,i=0;
  //   icr = icr*10;
  //   while (i) 
  //   {
      
  //   }
  //   if((rammer_t.real_current >STUCK_BULLET_THRE) || (rammer_t.real_current < -STUCK_BULLET_THRE))
  //   {
  //     if(i == 0)
  //      rammer_t.real_angle = PCycleNumerical(rammer_t.real_angle);
  //     else
  //     {
  //       rammer_t.real_angle = MCycleNumerical(rammer_t.real_angle);
  //     }
      
  //   }
  // }
/*-----------------------------------file of end------------------------------*/


