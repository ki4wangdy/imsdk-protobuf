
#ifndef _IM_SDK_LOG_H_
#define _IM_SDK_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct log_st{
	void (*log_debug)(const char* file, int line, const char *msgfmt, ...);	
}*log_t;

#define ZONE __FILE__,__LINE__

#define im_sdk_log_stderr	0x01
#define im_sdk_log_android	0x04

typedef void(*log_type_t)(const char* file, int line, const char *msgfmt, ...);

log_t im_sdk_log_init(int type,log_type_t fc);
void  im_sdk_log_free(log_t t);

#endif