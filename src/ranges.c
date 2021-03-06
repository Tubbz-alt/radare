/*
 * Copyright (C) 2008, 2009
 *       pancake <youterm.com>
 *
 * radare is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * radare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with radare; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "main.h"

// TODO: add tags to ranges
//    >> art command

static struct list_head ranges;
static int ranges_is_init = 0;
static int ranges_changed = 0;

int ranges_init()
{
	if (ranges_is_init)
		return 1;
	INIT_LIST_HEAD(&ranges);
	return 0;
}

void ranges_free()
{
	struct list_head *pos;
	struct range_t *h;
	list_for_each(pos, &ranges) {
		h = list_entry(pos, struct range_t, list);
		free(h);
	}
	INIT_LIST_HEAD(&ranges);
}

static int ranges_get_flags()
{
	int flags = 0;
	flags |= config_get("range.traces")?RANGE_TRACES:0;
	flags |= config_get("range.graphs")?RANGE_GRAPHS:0;
	flags |= config_get("range.functions")?RANGE_FUNCTIONS:0;
	return flags;
}

static void ranges_abs(ut64 *f, ut64 *t)
{
	if (*f>*t) {
		ut64 tmp = *f;
		*f = *t;
		*t = tmp;
	}
}

struct range_t *ranges_get(struct list_head *rang, ut64 addr)
{
	struct range_t *r;
	struct list_head *pos;

	if (rang == NULL)
		rang = &ranges;

	list_for_each(pos, rang) {
		r = list_entry(pos, struct range_t, list);
		if (addr >= r->from && addr < r->to)
			return r;
	}

	return NULL;
}

ut64 ranges_size(struct list_head *rang)
{
	struct list_head *pos;
	struct range_t *r;
	ut64 sum = 0;

	if (rang == NULL)
		rang = &ranges;

	list_for_each(pos, rang) {
		r = list_entry(pos, struct range_t, list);
		sum += r->to - r->from;
	}

	return sum;
}

void (*ranges_new_callback)(struct range_t *r) = NULL;

/* TODO: merge with ranges_init */
struct list_head *ranges_new(const char *string)
{
	ut64 addr, addr2;
	struct list_head *rgs;
	int i, len = strlen(string)+1;
	char *str = alloca(len);
	char *p = str;
	char *p2 = NULL;
	struct range_t *r;

	memcpy(str, string, len);

	rgs = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(rgs);

	for(i=0;i<len;i++) {
		switch(str[i]) {
		case '-':
			str[i]='\0';
			p2 = p;
			p = str+i+1;
			break;
		case ',':
			str[i]='\0';
			if (p2) {
				addr = get_offset(p);
				addr2 = get_offset(p2);
				//printf("range: %s-%s\n",p2,p);
				r = ranges_add(rgs, addr, addr2, 1);
				if (ranges_new_callback != NULL)
					ranges_new_callback(r);
				p2 = NULL;
			} else {
				addr = get_offset(p);
				//printf("%s\n", p);
				r = ranges_add(rgs, addr, addr+1, 1);
				if (ranges_new_callback != NULL)
					ranges_new_callback(r);
			}
			p = str+i+1;
			str[i]=',';
			break;
		}
	}
	if (p2) {
		addr = get_offset(p);
		addr2 = get_offset(p2);
		//printf("range: %s-%s\n",p2,p);
		r = ranges_add(rgs, addr, addr2, 1);
		if (ranges_new_callback != NULL)
			ranges_new_callback(r);
	} else 
	if (p) {
		addr = get_offset(p);
		//printf("offset: %s\n",p);
		r = ranges_add(rgs, addr, addr+1, 1);
		if (ranges_new_callback != NULL)
			ranges_new_callback(r);
	}

	return rgs;
}

#if 0
    update to      new one     update from   update from/to  ignore

   |______|        |___|           |_____|      |____|      |_______|  range_t
+     |______|   +      |__|   + |___|      + |_________|  +  |__|     from/to
  ------------   -----------   -----------  -------------  -----------
=  |_________|   = |___||__|   = |_______|  = |_________|   |_______|  result
#endif

struct range_t *ranges_add(struct list_head *rang, ut64 from, ut64 to, int rw)
{
	struct list_head *pos;
	struct range_t *r;
	struct range_t *ret;
	int add = 1;

	if (rang == NULL)
		rang = &ranges;

	ranges_abs(&from, &to);

