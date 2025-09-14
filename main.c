#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

void GPIO_Config(void) {
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio);

    GPIO_SetBits(GPIOC, GPIO_Pin_13); // T?t LED ban d?u (PC13 active low)
}

void TIM2_Config(void) {
    TIM_TimeBaseInitTypeDef tim;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    tim.TIM_Prescaler = 7200 - 1;   // 72MHz/7200 = 10kHz
    tim.TIM_Period = 5000 - 1;      // 10kHz / 5000 = 2Hz -> 500ms
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        GPIOC->ODR ^= GPIO_Pin_13;  // Ð?o LED PC13
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

int main(void) {
    GPIO_Config();
    TIM2_Config();
    while (1) {
        // Chuong trình chính r?ng
    }
}
