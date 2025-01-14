#include <stdio.h>
#include "xparameters.h"
#include "xil_io.h"
#include "xgpio.h"
// #include "xgpiops.h"
#include "xscugic.h"
// #include "xscutimer.h"
#include "xtmrctr.h"
#include "xil_exception.h"
#include "sleep.h"

#define DELAY_10_SECONDS	10000UL
#define DELAY_1_SECOND		1000UL
#define DELAY_HALF_SECOND	500UL

#define LED_GIC_ADDRESS XPAR_XSCUGIC_0_BASEADDR
#define LED_TIMER_ADDRESS XPAR_AXI_TIMER_LED_BASEADDR

#define LED_ID_ADDRESS XPAR_AXI_GPIO_LED_BASEADDR
#define LED_CHANNEL 1

#define LED_1_MASK		0b0001
#define LED_1_INV_MASK	0b1110

#define LED_PIN 0

#define LOAD_VALUE 0X9AF8D9F

#define LED_GPIO_DEVICE_ID   0
#define LED_TIMER_DEVICE_ID  1
#define LED_GIC_DEVICE_ID    2

#define TIMER_COUNTER_0       0
#define TIMER_COUNTER_1       1

// XGpioPs led_device;
// XGpioPs_Config *led_gpio_cfg_ptr;

XGpio led_device;
XGpio_Config *gpio_cfg_ptr;


XTmrCtr led_timer;
XTmrCtr_Config *led_timer_cfg_ptr;

XScuGic gic_interrupt_controller;
XScuGic_Config *gic_cfg_ptr;

u32 led_value = 0;

void LEDTimerInterruptHandler(void *CallBackRef) {
    // u32 current_state = XGpio_DiscreteRead(&led_device, LED_CHANNEL);
    // XGpio_DiscreteWrite(&led_device, LED_CHANNEL, !current_state);

    XGpio_DiscreteWrite(&led_device, LED_CHANNEL, led_value | LED_1_MASK);
	sleep(1);
	XGpio_DiscreteWrite(&led_device, LED_CHANNEL, led_value & LED_1_INV_MASK);

    xil_printf("AXI Timer Interrupt Triggered!\r\n");

    // Clear the interrupt
	// XScuTimer_ClearInterruptStatus((XScuTimer *)CallBackRef);
    if (XTmrCtr_IsExpired(&led_device, TIMER_COUNTER_0)) {
        xil_printf("AXI Timer Interrupt Expired!\r\n");
    }
}

// void LEDTimerInterruptHandler(void *CallBackRef) {
// 	int current_state = XGpioPs_ReadPin(&led_device, LED_PIN);
//     XGpioPs_WritePin(&led_device, LED_PIN, !current_state);

//     if (XTmrCtr_IsExpired(&led_device, TIMER_COUNTER_0)) {
//         xil_printf("AXI Timer Interrupt Triggered!\r\n");
//     }
// }

void LEDGPIOInit() {
	gpio_cfg_ptr = XGpio_LookupConfig(LED_ID_ADDRESS);
    if (gpio_cfg_ptr == NULL) {
        printf("Failed to find configuration GPIO for Base Address: 0x%08X\n", LED_ID_ADDRESS);
    }
	XGpio_CfgInitialize(&led_device, gpio_cfg_ptr, gpio_cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&led_device, LED_CHANNEL, 0);
	XGpio_DiscreteWrite(&led_device, LED_CHANNEL, LED_1_INV_MASK);
}

// GPIO initialization
// void LEDGPIOInit() {
//     // XGpioPs_Config *Config;

//     // Initialize the GPIO driver
//     // led_gpio_cfg_ptr = XGpioPs_LookupConfig(LED_GPIO_DEVICE_ID);
//     led_gpio_cfg_ptr = XGpioPs_LookupConfig(LED_ID_ADDRESS);
//     if (led_gpio_cfg_ptr == NULL) {
//         printf("Failed to find configuration for Base Address: 0x%08X\n", LED_ID_ADDRESS);
//     }

//     XGpioPs_CfgInitialize(&led_device, led_gpio_cfg_ptr, led_gpio_cfg_ptr->BaseAddr);

//     // Set the LED pin as output
//     XGpioPs_SetDirectionPin(&led_device, LED_PIN, 1);
//     XGpioPs_SetOutputEnablePin(&led_device, LED_PIN, 1);
// }

void SetupTimerInterrupt() {
    gic_cfg_ptr = XScuGic_LookupConfig(LED_GIC_ADDRESS);
    if (gic_cfg_ptr == NULL) {
        printf("Failed to find configuration GIC for Base Address: 0x%08X\n", LED_GIC_ADDRESS);
    }
	XScuGic_CfgInitialize(&gic_interrupt_controller, gic_cfg_ptr, gic_cfg_ptr->CpuBaseAddress);

    XScuGic_Connect(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR, (Xil_ExceptionHandler)LEDTimerInterruptHandler, (void *)&led_timer);
	XScuGic_Enable(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR);

    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &gic_interrupt_controller);
    Xil_ExceptionEnable();
}

void LEDTimerInit() {
    led_timer_cfg_ptr = XTmrCtr_LookupConfig(LED_TIMER_ADDRESS);
    if (led_timer_cfg_ptr == NULL) {
        printf("Failed to find configuration led timer for Base Address: 0x%08X\n", LED_TIMER_ADDRESS);
    }
    XTmrCtr_CfgInitialize(&led_timer, led_timer_cfg_ptr, led_timer_cfg_ptr->BaseAddress);

    // XTmrCtr_Initialize(&led_timer, LED_TIMER_ADDRESS);
    XTmrCtr_SetOptions(&led_timer, TIMER_COUNTER_0,
                       XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

    XTmrCtr_SetResetValue(&led_timer, TIMER_COUNTER_0, LOAD_VALUE);

    SetupTimerInterrupt();

    // Enable timer interrupts
    XTmrCtr_EnableIntr(led_timer.BaseAddress, TIMER_COUNTER_0);

    // Start the timer
    XTmrCtr_Start(&led_timer, TIMER_COUNTER_0);
}


int main() {
	LEDGPIOInit();
	LEDTimerInit();

	while(1);

	return 0;
}