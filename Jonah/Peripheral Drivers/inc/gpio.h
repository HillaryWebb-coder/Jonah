#ifndef __GPIO_H
#define __GPIO_H

#include "RTE_Components.h"
#include CMSIS_device_header

typedef enum {
    INPUT_ANALOG,
    INPUT_FLOATING,
    INPUT_PULL_DOWN,
    INPUT_PULL_UP,
    OUTPUT_PUSH_PULL,
    OUTPUT_OPEN_DRAIN,
    AF_OUTPUT_PUSH_PULL,
    AF_OUTPUT_OPEN_DRAIN
} GPIOMode_TypeDef;


void GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);
void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FlagStatus PinState);
FlagStatus GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif /* __GPIO_H */