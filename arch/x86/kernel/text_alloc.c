// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Kernel module help for x86.
 *  Copyright (C) 2001 Rusty Russell.
 */

#include <linux/kasan.h>
#include <linux/mm.h>
#include <linux/moduleloader.h>
#include <linux/vmalloc.h>
#include <asm/setup.h>

void *text_alloc(unsigned long size)
{
	void *p;

	if (PAGE_ALIGN(size) > MODULES_LEN)
		return NULL;

	p = __vmalloc_node_range(size, MODULE_ALIGN, MODULES_VADDR, MODULES_END,
				 GFP_KERNEL, PAGE_KERNEL, 0, NUMA_NO_NODE,
				 __builtin_return_address(0));

	if (p && (kasan_module_alloc(p, size) < 0)) {
		vfree(p);
		return NULL;
	}

	return p;
}

void text_free(void *region)
{
	/*
	 * This memory may be read-only, and freeing VM_FLUSH_RESET_PERMS memory
	 * in an interrupt is not supported by vmalloc.
	 */
	lockdep_assert_irqs_enabled();

	vfree(region);
}
