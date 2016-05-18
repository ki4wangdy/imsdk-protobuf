
#include <client.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <message.h>
#include <message_const.h>
#include <imutil.h>

struct client_st{
	int isconnect;
	int fd;
	char ip[20];
	int  port;
	long readcount;
	long writecount;
	log_t log;
	callback_t cb;
};

void im_client_set_log(client_t ts, log_t t){
	ts->log = t;
}

void im_client_set_callback(client_t ts, callback_t cb){
	ts->cb = cb;
}

client_t  im_client_init(){
	client_t ts = calloc(1,sizeof(struct client_st));
	return ts;
}

int im_client_connect(client_t t, char* ip, int port){

	int cfd;
	int recbytes;
	int sin_size;
	char buffer[1024] = "";   

	struct sockaddr_in s_add,c_add;

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd < 0){
		if(t->log){
			t->log->log_debug(ZONE,"socket fail !");
		}
		if (t->cb){
			t->cb->im_callback_connect_error("socket fail !");
		}
		return false;
	}

	if(t->log){
		t->log->log_debug(ZONE,"socket ok, the fd:%d",cfd);
	}

	int timeout = 30 * 1000;
	setsockopt( cfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof( timeout ));

	int opt=1;
	setsockopt(cfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family = AF_INET;
	s_add.sin_addr.s_addr = inet_addr(ip);
	s_add.sin_port = htons(port);

	if(t->log){
		t->log->log_debug(ZONE,"s_addr = %#x ,port : %#x",
			s_add.sin_addr.s_addr,s_add.sin_port);
	}

	int code = connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr));
	if(code < 0){
		if(t->log){
			t->log->log_debug(ZONE,"connect fail !");
		}
		if (t->cb){
			t->cb->im_callback_connect_error("connect fail !");
		}
		return false;
	}

	memcpy(t->ip,ip,strlen(ip));
	t->port = port;

	t->fd = cfd;
	t->isconnect = true;

	if(t->log){
		t->log->log_debug(ZONE,"the connect success's fd:%d !",t->fd);
	}

	if (t->cb){
		t->cb->im_callback_connect_success();
	}
	return true;
}

int im_client_login(client_t t, char* uid, char* token, int* body_len){
	// start to login
	int strlen = 0;
	char* str = im_sdk_login_string(uid,token,&strlen);
	int code = im_client_send(t,str,strlen);
	if(code == false){
		if (t->log){
			t->log->log_debug(ZONE, "send fail !");
		}
		if(t->cb){
			t->cb->im_callback_connect_error("send fail!\n");
		}
		free(str);
		return false;
	}
	free(str);

	do{
		int code = im_client_rec(t);
		if(code == false){
			close(t->fd);
			if (t->log){
				t->log->log_debug(ZONE, "recevied fail !");
			}
			if(t->cb){
				t->cb->im_callback_connect_error("recevied fail");
			}
			t->isconnect = false;		
			return false;
		}
	}while(im_client_isconnected(t));
	return true;
}

int im_client_rec(client_t t){
	if(!t->isconnect || t->fd == 0){
		if (t->log){
			t->log->log_debug(ZONE, "im_client_rec,!t->isconnect || t->fd == 0 %d,%d !",t->isconnect,t->fd);
		}
		return false;
	}
	int capacity = 1024;
	char* buf = calloc(1,capacity);
	char* temp = buf;
	int readedSize = 0;
	int index = 0;
	int rsize = 0;
	int iscontinue = 0;
	int indx = 0;
	int len = 0;
	do{
		// 1.read
		char tempBuf[1024];
		rsize = read(t->fd,tempBuf,1024);
		if(rsize < 0){
			free(buf);
			if (t->log){
				t->log->log_debug(ZONE, "read fail , count < 0 !");
			}
			close(t->fd);
			t->isconnect = false;
			if (t->cb){
				t->cb->im_callback_connect_error("read fail, the reason is %s !");
			}
			return false;
		}
		iscontinue = 0;
		if(len + rsize >= capacity){
			capacity = 10 * capacity;
			buf = realloc(temp,capacity);
			temp = buf;
			iscontinue = 1;
		}
		if(rsize != 0){
			memcpy(buf + len,tempBuf,rsize);
			len += rsize;
		}
	}while(iscontinue);
	t->readcount += len;
	if (t->cb){
		t->cb->im_callback_read(t, temp, len);
	}
	free(temp);
	return len == 0 ? false : true;
}

void im_client_cb_read(client_t t, char* temp, int len){
	if (t && t->cb){
		t->cb->im_callback_read(t, temp, len);
	}
}

int im_client_send(client_t t,char* msg, int len){
	if(!t->isconnect || t->fd == 0 ){
		if (t->log){
			t->log->log_debug(ZONE, "im_client_send,!t->isconnect || t->fd == 0 %d,%d !",t->isconnect,t->fd);
		}
		return false;
	}
	int count = write(t->fd,msg,len);
	if(count >= 0){
		t->writecount += count;
		if (t->cb){
			t->cb->im_callback_write(msg, count);
		}
		return count;
	}
	t->isconnect = false;
	close(t->fd);
	if(t->cb){
		t->cb->im_callback_connect_error("the write is error");
	}
	return false;
}

int im_client_isconnected(client_t t){
	return t->isconnect;
}

char* im_client_getserver(client_t t){
	int size = strlen(t->ip)+1;
	char* ip = calloc(1,size);
	memcpy(ip,t->ip,size);
	return ip;
}

int im_client_getserver_port(client_t t){
	return t->port;
}

void im_client_disconnect(client_t t){
	assert(t);
	close(t->fd);
	t->isconnect = false;
}

void im_client_debug(client_t t, const char *msgfmt, ...){
	if (t){
		t->log->log_debug(ZONE,msgfmt);
	}
}

void im_client_free(client_t t){
	assert(t);
	im_client_callback_free(t->cb);
	im_sdk_log_free(t->log);
	free(t);
}





