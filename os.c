/**
 * @author  Mohsin Shan
 * @version 0.1 , 18/12/16
 */
 
#include "os.h"
#include <stdint.h>
#include "stm32f407xx.h"
#include "BSP.h"

#define NUMTHREADS  8                   /* Maximum number of threads */
#define NUMPERIODIC 3                   /* Maximum number of periodic threads */
#define STACKSIZE   100                 /* Stack Size for each thread */


int32_t *PeriodicSemaphore[NUMPERIODIC];    /* Periodic Semaphores for Periodic threads */
uint32_t Period[NUMPERIODIC];               /* List of Periods for Real time 
                                               Periodic threads 
                                            */


extern void StartOS();              /* Start the OS. Defined is assembley language */

uint32_t thread_free_slot = 0;      /* Free slot for next thread */
uint32_t pthread_free_slot = 0 ;    /* Free Slot for next periodic thread */
uint64_t ticks;                     /* Number of ticks since initialization in ms */

struct tcb{
  int32_t *sp;                      /* Pointer to current value in stack */ 
  struct tcb *next;                 /* Next thread in the list */
    int32_t *blocked;               /* nonzero if blocked on this semaphore */
    int32_t sleeping;               /* nonzero if this thread is sleeping */
    char *name;                     /* Name of thread */    
    uint8_t priority;               /* Priority of thread */
    uint8_t valid;                  /* To check whether the thread slot is in use or not */
};
typedef struct tcb tcbType;

tcbType tcbs[NUMTHREADS];           /* List of thread control blocks for threads */     
tcbType *RunPt;                     /* Tcb for current thread */
int32_t Stacks[NUMTHREADS][STACKSIZE];  /* List of Stacks for each thread */

/**
 * @brief   Initialize the data structures in OS.
 * @param   void
 * @return  void
 */
void OS_Init(void)
{
    __disable_irq();        /* Disable Interrupts */
    
    ticks = 0;              /* Initialize ticks */
    
    for(int i=0; i < NUMTHREADS; i++) {     /* Initialize the data structures */
        tcbs[i].next = &tcbs[(i + 1) % NUMTHREADS];     /* Link all the tcbs */ 
        tcbs[i].blocked = 0;            /* No thread is blocked */
        tcbs[i].sleeping = 0;           /* Set sleeping duration to 0 */
        tcbs[i].valid = 0;              /* Set the state to be invalid */
    }
    RunPt = &tcbs[0];                   /* Runpt Holds the current thread. Assign thread 0*/
    __enable_irq();                     /* Enable Interrupts */
    
}

/**
 * @brief   Add thread in OS.
 * @param   thread:     function pointer to function.
 *                  priority:   priority of thread. Between 0-255.
 * @return  void
 */
void OS_AddThread(void(*thread)(void), uint32_t priority)
{
    __disable_irq();                /* Disable Interrupts */
    
    SetInitialStack(thread_free_slot);         /* Initialize Stack for the thread */
    Stacks[thread_free_slot][STACKSIZE-2]  = (uint32_t)thread;  /* Store function pointer value in PC */ 
    tcbs[thread_free_slot].priority = priority;     /* Set the priority */
    tcbs[thread_free_slot].valid = 1;               /* Set the valid bit */
    thread_free_slot++;                             /* Increment the free slot counter */   
    __enable_irq();                                 /* Enable Interrupts */     
}

/**
 * @brief   Initialize Stack for the thread.
 * @param   void
 * @return  void
 */
void SetInitialStack(int i)
{
    /* Set the stack in tcbs strucutre to correct address. */
  tcbs[i].sp = &Stacks[i][STACKSIZE-16]; 
  
  /* Initialize the stack */
  Stacks[i][STACKSIZE-1] = 0x01000000;
  Stacks[i][STACKSIZE-3] = 0x01000000;
  Stacks[i][STACKSIZE-4] = 0x01000000;
  Stacks[i][STACKSIZE-5] = 0x01000000;
  Stacks[i][STACKSIZE-6] = 0x01000000;
  Stacks[i][STACKSIZE-8] = 0x01000000;
  Stacks[i][STACKSIZE-9] = 0x01000000;
  Stacks[i][STACKSIZE-10] = 0x01000000;
  Stacks[i][STACKSIZE-11] = 0x01000000;
  Stacks[i][STACKSIZE-12] = 0x01000000;
  Stacks[i][STACKSIZE-13] = 0x01000000;
  Stacks[i][STACKSIZE-14] = 0x01000000;
  Stacks[i][STACKSIZE-15] = 0x01000000;
  Stacks[i][STACKSIZE-16] = 0x01000000;
}

/**
 * @brief   Start the OS.
 * @param   theTimeSlice: Time Slice for each thread.
 * @return  void
 */
void OS_Launch(uint32_t theTimeSlice)
{
    SysTick_Config(theTimeSlice / 1000);    /* Start Systick. Interrupt wil   
                                             *  occuer after every theTimeSlice 
                                             * ms
                                             */
    StartOS();                              /* Start the OS */
}

