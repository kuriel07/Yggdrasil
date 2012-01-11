#ifndef _EFS_PORT_H
#include "defs.h"
#include "efat.h"

#define	_FAPPEND	0x0008	/* append (writes guaranteed at the end) */
#define	_FCREAT		0x0200	/* open with file create */
#define	_FTRUNC		0x0400	/* open with truncation */
#define	O_RDONLY	FILE_OPEN_READ	/* +1 == FREAD */
#define	O_WRONLY	FILE_OPEN_WRITE		/* +1 == FWRITE */
#define	O_RDWR		(FILE_OPEN_READ | FILE_OPEN_WRITE)	/* +1 == FREAD|FWRITE */
#define	O_APPEND	FILE_MODE_APPEND
#define	O_CREAT		0
#define	O_TRUNC		0
#define File		file_File

File * open(eint8 *filename, eint8 mode, uint16 dummy);
#define lseek(file, offset, mode)	(efat_Seek(file, offset))
#define read(buffer, size, file)	(efat_Read (size, file, buffer))
#define write(buffer, size, file)	(efat_Write (size, file, buffer))
#define close(file)					(efat_Close(file))


#define _EFS_PORT_H
#endif