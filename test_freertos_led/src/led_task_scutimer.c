#include "led_task.h"

// XGpioPs led_device;
// XGpioPs_Config *led_gpio_cfg_ptr;

XGpio led_device;
XGpio_Config *led_gpio_cfg_ptr;

XScuTimer led_timer;
XScuTimer_Config *led_timer_cfg_ptr;

XScuGic gic_interrupt_controller;
XScuGic_Config *gic_cfg_ptr;

void LEDTimerInterruptHandler(void *CallBackRef) {
    // printf("SCU Timer Interrupt Triggered!\r\n");
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // Give the semaphore from the ISR
    xSemaphoreGiveFromISR(xTimerSemaphore, &xHigherPriorityTaskWoken);

    // Clear the interrupt
	// XScuTimer_ClearInterruptStatus((XScuTimer *)CallBackRef);
    XScuTimer_ClearInterruptStatus((XScuTimer *)&led_timer);

    // Request a context switch if necessary
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    // xil_printf("Timer started, status: %lu\n", XScuTimer_IsExpired(&led_timer));
}

void SetupTimerInterrupt() {
    gic_cfg_ptr = XScuGic_LookupConfig(LED_GIC_ADDRESS);
    if (gic_cfg_ptr == NULL) {
        printf("Failed to find configuration GIC for Base Address: 0x%08X\n", LED_GIC_ADDRESS);
        exit(1);
    }
    
	XScuGic_CfgInitialize(&gic_interrupt_controller, gic_cfg_ptr, gic_cfg_ptr->CpuBaseAddress);

    int Status = XScuGic_Connect(&gic_interrupt_controller, XPAR_SCUTIMER_INTR, 
    (Xil_ExceptionHandler)LEDTimerInterruptHandler, (void *)&led_timer);
	// int Status = XScuGic_Connect(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR, 
            // (Xil_ExceptionHandler)XTmrCtr_InterruptHandler, (void *)&led_timer);
    if (Status != XST_SUCCESS) {
        xil_printf("Interrupt Connection Failed!\r\n");
        exit(1);
    }

    if (Status == XST_SUCCESS) {
        xil_printf("Interrupt Handler Registered at Address: 0x%08X\r\n", (u32)LEDTimerInterruptHandler);
    } else {
        xil_printf("Failed to register interrupt handler.\r\n");
    }

    // XScuGic_InterruptMaptoCpu(&gic_interrupt_controller, 0, XPAR_FABRIC_AXI_TIMER_LED_INTR);

    // XScuGic_SetPriorityTriggerType(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR,
	// 			    64, 0x3);
    
    XScuGic_Enable(&gic_interrupt_controller, XPAR_SCUTIMER_INTR);

    // Enable globle interrupt
    Xil_ExceptionInit();
    
    // Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &gic_interrupt_controller);
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &gic_interrupt_controller);
    // Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, 
    //                          (Xil_ExceptionHandler)CustomGICHandler, 
    //                          &gic_interrupt_controller);

    // XScuGic_SoftwareIntr(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR, 0x01);
    Xil_ExceptionEnable();
    printf("Setup timer interrupt.\n");
    // Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
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

void LEDTimerInit() {
    SetupTimerInterrupt();
    led_timer_cfg_ptr = XScuTimer_LookupConfig(XPAR_SCUTIMER_BASEADDR);
	XScuTimer_CfgInitialize(&led_timer, led_timer_cfg_ptr, led_timer_cfg_ptr->BaseAddr);
	
    // XScuTimer_DisableInterrupt(&led_timer);
    // XScuTimer_Stop(&led_timer);
    // XScuTimer_ClearInterruptStatus(&led_timer);

    XScuTimer_LoadTimer(&led_timer, LOAD_VALUE);
    // XScuTimer_SetPrescaler(&led_timer, 127);
	XScuTimer_EnableAutoReload(&led_timer);
    // XScuTimer_SetOptions(&led_timer, XSCUTIMER_DOWN_COUNT_OPTION);
    XScuTimer_EnableInterrupt(&led_timer);
    XScuTimer_Start(&led_timer);
    printf("Setup timer\n");
    xil_printf("Timer started, status: %lu\n", XScuTimer_IsExpired(&led_timer));
    u32 timer_control = XScuTimer_ReadReg(led_timer.Config.BaseAddr, XSCUTIMER_CONTROL_OFFSET);
    u32 timer_counter = XScuTimer_ReadReg(led_timer.Config.BaseAddr, XSCUTIMER_COUNTER_OFFSET);

    xil_printf("Timer Control Register: 0x%08X\n", timer_control);
    xil_printf("Timer Counter Register: 0x%08X\n", timer_counter);
}

static void LEDTask(void *pvParameters) {
	// const TickType_t x1second = pdMS_TO_TICKS( DELAY_1_SECOND );
    // xTimerSemaphore = xSemaphoreCreateBinary();
	for( ;; ) {
        if (xSemaphoreTake(xTimerSemaphore, portMAX_DELAY) == pdTRUE) {
            // Toggle the LED
            u32 current_state = XGpio_DiscreteRead(&led_device, LED_CHANNEL);
            xil_printf("0x%08X\n", current_state);            
            XGpio_DiscreteWrite(&led_device, LED_CHANNEL, (!current_state) & LED_1_MASK);
            current_state = XGpio_DiscreteRead(&led_device, LED_CHANNEL);
            xil_printf("0x%08X\n", current_state); 
            xil_printf("LED toggled!\n");
        }
        u32 timer_counter = XScuTimer_ReadReg(led_timer.Config.BaseAddr, XSCUTIMER_COUNTER_OFFSET);
        xil_printf("Timer Counter Register: 0x%08X\n", timer_counter);
	}
}

void LEDTaskInit(u8 priority) {
    LEDGPIOInit();
    const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );  

    xTimerSemaphore = xSemaphoreCreateBinary();
    if (xTimerSemaphore == NULL) {
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
    LEDTimerInit();
    printf("Setup LED Task.\n");
}