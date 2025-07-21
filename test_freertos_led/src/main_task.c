#include "main_task.h"

// Todo: interrupt from DMA, then read out XADC data from DMA.

static void MainTask(void *pvParameters) {
    // Todo: 1. waiting start semaphore to notice start led task and XADC task
    //       2. waiting stop semaphore to stop led task and XDAC task

}

TaskHandle_t MainTaskInit(u8 priority, TaskHandle_t xLEDTaskHandle) {
    xMainTaskHandle = xTaskCreateStatic(
                        MainTask,
                        "MAIN",
                        configMINIMAL_STACK_SIZE,
                        NULL,
                        priority,
                        xMainStack,
                        &xMainTaskBuffer
                    );
    LEDTaskStop(xLEDTaskHandle);
    return xMainTaskHandle;
}