/**
 * @brief   Determines the next thread to run. Uses priority based scheduling.
 * @param   void
 * @return  void
 */
void Scheduler(void)
{
    ticks++;            /* Increment the ticks */
    
    /* Local variables to select best candidate to run next */
    uint32_t max = 255;                                             
    tcbType *pt;        /* Structure to loop through the list */
    tcbType *bestpt;    /* Best candidate */
    
    int i;
    for(i = 0; i < NUMTHREADS; i++) /* For every sleeping thread decrement the sleep time */
    {
        if (tcbs[i].sleeping)
                tcbs[i].sleeping--;
    }
    
    /*
     * Loop through the entire list. Select the candidate with highest priority.
     */
    pt = RunPt;
    do {    
        pt = pt->next;
        
        if ((pt->valid) && (pt->priority < max) && (!pt->blocked) && (!pt->sleeping))
        {
            max = pt->priority;
            bestpt = pt;
        }
        } while(RunPt != pt);
    RunPt = bestpt;         /* Set the RunPt to the best Candidate */ 
}

/**
 * @brief   Wait for the given number of ticks. The thread will be blocked.
 * @param   uint32_t sleepTime: Ticks to sleep.
 * @return  void
 */
void OS_Sleep(uint32_t sleepTime)
{
    RunPt->sleeping = sleepTime;    /* Set the Sleep time for the thread */
    OS_Suspend();                   /* Suspend the thread */
}

/**
 * @brief   Give up the cpu to the next thread.
 * @param   void
 * @return  void
 */
void OS_Suspend(void)
{
    SysTick->VAL   = 0UL;   /* Set the systick counter to 0 */
  SCB->ICSR |= (1 << 26);   /* Set the Systick pendig bit to force 
                             * Systick exception
                             */
}

/**
 * @brief   Initialize the semaphore to the given value.
 * @param   uint32_t *semapt: Semaphore.
 *                  int32_t value:      Initial Value.
 * @return  void
 */
void OS_InitSemaphore(int32_t *semaPt, int32_t value)
{
    *semaPt = value;        /* Initialize the semaphore */
}


/**
 * @brief   Wait of the semaphore. The thread will be blocked.
 * @param   void
 * @return  void
 */
void OS_Wait(int32_t *semaPt){
  
    __disable_irq();        /* Disable Interrupts */
    (*semaPt)--;            /* Decrement Semaphore counter */   
    
    /* If semapt less than 0, block the thread and Suspend */
    if (*semaPt < 0) {
        RunPt->blocked = semaPt;
        __enable_irq();
        OS_Suspend();
    }
    
    __enable_irq();         /* Enable Interrupts */
}

/**
 * @brief   Release the semaphore. If some thread is waiting on it. Wake it up.
 * @param   void
 * @return  void
 */
void OS_Signal(int32_t *semaPt){

    tcbType *pt;            /* Local Varaible to loop throught the thread list */
    __disable_irq();        /* Disable Interrupts */
        
    (*semaPt)++;            /* Increment the Semaphore */
    
    /*
     * If Some thread was waiting on this semaphore Wake it up.
     */
    if (*semaPt <= 0) {
        
        pt = RunPt->next;
        while(pt->blocked != semaPt)
            pt = pt->next;
        
        pt->blocked = 0;
    }
    __enable_irq();         /* Enable Interrupts */
}

/**
 * @brief   Add real time threads in OS.
 * @param   thread:     function pointer to function.
 *                  priority:   priority of thread. Between 0-255.
 * @return  void
 */
void OS_PeriodTrigger_Init(int32_t *semaPt, uint32_t period)
{
    PeriodicSemaphore[pthread_free_slot] = semaPt;  /* The semaphore on which the thread will wait */
    Period[pthread_free_slot] = period;             /* Set the Period */
    BSP_PeriodicTask_Init(RealTimeEvents,           /* Initialize the Task */
                        Period[pthread_free_slot], 0,  pthread_free_slot);
    pthread_free_slot++;                            /* Increment the periodic thread counter */
}


/**
 * @brief   This function handles realtime threads in OS.
 * @param   void
 * @return  void
 */
void RealTimeEvents(void)
{
    static int32_t realCount = -10; /* Sort of delay */
    int flag = 0;                       
    realCount++;
  
    /*
     * If there is some thread which needs to run now, signal it.
     */
    if (realCount >= 0) {
        for(int i = 0; i < NUMPERIODIC; i++)
        {
            if ((realCount % Period[i]) == 0) {
                
                /*
                 * The thread will be waiting on the semaphore. Release it.
                 */
                OS_Signal(PeriodicSemaphore[i]);                
                flag = 1;
            }
    
        }
    
        /* 
         * If there is some real time thread to run yield the CPU.
         */
        if(flag){
            OS_Suspend();                                                           
        }   
  }
    
}
    