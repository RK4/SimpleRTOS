/**
 * @author  Mohsin Shan
 * @version 0.1 , 18/12/16
 */
#include <os.h>
#include <config.h>
#include <stdint.h>

/* List of tcb's */
tcbType tcbs[NUMTHREADS];

/* Tcb for current thread */  
tcbType *RunPt;                     

/* Free slot for next thread */
uint32_t thread_free_slot = 0;      

/* Number of ticks since initialization in ms */ 
uint64_t ticks;  

/* List of Stacks for each thread */
int32_t Stacks[NUMTHREADS][STACKSIZE];

/**
 * @brief   Initialize the data structures in OS.
 * @param   voidtcbType tcbs[NUMTHREADS];
 * @return  void
 */
void OS_Init(void)
{
    /* Disable Interrupts */
    __disable_irq();        
    
    /* Initialize ticks */
    ticks = 0;              

    /* Initialize the data structures */
    for(int i=0; i < NUMTHREADS; i++) { 
        /* Link all the tcbs in a circular loop */

        tcbs[i].next = &tcbs[(i + 1) % NUMTHREADS];     
         /* No thread is blocked */
        tcbs[i].blocked = 0;       
        /* Set sleeping duration to 0 */    
        tcbs[i].sleeping = 0;           
        /* Set the state to be invalid */
        tcbs[i].valid = 0;              
    }
    
    /* Runpt Holds the current thread. Assign thread 0*/
    RunPt = &tcbs[0];                 
    
    /* Enable Interrupts */ 
    __enable_irq();                   
    
}

/**
 * @brief   Start the OS.
 * @param   theTimeSlice: Time Slice for each thread.
 * @return  void
 */
void OS_Launch(uint32_t theTimeSlice)
{
    /* Start Systick. Interrupt will occur after every theTimeSlice ms. */
    SysTick_Config(theTimeSlice / 1000); 
    
    /* Start the OS */
    StartOS();                              
}

/**
 * @brief   Determines the next thread to run. Uses priority based scheduling.
 * @param   void
 * @return  void
 */
 void Scheduler(void)
{
    /* Increment the ticks */
    ticks++;            
    
    /* Local variables to select best candidate to run next */
    uint32_t max = 255;
     /* Structure to loop through the list */                                             
    tcbType *pt;       
    /* Best candidate */
    tcbType *bestpt;    
    
    int i;
    /* For every sleeping thread decrement the sleep time */
    for(i = 0; i < NUMTHREADS; i++) 
    {
        if (tcbs[i].sleeping)
                tcbs[i].sleeping--;
    }
    
    
     /* Loop through the entire list. Select the candidate with highest priority. */
    pt = RunPt;
    do {    
        pt = pt->next;
        
        if ((pt->valid) && (pt->priority < max) && (!pt->blocked) && (!pt->sleeping))
        {
            max = pt->priority;
            bestpt = pt;
        }
        } while(RunPt != pt);

    /* Set the RunPt to the best Candidate */ 
    RunPt = bestpt;         
}


 void OS_AddThread(void(*thread)(void), uint32_t priority)
 {
     __disable_irq();                /* Disable Interrupts */

     SetInitialStack(thread_free_slot);         /* Initialize Stack for the thread */
     Stacks[thread_free_slot][STACKSIZE-2]  = (uint32_t)thread;  /* Store function pointer value in PC */
     tcbs[thread_free_slot].priority = priority;     /* Set the priority */
     tcbs[thread_free_slot].valid = 1;               /* Set the valid bit */
     //tcbs[thread_free_slot].name= 'mohsin';
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
  * @brief   Wait for the given number of ticks. The thread will be blocked.
  * @param   uint32_t sleepTime: Ticks to sleep.
  * @return  void
  */
 void OS_Sleep(uint32_t sleepTime)
 {
     /* Set the Sleep time for the thread */
     RunPt->sleeping = sleepTime;
     /* Suspend the thread */
     OS_Suspend();
 }

 /**
  * @brief   Give up the cpu to the next thread.
  * @param   void
  * @return  void
  */
 void OS_Suspend(void)
 {
     /* Set the systick counter to 0 */
     SysTick->VAL   = 0UL;
     /* Set the Systick pendig bit to force Systick exception */
     SCB->ICSR |= (1 << 26);
 }

 /**
  * @brief   Initialize the semaphore to the given value.
  * @param   uint32_t *semapt: Semaphore.
  *          int32_t value:      Initial Value.
  * @return  void
  */
 void OS_InitSemaphore(int32_t *semaPt, int32_t value)
 {
      /* Initialize the semaphore */
     *semaPt = value;
 }

 /**
  * @brief   Wait of the semaphore. The thread will be blocked.
  * @param   void
  * @return  void
  */
 void OS_Wait(int32_t *semaPt){

     /* Disable Interrupts */
     __disable_irq();
     /* Decrement Semaphore counter */
     (*semaPt)--;

     /* If semapt less than 0, block the thread and Suspend */
     if (*semaPt < 0) {
         RunPt->blocked = semaPt;
         __enable_irq();
         OS_Suspend();
     }

     /* Enable Interrupts */
     __enable_irq();
 }

 /**
  * @brief   Release the semaphore. If some thread is waiting on it. Wake it up.
  * @param   void
  * @return  void
  */
 void OS_Signal(int32_t *semaPt){

     /* Local Varaible to loop throught the thread list */
     tcbType *pt;
     /* Disable Interrupts */
     __disable_irq();

     /* Increment the Semaphore */
     (*semaPt)++;


     /* If Some thread was waiting on this semaphore Wake it up. */
     if (*semaPt <= 0) {

         pt = RunPt->next;
         while(pt->blocked != semaPt)
             pt = pt->next;

         pt->blocked = 0;
     }
     /* Enable Interrupts */
     __enable_irq();
 }
