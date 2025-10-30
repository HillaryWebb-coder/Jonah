#include "RTE_Components.h"
#include CMSIS_device_header

#include "gpio.h"
#include "spi.h"

#define STATUS_LED_PIN 13
#define STATUS_LED_PORT GPIOC

#define DISPLAY_CS_PIN 4
#define DISPLAY_CS_PORT GPIOA

int main() {
    // Initialize the GPIO pin for the status LED
    GPIO_Init(STATUS_LED_PORT, STATUS_LED_PIN, OUTPUT_PUSH_PULL);
    SPI_Init(SPI1, SPI_BAUDRATEPRESCALER_64);

    while(1) {
        SPI_Transmit(SPI1, 0x15);
        GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN);
        // simple delay
        for(volatile int i = 0; i < 1000000; i++);
        for(volatile int i = 0; i < 1000000; i++);    
    }
}


