/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _OPULINKS_LOG_H_
#define _OPULINKS_LOG_H_

#include "msg.h"

#define LOG_LOCAL_LEVEL 3

typedef enum {
    LOG_NONE,       /*!< No log output */
    LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,       /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} log_level_t;

#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)

#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " %s: " format LOG_RESET_COLOR "\n"

#define LOG_EARLY_IMPL(tag, format, log_level, log_tag_letter, ...) \
    do {   \
        if (LOG_LOCAL_LEVEL >= log_level) {                         \
            tracer_log(LOG_MED_LEVEL, LOG_FORMAT(log_tag_letter, format), tag, ##__VA_ARGS__);  \
    }} while(0)

#define EARLY_LOGE(tag, format, ...) LOG_EARLY_IMPL(tag, format, LOG_INFO,    E, ##__VA_ARGS__)
#define EARLY_LOGW(tag, format, ...) LOG_EARLY_IMPL(tag, format, LOG_INFO,    W, ##__VA_ARGS__)
#define EARLY_LOGI(tag, format, ...) LOG_EARLY_IMPL(tag, format, LOG_INFO,    I, ##__VA_ARGS__)

#define LOGE(tag, format, ...)  EARLY_LOGE(tag, format, ##__VA_ARGS__)
#define LOGW(tag, format, ...)  EARLY_LOGE(tag, format, ##__VA_ARGS__)
#define LOGI(tag, format, ...)  EARLY_LOGI(tag, format, ##__VA_ARGS__)

#define LOGE_DRCT(tag, format, ...) tracer_drct_printf(LOG_FORMAT(E, format), tag, ##__VA_ARGS__);
#define LOGW_DRCT(tag, format, ...) tracer_drct_printf(LOG_FORMAT(W, format), tag, ##__VA_ARGS__);
#define LOGI_DRCT(tag, format, ...) tracer_drct_printf(LOG_FORMAT(I, format), tag, ##__VA_ARGS__);
    
#endif /* _OPULINKS_LOG_H_ */
