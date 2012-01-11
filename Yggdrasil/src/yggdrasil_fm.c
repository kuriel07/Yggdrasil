#include "efat.h"
#include "yggdrasil.h"
#include "yggdrasil_fm.h"
#include "yggdrasil_pv.h"
#include "resources.h"
#include "lcd.h"
#include "ioman.h"
#include "sd.h"

//fill memory with specific tile image
void RenderToTile(uint16 ysize_tile, uint16 y_size, uint16 * buffer, uint16 * img) {
	uint16 i;
	if(ysize_tile < y_size ) {
		for(i=0; i<(y_size - ysize_tile); i+= ysize_tile) {
			m_memcopy((void *)((uint32)buffer + (uint32)(i * 240 * sizeof(uint16))), (void *)img, (ysize_tile * 240 * sizeof(uint16)));
		}
	}
	m_memcopy((void *)((uint32)buffer + (uint32)(i * 240 * sizeof(uint16))), (void *)img, ((y_size - i) * 240 * sizeof(uint16)));
}

void HandleFMLauncher(Shell_Object * sender) {
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
						CreateFileManager();
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
		//RaiseEvent();
		sender->msg = msg->next;
		m_heap_free(heap, msg);
	}
}

Shell_Object * CreateScrollArea(efat_Entry * entry, efat_FileSystem * fs, Shell_Object * parent) {
	Rect r;
	uint16 file_count = 0;
	uint16 area_height;
	Graphic_Property bmp;
	Shell_Object * obj;	
	Shell_Object * dirObj;
	r.x = 0;
	r.y = 20;
	r.width = 240;
	r.height = 240;
	bmp.key = C_MAGENTA;
	//set image size according to the calculated value
	bmp.width = 240;
	bmp.height = 240;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	obj = CreateObject( TRUE, NONE, 65536 + (sizeof(uint16) * bmp.width * bmp.height), "AreaScroll", &r,
			NULL, &bmp, parent, HandleScrollArea);
	obj->properties = m_heap_alloc(obj->heap, sizeof(ScrollAreaProperties));
	dirObj = CreateDirectoryList(entry, fs, obj);
	((DirectoryListProperties *)dirObj->properties)->scroll_area = obj;
	((ScrollAreaProperties *)obj->properties)->area = dirObj;
	obj->graphic->image = dirObj->graphic->allocated_image;
	return obj;
}

Shell_Object * CreateDirectoryList(efat_Entry * entry, efat_FileSystem * fs, Shell_Object * parent) {
	Rect r;
	uint16 file_count = 0;
	uint16 area_height;
	Graphic_Property bmp;
	Shell_Object * obj;
	Graphic * bitmap;
	DirectoryListProperties * dl_prop;
	efat_DirList * dirlist;
	//clone dirlist
	dirlist = (efat_DirList *)m_heap_alloc(parent->heap, sizeof(efat_DirList));
	dirlist->entry = (efat_Entry *)m_heap_alloc(parent->heap, sizeof(efat_Entry));
	dirlist->fs = fs;
	dirlist->next = 1;
	dirlist->cluster = entry->cluster;
	//calculate image area size
	while(efat_DirListNext(dirlist)) {
		file_count++;
	}
	area_height = (file_count * 20);
	if(area_height < 240) {
		area_height = 240;
	}
	r.x = 0;
	r.y = 0;
	r.width = 240;
	r.height = 240;
	bmp.key = C_MAGENTA;
	//set image size according to the calculated value
	bmp.width = 240;
	bmp.height = area_height;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	obj = CreateObject( TRUE, STREAM, 65536 + (sizeof(uint16) * bmp.width * bmp.height), "AreaScroll", &r,
			NULL, &bmp, parent, HandleDirectoryList);
	//render tile image on area
	RenderToTile(40, area_height, obj->graphic->allocated_image, fm_tile_img);
	//create DirectoryListProperties
	obj->properties = m_heap_alloc(obj->heap, sizeof(DirectoryListProperties));
	dl_prop = ((DirectoryListProperties *)obj->properties);
	dl_prop->vertical_scrl = 0;
	dl_prop->entry = m_heap_alloc(obj->heap, sizeof(efat_Entry));
	m_memcopy(dl_prop->entry, entry, sizeof(efat_Entry));
	dl_prop->fs = fs;
	//m_memcopy(dlist, dirlist, sizeof(efat_DirList));
	file_count = 0;
	dirlist->next = 1;
	while(efat_DirListNext(dirlist)) {
		CreateFileObject(file_count, dirlist->fs, dirlist->entry, obj);
		file_count++;
	}
	m_heap_free(parent->heap, dirlist->entry);
	m_heap_free(parent->heap, dirlist);
	return obj;
}


