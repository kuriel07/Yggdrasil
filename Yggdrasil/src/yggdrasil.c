#include "defs.h"
#include "config.h"
#include "app_cfg.h"
#include "yggdrasil.h"
#include "yggdrasil_fm.h"
#include "yggdrasil_pv.h"
#include "midgard.h"
#include "lcd.h"
#include "resources.h"
#include "hcd.h"
#include "efat.h"
#include "sd.h"

//operating system configuration
OS_Configuration _osc;

//========================================================================//
OS_STK	ShellTaskStk[ShellTaskStkLength]; 		// Define ShellTaskStack	
OS_STK	TouchTaskStk[TouchTaskStkLength];       // Define TouchTaskStack 
OS_STK	HostTaskStk[HostTaskStkLength];

void DisplayStackPush(Shell_Object * object)
{
	if(_osc._disp_stack_index < 128)
		_osc._display_stack[_osc._disp_stack_index++] = object;
}

Shell_Object * DisplayStackPop(void)
{
	if(_osc._disp_stack_index > 1)
		return _osc._display_stack[--_osc._disp_stack_index];
	return NULL;
}

#include "yggdrasil_handles.c"
#include "yggdrasil_messages.c"
#include "yggdrasil_events.c"
#include "yggdrasil_keyboard.c"
#include "yggdrasil_components.c"
#include "yggdrasil_task.c"

//TODO:
//destroy object (also garbage collector), masih bug??
//kemungkinan ada heap yang belum dibebaskan atau heap 
//tidak bisa dibebaskan karena tidak menggunakan heap manager
//sesuai object(kemungkinan pada grandchild)
// * fixed destroy object bug on grandchild that can cause leakage (April 13 2011)
void DestroyObject(Shell_Object * obj)
{
	#ifndef QUEUE_SIZE
	#define QUEUE_SIZE ENQUEUE_SIZE
	#endif
	uint16 id= 0;
	uint16 stack_index=1;
	Shell_Object * currentObj;
	Shell_Object * temp;
	Shell_Object * node_stack[QUEUE_SIZE];	//nodes queue, maksimum hanya menyimpan 5 child
	//Heap_Manager * heap = obj->heap;
	currentObj = obj->child;	
	get_next_child:
	if(currentObj == NULL) {
		//printf("destroy finished\n");
		goto object_destroy_finish;
	}
	//printf("delete object : %d\n", currentObj);
	temp = currentObj;
	if(currentObj->child != NULL) {
		//jangan dihapus dulu, hapus childs2 nya terlebih dahulu
		node_stack[stack_index++] = currentObj;
		//set ke child
		currentObj = currentObj->child;
		//ketika kembali child sudah dihapus, jadi set child = NULL
		temp->child = NULL;
		//deleting heap also deleting all messages
		goto get_next_child;
	}
	if(currentObj->sibling != NULL) {
		//enqueue sibling, if this node have sibling
		node_stack[stack_index++] = currentObj->sibling;
	}
		//dequeue (DFS)
	if(stack_index == 1) {
		//deleting heap also deleting all messages
		m_delete_heap(temp->heap);		//delete current object heap manager
		m_heap_free(temp->parent->heap, temp);		//before goto previous sibling, delete last child
		goto object_destroy_finish;
	}
	//deleting heap also deleting all messages
	m_delete_heap(temp->heap);		//delete current object heap manager
	m_heap_free(temp->parent->heap, temp);		//before goto previous sibling, delete last child
	currentObj = node_stack[--stack_index];
	goto get_next_child;
	object_destroy_finish:
	//destroy heap manager
	m_delete_heap(obj->heap);
	//last delete root object
	if(obj != _osc._shell_root) {	//check if object = _shell_root
		if(obj->parent != NULL) {
			m_heap_free(obj->parent->heap, obj);
		} else {
			m_free(obj);
		}
	}
	//Uart_Printf("total heap : %d, used heap : %d\n", _total_heap, _used_heap);
}

Graphic * CreateGraphic(enum RenderType type, uint16 * image, Graphic_Property * bmp, Heap_Manager * heap)
{
	uint32 size_alloc = (bmp->width * bmp->height * sizeof(uint16));
	Graphic * btmp;
	btmp = m_heap_alloc(heap, sizeof(Graphic));
	btmp->type = type;
	btmp->property.alpha = bmp->alpha;
	btmp->property.key = bmp->key;
	btmp->property.width = bmp->width;
	btmp->property.height = bmp->height;
	btmp->property.c_size.width = bmp->c_size.width;
	btmp->property.c_size.height = bmp->c_size.height;
	btmp->property.c_pointer.x = 0;
	btmp->property.c_pointer.y = 0;
	if(bmp->c_size.width != 0 && bmp->c_size.height != 0) {
		bmp->c_buffer = m_heap_alloc(heap, bmp->c_size.width * bmp->c_size.height);
	}
	btmp->property.c_buffer = bmp->c_buffer;
	//allocate new memory for image
	btmp->allocated_image = m_heap_alloc(heap, size_alloc);
	//copy image
	if(image != NULL) {
		m_memcopy(btmp->allocated_image, image, size_alloc);
	}
	//for rendering purpose
	btmp->image = btmp->allocated_image;
	return btmp;
}

