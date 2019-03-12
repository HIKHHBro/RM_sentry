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
#include "baseclass.h"
#define MAXSIZE    8
typedef struct SqQueue{
	float data[MAXSIZE];
	unsigned char front;//队首指针
	unsigned char rear;//队尾指针
}SqQueue;
SqQueue GyinitQueue(void);
float isEmpty(SqQueue qu);
float enQueue(SqQueue *qu,float x,uint8_t size);
float deQueue(SqQueue *qu,float *y,uint8_t size);
#endif	// __DATASTRUCTURE_H
/*-----------------------------------file of end------------------------------*/

