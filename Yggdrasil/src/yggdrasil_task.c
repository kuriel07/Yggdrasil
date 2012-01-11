void HandleTaskTop(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
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
						task_prop = (Task_Properties *)sender->properties;
						_osc._top_object = task_prop->frame;
						_osc._shell_task->visible = FALSE;
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
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

void HandleShowTask(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	#ifndef QUEUE_SIZE
	#define QUEUE_SIZE ENQUEUE_SIZE
	#endif
	uchar top_detected = 0;
	uint16 _dequeue_index = 0;
	uint16 _enqueue_index = 0;
	uint16 _stack_index = 0;
	uint16 __stack_index = 0;
	int16 _xmid = (240 - 48) / 2;
	int16 i;
	Graphic * t_graphic;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Event_Handler * event;
	Shell_Object * temp;
	Shell_Object * prev_temp = NULL;
	Heap_Manager * heap = sender->heap;
	Task_Properties * task_prop;
	Shell_Object * top_task;
	Shell_Object * object_queue[QUEUE_SIZE];
	Shell_Object * object_stack[QUEUE_SIZE];
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
						//1st pass
						//copy displayed top image to allocated image
						m_memcopy(_osc._shell_task->graphic->allocated_image, _LCD_BUFFER, (sizeof(uint16) * _osc._shell_task->graphic->property.width * _osc._shell_task->graphic->property.height));
						//create inactive screen
						Buffer_Paint_Inactive(0, 0, LCD_WIDTH, LCD_HEIGHT, _osc._shell_task->graphic->allocated_image);
						//t_graphic = m_heap_alloc(heap, sizeof(Graphic));
						//t_graphic->property.width = LCD_WIDTH;
						//t_graphic->property.height = LCD_HEIGHT;
						//t_graphic->allocated_image = inactive_screen_img;	//set to current displayed image
						//t_graphic->image = t_graphic->allocated_image;
						//t_graphic->property.alpha = 70;
						//Buffer_Paint_Bmp_Alpha(0, 0, LCD_WIDTH, LCD_HEIGHT, _shell_task->graphic->allocated_image, t_graphic);
						//m_heap_free(heap, t_graphic);
						
						_xmid = 0;
						temp = _osc._shell_task->child;
						while(temp != NULL) {
							_xmid++;
							temp = temp->sibling;
						}
						_xmid = _xmid / 2;
						i = 0;
						temp = _osc._shell_task->child;
						while(temp != NULL) {
							temp->rect.y = 64;			//set y coordinate
							task_prop = temp->properties;
							temp->child = NULL;
							if(i == _xmid) {
								top_detected = 1;
							}
							i++;
							if(task_prop->frame == _osc._top_object) {
								top_task = temp;
								//update task icon (48 x 64)px
								t_graphic = m_heap_alloc(heap, sizeof(Graphic));
								t_graphic->property.width = LCD_WIDTH;
								t_graphic->property.height = LCD_HEIGHT;
								t_graphic->allocated_image = _LCD_BUFFER;	//set to current displayed image
								UpdateTaskIcon(temp->graphic, t_graphic);
								m_heap_free(heap, t_graphic);
							} else {
								if(top_detected) {
									object_stack[_stack_index++] = temp;
								} else {
									object_queue[_enqueue_index++] = temp;
								}
							}
							temp = temp->sibling;
						}
						_xmid = (LCD_WIDTH - 48) / 2;
						//rearrange _shell_task->child
						_osc._shell_task->child = NULL;
						prev_temp = NULL;
						for(i = (_xmid - (20 * _enqueue_index)); i<_xmid; i+=20) {
							temp = object_queue[_dequeue_index++];
							temp->rect.x = i;
							//temp->handle = HandleTaskLeft
							if(prev_temp == NULL) {
								_osc._shell_task->child = temp;
							} else {
								prev_temp->sibling = temp;
							}
							prev_temp = temp;
						}
						__stack_index = _stack_index;
						for(i = (_xmid + (20 * _stack_index)); i>_xmid; i-=20) {
							temp = object_stack[--__stack_index];
							temp->rect.x = i;
							//temp->handle = HandleTaskRight
							if(prev_temp == NULL) {
								_osc._shell_task->child = temp;
							} else {
								prev_temp->sibling = temp;
							}
							prev_temp = temp;
							if(_stack_index == 0) {
								i = 0;		//break
							}
						}
						if(prev_temp != NULL) {
							top_task->rect.x = _xmid;
							top_task->sibling = NULL;
							//top_task->handle = HandleTaskTop;
							prev_temp->sibling = top_task; 
						} else {
							top_task->rect.x = _xmid;
							top_task->sibling = NULL;
							//top_task->handle = HandleTaskTop;
							_osc._shell_task->child = top_task;
						}
						_osc._shell_task->visible = FALSE;
						break;
					case TS_MOVE:
						break;
					case TS_RELEASE:
						//2nd pass
						_xmid = 0;
						temp = _osc._shell_task->child;
						while(temp != NULL) {
							_xmid++;
							temp = temp->sibling;
						}
						_xmid = _xmid / 2;
						i = 0;
						temp = _osc._shell_task->child;
						while(temp != NULL) {
							temp->rect.y = 64;			//set y coordinate
							//temp->sibling = NULL;		//clear next sibling
							task_prop = temp->properties;
							temp->child = NULL;
							if(i == _xmid) {
								top_detected = 1;
							}
							i++;
							if(task_prop->frame == _osc._top_object) {
								top_task = temp;
							} else {
								if(top_detected) {
									object_stack[_stack_index++] = temp;
								} else {
									object_queue[_enqueue_index++] = temp;
								}
							}
							temp = temp->sibling;
						}
						_xmid = (240 - 48) / 2;
						//rearrange _shell_task->child
						//Uart_Printf("%d %d\n", _enqueue_index, _stack_index);
						_osc._shell_task->child = NULL;
						prev_temp = NULL;
						for(i = (_xmid - (20 * _enqueue_index)); i<_xmid; i+=20) {
							temp = object_queue[_dequeue_index++];
							temp->rect.x = i;
							//temp->handle = HandleTaskLeft
							if(prev_temp == NULL) {
								_osc._shell_task->child = temp;
							} else {
								prev_temp->sibling = temp;
							}
							prev_temp = temp;
						}
						__stack_index = _stack_index;
						for(i = (_xmid + (20 * _stack_index)); i>_xmid; i-=20) {
							temp = object_stack[--__stack_index];
							temp->rect.x = i;
							//temp->handle = HandleTaskRight
							if(prev_temp == NULL) {
								_osc._shell_task->child = temp;
							} else {
								prev_temp->sibling = temp;
							}
							prev_temp = temp;
							if(_stack_index == 0) {
								i = 0;		//break
							}
						}
						if(prev_temp != NULL) {
							top_task->rect.x = _xmid;
							top_task->sibling = NULL;
							//top_task->handle = HandleTaskTop;
							prev_temp->sibling = top_task; 
						} else {
							top_task->rect.x = _xmid;
							top_task->sibling = NULL;
							//top_task->handle = HandleTaskTop;
							_osc._shell_task->child = top_task;
						}
						_osc._shell_task->visible = TRUE;
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
		RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}