#include "efat.h"
#include "defs.h"
#include "efs_port.h"
#include "file.h"

static efat_FileSystem * _efs;

File * open(eint8 *filename, eint8 mode, uint16 dummy)
{
	File * file;
	file = file_OpenStatus(filename, _efs, mode);
	return file;
}