	list_for_each(pos, rang) {
		r = list_entry(pos, struct range_t, list);
		if (r->from == from && r->to==to) {
			add = 0;
		} else
		if (r->from<=from && r->from <= to && r->to>=from && r->to <= to) {
			r->to = to;
			ret = r;
			add = 0;
		} else
		if (r->from>=from && r->from<=to && r->to>=from && r->to >= to) {
			r->from = from;
			ret = r;
			add = 0;
		} else
		if (r->from<=from && r->from<=to && r->to>=from && r->to >= to) {
			/* ignore */
			add = 0;
		} else
		if (r->from>=from && r->from<=to && r->to>=from && r->to <= to) {
			r->from = from;
			r->to = to;
			ret = r;
			add = 0;
		}
	}

	if (rw && add) {
		ret = (struct range_t *)malloc(sizeof(struct range_t));
		ret->from = from;
		ret->to = to;
		ret->datalen = 0;
		ret->data = NULL;
		list_add_tail(&(ret->list), rang);
		ranges_changed = 1;
	}

	return ret;
}

#if 0
    update to      ignore      update from      delete        split

   |______|        |___|           |_____|      |____|       |________|  range_t
-     |______|   -      |__|   - |___|      - |_________|  -    |__|     from/to
  ------------   -----------   -----------  -------------  ------------
=  |__|          =             =     |___|  =                |__|  |__|   result
#endif

int ranges_sub(struct list_head *rang, ut64 from, ut64 to)
{
	struct range_t *r;
	struct list_head *pos;

	if (rang == NULL)
		rang = &ranges;

	ranges_abs(&from, &to);

	__reloop:
	list_for_each(pos, rang) {
		r = list_entry(pos, struct range_t, list);
		/* update to */
		if (r->from<from && r->from < to && r->to>from && r->to < to) {
			r->to = from;
		} else
		/* update from */
		if (r->from>from && r->from<to && r->to>from && r->to>to) {
			r->from = to;
		}
		/* delete */
		if (r->from>from && r->from<to && r->to>from && r->to < to) {
			/* delete */
			list_del(&(r->list));
			ranges_changed = 1;
			goto __reloop;
		}
		/* split */
		if (r->from<from && r->from<to && r->to>from && r->to > to) {
			r->to = from;
			ranges_add(rang, to, r->to, 1);
			goto __reloop;
		}
	}
	return 0;
}

/* TODO: should remove some of them right? */
int ranges_merge(struct list_head *rang)
{
	struct list_head *pos;

	if (rang == NULL)
		rang = &ranges;

	list_for_each(pos, &ranges) {
		struct range_t *r = list_entry(pos, struct range_t, list);
		ranges_add(rang, r->from, r->to, 0);
	}
	return 0;
}

int ranges_is_used(ut64 addr)
{
	struct list_head *pos;
	list_for_each(pos, &ranges) {
		struct range_t *r = list_entry(pos, struct range_t, list);
		if (addr >= r->from && addr <= r->to)
			return 1;
	}
	return 0;
}

int ranges_sort()
{
	struct list_head *pos, *pos2;
	struct list_head *n, *n2;
	int ch=0;

	if (ranges_changed==0)
		return 0;

	list_for_each_safe(pos, n, &ranges) {
		struct range_t *r = list_entry(pos, struct range_t, list);
		list_for_each_safe(pos2, n2, &ranges) {
			struct range_t *r2 = list_entry(pos2, struct range_t, list);
			if ((r != r2) && (r->from > r2->from)) {
				list_move(pos, pos2);
				ch=1;
			}
		}
	}
	return ranges_changed = ch;
}

int ranges_percent()
{
	struct list_head *pos;
	int w, i;
	ut64 seek, step;
	ut64 dif, from = -1, to = -1;

	list_for_each(pos, &ranges) {
		struct range_t *r = list_entry(pos, struct range_t, list);
		if (from == -1) {
			/* init */
			from = r->from;
			to = r->to;
		} else {
			if (from>r->from)
				from = r->from;
			if (to<r->to)
				to = r->to;
		}
	}
	w = 65 ; // columns
	if (from == -1) {
		step = from = to = 0;
	} else {
		dif = to-from;
		if (dif<w) step = 1; // XXX
		else step = dif/w;
		seek = 0;
	}
	seek = 0;
	cons_printf("0x%08llx [", from);
	for(i=0;i<w;i++) {
		if (ranges_is_used(seek))
			cons_strcat("#");
		else cons_strcat(".");
		seek += step;
	}
	cons_printf("] 0x%08llx\n", to);
	return 0;
}

