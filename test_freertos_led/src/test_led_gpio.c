#include <stdio.h>
#include "xparameters.h"
#include "xil_io.h"
#include "xgpio.h"

#include "sleep.h"

XGpio led_device;
XGpio_Config *gpio_cfg_ptr;

#define LED_ID_ADDRESS XPAR_AXI_GPIO_LED_BASEADDR
#define LED_CHANNEL 1

#define LED_1_MASK		0b0001
#define LED_1_INV_MASK	0b0000


void LEDGPIOInit() {
	gpio_cfg_ptr = XGpio_LookupConfig(LED_ID_ADDRESS);
    if (gpio_cfg_ptr == NULL) {
        printf("Failed to find configuration GPIO for Base Address: 0x%08X\n", LED_ID_ADDRESS);
    }
	XGpio_CfgInitialize(&led_device, gpio_cfg_ptr, gpio_cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&led_device, LED_CHANNEL, 0);
	XGpio_DiscreteWrite(&led_device, LED_CHANNEL, LED_1_INV_MASK);
}


int main() {
	LEDGPIOInit();

    XGpio_DiscreteWrite(&led_device, LED_CHANNEL, LED_1_MASK);
    printf("0x%08X\r\n", XGpio_DiscreteRead(&led_device, LED_CHANNEL));
	sleep(1);
	XGpio_DiscreteWrite(&led_device, LED_CHANNEL, LED_1_INV_MASK);

	while(1);

	return 0;
}