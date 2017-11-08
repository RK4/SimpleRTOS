#include <os.h>
#include <periodic_thread.h>
#include <bsp.h>

/* Free Slot for next periodic thread */
uint32_t pthread_free_slot = 0 ;

/* Periodic Semaphores for Periodic threads */
int32_t *PeriodicSemaphore[NUMPERIODIC];

/* List of Periods for Real time Periodic threads */
uint32_t Period[NUMPERIODIC];

/**
 * @brief   Add real time threads in OS.
 * @param   period: Period of thread.
 * @return  void
 */
void OS_PeriodTrigger_Init(int32_t *semaPt, uint32_t period)
{
    /* The semaphore on which the thread will wait */
    PeriodicSemaphore[pthread_free_slot] = semaPt; 
     /* Set the Period */ 
    Period[pthread_free_slot] = period;            
    /* Initialize the Task */
    BSP_PeriodicTask_Init(RealTimeEvents, Period[pthread_free_slot], 0,  pthread_free_slot);
    /* Increment the periodic thread counter */
    pthread_free_slot++;                            
}

/**
 * @brief   This function handles realtime threads in OS.
 * @param   void
 * @return  void
 */
void RealTimeEvents(void)
{
	/* Sort of delay */
    static int32_t realCount = -10;
    int flag = 0;                       
    realCount++;
  
    /* If there is some thread which needs to run now, signal it. */  
    if (realCount >= 0) {
        for(int i = 0; i < NUMPERIODIC; i++)
        {
            if ((realCount % Period[i]) == 0) {
                
                /* The thread will be waiting on the semaphore. Release it. */
                OS_Signal(PeriodicSemaphore[i]);                
                flag = 1;
            }
        }
    
        /* If there is some real time thread to run yield the CPU. */
        if(flag){
            OS_Suspend();                                                           
        }   
  }
    
}
