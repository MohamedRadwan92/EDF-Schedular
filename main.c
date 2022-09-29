/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "semphr.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

/* Handles to be Defined for Usage of the system */

TaskHandle_t Button_1Toggle = NULL;

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */

static void prvSetupHardware( void );
/*---------------- Global Variables -------------------------*/

pinState_t Button_1 = PIN_IS_LOW;
pinState_t Button_2 = PIN_IS_LOW;

TaskHandle_t ToggleButton1 = NULL;
TaskHandle_t ToggleButton2 = NULL;
TaskHandle_t sendData_UART = NULL;
TaskHandle_t recieveData_UART = NULL;
TaskHandle_t Load1_Simulation = NULL;
TaskHandle_t Load2_Simulation = NULL;

void ToggleButton_1( void * pvParameters )
{
	int countHigh1 = 0, countLow1 = 0;
    for( ;; )
    {
				Button_1 = GPIO_read(PORT_1,PIN0);
				if(Button_1 == PIN_IS_HIGH){
					countHigh1++;
				}else if(Button_1 == PIN_IS_LOW){
					countLow1++;					
				}
				
			if(countHigh1 >= 3){
				Button_1 = PIN_IS_HIGH;
				countLow1 = 0;
			}else if (countLow1 >= 3){
				Button_1 = PIN_IS_LOW;
				countHigh1 = 0;
			}
			
				GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
				vTaskDelay(50);
		  	GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);
    }
}

void ToggleButton_2( void * pvParameters )
{
	int countHigh2 = 0, countLow2 = 0;
    for( ;; )
    {
				Button_2 = GPIO_read(PORT_1,PIN1);
				if(Button_2 == PIN_IS_HIGH){
					countHigh2++;
				}else if(Button_1 == PIN_IS_LOW){
					countLow2++;
				}
				
			if(countHigh2 >= 3){
				Button_1 = PIN_IS_HIGH;
				countLow2 = 0;
			}else if (countLow2 >= 3){
				Button_1 = PIN_IS_LOW;
				countHigh2 = 0;
			}
			
				GPIO_write(PORT_0,PIN2,PIN_IS_LOW);
				vTaskDelay(50);
	  		GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
    }
}

void sendDataUART( void * pvParameters )
{
	portBASE_TYPE SendCondition1,SendCondition2;
    for( ;; )
    {
			if( Button_1 == PIN_IS_HIGH){
				SendCondition1 = vSerialPutString("Button1ON", 10);
			}else{
				SendCondition1 = vSerialPutString("Button1OFF", 11);
			}
			
			if( Button_2 == PIN_IS_HIGH){
				SendCondition2 = vSerialPutString("Button2ON", 10);
			}else{
				SendCondition2 = vSerialPutString("Button2OFF", 11);
			}
			
			
				GPIO_write(PORT_0,PIN3,PIN_IS_LOW);
				vTaskDelay(100);
			 GPIO_write(PORT_0,PIN3,PIN_IS_HIGH);
    }
}

void recieveDataUART( void * pvParameters )
{ int i;
    for( ;; )
    {
			for(i=0;i<80;i++);
			
				GPIO_write(PORT_0,PIN4,PIN_IS_LOW);
			 	vTaskDelay(20);
			  GPIO_write(PORT_0,PIN4,PIN_IS_HIGH);
    }
}

void Load_1_Simulation( void * pvParameters )
{ int i;
    for( ;; )
    {
			for(i=0 ; i<35000; i++);
			
				GPIO_write(PORT_0,PIN5,PIN_IS_LOW);
			 	vTaskDelay(10);
			  GPIO_write(PORT_0,PIN5,PIN_IS_HIGH);
    }
}

void Load_2_Simulation( void * pvParameters )
{ int i;
    for( ;; )
    {
			for(i=0 ; i<87000; i++){
				
			}
			
				GPIO_write(PORT_0,PIN6,PIN_IS_LOW);
			 	vTaskDelay(100);
			  GPIO_write(PORT_0,PIN6,PIN_IS_HIGH);
    }
}


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	
	BaseType_t TaskCheck;
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	
    /* Create Tasks here */
	
			xTaskCreate(
                    ToggleButton_1,       /* Function that implements the task. */
                    "ToggleButton1",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &ToggleButton1 );      /* Used to pass out the created task's handle. */
										
	xTaskCreate(
                    ToggleButton_2,       /* Function that implements the task. */
                    "ToggleButton2",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &ToggleButton2 );      /* Used to pass out the created task's handle. */
										
	xTaskCreate(
                    sendDataUART,       /* Function that implements the task. */
                    "sendDataUART",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &sendData_UART );      /* Used to pass out the created task's handle. */
										
	xTaskCreate(
                    recieveDataUART,       /* Function that implements the task. */
                    "sendDataUART",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &recieveData_UART );      /* Used to pass out the created task's handle. */
						
	xTaskCreate(
                    Load_1_Simulation,       /* Function that implements the task. */
                    "Load_1_Simulation",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Load1_Simulation );      /* Used to pass out the created task's handle. */
	
										
	xTaskCreate(
                    Load_2_Simulation,       /* Function that implements the task. */
                    "Load_2_Simulation",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Load2_Simulation );      /* Used to pass out the created task's handle. */
						


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


