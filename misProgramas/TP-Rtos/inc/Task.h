/*
 * Task.h
 *
 *  Created on: 10 jul. 2020
 *      Author: hanes
 */

#ifndef PROGRAMS_RTOS2_INC_TASK_H_
#define PROGRAMS_RTOS2_INC_TASK_H_

/*=====[Prototypes (declarations) of public functions]=======================*/

/**
 * @brief Function to create task for transmit data for UART
 *
 * @param[in] Parameters void pointer to pass of task.
 *
 * \return If operation was success return 1 in other case 0.
 */
uint8_t txUART_Start(void *parameters);

/**
 * @brief Function to create task for receive data for UART
 *
 * @param[in] Parameters void pointer to pass of task.
 *
 * \return If operation was success return 1 in other case 0.
 */
uint8_t rxUART_Start(void *parameters);

/**
 * @brief Function to create task for a blinky led
 *
 * @param[in] Parameters void pointer to pass of task.
 *
 * \return If operation was success return 1 in other case 0.
 */
uint8_t Alive_Task(void *parameters);

#endif /* PROGRAMS_RTOS2_INC_TASK_H_ */
