/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,海康平头哥,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : DataStructure.h                                                
	|--Version     : v1.0                                                            
	|--Author      : 海康平头哥                                                       
	|--Date        : 2019-02-21               
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
#ifndef __DATASTRUCTURE_H 
#define __DATASTRUCTURE_H 
#include "baseclass.h "
#define MAXSIZE    5
#define QUEUE_ADDER   10
typedef struct SqQueue{
	int16_t data[MAXSIZE];
	unsigned char front;//队首指针
	unsigned char rear;//队尾指针
	unsigned char size;//队列长度
}SqQueue;
	void GyinitQueue(SqQueue *sq);
char isEmpty(SqQueue qu);
int16_t enQueue(SqQueue *qu,int16_t x,uint8_t size);
int16_t deQueue(SqQueue *qu,int16_t *y,uint8_t size);

/* -------------- int16_t 循环队列 ----------------- */

typedef struct Int16Queue
{
   	int16_t *data;
	uint16_t front;//队首指针
	uint16_t rear;//队尾指针
	uint16_t  size;//队列长度
	uint16_t (*isEmpty)(struct Int16Queue qu);
	int16_t (*enQueue)(struct Int16Queue *qu,int16_t x);
  int16_t (*deQueue)(struct Int16Queue *qu,int16_t *y);
	int16_t (*print)(struct Int16Queue *qu);
}Int16Queue;
	void Int16QueueCreate(Int16Queue *sq,uint16_t size);
		uint16_t Int16isEmpty(Int16Queue qu);
			int16_t Int16enQueue(Int16Queue *qu,int16_t x);
				int16_t Int16deQueue(Int16Queue *qu,int16_t *y);
//					int16_t Int16printQueue(Int16Queue qu);
	int16_t Int16printQueue(Int16Queue* qu);
#endif	// __DATASTRUCTURE_H
/*-----------------------------------file of end------------------------------*/

