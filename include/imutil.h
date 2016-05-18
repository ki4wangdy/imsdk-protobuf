
#ifndef _IM_SDK_UTIL_H_
#define _IM_SDK_UTIL_H_

char* im_sdk_login_string(char* uid, char* token, int* len);

char* im_sdk_message_pang_packet(int* len);
char* im_sdk_message_ack(char* msgid,int* len);

char* im_sdk_send_message_construct(char* id, char* from, char* to, char* msg, char* msgtype, int* len);

char* im_sdk_send_friend_request(char* id, char* from, char* to, char* senderName, char* msg,int* len);
char* im_sdk_send_friend_agree(char* id, char* from, char* to, char* senderName, char* msg, int* len);

#endif