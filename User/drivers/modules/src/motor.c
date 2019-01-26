/**
	|------------------------------- Copyright ----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2018,����ƽͷ��,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                           All Rights Reserved                              |
	|                                                                            |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
	|                   https://github.com/GCUWildwolfteam                       |
	|----------------------------------------------------------------------------|
	|--FileName    : motor.c                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2018-12-31               
	|--Libsupports : ��׼���HAL��
	|--Description : 1��maxion���+RoboModule���� 
	|								 2��3508���+c610���
	|								 3��6623���
	|                4��6002���
	|								 5��                                                        
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#include "motor.h" 
/* ============================= RM6623 of begin ============================ */
	static int16_t s_max_motor_lines = 8192 ;//���һȦ�������
	/**
		* @Data    2019-01-18 19:59
		* @brief   RM6623�ṹ���ʼ��
		* @param   RM6623����ṹ���ַ
		* @retval  void
		*/
	void RM6623StructInit(RM6623Struct *RM6623,CAN_HandleTypeDef *hcanx)
	{
		RM6623->id = 0;
		RM6623->target = 0;				//Ŀ��ֵ
		RM6623->tem_target = 0;		//��ʱĿ��ֵ
		RM6623->real_current = 0; //��ʵ����
		RM6623->real_angle = 0;		//��ʵ�Ƕ�
		RM6623->tem_angle = 0;		//��ʱ�Ƕ�
		RM6623->zero = 0;					//������
		RM6623->Percentage = 0;		//ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1��
		RM6623->hcanx = hcanx;
		/*���ݶ�ȡʱ�������������ת������p��thresholds = p -s_max_motor_lines */
		RM6623->thresholds = 1200 - s_max_motor_lines;//1200��Ϲ���ģ���ƽ�
	}
		/**
		* @Data    2019-01-18 20:03
		* @brief   6623���ݽ���
		* @param   RM6623����ṹ���ַ
		* @retval  HAL Status
		*/
	void RM6623ParseData(RM6623Struct*RM6623,uint8_t *data)
	{
		int16_t tem_angle = 0;
		RM6623->real_current = ((int16_t)(data[4] << 8) | data[5]);
		tem_angle = ((int16_t)(data[0] << 8) | data[1]);
		tem_angle = RatiometricConversion(tem_angle, RM6623->thresholds,\
																								 RM6623->Percentage);
		RM6623->real_angle = zeroArgument(tem_angle, RM6623->thresholds);
	}

/* ============================= RM6623 of end ============================== */
/* ============================= 3508 of begin ============================== */
	/**
	* @Data    2019-01-19 00:42
	* @brief   3508���ݽ���
	* @param   RM3508Struct 3508�ṹ��ָ��
	* @param   *data  ���յ�����ָ��
	* @retval  void
	*/
	void RM3508ParseData(RM3508Struct *RM3508,uint8_t *data)
	{
		RM3508->real_angle=((int16_t)(data[0]<<8)|data[1]);
		RM3508->real_speed=((int16_t)(data[2]<<8)|data[3]);
		RM3508->real_current=((int16_t)(data[4]<<8)|data[5]);
	}
/* ============================== 3508 of end =============================== */
/* ============================= M2006 of begin ============================= */
	/**
		* @Data    2019-01-26 18:21
		* @brief   M2006���ݽ���
		* @param   M2006truct 2006�ṹ��ָ��
		* @param   *data  ���յ�����ָ��
		* @retval  void
		*/
		void RM2006ParseData(M2006Struct *M2006,uint8_t *data)
		{
			M2006->real_angle  = ((int16_t)(data[0]<<8)|data[1]);
    	M2006->real_speed = ((int16_t)(data[2]<<8)|data[3]);
			M2006->real_current=((int16_t)(data[4]<<8)|data[5]);
		}
/* ============================== M2006 of end ============================== */
/* ==============================common of begin ============================ */
	/**
	* @Data    2019-01-18 20:14
	* @brief   ����ת��  
	* @param   real ��ʵֵ
	* @param   threshold һȦ��������ķ�ֵ
	* @param   perce //ת������������ǰ�Ƕ�:���ٺ�ĽǶ� = x:1
	* @retval  int16_t ������Ŀ��ֵ
	*/
	int16_t RatiometricConversion(int16_t real,int16_t threshold,int16_t perce)
	{
		static int32_t last_real,tem=0;
		static uint16_t  coefficient=0;
		if (real - last_real < threshold)
		{
			/*�Ƕ�ϵ��ѭ������,����ת36Ȧ����ϵ��,���÷�Χ[0,perce] */
			coefficient =(coefficient+1)%(perce);
		}
		else if(last_real - real < threshold)
		{
			coefficient = (perce-1)-((((perce-1) - coefficient)+1)%(perce));
		}
		last_real = real;//��������ֵ
		tem = real + (s_max_motor_lines* coefficient); //ת���ܽǶ�
		return ((int16_t)(tem/(perce)));//���������תһȦ
	}
	/**
	* @Data    2019-01-18 20:48
	* @brief   ��㴦��
	* @param   real ��ʵֵ
	* @param   threshold һȦ��������ķ�ֵ
	* @retval  int16_t ����֮���Ŀ��ֵ
	*/
	int16_t zeroArgument(int16_t real,int16_t threshold)
	{
		static int16_t last_realdata = 0,counter = 0;
		if(real - last_realdata < threshold) 
			counter = 1;
		if(last_realdata - real <threshold)
			counter = -1;
			else counter = 0;
		last_realdata = real;
		return (real + (s_max_motor_lines * counter));
	}
/* ============================ common of end =============================== */
/*---------------------------------file of end--------------------------------*/
