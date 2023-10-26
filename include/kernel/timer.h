#ifndef _KERNEL_TIMER_H_
#define _KERNEL_TIMER_H_

#define INT_TICKS_PER_SECOND        100

#define STR_TIMER_INSTALLED         "Installed timer"
#define STR_TICK                    "Tick"
#define STR_TOCK                    "Tock"
#define STR_ERROR_TIMER_INSTALL     "Error installing timer!"

void timer_install();

#endif