//shell standard object creation
Shell_Object * CreateObject(uchar visible, enum RenderType type, uint32 heapsize, char * text,
			Rect * r, uchar * image, Graphic_Property * bmp, Shell_Object *parent, 
			void (* handle)(Shell_Object *sender))
{
	Shell_Object * object;
	Shell_Object * obj_parent;
	Shell_Object * obj_sibling;
	//OS_STK * stack;
	if(parent == NULL) {
		//no parent, use standard allocation
		object = m_alloc(sizeof(Shell_Object));
	} else {
		//with parent, use parent heap allocation
		object = m_heap_alloc(parent->heap, sizeof(Shell_Object));
		//Uart_Printf("parent heap used\n");
	}
	object->visible = visible;
	object->cursor.x = 0;
	object->cursor.y = 0;
	object->rect.x = r->x;
	object->rect.y = r->y;
	object->rect.width = r->width;
	object->rect.height = r->height;
	object->heap = m_create_heap(heapsize);
	object->text = text;
	//Uart_Printf("object heap created\n");
	/*if(text != NULL)
	{
		//alocate new heap to store text
		object->text = m_heap_alloc(object->heap, OBJECT_MAX_TEXT_LENGTH);
		m_memcopy(object->text, text, OBJECT_MAX_TEXT_LENGTH);
		//object->text = text;
	}*/
	object->graphic = CreateGraphic(type, image, bmp, object->heap);
	object->handle = handle;	//handle to the specified object
	object->msg = NULL;
	object->parent = parent;
	object->sibling = NULL;
	object->child = NULL;
	object->events = NULL;
	object->properties = NULL;	//properties selalu NULL (reserve), each objects have different properties
	if(parent != NULL) {
		//if rendertype == FORCE, force render to parent
		if(type == FORCE) {
			//consider using alphakey
			Buffer_Paint_Bmp_Key_Var(
				object->rect.x, 
				object->rect.y, 
				object->rect.width, 
				object->rect.height, 
				parent->graphic->property.width, 
				parent->graphic->property.height, 
				(uint16 *)parent->graphic->image, 
				object->graphic);
		}
		obj_parent = parent;
		obj_sibling = obj_parent->child;
		if(obj_sibling != NULL) {
			while(obj_sibling->sibling != NULL)
			{
				obj_sibling = obj_sibling->sibling;
			}
			obj_sibling->sibling = object;
		}
		else
		{
			obj_parent->child = object;
		}
	}
	return object;
}

//downsampling image (interpolated, optimized for 565)
void UpdateTaskIcon(Graphic * dst, Graphic * src) {
	uint16 i, j;
	uint16 i_k, j_k;
	uint16 pixel1, pixel2, pixel3, pixel4;
	uint32 offset, offset2;
	for(j=2; j<(dst->property.height - 2); j++) {
		for(i=2; i<(dst->property.width - 2); i++) {
			j_k = (src->property.height * j) / dst->property.height;
			i_k = (src->property.width  * i) / dst->property.width;
			offset = (j_k * src->property.width) + i_k;
			offset2 = ((j_k + 1) * src->property.width) + i_k;
			pixel1 = *(uint16 *)((uint32)src->allocated_image + ((offset) * sizeof(uint16)));
			pixel2 = *(uint16 *)((uint32)src->allocated_image + ((offset + 1) * sizeof(uint16)));
			pixel3 = *(uint16 *)((uint32)src->allocated_image + ((offset2) * sizeof(uint16)));
			pixel4 = *(uint16 *)((uint32)src->allocated_image + ((offset2 + 1) * sizeof(uint16)));
			pixel1 = (pixel1>>2) & 0x39e7;
			pixel2 = (pixel2>>2) & 0x39e7;
			pixel3 = (pixel3>>2) & 0x39e7;
			pixel4 = (pixel4>>2) & 0x39e7;
			*(uint16 *)((uint32)dst->allocated_image + (((j * dst->property.width) + i) * sizeof(uint16))) = (pixel1 + pixel2 + pixel3 + pixel4);
		}
	}
}

