/* radare - LGPL - Copyright 2009 pancake<nopcode.org> */

#include <r_bp.h>

int r_bp_init(struct r_bp_t *bp)
{
}

struct r_bp_t *r_bp_new()
{
	struct r_bp_t *bp = MALLOC_STRUCT(struct r_bpt);
	r_bp_init(bp);
	return bp;
}

//int r_bp_add(struct r_bp_t *bp)
