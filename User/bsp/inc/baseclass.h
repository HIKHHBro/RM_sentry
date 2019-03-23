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
#include "userdriverconfig.h"
/* -------------- ���԰汾�ͷ��а汾ѡ�� ----------------- */
/*���а�������к궨��ע�͵�*/
#define DEBUG_BY_KEIL

	#define MAX(X,Y) ((X)>(Y)?(Y):(X))  //������ֵ
	#define MIN(X,Y) ((X)<(Y)?(Y):(X))  //����Сֵ
	#define ABS(X)  ( (X)<0?(-X):(X))    //ȡ����ֵ

/* ----------------- ��־λ�����ⲿ���� -------------------- */
	extern unsigned int e_periphera_interface_flag; 	//����ӿ�ʹ��״̬��������
	extern unsigned int e_periphera_error_flag;		//����ӿڴ���״̬��������
	extern unsigned int e_objects_flag;     //����ʹ�ñ�־
/* ----------------- ����״̬λ �� -------------------- */

/* uint32_t status(0������͵�λ��31�������λ)��Ϊ�˷������������־λ��
 * -------------------------------------------------
 *������ |   0bit   |   1bit    |   2bit   |    3bit    | 7~3bit |
 *�ݳ�ʼ--------------------------------------------
 *��λ   | ��ʼ���ɹ�| �������� | �������� |�������óɹ�| ����  |
 * ----------------------------------------------------------------
 * ����| 8bit | 9bit | 10bit  | 11bit  | 12bit  | 13bit | 14~15bit | 
 * ��ʼ-------------------------------------------------------------
 * ��λ| can1 | CAN2 | UASRT1 | UASRT3 | UASRT6 | UART7 |   ����    | 
 * -----------------------------------------------------------------
 * |        24~31bit      | 
 * -------------------------
 * |  ����(ģ���Զ���λ)  | 
 * -------------------------
 */
	#define MOD_READ                    0x00000000U//ģ��׼������
	#define INIT_OK                     0x00000001U//��ʼ���ɹ�
  #define RUNING_OK                   0x00000002U//��������
  #define RX_OK                       0x00000004U//��������
  #define START_OK                    0x00000008U//�������óɹ�
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
	/* -------------- �� ----------------- */
	#define	CYCLE_NUMERICAL(data,max) ((data+1)%max)
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

