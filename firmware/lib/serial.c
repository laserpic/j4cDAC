#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"

#include <stdarg.h>
#include <stdio.h>

#define DEBUG_UART	((LPC_UART_TypeDef *)LPC_UART0)

void serial_init() {
	/* Configure pins */
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);

	/* Start up the UART */
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = 115200;
	UART_Init(DEBUG_UART, &UARTConfigStruct);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	UART_FIFOConfig(DEBUG_UART, &UARTFIFOConfigStruct);
	UART_TxCmd(DEBUG_UART, ENABLE);
}

void outputf(const char *fmt, ...) {
        va_list va;
	char buffer[80];
	int n;

	va_start(va, fmt);
	n = vsnprintf(buffer, sizeof(buffer) - 2, fmt, va);

	if (n > (sizeof(buffer) - 2)) n = sizeof(buffer) - 2;

	buffer[n] = '\r';
	buffer[n + 1] = '\n';

	UART_Send(DEBUG_UART, (uint8_t *)buffer, n + 2, BLOCKING);
}