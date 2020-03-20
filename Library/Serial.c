#include "Serial.h"

char serialReceivedCharacter = 0;
uint8_t serialNewDataAvailable = 0;
char* serialTransmitData = 0;
uint8_t serialTransmitCompleted = 0;


void Serial_Init() {
	//Change the function of TX and RX pins for UART.
	set_uart_func(&Serial_UART_TX_PIN);
	set_uart_func(&Serial_UART_RX_PIN);
	
	//Turn on UART0.
	PCONP |= (1<<3);
	
	//Enable FIFO for UART0.
	LPC_UART0 ->FCR |= (1<<0);
	
	//In order to change the DLM, DLL and FDR values, Write correct code for enabling the access to Divisor Latches.
	LPC_UART0->LCR |= (1<<7);
	
	
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	Serial_UART->DLM = 0x00;
	Serial_UART->DLL = 0x13;
	Serial_UART->FDR = 0x05 << 0 | 0x07 << 4;
	
	
	//Write correct code for disabling the access to Divisor Latches.
	LPC_UART0->LCR &= ~(1 << 7);
	
	
	//Change LCR register value for 8-bit character transfer, 1 stop bits and Even Parity.
	
							
	//Enable the Receive Data Available and THRE Interrupt.
	
	//Enable UART0_IRQn Interrupt.
	
	//Set UART0_IRQn Priority to 5.
}

void set_uart_func(volatile uint32_t* iocon){
	uint32_t ioconRegisterValue = *iocon;
	ioconRegisterValue |= 0x4;
	ioconRegisterValue &= ~(3 << 0);
	*iocon = ioconRegisterValue;
}

void UART0_IRQHandler() {	
	uint32_t currentInterrupt = 0x00;
	
	//First if statement is for Receive Data Available interrupt.
	//When Receive Data Available, the code will enter to the first if condition.
	if(currentInterrupt == 0x00) {
		serialReceivedCharacter = Serial_ReadData();
		serialNewDataAvailable = 1;
	}
	//Second if statement is for THRE interrupt
	//When THRE interrupt is handled, the code will enter to the second if condition.
	else if(currentInterrupt == 0x00) {
		if(*serialTransmitData > 0) {
			Serial_WriteData(*serialTransmitData++);
		}
		else {
			serialTransmitCompleted = 1;
		}
	}
}

char Serial_ReadData() {
	return Serial_UART->RBR;
}

void Serial_WriteData(const char data) {
	serialTransmitCompleted = 0;
	Serial_UART->THR = data;
}

