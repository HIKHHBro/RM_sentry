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
	|--FileName    : DataStructure.c                                              
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
#include "DataStructure.h" 
/**
	* @Data    2019-02-21 15:02
	* @brief   创建队列
	* @param   void
	* @retval  void
	*/
	SqQueue GyinitQueue(void)
	{
		SqQueue *sq=(struct SqQueue*)malloc(sizeof(struct SqQueue));
		if(sq ==NULL)
			return (*sq);
		sq->rear=sq->front=0;
		return *sq;
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   判断循环队列是否为空
	* @param   void
	* @retval  void
	*/
	float isEmpty(SqQueue qu)
	{
		return (qu.front ==qu.rear?1:0);
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   元素进循环队列
	* @param   void
	* @retval  void
	*/
	float enQueue(SqQueue *qu,float x,uint8_t size)
	{
		if((qu->rear+1)%size ==qu->front){
			return 0;
		}
		qu->rear=(qu->rear+1)%size;
		qu->data[qu->rear]=x;
		return 1;
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   元素出循环队列
	* @param   void
	* @retval  void
	*/
	float deQueue(SqQueue *qu,float *y,uint8_t size)
	{
		if(qu->rear ==qu->front){
			return 0;
		}
		*y=qu->data[qu->front];
		qu->front=(qu->front+1)%size;
		return 1;
	}
/**
	* @Data    2019-02-21 15:04
	* @brief   打印循环队列
	* @param   void
	* @retval  void
	*/
	int printQueue(SqQueue qu)
	{
		if(qu.rear ==qu.front){
			return 0;
		}
		while(qu.rear !=qu.front){
			qu.front=(qu.front+1)%MAXSIZE;
//			printf("当前队列值=%d\n",qu.data[qu.front]);
		}
		return 1;
	}

/*-----------------------------------file of end------------------------------*/