//shell standard frame creation
Shell_Object * CreateFrame(char * name, uint16 * background) {
	//frame creation
	Rect r;
	Graphic_Property bmp;
	Shell_Object * parent;
	Shell_Object * frame;
	Shell_Object * obj;
	Shell_Object * temp;
	Shell_Object * task;
	VKButton_Properties * prop;
	//#ifdef OS_uCOS_II_H
	//OS_ENTER_CRITICAL();
	//#endif
	r.x = 0;
	r.y = 0;
	r.width = 240;
	r.height = 320;
	bmp.key = C_MAGENTA;
	bmp.width = 240;
	bmp.height = 320;
	bmp.c_buffer = NULL;
	parent = CreateObject( TRUE, STREAM, 8192 + (sizeof(uint16) * bmp.width * bmp.height), name, &r,
			background, &bmp, NULL, HandleFrame);
	Buffer_PrintTextVar( 15, 7, 3,  C_WHITE, r.width, r.height, parent->graphic->image, "%s\n", name);
	//close button creation
	r.x = 216;
	r.y = 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_buffer = NULL;
	temp = CreateObject( TRUE, FORCE, 4096  + (sizeof(uint16) * bmp.width * bmp.height), NULL, &r,
			close_button, &bmp, parent, HandleCloseButton);
	//hide button creation
	r.x = 196;
	r.y = 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_buffer = NULL;
	temp = CreateObject( TRUE, FORCE, 4096  + (sizeof(uint16) * bmp.width * bmp.height), NULL, &r,
			hide_button, &bmp, parent, HandleHideButton);
	//hide button creation
	r.x = 176;
	r.y = 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_buffer = NULL;
	temp = CreateObject( TRUE, FORCE, 4096  + (sizeof(uint16) * bmp.width * bmp.height), NULL, &r,
			vk_button, &bmp, parent, HandleVKButton);
	//allocate memories for properties
	temp->properties = m_heap_alloc(temp->heap, sizeof(VKButton_Properties));
	prop = temp->properties;
	prop->keyboard = NULL;
	//show task creation
	r.x = 156;
	r.y = 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_buffer = NULL;
	temp = CreateObject( TRUE, FORCE, 4096  + (sizeof(uint16) * bmp.width * bmp.height), NULL, &r,
			task_button, &bmp, parent, HandleShowTask);
	CreateTaskIcon(parent);
	//Uart_Printf("%d %d\n", task_prop->frame, parent);
	//set top
	_osc._top_object = parent;
	//user panel creation
	return parent;	//return panel
}

Shell_Object * CreateMsgBox(Shell_Object * sender, char * text) {
	//frame creation
	Rect r;
	Graphic_Property bmp;
	Shell_Object * parent;
	Shell_Object * _taskbar;
	Shell_Object * obj;
	Shell_Object * temp;
	r.width = 150;
	r.height = 100;
	r.x = (LCD_WIDTH - r.width) / 2;
	r.y = (LCD_HEIGHT - (r.height + 100)) / 2;
	bmp.key = C_MAGENTA;
	bmp.width = 150;
	bmp.height = 20;
	bmp.c_buffer = NULL;
	obj = CreateObject( TRUE, KEY, 8192 + (sizeof(uint16) * bmp.width * bmp.height), text, &r,
			msgbox, &bmp, sender, HandleMsgBox);
	//render text to current object on object creation
	Buffer_PrintTextVar( 18, 7, (((obj->rect.height - 16) / 2) + 1), C_WHITE, r.width, r.height, obj->graphic->image, "%s\n", text);
	r.x = 56;
	r.y = 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_buffer = NULL;
	temp = CreateObject( TRUE, FORCE, 4096 + (sizeof(uint16) * bmp.width * bmp.height), NULL, &r,
			close_button, &bmp, obj, HandleMsgBoxCloseButton);
	return obj;	//return panel
}

Shell_Object * CreateLauncher(int16 x, int16 y, char * name, uint16 * image, void (* handle)(Shell_Object * sender)) {
	//launcher creation
	Rect r;
	Graphic_Property bmp;
	Shell_Object * obj;
	r.x = x;
	r.y = y;
	r.width = 48;
	r.height = 48;
	bmp.key = C_MAGENTA;
	bmp.width = 48;
	bmp.height = 48;
	bmp.c_buffer = NULL;
	//allocate 16 KB heap for launcher (enough, image only used 4600 byte of heap)
	obj = CreateObject( TRUE, KEY, (16 * 1024), name, &r,
			image, &bmp, _osc._shell_slider, handle);
	//render text on MS_REFRESH (not on object creation)
	return obj;
}

Shell_Object * CreateButton(int16 x, int16 y, char * text, Shell_Object * parent) {
	//button creation
	Rect r;
	Graphic_Property bmp;
	Shell_Object * obj;
	r.x = x;
	r.y = y;
	r.width = 80;
	r.height = 20;
	bmp.key = C_MAGENTA;
	bmp.width = 80;
	bmp.height = 20;
	bmp.c_buffer = NULL;
	obj = CreateObject( TRUE, FORCE, 4096 + (sizeof(uint16) * bmp.width * bmp.height), text, &r,
			button, &bmp, parent, HandleButton);
	Buffer_PrintTextVar( 12, 7, 0, C_WHITE, r.width, r.height, obj->graphic->image, "%s\n", text);
	return obj;
}

