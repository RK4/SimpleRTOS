#ifndef __BSP_H
#define __BSP_H

/**
 * @author  Mohsin Shan
 * @version 0.1 , 18/12/16
 */
 
/**
 * @brief   Add realtime Periodic thread in the OS.
 * @param   void (*periodicTask): Function pointer to thread.
 *                  int period: Period of thread in ms.
 *                  int Priority: Priority between 0 and 255.
 *          int thread_num: ID of thread. 
 * @return  void
 */
void BSP_PeriodicTask_Init(void (*periodicTask)(), int period, 
                           int priority, int thread_num);

#endif