Shell_Object * CreateFileObject(uint16 no, efat_FileSystem *fs, efat_Entry * entry, Shell_Object * parent) {
	Rect r;
	Graphic_Property bmp;
	Shell_Object * obj;
	Graphic * bitmap;
	efat_Entry * file_entry;
	r.x = 5;
	r.y = (no * 20) + 2;
	r.width = 16;
	r.height = 16;
	bmp.key = C_MAGENTA;
	bmp.width = 16;
	bmp.height = 16;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	if(entry->attrib & EFAT_DIRECTORY) {		//is directory??
		obj = CreateObject( TRUE, FORCE, 16384 + (sizeof(uint16) * bmp.width * bmp.height), "FileObj", &r,
			folder_icon, &bmp, parent, HandleFileObject);
	} else {									//is file??
		obj = CreateObject( TRUE, FORCE, 16384 + (sizeof(uint16) * bmp.width * bmp.height), "FileObj", &r,
			file_icon, &bmp, parent, HandleFileObject);
	}
	//clone current entry
	obj->properties = m_heap_alloc(obj->heap, sizeof(FileObjectProperties));
	file_entry = m_heap_alloc(obj->heap, sizeof(efat_Entry));
	m_memcopy(file_entry, entry, sizeof(efat_Entry));
	file_entry->cluster = fat_GetDataCluster(entry->cluster, fs);
	if(file_entry->cluster < fs->cluster_begin_lba) {
		file_entry->cluster = fs->cluster_begin_lba;
	}
	((FileObjectProperties *)obj->properties)->entry = file_entry;
	((FileObjectProperties *)obj->properties)->fs = fs;
	//force print text
	Buffer_PrintfColor(30, 0xffff, (r.x + 20), r.y, parent->graphic->allocated_image, "%s\n", entry->lfn);
	//Buffer_PrintTextVar(30, r.x, r.y, C_WHITE, 180, r.height, parent->graphic->allocated_image, "%s\n", entry->lfn);
	return obj;
}

Shell_Object * CreateFileDragObject(int16 x, int16 y, Shell_Object * parent, Shell_Object * clone) {
	Rect r;
	Graphic_Property bmp;
	Shell_Object * obj;
	Graphic * bitmap;
	efat_Entry * file_entry;
	r.x = x;
	r.y = y;
	r.width = clone->rect.width;
	r.height = clone->rect.height;
	bmp.key = C_MAGENTA;
	bmp.width = clone->graphic->property.width;
	bmp.height = clone->graphic->property.height;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	obj = CreateObject( TRUE, KEY, 16384 + (sizeof(uint16) * bmp.width * bmp.height), "DragObj", &r,
		clone->graphic->allocated_image, &bmp, parent, HandleFileDragObject);
	//clone current entry
	obj->properties = m_heap_alloc(obj->heap, sizeof(FileObjectProperties));
	file_entry = m_heap_alloc(obj->heap, sizeof(efat_Entry));
	m_memcopy(file_entry, ((FileObjectProperties *)(clone->properties))->entry, sizeof(efat_Entry));
	((FileObjectProperties *)obj->properties)->entry = file_entry;
	((FileObjectProperties *)obj->properties)->fs = ((FileObjectProperties *)(clone->properties))->fs;
	return obj;
}