//initialize default shell objects such as wallpaper, taskbar, etc
void InitShell(void)
{
	//_root generation, create desktop wallpaper/background
	Rect r;
	Graphic_Property bmp;
	Shell_Object * obj;
	Graphic * bitmap;
	Shell_Object * task;
	Task_Properties * task_prop;
	Shell_Object * dock;
	//initialize operating system configurations
	_osc._shell_background = wallpaper;
	//previous top object for rendering
	_osc.__top_object = NULL;
	//shell keyboard
	_osc._shell_keyboard = NULL;
	//focused object (must be child of _top_object)
 	_osc._focused_object = NULL;
	//refresh state
	_osc._force_refresh = FALSE;
	//heap manager for switching between backbuffer and display buffer
	_osc._disp_stack_index = 0;
	//current selected object
	_osc._selected_object = NULL;
	_osc._hover_object = NULL;
	r.x = 0;
	r.y = 0;
	r.width = 240;
	r.height = 320;
	bmp.key = C_MAGENTA;
	bmp.width = 240;
	bmp.height = 320;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	_osc._shell_task = CreateObject(
			FALSE, 		//visible
			STREAM,		//render type, stream from memory directly
			(1024 * 32) + (sizeof(uint16) * bmp.width * bmp.height),	//heapsize	32KB
			"Yggdratask",
			&r,			//rectangle
			_osc._shell_background,		//background image
			&bmp,		//background properties
			NULL,		//parent
			HandleTaskDock);		//handle
	r.x = 0;
	r.y = 0;
	r.width = 240;
	r.height = 320;
	bmp.key = C_MAGENTA;
	bmp.width = 240;
	bmp.height = 320;
	bmp.c_size.width = 16;
	bmp.c_size.height = 12;
	_osc._shell_root = CreateObject(
			TRUE, 		//visible
			STREAM,		//render type, stream from memory directly
			(1024 * 1024),	//heapsize (768KB heap for backbuffer (307KB) and display buffer (307KB))
			"Yggdrasil",
			&r,			//rectangle
			_osc._shell_background,		//background image
			&bmp,		//background properties
			NULL,		//parent
			HandleRoot);		//handle
	r.x = 0;
	r.y = LCD_HEIGHT - 70;
	r.width = 240;
	r.height = 70;
	bmp.alpha = 70;
	bmp.key = C_MAGENTA;
	bmp.width = 240;
	bmp.height = 70;
	bmp.c_buffer = NULL;
	//render type = NONE, do not render this object on display task
	_osc._shell_slider = CreateObject( TRUE, NONE, 8192 + (sizeof(uint16) * bmp.width * bmp.height), "Ygglider", &r,
			slider, &bmp, _osc._shell_root, HandleSlider);
	r.x = 0;
	r.y = 0;
	r.width = 240;
	r.height = 20;
	bmp.key = C_MAGENTA;
	bmp.width = 240;
	bmp.height = 20;
	bmp.c_buffer = NULL;
	dock = CreateObject( TRUE, STREAM, 8192 + (sizeof(uint16) * bmp.width * bmp.height), "YggdraDock", &r,
			taskdock_img, &bmp, _osc._shell_root, HandleRootDock);
	r.x = 0;
	r.y = 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_buffer = NULL;
	CreateObject( TRUE, FORCE, 8192 + (sizeof(uint16) * bmp.width * bmp.height), "YggdraLogo", &r,
			yggdrasil_btn_ico, &bmp, dock, HandleRootButton);
	r.x = 16;
	r.y = 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_buffer = NULL;
	CreateObject( TRUE, FORCE, 8192 + (sizeof(uint16) * bmp.width * bmp.height), "ShowTask", &r,
			root_task_btn_ico, &bmp, dock, HandleShowTask);
	CreateTaskIcon(_osc._shell_root);							//create root desktop as task
	//refresh _shell_root by sending message to HandleRoot
	CreateLauncher(5, 5, "Media", icon_media, HandleLauncher);
	CreateLauncher(60, 5, "Explorer", explorer_ico, HandleFMLauncher);
	CreateLauncher(115, 5, "Viewer", explorer_ico, HandlePVLauncher);
	_osc._top_object = _osc._shell_root;
	_osc.__top_object = _osc._shell_root;
}

void CreateTaskIcon(Shell_Object * frame) {
	Rect r;
	Graphic_Property bmp;
	Shell_Object * task;
	Task_Properties * task_prop;
	//task object creation
	r.x = 0;
	r.y = 0;
	r.width = 48;
	r.height = 64;
	bmp.key = C_MAGENTA;
	bmp.width = 48;
	bmp.height = 64;
	bmp.c_buffer = NULL;
	task = CreateObject( TRUE, KEY, 8192 + (sizeof(uint16) * bmp.width * bmp.height), frame->text, &r,
			task_ico_frm, &bmp, _osc._shell_task, HandleTaskTop);
	task_prop = m_heap_alloc(task->heap, sizeof(Task_Properties));
	task_prop->frame = frame;
	task->properties = task_prop;
	UpdateTaskIcon(task->graphic, frame->graphic);
}

static Shell_Object * PrevObject(Shell_Object * object)
{
	Shell_Object * obj;
	obj = object->parent->child;
	if(obj != NULL) {
		while(obj->sibling != object) {
			obj = obj->sibling;
		}
	}
	return obj;
}

