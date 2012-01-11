#ifndef _YYGDRASIL_H
#include "defs.h"
#include "midgard.h"
#include "ucos_ii.h"
#include "ioman.h"
//#define MEM_HEAP_SIZE	(1024*1024*12)

#define ENQUEUE_SIZE	24

#define RGB(r,g,b)   	(uint16)( (r << 11) + (g << 5) + b )
#define FROM_BCD(n)		((((n) >> 4) * 10) + ((n) & 0xf))
#define TO_BCD(n)		((((n) / 10) << 4) | ((n) % 10))

//primary color definition
#define C_MAGENTA 		0xf81f
#define C_YELLOW 		RGB(0xff,0xff,0x00)
#define C_CYAN 			RGB(0x00,0xff,0xff)
#define C_RED 			RGB(0xff,0x00,0x00)
#define C_GREEN 		RGB(0x00,0xff,0x00)
#define C_BLUE 			RGB(0x00,0x00,0xff)
#define C_WHITE			0xffff

//Messages properties
//message type declaration
#define M_INPUT_MSG		5
#define M_SYSTEM_MSG	7

//input message state declaration
#define TS_PRESS 	9		//touchscreen
#define TS_RELEASE 	10		//touchscreen
#define TS_MOVE		11		//touchscreen
#define KI_PRESS	19		//keyboard
#define KI_RELEASE	20		//keyboard

//system message param declaration
#define MS_HIDE			27
#define MS_SHOW			28
#define MS_MINIMIZE		41
#define MS_MAXIMIZE		42
#define MS_REFRESH		67
#define MS_TOP			36
#define MS_DESTROY		4
#define MS_REMOVE		6
#define MS_DRAG			123
#define MS_DROP			120

#define REQCNT 30
#define ADCPRS 9	//YH 0627
#define LOOP 1

//NONE = didn't render object
//STREAM = stream object with memcopy
//KEY = render with keying
//ALPHA = render with opacity
//ALPHAKEY = render with opacity and keying
//FORCE = force object to render on creation
enum RenderType { NONE = 1, STREAM = 2, KEY = 4, ALPHA = 8, ALPHAKEY=12, FORCE = 16 };

typedef struct TouchPoint TouchPoint;
typedef struct Graphic_Property Graphic_Property;
typedef struct Graphic Graphic;
typedef struct Input_Message Input_Message;
typedef struct System_Message System_Message;
typedef struct Message Message;
typedef struct Event_Handler Event_Handler;
typedef struct Shell_Object Shell_Object;
typedef struct Task_Properties Task_Properties;

typedef struct VKButton_Properties VKButton_Properties;
typedef struct OS_Configuration OS_Configuration;

struct TouchPoint {
	uchar state;
	struct Point point;
};

//shell standard bitmap data type
struct Graphic_Property {
	uchar alpha;		//alpha blending
	uint16 key;			//transparency key
	uint16 width;		//image width
	uint16 height;		//image height
	uchar * c_buffer;	//character buffer array
	struct Size c_size;		//character buffer size
	struct Point c_pointer;	//character buffer pointer
};

struct Graphic {
	enum RenderType type;
	struct Graphic_Property property;
	uint16 * allocated_image;		//RGB565	
	uint16 * image;	//this will hold pointer to the allocated image
};

struct Message {
	uchar type;
	uchar param;
	struct Message * next;
};

struct Input_Message {
	uchar type;
	uchar param;
	struct Input_Message * next;
	int16 x;
	int16 y;
};

struct System_Message {
	uchar type;
	uchar param;
	struct System_Message * next;
	int16 x;
	int16 y;
	void * buffer;
};

struct Event_Handler {
	uchar type;
	struct Event_Handler * next;
	void (* handle)(struct Shell_Object *sender);
};

//shell standard object
struct Shell_Object {
	uchar visible;
	char * text;
	struct Point cursor;
	struct Rect rect;
	struct Graphic * graphic;
	void (* handle)(struct Shell_Object *sender);
	struct Message * msg;
	struct Shell_Object * parent;
	struct Shell_Object * sibling;
	struct Shell_Object * child;
	struct Heap_Manager * heap;
	struct Event_Handler * events;
	void * properties;
};

struct VKButton_Properties {
	Shell_Object * keyboard;
};

struct Task_Properties {
	Shell_Object * frame;	
};

struct OS_Configuration {
	//shell configuration/management
	Shell_Object * _shell_root;
	Shell_Object * _shell_slider;
	Shell_Object * _shell_task;
	uchar * _shell_background;
	//display configuration/management
	uchar _force_refresh;
	Heap_Manager * _display_heap;
	Shell_Object * _top_object;
	Shell_Object * __top_object;
	Shell_Object * _display_stack[128];
	uchar _disp_stack_index;
	Shell_Object * _shell_keyboard;
	//touch task configuration/management
	TouchPoint _prevTouch;
	Shell_Object * _selected_object;
	Shell_Object * _hover_object;
	Shell_Object * _focused_object;
	//file system configuration/management
	HW_Interface _fm_drives[256];
};

extern OS_Configuration _osc;
//shell standard message creation
void SendInputMessage(uchar state, int16 x, int16 y, Shell_Object * obj);
void SendSystemMessage(uchar param, int16 x, int16 y, uchar * buffer, Shell_Object * obj);
void SendMessage(Shell_Object * obj, Message * message);
//create standard graphic
Graphic * CreateGraphic(enum RenderType type, uint16 * image, 
			Graphic_Property * bmp, Heap_Manager * heap);
//create standard Shell_Object
Shell_Object * CreateObject(uchar visible, enum RenderType type, uint32 heapsize, char * text,
			Rect * r, uchar * image, Graphic_Property * bmp, Shell_Object *parent, 
			void (* handle)(Shell_Object *sender));
//create task icon for task dock
void CreateTaskIcon(Shell_Object * frame);
//create window frame based of Shell_Object (CreateObject)
Shell_Object * CreateFrame(char * name, uint16 * background);
//create standard application launcher for slider
Shell_Object * CreateLauncher(int16 x, int16 y, char * name, uint16 * image, void (* handle)(Shell_Object * sender));
//create standard button
Shell_Object * CreateButton(int16 x, int16 y, char * text, Shell_Object * parent);
//create standard message box
Shell_Object * CreateMsgBox(Shell_Object * sender, char * text);

void InstallEvent(uchar type, Shell_Object * obj, void (* handle)(struct Shell_Object *sender));
void DeleteEvent(Shell_Object * obj, void (* handle)(struct Shell_Object *sender));
void UpdateTaskIcon(Graphic * dst, Graphic * src);
			
void InitShell(void);
void ShellTask(void *pdata);
void TouchTask(void *pdata);
void HostTask(void * pdata);
void MessageTask(void *pdata);
void FullScreenRender(uint16 * buffer, Shell_Object * top_object);
void RenderToTile(uint16 ysize_tile, uint16 y_size, uint16 * buffer, uint16 * img);
void DestroyObject(Shell_Object * obj);

Shell_Object * CreateKeyboard(Shell_Object * parent);
Shell_Object * CreateSampleTextBox(void);

#define _YYGDRASIL_H
#endif