Shell_Object * CreateFileManager(void)
{
	Rect r;
	Graphic_Property bmp;
	Shell_Object * parent;
	Shell_Object * obj;
	efat_FileSystem * fs;
	efat_DirList * dirlist;
	Shell_Object * scroll_area;
	//create sd card interface and initialize
	Uart_Printf("initializing sd driver");
	init_sd_card(&_osc._fm_drives[0]);
	Uart_Printf("sd driver initialized\n");
	fs = efat_Init(0, &_osc._fm_drives[0]);
	Uart_Printf("fat fs initialized\n");
	parent = CreateFrame("Explorer", fm_background_img);
	Uart_Printf("frame created\n");
	if(fs != NULL) {
		efat_Entry * entry = m_heap_alloc(parent->heap, sizeof(efat_Entry));
		fat_GetDirEntry(0, fs->cluster_begin_lba, entry, fs);
		entry->cluster = fs->cluster_begin_lba;
		scroll_area = CreateScrollArea(entry, fs, parent);
		m_heap_free(parent->heap, entry);
	}
	//create open
	r.x = 6;
	r.y = 266;
	r.width = 48;
	r.height = 48;
	bmp.key = C_MAGENTA;
	bmp.width = 48;
	bmp.height = 48;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	obj = CreateObject( TRUE, KEY, 16384 + (sizeof(uint16) * bmp.width * bmp.height), "OpenFile", &r,
		fm_open_img, &bmp, parent, HandleFileOpenIcon);
	obj->properties = m_heap_alloc(obj->heap, sizeof(FileActionProperties));
	((FileActionProperties *)(obj->properties))->lister = scroll_area;
	//create copy(magnet)
	r.x = 60;
	r.y = 266;
	r.width = 48;
	r.height = 48;
	bmp.key = C_MAGENTA;
	bmp.width = 48;
	bmp.height = 48;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	obj = CreateObject( TRUE, KEY, 16384 + (sizeof(uint16) * bmp.width * bmp.height), "CopyFile", &r,
		fm_copy_img, &bmp, parent, HandleFileCopyIcon);
	obj->properties = m_heap_alloc(obj->heap, sizeof(FileActionProperties));
	((FileActionProperties *)(obj->properties))->lister = scroll_area;
	//create recycler
	r.x = 186;
	r.y = 266;
	r.width = 48;
	r.height = 48;
	bmp.key = C_MAGENTA;
	bmp.width = 48;
	bmp.height = 48;
	bmp.c_size.width = 0;
	bmp.c_size.height = 0;
	obj = CreateObject( TRUE, KEY, 16384 + (sizeof(uint16) * bmp.width * bmp.height), "DeleteFile", &r,
		fm_recycler_img, &bmp, parent, HandleFileDeleteIcon);
	obj->properties = m_heap_alloc(obj->heap, sizeof(FileActionProperties));
	((FileActionProperties *)(obj->properties))->lister = scroll_area;
	return parent;
}

