#include <stdio.h>
#include "xparameters.h"
#include "xscugic.h"
#include "xtmrctr.h"
#include "sleep.h"

#define LED_GIC_ADDRESS XPAR_XSCUGIC_0_BASEADDR
#define LED_TIMER_ADDRESS XPAR_AXI_TIMER_LED_BASEADDR

#define TIMER_COUNTER_0       0
#define TIMER_COUNTER_1       1

#define LOAD_VALUE 0X9AF8D9F

XTmrCtr led_timer;
XTmrCtr_Config *led_timer_cfg_ptr;

XScuGic gic_interrupt_controller;
XScuGic_Config *gic_cfg_ptr;


void LEDTimerInit() {
    led_timer_cfg_ptr = XTmrCtr_LookupConfig(LED_TIMER_ADDRESS);
    if (led_timer_cfg_ptr == NULL) {
        printf("Failed to find configuration led timer for Base Address: 0x%08X\n", LED_TIMER_ADDRESS);
    }
    XTmrCtr_CfgInitialize(&led_timer, led_timer_cfg_ptr, led_timer_cfg_ptr->BaseAddress);

    // XTmrCtr_Initialize(&led_timer, LED_TIMER_ADDRESS);
    // XTmrCtr_SetOptions(&led_timer, TIMER_COUNTER_0,
    //                    XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
    XTmrCtr_SetOptions(&led_timer, TIMER_COUNTER_0, 0);

    // XTmrCtr_SetResetValue(&led_timer, TIMER_COUNTER_0, LOAD_VALUE);

    // SetupTimerInterrupt();

    // Enable timer interrupts
    // XTmrCtr_EnableIntr(led_timer.BaseAddress, TIMER_COUNTER_0);

    // Start the timer
    XTmrCtr_Start(&led_timer, TIMER_COUNTER_0);
}

int main() {
	LEDTimerInit();
    u32 TimerValue;

    sleep(1);

    TimerValue = XTmrCtr_GetValue(&led_timer, TIMER_COUNTER_0);
    xil_printf("Timer Value: %d\r\n", TimerValue);

    // Stop the timer
    XTmrCtr_Stop(&led_timer, TIMER_COUNTER_0);

	while(1);

	return 0;
}