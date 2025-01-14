#include <sleep.h>
#include <stdio.h>
#include "xparameters.h"
#include "xscugic.h"
#include "xtmrctr.h"

#define LED_GIC_ADDRESS XPAR_XSCUGIC_0_BASEADDR
#define LED_TIMER_ADDRESS XPAR_AXI_TIMER_LED_BASEADDR

#define TIMER_COUNTER_0       0
#define TIMER_COUNTER_1       1

#define LOAD_VALUE 0X7B80

XTmrCtr led_timer;
XTmrCtr_Config *led_timer_cfg_ptr;

XScuGic gic_interrupt_controller;
XScuGic_Config *gic_cfg_ptr;

volatile int TimerInterruptFlag = 0;

void LEDTimerInterruptHandler(void *CallBackRef) {
    TimerInterruptFlag = 1;
    XTmrCtr_WriteReg(led_timer.BaseAddress, TIMER_COUNTER_0, 0x0C, 0x00000001);

    // printf("AXI Timer Interrupt Triggered!\r\n");

    // // Clear the interrupt
	// // XScuTimer_ClearInterruptStatus((XScuTimer *)CallBackRef);
    // if (XTmrCtr_IsExpired(&led_timer, TIMER_COUNTER_0)) {
    //     xil_printf("AXI Timer Interrupt Expired!\r\n");
    //     exit(1);
    // }
}

void CustomGICHandler(void *CallBackRef) {
    xil_printf(">>> GIC Interrupt Triggered! <<<\r\n");
    XScuGic_InterruptHandler(CallBackRef);
}

void SetupTimerInterrupt() {
    gic_cfg_ptr = XScuGic_LookupConfig(LED_GIC_ADDRESS);
    if (gic_cfg_ptr == NULL) {
        printf("Failed to find configuration GIC for Base Address: 0x%08X\n", LED_GIC_ADDRESS);
        exit(1);
    }
    
	XScuGic_CfgInitialize(&gic_interrupt_controller, gic_cfg_ptr, gic_cfg_ptr->CpuBaseAddress);

    // int Status = XScuGic_Connect(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR, 
    // (Xil_ExceptionHandler)LEDTimerInterruptHandler, (void *)&led_timer);
	int Status = XScuGic_Connect(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR, 
            (Xil_ExceptionHandler)XTmrCtr_InterruptHandler, (void *)&led_timer);
    if (Status != XST_SUCCESS) {
        xil_printf("Interrupt Connection Failed!\r\n");
        exit(1);
    }

    // XScuGic_SetPriorityTriggerType(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR,
	// 			    0xA0, 0x3);
    
    XScuGic_Enable(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR);

    // Enable globle interrupt
    Xil_ExceptionInit();
    
    // Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &gic_interrupt_controller);
    // Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &gic_interrupt_controller);
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, 
                             (Xil_ExceptionHandler)CustomGICHandler, 
                             &gic_interrupt_controller);
    Xil_ExceptionEnable();
    printf("Setup timer interrupt.\n");
    // Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
}

void LEDTimerInit() {
    SetupTimerInterrupt();

    led_timer_cfg_ptr = XTmrCtr_LookupConfig(LED_TIMER_ADDRESS);
    if (led_timer_cfg_ptr == NULL) {
        printf("Failed to find configuration led timer for Base Address: 0x%08X\n", LED_TIMER_ADDRESS);
        exit(1);
    }
    XTmrCtr_CfgInitialize(&led_timer, led_timer_cfg_ptr, led_timer_cfg_ptr->BaseAddress);

    // XTmrCtr_Initialize(&led_timer, LED_TIMER_ADDRESS);

    XTmrCtr_SetHandler(&led_timer, (XTmrCtr_Handler)LEDTimerInterruptHandler, &led_timer);
    XTmrCtr_SetOptions(&led_timer, TIMER_COUNTER_0,
                       XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION);

    XTmrCtr_SetResetValue(&led_timer, TIMER_COUNTER_0, LOAD_VALUE);

    // Enable timer interrupts
    // XTmrCtr_Enable(&led_timer, TIMER_COUNTER_0);
    XTmrCtr_EnableIntr(led_timer.BaseAddress, TIMER_COUNTER_0);
    // XTmrCtr_EnableIntr(led_timer_cfg_ptr->BaseAddress, TIMER_COUNTER_0);

    // SetupTimerInterrupt();

    // Start the timer
    // XTmrCtr_Reset(&led_timer, TIMER_COUNTER_0);
    XTmrCtr_Start(&led_timer, TIMER_COUNTER_0);
    printf("Setup timer Standalone.\n");
}

