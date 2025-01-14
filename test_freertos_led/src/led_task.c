#include "led_task.h"
#include <projdefs.h>

// XGpioPs led_device;
// XGpioPs_Config *led_gpio_cfg_ptr;

XGpio led_device;
XGpio_Config *led_gpio_cfg_ptr;


void vLEDTimerCallback(TimerHandle_t pxTimer) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Give the semaphore to notify the LED task
    xSemaphoreGiveFromISR(xLEDTimerSemaphore, &xHigherPriorityTaskWoken);

    // Force a context switch if necessary
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


// GPIO initialization
void LEDGPIOInit() {
	led_gpio_cfg_ptr = XGpio_LookupConfig(LED_ID_ADDRESS);
    if (led_gpio_cfg_ptr == NULL) {
        printf("Failed to find configuration GPIO for Base Address: 0x%08X\n", LED_ID_ADDRESS);
    }
	XGpio_CfgInitialize(&led_device, led_gpio_cfg_ptr, led_gpio_cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&led_device, LED_CHANNEL, 0);
	XGpio_DiscreteWrite(&led_device, LED_CHANNEL, LED_1_INV_MASK);
    printf("Setup LED GPIO.\n");
}

void LEDTimerInit(u32 period) {
    xLEDTimer = xTimerCreate( (const char *) "LED Timer",
							pdMS_TO_TICKS(period),
							pdTRUE,
							(void *) TIMER_ID,
							vLEDTimerCallback);
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
            u32 current_state = XGpio_DiscreteRead(&led_device, LED_CHANNEL);
            // xil_printf("0x%08X\n", current_state);            
            XGpio_DiscreteWrite(&led_device, LED_CHANNEL, (!current_state) & LED_1_MASK);
            current_state = XGpio_DiscreteRead(&led_device, LED_CHANNEL);
            // xil_printf("0x%08X\n", current_state); 
            xil_printf("LED toggled!\n");
        }
	}
}

void LEDTaskInit(u8 priority) {
    LEDGPIOInit();
    const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );  

    xLEDTimerSemaphore = xSemaphoreCreateBinary();
    if (xLEDTimerSemaphore == NULL) {
        xil_printf("Failed to create semaphore!\n");
        return;
    }  

    int status = xTaskCreateStatic( 	LEDTask, 					/* The function that implements the task. */
                        "LED", 		/* Text name for the task, provided to assist debugging only. */
					    configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
					    NULL, 						/* The task parameter is not used, so set to NULL. */
					    priority,
                        xLEDStack,			/* The task runs at the idle priority. */
					    &xLEDTaskBuffer );
    if(status == NULL) {
        xil_printf("Failed to create LED task!\n");
        return;
    }
    LEDTimerInit(100);
    printf("Setup LED Task.\n");
}