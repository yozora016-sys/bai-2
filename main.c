#include "stm32f10x.h" 


#define LED1_PORT GPIOA
#define LED1_PIN  GPIO_Pin_5
#define LED2_PORT GPIOC
#define LED2_PIN  GPIO_Pin_13
#define BUTTON_PORT GPIOA
#define BUTTON_PIN GPIO_Pin_4

// Bi?n tr?ng thái
uint32_t last_blink_time = 0; 
uint8_t previous_button_state = 1; 

// Hàm c?u hình GPIO
void GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = LED1_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED1_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED2_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = BUTTON_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
}

void SysTick_Init(void) {
    SysTick_Config(SystemCoreClock / 1000); 
}

volatile uint32_t ms_tick = 0;

void SysTick_Handler(void) {
    ms_tick++;
}

uint32_t get_ms_tick(void) {
    return ms_tick;
}

int main(void) {
    SystemInit(); 
    SysTick_Init(); 
    GPIO_Configuration(); 

    while(1) {
        // --- task 1: Nh?p nháy LED ---
        if (get_ms_tick() - last_blink_time >= 500) {
            GPIO_SetBits(LED1_PORT, LED1_PIN);
            GPIO_SetBits(LED2_PORT, LED2_PIN);
 
            last_blink_time = get_ms_tick();
        }

        // --- task 2: Ði?u khi?n LED b?ng nút nh?n ---
        uint8_t current_button_state = GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN);

        if (current_button_state == 0 && previous_button_state == 1) {      
            uint32_t debounce_start_time = get_ms_tick();
            while(get_ms_tick() - debounce_start_time < 20 && GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN) == 0);

            if (GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN) == 0) {
                GPIO_SetBits(LED1_PORT, LED1_PIN);
            }
        }
        
        previous_button_state = current_button_state;
    }
}