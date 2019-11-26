#include "stm32l476xx.h"
#include "LED.h"
#include "LCD.h"
#include<stdio.h>
#include<stdlib.h>


uint8_t generateCode(uint8_t seed);
void Keypad_Init(void);
uint8_t scanKeys(void);
void delay(void);

//volatile uint32_t TimeDelay;
//volatile uint32_t timer;
//void SysTick_Initialize(uint32_t ticks);
//void Delay(uint32_t nTime);
//void SysTick_Handler(void);



int main(void) {
	uint8_t passcode[1] = "1";
	uint8_t guess[1] = "N";
	
	LED_Init();
	Keypad_Init();
	//SysTick_Initialize(4000);
	//timer = 0;
	//int i
	//for (i = 0; i < 4; i++) {
	passcode[0] = generateCode(0x00); //+i
	//}

	while(1) {
		guess[0] = scanKeys();
		if (guess[0] == passcode[0]) {
			Green_LED_On();
			delay();
			Green_LED_Off();
			passcode[0] = generateCode(guess[0]);
			guess[0] = 'N';
		}else{
			Red_LED_On();
			delay();
			Red_LED_Off();
		}
	}
	return 0;
}

uint8_t generateCode(uint8_t seed) {
	uint8_t code = (seed % 10) + 48;
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
	//Add while loop to continue to check for an input (Use a bool)
	uint8_t key = 0x4E; //N;
	int output;
	bool input = 0;
	
	while(!input) {
		//scanning row 1
		GPIOA->MODER &= ~0x00000CF0; //set pin 2,3,5 to input to prevent short circuit
		GPIOA->ODR &= ~0x00000002;    //set pin 1 to 0;
		output = (GPIOA->ODR & GPIO_ODR_ODR_1);
		delay();
		if ((GPIOE->IDR & GPIO_IDR_IDR_10) == output) {
			key = 0x31; //1
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_11) == output) {
			key = 0x32; //2
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_12) == output) {
			key = 0x33; //3
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_13) == output) {
			key = 0x41; //A
			input = 1;
		}
		Keypad_Init();


		//scanning row 2
		GPIOA->MODER &= ~0x00000CCC; //set pin 1,3,5 to input to prevent short circuit
		GPIOA->ODR &= ~0x00000004; //set pin 2 to 0;
		output = (GPIOA->ODR & GPIO_ODR_ODR_2);
		delay();
		if ((GPIOE->IDR & GPIO_IDR_IDR_10) == output) {
			key = 0x34; //4
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_11) == output) {
			key = 0x35; //5
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_12) == output) {
			key = 0x36; //6
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_13) == output) {
			key = 0x42; //B
			input = 1;
		}
		Keypad_Init();

		//scanning row 3
		GPIOA->MODER &= ~0x00000C3C; //set pin 1,2,5 to input to prevent short circuit
		GPIOA->ODR &= ~0x00000008; //set pin 3 to 0;
		output = (GPIOA->ODR & GPIO_ODR_ODR_3);
		delay();
		if ((GPIOE->IDR & GPIO_IDR_IDR_10) == output) {
			key = 0x37; //7
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_11) == output) {
				key = 0x38; //8
				input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_12) == output) {
			key = 0x39; //9
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_13) == output) {
			key = 0x43; //C
			input = 1;
		}
		Keypad_Init();

		//scanning row 5
		GPIOA->MODER &= ~0x000000FC; //set pin 1,2,3 to input to prevent short circuit
		GPIOA->ODR &= ~0x00000010; //set pin 5 to 0;
		output = (GPIOA->ODR & GPIO_ODR_ODR_5);
		delay();
		if ((GPIOE->IDR & GPIO_IDR_IDR_10) == output) {
			key = 0x2A; //*
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_11) == output) {
			key = 0x30; //0
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_12) == output) {
			key = 0x23; //#
			input = 1;
		}else if ((GPIOE->IDR & GPIO_IDR_IDR_13) == output) {
			key = 0x44; //D
			input = 1;
		}	
		Keypad_Init();
	}
	
	return key;
}

//void SysTick_Initialize(uint32_t ticks) {
//	
//	// Disable SysTick IRQ and SysTick counter
//	SysTick->CTRL = 0;
//	
//	// Set reload register
//	SysTick->LOAD = ticks - 1;
//	
//	// Set interrupt priority of SysTick
//	// Make SysTick least urgent (i.e. highest priority number)
//	// __NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
//	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
//	
//	// Reset the SysTick counter value
//	SysTick->VAL = 0;
//	
//	// Select processor clock
//	// 1 = processor clock; 0 = external clock
//	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
//	
//	// Enable SysTick exception request
//	// 1 = counting down to zero asserts the SysTick exception request
//	// 0 = counting down to zero does no assert the SysTick exception request
//	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
//	
//	// Enable SysTick timer
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
//}


void delay() {
	int i;
	for (i = 0; i < 500000; i++) {
	}
}

//SysTick interrupt service routine
//void SysTick_Handler(void) {
//	if (TimeDelay > 0) {
//		TimeDelay--;
//	}
//	timer++;
//}

//// nTime: specifies the delay time length
//void Delay(uint32_t nTime) {
//	TimeDelay = nTime;
//	while(TimeDelay != 0);
//}
