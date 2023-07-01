
/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                         3       /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_EVENT      *R1;                                    //lab3
OS_EVENT      *R2;
INT8U         err1;                                  //lab3
INT8U         err2;
/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
        void  Print(void);
        void  Task1(void *data);                       /* Function prototypes of tasks                  */
        void  Task2(void *data);                       /* Function prototypes of tasks                  */
        void  Task3(void *data);                       /* Function prototypes of tasks                  */
        void  initDisp(void);
        void  SetArg(void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

void  main (void)
{
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
    PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

    OS_ENTER_CRITICAL();
    PC_VectSet(0x08, OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    OS_EXIT_CRITICAL();

    OSTaskCreate(Task1, (void *)0, &TaskStk[0][TASK_STK_SIZE-1],3);
    OSTaskCreate(Task2, (void *)0, &TaskStk[1][TASK_STK_SIZE-1],4);
    // OSTaskCreate(Task3, (void *)0, &TaskStk[2][TASK_STK_SIZE-1],5);//task1
    
    R1 = OSMutexCreate(1, &err1);
    R2 = OSMutexCreate(2, &err2); 
    SetArg();
    OSStart();                                             /* Start multitasking                       */
}


/*
*********************************************************************************************************
*                                                  TASKS 1
*********************************************************************************************************
*/
void  Task1 (void *pdata)
{   
    INT8U  err;

    INT8U start;
    INT8U end;
    INT8U toDelay;
    
    INT8U lockR1;//lab3
    INT8U lockR2;
    INT8U lockR3;//task2

    INT16S key;
    pdata = pdata;                                         /* Prevent compiler warning                 */
    
    start = 0;

    lockR1 = 0;//lab3
    lockR2 = 0;
    lockR3 = 0;
    // OSTCBCur->compTime = 6;
    // OSTCBCur->period = 30;
    
    while(1) {
        
        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                PC_DOSReturn();                            /* Return to DOS                            */
            }
        }
        // OSTimeDly(8); //task1
        OSTimeDly(5); //task2
        while(OSTCBCur->compTime>0){
            //task1
            // if(OSTCBCur->compTime==4 && lockR1 == 0){
            //     OSMutexPend(R1, 5, &err1);
            //     OS_ENTER_CRITICAL();
            //     // printf("1 prio %d\n",OSTCBCur->OSTCBPrio);
            //     lockR1 = 1;
            //     buf[tindex][0] = 3;
            //     buf[tindex][1] = 1; //R1
            //     buf[tindex][2] = 3;
            //     buf[tindex][3] = OSTime;
            //     tindex = tindex + 1;
            //     OS_EXIT_CRITICAL();
            // }else if(OSTCBCur->compTime==2 && lockR2 == 0){
            //     OSMutexPend(R2, 5, &err2);
            //     OS_ENTER_CRITICAL();
            //     // printf("1 prio %d\n",OSTCBCur->OSTCBPrio);
            //     lockR2 = 1;
            //     buf[tindex][0] = 3;
            //     buf[tindex][1] = 2; //R1
            //     buf[tindex][2] = 3;
            //     buf[tindex][3] = OSTime;
            //     tindex = tindex + 1;
            //     OS_EXIT_CRITICAL();
            // }

            //task2
            if(OSTCBCur->compTime==6 && lockR1 == 0){
                OSMutexPend(R1, 5, &err1);
                OS_ENTER_CRITICAL();
                
                lockR1 = 1;
                buf[tindex][0] = 3;
                buf[tindex][1] = 1; //R1
                buf[tindex][2] = 3;
                buf[tindex][3] = OSTime;
                tindex = tindex + 1;
                OS_EXIT_CRITICAL();
            }else if(OSTCBCur->compTime==9 && lockR2 == 0){
                OSMutexPend(R2, 5, &err2);
                OS_ENTER_CRITICAL();
                lockR2 = 1;
                buf[tindex][0] = 3;
                buf[tindex][1] = 2; //R1
                buf[tindex][2] = 3;
                buf[tindex][3] = OSTime;
                tindex = tindex + 1;
                OS_EXIT_CRITICAL();
            }else if(OSTCBCur->compTime==3 && lockR3 == 0){
                OSMutexPost(R1);//lab3
                OS_ENTER_CRITICAL();
                lockR3 = 1;
                buf[tindex][0] = 4;
                buf[tindex][1] = 1; //R1
                buf[tindex][2] = 3;
                buf[tindex][3] = OSTime;
                tindex = tindex + 1;
                OS_EXIT_CRITICAL();

                // OSMutexPend(R2, 5, &err2);
                // OS_ENTER_CRITICAL();
                // lockR3 = 1;
                // buf[tindex][0] = 3;
                // buf[tindex][1] = 2; //R1
                // buf[tindex][2] = 3;
                // buf[tindex][3] = OSTime;
                // tindex = tindex + 1;
                // OS_EXIT_CRITICAL();
            }
            
            
        }
        
        OSMutexPost(R2);//task12 lab3
        // OSMutexPost(R1);//task1
        end = OSTimeGet();
        OS_ENTER_CRITICAL();
        // printf("1 unlock prio %d\n",OSTCBCur->OSTCBPrio);
        lockR1 = 0;
        lockR2 = 0;
        lockR3 = 0;
/*task1      
        buf[tindex][0] = 4;
        buf[tindex][1] = 1; //R1
        buf[tindex][2] = OSTCBCur->OSTCBPrio;
        buf[tindex][3] = end;
        tindex = tindex + 1;

        buf[tindex][0] = 4;
        buf[tindex][1] = 2; //R1
        buf[tindex][2] = OSTCBCur->OSTCBPrio;
        buf[tindex][3] = end;
        tindex = tindex + 1;
*/  
        //task2
        buf[tindex][0] = 4;
        buf[tindex][1] = 2; //R1
        buf[tindex][2] = OSTCBCur->OSTCBPrio;
        buf[tindex][3] = end;
        tindex = tindex + 1;

        Print();
        end = OSTimeGet();
        toDelay=(OSTCBCur->period)-(end-start);
        // printf("toDelay %d\n",toDelay);
        start = start + (OSTCBCur->period);
        // OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;//lab2
        // OSTCBCur->compTime = 6;//task 1
        OSTCBCur->compTime = 11;//task 12
        OSTimeDly(toDelay);        
        
        OS_EXIT_CRITICAL();
        
              

        
        
       
    }
}
/*
*********************************************************************************************************
*                                                  TASKS 2
*********************************************************************************************************
*/

