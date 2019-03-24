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
	|--FileName    : power_buffer_pool.c                                              
	|--Version     : v1.0                                                          
	|--Author      : ����ƽͷ��                                                     
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

/**
	* @Data    2019-03-24 19:59
	* @brief   pid���ֵӳ�����
	* @param   void
	* @retval  void ��λA
	*/
float OutMapCurrent(int16_t coe,int16_t input)
{
	//-16384 ~ 0 ~ 16384,3508
	return (int16_t)(coe*input);
}
/**
	* @Data    2019-03-24 21:14
	* @brief   ����ӳ��pid���ֵ
	* @param   void
	* @retval  void
	*/
	int16_t CurrentMapOut(int16_t coe,int16_t current)
	{
		return (int16_t)(current / (float)(coe));
	}
/**
	* @Data    2019-03-24 20:18
	* @brief   ����س�ˮ�� //������
	* @param   input �����
	* @retval  void  
	*/
float time_coe=0.001;//��λms
int16_t GetOutlet(powerBufferPoolStruct* pbs,int16_t input)
{
	int16_t cur;//��λA
  uint8_t state;
	state = GetPowerPoolState(pbs);
  cur = CurrentMapping(pbs->current_mapp_coe,input);
  switch (state) 
  {
    case POOL_HIGH:
    if(cur > pbs->high_current_threshold)
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->high_current_threshold);
		}
      break;
    case POOL_MID:
    if(cur > pbs->mid_current_threshold)
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->mid_current_threshold);
		}
      break;
    case POOL_LOW:
    if(cur > pbs->mid_current_threshold)
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->low_current_threshold);
		}
      break;
    case POOL_EMPTY:
    if(cur > pbs->safe_current_threshold)
		{
			input = CurrentMapOut(pbs->current_mapp_coe,pbs->safe_current_threshold);
		}
      break;
    default:
      break;
  }
return input;
}
  /**
  * @Data    2019-03-24 22:56
  * @brief   �������ע��
  * @param   void
  * @retval  void
  */
  uint8_t Inject(powerBufferPoolStruct* pbs)
  {
    if(pbs->r_w >= pbs->max_w)
    {
      pbs->r_w =  pbs->max_w;
      return POOL_FULL;
    }
    else if(pbs->pcurrentMeter_t->power < pbs->max_p)
    {
      pbs->r_w = pbs->r_w - ((pbs->pcurrentMeter_t->power - pbs->max_p)*pbs->period);
      return POOL_PADDING;
    }
  }
  /**
  * @Data    2019-03-24 23:23
  * @brief   ��ȡ�����״̬
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
    else POOL_EMPTY;
  }
/*-----------------------------------file of end------------------------------*/


