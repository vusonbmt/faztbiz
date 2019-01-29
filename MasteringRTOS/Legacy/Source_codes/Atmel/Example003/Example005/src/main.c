/*
    FreeRTOS V6.0.5 - Copyright (C) 2009 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

#include <asf.h>
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* The two task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* Used to hold the handle of Task2. */
xTaskHandle xTask2Handle;

/*-----------------------------------------------------------*/
/**
 *  Configure UART console.
 */
// [main_console_configure]
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

int main( void )
{
	/* This function initializes the MCU clock  */
	sysclk_init();
	/* Board initialization */
	board_init();
	
	/* Initialize the serial I/O(console ) */
	configure_console();

#if (configUSE_TRACE_FACILITY == 1)
	/*Allocates, if necessary, and initializes the recorder data structure */
	vTraceInitTraceData();

	/*Starts the recorder*/
	uiTraceStart();
#endif 
	

	/* Create the first task at priority 1.  This time the task parameter is
	not used and is set to NULL.  The task handle is also not used so likewise 
	is also set to NULL. */
	xTaskCreate( vTask1, "Task 1", 240, NULL, 1, NULL );
         /* The task is created at priority 1 ^. */

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();	
	
	for( ;; );
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
const portTickType xDelay100ms = 100 / portTICK_RATE_MS;

	for( ;; )
	{
		/* Print out the name of this task. */
		printf( "Task1 is running\n" );

		/* Create task 2 at a higher priority.  Again the task parameter is not 
		used so is set to NULL - BUT this time we want to obtain a handle to the 
		task so pass in the address of the xTask2Handle variable. */
		xTaskCreate( vTask2, "Task 2", 240, NULL, 2, &xTask2Handle );
			/* The task handle is the last parameter ^^^^^^^^^^^^^ */

		/* Task2 has/had the higher priority, so for Task1 to reach here Task2
		must have already executed and deleted itself.  Delay for 100 
		milliseconds. */
		vTaskDelay( xDelay100ms );
	}
}

/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
	/* Task2 does nothing but delete itself.  To do this it could call vTaskDelete()
	using a NULL parameter, but instead and purely for demonstration purposes it
	instead calls vTaskDelete() with its own task handle. */
	printf( "Task2 is running and about to delete itself\n" );
	vTaskDelete( xTask2Handle );
	//vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}


