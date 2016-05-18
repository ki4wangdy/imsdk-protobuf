
#include <client.h>

#include <type.h>

//gcc -I. main.c ./libimsdk.a ../imProtocal/libimProtocal.a -o main

static void login_and_send(){

	client_t t = im_client_init();
	log_t log = im_sdk_log_init(im_sdk_log_stderr);

	im_client_set_log(t, log);
	im_client_set_callback(t,im_client_callback_init());
	im_client_connect(t, "192.168.160.129", 5222);

	int len = 0;
	int code = im_client_login(t, "100056", "3k4k23k4kk3234", &len);
	if(code == false){
		return ;
	}

	int i = 0;
	for (; i < 10000 * 100;){
		// start to send message
		message_type_t msg = message_type_init("ki-id-1", "100056", "100057", "hello world", Message_Message_Type);

		char* sendmsg = NULL;
		len = 0;
		message_type_serialize(msg, &sendmsg, &len);

		int isok = im_client_send(t, sendmsg, len);
		if (isok){
			fprintf(stderr, "send successfully\n");
		}

		free(sendmsg);
		message_type_free(msg);

	}

	free(str);

	im_client_free(t);
}

static void login_and_rec(){

	client_t t = im_client_init();
	log_t log = im_sdk_log_init(im_sdk_log_stderr);

	im_client_set_log(t, log);
	im_client_connect(t, "192.168.160.129", 5222);

	int len = 0;
	char* body = im_client_login(t, "100056", "3k4k23k4kk3234", &len);

	char* str = calloc(1, len + 1);
	memcpy(str, body, len + 1);

	fprintf(stderr, "%s\n", str);

	// start to rec
	do{
		char* msgstr = NULL;
		len = 0;
		msgstr = im_client_rec(t, &len);
		if (msgstr){
			message_type_t msg = message_type_deserialization(msgstr);
			char* from = message_type_get_from(msg);
			char* type = message_type_get_type(msg);
			char* body = message_type_get_body(msg);
			fprintf(stderr, "the type is %s, the from is %s, the body is %s\n", type, from, body);
			free(from);
			free(body);
			free(type);
			free(msgstr);
			message_type_free(msg);
		}
	} while (1);

	im_client_free(t);
}

int main(){
	login_and_send();
}