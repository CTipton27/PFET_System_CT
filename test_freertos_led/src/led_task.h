#ifndef LED_TASK_H
#define LED_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xgpio.h"

// #include "xscugic.h"
// #include "xscutimer.h"

#define TIMER_ID	1

#define DELAY_10_SECONDS	10000UL
#define DELAY_1_SECOND		1000UL
#define DELAY_HALF_SECOND	500UL

#define LED_GIC_ADDRESS XPAR_XSCUGIC_0_BASEADDR
#define LED_TIMER_ADDRESS XPAR_AXI_GPIO_LED_BASEADDR

#define LED_ID_ADDRESS XPAR_AXI_GPIO_LED_BASEADDR
#define LED_CHANNEL 1

#define LED_1_MASK		0b0001
#define LED_1_INV_MASK	0b0000

#define LED_PIN 0

#define LOAD_VALUE 0x000FFFFF

#define LED_TASK_STACK_SIZE 1024

static TaskHandle_t xLEDTaskHandle = NULL;
static StaticTask_t xLEDTaskBuffer;
static SemaphoreHandle_t xLEDTimerSemaphore;
static TimerHandle_t xLEDTimer;

extern XGpio xLEDDevice;
extern XGpio_Config *xLEDGpioCfgPtr;

// extern XGpioPs led_device;
// extern XGpioPs_Config *led_gpio_cfg_ptr;

// extern XScuTimer led_timer;
// extern XScuTimer_Config *led_timer_cfg_ptr;

// extern XScuGic gic_interrupt_controller;
// extern XScuGic_Config *gic_cfg_ptr;



static StackType_t xLEDStack[LED_TASK_STACK_SIZE];

// u32 led_value = 0;

// void LEDGPIOInit() {
// 	gpio_cfg_ptr = XGpio_LookupConfig(LED_ID_ADDRESS);
// 	XGpio_CfgInitialize(&led_device, gpio_cfg_ptr, gpio_cfg_ptr->BaseAddress);
// 	XGpio_SetDataDirection(&led_device, LED_CHANNEL, 0);
// 	XGpio_DiscreteWrite(&led_device, LED_CHANNEL, 0);
// }

static void LEDTimerCallback(TimerHandle_t pxTimer);

void LEDGPIOInit();

void LEDTimerInit();

static void LEDTask(void *pvParameters);

TaskHandle_t LEDTaskInit(u8 priority);

void LEDTaskStop(TaskHandle_t xLEDTaskHandle);

void LEDTaskStart(TaskHandle_t xLEDTaskHandle);

#endif // LED_TASK_H