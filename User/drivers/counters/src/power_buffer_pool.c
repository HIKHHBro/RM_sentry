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
	|--FileName    : power_buffer_pool.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-03-24             
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
#include "power_buffer_pool.h"
#define POOL_FULL  0x01
#define POOL_PADDING 0x02
#define POOL_EMPTY  0x03
#define POOL_HIGH   0x04
#define POOL_MID    0x05
#define POOL_LOW    0x06
#define POOL_PASS 0x07

/**
	* @Data    2019-03-24 19:59
	* @brief   pid输出值映射电流
	* @param   void
	* @retval  void 单位A
	*/
float OutMapCurrent(float coe,int16_t input)
{
	//-16384 ~ 0 ~ 16384,3508
	return (float)(coe*input);
}
/**
	* @Data    2019-03-24 21:14
	* @brief   电流映射pid输出值
	* @param   void
	* @retval  void
	*/
	int16_t CurrentMapOut(float coe,float current)
	{
		return (int16_t)(current / (float)(coe));
	}
/**
	* @Data    2019-03-24 20:18
	* @brief   缓存池出水口 //待完善
	* @param   input 输出量
	* @retval  void  
	*/
  	int16_t cur;//单位A
float time_coe=0.001;//单位ms
int16_t GetOutlet(powerBufferPoolStruct* pbs,int16_t input)
{
	//int16_t cur;//单位A
  uint8_t state;
	state = GetPowerPoolState(pbs);
  cur = OutMapCurrent(pbs->current_mapp_coe,input);
  if(cur<0)
  {
    pbs->current_mapp_coe = -pbs->current_mapp_coe;
  }
  switch (state) 
  {
    case POOL_HIGH:
    if(ABS(cur) > pbs->high_current_threshold) 
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->high_current_threshold);
		}
      break;
    case POOL_MID:
    if(ABS(cur) > pbs->mid_current_threshold)
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->mid_current_threshold);
		}
      break;
    case POOL_LOW:
    if(ABS(cur) > pbs->low_current_threshold)
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->low_current_threshold);
		}
      break;
    case POOL_EMPTY:
    if(ABS(cur) > pbs->safe_current_threshold)
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->safe_current_threshold);
		}
      break;
    default:
      break;
  }
  pbs->current_mapp_coe  = ABS(pbs->current_mapp_coe);
return input;
}
  /**
  * @Data    2019-03-24 22:56
  * @brief   往缓存池注入
  * @param   void
  * @retval  void
  */
  uint8_t Inject(powerBufferPoolStruct* pbs)
  {
    uint8_t status = 0;
   pbs->r_p =pbs->pcurrentMeter_t->current * pbs->pcurrentMeter_t->volt;
    if(pbs->r_p <= pbs->max_p)
    {
      pbs->r_w = pbs->r_w - ((pbs->r_p - pbs->max_p)*pbs->period);
      status = POOL_PADDING;
    }
    else if(pbs->r_p > pbs->max_p)
    {
       pbs->r_w = pbs->r_w - ((pbs->r_p - pbs->max_p)*pbs->period);
      status = POOL_PASS;
    }
    if(pbs->r_w >= pbs->max_w)
    {
      pbs->r_w =  pbs->max_w;
      status = POOL_FULL;
    }
    if(pbs->r_w > pbs->p_refereeSystem_t->p_power_heat_data_t->chassis_power_buffer)
      pbs->r_w = pbs->p_refereeSystem_t->p_power_heat_data_t->chassis_power_buffer;
    return status;
  }
  /**
  * @Data    2019-03-24 23:23
  * @brief   获取缓存池状态
  * @param   void
  * @retval  void
  */
  uint8_t GetPowerPoolState(powerBufferPoolStruct* pbs)
  {
    if(pbs->r_w >= pbs->high_water_level)
    return POOL_HIGH;
    else if(pbs->r_w >= pbs->mid_water_level)
    return POOL_MID;
    else if(pbs->r_w >= pbs->low_water_level)
    return POOL_LOW;
    else return POOL_EMPTY;
  }
  /**
  * @Data    2019-03-24 23:23
  * @brief   输入输出接口
  * @param   void
  * @retval  void
  */
void SetInPut(powerBufferPoolStruct* pbs,int16_t *input,uint8_t len)
{
  int16_t suminput =0;
  uint8_t i=0;
 float coe[len];
  for(i=0;i < len;i++)
   suminput +=ABS(input[i]);
  for(i=0;i < len;i++)
  coe[i] = ((float)(input[i]))/((float)(suminput));
  suminput = GetOutlet(pbs,suminput);
  for(i=0;i < len;i++)
  input[i] = (int16_t)(suminput *coe[i]);
}

/*-----------------------------------file of end------------------------------*/


