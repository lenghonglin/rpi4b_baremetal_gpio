#include "io.h"

struct timer_regs {
    volatile unsigned int control_status;
    volatile unsigned int counter_lo;
    volatile unsigned int counter_hi;
    volatile unsigned int compare[4];
};

#define REGS_TIMER ((struct timer_regs *)(PERIPHERAL_BASE + 0x00003000))


unsigned long HAL_GetTick(void) {
    unsigned int hi = REGS_TIMER->counter_hi;
    unsigned int lo = REGS_TIMER->counter_lo;

    //double check hi value didn't change after setting it...
    if (hi != REGS_TIMER->counter_hi) {
        hi = REGS_TIMER->counter_hi;
        lo = REGS_TIMER->counter_lo;
    }

    return ((unsigned long)hi << 32) | lo;
}

void HAL_Delay(unsigned int ms) {
    unsigned long start = HAL_GetTick();

    while(HAL_GetTick() < start + (ms * 1000));
}


void main()
{
    uart_init();
    uart_writeText("Hello world!\n");
    gpio_initOutputPinWithPullNone(8);  //CS0
    gpio_setPinOutputBool(8, 0);
    HAL_Delay(1000);
    gpio_setPinOutputBool(8, 1);
    HAL_Delay(1000);
    gpio_setPinOutputBool(8, 0);
    while (1);
}
