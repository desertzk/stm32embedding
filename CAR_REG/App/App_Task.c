#include "App_Task.h"

TaskHandle_t start_task_handle;
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
void App_Task_Start(void *pvParameters);

/* ��ȡ��̬���ݵ��������� */
TaskHandle_t data_task_handle;
#define DATA_TASK_STACK 128
#define DATA_TASK_PRIORITY 3
void App_Task_GetData(void *pvParameters);

/* ����PID���Ƶ��������� */
TaskHandle_t pid_task_handle;
#define PID_TASK_STACK 128
#define PID_TASK_PRIORITY 3
void App_Task_PID(void *pvParameters);

/* ����PID���Ƶ��������� */
TaskHandle_t display_task_handle;
#define DISPLAY_TASK_STACK 128
#define DISPLAY_TASK_PRIORITY 3
void App_Task_Display(void *pvParameters);

void App_Task_Init(void)
{
    /* 1. ������������ */
    xTaskCreate(
        (TaskFunction_t)App_Task_Start,
        (char *)"App_Task_Start",
        (configSTACK_DEPTH_TYPE)START_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t *)&start_task_handle);

    /* 2. ���������� */
    vTaskStartScheduler();
}

/**
 * @description: �����������ڴ�����������
 * @param {void *} pvParameters
 * @return {*}
 */
void App_Task_Start(void *pvParameters)
{
    taskENTER_CRITICAL();
    xTaskCreate(
        (TaskFunction_t)App_Task_GetData,
        (char *)"App_Task_GetData",
        (configSTACK_DEPTH_TYPE)DATA_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)DATA_TASK_PRIORITY,
        (TaskHandle_t *)&data_task_handle);
    xTaskCreate(
        (TaskFunction_t)App_Task_PID,
        (char *)"App_Task_PID",
        (configSTACK_DEPTH_TYPE)PID_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)PID_TASK_PRIORITY,
        (TaskHandle_t *)&pid_task_handle);
    xTaskCreate(
        (TaskFunction_t)App_Task_Display,
        (char *)"App_Task_Display",
        (configSTACK_DEPTH_TYPE)DISPLAY_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)DISPLAY_TASK_PRIORITY,
        (TaskHandle_t *)&display_task_handle);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

/**
 * @description: ��ȡ��̬���ݣ��Ƕȡ�������ֵ
 * @param {void} *pvParameters
 * @return {*}
 */
void App_Task_GetData(void *pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();

    while(1)
    {
        App_Car_GetAngle();
        /* ��ȡ�����ݣ�֪ͨpid�������� */
        xTaskNotifyGive(pid_task_handle);
        vTaskDelayUntil(&pxPreviousWakeTime,10);
    }

}

/**
 * @description: ����PID���Ƶ�����
 * @param {void} *pvParameters
 * @return {*}
 */
void App_Task_PID(void *pvParameters)
{
    while(1)
    {
        /* �ȴ�����ȡ�������񡱵�֪ͨ */
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        App_Car_PID();
    }
}

/**
 * @description: OLED��ʾ����
 * @param {void} *pvParameters
 * @return {*}
 */
void App_Task_Display(void *pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    while(1)
    {
        App_Car_Display();
        vTaskDelayUntil(&pxPreviousWakeTime,50);
    }

}
