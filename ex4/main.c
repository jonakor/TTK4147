#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gpio.h>
#include <power_clocks_lib.h>
#include <sysclk.h>
#include <compiler.h>
#include <board.h>
#include <stdio_usb.h>


#define CONFIG_USART_IF (AVR32_USART2)

#include "FreeRTOS.h"
#include "task.h"

#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00


void busy_delay_ms(int delay){
    for(; delay != 0; delay--){
        for(int i = 0; i < 2108; i++){
            asm volatile ("" ::: "memory");
        }
    }
}

void busy_delay_short(void){
    for(int i = 0; i < 10; i++){
        asm volatile ("" ::: "memory");
    }
}

void init(){
	board_init();
	
    gpio_configure_pin(TEST_A, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_B, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_C, GPIO_DIR_INPUT);
    gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
	
	stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
	    setbuf(stdout, NULL);
	    setbuf(stdin,  NULL);
    #endif
}

static void taskFn(void* args){
	const portTickType delay = 1000 / portTICK_RATE_MS;
	
    int iter = 0;

	while(1){
		gpio_toggle_pin(LED0_GPIO);
		printf("tick %d\n", iter++);
		
		vTaskDelay(delay);
	}
}

typedef struct {
	int pin;
	int interval;
	} ledPara_t;


static void ledTask(void* args) {
	
	while(1){
		gpio_toggle_pin(((ledPara_t*) args)->pin);
		vTaskDelay(((ledPara_t*) args)->interval);
	}
}


struct handlePinArgs {
	unsigned int testPin;
	unsigned int responsePin;
	};
	

static void handlePin(void* args) {
	struct handlePinArgs a = *(struct handlePinArgs*)args;
	portTickType xLastWakeTime = xTaskGetTickCount();
	while(1) {		
		if (gpio_pin_is_low(a.testPin)) {
			if (a.testPin == TEST_C) {
				busy_delay_ms(3);
				gpio_set_pin_low(a.responsePin);
				vTaskDelay(1);
				gpio_set_pin_high(a.responsePin);
			}
			else {
				gpio_set_pin_low(a.responsePin);
				vTaskDelay(1);
				gpio_set_pin_high(a.responsePin);
			}
		}
		else {
			vTaskDelay(1);
		}
	}
	
}


int main(){
	init();
	
	struct handlePinArgs A = {TEST_A, RESPONSE_A};
    struct handlePinArgs B = {TEST_B, RESPONSE_B};
	struct handlePinArgs C = {TEST_C, RESPONSE_C};
		
	xTaskCreate(handlePin, "A", 1024, &A, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(handlePin, "B", 1024, &B, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(handlePin, "C", 1024, &C, tskIDLE_PRIORITY + 1, NULL);

	
	vTaskStartScheduler();
	
	while(1);
		
	return 0;
}

