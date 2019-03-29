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
	void GyinitQueue(SqQueue *sq)
	{
		sq->rear=sq->front=0;
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   判断循环队列是否为空
	* @param   void
	* @retval  void
	*/
	char isEmpty(SqQueue qu)
	{
		return (qu.front ==qu.rear?1:0);
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   元素进循环队列
	* @param   void
	* @retval  void
	*/
	int16_t enQueue(SqQueue *qu,int16_t x,uint8_t size)
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
	int16_t deQueue(SqQueue *qu,int16_t *y,uint8_t size)
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
/* =========================== int16_t 队列 of begin =========================== */
	/**
	* @Data    2019-02-21 15:02
	* @brief   创建队列
	* @param   void
	* @retval  void
	*/
	void Int16QueueCreate(Int16Queue *sq,uint16_t size)
	{
    sq->rear =1;
    sq->front =0;
		sq->size = size;
		sq->data = (int16_t *)malloc(sizeof(int16_t) * sq->size);
		sq->deQueue = Int16deQueue;
		sq->enQueue = Int16enQueue;
		sq->isEmpty = Int16isEmpty;
		sq->print = Int16printQueue;
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   判断循环队列是否为空
	* @param   void
	* @retval  void
	*/
	uint16_t Int16isEmpty(Int16Queue qu)
	{
		return (qu.front ==qu.rear?1:0);
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   元素进循环队列
	* @param   void
	* @retval  void
	*/
	int16_t Int16enQueue(Int16Queue *qu,int16_t x)
	{
		// if((qu->rear+1)%qu->size ==qu->front){
		// 	return 0;
		// }
		qu->rear=(qu->rear+1)%qu->size;
	 qu->front=(qu->front+1)%qu->size;
		qu->data[qu->rear]=x;
		return 1;
	}
/**
	* @Data    2019-02-21 15:03
	* @brief   元素出循环队列
	* @param   void
	* @retval  void
	*/
	int16_t Int16deQueue(Int16Queue *qu,int16_t *y)
	{
		if(qu->front == qu->rear)
		return 0;
		*y=qu->data[qu->front];
		qu->front=(qu->size -1) - (((qu->size -1) -qu->front)+1)%qu->size;
		return 1;
	}
/**
	* @Data    2019-02-21 15:04
	* @brief   打印循环队列
	* @param   void
	* @retval  void
	*/
//	int16_t Int16printQueue(Int16Queue qu)
//	{
//		qu.front=(qu.size -1) - (((qu.size -1) -qu.front)+1)%qu.size;
//		if(qu.front == qu.rear)
//		return 0;
//    printf("qu.data[%d]=%d\r\n",qu.front,qu.data[qu.front]);
//		return 1;
//	}
	int16_t Int16printQueue(Int16Queue* qu)
	{
   if(qu->front == qu->rear)
		return 0;
    printf("data[%d]=%d\r\n",qu->front,qu->data[qu->front]);
		qu->front=(qu->size -1) - (((qu->size -1) -qu->front)+1)%qu->size;
		return 1;
	}
/* =========================== int16_t 队列 of end =========================== */


/*-----------------------------------file of end------------------------------*/
