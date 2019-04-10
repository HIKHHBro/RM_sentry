/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,����ƽͷ��,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : parse.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2019-02-02               
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
#ifndef __PARSE_H 
#define __PARSE_H 
 #include "chassis.h"
 #include "gimbal.h"  
 #include "pc_data.h" 
 #include "communicate.h" 
  #define PARSE_DELAY 2
  #define COUNTER_TIME_I 1000  //֡��ͳ��Ƶ��1s
 void ParseInit(void);
 void ParseData(void);
 	const dbusStruct* GetRcStructAddr(void);
	uint32_t GetRcStatus(void);
	uint32_t GetPcDataStatus(void);
  const pcDataStruct* GetPcDataStructAddr(void);
    void SetCounterPc(int32_t counter);
      void SetCounterRc(int32_t counter)
#endif	// __PARSE_H
/*-----------------------------------file of end------------------------------*/


