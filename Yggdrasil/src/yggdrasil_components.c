static uchar _hide_cursor = TRUE;

static void HandleTextBox(Shell_Object * sender)
{
	uchar key;
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
						//Uart_Printf("set focus\n");
						_osc._focused_object = sender;
						break;
					case KI_PRESS:
						key = (uchar)imp->x;
						if(key == 8) {
							//if cursor x > 0,  decrement cursor x
							if(sender->graphic->property.c_pointer.x > 0) {
								sender->graphic->property.c_pointer.x--;
							}
							else {
								//if cursor x = 0,  decrement cursor y
								if(sender->graphic->property.c_pointer.y > 0) {
									sender->graphic->property.c_pointer.y--;
									sender->graphic->property.c_pointer.x = 0;
									//loop until find enter or cursor x == num_of_char
									while(sender->graphic->property.c_buffer[sender->graphic->property.c_pointer.x + (sender->graphic->property.c_pointer.y * sender->graphic->property.c_size.width)] != 13
										&& sender->graphic->property.c_pointer.x < sender->graphic->property.c_size.width)
									{
										sender->graphic->property.c_pointer.x++;
									}
								}
							}
						} else {
							Buffer_PutChar(
							(sender->graphic->property.c_pointer.x * 8), 
							(sender->graphic->property.c_pointer.y * 16), 
							key, //character to print
							0x0000,	//forecolor
							0xffff, //backcolor
							(uint16 *)sender->graphic->image, 
							sender->graphic->property.width, 
							sender->graphic->property.height);
							//put key in character buffer for GetText
							sender->graphic->property.c_buffer[sender->graphic->property.c_pointer.x + (sender->graphic->property.c_pointer.y * sender->graphic->property.c_size.width)] = key;
							if(key == 0x0a) {	//0x0d = enter, 0x0a = newline
								//raise cursor y if enter
								if(sender->graphic->property.c_pointer.y < sender->graphic->property.c_size.height) {
									sender->graphic->property.c_pointer.y += 1;
									sender->graphic->property.c_pointer.x = 0;
								}
							} else {
								//raise cursor x if no enter
								sender->graphic->property.c_pointer.x += 1;
							}
						}
						DisplayStackPush(sender);
						break;
					case KI_RELEASE:	//raise event handler
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
						//Uart_Printf("textbox refresh\n");
						if(sender == _osc._focused_object) {
							//if object = focused, set blink on
							if(_hide_cursor) {
								Buffer_PutChar(
									(sender->graphic->property.c_pointer.x * 8), 
									(sender->graphic->property.c_pointer.y * 16), 
									'_', //print cursor
									0x0000,	//forecolor
									0xffff, //backcolor
									(uint16 *)sender->graphic->image, 
									sender->graphic->property.width, 
									sender->graphic->property.height);
								sender->graphic->property.c_buffer[sender->graphic->property.c_pointer.x + (sender->graphic->property.c_pointer.y * sender->graphic->property.c_size.width)] = ' ';
								_hide_cursor = FALSE;
							} else {
								Buffer_PutChar(
									(sender->graphic->property.c_pointer.x * 8), 
									(sender->graphic->property.c_pointer.y * 16), 
									' ', //print cursor
									0x0000,	//forecolor
									0xffff, //backcolor
									(uint16 *)sender->graphic->image, 
									sender->graphic->property.width, 
									sender->graphic->property.height);
								_hide_cursor = TRUE;
							}
						} else {
							//if object |= focused, set blink off
							Buffer_PutChar(
								(sender->graphic->property.c_pointer.x * 8), 
								(sender->graphic->property.c_pointer.y * 16), 
								' ', //print cursor
								0x0000,	//forecolor
								0xffff, //backcolor
								(uint16 *)sender->graphic->image, 
								sender->graphic->property.width, 
								sender->graphic->property.height);
							_hide_cursor = TRUE;
						}
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

Shell_Object * CreateTextBox(int16 x, int16 y, uchar num_of_char, uchar num_of_line, Shell_Object * parent)
{
	uint32 size_alloc;
	Rect r;
	Graphic_Property bmp;
	uchar * fill;
	Shell_Object * obj;
	r.x = x;
	r.y = y;
	r.width = num_of_char * 8;		//calculate textbox width
	r.height = num_of_line * 16;	//calculate textbox height
	bmp.key = C_MAGENTA;
	bmp.width = num_of_char * 8;	//calculate image width
	bmp.height = num_of_line * 16;	//calculate image height
	bmp.c_size.width = num_of_char;	//amount of characters perline
	bmp.c_size.height = num_of_line;	//amount of displayed line
	obj = CreateObject(TRUE, KEY, 16384 + (bmp.width * bmp.height * sizeof(uint16)), "",
			&r, NULL, &bmp, parent, 
			HandleTextBox);
	obj->graphic->property.c_pointer.x = 0;
	obj->graphic->property.c_pointer.y = 0;
	size_alloc = (bmp.width * bmp.height * sizeof(uint16)) - 1;
	//fill with blank, better use memory clear
	for(; size_alloc > 0; size_alloc--) {
		*(uchar *)((uint32)obj->graphic->image + size_alloc) = 0xff;
	}
	return obj;
}

Shell_Object * CreateSampleTextBox(void)
{
	Shell_Object * parent;
	Shell_Object * obj;
	parent = CreateFrame("Sample", panel);
	Uart_Printf("creating textbox\n");
	obj = CreateTextBox(24, 40, 24, 5, parent);
	return obj;
}

uchar * GetText(Shell_Object * object)
{
	return object->graphic->property.c_buffer;
}
