#include <sleep.h>
#include <stdio.h>
#include "xparameters.h"
#include "xscugic.h"
#include "xscutimer.h"
#include "xil_exception.h"

#define LED_GIC_ADDRESS XPAR_XSCUGIC_0_BASEADDR
#define LED_TIMER_ADDRESS XPAR_AXI_TIMER_LED_BASEADDR

#define AXI_TIMER_FREQ XPAR_AXI_TIMER_LED_CLOCK_FREQUENCY

#define TIMER_COUNTER_0       0
#define TIMER_COUNTER_1       1

#define LOAD_VALUE 0X7B80

XScuTimer timer;
XScuTimer_Config *timer_cfg_ptr;

XScuGic gic_interrupt_controller;
XScuGic_Config *gic_cfg_ptr;

volatile int TimerInterruptFlag = 0;

void LEDTimerInterruptHandler(void *CallBackRef) {
    TimerInterruptFlag = 1;

    printf("SCU Timer Interrupt Triggered!\r\n");
}

void SetupTimerInterrupt() {
    gic_cfg_ptr = XScuGic_LookupConfig(LED_GIC_ADDRESS);
    if (gic_cfg_ptr == NULL) {
        printf("Failed to find configuration GIC for Base Address: 0x%08X\n", LED_GIC_ADDRESS);
        exit(1);
    }
    
	XScuGic_CfgInitialize(&gic_interrupt_controller, gic_cfg_ptr, gic_cfg_ptr->CpuBaseAddress);

    int Status = XScuGic_Connect(&gic_interrupt_controller, XPAR_SCUTIMER_INTR, 
    (Xil_ExceptionHandler)LEDTimerInterruptHandler, (void *)&timer);
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

void LEDTimerInit() {
    SetupTimerInterrupt();
    timer_cfg_ptr = XScuTimer_LookupConfig(XPAR_SCUTIMER_BASEADDR);
	XScuTimer_CfgInitialize(&timer, timer_cfg_ptr, timer_cfg_ptr->BaseAddr);
	XScuTimer_LoadTimer(&timer, LOAD_VALUE);
	XScuTimer_EnableAutoReload(&timer);
    XScuTimer_EnableInterrupt(&timer);
    XScuTimer_Start(&timer);
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
    xil_printf("GIC CPU Interface Base Address: 0x%08X\r\n", gic_interrupt_controller.Config->CpuBaseAddress);
    xil_printf("GIC Distributor Base Address: 0x%08X\r\n", gic_interrupt_controller.Config->DistBaseAddress);
    // u32 iser_offset = 0x100 + ((XPAR_FABRIC_AXI_TIMER_LED_INTR / 32) * 4);  // Offset for the interrupt group
    u32 iser_value = Xil_In32(gic_interrupt_controller.Config->DistBaseAddress + iser_offset);

    xil_printf("ICDISER[%d]: 0x%08X\r\n", XPAR_SCUTIMER_INTR / 32, iser_value);
    xil_printf("IRQ Handler Address: 0x%08X\r\n", (u32)XScuGic_InterruptHandler);


    // Check if the specific interrupt is enabled
    xil_printf("Interrupt ID %d Enabled: %s\r\n", InterruptID, 
               (icdiser & (1 << (InterruptID % 32))) ? "YES" : "NO");
    
    if ((icddcr & 0x1) && (iccicr & 0x1)) {
        xil_printf("GIC Mode: Hardware Interrupts ENABLED\n");
    } else {
        xil_printf("GIC Mode: Software Interrupts (or Disabled)\n");
    }

    if (icddcr & 0x1) {
        xil_printf("Group 0 Interrupts are ENABLED.\n");
    } else {
        xil_printf("Group 0 Interrupts are DISABLED.\n");
    }

    xil_printf("----------------------------\r\n");

    u32 reg_offset = (InterruptID / 32) * 4; 
    u32 bit_mask   = 1 << (InterruptID % 32);       // Mask for the specific interrupt

    // Read Interrupt Set-Pending Register (ICDISPR)
    u32 icdispr = Xil_In32(DistBaseAddr + 0x200 + reg_offset);

    // Read Interrupt Clear-Pending Register (ICDICPR)
    u32 icdicpr = Xil_In32(DistBaseAddr + 0x280 + reg_offset);

    xil_printf("----------------------------\r\n");
    xil_printf("Interrupt Set-Pending Register (ICDISPR): 0x%08X\r\n", icdispr);
    xil_printf("Interrupt Clear-Pending Register (ICDICPR): 0x%08X\r\n", icdicpr);

    // Check if the specific interrupt is pending
    xil_printf("Interrupt ID %d Pending Status: %s\r\n", InterruptID, 
               (icdispr & bit_mask) ? "PENDING" : "NOT PENDING");

    // Check if the interrupt is cleared
    xil_printf("Interrupt ID %d Cleared Status: %s\r\n", InterruptID, 
               (icdicpr & bit_mask) ? "CLEARED" : "NOT CLEARED");

    xil_printf("----------------------------\r\n");

    reg_offset = (InterruptID / 4) * 4;  // 4 interrupts per 32-bit register
    u32 byte_shift = (InterruptID % 4) * 8;  // Each interrupt takes 8 bits

    // Read the Interrupt Priority Register
    u32 icdipr = Xil_In32(DistBaseAddr + 0x400 + reg_offset);

    // Extract the priority level for the specific interrupt
    u8 priority = (icdipr >> byte_shift) & 0xFF;

    xil_printf("----------------------------\r\n");
    xil_printf("Interrupt Priority Register (ICDIPR): 0x%08X\r\n", icdipr);
    xil_printf("AXI Timer Interrupt ID %d Priority Level: %d\r\n", InterruptID, priority);
    xil_printf("----------------------------\r\n");
}


// void PrintTimerState(XTmrCtr *TimerInstance, u8 TimerCounter) {
//     u32 TCSR, TLR, TCR;

//     // Read Control/Status Register
//     TCSR = XTmrCtr_ReadReg(TimerInstance->BaseAddress, TimerCounter, TimerCounter * 0x10 + 0x00);

//     // Read Load Register
//     TLR = XTmrCtr_ReadReg(TimerInstance->BaseAddress, TimerCounter, TimerCounter * 0x10 + 0x04);

//     // Read Current Timer Value
//     TCR = XTmrCtr_ReadReg(TimerInstance->BaseAddress, TimerCounter, TimerCounter * 0x10 + 0x08);

//     xil_printf("Timer %d State:\r\n", TimerCounter);
//     xil_printf("TCSR: 0x%08X\r\n", TCSR);
//     xil_printf("TLR : 0x%08X\r\n", TLR);
//     xil_printf("TCR : 0x%08X\r\n", TCR);

//     // Check important control bits
//     xil_printf("Interrupt Enabled (ENIT): %s\r\n", (TCSR & (1 << 6)) ? "YES" : "NO");
//     xil_printf("Timer Enabled (ENT): %s\r\n", (TCSR & (1 << 7)) ? "YES" : "NO");
//     xil_printf("Auto Reload (ARHT): %s\r\n", (TCSR & (1 << 4)) ? "YES" : "NO");
//     xil_printf("Interrupt Occurred (TINT): %s\r\n", (TCSR & (1 << 8)) ? "YES" : "NO");

//     if (TCSR & (1 << 6)) {
//         xil_printf("AXI Timer Mode: Hardware Interrupts ENABLED\n");
//     } else {
//         xil_printf("AXI Timer Mode: Software Interrupts (or Disabled)\n");
//     }
//     xil_printf("--------------------------------\n");
// }

int main() {
    // SetupTimerInterrupt();
	LEDTimerInit();

    sleep(1);
    PrintGICStatus(&gic_interrupt_controller, XPAR_SCUTIMER_INTERRUPTS);
    // PrintTimerState(&led_timer, TIMER_COUNTER_0);

    // sleep(1);

    // u32 TimerValue;
    // TimerValue = XTmrCtr_GetValue(&led_timer, TIMER_COUNTER_0);
    // xil_printf("Timer Value: 0x%08X\r\n", TimerValue);

    // sleep(1);
    // TimerValue = XTmrCtr_GetValue(&led_timer, TIMER_COUNTER_0);
    // xil_printf("Timer Value: 0x%08X\r\n", TimerValue);

    xil_printf("System Initialization Complete. Waiting for Interrupts...\r\n");


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