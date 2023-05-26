#include <stdint.h>
#include <stdio.h>
#include <stm8s_uart1.h>
#include <stm8s_tim1.h>


void InitUART(void)
{
        UART1_DeInit();
        UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
                        UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
        UART1_Cmd(ENABLE);
}

void InitTMR1(void) {
        TIM2_DeInit();
        TIM2_TimeBaseInit(TIM2_PRESCALER_4096, 1); // 1 = ~1ms
        TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
        TIM2_Cmd(ENABLE);
}

