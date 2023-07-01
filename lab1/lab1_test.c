/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"



#define  TASK_STK_SIZE                2048    /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                         3       /* Number of identical tasks                          */
OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_CPU_SR cpu_sr;
/* Definition of Task Stacks */
/*
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];

*/

/* Definition of Task Priorities */
/*
#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
*/
/* Prints "Hello World" and sleeps for three seconds */
/*void task1(void* pdata)
{
  while (1)
  { 
    printf("Hello from task1\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}*/

void Print(void){



    while(pindex < tindex &&tindex > 100){
        if(buf[pindex][0]==0){
           // preempt
        	printf("		%d		Preempt		%d		%d \n",buf[pindex][3],buf[pindex][1],buf[pindex][2]);
        	if(buf[pindex][3]<=deadtask && buf[pindex+1][3]>=deadtask){
        		printf("		%d		task3 deadline \n",deadtask);
        		        		        			   deadtask = deadtask + 9;
        	}

        }else if(buf[pindex][0]==1){
        	printf("		%d		Complete		%d		%d\n",buf[pindex][3],buf[pindex][1],buf[pindex][2]);
        	if(buf[pindex][3]<=deadtask && buf[pindex+1][3]>=deadtask){
        	        		printf("		%d		task3 deadline \n",deadtask);
        	        		        		        			   deadtask = deadtask + 9;
        	        	}
        }else{

        		//printf("		%d		task3 deadline \n",deadtask);
        		        			   //deadtask = deadtask + 9;


        }
        pindex = pindex + 1;
    }
    /*if(tindex > 47){
        tindex = 0;
        pindex = 0;

    }*/
}
void  Task1 (void *pdata)
{



    INT8U start;
    INT8U end;
    INT8U toDelay;

    pdata = pdata;                                         /* Prevent compiler warning                 */

    start = OSTimeGet();

    OSTCBCur->compTime = 1;

    OSTCBCur->period = 3;

    while(1) {



        while(OSTCBCur->compTime>0){
            //donothing
            //waiting
        }
        end = OSTimeGet();

        OS_ENTER_CRITICAL();
        Print();
        toDelay=(OSTCBCur->period)-(end-start);
        start = start + (OSTCBCur->period);
        OSTCBCur->compTime = 1;
        OSTimeDly(toDelay);                            /* Delay remaining time                     */
        OS_EXIT_CRITICAL();
    }
}
/* Prints "Hello World" and sleeps for three seconds */
/*void task2(void* pdata)
{
  while (1)
  { 
    printf("Hello from task2\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}*/
void  Task2 (void *pdata)
{



    INT8U start;
    INT8U end;
    INT8U toDelay;

    pdata = pdata;                                         /* Prevent compiler warning                 */

    start = OSTimeGet();

    OSTCBCur->compTime = 3;
    OSTCBCur->period = 6;
    while(1) {

        while(OSTCBCur->compTime>0){
            //donothing
            //waiting
        }

        end = OSTimeGet();

        OS_ENTER_CRITICAL();
        //Print();
        toDelay=(OSTCBCur->period)-(end-start);
        start = start + (OSTCBCur->period);
        OSTCBCur->compTime = 3;
        OSTimeDly(toDelay);                            /* Delay remaining time                                 */
        OS_EXIT_CRITICAL();
    }
}

void  Task3 (void *pdata)
{

    //INT8U start;
    INT8U end;
    INT8U toDelay;

    pdata = pdata;                                         /* Prevent compiler warning                 */


    OSTCBCur->start_time = OSTimeGet();
    deadtask = 9;
    OSTCBCur->compTime = 4;
    OSTCBCur->period = 9;
    while(1) {

        while(OSTCBCur->compTime>0){
            //donothing
            //waiting
        }

        end = OSTimeGet();

        OS_ENTER_CRITICAL();
        toDelay=(OSTCBCur->period)-(end-OSTCBCur->start_time);
        OSTCBCur->start_time = OSTCBCur->start_time + (OSTCBCur->period);
        OSTCBCur->compTime = 4;
        OSTimeDly(toDelay);                            /* Delay remaining time                                 */

        OS_EXIT_CRITICAL();
    }
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{
  OSInit();

  OSTaskCreate(Task1, (void *)0, &TaskStk[0][TASK_STK_SIZE-1],1);
  OSTaskCreate(Task2, (void *)0, &TaskStk[1][TASK_STK_SIZE-1],2);
  OSTaskCreate(Task3, (void *)0, &TaskStk[2][TASK_STK_SIZE-1],3);
 /* OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);*/
  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
