//-----------------------------------------------------------------------------
/*
 * https://github.com/rxi/log.c
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 *
 * https://github.com/deadsy/picox
 * Modified by Jason T. Harris for RPi Pico. Copyright (c) 2021
 *
 */
//-----------------------------------------------------------------------------

#ifndef LOG_H
#define LOG_H

//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "pico/time.h"

//-----------------------------------------------------------------------------

#define LOG_VERSION "0.1.0"

typedef struct {
	va_list ap;
	const char *fmt;
	const char *file;
	absolute_time_t time;
	void *udata;
	uint core;
	int line;
	int level;
} log_Event;

typedef void (*log_LogFn)(log_Event * ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#ifdef LOG_ENABLE

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#else				// remove logging

#define log_trace(...) do{}while(0)
#define log_debug(...) do{}while(0)
#define log_info(...)  do{}while(0)
#define log_warn(...)  do{}while(0)
#define log_error(...) do{}while(0)
#define log_fatal(...) do{}while(0)

#endif

int log_init(int level, bool color);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_color(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE * fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

//-----------------------------------------------------------------------------

#endif				// LOG_H

//-----------------------------------------------------------------------------
