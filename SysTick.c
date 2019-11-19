#include "stm32l476xx.h"
#include "LED.h"
#include "LCD.h"

volatile uint32_t TimeDelay;
void SysTick_Initialize(uint32_t ticks);
void Delay(uint32_t nTime);
void SysTick_Handler(void);

int main(void) {
	uint32_t ticks = 4000;	
	uint8_t currTime[6] = "000000";
	uint32_t totalTime = 0;
	bool status = 0;
	bool start = 0;
	LED_Init(); //Initializes LED
  SysTick_Initialize(ticks);
	LCD_Initialization();
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODER0);
	LCD_DisplayString(&currTime);
	while(1) {
		if (GPIOA->IDR & GPIO_IDR_IDR_0) {
				while(GPIOA->IDR & GPIO_IDR_IDR_0){
				}
				start ^= 1;
		}
		if (start == 1) {
			Delay(1);
			Red_LED_Toggle();
			totalTime += 23;
				Time_Conversion(totalTime, &currTime);
				LCD_DisplayString(&currTime);
		}
	}
	
}

// ticks: number of ticks between two interrupts
void SysTick_Initialize(uint32_t ticks) {
	
	// Disable SysTick IRQ and SysTick counter
	SysTick->CTRL = 0;
	
	// Set reload register
	SysTick->LOAD = ticks - 1;
	
	// Set interrupt priority of SysTick
	// Make SysTick least urgent (i.e. highest priority number)
	// __NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	// Reset the SysTick counter value
	SysTick->VAL = 0;
	
	// Select processor clock
	// 1 = processor clock; 0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	// Enable SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does no assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// SysTick interrupt service routine
void SysTick_Handler(void) {
	if (TimeDelay > 0)
		TimeDelay--;
}

// nTime: specifies the delay time length
void Delay(uint32_t nTime) {
	TimeDelay = nTime;
	while(TimeDelay != 0);
}
