#ifndef _INCLUDE_RABIN_H_
#define _INCLUDE_RABIN_H_

enum {
	FILETYPE_UNK = 0,
	FILETYPE_ELF,
	FILETYPE_MZ,
	FILETYPE_PE,
	FILETYPE_CLASS,
	FILETYPE_DEX,
	FILETYPE_MACHO,
	FILETYPE_CSRFW
};

#define ACTION_UNK      0x0000
#define ACTION_ENTRY    0x0001 
#define ACTION_IMPORTS  0x0002 
#define ACTION_SYMBOLS  0x0004 
#define ACTION_LIBS     0x0008 
//#define ACTION_EXPORTS  0x0010 
#define ACTION_SECTIONS 0x0020 
#define ACTION_CHECKSUM 0x0040 
#define ACTION_BASE     0x0080
#define ACTION_ARCH     0x0100
#define ACTION_FILETYPE 0x0200
//#define ACTION_OTHERS	0x0400
#define ACTION_STRINGS  0x0800
#define ACTION_NOP      0x1000
#define ACTION_INFO     0x2000

#endif
