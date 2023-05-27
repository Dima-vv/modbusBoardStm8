#include <stdint.h>
#include <stdio.h>
#include <stm8s_gpio.h>
#include <stm8s_uart1.h>
#include <stm8s_tim1.h>
#include <stm8s_flash.h>
#include "PetitModbus.h"

#define RELAY_1_GPIO GPIOB
#define RELAY_2_GPIO GPIOC
#define RELAY_3_GPIO GPIOC
#define RELAY_4_GPIO GPIOC
#define RELAY_1_PIN GPIO_PIN_4
#define RELAY_2_PIN GPIO_PIN_3
#define RELAY_3_PIN GPIO_PIN_4
#define RELAY_4_PIN GPIO_PIN_5

const GPIO_TypeDef *relayPorts[] = {RELAY_1_GPIO, RELAY_2_GPIO, RELAY_3_GPIO, RELAY_4_GPIO};
const GPIO_Pin_TypeDef relayPins[] = {RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN};

#define IN_1_GPIO GPIOD
#define IN_2_GPIO GPIOD
#define IN_3_GPIO GPIOC
#define IN_4_GPIO GPIOC
#define IN_1_PIN GPIO_PIN_3
#define IN_2_PIN GPIO_PIN_2
#define IN_3_PIN GPIO_PIN_7
#define IN_4_PIN GPIO_PIN_6

const GPIO_TypeDef *inPorts[] = {IN_1_GPIO, IN_2_GPIO, IN_3_GPIO, IN_3_GPIO};
const GPIO_Pin_TypeDef inPins[] = {IN_1_PIN, IN_2_PIN, IN_3_PIN, IN_4_PIN};

#define LED GPIO_PIN_4 #GPIOD;

void UART_RX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_RX)
{
	unsigned char tmp = UART1_ReceiveData8();
	ReceiveInterrupt(tmp);
}

void timer_isr() __interrupt(ITC_IRQ_TIM2_OVF)
{
	static uint16_t ms = 0;
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);

	if (++ms > 1000)
	{
		ms = 0;
		for (uint8_t i = 0; i < 4; i++)
		{
			if (PetitRegisters[i + 4].ActValue)
			{
				--PetitRegisters[i + 4].ActValue;
				if (0 == PetitRegisters[i + 4].ActValue)
				{
					GPIO_WriteHigh(relayPorts[i], relayPins[i]);
					PetitRegisters[i].ActValue = 0;
				}
			}
		}
	}
	PetitModBus_TimerValues();
}


// PetitRegisters:
// 0,1,2,3 - relays
// 4,5,6,7 - relays timers
// 8,9,10,11 - inputs
// 12 - Modbus address

void main(void)
{
	CLK_DeInit();
	// CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV2);

	// 4 relays pins configure as out and set int to hi (relay off)
	// 4 input pins configure as in with pullup
	for (int i = 0; i < 4; i++)
	{
		GPIO_Init(relayPorts[i], relayPins[i], GPIO_MODE_OUT_PP_LOW_SLOW);
		GPIO_WriteHigh(relayPorts[i], relayPins[i]);
		GPIO_Init(inPorts[i], inPins[i], GPIO_MODE_IN_PU_NO_IT);
	}

	FLASH_DeInit();
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	int mbAddr = FLASH_ReadByte(0x4001);
	PetitRegisters[12].ActValue = mbAddr;
	if (mbAddr == 0 || mbAddr == 255 || mbAddr == 1)
		mbAddr = 2;

	InitPetitModbus(mbAddr);
	enableInterrupts();

	for (;;)
	{
		// inputs
		for (int i = 0; i < 4; i++)
		{
			if (GPIO_ReadInputPin(inPorts[i], inPins[i]))
				PetitRegisters[i + 8].ActValue = 1;
			else
				PetitRegisters[i + 8].ActValue = 0;
		}

		ProcessPetitModbus();

		// Relays
		for (int i = 0; i < 4; i++)
		{
			if (PetitRegisters[i].ActValue & 0x0001)
			{
				GPIO_WriteLow(relayPorts[i], relayPins[i]); // relay  on
				if (PetitRegisters[i].ActValue & 0xFFFE)
				{
					if (PetitRegisters[i + 4].ActValue == 0)
						PetitRegisters[i + 4].ActValue = PetitRegisters[i].ActValue >> 1;
				}
				else
					PetitRegisters[i + 4].ActValue = 0;
			}
			else {
				GPIO_WriteHigh(relayPorts[i], relayPins[i]); // relay off
				PetitRegisters[i + 4].ActValue = 0; // clear timer
			}
		}
		if (PetitRegisters[12].ActValue && PetitRegisters[12].ActValue != 0 && PetitRegisters[12].ActValue != 255 && PetitRegisters[12].ActValue != mbAddr) {			
			FLASH_ProgramByte(0x4001, PetitRegisters[12].ActValue);
			mbAddr = PetitRegisters[12].ActValue;
		}
	}
}
