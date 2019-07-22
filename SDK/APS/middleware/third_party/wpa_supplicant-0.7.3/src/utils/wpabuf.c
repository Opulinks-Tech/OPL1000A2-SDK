/*
 * Dynamic data buffer
 * Copyright (c) 2007-2009, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */
#include "includes.h"
#include "common.h"
#include "trace.h"
#include "wpabuf.h"

//#define WPA_TRACE

void wpabuf_overflow_impl(const struct wpabuf *buf, size_t len)
{
#if 0
#ifdef WPA_TRACE
	struct wpabuf_trace *trace = wpabuf_get_trace(buf);
	if (trace->magic != WPABUF_MAGIC) {
		wpa_printf(MSG_ERROR, "wpabuf: invalid magic %x",
			   trace->magic);
	}
#endif /* WPA_TRACE */
#endif //#if 0

	wpa_printf(MSG_ERROR, "WPA: wpabuf %p (size=%lu used=%lu) overflow len=%lu",
		   buf, (unsigned long) buf->size, (unsigned long) buf->used,
		   (unsigned long) len);
	wpa_trace_show("wpabuf overflow");
	abort();
}

/**
 * wpabuf_alloc - Allocate a wpabuf of the given size
 * @len: Length for the allocated buffer
 * Returns: Buffer to the allocated wpabuf or %NULL on failure
 */
struct wpabuf * wpabuf_alloc_impl(size_t len)
{
#if 0
#ifdef WPA_TRACE
	struct wpabuf_trace *trace = os_zalloc(sizeof(struct wpabuf_trace) +
					       sizeof(struct wpabuf) + len);
	struct wpabuf *buf;
	if (trace == NULL)
		return NULL;
	trace->magic = WPABUF_MAGIC;
	buf = (struct wpabuf *) (trace + 1);
#else /* WPA_TRACE */
	struct wpabuf *buf = os_zalloc(sizeof(struct wpabuf) + len);
	if (buf == NULL)
		return NULL;
#endif /* WPA_TRACE */
#endif //#if 0
	struct wpabuf *buf = os_zalloc(sizeof(struct wpabuf) + len);
	if (buf == NULL)
		return NULL;

	buf->size = len;
	return buf;
}

struct wpabuf * wpabuf_alloc_ext_data_impl(u8 *data, size_t len)
{
#if 0
#ifdef WPA_TRACE
	struct wpabuf_trace *trace = os_zalloc(sizeof(struct wpabuf_trace) +
					       sizeof(struct wpabuf));
	struct wpabuf *buf;
	if (trace == NULL)
		return NULL;
	trace->magic = WPABUF_MAGIC;
	buf = (struct wpabuf *) (trace + 1);
#else /* WPA_TRACE */
	struct wpabuf *buf = os_zalloc(sizeof(struct wpabuf));
	if (buf == NULL)
		return NULL;
#endif /* WPA_TRACE */
#endif //#if 0

    struct wpabuf *buf = os_zalloc(sizeof(struct wpabuf));
    if (buf == NULL)
        return NULL;

	buf->size = len;
	buf->used = len;
	buf->ext_data = data;

	return buf;
}

struct wpabuf * wpabuf_alloc_copy_impl(const void *data, size_t len)
{
	struct wpabuf *buf = wpabuf_alloc(len);
	if (buf)
		wpabuf_put_data(buf, data, len);
	return buf;
}

struct wpabuf * wpabuf_dup_impl(const struct wpabuf *src)
{
	struct wpabuf *buf = wpabuf_alloc(wpabuf_len(src));
	if (buf)
		wpabuf_put_data(buf, wpabuf_head(src), wpabuf_len(src));
	return buf;
}

/**
 * wpabuf_free - Free a wpabuf
 * @buf: wpabuf buffer
 */
void wpabuf_free_impl(struct wpabuf *buf)
{
#if 0
#ifdef WPA_TRACE
	struct wpabuf_trace *trace;
	if (buf == NULL)
		return;
	trace = wpabuf_get_trace(buf);
	if (trace->magic != WPABUF_MAGIC) {
		wpa_printf(MSG_ERROR, "wpabuf_free: invalid magic %x",
			   trace->magic);
		wpa_trace_show("wpabuf_free magic mismatch");
		abort();
	}
	os_free(buf->ext_data);
	os_free(trace);
#else /* WPA_TRACE */
	if (buf == NULL)
		return;
	os_free(buf->ext_data);
	os_free(buf);
#endif /* WPA_TRACE */
#endif

    if (buf == NULL)
        return;
    os_free(buf->ext_data);
    os_free(buf);

}

void * wpabuf_put_impl(struct wpabuf *buf, size_t len)
{
	void *tmp = wpabuf_mhead_u8(buf) + wpabuf_len(buf);
	buf->used += len;
	if (buf->used > buf->size) {
		wpabuf_overflow(buf, len);
	}
	return tmp;
}

/**
 * wpabuf_concat - Concatenate two buffers into a newly allocated one
 * @a: First buffer
 * @b: Second buffer
 * Returns: wpabuf with concatenated a + b data or %NULL on failure
 *
 * Both buffers a and b will be freed regardless of the return value. Input
 * buffers can be %NULL which is interpreted as an empty buffer.
 */
struct wpabuf * wpabuf_concat_impl(struct wpabuf *a, struct wpabuf *b)
{
	struct wpabuf *n = NULL;
	size_t len = 0;

	if (b == NULL)
		return a;

	if (a)
		len += wpabuf_len(a);
	if (b)
		len += wpabuf_len(b);

	n = wpabuf_alloc(len);
	if (n) {
		if (a)
			wpabuf_put_buf(n, a);
		if (b)
			wpabuf_put_buf(n, b);
	}

