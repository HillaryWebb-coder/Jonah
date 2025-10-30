#ifndef __SPI_H
#define __SPI_H

#include "RTE_Components.h"
#include <stdint.h>
#include CMSIS_device_header

/* SPI baud rate prescaler values */
#define SPI_BAUDRATEPRESCALER_2   (0x0000U)  /* SPI frequency = APB2/2  */
#define SPI_BAUDRATEPRESCALER_4   (0x0008U)  /* SPI frequency = APB2/4  */
#define SPI_BAUDRATEPRESCALER_8   (0x0010U)  /* SPI frequency = APB2/8  */
#define SPI_BAUDRATEPRESCALER_16  (0x0018U)  /* SPI frequency = APB2/16 */
#define SPI_BAUDRATEPRESCALER_32  (0x0020U)  /* SPI frequency = APB2/32 */
#define SPI_BAUDRATEPRESCALER_64  (0x0028U)  /* SPI frequency = APB2/64 */
#define SPI_BAUDRATEPRESCALER_128 (0x0030U)  /* SPI frequency = APB2/128*/
#define SPI_BAUDRATEPRESCALER_256 (0x0038U)  /* SPI frequency = APB2/256*/

void SPI_Init(SPI_TypeDef* SPIx, uint32_t BaudRatePrescaler);
void SPI_Transmit(SPI_TypeDef* SPIx, uint8_t data);

#endif /* __SPI_H */