void  Task2 (void *pdata)
{
    INT8U  err;

    INT8U start;
    INT8U end;
    INT8U toDelay;
    
    INT8U lockR1;//lab3
    INT8U lockR2;
    INT8U lockR3;

    INT16S key;
    pdata = pdata;                                         /* Prevent compiler warning                 */
    
    start = 0;

    lockR1 = 0;//lab3
    lockR2 = 0;
    lockR3 = 0;//task2
    // OSTCBCur->compTime = 6;
    // OSTCBCur->period = 30;
    
    
    while(1) {
         
        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                PC_DOSReturn();                            /* Return to DOS                            */
            }
        }
        // OSTimeDly(4);//task1 lab3
        while(OSTCBCur->compTime>0){
            //task1
            // if(OSTCBCur->compTime==4 && lockR2 == 0){
            //     OSMutexPend(R2, 5, &err2);
            //     OS_ENTER_CRITICAL();
            //     // printf("2 prio %d\n",OSTCBCur->OSTCBPrio);
            //     lockR2 = 1;
            //     buf[tindex][0] = 3;
            //     buf[tindex][1] = 2; //R1
            //     buf[tindex][2] = 4;
            //     buf[tindex][3] = OSTime;
            //     tindex = tindex + 1;
            //     OS_EXIT_CRITICAL();
            // }
            //task2
            if(OSTCBCur->compTime==10 && lockR1 == 0){
                OSMutexPend(R1, 5, &err1);
                OS_ENTER_CRITICAL();
                lockR1 = 1;
                buf[tindex][0] = 3;
                buf[tindex][1] = 1; //R1
                buf[tindex][2] = 4;
                buf[tindex][3] = OSTime;
                tindex = tindex + 1;
                OS_EXIT_CRITICAL();
            }else if(OSTCBCur->compTime==4 && lockR2 == 0){
                OSMutexPend(R2, 5, &err2);
                OS_ENTER_CRITICAL();
                lockR2 = 1;
                buf[tindex][0] = 3;
                buf[tindex][1] = 2; //R1
                buf[tindex][2] = 4;
                buf[tindex][3] = OSTime;
                tindex = tindex + 1;
                OS_EXIT_CRITICAL();
            }else if(OSTCBCur->compTime==2 && lockR3 == 0){
                OSMutexPost(R2);
                OS_ENTER_CRITICAL();  
                lockR3 = 1;   
                buf[tindex][0] = 4;
                buf[tindex][1] = 2; //R1
                buf[tindex][2] = 4;
                buf[tindex][3] = OSTime;
                tindex = tindex + 1;
                OS_EXIT_CRITICAL();
            }
        }
        OSMutexPost(R1);
        
        
        end = OSTimeGet();
        
        OS_ENTER_CRITICAL();
        // printf("2 unlock prio %d\n",OSTCBCur->OSTCBPrio);
        // Print();
        lockR1 = 0;
        lockR2 = 0;
        lockR3 = 0;
        buf[tindex][0] = 4;
        buf[tindex][1] = 1; //R1
        buf[tindex][2] = 4;
        buf[tindex][3] = OSTime;
        tindex = tindex + 1;
        toDelay=(OSTCBCur->period)-(end-start);
        start = start + (OSTCBCur->period);
        // printf("toDelay %d\n",toDelay);
        // OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;//lab2
        // OSTCBCur->compTime = 3;//task1
        // OSTCBCur->compTime = 6;//task1
        OSTCBCur->compTime = 12;//task1
        OSTimeDly(toDelay);                            /* Delay remaining time                                 */
        OS_EXIT_CRITICAL();
    }
}
/*
*********************************************************************************************************
*                                                  TASKS 3
*********************************************************************************************************
*/

