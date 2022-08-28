#ifndef __SPI_H__
#define __SPI_H__
#include "main.h"



extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;



void MX_SPI1_Init(void);
void MX_SPI2_Init(void);

void MX_SPI1_DeInit(void);
void MX_SPI2_DeInit(void);

#endif /* __SPI_H__ */
