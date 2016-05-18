
#include <jnicontext.h>

jni_context_t jni_context_init(JavaVM* vm, jobject imserver){
	jni_context_t t = calloc(1, sizeof(struct jni_context_st));
	t->obj = imserver;
	t->vm = vm;
	return t;
}

void jni_context_call_void_arg_method(jni_context_t t, char* methodName, char* methodSign){
	if (t->vm){
		JNIEnv* newEnv = NULL;
		if ((*(t->vm))->GetEnv(t->vm, (void**)&newEnv, JNI_VERSION_1_4) != JNI_OK){
			return;
		}
		jclass c = (jclass)(*newEnv)->GetObjectClass(newEnv,t->obj);
		jmethodID methodId = (jclass)(*newEnv)->GetMethodID(newEnv, c, methodName, methodSign);
		(*newEnv)->CallVoidMethod(newEnv, t->obj, methodId);
		(*newEnv)->DeleteLocalRef(newEnv, c);
	}
}

void jni_context_call_msg_received(jni_context_t t, char* methodName, char* methodSign, 
	char* id, int msgtype, char* from, char* msg){
	if (t->vm){
		JNIEnv* newEnv = NULL;
		if ((*(t->vm))->GetEnv(t->vm, (void**)&newEnv, JNI_VERSION_1_4) != JNI_OK){
			return;
		}

		jstring msgid     = (jstring)(*newEnv)->NewStringUTF(newEnv, id);
		jstring jidString = (jstring)(*newEnv)->NewStringUTF(newEnv, from);
		jstring msgString = (jstring)(*newEnv)->NewStringUTF(newEnv, msg);

		jclass c = (jclass)(*newEnv)->GetObjectClass(newEnv, t->obj);
		jmethodID methodId = (jmethodID)(*newEnv)->GetMethodID(newEnv, c, methodName, methodSign);
		(*newEnv)->CallVoidMethod(newEnv, t->obj, methodId, msgid, msgtype, jidString, msgString);
		(*newEnv)->DeleteLocalRef(newEnv, jidString);
		(*newEnv)->DeleteLocalRef(newEnv, msgString);
		(*newEnv)->DeleteLocalRef(newEnv, msgid);
	}
}

void jni_context_call_msg_received_ack(jni_context_t t, char* methodName, char* methodSign,char* msgid){
	if (t->vm){
		JNIEnv* newEnv = NULL;
		if ((*(t->vm))->GetEnv(t->vm, (void**)&newEnv, JNI_VERSION_1_4) != JNI_OK){
			return;
		}

		jstring msgIdString = (jstring)(*newEnv)->NewStringUTF(newEnv, msgid);
		jclass c = (jclass)(*newEnv)->GetObjectClass(newEnv, t->obj);
		jmethodID methodId = (jmethodID)(*newEnv)->GetMethodID(newEnv, c, methodName, methodSign);
		(*newEnv)->CallVoidMethod(newEnv, t->obj, methodId, msgIdString);
		(*newEnv)->DeleteLocalRef(newEnv, msgIdString);

	}
}

void jni_context_call_user_login_suceess(jni_context_t t, char* methodName, char* methodSign,char* body){
	if (t->vm){
		JNIEnv* newEnv = NULL;
		if ((*(t->vm))->GetEnv(t->vm, (void**)&newEnv, JNI_VERSION_1_4) != JNI_OK){
			return;
		}

		jstring msgBodyString = (jstring)(*newEnv)->NewStringUTF(newEnv, body);
		jclass c = (jclass)(*newEnv)->GetObjectClass(newEnv, t->obj);
		jmethodID methodId = (jmethodID)(*newEnv)->GetMethodID(newEnv, c, methodName, methodSign);
		(*newEnv)->CallVoidMethod(newEnv, t->obj, methodId, msgBodyString);
		(*newEnv)->DeleteLocalRef(newEnv, msgBodyString);

	}
}

void  jni_context_free(jni_context_t t){
	free(t);
}

JavaVM* jni_context_get_vm(jni_context_t t){
	return t->vm;
}

jobject jni_context_get_object(jni_context_t t){
	return t->obj;
}
