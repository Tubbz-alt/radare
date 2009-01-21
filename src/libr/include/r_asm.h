/* radare - LGPL - Copyright 2009 nibble<.ds@gmail.com> */

#ifndef _INCLUDE_R_ASM_H_
#define _INCLUDE_R_ASM_H_

enum {
	R_ASM_ARCH_NULL  = 0,
	R_ASM_ARCH_X86   = 1,
	R_ASM_ARCH_ARM   = 2,
	R_ASM_ARCH_PPC   = 3,
	R_ASM_ARCH_M68K  = 4,
	R_ASM_ARCH_JAVA  = 5,
	R_ASM_ARCH_MIPS  = 6,
	R_ASM_ARCH_SPARC = 7,
	R_ASM_ARCH_CSR   = 8,
	R_ASM_ARCH_MSIL  = 9,
	R_ASM_ARCH_OBJD  = 10,
	R_ASM_ARCH_BF    = 11,
};

enum {
	R_ASM_SYN_NULL   = 0,
	R_ASM_SYN_INTEL  = 1,
	R_ASM_SYN_ATT    = 2,
	R_ASM_SYN_OLLY   = 3,
	R_ASM_SYN_PSEUDO = 4
};

struct r_asm_t {
	u32 arch;
	u32 bits;
	u32 big_endian;
	u32 syntax;
	u64 pc;
	char buf_asm[255];
	char buf_hex[255];
	u32 (*r_asm_disasm)(struct r_asm_t *a, u8 *buf, u32 len);
};

/* asm.c */
int r_asm_init(struct r_asm_t *a);
struct r_asm_t *r_asm_new();
void r_asm_free(struct r_asm_t *a);
int r_asm_set_arch(struct r_asm_t *a, u32 arch);
int r_asm_set_bits(struct r_asm_t *a, u32 bits);
int r_asm_set_big_endian(struct r_asm_t *a, u32 boolean);
int r_asm_set_syntax(struct r_asm_t *a, u32 syntax);
int r_asm_set_pc(struct r_asm_t *a, u64 pc);
u32 r_asm_disasm(struct r_asm_t *a, u8 *buf, u32 len);

/* arch/x86/asm.c */
u32 r_asm_x86_disasm(struct r_asm_t *a, u8 *buf, u32 len);

/* arch/arm/asm.c */
u32 r_asm_arm_disasm(struct r_asm_t *a, u8 *buf, u32 len);
#endif