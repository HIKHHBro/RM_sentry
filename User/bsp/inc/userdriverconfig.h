/**
  |-------------------------------- Copyright -----------------------------------|
  |                                                                              |
  |                        (C) Copyright 2019,海康平头哥,                         |
  |            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
  |                            All Rights Reserved                               |
  |                                                                              |
  |            By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)          |
  |                     https://github.com/GCUWildwolfteam                       |
  |------------------------------------------------------------------------------|
  |--FileName    : userdriverconfig.h                                                
  |--Version     : v1.0                                                            
  |--Author      : 海康平头哥                                                       
  |--Date        : 2019-03-16               
  |--Libsupports : 
  |--Description : 配置底层接口                                                     
  |--FunctionList                                                       
  |-------1. ....                                                       
  |          <version>:                                                       
  |     <modify staff>:                                                       
  |             <data>:                                                       
  |      <description>:                                                        
  |-------2. ...                                                       
  |---------------------------------declaration of end----------------------------|
 **/
#ifndef __USERDRIVERCONFIG_H 
#define __USERDRIVERCONFIG_H 
/* -------- stm32 库选择 1是开启，0是开闭--------- */
// #define HAL_F1 0
#define HAL_F4 1
/* ----------------- 开发板的选择 1是开启，0是开闭-------------------- */
//#define RM_NEW_BOARD 0
//#define RM_OLD_BOARD 0
#define BINGE_BOARD 1 //斌哥的板子
#if HAL_F1 
	// #include "stm32f1xx_hal.h"
	// #include <stdlib.h>
#elif HAL_F4
		#include "stm32f4xx_hal.h"
		#include "cmsis_os.h"
		#include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
#endif 
/* -------------比赛颜色接口 ----------------- */
#define BLUE_GAME 0
#define RED_GAME  1

 //#define DEBUG_BY
/* ------------- 按键接口----------------- */
#define DISABLE_MOD 3
#define RC_MOD      2
#define AUTO_MOD    1
/* ----------------- led引脚宏定义定义 -------------------- */
  #if RM_NEW_BOARD //新板引脚宏定义
      #define LED_1 GPIO_PIN_1
      #define LED_2 GPIO_PIN_2
      #define LED_3 GPIO_PIN_3
      #define LED_4 GPIO_PIN_4
      #define LED_5 GPIO_PIN_5
      #define LED_6 GPIO_PIN_6
      #define LED_7 GPIO_PIN_7
      #define LED_8 GPIO_PIN_8
      #define LED_GPIO GPIOG
      #define LED_TOTAL 8 //led等个数
      #define LED_ORIGIN_PIN 1  //灯GPIO起点引脚
      
      #define RED_LED GPIO_PIN_11//PE11
      #define GREEN_LED GPIO_PIN_14//PE14
      #define COLOR_LED_GPIO GPIOE
    /* ----------------- 激光红点引脚宏定义定义 -------------------- */  
      #define LASER GPIO_PIN_13//PG13
      #define LASER_GPIO GPIOG
    /* -----------------电源管理引脚宏定义定义 -------------------- */   
      #define POWER_1 GPIO_PIN_2//PH2
      #define POWER_2 GPIO_PIN_3//PH3
      #define POWER_3 GPIO_PIN_4//PH4
      #define POWER_4 GPIO_PIN_5//PH5
      #define POWER_GPIO GPIOH
    /* -----------------蜂鸣器引脚宏定义定义 -------------------- */     
      #define BUZZER GPIO_PIN_6//PH6  TIM12_CH1
      #define BUZZER_GPIO GPIOH
    /* -------------- 超声波引脚宏定义 ----------------- */
      #define SONIC            GPIO_PIN_1//PA1 Trig
      #define SONIC_LEFT_E     GPIO_PIN_2//PA2 Echo 左边
      #define SONIC_RIGHT_E    GPIO_PIN_3//PA3 Echo 右边
      #define SONIC_GPIO       GPIOA
    /* -------------- 激光红外开关引脚宏定义 ----------------- */
    #define LASER_SWITCH       GPIO_PIN_5 //后侧
    #define LASER_SWITCH_BEF   GPIO_PIN_6//前侧（靠灯条）
    #define LASER_SWITCH_GPIO  GPIOI 
    /* ----------------- 编码器宏定义接口 -------------------- */
     #define ENCODE_A    GPIO_PIN_10//编码器A相  
     #define ENCODE_B    GPIO_PIN_11//编码器B相   
     
    extern UART_HandleTypeDef huart1;//串口1
    extern UART_HandleTypeDef huart2;//串口2
    extern UART_HandleTypeDef huart6;
    extern UART_HandleTypeDef huart3;
    extern CAN_HandleTypeDef hcan1;
      extern CAN_HandleTypeDef hcan2;
    extern TIM_HandleTypeDef htim5;
    extern TIM_HandleTypeDef htim2;
		extern TIM_HandleTypeDef htim4;//摩擦轮电机
     #define ENCOER_TIM (&htim5)//编码器宏接口
     #define HCSR04_TIM (&htim2)//超声波接口
		 #define FRICTIONGEAR (&htim4)//摩擦轮定时器电机红接口
		 #define FRICTIONGEAR_1 (TIM_CHANNEL_1)//摩擦轮1PWM通道
		 #define FRICTIONGEAR_2 (TIM_CHANNEL_2)//摩擦轮2PWM通道
		 #define FRICTIONGEAR_1_START_V (1000U)//摩擦轮1启动型号
		 #define FRICTIONGEAR_2_START_V (1000U)//摩擦轮2启动型号
		 #define GIMBAL_CAN (&hcan1)    //云台电机的can
     #define CHASSIS_CAN (&hcan1)    //云台电机的can
     #define  GY955_CAN (&hcan2)
     #define PC_DATA_UASRT (&huart6)//小电脑数据接收串口
     #define COMMUNICAT    (&huart3)//裁判系统串口
  #elif BINGE_BOARD
    extern CAN_HandleTypeDef hcan1;
    extern CAN_HandleTypeDef hcan2;
   // extern CAN_HandleTypeDef hcan2;
    extern UART_HandleTypeDef huart1;//串口1
    extern UART_HandleTypeDef huart2;//串口2
    extern UART_HandleTypeDef huart3;
    extern UART_HandleTypeDef huart7;
    extern UART_HandleTypeDef huart8;
	 	extern TIM_HandleTypeDef htim1;//摩擦轮电机
    extern TIM_HandleTypeDef htim3;
    extern TIM_HandleTypeDef htim2;
    extern TIM_HandleTypeDef htim7;
    extern TIM_HandleTypeDef htim12;
