
//shell message sender, this will only add message to the object message queue
void SendMessage(Shell_Object * obj, Message * message)
{
	#ifdef OS_uCOS_II_H
	//heap used by several task simultaneously like sharing memory
	#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
    #endif
    #endif
	Message * temp;
	//SendMessage is a critical process (it can be access from more than
	//one task simultaneously) therefore...
	#ifdef OS_uCOS_II_H
	OS_ENTER_CRITICAL();
	#endif
	if(obj->msg == NULL)
	{
		obj->msg = message;
	}
	else
	{
		temp = obj->msg;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = message;
	}
	//don't execute message yet
	if(obj->handle != NULL)
		obj->handle(obj);	//run event handler
	#ifdef OS_uCOS_II_H
	OS_EXIT_CRITICAL();
	#endif
}

//shell standard input message creation
void SendInputMessage(uchar state, int16 x, int16 y, Shell_Object * obj)
{
	Input_Message * msg = m_heap_alloc(obj->heap, sizeof(Input_Message));
	msg->type = M_INPUT_MSG;	//message type definition
	msg->next = NULL;			//always NULL for newly created message
	msg->param = state;			//input state (PRESS/RELEASE/MOVE)
	msg->x = x;					//x
	msg->y = y;					//y
	SendMessage(obj, (Message *)msg);
}

//shell standard system message creation
void SendSystemMessage(uchar param, int16 x, int16 y, uchar * buffer, Shell_Object * obj)
{
	System_Message * msg = m_heap_alloc(obj->heap, sizeof(System_Message));
	msg->type = M_SYSTEM_MSG;	//message type definition
	msg->next = NULL;			//always NULL for newly created message
	msg->param = param;			//system command
	msg->x = x;
	msg->y = y;
	msg->buffer = buffer;		//buffer ke backbuffer pada MS_REFRESH
	SendMessage(obj, (Message *)msg);
}

//this task will handle all message sending to the corresponding object
void MessageTask(void *pdata)
{
	#ifndef QUEUE_SIZE
	#define QUEUE_SIZE ENQUEUE_SIZE
	#endif
	uint16 stack_index = 1;
	Shell_Object * currentObj = _osc._shell_root;
	Shell_Object * node_stack[QUEUE_SIZE];
	//execute send message
	while(1) {
		stack_index = 1;
		//iterate all message for root + subroots objects
		currentObj = _osc._shell_root;
		//send object message
		if(currentObj->msg != NULL && currentObj->handle != NULL)
		{
			currentObj->handle(currentObj);
		}
		currentObj = currentObj->child;
		get_root_next_child:
		if(currentObj == NULL) {
			goto shell_root_detect_finish;
		}
		if(currentObj->sibling != NULL) {
			//enqueue sibling, if this node have sibling
			node_stack[stack_index++] = currentObj->sibling;
		}
		//send object message
		if(currentObj->msg != NULL && currentObj->handle != NULL)
		{
			currentObj->handle(currentObj);
		}
		if(currentObj->child != NULL) {
			currentObj = currentObj->child;
			goto get_root_next_child;
		}
		if(stack_index == 1) {
			goto shell_root_detect_finish;
		}
		//dequeue (DFS)
		currentObj = node_stack[--stack_index];
		goto get_root_next_child;
		shell_root_detect_finish:
		stack_index = 1;
		
		//iterate all message for _shell_task
		currentObj = _osc._shell_task;
		//send taskdock message
		if(currentObj->msg != NULL && currentObj->handle != NULL)
		{
			currentObj->handle(currentObj);
		}
		currentObj = currentObj->child;
		get_task_next_sibling:
		if(currentObj == NULL) {
			goto shell_task_detect_finish;
		}
		//send taskbars message
		if(currentObj->msg != NULL && currentObj->handle != NULL)
		{
			currentObj->handle(currentObj);
		}
		currentObj = currentObj->sibling;
		goto get_task_next_sibling;
    	shell_task_detect_finish:
    	//OS delay
		OSTimeDly(100/5);
	}
}