void PrintGICStatus(XScuGic *GicInstance, u32 InterruptID) {
    // Validate input pointer
    if (GicInstance == NULL || GicInstance->Config == NULL) {
        xil_printf("Invalid GIC instance provided.\r\n");
        return;
    }

    u32 DistBaseAddr = GicInstance->Config->DistBaseAddress;
    u32 CpuBaseAddr  = GicInstance->Config->CpuBaseAddress;

    // Read Distributor Control Register (ICDDCR)
    u32 icddcr = Xil_In32(DistBaseAddr + 0x000);

    // Read CPU Interface Control Register (ICCICR)
    u32 iccicr = Xil_In32(CpuBaseAddr + 0x000);

    // Read Interrupt Priority Mask Register (ICCPMR)
    u32 iccpmr = Xil_In32(CpuBaseAddr + 0x004);

    // Read Binary Point Register (ICCBPR)
    u32 iccbpr = Xil_In32(CpuBaseAddr + 0x008);

    // Read Interrupt Acknowledge Register (ICCIAR)
    u32 icciar = Xil_In32(CpuBaseAddr + 0x00C);

    // Read Interrupt Set-Enable Register for the specific interrupt
    u32 iser_offset = 0x100 + ((InterruptID / 32) * 4);  // ICDISERn Offset
    u32 icdiser = Xil_In32(DistBaseAddr + iser_offset);

    xil_printf("GIC Status:\r\n");
    xil_printf("----------------------------\r\n");
    xil_printf("Distributor Control Register (ICDDCR): 0x%08X - %s\r\n", icddcr, (icddcr & 0x1) ? "ENABLED" : "DISABLED");
    xil_printf("CPU Interface Control Register (ICCICR): 0x%08X - %s\r\n", iccicr, (iccicr & 0x1) ? "ENABLED" : "DISABLED");
    xil_printf("Interrupt Priority Mask Register (ICCPMR): 0x%08X - Priority Level: %d\r\n", iccpmr, iccpmr);
    xil_printf("Binary Point Register (ICCBPR): 0x%08X\r\n", iccbpr);
    xil_printf("Interrupt Acknowledge Register (ICCIAR): 0x%08X\r\n", icciar);

    // Check if the specific interrupt is enabled
    xil_printf("Interrupt ID %d Enabled: %s\r\n", InterruptID, 
               (icdiser & (1 << (InterruptID % 32))) ? "YES" : "NO");
    
    if ((icddcr & 0x1) && (iccicr & 0x1)) {
        xil_printf("GIC Mode: Hardware Interrupts ENABLED\n");
    } else {
        xil_printf("GIC Mode: Software Interrupts (or Disabled)\n");
    }

    xil_printf("----------------------------\r\n");
}


void PrintTimerState(XTmrCtr *TimerInstance, u8 TimerCounter) {
    u32 TCSR, TLR, TCR;

    // Read Control/Status Register
    TCSR = XTmrCtr_ReadReg(TimerInstance->BaseAddress, TimerCounter, TimerCounter * 0x10 + 0x00);

    // Read Load Register
    TLR = XTmrCtr_ReadReg(TimerInstance->BaseAddress, TimerCounter, TimerCounter * 0x10 + 0x04);

    // Read Current Timer Value
    TCR = XTmrCtr_ReadReg(TimerInstance->BaseAddress, TimerCounter, TimerCounter * 0x10 + 0x08);

    xil_printf("Timer %d State:\r\n", TimerCounter);
    xil_printf("TCSR: 0x%08X\r\n", TCSR);
    xil_printf("TLR : 0x%08X\r\n", TLR);
    xil_printf("TCR : 0x%08X\r\n", TCR);

    // Check important control bits
    xil_printf("Interrupt Enabled (ENIT): %s\r\n", (TCSR & (1 << 6)) ? "YES" : "NO");
    xil_printf("Timer Enabled (ENT): %s\r\n", (TCSR & (1 << 7)) ? "YES" : "NO");
    xil_printf("Auto Reload (ARHT): %s\r\n", (TCSR & (1 << 4)) ? "YES" : "NO");
    xil_printf("Interrupt Occurred (TINT): %s\r\n", (TCSR & (1 << 8)) ? "YES" : "NO");

    if (TCSR & (1 << 6)) {
        xil_printf("AXI Timer Mode: Hardware Interrupts ENABLED\n");
    } else {
        xil_printf("AXI Timer Mode: Software Interrupts (or Disabled)\n");
    }
    xil_printf("--------------------------------\n");
}

int main() {
    // SetupTimerInterrupt();
	LEDTimerInit();

    sleep(1);
    PrintGICStatus(&gic_interrupt_controller, XPAR_FABRIC_AXI_TIMER_LED_INTR);
    PrintTimerState(&led_timer, TIMER_COUNTER_0);

    sleep(1);

    u32 TimerValue;
    TimerValue = XTmrCtr_GetValue(&led_timer, TIMER_COUNTER_0);
    xil_printf("Timer Value: 0x%08X\r\n", TimerValue);

    sleep(1);
    TimerValue = XTmrCtr_GetValue(&led_timer, TIMER_COUNTER_0);
    xil_printf("Timer Value: 0x%08X\r\n", TimerValue);


    // Stop the timer
    // XTmrCtr_Stop(&led_timer, TIMER_COUNTER_0);

	while(1) {
        if (TimerInterruptFlag) {
            xil_printf("Timer Interrupt Flag Detected in Main Loop!\r\n");
            TimerInterruptFlag = 0;  // Reset the flag
        }
    }

	return 0;
}