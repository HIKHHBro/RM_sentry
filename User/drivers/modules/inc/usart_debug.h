#ifndef __NIMING_H
#define __NIMING_H
#include "baseclass.h "
#ifdef DEBUG_BY_KEIL
typedef struct 
{
	void(*f)(float,float,UART_HandleTypeDef* huart);
	void(*i16)(int16_t,int16_t,UART_HandleTypeDef* huart);
	void(*uint32)(UART_HandleTypeDef* huart,uint32_t data);
	uint8_t flag;
	void* pbezierDebug;
}Niming_Class;
extern Niming_Class  Debug_t;
void DebugClassInit(void);
#endif
#endif	// __NIMING_H
