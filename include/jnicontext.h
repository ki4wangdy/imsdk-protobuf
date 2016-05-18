
#ifndef _JNI_CONTEXT_H_
#define _JNI_CONTEXT_H_

#include <client.h>
#include <jni.h>

typedef struct jni_context_st{
	JavaVM* vm;
	jobject obj;
}*jni_context_t;

jni_context_t jni_context_init(JavaVM* vm, jobject imserver);

JavaVM* jni_context_get_vm(jni_context_t);
jobject jni_context_get_object(jni_context_t);

void jni_context_call_void_arg_method(jni_context_t t, char* methodName, char* methodSign);
void jni_context_call_msg_received(jni_context_t t, char* methodName, char* methodSign,
	char*id, int msgtype, char* from, char* msg);

void jni_context_call_msg_received_ack(jni_context_t t, char* methodName, char* methodSign,char* msgid);

void  jni_context_free(jni_context_t t);

#endif