void FullScreenRender(uint16 * buffer, Shell_Object * top_object) 
{
	#ifndef QUEUE_SIZE
	#define QUEUE_SIZE ENQUEUE_SIZE
	#endif
	int16 x = 0, y = 0;
	uint16 stack_index = 1;
	Shell_Object * currentObj;
	Shell_Object * temp;
	Shell_Object * node_stack[QUEUE_SIZE];	//nodes queue, maksimum hanya menyimpan 5 child
	Shell_Object * prev_top_object = NULL;
	currentObj = top_object;
	//render top object
	switch(currentObj->graphic->type) {
		case STREAM:
			m_memcopy((uint16 *)((uint32)buffer + (currentObj->rect.y * LCD_WIDTH * sizeof(uint16))), currentObj->graphic->image, 
			(currentObj->graphic->property.width * currentObj->graphic->property.height) * sizeof(uint16));
			break;
		case KEY:
			Buffer_Paint_Bmp_Key(
				x + currentObj->rect.x,
				y + currentObj->rect.y,
				currentObj->rect.width,
				currentObj->rect.height,
				buffer,
				currentObj->graphic);
			break;
		case ALPHA:
			Buffer_Paint_Bmp_Alpha(
				x + currentObj->rect.x,
				y + currentObj->rect.y,
				currentObj->rect.width,
				currentObj->rect.height,
				buffer,
				currentObj->graphic);
			break;
		case ALPHAKEY:
			Buffer_Paint_Bmp_AlphaKey(
				x + currentObj->rect.x,
				y + currentObj->rect.y,
				currentObj->rect.width,
				currentObj->rect.height,
				buffer,
				currentObj->graphic);
			break;
		default:
			break;
	}
	currentObj = currentObj->child;	//remember _shell_root didn't have sibling
	get_next_child:
	if(currentObj == NULL) {
		goto shell_paint_finish;
	}
	if(currentObj->sibling != NULL) {
		//enqueue sibling, if this have sibling
		node_stack[stack_index++] = currentObj->sibling;
	}
	if(currentObj == top_object) {
		goto shell_paint_finish;
	}
	
	if(currentObj->visible != FALSE) {
		x = 0;
		y = 0;
		temp = currentObj->parent;
		while(temp != top_object)		//calculate client absolute position
		{
			x += temp->rect.x;
			y += temp->rect.y;
			temp = temp->parent;
		}
		//send message to the corresponding object after drawing
		//each object will respond accordingly to the message
		SendSystemMessage(
			MS_REFRESH, 
			(x + currentObj->rect.x), 
			(y + currentObj->rect.y), 
			buffer, 
			currentObj);
		//paint screen
		switch(currentObj->graphic->type) {
			case KEY:
				Buffer_Paint_Bmp_Key(
					x + currentObj->rect.x,
					y + currentObj->rect.y,
					currentObj->rect.width,
					currentObj->rect.height,
					buffer,
					currentObj->graphic);
				break;
			case ALPHA:
				Buffer_Paint_Bmp_Alpha(
					x + currentObj->rect.x,
					y + currentObj->rect.y,
					currentObj->rect.width,
					currentObj->rect.height,
					buffer,
					currentObj->graphic);
				break;
			case ALPHAKEY:
				Buffer_Paint_Bmp_AlphaKey(
					x + currentObj->rect.x,
					y + currentObj->rect.y,
					currentObj->rect.width,
					currentObj->rect.height,
					buffer,
					currentObj->graphic);
				break;
			case STREAM:
				m_memcopy((uint16 *)((uint32)buffer + ((y + currentObj->rect.y) * LCD_WIDTH * sizeof(uint16))), currentObj->graphic->image, 
				(currentObj->rect.width * currentObj->rect.height) * sizeof(uint16));
				break;
			default:
				break;
		}
		
		//if parent is visible then child also visible
		if(currentObj->child != NULL) {
			currentObj = currentObj->child;
			goto get_next_child;
		}			
	}
	if(stack_index == 1) {
		goto shell_paint_finish;
	}
	//dequeue (DFS)
	currentObj = node_stack[--stack_index];
	goto get_next_child;
	shell_paint_finish:
	return;
}

