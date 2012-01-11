
static void HandleKeys(Shell_Object * sender)
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
						SendInputMessage(KI_PRESS, sender->text[0], 0, _osc._focused_object);
						break;
					case TS_MOVE:
						break;
					case TS_RELEASE:	//raise event handler
						SendInputMessage(KI_RELEASE, sender->text[0], 0, _osc._focused_object);
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


Shell_Object * CreateKeyboardButton(int16 x, int16 y, uchar * text, uchar * image, Shell_Object * parent) {
	//button creation
	Rect r;
	Graphic_Property bmp;
	Shell_Object * obj;
	r.x = x;
	r.y = y;
	r.width = 24;
	r.height = 20;
	bmp.key = C_MAGENTA;
	bmp.width = 24;
	bmp.height = 20;
	bmp.c_buffer = NULL;
	obj = CreateObject( TRUE, FORCE, 4096 + (sizeof(uint16) * bmp.width * bmp.height), text, &r,
			image, &bmp, parent, HandleKeys);
	return obj;
}

Shell_Object * CreateKeyboard(Shell_Object * parent)
{
	Rect r;
	Graphic_Property bmp;
	Shell_Object * object;
	Shell_Object * temp;
	r.x = 0;
	r.y = LCD_HEIGHT - 150;
	r.width = 240;
	r.height = 150;
	bmp.key = C_MAGENTA;
	bmp.width = 240;
	bmp.height = 150;
	bmp.c_buffer = NULL;
	object = CreateObject( TRUE, STREAM, 8192 + (sizeof(uint16) * bmp.width * bmp.height), "osk", &r,
			k_panel, &bmp, parent, HandleFrame);
	//printf("parent child : %d\n", parent->child);
	//1234567890
	CreateKeyboardButton(20, 12, "1", _key_1, object);
	CreateKeyboardButton(33, 33, "2", _key_2, object);
	CreateKeyboardButton(45, 12, "3", _key_3, object);
	CreateKeyboardButton(58, 33, "4", _key_4, object);
	CreateKeyboardButton(70, 12, "5", _key_5, object);
	CreateKeyboardButton(146, 12, "6", _key_6, object);
	CreateKeyboardButton(158, 33, "7", _key_7, object);
	CreateKeyboardButton(171, 12, "8", _key_8, object);
	CreateKeyboardButton(183, 33, "9", _key_9, object);
	CreateKeyboardButton(196, 12, "0", _key_0, object);
	//qwertyuiop
	CreateKeyboardButton(8, 33, "q", _key_q, object);
	CreateKeyboardButton(20, 54, "w", _key_w, object);
	CreateKeyboardButton(45, 54, "e", _key_e, object);
	CreateKeyboardButton(70, 54, "r", _key_r, object);
	CreateKeyboardButton(95, 54, "t", _key_t, object);
	CreateKeyboardButton(121, 54, "y", _key_y, object);
	CreateKeyboardButton(146, 54, "u", _key_u, object);
	CreateKeyboardButton(171, 54, "i", _key_i, object);
	CreateKeyboardButton(196, 54, "o", _key_o, object);
	CreateKeyboardButton(208, 33, "p", _key_p, object);
	//asdfghjkl
	CreateKeyboardButton(8, 75, "a", _key_a, object);
	CreateKeyboardButton(33, 75, "s", _key_s, object);
	CreateKeyboardButton(58, 75, "d", _key_d, object);
	CreateKeyboardButton(83, 75, "f", _key_f, object);
	CreateKeyboardButton(108, 75, "g", _key_g, object);
	CreateKeyboardButton(133, 75, "h", _key_h, object);
	CreateKeyboardButton(158, 75, "j", _key_j, object);
	CreateKeyboardButton(183, 75, "k", _key_k, object);
	CreateKeyboardButton(208, 75, "l", _key_l, object);
	//zxcvbnm
	CreateKeyboardButton(20, 96, "z", _key_z, object);
	CreateKeyboardButton(45, 96, "x", _key_x, object);
	CreateKeyboardButton(70, 96, "c", _key_c, object);
	CreateKeyboardButton(95, 96, "v", _key_v, object);
	CreateKeyboardButton(120, 96, "b", _key_b, object);
	CreateKeyboardButton(145, 96, "n", _key_n, object);
	CreateKeyboardButton(170, 96, "m", _key_m, object);
	CreateKeyboardButton(195, 96, "\x8", _key_backspace, object);
	
	//create enter key
	r.x = 183;
	r.y = 117;
	r.width = 49;
	r.height = 20;
	bmp.key = C_MAGENTA;
	bmp.width = 49;
	bmp.height = 20;
	bmp.c_buffer = NULL;
	CreateObject( TRUE, FORCE, 4096 + (sizeof(uint16) * bmp.width * bmp.height), "\xa", &r,
			_key_enter, &bmp, object, HandleKeys);
	
	//create space key
	r.x = 58;
	r.y = 117;
	r.width = 124;
	r.height = 20;
	bmp.key = C_MAGENTA;
	bmp.width = 124;
	bmp.height = 20;
	bmp.c_buffer = NULL;
	CreateObject( TRUE, FORCE, 4096 + (sizeof(uint16) * bmp.width * bmp.height), " ", &r,
			_key_space, &bmp, object, HandleKeys);
	return object;
}

static void HandleOSK(Shell_Object * sender) {
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
						//SendSystemMessage(MS_KEYBOARD, 0, 0, NULL, _shell_task);
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
