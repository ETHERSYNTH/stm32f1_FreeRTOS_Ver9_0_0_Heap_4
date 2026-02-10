#include "beep.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void Beep1_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;   // AFIO(Alternate Function I/O) clocking for EXTI
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN(BEEP1_GPIO_PIN_NUM);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// GPIO_Mode_Out_PP - it is usual (output Push-Pull) / GPIO_Mode_AF_PP - it is alternative (Alternate Function Push-Pull) 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT(BEEP1_GPIO_PORT_NUM), &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_PORT(BEEP1_GPIO_PORT_NUM),GPIO_PIN(BEEP1_GPIO_PIN_NUM));
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN(BUTTON1_GPIO_PIN_NUM);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU, 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT(BUTTON1_GPIO_PORT_NUM), &GPIO_InitStructure);
	
	// PB10 (EXTI10)
	// AFIO->EXTICR[2] — Register for source choice for lines EXTI 8-11
	// EXTICR[0] — EXTICR1:Line 0-3
	// EXTICR[1] — EXTICR2:Line 4-7
	// EXTICR[2] — EXTICR3:Line 8-11
	// EXTICR[3] — EXTICR4:Line 12-15
	AFIO->EXTICR[2] &= ~(AFIO_EXTICR3_EXTI10);
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PB; // Connect
    
    // EXTI10 settings
    EXTI->IMR |= EXTI_IMR_MR10; 	// Mask between EVENT - INTERRUPT, Line 10 enable
	EXTI->FTSR |= EXTI_FTSR_TR10;	// fall edge	
	EXTI->RTSR |= EXTI_RTSR_TR10;	// rise edge

    // NVIC settings
    NVIC_SetPriority(EXTI15_10_IRQn, 1);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	//__enable_irq ();
	
	
}

void Beep2_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN(BEEP2_GPIO_PIN_NUM);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT(BEEP1_GPIO_PORT_NUM), &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_PORT(BEEP2_GPIO_PORT_NUM),GPIO_PIN(BEEP2_GPIO_PIN_NUM));
}
