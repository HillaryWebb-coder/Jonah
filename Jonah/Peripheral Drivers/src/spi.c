#include "spi.h"
#include <stdint.h>
#include "gpio.h"
#include "stm32f10x.h"

void SPI_Init(SPI_TypeDef* SPIx, uint32_t BaudRatePrescaler) {
    // Enable clock for the SPI peripheral
    if (SPIx == SPI1) {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    } else if (SPIx == SPI2) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    } else {
        // Unsupported SPI peripheral
        return;
    }

    // configure GPIO pins for SPI functionality here as needed
   // Configure GPIO pins for SPI functionality
   GPIO_Init(GPIOA, 4, AF_OUTPUT_PUSH_PULL); // NSS
    GPIO_Init(GPIOA, 5, AF_OUTPUT_PUSH_PULL); // SCK
    GPIO_Init(GPIOA, 7, AF_OUTPUT_PUSH_PULL); // MOSI
    GPIO_Init(GPIOA, 6, INPUT_PULL_UP);   // MISO

    // Configure SPI control registers for master, 8-bit, software NSS
    SPIx->CR1 = 0;                 // Clear all bits first
    SPIx->CR1 |= BaudRatePrescaler;// Set baud rate prescaler
    SPIx->CR2 |= SPI_CR2_SSOE;    // Enable SS output for master mode
    SPIx->CR1 |= SPI_CR1_MSTR;     // Master mode
    // Keep defaults: CPOL=0, CPHA=0 (SPI mode 0), MSB first, 8-bit data
    
    SPIx->CR2 = 0;                 // No interrupts or DMA

    // Enable the SPI peripheral
    SPIx->CR1 |= SPI_CR1_SPE;
}

void SPI_Transmit(SPI_TypeDef* SPIx, uint8_t data) {
    // Wait until TXE (Transmit buffer empty) flag is set
    while (!(SPIx->SR & SPI_SR_TXE));    
    // Send data
    SPIx->DR = data;

}