void ShellTask(void *pdata) //Main Task create taks0 and task1
{
   	uint32 h, w;
	int16 x = 0, y = 0;
	#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
   	OS_CPU_SR  cpu_sr;
   	#endif
	uint16 * lcdbuffer;		//next buffer
	uint16 * lcdcbuffer;	//current buffer
	uint16 * _top_buffer;
	uint16 * __top_buffer;
	//for thumbnail update
	Task_Properties * task_prop;
	Shell_Object * temp;
	Graphic * t_graphic;
	
	OS_ENTER_CRITICAL();
   	Timer0Init();//initial timer0 for ucos time tick
   	ISRInit();   //initial interrupt prio or enable or disable
	
	OS_EXIT_CRITICAL();
	OSStatInit();
	//create new heap for 7 display layer
	_osc._display_heap = m_create_heap((sizeof(uint16) * LCD_WIDTH * LCD_HEIGHT * 7) + 4096);
	//initialize locals storage/IO peripherals
	for(h=0;h<256;h++) {
		_osc._fm_drives[h].id = h;
		_osc._fm_drives[h].rca = 0;
	}
	InitShell();	
	lcdbuffer = m_heap_alloc(_osc._display_heap, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
	//initialize display with _shell_root image
	Buffer_Paint_Bmp_Key(
			_osc._shell_root->rect.x,
			_osc._shell_root->rect.y,
			_osc._shell_root->rect.width,
			_osc._shell_root->rect.height,
			lcdbuffer,
			_osc._shell_root->graphic);	
	
	Lcd_Init(lcdbuffer);
	//lcdcbuffer = lcdbuffer;
	//LcdBkLtSet( 70 ) ;
	
	Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);		//turn on video
    //create touch task for input handler 2nd
    OSTaskCreate (TouchTask,(void *)0, &TouchTaskStk[TouchTaskStkLength - 1], TouchTaskPrio);
    OSTaskCreate (HostTask,(void *)0, &HostTaskStk[HostTaskStkLength - 1], HostTaskPrio);
    //draw _shell_root first before creating any task
    SendSystemMessage(MS_REFRESH, 0, 0, NULL, _osc._shell_root);
    Uart_Printf("Operating System loaded...\n");
    while(1) {
    	//disable kernel
    	OS_ENTER_CRITICAL();
		
		if(_osc._shell_task->visible) {
			//allocate new display buffer (backbuffer)
			lcdbuffer = m_heap_alloc(_osc._display_heap, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
			FullScreenRender(lcdbuffer, _osc._shell_task);
		    //get previous display buffer
			lcdcbuffer = _LCD_BUFFER;
			//set display buffer to backbuffer
			Lcd_Present(lcdbuffer);			//present, set lcd controller to new buffer
			//free previous display buffer
			m_heap_free(_osc._display_heap, lcdcbuffer);
		} else {
			if(_osc.__top_object != _osc._top_object) {
				SendSystemMessage(
						MS_REFRESH, 
						0, 
						0, 
						NULL, 
						_osc._shell_root);
				h = (LCD_HEIGHT * LCD_WIDTH) * sizeof(uint16);
				//allocate memory for object rendering
				_top_buffer = m_heap_alloc(_osc._display_heap, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
				__top_buffer = m_heap_alloc(_osc._display_heap, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
				//render top object first before animate (instead of object->graphic->image)
				FullScreenRender(_top_buffer, _osc._top_object);
				//copy previous display buffer instead of render it
				m_memcopy(__top_buffer, _LCD_BUFFER, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
				//use animation for changing sequence between object
				if(_osc._top_object != _osc._shell_root) {
					for(y = 0; y < LCD_HEIGHT; y += 40) {
						//allocate new display buffer (backbuffer)
						lcdbuffer = m_heap_alloc(_osc._display_heap, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
				    	w = (y * LCD_WIDTH) * sizeof(uint16);
				    	m_memcopy(lcdbuffer, (uint16 *)((uint32)_top_buffer + (h - w)), w);
				    	m_memcopy((uint16 *)((uint32)lcdbuffer + w), (uint16 *)((uint32)__top_buffer + w), ((LCD_HEIGHT - y) * LCD_WIDTH) * sizeof(uint16));		
					    //get previous display buffer
						lcdcbuffer = _LCD_BUFFER;
						//set display buffer to backbuffer
						Lcd_Present(lcdbuffer);			//present, set lcd controller to new buffer
						//free previous display buffer
						m_heap_free(_osc._display_heap, lcdcbuffer);
						Delay(250);
					}
				} else {
					for(y = LCD_HEIGHT; y > 0; y -= 40) {
						//allocate new display buffer (backbuffer)
						lcdbuffer = m_heap_alloc(_osc._display_heap, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
				    	w = (y * LCD_WIDTH) * sizeof(uint16);
				    	m_memcopy(lcdbuffer, (uint16 *)((uint32)__top_buffer + (h - w)), w);
				    	m_memcopy((uint16 *)((uint32)lcdbuffer + w), (uint16 *)((uint32)_top_buffer + w), ((LCD_HEIGHT - y) * LCD_WIDTH) * sizeof(uint16));	
					    //get previous display buffer
						lcdcbuffer = _LCD_BUFFER;
						//set display buffer to backbuffer
						Lcd_Present(lcdbuffer);			//present, set lcd controller to new buffer
						//free previous display buffer
						m_heap_free(_osc._display_heap, lcdcbuffer);
						Delay(250);
					}
				}
				//set __top_object to _top_object
				_osc.__top_object = _osc._top_object;
				//update thumbnail, iterate all task object
				temp = _osc._shell_task->child;
				while(temp != NULL) {
					task_prop = temp->properties;
					if(task_prop->frame == _osc._top_object) {
						t_graphic = m_heap_alloc(_osc._display_heap, sizeof(Graphic));
						//update task icon only need width, height and source image
						t_graphic->property.width = LCD_WIDTH;
						t_graphic->property.height = LCD_HEIGHT;
						t_graphic->allocated_image = _LCD_BUFFER;	//set to current displayed image
						UpdateTaskIcon(temp->graphic, t_graphic);
						m_heap_free(_osc._display_heap, t_graphic);
					}
					temp = temp->sibling;
				}
				//delete temporary heap for object rendering
				m_heap_free(_osc._display_heap, _top_buffer);
				m_heap_free(_osc._display_heap, __top_buffer);
		    } else {
			    //allocate new display buffer (backbuffer)
				lcdbuffer = m_heap_alloc(_osc._display_heap, sizeof(uint16) * LCD_HEIGHT * LCD_WIDTH);
				FullScreenRender(lcdbuffer, _osc._top_object);
			    //get previous display buffer
				lcdcbuffer = _LCD_BUFFER;
				//set display buffer to backbuffer
				Lcd_Present(lcdbuffer);			//present, set lcd controller to new buffer
				//free previous display buffer
				//OS_ENTER_CRITICAL();
				m_heap_free(_osc._display_heap, lcdcbuffer);
			}
		}
		
		//enable kernel
		OS_EXIT_CRITICAL();
		//switch task
    	OSTimeDly(200/8);
    }
}

void InitTouchPanel(void)
{
	rADCDLY = 50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
    rADCCON = (1<<14)|(ADCPRS<<6);    //ADCPRS En, ADCPRS Value
	rADCCON &= 0xfffc;
	//rADCTSC = (1<<3)|(1<<2)|0x00;         //Pull-up disable, Seq. X,Y postion measure.
	rADCTSC = 0xd3;
	//rADCTSC |= (1<<3);
	rADCTSC = rADCTSC & ~(1<<8); 	// Detect stylus Down interrupt signal.
	_osc._prevTouch.state = TS_RELEASE;
	rINTSUBMSK &= ~(BIT_SUB_TC);
}

TouchPoint GetTouchPoint(void)
{
	TouchPoint tp;
	while(1) {
		if((rSUBSRCPND & (BIT_SUB_TC)) == BIT_SUB_TC) {
			//enable normal sequencing
			rADCTSC &= 0xf0; //disable waiting for interrupt
			rADCTSC |= (1<<3)|(1<<2);
			//clear interrupt pending source
			//rSUBSRCPND &= ~(BIT_SUB_TC);
			//rADCDLY =  40000;                 //Normal conversion mode delay about (1/50M)*40000=0.8ms
			rADCCON |= 0x1;                   //start ADC
    		//clear pending bit
    		rSUBSRCPND |= (BIT_SUB_TC);
			//start ADC for normal sequencing
			while((rADCCON&0x8000) == 0x0000) {
				//OSTimeDly(100);
			}
			while(rADCCON & 0x1) {			//check if Enable_start is low
				//OSTimeDly(100);
			}
			while(!(rADCCON & 0x8000)) {        //check if EC(End of Conversion) flag is high, This line is necessary~!!
				//OSTimeDly(100);
			}
			//cek touch interrupt source
			tp.point.x = 240 - ((uint32)((rADCDAT0&0x3ff) * LCD_WIDTH) / 0x310) + 40;	//x position
    		tp.point.y = ((uint32)((rADCDAT1&0x3ff) * LCD_HEIGHT) / 0x320) - 50;	//y position
    		//disable normal sequencing
    		rADCTSC &= ~(1<<3)|(1<<2);
    		if(_osc._prevTouch.state == TS_RELEASE) {
    			tp.state = TS_PRESS;
    		} else {
    			tp.state = TS_RELEASE;
    			rADCTSC = 0xd3;
				rADCTSC = rADCTSC & ~(1<<8);
    		}
    		//set _prevTouch
    		_osc._prevTouch.point.x = tp.point.x;
    		_osc._prevTouch.point.y = tp.point.y;
    		_osc._prevTouch.state = tp.state;
    		return tp;
		}
		//jika state != release (brarti pernah terjadi interrupt)
		if(_osc._prevTouch.state != TS_RELEASE) {
			//enable normal sequencing for ADC conversing
			rADCTSC &= 0xf0;	//disable waiting for interrupt
			rADCTSC |= (1<<3)|(1<<2);
			//clear interrupt pending source (if there is)
			rSUBSRCPND |= (BIT_SUB_TC);
			//rADCDLY =  40000;                 //Normal conversion mode delay about (1/50M)*40000=0.8ms
			rADCCON |= 0x1;                   //start ADC
			while((rADCCON&0x8000) == 0x0000) {
				//OSTimeDly(100);
			}
			while(rADCCON & 0x1) {			//check if Enable_start is low
				//OSTimeDly(100);
			}
			while(!(rADCCON & 0x8000)) {        //check if EC(End of Conversion) flag is high, This line is necessary~!!
				//OSTimeDly(100);
			}
			//cek touch interrupt source
			tp.point.x = 240 - ((uint32)((rADCDAT0 & 0x3ff) * LCD_WIDTH) / 0x310) + 40;	//x position
    		tp.point.y = ((uint32)((rADCDAT1 & 0x3ff) * LCD_HEIGHT) / 0x320) - 50;	//y position
			//disable normal sequencing
    		rADCTSC &= ~(1<<3)|(1<<2);
			//wait for interrupt
			rADCTSC = 0xd3;					//
			rADCTSC = rADCTSC | (1<<8); 	// Detect stylus up interrupt signal.
			tp.state = TS_MOVE;
			//set _prevTouch
			_osc._prevTouch.point.x = tp.point.x;
    		_osc._prevTouch.point.y = tp.point.y;
    		_osc._prevTouch.state = tp.state;
    		return tp;
		} 
		else {
			OSTimeDly(100/5);
		}
	}
}

void TouchTask(void *pdata) //task for test
{
	#ifndef QUEUE_SIZE
	#define QUEUE_SIZE ENQUEUE_SIZE
	#endif
	uint16 stack_index = 1;
	uint16 h, w;
	int16 x = 0, y = 0;
	Rect r;
	TouchPoint tp;
	Shell_Object * currentObj = _osc._shell_root;
	Shell_Object * temp;
	Shell_Object * node_stack[QUEUE_SIZE];
	Shell_Object * top_object;
	//Message * msg;
	
	InitTouchPanel();
	
	while(1) {
		//poll touch screen
		tp = GetTouchPoint();
		stack_index=1;
		//objects detection (framebuffer)
		//if(tp.state == TS_PRESS) {
		if(_osc._shell_task->visible) {
			top_object = _osc._shell_task;	
		} else {
			top_object = _osc._top_object;
		}
		currentObj = top_object;
    	if(currentObj->visible != FALSE) {
			x = 0;
			y = 0;
			//detect touch
			if(tp.point.x > x && tp.point.x < x + currentObj->rect.width) {
			if(tp.point.y > y && tp.point.y < y + currentObj->rect.height){
					if(tp.state == TS_PRESS) {
						_osc._selected_object = currentObj;	//set selected object
					} else {
						if(currentObj != _osc._selected_object)
						{ _osc._hover_object = currentObj; }
					}
				}
			}
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
		if(currentObj->visible != FALSE) {
			x = 0;
			y = 0;
			temp = currentObj->parent;
			while(temp != top_object)		//calculate client absolute position
			{
				x += temp->rect.x;
				y += temp->rect.y;
				temp = temp->parent;
			}
			
			x += currentObj->rect.x;
			y += currentObj->rect.y;
			if(tp.point.x > x && tp.point.x < x + currentObj->rect.width) {
			if(tp.point.y > y && tp.point.y < y + currentObj->rect.height) {
					if(tp.state == TS_PRESS) {
						_osc._selected_object = currentObj;	//set selected object
					} else {
						if(currentObj != _osc._selected_object)
						{ _osc._hover_object = currentObj; }
					}
					//if parent is visible then child also visible therefore
					//check all childs for touch detection
					if(currentObj->child != NULL) {
						currentObj = currentObj->child;
						goto get_root_next_child;
					}
				}
			}
		}
		if(stack_index == 1) {
			goto shell_root_detect_finish;
		}
		//dequeue (DFS)
		currentObj = node_stack[--stack_index];
		goto get_root_next_child;
    	shell_root_detect_finish:
    	stack_index = 1;
    	//}    	
    	
    	if(_osc._selected_object != _osc._shell_root && _osc._selected_object != NULL) {
    		//send standard input message to selected object
    		if(_osc._hover_object != NULL && _osc._selected_object != NULL) {
    			if(tp.state == TS_MOVE) {
    				SendSystemMessage(MS_DRAG, tp.point.x, tp.point.y, _osc._selected_object, _osc._hover_object);
    			}
    			if(tp.state == TS_RELEASE) {
    				SendSystemMessage(MS_DROP, tp.point.x, tp.point.y, _osc._selected_object, _osc._hover_object);
    			}
    		}
    		SendInputMessage(tp.state, tp.point.x, tp.point.y, _osc._selected_object);
    	}
    	if(tp.state == TS_RELEASE)
    	{
    		//selectedObj = none
    		_osc._selected_object = NULL;
    		_osc._hover_object = NULL;
    	}
    	OSTimeDly(100/5);
	}
}

void HostTask(void *pdata) {
	hcd_stop();					//stop usb clock 
	hcd_init();					//only initialize memory
	hcd_create_host_controller(0x49000000, 0x10000);		//initialize hcd, hcca, bulk, control
	hcd_start();				//start usb by supplying clock to the specified host controller
	while(1) {
		usb_periodic();
		OSTimeDly(100/5);
	}
}

