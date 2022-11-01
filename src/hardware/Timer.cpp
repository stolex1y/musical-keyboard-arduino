#include "hardware/Timer.h"

#include <stdint.h>

#include <../.pio/libdeps/nanoatmega328/GyverTimers/src/GyverTimers.h>
#include <../.pio/libdeps/nanoatmega328/TimerMs/src/TimerMs.h>

static void stub() {}

static TimerMs Timer3;

typedef struct Timer {
    TimerCallback callback;
} Timer;

static Timer timers[] = {
        [TIM1] = { stub },
        [TIM2] = { stub },
        [TIM3] = { stub }
};

void timerStart(TimerN timer) {
    switch (timer) {
        case TIM1:
            Timer1.enableISR(CHANNEL_A);
            Timer1.restart();
            break;
        case TIM2:
            Timer2.enableISR(CHANNEL_A);
            Timer2.restart();
            break;
        case TIM3:
            Timer3.start();
            Timer3.attach(timers[TIM3].callback);
            break;
    }
}

void timerStop(TimerN timer) {
    switch (timer) {
        case TIM1:
            Timer1.disableISR();
            Timer1.stop();
            break;
        case TIM2:
            Timer2.disableISR();
            Timer2.stop();
            break;
        case TIM3:
            Timer3.stop();
            break;
    }
}

void timerSetPeriod(TimerN timer, uint32_t periodMs) {
    switch (timer) {
        case TIM1:
            Timer1.setPeriod(periodMs * 1000);
            break;
        case TIM2:
            Timer2.setPeriod(periodMs * 1000);
            break;
        case TIM3:
            Timer3.setTime(periodMs);
            break;
    }
}

void timerSetCallback(TimerN timer, TimerCallback callback) {
    timers[timer].callback = callback;
}

void timerInvokeCallback(TimerN timer) {
    timers[timer].callback();
}

void timerTick() {
    Timer3.tick();
}


ISR(TIMER1_A) {
    timerInvokeCallback(TIM1);
}

/*
ISR(TIMER2_A) {
    timerInvokeCallback(TIM2);
}
*/

/*
enum TimerN {
    TIM6 = 0,
    TIM7
};
 
 typedef struct Timer {
    uint16_t freq;
    uint16_t counter;
    TimerCallback callback;
    TIM_HandleTypeDef *htim;
} Timer;
 
 
static Timer timers[] = {
        [TIM6] = { 0 },
        [TIM7] = { 0 }
};
 
void timer6Stop() {
    HAL_TIM_Base_Stop_IT(timers[TIM6].htim);
}

void timer6Start() {
    HAL_TIM_Base_Start_IT(timers[TIM6].htim);
}

void timer6Init(TIM_HandleTypeDef *htim) {
    timers[TIM6].htim = htim;
    timers[TIM6].callback = stub;
}

void timer6SetPeriod(uint32_t periodUs) {
    TIM6->PSC = (HAL_RCC_GetPCLK1Freq() / periodUs) - 1;
    TIM6->CCR1 = 65535;
}

void timer6SetCallback(TimerCallback callback) {
    timers[TIM6].callback = callback;
}

void timer6Callback() {
    timers[TIM6].callback();
}*/



