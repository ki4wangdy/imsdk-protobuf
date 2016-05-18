
#include <jni_imservice.h>
#include <android/log.h>

#include <client.h>
#include <jnicontext.h>

#include <message.h>
#include <message_const.h>
#include <imutil.h>

client_t mclient = NULL;
jni_context_t jnicontext = NULL;

#define LOG_TAG "cj_imcore"

//#ifdef DEBUG
	#define CJ_LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
//#else
//	#define CJ_LOG(...)
//#endif

#ifndef MAX_SIZE
	#define MAX_SIZE 512
#endif

void im_server_android_log(const char* file, int line, const char *msgfmt, ...){

	va_list ap;
	char *pos, message[MAX_SIZE];
	int sz;

	snprintf(message, MAX_SIZE, "sx (%s:%d) ", file, line);

	for (pos = message; *pos != '\0'; pos++);
	sz = pos - message;

	va_start(ap, msgfmt);
	vsnprintf(pos, MAX_SIZE - sz, msgfmt, ap);
	va_end(ap);

	//jni_context_call_user_login_suceess(jnicontext, "onJniLogPrint", "(Ljava/lang/String;)V", message);
	CJ_LOG("the file:%s the line:%d, the msg:%s", file, line, message);
}

static void _jni_callback_connect_success(){
	if (jnicontext){
		jni_context_call_void_arg_method(jnicontext, "onConnected", "()V");
	}
}

static void _jni_callback_connect_error(char* error){
	if (jnicontext){
		jni_context_call_void_arg_method(jnicontext, "onDisconnected", "()V");
	}
}

static void _jni_callback_login_result(char* str, int len){
	char* msg = calloc(1, len);
	memcpy(msg, str, len);
	CJ_LOG("the login's result is %s:", msg);
	free(msg);
}

static void _message_read_module(void* arg, void* msg){
	client_t t = (client_t)arg;
	char* body = (char*)message_get_body(msg);
	char* from = (char*)message_get_from(msg);
	char* id   = (char*)message_get_id(msg);

	// received the message , route this message to different module
	int type_int = message_get_type(msg);

	CJ_LOG("receive a message : %s, the type : %d, the from : %s", body, type_int, from);

	if (jnicontext){
		jni_context_call_msg_received(jnicontext,
			"onRecevicedMsg", "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V",
			id, type_int, from, body);
	}

}

static void _message_send_ack(void* arg, char* msgid){
	client_t t = (client_t)arg;
	int len = 0;
	char* msgstr = im_sdk_message_ack(msgid, &len);
	im_client_send(t, msgstr, len);
	CJ_LOG("the client send an ack, the msgid:%s", msgid);
	free(msgstr);
}

static void _mesage_pang(void* arg){
	time_t tms = time(NULL);
	client_t t = (client_t)arg;
	int len = 0;
	char* pang = im_sdk_message_pang_packet(&len);
	im_client_send(t, pang, len);
	CJ_LOG("im sdk receive a ping and send pang");
	free(pang);
}

static void _message_receive_ack(void* arg, char* msgid){
	CJ_LOG("the client receive an ack , the msgid:%s", msgid);
	jni_context_call_msg_received_ack(jnicontext,"onMsgServerReceivedAck","(Ljava/lang/String;)V",msgid);
}

static void _message_receive_login(void* arg, char* msgbody){
	CJ_LOG("user login success, the body:%s", msgbody);
	jni_context_call_user_login_suceess(jnicontext,"onUserLoginSuccess","(Ljava/lang/String;)V",msgbody);
}

static void _jni_callback_read(void* arg, char* str, int len){
	char* id = NULL;
	char* temp = NULL;
	if (len == 0){
		return;
	}
	void* msg = message_parse_from_data(str,len);
	CJ_LOG("received an message, msg:%s the len:%d", str, len);
	if (!msg){
		CJ_LOG("parse the message is error, the string:%s the len:%d",str,len);
		return;
	}

	int t = message_get_type(msg);
	switch (t){
	case Message_Login_Type:
		temp = (char*)message_get_body(msg);
		_message_receive_login(arg, temp);
		break;
	case Message_Ack_Type:
		temp = (char*)message_get_body(msg);
		CJ_LOG("received an message ack, the id:%s",temp);
		_message_receive_ack(arg, temp);
		break;
	case Message_Ping_Type:
		_mesage_pang(arg);
		break;
	case Message_Friend_Add_Type:
	case Message_Friend_Agree_Type:
	case Message_Friend_Reply_Type:
	case Message_Message_Type:
		_message_read_module(arg, msg);
		id = (char*)message_get_id(msg);
		_message_send_ack(arg, id);
		break;
	}
over:
	message_destory(msg);
}

