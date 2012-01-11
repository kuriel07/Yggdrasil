#include "defs.h"
#include "efat.h"
#include "yggdrasil.h"
#include "ioman.h"
//#include "png.h"
#ifndef _YGGDRASIL_FM_H

//untuk local system message pastikan diatas 0x80
#define FM_SETENTRY		(128 + 9)
#define FM_REFRESH		(128 + 20)

typedef struct DirectoryListProperties DirectoryListProperties;
typedef struct FileObjectProperties FileObjectProperties;
typedef struct FileSystemListProperties FileSystemListProperties;
typedef struct ScrollAreaProperties ScrollAreaProperties;
typedef struct FileActionProperties FileActionProperties;
typedef struct StorageList StorageList;

struct DirectoryListProperties {
	Shell_Object * scroll_area;
	int16 vertical_scrl;
	efat_Entry * entry;		//current entry
	efat_FileSystem * fs;
};

struct FileSystemListProperties {
	Shell_Object * list;
	efat_Entry * entry;
	HW_Interface * card;	//hardware interface
};

struct FileObjectProperties {
	efat_Entry * entry;	
	efat_FileSystem * fs;
};

struct ScrollAreaProperties {
	Shell_Object * area;
};

struct FileActionProperties {
	Shell_Object * lister;
};

struct StorageList {
	HW_Interface * interface;
	StorageList * next;
};

Shell_Object * CreateFileManager(void);
Shell_Object * CreateScrollArea(efat_Entry * entry, efat_FileSystem * fs, Shell_Object * parent);
Shell_Object * CreateFileObject(uint16 no, efat_FileSystem *fs, efat_Entry * entry, Shell_Object * parent); 
Shell_Object * CreateDirectoryList(efat_Entry * entry, efat_FileSystem * fs, Shell_Object * parent);
Shell_Object * CreateFileDragObject(int16 x, int16 y, Shell_Object * parent, Shell_Object * clone);

void HandleDirectoryList(Shell_Object * sender);
void HandleFileObject(Shell_Object * sender);
void HandleFileDragObject(Shell_Object * sender);
void HandleScrollArea(Shell_Object * sender);
void HandleFileOpenIcon(Shell_Object * sender);
void HandleFileCopyIcon(Shell_Object * sender);
void HandleFileDeleteIcon(Shell_Object * sender);
void HandleFileCopiedIcon(Shell_Object * sender);

void HandleFMLauncher(Shell_Object * sender);
#define _YGGDRASIL_FM_H
#endif