void HandleDirectoryList(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	int16 i;
	uint16 * backbuffer = NULL;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * currentObj;
	Heap_Manager * heap = sender->heap;
	DirectoryListProperties * dl_prop = (DirectoryListProperties *)sender->properties;
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
						//_top_object = sender;
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_MOVE:
						dl_prop->vertical_scrl -= (imp->y - sender->cursor.y);
						//sender->child->rect.y +=  (imp->y - sender->cursor.y);
						/*currentObj = sender->child;
						while(currentObj != NULL) {
							currentObj->rect.y += (imp->y - sender->cursor.y);
							currentObj = currentObj->sibling;
						}*/
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
						//backbuffer = (uint16 *)smp->buffer;
						//check vertical scroll offset first
						if(dl_prop->vertical_scrl < 0) {
							dl_prop->vertical_scrl = 0;
						}
						if(dl_prop->vertical_scrl > (sender->graphic->property.height - sender->rect.height)) {
							dl_prop->vertical_scrl = (sender->graphic->property.height - sender->rect.height);
						}
						i=0;
						currentObj = sender->child;
						while(currentObj != NULL) {
							currentObj->rect.y = (i - (int16)dl_prop->vertical_scrl);
							i += 20;
							currentObj = currentObj->sibling;
						}
						//change image pointer but didn't change allocated image
						sender->graphic->image = (uint16 *)((uint32)sender->graphic->allocated_image + (dl_prop->vertical_scrl * sender->graphic->property.width * sizeof(uint16)));
						//supposedly change any outside region file_object to invisible
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

void HandleFileObject(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	uint16 * backbuffer = NULL;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * currentObj;
	Shell_Object * selectedObj;
	Heap_Manager * heap = sender->heap;
	FileObjectProperties * fo_prop = (FileObjectProperties *)sender->properties;
	FileObjectProperties * file_prop;
	Shell_Object * lister = sender->parent->parent;
	efat_DirList * list;
	Shell_Object * dragObj;
	int16 x, y;
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
						/*selectedObj = CreateFileDragObject(imp->x - (sender->rect.width / 2), imp->y - (sender->rect.height / 2), sender->parent->parent->parent, sender);
						selectedObj->cursor.x = imp->x;
						selectedObj->cursor.y = imp->y;
						_selected_object = selectedObj;*/
						break;
					case TS_MOVE:
						x = 0;
						y = 0;
						currentObj = sender;
						while(currentObj != _osc._top_object)		//calculate client absolute position
						{
							x += currentObj->rect.x;
							y += currentObj->rect.y;
							currentObj = currentObj->parent;
						}
						currentObj = sender;
						if(imp->x < (x - 5) || imp->x > (x + currentObj->rect.width + 5)) {
							selectedObj = CreateFileDragObject(imp->x - (sender->rect.width / 2), imp->y - (sender->rect.height / 2), sender->parent->parent->parent, sender);
							selectedObj->cursor.x = imp->x;
							selectedObj->cursor.y = imp->y;
							_osc._selected_object = selectedObj;
						}
						if(imp->y < (y - 5) || imp->y > (y + currentObj->rect.height + 5)){
							selectedObj = CreateFileDragObject(imp->x - (sender->rect.width / 2), imp->y - (sender->rect.height / 2), sender->parent->parent->parent, sender);
							selectedObj->cursor.x = imp->x;
							selectedObj->cursor.y = imp->y;
							_osc._selected_object = selectedObj;
						}
						break;
					case TS_RELEASE:
						file_prop = ((FileObjectProperties *)sender->properties);
						if(file_prop->entry->attrib & EFAT_DIRECTORY) {	//cek is directory
							list = (efat_DirList *)m_heap_alloc(heap, sizeof(efat_DirList));
							list->entry = (efat_Entry *)m_heap_alloc(heap, sizeof(efat_Entry));
							list->fs = file_prop->fs;
							list->next = 0;
							list->cluster = file_prop->entry->cluster;
							m_memcopy(list->entry, file_prop->entry, sizeof(efat_Entry));
							while(sender->msg != NULL) {
								sender->msg = msg->next;
								m_heap_free(heap, msg);
							}
							SendSystemMessage(FM_SETENTRY, 0, 0, list, lister);
							return;
							//m_heap_free(heap, list->entry);
							//m_heap_free(heap, list);
						} else {
							//CreatePicViewer("test.png", file_prop->fs);
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
					case MS_DESTROY:
						break;
					case MS_REFRESH:
						break;
					case MS_DROP:
						/*dragObj = (Shell_Object *)smp->buffer;
						file_prop = ((FileObjectProperties *)dragObj->properties);
						if(file_prop->entry->attrib & EFAT_DIRECTORY) {	//cek is directory
							list = (efat_DirList *)m_heap_alloc(heap, sizeof(efat_DirList));
							list->entry = (efat_Entry *)m_heap_alloc(heap, sizeof(efat_Entry));
							list->fs = file_prop->fs;
							list->next = 0;
							list->cluster = file_prop->entry->cluster;
							m_memcopy(list->entry, file_prop->entry, sizeof(efat_Entry));
							while(sender->msg != NULL) {
								sender->msg = msg->next;
								m_heap_free(heap, msg);
							}
							SendSystemMessage(FM_SETENTRY, 0, 0, list, lister);
							return;
							//m_heap_free(heap, list->entry);
							//m_heap_free(heap, list);
						}*/
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

void HandleFileDragObject(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	uint16 * backbuffer = NULL;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * currentObj;
	Heap_Manager * heap = sender->heap;
	FileObjectProperties * fo_prop = (FileObjectProperties *)sender->properties;
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
						//move file drag object
						sender->rect.x += (imp->x - sender->cursor.x);
						sender->rect.y += (imp->y - sender->cursor.y);
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_RELEASE:
						currentObj = sender->parent;
						currentObj = currentObj->child;
						while(currentObj->sibling != sender) {
							currentObj = currentObj->sibling;
						}
						currentObj->sibling = sender->sibling;
						while(sender->msg != NULL) {
							sender->msg = msg->next;
							m_heap_free(heap, msg);
						}
						DestroyObject(sender);
						return;
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
						//printf text on display
						if(smp->buffer != NULL)
							Buffer_PrintText(
								30,
								(smp->x + 20), 
								smp->y, 
								C_WHITE, 
								smp->buffer, "%s\n", ((FileObjectProperties *)sender->properties)->entry->lfn);
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


void HandleScrollArea(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	//uint16 * backbuffer = NULL;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Heap_Manager * heap = sender->heap;
	ScrollAreaProperties * area = ((ScrollAreaProperties *)sender->properties)->area;
	efat_DirList * dlist;
	efat_DirList * dirlist;
	Shell_Object * dirObj = ((ScrollAreaProperties *)sender->properties)->area;
	efat_Entry * entry;
	efat_FileSystem * fs;
	while(sender->msg != NULL)
	{
		msg = sender->msg;
		SendMessage(area, msg);
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
					case FM_REFRESH:
						if(sender->child != NULL) {
							sender->child = NULL;
						}
						dlist = m_heap_alloc(sender->heap, sizeof(efat_DirList));
						dlist->entry = m_heap_alloc(sender->heap, sizeof(fat_DirEntry));
						m_memcopy(dlist->entry, ((DirectoryListProperties *)dirObj->properties)->entry, sizeof(fat_DirEntry));
						dlist->next = 0;
						dlist->fs = ((DirectoryListProperties *)dirObj->properties)->fs;
						while(dirObj->msg != NULL) {
							dirObj->msg = dirObj->msg->next;
						}
						DestroyObject(dirObj);
						dirObj = CreateDirectoryList(dlist->entry, dlist->fs, sender);
						((DirectoryListProperties *)dirObj->properties)->scroll_area = sender;
						((ScrollAreaProperties *)sender->properties)->area = dirObj;
						sender->graphic->image = dirObj->graphic->allocated_image;
						m_heap_free(sender->heap, dlist->entry);
						m_heap_free(sender->heap, dlist);
						break;
					case FM_SETENTRY:		//set current path to list
						dirlist = (efat_DirList *)smp->buffer;
						if(sender->child != NULL) {
							sender->child = NULL;
						}
						DestroyObject(dirObj);
						dlist = m_heap_alloc(sender->heap, sizeof(efat_DirList));
						dlist->entry = m_heap_alloc(sender->heap, sizeof(fat_DirEntry));
						m_memcopy(dlist->entry, dirlist->entry, sizeof(fat_DirEntry));
						Uart_Printf("cluster : %x\n", dlist->entry->cluster);
						dlist->next = 0;
						dlist->fs = dirlist->fs;
						dirObj = CreateDirectoryList(dlist->entry, dlist->fs, sender);
						((DirectoryListProperties *)dirObj->properties)->scroll_area = sender;
						((ScrollAreaProperties *)sender->properties)->area = dirObj;
						sender->graphic->image = dirObj->graphic->allocated_image;
						m_heap_free(sender->heap, dlist->entry);
						m_heap_free(sender->heap, dlist);
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

/****************************************************************************************/
/*																						*/
/*																						*/
/*																						*/
/*																						*/
/****************************************************************************************/

void HandleFileOpenIcon(Shell_Object * sender) {
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * dragObj;
	Heap_Manager * heap = sender->heap;
	FileObjectProperties * file_prop;
	Shell_Object * lister = ((FileActionProperties *)sender->properties)->lister;
	efat_DirList * list;
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
					case MS_DROP:
						dragObj = (Shell_Object *)smp->buffer;
						file_prop = ((FileObjectProperties *)dragObj->properties);
						if(file_prop->entry->attrib & EFAT_DIRECTORY) {	//cek is directory
							list = (efat_DirList *)m_heap_alloc(heap, sizeof(efat_DirList));
							list->entry = (efat_Entry *)m_heap_alloc(heap, sizeof(efat_Entry));
							list->fs = file_prop->fs;
							list->next = 0;
							list->cluster = file_prop->entry->cluster;
							m_memcopy(list->entry, file_prop->entry, sizeof(efat_Entry));
							SendSystemMessage(FM_SETENTRY, 0, 0, list, lister);
							m_heap_free(heap, list->entry);
							m_heap_free(heap, list);
						}
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

void HandleFileCopyIcon(Shell_Object * sender) {
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * dragObj;
	Heap_Manager * heap = sender->heap;
	FileObjectProperties * file_prop;
	Shell_Object * lister = ((FileActionProperties *)sender->properties)->lister;
	efat_DirList * list;
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
					case MS_DROP:
						dragObj = (Shell_Object *)smp->buffer;
						file_prop = (FileObjectProperties *)dragObj->properties;
						if(file_prop->entry->attrib & EFAT_DIRECTORY) {	//cek is directory
							
						}
						dragObj->handle = HandleFileCopiedIcon;
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

void HandleFileDeleteIcon(Shell_Object * sender) {
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * dragObj;
	Heap_Manager * heap = sender->heap;
	FileObjectProperties * file_prop;
	Shell_Object * lister = ((FileActionProperties *)sender->properties)->lister;
	efat_DirList * list;
	efat_Entry * entry;
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
					case MS_DROP:
						dragObj = (Shell_Object *)smp->buffer;
						file_prop = (FileObjectProperties *)dragObj->properties;
						if(file_prop->entry->attrib & EFAT_DIRECTORY) {	//cek is directory
							Uart_Printf("directory!\n");
						} else {
							entry = m_heap_alloc(sender->heap, sizeof(efat_Entry));
							m_memcopy(entry, file_prop->entry, sizeof(efat_Entry));
							fat_RemoveDirEntry(entry, file_prop->fs);
							m_heap_free(sender->heap, entry);
							SendSystemMessage(FM_REFRESH, 0, 0, 0, lister);
						}
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

void HandleFileCopiedIcon(Shell_Object * sender) {
	//do not loop here, this event handler, event handler 
	//activated by sending message to specified object
	uint16 * backbuffer = NULL;
	Message * msg;
	Input_Message * imp;
	System_Message * smp;
	Shell_Object * currentObj;
	Heap_Manager * heap = sender->heap;
	FileObjectProperties * fo_prop = (FileObjectProperties *)sender->properties;
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
						sender->cursor.x = imp->x;
						sender->cursor.y = imp->y;
						break;
					case TS_MOVE:
						//move file drag object
						sender->rect.x += (imp->x - sender->cursor.x);
						sender->rect.y += (imp->y - sender->cursor.y);
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
