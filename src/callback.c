
#include <client.h>
#include <message.h>
#include <message_const.h>
#include <imutil.h>

static void _connect_success(){
	fprintf(stderr,"the connect is ok\n");
}

static void _connect_error(char* error){
	fprintf(stderr,"the connect is error , %s\n",error);
}

static void _login_result(char* str, int len){
	fprintf(stderr,"the login is %s\n",str);
}

static void _mesage_pang(void* arg){
	time_t tms = time(NULL);
	client_t t = (client_t)arg;
	int len = 0;
	char* pang = im_sdk_message_pang_packet(&len);
	im_client_send(t, pang, len);
	fprintf(stderr, "im sdk receive a ping and send pang");
	free(pang);
}

static void _message_receive_ack(void* arg, char* msgid){
	fprintf(stderr, "im sdk receive ack %s\n", msgid);
}

static void _message_send_ack(void* arg,char* msgid){
	client_t t = (client_t)arg;
	int len = 0;
	char* msgstr = im_sdk_message_ack(msgid, &len);
	im_client_send(t, msgstr, len);
	fprintf(stderr, "im sdk send ack %s\n", msgid);
	free(msgstr);
}

static void _message_read_module(void* arg, void* msg){
	client_t t = (client_t)arg;
	int len = 0;
	const char* body = message_get_body(msg);
	fprintf(stderr, "im sdk receive a message %s\n", body);
}

static void _message_read(void* arg, char* str, int len){
	if(len == 0){
		return ;
	}
	void* msg = message_parse_from_data(str,len);
	int t = message_get_type(msg);
	char* id = (char*)message_get_id(msg);
	char* temp = NULL;
	switch(t){
		case Message_Login_Type:
			break;
		case Message_Ack_Type:
			temp = (char*)message_get_body(msg);
			_message_receive_ack(arg, temp);
			break;
		case Message_Ping_Type:
			_mesage_pang(arg);
			break;
		case Message_Friend_Add_Type:
			break;
		case Message_Friend_Agree_Type:
			break;
		case Message_Message_Type:
			_message_read_module(arg, msg);
			_message_send_ack(arg, id);
			break;
	}
over:
	message_destory(msg);
}

static void _message_write(char* str, int len){
	char* re = calloc(1,len+1);
	memcpy(re,str,len);
	fprintf(stderr,"the write: %s\n",re);
	free(re);
}

callback_t im_client_callback_init(){
	callback_t c = calloc(1,sizeof(struct callback_st));
	c->im_callback_connect_error = _connect_error;
	c->im_callback_connect_success = _connect_success;
	c->im_callback_login_result = _login_result;
	c->im_callback_read = _message_read;
	c->im_callback_write = _message_write;
	return c;
}

void im_client_callback_free(callback_t t){
	assert(t);
	free(t);
}
