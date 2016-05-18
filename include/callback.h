
#ifndef _IM_SDK_CALLBACK_H_
#define _IM_SDK_CALLBACK_H_

typedef struct callback_st{

	/*连接相关回调**/
	void (*im_callback_connect_success)();
	void (*im_callback_connect_error)(char* error);

	/*登录相关回调**/
	void (*im_callback_login_result)(char* str, int len);

	/*已读消息函数**/
	void (*im_callback_read)(void* t, char* msg, int len);

	/*已写消息函数**/
	void (*im_callback_write)(char* msg, int len);

}*callback_t;

callback_t im_client_callback_init();
void	   im_client_callback_free(callback_t t);

#endif