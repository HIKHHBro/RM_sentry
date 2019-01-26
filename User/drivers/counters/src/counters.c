/**
	|----------------------------- Copyright ------------------------------------|
	|                                                                            |
	|                     (C) Copyright 2018,����ƽͷ��,                          |
	|         1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China        |
	|                  -       All Rights Reserved                               |
	|                                                                            |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
	|                  https://github.com/GCUWildwolfteam                        |
	|----------------------------------------------------------------------------|
	|--FileName    : counters.c                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2018-11-27               
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
#include "counters.h" 
#include <Math.h>

/* ===================== Bezier(���ױ���������) of begin ===================== */
	/* -------------------------------- begin  -------------------------------- */
		/**
		* @author  ����ƽͷ�� 
		* @Data    2018-12-10 19:53
		* @brief   
		* @param   int x
		* @param 	 int	y;  //��ǰ����
		* @retval  float t;  //���߱仯
		*/
	/* -------------------------------- end ----------------------------------- */
//		float Bezier(int x,int y)
//		{
//			float t;  //���߱仯
//			int x0,x1,y0,y1,x2,y2;  //P0,P1,P3��
//			int A ,B,C;//���׷���  At^2+Bt+C=0
//			int temp=0;  //��ʱ���� //ȷ�������㣬Ԥ�����temp
//			A = x2-x0-x1;
//			B = 2*x1;
//			C = x0-x;
//            temp = sqrt(B^2-4*A*C);  //ȷ�������㣬Ԥ�����temp
//			t = (-B + temp)/(2*A);

//			return t;
//		}
		
   
/* ====================== Bezier(���ױ���������) of end ====================== */
/* =========================== PID������ of begin =========================== */
		  /**
			* @Data    2018-12-29 21:37
			* @brief   PID������
			* @param   float* d pid������ز�����ָ��
			* @param   int16* u  ����û������ݽӿ�
			* @retval  void
			*/
			void PIDControl(float* d,int32_t* u)
			{
				switch(u[0])
				{
					case LOCATION_MODE:
					   u[ER] = u[TARGET] - u[REAL];
						 u[INTEGRAL_ER] += u[ER];
						 u[INTEGRAL_ER] = MAX(d[INTEGRAL_ER],d[LIMIT_MAX]);
						 u[INTEGRAL_ER] = MIN(d[INTEGRAL_ER],d[LIMIT_MIN]);
						 d[P_OUT] = d[KP] * d[ER];
						 d[I_OUT] = d[KI] * d[INTEGRAL_ER];
						 d[D_OUT] = d[KD] * (d[INTEGRAL_ER] - d[INTEGRAL_ER]);
						 u[PID_OUT] = (int32_t)(d[P_OUT] + d[I_OUT] +  d[D_OUT]);
						break;
					case INCREMENTAL_MODE:

						break;
					default:
						break ;

				}
			}

		  /**
			* @Data    2018-12-29 22:39
			* @brief   pid���ݴ�ֵ
			* @param   void
			* @retval  void
			*/
//			void PID_Pass_By_Value()
//			{
//				float d[10];
//				int32_t u[15];
//				d[P_OUT]												= 0;	
//				d[I_OUT]												=	0;	
//				d[D_OUT]												=	0;	
//				d[KP]														=	0;	
//				d[KI]														=	0;	
//				d[KD]														=	0;	
//				u[LOCATION_MODE]								=	0;	
//				u[INCREMENTAL_MODE]							=	0;	
//				u[ER]														=	0;	
//				u[LAST_ER]											=	0;	
//				u[BEFORE_LAST_ER]								=	0;	
//				u[INTEGRAL_ER]									=	0;	
//				u[TARGET]												=	0;	
//				u[REAL]													=	0;	
//				u[LIMIT_MAX]										=	0;	
//				u[LIMIT_MIN]										=	0;	

//				PIDControl(d,u);
//			}
/* ============================ PID������ of end ============================ */
/*--------------------------------file of end---------------------------------*/


