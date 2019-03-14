/**
	|-------------------------------- Copyright |--------------------------------|
	|                                                                            |
	|                        (C) Copyright 2018,����ƽͷ��,                       |
	|         1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China        |
	|                         All Rights Reserved                                |
	|                                                                            |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
	|                   https://github.com/GCUWildwolfteam                       |
	|----------------------------------------------------------------------------|
	|--FileName    : baseclass.h
	|--Version     : v1.0
	|--Author      : ����ƽͷ��
	|--Date        : 2018-11-27 
	|-- Libsupports:           
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
#ifndef __BASECLASS_H 
#define __BASECLASS_H 
/* -------- stm32 ��ѡ�� 1�ǿ�����0�ǿ���--------- */
// #define HAL_F1 0
#define HAL_F4 1
/* -------------- ���԰汾�ͷ��а汾ѡ�� ----------------- */
/*���а�������к궨��ע�͵�*/
#define DEBUG_BY_KEIL
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
  #define LASER GPIO_PIN_12//PG13
  #define LASER_GPIO GPIOG
/* -----------------��Դ�������ź궨�嶨�� -------------------- */   
  #define POWER1 GPIO_PIN_2//PH2
  #define POWER2 GPIO_PIN_3//PH3
  #define POWER3 GPIO_PIN_4//PH4
  #define POWER4 GPIO_PIN_5//PH5
  #define POWER_GPIO GPIOH
/* -----------------���������ź궨�嶨�� -------------------- */     
  #define BUZZER GPIO_PIN_6//PH6  TIM12_CH1
  #define BUZZER_GPIO GPIOH
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
	#define MAX(X,Y) (X)>(Y)?(Y):(X)  //������ֵ
	#define MIN(X,Y) (X)<(Y)?(Y):(X)  //����Сֵ
	#define ABS(X)   (X)<0?(-X):(X)    //ȡ����ֵ

/* ----------------- ��־λ�����ⲿ���� -------------------- */
	extern unsigned int e_periphera_interface_flag; 	//����ӿ�ʹ��״̬��������
	extern unsigned int e_periphera_error_flag;		//����ӿڴ���״̬��������
	extern unsigned int e_objects_flag;     //����ʹ�ñ�־
/* ----------------- ����״̬λ �� -------------------- */

/* uint32_t status(0������͵�λ��31�������λ)��Ϊ�˷������������־λ��
 * -------------------------------------------------
 *������ |   0bit   |   1bit   |  2bit   |  7~3bit |
 *�ݳ�ʼ--------------------------------------------
 *��λ   | ��ʼ���ɹ�| �������� | �������� |    ����  |
 * ----------------------------------------------------------------
 * ����| 8bit | 9bit | 10bit  | 11bit  | 12bit  | 13bit | 14~15bit | 
 * ��ʼ-------------------------------------------------------------
 * ��λ| can1 | CAN2 | UASRT1 | UASRT3 | UASRT6 | UART7 |   ����    | 
 * -----------------------------------------------------------------
 */
	#define INIT_OK                     0x00000001U//��ʼ���ɹ�
  #define RUNING_OK                   0x00000002U//��������
  #define RX_OK                       0x00000004U//��������
  #define CAN1_INIT_OK                0x00000100U//can1����
  #define CAN2_INIT_OK	              0x00000200U//can2����
  #define UASRT1_INIT_OK	            0x00000400U//UASRT1����
/* ----------------- ����ʹ�ñ�־λ��  -------------------- */
	#define USART1_BY_USED    						0x0001U//����1��ʹ��
	#define USART2_BY_USED    						0x0002U//����2��ʹ��
	#define USART3_BY_USED    						0x0004U//����3��ʹ��
	#define UART4_BY_USED    							0x0008U//����4��ʹ��
	#define UART5_BY_USED    							0x0010U//����5��ʹ��
	#define UART6_BY_USED    							0x0020U//����6��ʹ��
	#define USART1_DMA_BY_USED    						0x0040U//����1��ʹ��
	#define USART2_DMA_BY_USED    						0x0080U//����2��ʹ��
	#define USART3_DMA_BY_USED    						0x0100U//����3��ʹ��
	#define UART4_DMA_BY_USED    							0x0200U//����4��ʹ��
	#define UART5_DMA_BY_USED    							0x0400U//����5��ʹ��
	#define UART6_DMA_BY_USED    							0x0800U//����6��ʹ��
	#define CAN1_BY_USED      						0x01000U//can1��ʹ��
	#define CAN2_BY_USED      						0x02000U//can2��ʹ��
	#define WHOLEPOSITOM_CLEAN							0x04000U//ȫ����λ����ɹ�
/* ----------------- ����ӿڴ���״̬�������� -------------------- */
	#define NO_ERROR   														 0x0000U
	#define FRAME_DROP  													 0x0001U
	#define RX_ERROR    													0x0002U
	#define GET_USART_ADDR_FAIL													0x0004U
	#define GET_CAN_ADDR_FAIL													0x0004U
/* ----------------- ����ʹ�ñ�־�� -------------------- */
	#define NO_CREATE_OBJECTS_OF_WHOLEPOSITION        0x0000U //����ȫ����λ�Ķ���
	#define CREATE_OBJECTS_OF_WHOLEPOSITION        0x0001U //����ȫ����λ�Ķ���
	#define CREATE_OBJECTS_OF_CHASSIS 						0x0002U //������̵Ķ���
	/* --��������һ��Ҫһ���ã�����֮��һ��Ҫ�ͷ� -- */
	#define CACHE_ADDR(CACHES,ADDRS) 	(CACHES = ADDRS) //�����ַ
	#define FREE_ADDR(CACHES) 	(CACHES = NULL)		//�ͷŵ�ַ  
/* -------------- �������� ----------------- */
	UART_HandleTypeDef* RecognizeUSARTType(UART_HandleTypeDef* huartx);
  CAN_HandleTypeDef* RecognizeCanType(CAN_HandleTypeDef* hcanx);
	void SetFrameDropflag(int16_t flag);
	void SetNormalflag(int16_t flag);
	HAL_StatusTypeDef RCREncryption(uint8_t *pdata, uint8_t size);
	HAL_StatusTypeDef RCRDecryption(uint8_t *pdata, uint8_t size);
	void MultibyteToByle(uint32_t data,uint8_t *pdata);
	void ByleToMultibyte(uint8_t *pdata,uint32_t *data);
	void TwobyteToByle(int16_t data,uint8_t *pdata);
	void DataFilling(uint8_t *pdata,uint8_t s,uint8_t size);
#endif	// __BASECLASS_H
	
 /*--------------------------------file of end--------------------------------*/


