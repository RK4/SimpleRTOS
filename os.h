/**
 * @author  Mohsin Shan
 * @version 0.1 , 18/12/16
 */
 
#include <stdint.h>

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
void OS_AddThread(void(*thread0)(void), uint32_t priority);

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
 * @brief   Add real time threads in OS.
 * @param   thread:     function pointer to function.
 *                  priority:   priority of thread. Between 0-255.
 * @return  void
 */
void OS_PeriodTrigger_Init(int32_t *semaPt, uint32_t period);

/**
 * @brief   This function handles realtime threads in OS.
 * @param   void
 * @return  void
 */
void RealTimeEvents(void);