void  Task3 (void *pdata)
{
    INT8U  err;

    INT8U start;
    INT8U end;
    INT8U toDelay;
    
    INT8U lockR1;
    INT8U lockR2;

    INT16S key;
    pdata = pdata;                                         /* Prevent compiler warning                 */
    
    start = 0;

    lockR1 = 0;
    lockR2 = 0;

    OSTCBCur->compTime = 9;
    OSTCBCur->period = 30;

    while(1) {
         if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                PC_DOSReturn();                            /* Return to DOS                            */
            }
        }
        while(OSTCBCur->compTime>0){
            //donothing
            //waiting
            if(OSTCBCur->compTime == 7 && lockR1 == 0){
                OSMutexPend(R1, 5, &err1);
                OS_ENTER_CRITICAL();
                // printf("3 prio %d\n",OSTCBCur->OSTCBPrio);
                lockR1 = 1;
                buf[tindex][0] = 3;
                buf[tindex][1] = OSTCBCur->OSTCBPrio; //R1
                buf[tindex][2] = 5;
                buf[tindex][3] = OSTime;
                tindex = tindex + 1;
                // printf("%d  %d",OSTCBPrioTbl[1]->period, OSTCBPrioTbl[5]->period);
                OS_EXIT_CRITICAL();
            }
            
        }
        
        OSMutexPost(R1);
        end = OSTimeGet();
        
        OS_ENTER_CRITICAL();
        // printf("3 unlock prio %d\n",OSTCBCur->OSTCBPrio);
        lockR1 = 0;
        lockR2 = 0;
        buf[tindex][0] = 4;
        buf[tindex][1] = 1; //R1
        buf[tindex][2] = 5;
        buf[tindex][3] = OSTime;
        tindex = tindex + 1;
        // Print();
        // printf("toDelay %d\n",toDelay);
        toDelay=(OSTCBCur->period)-(end-start);
        start = start + (OSTCBCur->period);
        // OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;//lab2
        OSTCBCur->compTime = 9;
        OSTimeDly(toDelay);                            /* Delay remaining time                                 */
        OS_EXIT_CRITICAL();
    }
}
/*
*********************************************************************************************************
*                                                  PRINT
*********************************************************************************************************
*/
void Print(void){

    INT8U    task1[80];
    INT8U    task2[80];
    INT8U    p[10];
    INT8U    time[10];
    INT8U    i;

    // time1 = OSTimeGet();
    // printf("%d %d\n", tindex, pindex);
    if(tindex > 20 &&pindex < 20){
        while(pindex <= 20){
            //  printf("%d %d  Complete  %d  %d\n",pindex,buf[pindex][3],buf[pindex][1],buf[pindex][2]);
            if(buf[pindex][0]==2){
            // preempt
            //    if(i <10)printf("%d  Preempt  %d  %d  %d  %d\n",buf[pindex][3],buf[pindex][1],buf[pindex][2],buf[pindex][4],buf[pindex][5]);
            printf("%d  Preempt  %d  %d \n",buf[pindex][3],buf[pindex][1],buf[pindex][2]); 
            }else if(buf[pindex][0]==1){
                printf("%d  Complete  %d  %d\n",buf[pindex][3],buf[pindex][1],buf[pindex][2]);
            // }
            }else if(buf[pindex][0]==3){
                //lock
                printf("%d  lock  R%d  Prio=%d changes to=%d\n",buf[pindex][3],buf[pindex][1],buf[pindex][2],buf[pindex][1]);
            }
            else if(buf[pindex][0]==4){
                //unlock
                printf("%d  unlock  R%d  Prio=%d changes to=%d\n",buf[pindex][3],buf[pindex][1],buf[pindex][1],buf[pindex][2]);
            }
            pindex = pindex + 1;
        }
    }
    // if(tindex > 80){
    //     tindex = 0;
        
    //     i = 0;
    //     // initDisp();
    // }
    // if(pindex > 80){
    //     pindex = 0;
    // }
    
}


