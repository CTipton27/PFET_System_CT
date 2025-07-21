#include "led_task.h"
#include <projdefs.h>

// XGpioPs led_device;
// XGpioPs_Config *led_gpio_cfg_ptr;

XGpio xLEDDevice;
XGpio_Config *xLEDGpioCfgPtr;


void LEDTimerCallback(TimerHandle_t pxTimer) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Give the semaphore to notify the LED task
    xSemaphoreGiveFromISR(xLEDTimerSemaphore, &xHigherPriorityTaskWoken);

    // Force a context switch if necessary
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


// GPIO initialization
void LEDGPIOInit() {
	xLEDGpioCfgPtr = XGpio_LookupConfig(LED_ID_ADDRESS);
    if (xLEDGpioCfgPtr == NULL) {
        printf("Failed to find configuration GPIO for Base Address: 0x%08X\n", LED_ID_ADDRESS);
    }
	XGpio_CfgInitialize(&xLEDDevice, xLEDGpioCfgPtr, xLEDGpioCfgPtr->BaseAddress);
	XGpio_SetDataDirection(&xLEDDevice, LED_CHANNEL, 0);
	XGpio_DiscreteWrite(&xLEDDevice, LED_CHANNEL, LED_1_INV_MASK);
    printf("Setup LED GPIO.\n");
}

void LEDTimerInit(u32 period) {
    xLEDTimer = xTimerCreate( (const char *) "LED Timer",
							pdMS_TO_TICKS(period),
							pdTRUE,
							(void *) TIMER_ID,
							LEDTimerCallback);
    if (xLEDTimer == NULL) {
        xil_printf("Failed to create LED timer.\n");
    } else {
        xil_printf("LED timer created.\n");

        // Start the timer with no block time
        if (xTimerStart(xLEDTimer, 0) != pdPASS) {
            xil_printf("Failed to start LED timer.\n");
        }
    }
}

static void LEDTask(void *pvParameters) {
	// const TickType_t x1second = pdMS_TO_TICKS( DELAY_1_SECOND );
    // xTimerSemaphore = xSemaphoreCreateBinary();
	for( ;; ) {
        if (xSemaphoreTake(xLEDTimerSemaphore, portMAX_DELAY) == pdTRUE) {
            // Toggle the LED
            u32 current_state = XGpio_DiscreteRead(&xLEDDevice, LED_CHANNEL);
            // xil_printf("0x%08X\n", current_state);            
            XGpio_DiscreteWrite(&xLEDDevice, LED_CHANNEL, (!current_state) & LED_1_MASK);
            current_state = XGpio_DiscreteRead(&xLEDDevice, LED_CHANNEL);
            // xil_printf("0x%08X\n", current_state); 
            xil_printf("LED toggled!\n");
        }
	}
}

TaskHandle_t LEDTaskInit(u8 priority) {
    LEDGPIOInit();
    const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );  

    xLEDTimerSemaphore = xSemaphoreCreateBinary();
    if (xLEDTimerSemaphore == NULL) {
        xil_printf("Failed to create semaphore!\n");
        exit(1);
    }  

    xLEDTaskHandle = xTaskCreateStatic( 	LEDTask, 					/* The function that implements the task. */
                        "LED", 		/* Text name for the task, provided to assist debugging only. */
					    configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
					    NULL, 						/* The task parameter is not used, so set to NULL. */
					    priority,
                        xLEDStack,			/* The task runs at the idle priority. */
					    &xLEDTaskBuffer );
    if(xLEDTaskHandle == NULL) {
        xil_printf("Failed to create LED task!\n");
        exit(1);
    }
    LEDTimerInit(100);
    // LEDTaskStop(xLEDTaskHandle);
    printf("Setup LED Task.\n");
    return xLEDTaskHandle;
}

void LEDTaskStop(TaskHandle_t xLEDTaskHandle) {
    vTaskSuspend(xLEDTaskHandle);
    xTimerStop(xLEDTimer, 0);
}

void LEDTaskStart(TaskHandle_t xLEDTaskHandle) {
    vTaskResume(xLEDTaskHandle);
    xTimerStart(xLEDTimer, 0);
}