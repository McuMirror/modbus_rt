﻿
#include "modbus_rt_platform_thread.h"
#include "modbus_rt_platform_memory.h"

#define UNUSED(x) (void)(x)

modbus_rt_thread_t *modbus_rt_thread_init( const char *name,
                                           void (*entry)(void *),
                                           void * param,
                                           unsigned int stack_size,
                                           unsigned int priority,
                                           unsigned int tick) {
    BaseType_t err;
    modbus_rt_thread_t *thread;
    
    thread = modbus_rt_calloc(sizeof(modbus_rt_thread_t), 1);

    (void)tick;

    err =  xTaskCreate(entry, name, stack_size, param, priority, &thread->thread);

    if(pdPASS != err) {
        modbus_rt_free(thread);
        return NULL;
    }

    return thread;
}

void modbus_rt_thread_startup(modbus_rt_thread_t* thread) {
    (void)thread;
}
void modbus_rt_thread_stop(modbus_rt_thread_t* thread) {
    vTaskSuspend(thread->thread);
}
void modbus_rt_thread_start(modbus_rt_thread_t* thread) {
    vTaskResume(thread->thread);
}
void modbus_rt_thread_destroy(modbus_rt_thread_t* thread) {
    modbus_rt_free(thread);
}

void modbus_rt_thread_exit(modbus_rt_thread_t* thread) {
    if (NULL == thread) {
        vTaskDelete(NULL);
        return;
    }
    vTaskDelete(thread->thread);
    return;
}

void modbus_rt_thread_sleep(unsigned int ms) {
    int delay_ticks = ms/portTICK_PERIOD_MS;
    if(delay_ticks <= 0) {
        delay_ticks = 1;
    }
    vTaskDelay(delay_ticks);
}

int modbus_rt_mutex_init(modbus_rt_mutex_t* m) {
    m->mutex = xSemaphoreCreateMutex();
    return 0;
}
int modbus_rt_mutex_lock(modbus_rt_mutex_t* m) {
    return xSemaphoreTake(m->mutex, portMAX_DELAY);
}
int modbus_rt_mutex_trylock(modbus_rt_mutex_t* m) {
    return xSemaphoreTake(m->mutex, 0);
}
int modbus_rt_mutex_unlock(modbus_rt_mutex_t* m) {
    return xSemaphoreGive(m->mutex);
}
int modbus_rt_mutex_destroy(modbus_rt_mutex_t* m) {
    vSemaphoreDelete(m->mutex);
    return 0;
}

int modbus_rt_sem_init(modbus_rt_sem_t* m) {
    m->sem = xSemaphoreCreateCounting(10, 0);
    return 0;
}
int modbus_rt_sem_wait(modbus_rt_sem_t* m) {
    return xSemaphoreTake(m->sem, portMAX_DELAY);
}
int modbus_rt_sem_wait_time(modbus_rt_sem_t* m, int32_t time) {
    return xSemaphoreTake(m->sem, time);
}
int modbus_rt_sem_post(modbus_rt_sem_t* m) {
    return xSemaphoreGive(m->sem);
}
int modbus_rt_sem_post_from_isr(modbus_rt_sem_t* m) {
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    return xSemaphoreGiveFromISR(m->sem, &xHigherPriorityTaskWoken);
}
int modbus_rt_sem_reset(modbus_rt_sem_t* m) {
    while(uxSemaphoreGetCount(m->sem) > 0) {
        xSemaphoreTake(m->sem, 0);
    }
    return 0;
}
int modbus_rt_sem_destroy(modbus_rt_sem_t* m) {
    vSemaphoreDelete(m->sem);
    return 0;
}


