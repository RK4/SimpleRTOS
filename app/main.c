#include <stm32f4_discovery.h>
#include <stm32f4xx_conf.h> 
#include <os.h>
#include <periodic_thread.h>

/* GPIO Init Structure required for LEDs */
GPIO_InitTypeDef  GPIO_InitStructure;

/**
  * @brief  This thread toggles led 12 and sleeps for 1 seconds.
  * @param  None
  * @retval None
  */
void thread_led0()
{
	while(1)
	{
		/* Toggle LED */
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		/* Sleep for 5 seconds */
		OS_Sleep(1000);
	}
}

/**
  * @brief  This thread toggles led 13 and sleeps for 2 seconds.
  * @param  None
  * @retval None
  */
void thread_led1()
{
	while(1)
	{
		/* Toggle LED */
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		/* Sleep for 3 seconds */
		OS_Sleep(2000);
	}
}

/**
  * @brief  This is a dummy thread. We need at least one running thread in the system in case
  * 		when other threads are sleeping. This thread has lowest priority.
  * @param  None
  * @retval None
  */
void thread_dummy()
{
	while(1);
}

void init_led()
{
	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

int main(void)
{
	/* Initialize OS */
	OS_Init();

	/* Initialize all LEDs */
	init_led();

	/* Add threads */
	OS_AddThread(thread_led0, 1);
	OS_AddThread(thread_led1, 2);
	OS_AddThread(thread_dummy, 254);

	/* Start OS */
	OS_Launch(168000000);

	/* Should not reach here */
}

