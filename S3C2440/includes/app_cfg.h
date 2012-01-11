/*//////////////////////////////////////////////////////////////
define the Tasks' Stk lengh Prio Function on the project
by: tangxiaofeng xidian 503
//////////////////////////////////////////////////////////////
*/

#ifndef APP_CFG_H
#define APP_CFG_H


#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  		0
#endif

#define USER_USING_MODE    0x10 
#include "2440addr.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "ucos_ii.h"  
#include "Target.h"
//Task Stk lengh

#define	ShellTaskStkLength			(1024*32)   	// Define the MainTask stack length  
#define	TouchTaskStkLength			(2048*64)   	// Define the Task1 stack length 
#define MessageTaskStkLength		(1024*32)
#define HostTaskStkLength			(1024*32)

//Task function
//void ShellTask(void *pdata);   		//MainTask task
//void TouchTask(void *pdata);		// Task1 

//Task Prio
#define NormalTaskPrio      7
#define ShellTaskPrio 		2
#define MessageTaskPrio 	1
#define TouchTaskPrio  		3
#define HostTaskPrio		4
#define TimerTaskPrio		6		//invoked every 1 second
#define DesktopTaskPrio		10
#define AppTaskPrio			12
#define IdleTaskPrio  		31
#define TaskUartPrio  		NormalTaskPrio+4


#endif