#ifndef __PERIODICTHREAD_H
#define __PERIODICTHREAD_H

#include <stdint.h>
#include "config.h"
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

#endif