void SetArg(void){
    INT8U i;
    OS_TCB* ptcb;
    OSTime = 0;
    ptcb = OSTCBList;
    while(ptcb->OSTCBPrio <= 5){
        if(ptcb->OSTCBPrio==3||ptcb->OSTCBPrio==4||ptcb->OSTCBPrio==5){
            if(ptcb->OSTCBPrio==3){
                ptcb->OldPrio = ptcb->OSTCBPrio;
                //task1
                // ptcb->compTime = 6;
                // ptcb->period = 30;
                

                //task2
                ptcb->compTime = 11;
                ptcb->period = 30;
               
                
            }else if(ptcb->OSTCBPrio==4){
                ptcb->OldPrio = ptcb->OSTCBPrio;
                //task1
                // ptcb->compTime = 6;
                // ptcb->period = 30;
                
            
                //task1
                ptcb->compTime = 12;
                ptcb->period = 30;
                
                
            }else if(ptcb->OSTCBPrio==5){
                ptcb->OldPrio = ptcb->OSTCBPrio;
                ptcb->compTime = 9;
                ptcb->period = 30;
               
            }
            
        }
        ptcb = ptcb->OSTCBNext;
    }
    for(i=0;i<80;i++){
        buf[i][0]=0;
        buf[i][1]=0;
        buf[i][2]=0;
        buf[i][3]=0;
    }
    
}