#ifndef _DEFS_DEFINED
#define _DMA_DEBUG 		0
//#define Try 	try
//#define Catch_anonymous catch
//#define Throw	throw
//#define Catch 	catch
//heap analysis for debugging
#if _DMA_DEBUG
#define	HEAP_CALC	 	1	 /* Should be made 0 to turn OFF debugging */
#endif

#define FS_TYPE						'A'
#define FS_UNFORMATTED 				3
#define FS_DESCRIPTION				"Asgard File System - ©2010 Agus Purwanto"	//size=40
#define FS_DESCRIPTION_SIZE			40
#define FILE_NOT_FOUND				0xff03
#define INSUFFICIENT_MEMORY			0xff02
#define FILE_CANNOT_BE_CREATED		0xff04
#define FILE_CRC_ERROR				0xff01
#define SUCCESS 					1
#define CLUSTER_SIZE 				32
#define CRC16_SIZE					2
#define NEXT_POINTER_SIZE			2
#define DATA_SIZE					(CLUSTER_SIZE - (CRC16_SIZE + NEXT_POINTER_SIZE))
#define CRC_SIZE					(DATA_SIZE + CRC16_SIZE)
#define QUEUE_SIZE 					16	//for BFS dan DFS
#define TRUE						1
#define FALSE						0

#define IOMAN_ATR					'\x3B','\x9C','\x94','\x00','\x68','\x86','\x8D','\x0C','\x86','\x98','\x02','\x44','\xC1','\x00','\x05','\x00'
#define IOMAN_ATR_SIZE				16

#define ACCESS_READ					

#define ALLOCATION_TABLE_OFFSET 	4
#define ALLOCATION_TABLE_SIZE 		508
#define ALLOCATION_DATA_OFFSET 		(ALLOCATION_TABLE_OFFSET + ALLOCATION_TABLE_SIZE)

#define EF_NULL						0xFF
#define EF_TRANSPARENT 				0x00
#define EF_LINIER					0x01
#define EF_CYCLIC					0x03
#define EF_EXECUTE					0x10
#define EF_WORKING					0x20
#define EF_INTERNAL					0x40

#define T_RFU						0x00
#define T_MF						0x01
#define T_DF						0x02
#define T_EF						0x04
#define T_CHV						0x08

#define DF_RESPONSE_SIZE			0x17
#define EF_RESPONSE_SIZE			0x0F

#define APDU_SUCCESS				0x9000
#define APDU_MEMORY_PROBLEM			0x9240
#define APDU_NO_EF_SELECTED			0x9400
#define APDU_FILE_NOT_FOUND			0x9404
#define APDU_OUT_OF_RANGE			0x9402
#define APDU_FILE_INCONSISTENT		0x9408
#define APDU_NO_CHV_INIT			0x9802
#define APDU_ACCESS_DENIED			0x9804
#define APDU_INCONTRADICTION_W_CHV	0x9808
#define APDU_INVALID_STATUS			0x9810
#define APDU_CHV_LAST_ATTEMPT		0x9840
#define APDU_MAX_VALUE_REACHED		0x9850
#define APDU_SUCCESS_RESPONSE		0x9F00
#define APDU_COMMAND_INVALID		0x6981
#define APDU_INCONSISTENT_PARAMETER 0x6A87
#define APDU_DATA_NOT_FOUND			0x6a88
#define APDU_WRONG_PARAMETER		0x6B00
#define APDU_INSTRUCTION_INVALID	0x6D00
#define APDU_CLASS_INVALID			0x6E00
#define APDU_FATAL_ERROR			0x6F00
#define APDU_CRC_ERROR				0x6581			//memory failure
#define APDU_WRONG_LENGTH			0x6700			

#define ACC_ALW						0
#define ACC_CHV1					1
#define ACC_CHV2					2
#define ACC_RFU						3
#define ACC_ADM1					4
#define ACC_ADM2					5
#define ACC_ADM3					6
#define ACC_ADM4					7
#define ACC_ADM5					8
#define ACC_ADM6					9
#define ACC_NVR						15

#define CHV_ALWAYS					0x00
#define CHV_ENABLE					1
#define CHV_DISABLE					2
#define CHV_BLOCK					4
#define CHV_UNBLOCK					8
#define CHV_NEVER					0x10
#define CHV_VERIFIED				0x80
#define CHV_PUK_FAILED				0x20

#define STAT_INVALID				0x00
#define STAT_VALID					0x01

#define OBJECT_MAX_TEXT_LENGTH		64

#ifndef NULL	//cek apakah NULL sudah pernah didefine sebelummnya
#define NULL						0
#endif

#define TRUE 	1   
#define FALSE 	0
#define OK		1
#define FAIL	0

#define ESC_KEY	('q')	// 0x1b
//primitive data type definition
typedef int int32;
typedef long eint32;
typedef unsigned int uint32;
typedef unsigned long euint32;
typedef unsigned long ulong;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned short euint16;
typedef unsigned short uint;
typedef unsigned char uchar;
typedef char int8;
typedef unsigned char uint8;
typedef const unsigned char uint8_t;	//taruh di code program
typedef const unsigned short uint16_t;	//taruh di code program
typedef const unsigned int uint32_t;
typedef char eint8;
typedef signed char esint8;
typedef unsigned char euint8;
typedef short eint16;
typedef signed short esint16;
typedef unsigned short euint16; 
typedef long eint32; 
typedef signed long esint32;
typedef unsigned long euint32; 
typedef float          	fp32;                    // µ¥¾«¶È¸¡µãÊý(32Î»³¤¶È)
typedef double         	fp64;                    // Ë«¾«¶È¸¡µãÊý(64Î»³¤¶È)
typedef int 			mode_t;
//custom data type definition
struct Rect {
	int16 x;
	int16 y;
	uint16 width;
	uint16 height;
};
typedef struct Rect Rect;

struct Point {
	int16 x;
	int16 y;
};
typedef struct Point Point;

struct Size {
	uint16 width;
	uint16 height;
};
typedef struct Size Size;

#define _LARGEFILE64_SOURCE 0
#define _FILE_OFFSET_BITS	0
#define __GNUC__			0
#define __GNUC_MINOR__		0

//Use Midgard MM for memory allocation to prevent collision
//#define malloc				m_alloc
//#define free				m_free
#define _DEFS_DEFINED
#endif
