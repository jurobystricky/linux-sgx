/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright(c) 2016-19 Intel Corporation.
 */

#ifndef MAIN_H
#define MAIN_H

struct encl_segment {
	off_t offset;
	size_t size;
	unsigned int prot;
	unsigned int flags;
};

struct encl {
	int fd;
	void *bin;
	off_t bin_size;
	void *src;
	size_t src_size;
	size_t encl_size;
	off_t encl_base;
	unsigned int nr_segments;
	struct encl_segment *segment_tbl;
	struct sgx_secs secs;
	struct sgx_sigstruct sigstruct;
};

void encl_delete(struct encl *ctx);
bool encl_load(const char *path, struct encl *encl);
bool encl_measure(struct encl *encl);
bool encl_build(struct encl *encl);

int sgx_call_vdso(void *rdi, void *rsi, long rdx, u32 leaf, void *r8, void *r9,
		  void *tcs, struct sgx_enclave_exception *ei, void *cb);

#endif /* MAIN_H */