	wpabuf_free(a);
	wpabuf_free(b);

	return n;
}

/**
 * wpabuf_zeropad - Pad buffer with 0x00 octets (prefix) to specified length
 * @buf: Buffer to be padded
 * @len: Length for the padded buffer
 * Returns: wpabuf padded to len octets or %NULL on failure
 *
 * If buf is longer than len octets or of same size, it will be returned as-is.
 * Otherwise a new buffer is allocated and prefixed with 0x00 octets followed
 * by the source data. The source buffer will be freed on error, i.e., caller
 * will only be responsible on freeing the returned buffer. If buf is %NULL,
 * %NULL will be returned.
 */
struct wpabuf * wpabuf_zeropad_impl(struct wpabuf *buf, size_t len)
{
	struct wpabuf *ret;
	size_t blen;

	if (buf == NULL)
		return NULL;

	blen = wpabuf_len(buf);
	if (blen >= len)
		return buf;

	ret = wpabuf_alloc(len);
	if (ret) {
		os_memset(wpabuf_put(ret, len - blen), 0, len - blen);
		wpabuf_put_buf(ret, buf);
	}
	wpabuf_free(buf);

	return ret;
}

void wpabuf_printf(struct wpabuf *buf, char *fmt, ...)
{
	va_list ap;
	void *tmp = wpabuf_mhead_u8(buf) + wpabuf_len(buf);
	int res;

	va_start(ap, fmt);
	res = vsnprintf(tmp, buf->size - buf->used, fmt, ap);
	va_end(ap);
	if (res < 0 || (size_t) res >= buf->size - buf->used)
		wpabuf_overflow(buf, res);
	buf->used += res;
}

RET_DATA wpabuf_overflow_fp_t wpabuf_overflow;
RET_DATA wpabuf_alloc_fp_t wpabuf_alloc;
RET_DATA wpabuf_alloc_ext_data_fp_t wpabuf_alloc_ext_data;
RET_DATA wpabuf_alloc_copy_fp_t wpabuf_alloc_copy;
RET_DATA wpabuf_dup_fp_t wpabuf_dup;
RET_DATA wpabuf_free_fp_t wpabuf_free;
RET_DATA wpabuf_put_fp_t wpabuf_put;
RET_DATA wpabuf_concat_fp_t wpabuf_concat;
RET_DATA wpabuf_zeropad_fp_t wpabuf_zeropad;

/*
   Interface Initialization: WPA Buffer
 */
void wpa_buf_func_init(void)
{
    wpabuf_overflow = wpabuf_overflow_impl;
    wpabuf_alloc = wpabuf_alloc_impl;
    wpabuf_alloc_ext_data = wpabuf_alloc_ext_data_impl;
    wpabuf_alloc_copy = wpabuf_alloc_copy_impl;
    wpabuf_dup = wpabuf_dup_impl;
    wpabuf_free = wpabuf_free_impl;
    wpabuf_put = wpabuf_put_impl;
    wpabuf_concat = wpabuf_concat_impl;
    wpabuf_zeropad = wpabuf_zeropad_impl;
}


#if 0
#ifdef WPA_TRACE
#define WPABUF_MAGIC 0x51a974e3

struct wpabuf_trace {
	unsigned int magic;
};

static struct wpabuf_trace * wpabuf_get_trace(const struct wpabuf *buf)
{
	return (struct wpabuf_trace *)
		((const u8 *) buf - sizeof(struct wpabuf_trace));
}
#endif /* WPA_TRACE */
#endif //#if 0

#if 0
int wpabuf_resize(struct wpabuf **_buf, size_t add_len)
{
	struct wpabuf *buf = *_buf;
#ifdef WPA_TRACE
	struct wpabuf_trace *trace;
#endif /* WPA_TRACE */

	if (buf == NULL) {
		*_buf = wpabuf_alloc(add_len);
		return *_buf == NULL ? -1 : 0;
	}

#ifdef WPA_TRACE
	trace = wpabuf_get_trace(buf);
	if (trace->magic != WPABUF_MAGIC) {
		wpa_printf(MSG_ERROR, "wpabuf: invalid magic %x",
			   trace->magic);
		wpa_trace_show("wpabuf_resize invalid magic");
		abort();
	}
#endif /* WPA_TRACE */

	if (buf->used + add_len > buf->size) {
		unsigned char *nbuf;
		if (buf->ext_data) {
			nbuf = os_realloc(buf->ext_data, buf->used + add_len);
			if (nbuf == NULL)
				return -1;
			os_memset(nbuf + buf->used, 0, add_len);
			buf->ext_data = nbuf;
		} else {
#ifdef WPA_TRACE
			nbuf = os_realloc(trace, sizeof(struct wpabuf_trace) +
					  sizeof(struct wpabuf) +
					  buf->used + add_len);
			if (nbuf == NULL)
				return -1;
			trace = (struct wpabuf_trace *) nbuf;
			buf = (struct wpabuf *) (trace + 1);
			os_memset(nbuf + sizeof(struct wpabuf_trace) +
				  sizeof(struct wpabuf) + buf->used, 0,
				  add_len);
#else /* WPA_TRACE */
			nbuf = os_realloc(buf, sizeof(struct wpabuf) +
					  buf->used + add_len);
			if (nbuf == NULL)
				return -1;
			buf = (struct wpabuf *) nbuf;
			os_memset(nbuf + sizeof(struct wpabuf) + buf->used, 0,
				  add_len);
#endif /* WPA_TRACE */
			*_buf = buf;
		}
		buf->size = buf->used + add_len;
	}

	return 0;
}
#endif


