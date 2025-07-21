#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

#include "led_task.h"

#define MAIN_TASK_STACK_SIZE 1024

static TaskHandle_t xMainTaskHandle = NULL;
static StaticTask_t xMainTaskBuffer;

static StackType_t xMainStack[MAIN_TASK_STACK_SIZE];

static void MainTask(void *pvParameters);

TaskHandle_t MainTaskInit(u8 priority, TaskHandle_t xLEDTaskHandle);

#endif // MAIN_TASK_H