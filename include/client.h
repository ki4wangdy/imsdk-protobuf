
#ifndef _IM_SDK_CLIENT_H_
#define _IM_SDK_CLIENT_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <logproxy.h>
#include <const.h>
#include <callback.h>

typedef struct client_st* client_t;

client_t  im_client_init();

void  im_client_set_log(client_t ts, log_t t);
void  im_client_set_callback(client_t ts, callback_t cb);

int   im_client_login(client_t t, char* uid, char* token, int* body_len);

int   im_client_connect(client_t t, char* ip, int port);
int   im_client_isconnected(client_t t);

int   im_client_rec(client_t t);
int   im_client_send(client_t t,char* msg, int len);

char* im_client_getserver(client_t t);
int   im_client_getserver_port(client_t t);

void  im_client_disconnect(client_t t);
void  im_client_debug(client_t t,const char *msgfmt, ...);

void  im_client_free(client_t t);

extern client_t mclient;
void im_client_cb_read(client_t t, char* temp, int len);

#ifdef __cplusplus
	}
#endif

#endif