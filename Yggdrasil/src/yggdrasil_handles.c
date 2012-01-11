#include "yggdrasil.h"
#include "lcd.h"
#include "midgard.h"
#include "config.h"

//raise event for each event handlers
#define RaiseEvent() {\
	if(sender->events != NULL) {\
		event = sender->events;\
		while(event != NULL) {\
			if(event->type == msg->param) {\
				event->handle(sender);\
			}\
			event = event->next;\
		}\
	}\
}

static int16 _slide_speed = 0;
static void HandleHideButton(Shell_Object * sender);
static void HandleShowButton(Shell_Object * sender);
//these handles below will provide standard handle for standard objects
//each handle will detemined how the object will react to and also raise event

static void HandleButton(Shell_Object * sender)
{
	Message * msg, * newmessage;
	Input_Message * imp;
	System_Message * smp;
	Heap_Manager * heap = sender->heap;
	Event_Handler * event;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						break;
					case TS_MOVE:
						break;
					case TS_RELEASE:	//raise event handler
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_REFRESH:
						if(smp->buffer != NULL)
							Buffer_PrintText(
								8,
								(smp->x + 7), 
								(smp->y + 3), 
								C_WHITE, 
								smp->buffer, "%s\n", sender->text);
						break;
					case MS_DESTROY:
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

static void HandleShowButton(Shell_Object * sender)
{
	Message * msg, * newmessage;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * obj = sender->parent;	//set obj = parent(frame)
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_RELEASE:	//destroy parent
						sender->handle = HandleHideButton;
						_osc._top_object = obj;
						
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

static void HandleHideButton(Shell_Object * sender)
{
	Message * msg, * newmessage;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * target = sender->parent;	//set obj = parent(frame)
	Shell_Object * temp;
	Shell_Object * prev;
	Heap_Manager * heap = sender->heap;
	Task_Properties * task_prop;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_RELEASE:
						//remove from _shell_task
						temp = _osc._shell_task->child;
						prev = NULL;
						//delete taskbar jika memiliki child == target
						while(temp != NULL)
						{
							task_prop = temp->properties;
							if(task_prop->frame == target)
							{
								if(prev != NULL) {
									//prev->sibling = temp->sibling;
									task_prop = prev->properties;
									_osc._top_object = task_prop->frame;
								} else {
									//_shell_task->child = temp->sibling;
									if(temp->sibling != NULL) {
										temp = temp->sibling;
										task_prop = temp->properties;
										_osc._top_object = task_prop->frame;
									} else {
										task_prop = _osc._shell_task->child->properties;
										_osc._top_object = task_prop->frame;
									}
								}
								//DestroyObject(target);
								//target = temp;
								temp = temp->sibling;
								//DestroyObject(target);
								break;
							}
							prev = temp;
							temp = temp->sibling;
						}
						//_top_object = _shell_root;
						//Uart_Printf("top : %s\n", temp->text);
						//jangan ubah2 posisi shell task, itu urusan HandleShowTask
						//set to first child (lowest depth)
						//temp = _shell_task->child;
						//obj->sibling = temp;
						//_shell_task->child = obj;
						//force display task to render immedietly
						//_force_refresh = TRUE;
						//}
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

static void HandleVKButton(Shell_Object * sender)
{
	Message * msg, * newmessage;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * obj = sender->parent;	//set obj = parent(frame)
	Shell_Object * temp;
	Heap_Manager * heap = sender->heap;
	VKButton_Properties * prop = sender->properties;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_RELEASE:
						if(prop != NULL) {
							if(prop->keyboard == NULL) {
								prop->keyboard = CreateKeyboard(sender->parent);
							} else {
								//only set visibility, not destroying the object
								prop->keyboard->visible = !prop->keyboard->visible;
							}
						}
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

//fungsi static untuk handle close button dari frame
static void HandleMsgBoxCloseButton(Shell_Object * sender)
{
	Message * msg, * newmessage;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * obj = sender->parent;	//set obj = parent(frame)
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
				
					case TS_PRESS:
						
						break;
					case TS_RELEASE:	//destroy parent
						//jangan lakukan apapun langsung send message ke parent
						Uart_Printf("destroy %s\n", obj->text);
						SendSystemMessage(MS_DESTROY, 0, 0, 0, obj);
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

//fungsi static untuk handle close button dari frame
static void HandleCloseButton(Shell_Object * sender)
{
	Message * msg, * newmessage;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * obj = sender->parent;	//set obj = parent(frame)
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
				
					case TS_PRESS:
						
						break;
					case TS_RELEASE:	//destroy parent
						//jangan lakukan apapun langsung send message ke parent
						//Uart_Printf("destroy %s\n", obj->text);
						SendSystemMessage(MS_DESTROY, 0, 0, 0, obj);
						//}
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

//fungsi static untuk handle frame
static void HandleFrame(Shell_Object * sender)
{
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	int16 x, y;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						//set to top layer, cek apakah sender memiliki sibling
						//jika tidak, brarti sender sudah berada pada top layer
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_MOVE:
						//sender->rect.x += (imp->x - sender->cursor.x);
						//sender->rect.y += (imp->y - sender->cursor.y);
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_RELEASE:
						sender->cursor.x = 0;
						sender->cursor.y = 0;
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:	//delete shell_object + all messages
						//each window can only be removed by _shell_task (try to prevent glitching)
						SendSystemMessage(MS_REMOVE, 0, 0, (uchar *)sender, _osc._shell_task);
						//Uart_Printf("destroyed\n");
						break;		//kembali pada SendMessage
					case MS_REFRESH:
						break;
					case MS_TOP:
						if(sender->sibling != NULL)
						{
							temp = sender->parent;
							if(temp->child == sender) {
								temp->child = sender->sibling;
								temp = sender->sibling;
							} else {
								temp = temp->child;
								while(temp->sibling != sender)
								{
									temp = temp->sibling;
								}	
								//prevsibling->sibling = nextsibling
								temp->sibling = sender->sibling;
								temp = sender->sibling;
							}
							while(temp->sibling != NULL)
							{
								temp = temp->sibling;
							}
							//lastsibling = sender
							temp->sibling = sender;
							sender->sibling = NULL;
						}
						_osc._top_object = sender;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

//fungsi static untuk handle frame
static void HandleMsgBox(Shell_Object * sender)
{
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	int16 x, y;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						//set to top layer, cek apakah sender memiliki sibling
						//jika tidak, brarti sender sudah berada pada top layer
						break;
					case TS_MOVE:
						//sender->cursor.x = imp->x;
						//sender->cursor.y = imp->y;
						break;
					case TS_RELEASE:
						//sender->cursor.x = 0;
						//sender->cursor.y = 0;
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:	//delete shell_object + all messages
						//OS_ENTER_CRITICAL();
						temp = sender->parent;
						if(temp->child == sender) {
							temp->child = sender->sibling;
							temp = sender->sibling; 
						}
						else {
							temp = temp->child;
							while(temp->sibling != sender)
							{
								temp = temp->sibling;
							}
							//prevsibling->sibling = nextsibling
							temp->sibling = sender->sibling;
							temp = sender->sibling;
						}
						//destroy taskbar
						//each window can only be removed by _shell_task (try to prevent glitching)
						SendSystemMessage(MS_REMOVE, 0, 0, (uchar *)sender, _osc._shell_task);
						break;		//kembali pada SendMessage
					case MS_REFRESH:
						
						break;
					case MS_TOP:
						
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

static void HandlePanel(Shell_Object * sender) {
	Message * msg, * newmessage;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * obj = sender->parent;	//set obj = parent(frame)
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:		
						break;
					case TS_RELEASE:	
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

static void HandleSlider(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						//set to top layer, cek apakah sender memiliki sibling
						//jika tidak, brarti sender sudah berada pada top layer
						
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						//_top_object = sender;
						break;
					case TS_MOVE:
						_slide_speed += (((imp->x - sender->cursor.x) * 8) / 10);
						temp = sender->child;
						while(temp != NULL)
						{	
							temp->rect.x -= _slide_speed;
							temp = temp->sibling;
						}
						_slide_speed = ((_slide_speed * 72) / 100);
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_RELEASE:
						sender->cursor.x = 0;
						sender->cursor.y = 0;
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:	
						break;
					case MS_REFRESH:
						temp = sender->child;
						while(temp != NULL)
						{
							temp->rect.x -= _slide_speed;
							temp = temp->sibling;
						}
						_slide_speed = ((_slide_speed * 72) / 100);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}				
}

static void HandleLauncher(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						//set to top layer, cek apakah sender memiliki sibling
						//jika tidak, brarti sender sudah berada pada top layer
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_MOVE:
						
						break;
					case TS_RELEASE:
						//create sample frame
						CreateSampleTextBox();
						sender->cursor.x = 0;
						sender->cursor.y = 0;
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:
						break;
					case MS_REFRESH:
						if(smp->buffer != NULL)
							Buffer_PrintText(
								6,
								(smp->x + sender->rect.width) - (96 / 2), 
								smp->y + (sender->rect.height), 
								C_WHITE, 
								smp->buffer, "%s\n", sender->text);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

static void HandleTaskBar(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	int16 x;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						//set to top layer, cek apakah sender memiliki sibling
						//jika tidak, brarti sender sudah berada pada top layer
						//Uart_Printf("%s\n", sender->child->name);
						break;
					case TS_MOVE:
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_RELEASE:
						//Uart_Printf("released\n");
						//SendInputMessage(TS_RELEASE, tp.point.x, tp.point.y, sender->child); 
						sender->cursor.x = 0;
						sender->cursor.y = 0;
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_REFRESH:
						break;
					case MS_DESTROY:
						//destroy all child objects, including current
						//DestroyObject(sender);	//delete shell_object
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}	
}

static void HandleTaskDock(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	int16 x;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Shell_Object * prev;
	Shell_Object * target;
	Heap_Manager * heap = sender->heap;
	Task_Properties * task_prop;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						//set to top layer, cek apakah sender memiliki sibling
						//jika tidak, brarti sender sudah berada pada top layer
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_MOVE:
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_RELEASE:
						sender->cursor.x = 0;
						sender->cursor.y = 0;
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_REFRESH:
						break;
					case MS_REMOVE:
						target = (Shell_Object *)smp->buffer;
						temp = _osc._shell_task->child;
						prev = NULL;
						//delete taskbar jika memiliki child == target
						while(temp != NULL)
						{
							task_prop = temp->properties;
							if(task_prop->frame == target)
							{
								if(prev != NULL) {
									prev->sibling = temp->sibling;
									task_prop = prev->properties;
									_osc._top_object = task_prop->frame;
								} else {
									_osc._shell_task->child = temp->sibling;
									task_prop = _osc._shell_task->child->properties;
									_osc._top_object = task_prop->frame;
								}
								DestroyObject(target);
								target = temp;
								temp = temp->sibling;
								DestroyObject(target);
								break;
							}
							prev = temp;
							temp = temp->sibling;
						}
						break;
					case MS_TOP:
						target = (Shell_Object *)smp->buffer;
						temp = _osc._shell_task->child;
						prev = NULL;
						//disort dulu sampe task->child == target => task->sibling = NULL
						while(temp != NULL)
						{
							if(temp == target && temp->sibling != NULL)
							{
								if(prev != NULL) {
									prev->sibling = temp->sibling;
								} else {
									_osc._shell_task->child = temp->sibling;
								}
								prev = temp;
								while(temp->sibling != NULL)
								{
									temp = temp->sibling;
								}
								temp->sibling = prev;
								prev->sibling = NULL;
								break;
							}
							prev = temp;
							temp = temp->sibling;
						}
						_osc._top_object = target;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}	
}

static void HandleRoot(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	#ifndef QUEUE_SIZE
	#define QUEUE_SIZE ENQUEUE_SIZE
	#endif
	uint16 stack_index = 1;
	uint16 * backbuffer = NULL;
	uint16 * currentbuffer;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * currentObj;
	Heap_Manager * heap = sender->heap;
	Shell_Object * node_stack[QUEUE_SIZE];
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						_osc._top_object = sender;
						break;
					case TS_MOVE:
						break;
					case TS_RELEASE:
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:
						break;
					case MS_REFRESH:
						//backbuffer = (uint16 *)smp->buffer;
						backbuffer = m_heap_alloc(_osc._shell_root->heap, (sizeof(uint32) * LCD_HEIGHT * LCD_WIDTH));
						//copy current wallpaper (pointed by _shell_background) to background
						m_memcopy(backbuffer, _osc._shell_background, (LCD_HEIGHT * LCD_WIDTH) * 2);
						//paint slider offscreen on backbuffer
						Buffer_Paint_Bmp_Alpha(
							0, 
							LCD_HEIGHT - 70,
							240, 
							70, 
							backbuffer, 
							_osc._shell_slider->graphic);
						//system information
						Buffer_Printf(30, 8, 202, backbuffer, "Core Clock : %d Mhz\n", GetCoreClock());
						Buffer_Printf(30, 8, 218, backbuffer, "Total Heap : %d Bytes\n", ((uint32)_total_heap));
						Buffer_Printf(30, 8, 234, (uint16 *)backbuffer, "Used Heap : %d Bytes\n", (_used_heap));
						//save current buffer
						currentbuffer = _osc._shell_root->graphic->image;
						//set _shell_root image to backbuffer
						_osc._shell_root->graphic->image = backbuffer;
						//destroy previous buffer
						m_heap_free(_osc._shell_root->heap, currentbuffer);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

void HandleRootDock(Shell_Object * sender) {
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * currentObj;
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						break;
					case TS_MOVE:
						break;
					case TS_RELEASE:
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:
						break;
					case MS_REFRESH:
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

void HandleRootButton(Shell_Object * sender) {
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * currentObj;
	Heap_Manager * heap = sender->heap;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		switch(msg->type)
		{
			case M_INPUT_MSG:
				imp = (Input_Message *)msg;
				switch(imp->param)
				{
					case TS_PRESS:
						break;
					case TS_MOVE:
						break;
					case TS_RELEASE:
						break;
					default:
						break;
				}
				break;
			case M_SYSTEM_MSG:
				smp = (System_Message *)msg;
				switch(smp->param)
				{
					case MS_DESTROY:
						break;
					case MS_REFRESH:
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}