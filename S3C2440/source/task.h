//*------------------------------------------------------------------------------------------------
//* 文件名				: task.h
//* 功能描述			: 定义系统运行的各个任务的优先级、堆栈大小及函数原型声明，之所以在这里集中指定各
//*						: 任务的优先级和堆栈大小主要是考虑能够避免优先级分配错误，能够直观的了解系统内存
//*						: 的占用情况
//* 作者    			: 焦海波
//* 版本				: 0.1
//* 建立日期、时间		: 2006/06/08 09:12
//* 最近修改日期、时间	: 
//* 修改原因			: 
//*------------------------------------------------------------------------------------------------

#ifndef	__task_h
#define	__task_h

//*---------------------------------------- 结构体定义 ---------------------------------------------
#define		T_START_PRIOR					3
#define		T_ETHERNETIF_INPUT_PRIOR		6
#define		T_WEBSERVER_PRIOR				10
#define		T_RUNTIP_PRIOR					14
/* 备注：4-5之间的优先级作为PIP或者保留，7-9之间的优先级由LwIP协议栈占用 */

#define		PIP_LOW_LEVEL_INPUT				4
#define		PIP_LOW_LEVEL_OUTPUT			5


#define		T_WEBSERVER_STKSIZE				1024
#define		T_ETHERNETIF_INPUT_STKSIZE		256
#define		T_RUNTIP_STKSIZE				64

#endif