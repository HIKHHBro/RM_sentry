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
  |--FileName    : userdriverconfig.h                                                
  |--Version     : v1.0                                                            
  |--Author      : ����ƽͷ��                                                       
  |--Date        : 2019-03-16               
  |--Libsupports : 
  |--Description : ���õײ�ӿ�                                                     
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
/* -------- stm32 ��ѡ�� 1�ǿ�����0�ǿ���--------- */
// #define HAL_F1 0
#define HAL_F4 1
/* ----------------- �������ѡ�� 1�ǿ�����0�ǿ���-------------------- */
#define RM_NEW_BOARD 1
//#define RM_OLD_BOARD 0
//#define BINGE_BOARD 1 //���İ���
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
/* ----------------- led���ź궨�嶨�� -------------------- */
  #if RM_NEW_BOARD //�°����ź궨��
      #define LED_1 GPIO_PIN_1
      #define LED_2 GPIO_PIN_2
      #define LED_3 GPIO_PIN_3
      #define LED_4 GPIO_PIN_4
      #define LED_5 GPIO_PIN_5
      #define LED_6 GPIO_PIN_6
      #define LED_7 GPIO_PIN_7
      #define LED_8 GPIO_PIN_8
      #define LED_GPIO GPIOG
      #define LED_TOTAL 8 //led�ȸ���
      #define LED_ORIGIN_PIN 1  //��GPIO�������
      
      #define RED_LED GPIO_PIN_11//PE11
      #define GREEN_LED GPIO_PIN_14//PE14
      #define COLOR_LED_GPIO GPIOE
    /* ----------------- �������ź궨�嶨�� -------------------- */  
      #define LASER GPIO_PIN_13//PG13
      #define LASER_GPIO GPIOG
    /* -----------------��Դ�������ź궨�嶨�� -------------------- */   
      #define POWER_1 GPIO_PIN_2//PH2
      #define POWER_2 GPIO_PIN_3//PH3
      #define POWER_3 GPIO_PIN_4//PH4
      #define POWER_4 GPIO_PIN_5//PH5
      #define POWER_GPIO GPIOH
    /* -----------------���������ź궨�嶨�� -------------------- */     
      #define BUZZER GPIO_PIN_6//PH6  TIM12_CH1
      #define BUZZER_GPIO GPIOH
    /* -------------- ���������ź궨�� ----------------- */
      #define SONIC            GPIO_PIN_1//PA1
      #define SONIC_GPIO       GPIOA
      
    /* ----------------- �������궨��ӿ� -------------------- */
    extern UART_HandleTypeDef huart1;//����1
    extern UART_HandleTypeDef huart2;//����2
    extern UART_HandleTypeDef huart6;
    extern UART_HandleTypeDef huart3;
    extern CAN_HandleTypeDef hcan1;
    extern TIM_HandleTypeDef htim5;
    extern TIM_HandleTypeDef htim2;
		extern TIM_HandleTypeDef htim4;//Ħ���ֵ��
		extern CAN_HandleTypeDef hcan1;
     #define ENCOER_TIM (&htim5)//��������ӿ�
     #define HCSR04_TIM (&htim2)//�������ӿ�
		 #define FRICTIONGEAR (&htim4)//Ħ���ֶ�ʱ�������ӿ�
		 #define FRICTIONGEAR_1 (TIM_CHANNEL_1)//Ħ����1PWMͨ��
		 #define FRICTIONGEAR_2 (TIM_CHANNEL_2)//Ħ����2PWMͨ��
		 #define FRICTIONGEAR_1_START_V (10U)//Ħ����1�����ͺ�
		 #define FRICTIONGEAR_2_START_V (10U)//Ħ����2�����ͺ�
		 #define GIMBAL_CAN (&hcan1)    //��̨�����can
     #define PC_DATA_UASRT (&huart6)//С�������ݽ��մ���
     #define COMMUNICAT    (&huart3)//����ϵͳ����
  #elif BINGE_BOARD
    #define LED_1 GPIO_PIN_9
    #define LED_2 GPIO_PIN_10
    #define LED_3 GPIO_PIN_11
    #define LED_4 GPIO_PIN_12
    #define LED_5 GPIO_PIN_13
    #define LED_6 GPIO_PIN_14
    #define LED_7 GPIO_PIN_15
    #define LED_GPIO GPIOE
    #define LED_TOTAL 7 //led�ȸ���
    #define LED_ORIGIN_PIN 9  //��GPIO�������
  #elif RM_OLD_BOARD
    #define LED_1 0
    #define LED_2 0
    #define LED_3 0
    #define LED_4 0
    #define LED_5 0
    #define LED_6 0
    #define LED_7 0
    #define LED_GPIO0G  0
    #define LED_TOTAL 0 //led�ȸ���
    #define LED_ORIGIN_PIN 0  //��GPIO�������
  #endif

#endif	// __USERDRIVERCONFIG_H

 /*------------------------------------file of end-------------------------------*/


