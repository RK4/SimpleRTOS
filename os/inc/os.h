#ifndef __OS_H
#define __OS_H

/**
 * @author  Mohsin Shan
 * @version 0.1 , 18/12/16
 */
 
#include <stdint.h>
#include <core_cm4.h>
#include "config.h"



struct tcb{
    /* Pointer to current value in stack */
    int32_t *sp;
     /* Next thread in the list */
    struct tcb *next;
    /* nonzero if blocked on this semaphore */
    int32_t *blocked;
    /* nonzero if this thread is sleeping */
    int32_t sleeping;
    /* Name of thread */
    //char name[10];
    /* Priority of thread */
    uint8_t priority;
    /* To check whether the thread slot is in use or not */
    uint8_t valid;
    };
typedef struct tcb tcbType;

/**
 * @brief   Initialize the data structures in OS.
 * @param   void
 * @return  void
 */
void OS_Init(void);

/**
 * @brief   Add thread in OS.
 * @param   thread:     function pointer to function.
 *                  priority:   priority of thread. Between 0-255.
 * @return  void
 */
void OS_AddThread(void(*thread)(void), uint32_t priority);

/**
 * @brief   Start the OS.
 * @param   theTimeSlice: Time Slice for each thread.
 * @return  void
 */
void OS_Launch(uint32_t theTimeSlice);

/**
 * @brief   Give up the cpu to the next thread.
 * @param   void
 * @return  void
 */
void OS_Suspend(void);

/**
 * @brief   Wait for the given number of ticks. The thread will be blocked.
 * @param   uint32_t sleepTime: Ticks to sleep.
 * @return  void
 */
void OS_Sleep(uint32_t sleepTime);

/**
 * @brief   Initialize the semaphore to the given value.
 * @param   uint32_t *semapt: Semaphore.
 *                  int32_t value:      Initial Value.
 * @return  void
 */
void OS_InitSemaphore(int32_t *semaPt, int32_t value);

/**
 * @brief   Wait of the semaphore. The thread will be blocked.
 * @param   void
 * @return  void
 */
void OS_Wait(int32_t *semaPt);

/**
 * @brief   Release the semaphore. If some thread is waiting on it. Wake it up.
 * @param   void
 * @return  void
 */
void OS_Signal(int32_t *semaPt);

/**
 * @brief   Initialize Stack for the thread.
 * @param   void
 * @return  void
 */
void SetInitialStack(int i);

/**
 * @brief   Start OS.
 * @param   void
 * @return  void
 */
void StartOS(void);

#endif
