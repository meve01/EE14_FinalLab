#include "stm32l476xx.h"
#include "LED.h"
#include "LCD.h"
#include<stdio.h>
#include<stdlib.h>

uint8_t generateCode(void);
void Keypad_Init(void);
void delay(void);
uint8_t scanKeys(void);


int main(void) {
	uint8_t passcode[1] = "1";
	uint8_t guess[1] = "N";
	
	LED_Init();
	Keypad_Init();
	//int i
	//for (i = 0; i < 4; i++) {
	passcode[0] = generateCode(); //+i
	//}

	while(1) {
		if (guess[0] == 'N') {
			guess[0] = scanKeys();
		}else {
			if (guess[0] == passcode[0]) {
				Green_LED_On();
				delay();
				Green_LED_Off();
				passcode[0] = generateCode();
				guess[0] = 'N';
			}else{
				Red_LED_On();
				delay();
				Red_LED_Off();
			}
		}
	}
	return 0;
}

uint8_t generateCode() {
	uint8_t code = (rand() % 10) + 48;
	return code;
}

void Keypad_Init() {
	
	// Enable the peripheral clock of GPIO Port	A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// GPIOA Mode: Output(01) (Pins 1,2,3,5)
	GPIOA->MODER &= ~(0x00000CFC); //clear
	GPIOA->MODER |= 0x00000454; //set

	// GPIO Speed: High Speed (11)
	GPIOA->OSPEEDR  &= ~(0x00000CFC); //clear 
	GPIOA->OSPEEDR |= 0x00000454; //set

	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER  &= ~0x0000002E; //clear

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10),
	GPIOA->PUPDR &= ~0x00000CFC; //clear

	// Enable the peripheral clock of GPIO Port	A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	// GPIOA Mode: Input(00) (Pins 10,11,12,13)
	GPIOE->MODER &= ~0x0FF00000; //clear

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10),
	GPIOE->PUPDR &= ~0x0FF00000;
}

uint8_t scanKeys() {

	uint8_t key = 0x4E; //N;
	
	//scanning row 1
	GPIOA->MODER &= ~0x00000CF8; //set pin 2,3,5 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_1; //set pin 1 to 0;
	if ((GPIOE->IDR & GPIO_IDR_IDR_10) == GPIOA->ODR) {
		key = 0x31; //1
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_11) == GPIOA->ODR) {
		key = 0x32; //2
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_12) == GPIOA->ODR) {
		key = 0x33; //3
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_13) == GPIOA->ODR) {
		key = 0x41; //A
	}
	GPIOA->MODER &= ~0x00000CFC; //clear
	GPIOA->MODER |= 0x00000454; //set


	//scanning row 2
	GPIOA->MODER &= ~0x00000CF4; //set pin 1,3,5 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_2; //set pin 2 to 0;
	if ((GPIOE->IDR & GPIO_IDR_IDR_10) == GPIOA->ODR) {
		key = 0x34; //4
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_11) == GPIOA->ODR) {
		key = 0x35; //5
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_12) == GPIOA->ODR) {
		key = 0x36; //6
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_13) == GPIOA->ODR) {
		key = 0x42; //B
	}
	GPIOA->MODER &= ~0x00000CFC; //clear
	GPIOA->MODER |= 0x00000454; //set

	//scanning row 3
	GPIOA->MODER &= ~0x00000CCC; //set pin 1,2,5 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_3; //set pin 3 to 0;
	if ((GPIOE->IDR & GPIO_IDR_IDR_10) == GPIOA->ODR) {
		key = 0x37; //7
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_11) == GPIOA->ODR) {
		key = 0x38; //8
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_12) == GPIOA->ODR) {
		key = 0x39; //9
	}else if ((GPIOE->IDR & GPIO_IDR_IDR_13) == GPIOA->ODR) {
		key = 0x43; //C
	}
	GPIOA->MODER &= ~0x00000CFC; //clear
	GPIOA->MODER |= 0x00000454; //set

	//scanning row 5
	GPIOA->MODER &= ~0x000000FC; //set pin 1,2,3 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_5; //set pin 5 to 0;
	if (~(GPIOE->IDR & GPIO_IDR_IDR_10) == GPIOA->ODR) {
		key = 0x2A; //*
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_11) == GPIOA->ODR) {
		key = 0x30; //0
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_12) == GPIOA->ODR) {
		key = 0x23; //#
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_13) == GPIOA->ODR) {
		key = 0x44; //D
	}

	return key;
}

void delay() {
	int i;
	for (i = 0; i < 500000; i++) {
	}
}
