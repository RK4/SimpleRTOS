/**
 * @author  Mohsin Shan
 * @version 0.1 , 18/12/16
 */
#include <stdint.h> 
#include <bsp.h>
#include <stm32f4xx.h>

void (*PeriodicTask[3])();      /* Array of Function Pointers to hold threads */ 


void BSP_PeriodicTask_Init1(void (*periodicTask)(), int period, int priority)
{   
    PeriodicTask[0] = periodicTask;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;     /* Enable Clock for Timer 2 */
    TIM2->CR1 &= ~TIM_CR1_CEN;              /* Disable Timer2 */
    TIM2->CR1 |= TIM_CR1_DIR;
    TIM2->PSC = 0;                          /* Set Prescaler to 0 */
    TIM2->ARR = (SystemCoreClock / 1000) * period - 1;  /* Set update value 
                                                           to period in ms  
                                                        */      
    NVIC_EnableIRQ(28);              /* Enable Interrupt in NVIC block */
    NVIC_SetPriority(28, priority);
    TIM2->DIER |= TIM_DIER_UIE;             /* Enable Timer Update Event Interrupt */
    TIM2->CR1 |= TIM_CR1_CEN;               /* Enable Timer */
}

void BSP_PeriodicTask_Init2(void (*periodicTask)(), int period, int priority)
{   
    PeriodicTask[1] = periodicTask;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     /* Enable Clock for Timer 3 */
    TIM3->CR1 &= ~TIM_CR1_CEN;              /* Disable Timer3 */
    TIM3->CR1 |= TIM_CR1_DIR;
    TIM3->PSC = 0;                          /* Set Prescaler to 0 */
    TIM3->ARR = (SystemCoreClock / 1000) * period - 1;  /* Set update value 
                                                            to period in ms  
                                                        */      
    
    NVIC_EnableIRQ(29);              /* Enable Interrupt in NVIC block */
    NVIC_SetPriority(29, priority);
    TIM3->DIER |= TIM_DIER_UIE;             /* Enable Timer Update Event Interrupt */
  TIM3->CR1 |= TIM_CR1_CEN;                 /* Enable Timer */
}

void BSP_PeriodicTask_Init3(void (*periodicTask)(), int period, int priority)
{   
    PeriodicTask[2] = periodicTask;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     /* Enable Clock for Timer 4 */
    TIM4->CR1 &= ~TIM_CR1_CEN;              /* Disable Timer4 */
    TIM4->CR1 |= TIM_CR1_DIR;
    TIM4->PSC = 0;                          /* Set Prescaler to 0 */
    
    TIM4->ARR = (SystemCoreClock / 1000) * period - 1;  /* Set update value 
                                                           to period in ms
                                                        */      
    NVIC_EnableIRQ(30);              /* Enable Interrupt in NVIC block */
    NVIC_SetPriority(30, priority);
    TIM4->DIER |= TIM_DIER_UIE;             /* Enable Timer Update Event Interrupt */
  TIM4->CR1 |= TIM_CR1_CEN;                 /* Enable Timer */
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;
    PeriodicTask[0]();    
}

void TIM3_IRQHandler()
{
    TIM3->SR &= ~TIM_SR_UIF;
    PeriodicTask[1]();   
}

void TIM4_IRQHandler()
{
    TIM4->SR &= ~TIM_SR_UIF;
    PeriodicTask[2]();   
}

/**
 * @brief   Add realtime Periodic thread in the OS.
 * @param   void (*periodicTask): Function pointer to thread.
 *                  int period: Period of thread in ms.
 *                  int Priority: Priority between 0 and 255.
 *          int thread_num: ID of thread. 
 * @return  void
 */
void BSP_PeriodicTask_Init(void (*periodicTask)(), int period, 
                           int priority, int thread_num)
{
    switch(thread_num) {
        case 0:
            BSP_PeriodicTask_Init1(periodicTask, period, priority);
            break;
        
        case 1:
            BSP_PeriodicTask_Init2(periodicTask, period, priority);
            break;
        
        case 2:
            BSP_PeriodicTask_Init3(periodicTask, period, priority);
            break;
    }
    
    
}