static void _jni_callback_write(char* msg, int len){
	char* str = calloc(1, len);
	memcpy(str, msg, len);
	CJ_LOG("send a message:%s, the len:%d", str, len);
	free(str);
}

/*
* Class:     com_imrub_shoulder_module_im_server_IMService
* Method:    initImserver
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_initImserver
(JNIEnv* env, jobject obj){

	// im client init
	mclient = im_client_init();

	// jni context , to save the glabal object
	jobject globalObj = (jobject)(*env)->NewGlobalRef(env, obj);
	JavaVM* vm = NULL;
	(*env)->GetJavaVM(env, &vm);
	jnicontext = jni_context_init(vm, globalObj);

	// init log system
	log_t ts = im_sdk_log_init(im_sdk_log_android, im_server_android_log);
	im_client_set_log(mclient, ts);

	// init callback system
	callback_t cb = im_client_callback_init();
	cb->im_callback_connect_error = _jni_callback_connect_error;
	cb->im_callback_connect_success = _jni_callback_connect_success;
	cb->im_callback_login_result = _jni_callback_login_result;
	cb->im_callback_read = _jni_callback_read;
	cb->im_callback_write = _jni_callback_write;
	im_client_set_callback(mclient, cb);

}

/*
* Class:     com_imrub_shoulder_module_im_server_IMService
* Method:    desotryserver
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_desotryserver
(JNIEnv* env, jobject obj){
	if (mclient){
		im_client_free(mclient);
		mclient = NULL;
	}
	if (jnicontext){
		jobject obj = jni_context_get_object(jnicontext);
		(*env)->DeleteGlobalRef(env, obj);
		jni_context_free(jnicontext);
		jnicontext = NULL;
	}
}

/*
* Class:     com_imrub_shoulder_module_im_server_IMService
* Method:    isconnected
* Signature: ()Z
*/
JNIEXPORT jboolean JNICALL Java_com_imrub_shoulder_module_im_server_IMService_isconnected
(JNIEnv* env, jobject obj){
	int isconnect = 0;
	if (mclient){
		isconnect = im_client_isconnected(mclient);
	}
	return (jboolean)isconnect;
}

/*
* Class:     com_imrub_shoulder_module_im_server_IMService
* Method:    sendMsgNative
* Signature: (Ljava/lang/String;Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_sendMsgNative
(JNIEnv* env, jobject obj, jstring id, jstring from, jstring to, jstring msgtype, jstring msg){

	char* id_str = (char*)(*env)->GetStringUTFChars(env, id, 0);
	char* from_str = (char*)(*env)->GetStringUTFChars(env, from, 0);
	char* to_str = (char*)(*env)->GetStringUTFChars(env, to, 0);
	char* msg_str = (char*)(*env)->GetStringUTFChars(env, msg, 0);
	char* msg_type = (char*)(*env)->GetStringUTFChars(env, msgtype, 0);
	if (mclient){
		int strlen = 0;
		char* strs = im_sdk_send_message_construct(id_str, from_str, to_str, msg_str,msg_type, &strlen);
		im_client_send(mclient, strs, strlen);
		free(strs);
	}
	(*env)->ReleaseStringUTFChars(env, id, id_str);
	(*env)->ReleaseStringUTFChars(env, from, from_str);
	(*env)->ReleaseStringUTFChars(env, to, to_str);
	(*env)->ReleaseStringUTFChars(env, msg, msg_str);
	(*env)->ReleaseStringUTFChars(env, msgtype, msg_type);
}


/*
* Class:     com_imrub_shoulder_module_im_server_IMService
* Method:    login
* Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_login
(JNIEnv * env, jobject obj, jstring uid, jstring token , jstring server){

	char* uid_str = (char*)(*env)->GetStringUTFChars(env, uid, 0);
	char* token_str = (char*)(*env)->GetStringUTFChars(env, token, 0);
	char* server_str = (char*)(*env)->GetStringUTFChars(env, server, 0);
	if (mclient){
		int len = 0;
		im_client_connect(mclient, server_str, 5222);
		im_client_login(mclient, uid_str, token_str, &len);
	}

	(*env)->ReleaseStringUTFChars(env, uid, uid_str);
	(*env)->ReleaseStringUTFChars(env, token, token_str);
	(*env)->ReleaseStringUTFChars(env, server, server_str);
}
