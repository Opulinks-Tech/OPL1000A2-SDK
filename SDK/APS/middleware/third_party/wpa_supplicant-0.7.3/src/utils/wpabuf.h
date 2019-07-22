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

#ifndef WPABUF_H
#define WPABUF_H

#include <string.h>

#ifndef SIZE_T
typedef unsigned int size_t;
#endif //SIZE_T

/*
 * Internal data structure for wpabuf. Please do not touch this directly from
 * elsewhere. This is only defined in header file to allow inline functions
 * from this file to access data.
 */
struct wpabuf {
	size_t size; /* total size of the allocated buffer */
	size_t used; /* length of data in the buffer */
	u8 *ext_data; /* pointer to external data; NULL if data follows
		       * struct wpabuf */
	/* optionally followed by the allocated buffer */
};


int wpabuf_resize(struct wpabuf **buf, size_t add_len);
void wpabuf_printf(struct wpabuf *buf, char *fmt, ...) PRINTF_FORMAT(2, 3);


typedef void (*wpabuf_overflow_fp_t)(const struct wpabuf *buf, size_t len);
typedef struct wpabuf *(*wpabuf_alloc_fp_t)(size_t len);
typedef struct wpabuf * (*wpabuf_alloc_ext_data_fp_t)(u8 *data, size_t len);
typedef struct wpabuf * (*wpabuf_alloc_copy_fp_t)(const void *data, size_t len);
typedef struct wpabuf * (*wpabuf_dup_fp_t)(const struct wpabuf *src);
typedef void (*wpabuf_free_fp_t)(struct wpabuf *buf);
typedef void * (*wpabuf_put_fp_t)(struct wpabuf *buf, size_t len);
typedef struct wpabuf * (*wpabuf_concat_fp_t)(struct wpabuf *a, struct wpabuf *b);
typedef struct wpabuf * (*wpabuf_zeropad_fp_t)(struct wpabuf *buf, size_t len);


extern wpabuf_overflow_fp_t wpabuf_overflow;
extern wpabuf_alloc_fp_t wpabuf_alloc;
extern wpabuf_alloc_ext_data_fp_t wpabuf_alloc_ext_data;
extern wpabuf_alloc_copy_fp_t wpabuf_alloc_copy;
extern wpabuf_dup_fp_t wpabuf_dup;
extern wpabuf_free_fp_t wpabuf_free;
extern wpabuf_put_fp_t wpabuf_put;
extern wpabuf_concat_fp_t wpabuf_concat;
extern wpabuf_zeropad_fp_t wpabuf_zeropad;


/*
   Interface Initialization: WPA Buffer
 */
void wpa_buf_func_init(void);



/**
 * wpabuf_size - Get the currently allocated size of a wpabuf buffer
 * @buf: wpabuf buffer
 * Returns: Currently allocated size of the buffer
 */
static size_t wpabuf_size(const struct wpabuf *buf)
{
	return buf->size;
}

/**
 * wpabuf_len - Get the current length of a wpabuf buffer data
 * @buf: wpabuf buffer
 * Returns: Currently used length of the buffer
 */
static size_t wpabuf_len(const struct wpabuf *buf)
{
	return buf->used;
}

/**
 * wpabuf_tailroom - Get size of available tail room in the end of the buffer
 * @buf: wpabuf buffer
 * Returns: Tail room (in bytes) of available space in the end of the buffer
 */
static size_t wpabuf_tailroom(const struct wpabuf *buf)
{
	return buf->size - buf->used;
}

/**
 * wpabuf_head - Get pointer to the head of the buffer data
 * @buf: wpabuf buffer
 * Returns: Pointer to the head of the buffer data
 */
static const void * wpabuf_head(const struct wpabuf *buf)
{
	if (buf->ext_data)
		return buf->ext_data;
	return buf + 1;
}

static const u8 * wpabuf_head_u8(const struct wpabuf *buf)
{
	return wpabuf_head(buf);
}

/**
 * wpabuf_mhead - Get modifiable pointer to the head of the buffer data
 * @buf: wpabuf buffer
 * Returns: Pointer to the head of the buffer data
 */
static void * wpabuf_mhead(struct wpabuf *buf)
{
	if (buf->ext_data)
		return buf->ext_data;
	return buf + 1;
}

static u8 * wpabuf_mhead_u8(struct wpabuf *buf)
{
	return wpabuf_mhead(buf);
}

static void wpabuf_put_u8(struct wpabuf *buf, u8 data)
{
	u8 *pos = wpabuf_put(buf, 1);
	*pos = data;
}

static void wpabuf_put_le16(struct wpabuf *buf, u16 data)
{
	u8 *pos = wpabuf_put(buf, 2);
	WPA_PUT_LE16(pos, data);
}

static void wpabuf_put_be16(struct wpabuf *buf, u16 data)
{
	u8 *pos = wpabuf_put(buf, 2);
	WPA_PUT_BE16(pos, data);
}

static void wpabuf_put_be24(struct wpabuf *buf, u32 data)
{
	u8 *pos = wpabuf_put(buf, 3);
	WPA_PUT_BE24(pos, data);
}

static void wpabuf_put_be32(struct wpabuf *buf, u32 data)
{
	u8 *pos = wpabuf_put(buf, 4);
	WPA_PUT_BE32(pos, data);
}

static void wpabuf_put_data(struct wpabuf *buf, const void *data, size_t len)
{
	if (data)
		os_memcpy(wpabuf_put(buf, len), data, len);
}

static void wpabuf_put_buf(struct wpabuf *dst, const struct wpabuf *src)
{
	wpabuf_put_data(dst, wpabuf_head(src), wpabuf_len(src));
}

static void wpabuf_set(struct wpabuf *buf, const void *data, size_t len)
{
	buf->ext_data = (u8 *) data;
	buf->size = buf->used = len;
}

static void wpabuf_put_str(struct wpabuf *dst, const char *str)
{
	wpabuf_put_data(dst, str, os_strlen(str));
}

#if 0
//struct wpabuf * wpabuf_alloc(size_t len);
//struct wpabuf * wpabuf_alloc_ext_data(u8 *data, size_t len);
//struct wpabuf * wpabuf_alloc_copy(const void *data, size_t len);
//struct wpabuf * wpabuf_dup(const struct wpabuf *src);
//void wpabuf_free(struct wpabuf *buf);
//void * wpabuf_put(struct wpabuf *buf, size_t len);
//struct wpabuf * wpabuf_concat(struct wpabuf *a, struct wpabuf *b);
//struct wpabuf * wpabuf_zeropad(struct wpabuf *buf, size_t len);
#endif //#if 0

#endif /* WPABUF_H */
