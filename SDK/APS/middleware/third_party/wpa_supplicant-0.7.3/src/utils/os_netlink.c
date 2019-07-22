/*
 * wpa_supplicant/hostapd / Empty OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file can be used as a starting point when adding a new OS target. The
 * functions here do not really work as-is since they are just empty or only
 * return an error value. os_internal.c can be used as another starting point
 * or reference since it has example implementation of many of these functions.
 */
#include "includes.h"
#include "os.h"
#include "string.h"
#include "stdlib.h"
#include "cmsis_os.h"
#include <time.h>

void os_sleep(os_time_t sec, os_time_t usec)
{
	if (sec){
		osDelay(sec * 1000);
		return;
	}
	if (usec){
		osDelay(usec);
		return;
	}
}

int os_get_time(struct os_time *t)
{
	return -1;
}

int os_mktime(int year, int month, int day, int hour, int min, int sec,
	      os_time_t *t)
{
	return -1;
}

int os_daemonize(const char *pid_file)
{
	return -1;
}

void os_daemonize_terminate(const char *pid_file)
{
}

int os_get_random(unsigned char *buf, size_t len)
{
    int i;
    //srand(time(NULL)); // randomize seed
    for(i=0;i<len;i++) buf[i] = (rand()%10);
	return 0;
}

unsigned long os_random(void)
{
	return rand();
}

char * os_rel2abs_path(const char *rel_path)
{
	return NULL; /* strdup(rel_path) can be used here */
}

int os_program_init(void)
{
	return 0;
}

void os_program_deinit(void)
{
}

char * os_readfile(const char *name, size_t *len)
{
	return NULL;
}

void * os_zalloc(size_t size)
{
	return calloc(1, size);;
}

#ifdef OS_NO_C_LIB_DEFINES
void * os_malloc(size_t size)
{
	return malloc(size);
}

void * os_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

void os_free(void *ptr)
{
	free(ptr);
}

void * os_memcpy(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;
	while (n--)
		*d++ = *s++;
	return dest;
}

void * os_memmove(void *dest, const void *src, size_t n)
{
	return memmove(dest, src, n);
}

void * os_memset(void *s, int c, size_t n)
{
	return memset(s, c, n);
}

int os_memcmp(const void *s1, const void *s2, size_t n)
{
	return memcmp(s1, s2, n);
}

char * os_strdup(const char *s)
{
	char *res;
	size_t len;
	if (s == NULL)
		return NULL;
	len = os_strlen(s);
	res = os_malloc(len + 1);
	if (res)
		os_memcpy(res, s, len + 1);
	return res;
}

size_t os_strlen(const char *s)
{
	return strlen(s);
}

int os_strcasecmp(const char *s1, const char *s2)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strcmp(s1, s2);
}

int os_strncasecmp(const char *s1, const char *s2, size_t n)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strncmp(s1, s2, n);
}

char * os_strchr(const char *s, int c)
{
	return strchr(s, c);
}

char * os_strrchr(const char *s, int c)
{
	return strrchr(s, c);
}

int os_strcmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}

int os_strncmp(const char *s1, const char *s2, size_t n)
{
	return strncmp(s1, s2, n);
}

char * os_strncpy(char *dest, const char *src, size_t n)
{
	return strncpy(dest, src, n);
}

size_t os_strlcpy(char *dest, const char *src, size_t size)
{
	const char *s = src;
	size_t left = size;

	if (left) {
		/* Copy string up to the maximum size of the dest buffer */
		while (--left != 0) {
			if ((*dest++ = *s++) == '\0')
				break;
		}
	}

	if (left == 0) {
		/* Not enough room for the string; force NUL-termination */
		if (size != 0)
			*dest = '\0';
		while (*s++)
			; /* determine total src string length */
	}

	return s - src - 1;
}

char * os_strstr(const char *haystack, const char *needle)
{
	return strstr(haystack, needle);
}

int os_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list ap;
	int ret;

	/* See http://www.ijs.si/software/snprintf/ for portable
	 * implementation of snprintf.
	 */

	va_start(ap, format);
	ret = vsnprintf(str, size, format, ap);
	va_end(ap);
	if (size > 0)
		str[size - 1] = '\0';
	return ret;

}
#endif /* OS_NO_C_LIB_DEFINES */


/*
int os_setenv(const char *name, const char *value, int overwrite)
{
	return -1;
}
*/

/*
int os_unsetenv(const char *name)
{
	return -1;
}
*/


