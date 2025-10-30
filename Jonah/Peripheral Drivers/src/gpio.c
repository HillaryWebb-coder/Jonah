#include "gpio.h"

void GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode) {
    // Enable clock for the GPIO port
    if (GPIOx == GPIOA) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } else if (GPIOx == GPIOB) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if (GPIOx == GPIOC) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    } else {
        // "Unsupported GPIO port"
    }

    /*
     * API convention: GPIO_Pin is the pin number (0..15), not a bitmask.
     * Configure CRL (pins 0..7) or CRH (pins 8..15). Each pin has 4 config bits
     * (CNF[1:0] and MODE[1:0]) located at (pin_index * 4).
     *
     * Encoding used (STM32F1):
     *  - MODE = 00: Input
     *  - MODE = 11: Output 50 MHz (we use this for push-pull/open-drain outputs)
     *  - CNF for input: 01 floating, 10 pull-up/pull-down
     *  - CNF for output: 00 push-pull, 01 open-drain
     */

    if (GPIO_Pin > 15) {
        return; // invalid pin
    }

    uint32_t pin_index = (uint32_t)GPIO_Pin;
    uint32_t shift = (pin_index % 8) * 4;
    volatile uint32_t *cfg_reg = (pin_index < 8) ? &GPIOx->CRL : &GPIOx->CRH;

    /* clear the 4 configuration bits for this pin */
    *cfg_reg &= ~(0xFu << shift);

    switch (GPIO_Mode) {
        case INPUT_ANALOG:
            /* Input Analog: CNF=00, MODE=00 */
            *cfg_reg |= (0x0u << shift);
            break;
        case INPUT_FLOATING:
            /* Input Floating: CNF=01, MODE=00,  */
            *cfg_reg |= (0x4u << shift);
            break;
        case INPUT_PULL_DOWN:
        /* Input Pull-down: CNF=10, MODE=00 */
        *cfg_reg |= (0x8u << shift);
            break;
        case INPUT_PULL_UP:
            /* Input Pull-up: CNF=10, MODE=00 PxODR=1 */
            *cfg_reg |= (0x8u << shift);
            GPIOx->ODR |= (0x1u << pin_index);
            break;
        case OUTPUT_PUSH_PULL:
            /* Output Push-Pull: CNF=00, MODE=11 */
            *cfg_reg |= (0x3u << shift);
            break;
        case OUTPUT_OPEN_DRAIN:
            /* Output Open-Drain: CNF=01, MODE=11 */
            *cfg_reg |= (0x7u << shift); 
            break;
        case AF_OUTPUT_PUSH_PULL:
            /* Alternate function output push-pull: CNF=10, MODE=11 */
            *cfg_reg |= (0xBu << shift);
            break;
        case AF_OUTPUT_OPEN_DRAIN:
            /* Alternate function output open-drain: CNF=11, MODE=11 */
            *cfg_reg |= (0xFu << shift);
        default:
            /* Invalid mode - do nothing */
            break;
    }
}

void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FlagStatus PinState) {
    uint32_t mask;
    if (GPIO_Pin <= 15) {
        mask = (1u << GPIO_Pin);
    } else {
        /* treat as mask */
        mask = (uint32_t)GPIO_Pin;
    }

    if (PinState == SET) {
        GPIOx->BSRR = mask;  /* set bits */
    } else {
        GPIOx->BSRR = (mask << 16); /* reset bits via upper half-word */
    }
}

FlagStatus GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    uint32_t mask;
    if (GPIO_Pin <= 15) {
        mask = (1u << GPIO_Pin);
    } else {
        mask = (uint32_t)GPIO_Pin;
    }

    return (GPIOx->IDR & mask) ? SET : RESET;
}

void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    /* Atomic toggle using BSRR: read ODR, compute set/reset masks and write once. */
    uint32_t mask;
    if (GPIO_Pin <= 15) {
        mask = (1u << GPIO_Pin);
    } else {
        mask = (uint32_t)GPIO_Pin;
    }

    uint32_t odr = GPIOx->ODR;
    uint32_t set_mask = (~odr) & mask; /* bits that are 0 and need to be set */
    uint32_t reset_mask = odr & mask;  /* bits that are 1 and need to be reset */
    GPIOx->BSRR = set_mask | (reset_mask << 16);
}