/* -----------------遥控宏定义接口 -------------------- */
     #define RC_UART (&huart1)//遥控串口宏定义
/* -------------调试和oled显示宏定义接口 ---------------- */
   /* ------ 调试版本和发行版本选择 --- */
    /*发行版请把这行宏定义注释掉*/
//    #define DEBUG_BY_KEIL
    #define DEBUG_UART (&huart2)
/* -----------------全场宏定义接口 -------------------- */
    //#define WHOLE_POSITION_UART 
/* ----------------- 编码器宏定义接口 -------------------- */
     #define ENCODE_A    GPIO_PIN_6//编码器A相   
     #define ENCODE_B    GPIO_PIN_7//编码器B相  
     #define ENCODE_Z    GPIO_PIN_6//编码器B相  
     #define ENCODE_Z_GPIO GPIOE
     #define ENCODE_GPIO GPIOC
     #define ENCOER_TIM  (&htim3)//编码器宏接口
/* -------------- 超声波引脚宏定义 ----------------- */
    #define SONIC            GPIO_PIN_1//PA2 Trig
    #define SONIC_LEFT_E     TIM_CHANNEL_3//PA0 Echo 左边
    #define SONIC_RIGHT_E    TIM_CHANNEL_4//PA1 Echo 右边
    #define SONIC_LEFT_CHA    HAL_TIM_ACTIVE_CHANNEL_3
    #define SONIC_RIGHT_CHA    HAL_TIM_ACTIVE_CHANNEL_4
    #define SONIC_GPIO       GPIOA
    #define HCSR04_TIM (&htim2)//超声波接口
/* -------------- 摩擦轮宏定义 ----------------- */
		 #define FRICTIONGEAR (&htim1)//摩擦轮定时器电机宏接口
		 #define FRICTIONGEAR_1 (TIM_CHANNEL_1)//摩擦轮1PWM通道
		 #define FRICTIONGEAR_1_START_V (1000U)//摩擦轮1启动信号
     #define FRICTIONGEAR_PWM  GPIO_PIN_8
     #define FRICTIONGEAR_GPIO   GPIOA
/* -----------------蜂鸣器引脚宏定义定义 -------------------- */     
      #define BUZZER GPIO_PIN_12//PH6  TIM12_CH1
      #define BUZZER_GPIO GPIOB
      #define BUZZER_TIM (&htim12)

		 #define GIMBAL_CAN (&hcan1)    //云台电机的can
     #define CHASSIS_CAN (&hcan2)    //云台电机的can
     #define  GYRO_CAN //(&hcan2)

     #define PC_DATA_UASRT (&huart8)//小电脑数据接收串口
     #define COMMUNICAT    (&huart3)//裁判系统串口

/* -------------- 激光红外开关引脚宏定义 ----------------- */
    #define LASER_SWITCH_BACK       GPIO_PIN_9 //后侧
    #define LASER_SWITCH_BEF   GPIO_PIN_11//前侧（靠灯条）
    #define LASER_SWITCH_GPIO  GPIOA 
/* ------限位开关宏接口 ------- */  
		 #define LIM_SW_RIGHT         GPIO_PIN_9
		 #define LIM_SW_LEFT          GPIO_PIN_10
     #define LIM_SWITCH_GPIO  	  GPIOA
/* -------------- LED引脚宏定义 ----------------- */
    #define LED_1 GPIO_PIN_9
    #define LED_2 GPIO_PIN_10
    #define LED_3 GPIO_PIN_11
    #define LED_4 GPIO_PIN_12
    #define LED_5 GPIO_PIN_13
    #define LED_6 GPIO_PIN_14
    #define LED_7 GPIO_PIN_15
    #define LED_GPIO GPIOE
    #define LED_TOTAL 7 //led等个数
    #define LED_ORIGIN_PIN 9  //灯GPIO起点引脚
    
    #define RED_LED GPIO_PIN_4//PB4
    #define GREEN_LED GPIO_PIN_5//PB5
    #define COLOR_LED_GPIO GPIOB
/* -------------KEY引脚宏定义 ----------------- */
    #define KEY_1 GPIO_PIN_7
    #define KEY_GPIO GPIOD
/* -------------帧率统计宏定义接口 ----------------- */
    #define _FPS_  //帧率统计宏开关
    //配置定时器为1us记数一次  ptim = 90， psc =89， arr = 0xFFFF-1 最长时间为65ms
    
  #elif RM_OLD_BOARD
    #define LED_1 0
    #define LED_2 0
    #define LED_3 0
    #define LED_4 0
    #define LED_5 0
    #define LED_6 0
    #define LED_7 0
    #define LED_GPIO0G  0
    #define LED_TOTAL 0 //led等个数
    #define LED_ORIGIN_PIN 0  //灯GPIO起点引脚
  #endif

#endif	// __USERDRIVERCONFIG_H

 /*------------------------------------file of end-------------------------------*/


