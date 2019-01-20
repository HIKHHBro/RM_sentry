/**
	|-------------------------------- Copyright ----------------------------------------|
	|                                                                                   |
	|                        (C) Copyright 2018,海康平头哥,                              |
	|            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China            |
	|                            All Rights Reserved                                    |
	|                                                                                   |
	|            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)               |
	|                     https://github.com/GCUWildwolfteam                            |
	|-----------------------------------------------------------------------------------|
	|--FileName    : counters.c                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2018-11-27               
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|---------------------------------declaration of end---------------------------------|
 **/
#include "counters.h" 
#include <Math.h>

	/* ================================== Bezier(二阶贝塞尔曲线) of begin ===================================== */
	/* -------------------------------- begin  -------------------------------- */
		/**
		* @author  海康平头哥 
		* @Data    2018-12-10 19:53
		* @brief   
		* @param   int x
		* @param 	 int	y;  //当前坐标
		* @retval  float t;  //曲线变化
		*/
	/* -------------------------------- end ----------------------------------- */
//		float Bezier(int x,int y)
//		{
//			float t;  //曲线变化
//			int x0,x1,y0,y1,x2,y2;  //P0,P1,P3点
//			int A ,B,C;//二阶方程  At^2+Bt+C=0
//			int temp=0;  //临时数据 //确定三个点，预先算出temp
//			A = x2-x0-x1;
//			B = 2*x1;
//			C = x0-x;
//            temp = sqrt(B^2-4*A*C);  //确定三个点，预先算出temp
//			t = (-B + temp)/(2*A);

//			return t;
//		}
		
   
	/* ================================== Bezier(二阶贝塞尔曲线) of end ======================================= */
	/* ================================== PID控制器 of begin ===================================== */
		  /**
			* @Data    2018-12-29 21:37
			* @brief   PID控制器
			* @param   float* d pid所有相关参数的指针
			* @param   int16* u  外接用户的数据接口
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
			* @brief   pid数据传值
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
	/* ================================== PID控制器 of end ======================================= */
	
	
	
 /*------------------------------------file of end------------------------------------*/


