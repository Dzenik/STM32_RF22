#ifndef __EXTI_H__
#define __EXTI_H__
#include <stdbool.h>

void extiInit();
bool extiTest();

inline void extiInterruptHandler(void);

#endif /* __EXTI_H__ */
