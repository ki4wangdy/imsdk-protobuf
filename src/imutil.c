
#include <client.h>
#include <message.h>
#include <message_const.h>

char* im_sdk_login_string(char* uid, char* token, int* len){
	void* msg = message_init_no_data();
	message_set_from(msg,uid,strlen(uid));
	message_set_body(msg,token,strlen(token));
	message_set_id(msg, "ki-id-1", 7);
	message_set_to(msg, "20004", 5);
	message_set_type(msg,Message_Login_Type);

	int size = 0;
	message_serialization_size(msg, &size);
	char* m = malloc(sizeof(char)*size);
	*len = size;

	message_to_serialization(msg, m, sizeof(char)*size);
	message_destory(msg);
	return m;
}

char* im_sdk_message_pang_packet(int* len){
	void* msg = message_init_no_data();
	message_set_type(msg, Message_Pang_Type);

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);
	message_destory(msg);
	return m;
}

char* im_sdk_message_ack(char* msgid, int* len){
	void* msg = message_init_no_data();
	message_set_type(msg, Message_Ack_Type);
	message_set_id(msg, msgid, strlen(msgid));

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);
	message_destory(msg);

	return m;
}

char* im_sdk_send_message_construct(char* id, char* from, char* to, char* body, char* msgtype, int* len){
	void* msg = message_init_no_data();

	message_set_id(msg, id, strlen(id));
	message_set_from(msg, from, strlen(from));
	message_set_to(msg, to, strlen(to));
	message_set_body(msg, body, strlen(body));

	int t = atoi(msgtype);
	message_set_type(msg, t);

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);
	message_destory(msg);
	return m;
}

char* im_sdk_send_friend_request(char* id, char* from, char* to, char* senderName, char* body, int* len){

	void* msg = message_init_no_data();

	message_set_id(msg, id, strlen(id));
	message_set_from(msg, from, strlen(from));
	message_set_to(msg, to, strlen(to));
	message_set_body(msg, body, strlen(body));
	message_set_type(msg, Message_Friend_Add_Type);

	message_add_header(msg, Message_Header_Sender_Name, sizeof(Message_Header_Sender_Name) - 1, 
			senderName, strlen(senderName));

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);
	message_destory(msg);
	return m;

}

char* im_sdk_send_friend_agree(char* id, char* from, char* to, char* senderName, char* body, int* len){

	void* msg = message_init_no_data();

	message_set_id(msg, id, strlen(id));
	message_set_from(msg, from, strlen(from));
	message_set_to(msg, to, strlen(to));
	message_set_body(msg, body, strlen(body));

	message_set_type(msg, Message_Friend_Agree_Type);
	message_set_body(msg, msg, strlen(msg));

	message_add_header(msg, Message_Header_Sender_Name, sizeof(Message_Header_Sender_Name) - 1,
		senderName, strlen(senderName));

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);
	message_destory(msg);
	return m;

}
