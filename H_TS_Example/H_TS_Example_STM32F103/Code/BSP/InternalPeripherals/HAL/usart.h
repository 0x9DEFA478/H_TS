#ifndef __USART_H__
#define __USART_H__
#include "main.h"
#include "Package.h"


typedef struct USART_Handle{
  void* ip_usart;
  void* Buffer;
  H_Stream* Stream;
  void* memoryMalloc;
}USART_Handle;

extern USART_Handle* usart1_Handle;

extern UART_HandleTypeDef huart1;


void MX_USART1_UART_Init(void);


#endif /* __USART_H__ */
