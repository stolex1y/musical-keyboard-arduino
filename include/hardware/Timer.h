#ifndef MUSICAL_KEYBOARD_TIMER_H
#define MUSICAL_KEYBOARD_TIMER_H

#include <stdint.h>

typedef void (*TimerCallback)();

enum TimerN {
    TIM1 = 0,
    TIM2,
    TIM3
};

void timerStop(TimerN timer);

void timerStart(TimerN timer);

void timerSetPeriod(TimerN timer, uint32_t periodMs);

void timerSetCallback(TimerN timer, TimerCallback callback);

void timerInvokeCallback(TimerN timer);

void timerTick();


static struct {
    void (*stop)(TimerN timer);

    void (*start)(TimerN timer);

    void (*setPeriod)(TimerN timer, uint32_t periodMs);

    void (*setCallback)(TimerN timer, TimerCallback callback);

    void (*invokeCallback)(TimerN timer);

    void (*tick)();
} const timer = {
        timerStop,
        timerStart,
        timerSetPeriod,
        timerSetCallback,
        timerInvokeCallback,
        timerTick,
};


#endif //MUSICAL_KEYBOARD_TIMER_H