int ranges_cmd(const char *arg)
{
	int flags;
	ut64 from, to;
	char *ptr;
	char *a = alloca(strlen(arg));

	strcpy(a, arg+1);

	switch(*arg) {
	case '?':
		eprintf("Usage: ar[b] [args]\n");
		eprintf("ar                ; show all ranges\n");
		eprintf("ar%%               ; show ranges in a visual percentage\n");
		eprintf("ar*               ; show ranges in radare commands\n");
		eprintf("arm               ; merge ranges (optimize linked list)\n");
		eprintf("arb [[from] [to]] ; boolean ranges against\n");
		eprintf("arb%% [[from] [to]]; boolean ranges against in visual pcent\n");
		eprintf("ar+ [from] [to]   ; add new range\n");
		eprintf("ar- [from] [to]   ; drop range\n");
		eprintf("ar-*              ; reset range tables\n");
		eprintf(" ; range.from     ; default boolean from address\n");
		eprintf(" ; range.to       ; default boolean to address\n");
		eprintf("ari               ; import ranges from:\n");
		eprintf(" ; range.traces   ; (true) join trace information (at?)\n");
		eprintf(" ; range.graphs   ; (true) join graph information (g?)\n");
		eprintf(" ; range.functions; (true) join functions information (CF)\n");
		eprintf(" ; e range.       ; show range config vars\n");
		break;
	case 'i':
		flags = ranges_get_flags();
		if (flags & RANGE_TRACES)
			radare_cmd_raw(".atr*", 0);
		if (flags & RANGE_GRAPHS)
			radare_cmd_raw(".gr*", 0);
		if (flags & RANGE_FUNCTIONS)
			radare_cmd_raw(".CF*", 0);
		break;
	case 'm':
		ranges_merge(NULL);
		break;
	case '+':
		if (*a==' ') a=a+1;
		ptr = strchr(a, ' ');
		if (ptr) {
			ptr[0]='\0';
			from = get_math(a);
			to = get_math(ptr+1);
			ranges_add(NULL, from, to, 1);
		} else {
			eprintf("Usage: ar+ [from] [to]\n");
		}
		break;
	case '%':
		ranges_percent();
		break;
	case '-':
		if (*a==' ') a=a+1;
		if (arg[1]=='*') {
			ranges_free();
		} else {
			ptr = strchr(a, ' ');
			if (ptr) {
				ptr[0]='\0';
				from = get_math(a);
				to = get_math(ptr+1);
				ranges_sub(NULL, from, to);
			} else {
				eprintf("Usage: ar- [from] [to]\n");
			}
		}
		break;
	case 'b': // bolean
		ptr = strchr(a, ' ');
		if (ptr) {
			ptr[0]='\0';
			from = get_math(a);
			to = get_math(ptr+1);
		} else {
			from = config_get_i("range.from");
			to = config_get_i("range.to");
		}
		ranges_boolean(from, to, ranges_get_flags());
		break;
	case '*':
		ranges_list(1);
		break;
	default:
		ranges_list(0);
		break;
	}
	return 0;
}

int ranges_list(int rad)
{
	ut64 total = 0;
	struct list_head *pos;
	ranges_sort();
	list_for_each(pos, &ranges) {
		struct range_t *r = list_entry(pos, struct range_t, list);
		if (rad) cons_printf("ar+ 0x%08llx 0x%08llx\n", r->from, r->to);
		else cons_printf("0x%08llx 0x%08llx ; %lld\n", r->from, r->to, r->to-r->from);
		total += (r->to-r->from);
	}
	eprintf("Total bytes: %lld\n", total);
	return 0;
}

int ranges_get_n(int n, ut64 *from, ut64 *to)
{
	struct list_head *pos;
	int count =0;
	ranges_sort();
	list_for_each(pos, &ranges) {
		struct range_t *r = list_entry(pos, struct range_t, list);
		if (count == n) {
			*from = r->from;
			*to = r->to;
			return 1;
		}
		count++;
	}
	return 0;
}

int ranges_boolean(ut64 from, ut64 to, int flags)
{
	ut64 total = 0;
	struct list_head *pos;
	struct range_t *r = NULL;

	eprintf("; ranges from 0x%08llx to 0x%08llx (flags=%d)\n",
		from, to, flags);
#if 0
     .....|______________________|...
      |_____|  |____|  |_______|
    ---------------------------------
            |__|    |__|       |_|      
#endif

	ranges_sort();

	list_for_each(pos, &ranges) {
		r = list_entry(pos, struct range_t, list);
		if (r->from > from && r->from < to) {
			cons_printf("0x%08llx .. 0x%08llx\n",
				from, r->from);
			total += (r->from - from);
			from = r->to;
		}
	}
	if (from < to) {
		cons_printf("0x%08llx .. 0x%08llx\n",
			from, to);
		total += (to-from);
	}
	// TODO
	// show ranges
	// show total bytes
	// show total booleaned bytes
	eprintf("Total bytes: %lld\n", total);
	return 0;
}
