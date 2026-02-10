#include "FreeRTOS.h"
#include "Header_Task1.h"
#include "task.h"
#include "queue.h"
#include "misc.h"
#include "beep.h"
#include <stdio.h>

/*
	uint16_t address;
	uint16_t *data;
	enum{
		COIL,
		DISCRETE,
		INPUT,
		HOLDING}type;
*/

struct modbus_reg{
	uint16_t address;
	uint16_t *data;
	enum{
		COIL,
		DISCRETE,
		INPUT,
		HOLDING}type;
	uint8_t size;
};	

struct modbus_reg modbus_reg_var;
volatile uint32_t offset;

// Prototypes
void LED0_Task(void * pvParameters);
void BUTT0_Task(void * pvParameters);

void EXTI15_10_IRQHandler(void);



static QueueHandle_t button_event;


int main(void)
{
	
	volatile uint32_t sizestr = sizeof(modbus_reg_var);
	
	struct modbus_reg reg;
	
	offset = (char*)&reg.address - (char*)&reg;  // 0
	offset = (char*)&reg.data - (char*)&reg;     // 4
	offset = (char*)&reg.type - (char*)&reg;     // 8
	offset = (char*)&reg.size - (char*)&reg;     // 9

/*	
Bytes: 0   1   2   3   4   5   6   7   8   9   10  11
      [address][pad][   data ptr   ][type][size][pad]
	
	Optimization Level0 [-O 0]
*/
	
	task1_init();
	
    xTaskCreate(LED0_Task, (const char *)"LED0", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(BUTT0_Task, (const char *)"BUTT", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL);

    vTaskStartScheduler();
}

void LED0_Task(void * pvParameters)
{
	Beep1_Init();
    while (1)
    {
		GPIO_SetBits(GPIO_PORT(BEEP1_GPIO_PORT_NUM),GPIO_PIN(BEEP1_GPIO_PIN_NUM));
        vTaskDelay(1000 / portTICK_RATE_MS);
		GPIO_ResetBits(GPIO_PORT(BEEP1_GPIO_PORT_NUM),GPIO_PIN(BEEP1_GPIO_PIN_NUM));
		vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void BUTT0_Task(void * pvParameters)
{
	Beep2_Init();
	button_event = xQueueCreate(10, sizeof(uint32_t));
	configASSERT(button_event);
	
    while (1)
    {
		uint32_t pin_status;
		BaseType_t result = xQueueReceive(button_event, &pin_status, portMAX_DELAY);
		if(pin_status)
			GPIO_SetBits(GPIO_PORT(BEEP2_GPIO_PORT_NUM),GPIO_PIN(BEEP2_GPIO_PIN_NUM)); // PA6 Enable 
		else
			GPIO_ResetBits(GPIO_PORT(BEEP2_GPIO_PORT_NUM),GPIO_PIN(BEEP2_GPIO_PIN_NUM)); // PA6 Disable
    }
}



void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR10)
    {

		
		
		BaseType_t xHigherPriorityTaskWoken = pdFALSE, xResult;
		
		uint32_t pin_status = GPIOB->IDR & (1 << BUTTON1_GPIO_PIN_NUM); //PB10
		xResult = xQueueSendFromISR(button_event, &pin_status, &xHigherPriorityTaskWoken);
		
		// Reset Interrupt Pending flag
        EXTI->PR = EXTI_PR_PR10; 
		
		if(xResult != pdFAIL){
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
    }
}

