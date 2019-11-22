#include "stm32l476xx.h"
#include "LED.h"
#include "LCD.h"
#include<stdio.h>
#include<stdlib.h>

void generateCode(uint8_t* passcode);
void playGame();
void Keypad_Init();
uint8_t scanKeys();


int main(void) {

	LED_Init();
	Keypad_Init();
	while(1) {
		playGame();
	}

	return 0;
}

void generateCode(uint8_t* passcode) {
		passcode  = (rand() % 10) + 48;
}

void playGame() {
	int i;
	uint8_t passcode = "1";
	uint8_t guess = 0x00;

	//for (i = 0; i < 4; i++) {
		generateCode(&passcode); //+i
	//}

	while(1) {
		if (guess == 0x00) {
			guess = scanKeys();
		}else {
			if (guess == passcode) {
				Green_LED_On();
				delay(1000);
				Green_LED_Off();
				generateCode(&passcode);
				guess = 0x00;
			}else{
				Red_LED_On();
				delay(1000);
				Red_LED_Off();
			}
		}
	}
}

void Keypad_Init() {
	
	// Enable the peripheral clock of GPIO Port	A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// GPIOA Mode: Output(01) (Pins 1,2,3,5)
	GPIOA->MODER &= ~0x00000CFC //clear
	GPIOA->MODER |= ~0x00000454 //set

	// GPIO Speed: High Speed (11)
	GPIOA->OSPEEDR  &= ~0x00000CFC //clear 
	GPIOA->OSPEEDR |= ~0x00000454 //set

	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER  &= ~0x0000002E //clear

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10),
	GPIOA->PUPDR &= ~0x00000CFC //clear

	// Enable the peripheral clock of GPIO Port	A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	// GPIOA Mode: Input(00) (Pins 10,11,12,13)
	GPIOE->MODER &= ~0x0FF00000 //clear

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10),
	GPIOE->PUPDR &= ~0x0FF00000

}

uint8_t scanKeys() {
	int i;

	//scanning row 1
	GPIOA->MODER &= ~0x00000CF8 //set pin 2,3,5 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_1; //set pin 1 to 0;
	if (~(GPIOE->IDR & GPIO_IDR_IDR_10)) {
		return '1';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_11)) {
		return '2';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_12)) {
		return '3';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_13)) {
		return 'A';
	}
	GPIOA->MODER &= ~0x00000CFC //clear
	GPIOA->MODER |= ~0x00000454 //set


	//scanning row 2
	GPIOA->MODER &= ~0x00000CF4 //set pin 1,3,5 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_2; //set pin 2 to 0;
	if (~(GPIOE->IDR & GPIO_IDR_IDR_10)) {
		return '4';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_11)) {
		return '5';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_12)) {
		return '6';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_13)) {
		return 'B';
	}
	GPIOA->MODER &= ~0x00000CFC //clear
	GPIOA->MODER |= ~0x00000454 //set

	//scanning row 3
	GPIOA->MODER &= ~0x00000CCC //set pin 1,2,5 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_3; //set pin 3 to 0;
	if (~(GPIOE->IDR & GPIO_IDR_IDR_10)) {
		return '7';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_11)) {
		return '8';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_12)) {
		return '9';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_13)) {
		return 'C';
	}
	GPIOA->MODER &= ~0x00000CFC //clear
	GPIOA->MODER |= ~0x00000454 //set

	//scanning row 5
	GPIOA->MODER &= ~0x000000FC //set pin 1,2,3 to input to prevent short circuit
	GPIOA->ODR &= ~GPIO_ODR_ODR_5; //set pin 5 to 0;
	if (~(GPIOE->IDR & GPIO_IDR_IDR_10)) {
		return '*';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_11)) {
		return '0';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_12)) {
		return '#';
	}else if (~(GPIOE->IDR & GPIO_IDR_IDR_13)) {
		return 'D';
	}

	return 0x00;

}