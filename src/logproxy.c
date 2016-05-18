
#include <logproxy.h>

#define MAX_SIZE 512

void im_sdk_log_stderr_(const char* file, int line, const char *msgfmt, ...){
	va_list ap;
	char *pos, message[MAX_SIZE];
	int sz;

	snprintf(message, MAX_SIZE, "sx (%s:%d) ", file, line);

	for (pos = message; *pos != '\0'; pos++);
	sz = pos - message;

	va_start(ap, msgfmt);
	vsnprintf(pos, MAX_SIZE - sz, msgfmt, ap);
	va_end(ap);

	fprintf(stderr, "%s", message);
	fprintf(stderr, "\n");

	fflush(stderr);
}

log_t im_sdk_log_init(int type, log_type_t fc){
	log_t ts = calloc(1,sizeof(struct log_st));
	switch (type){
		case im_sdk_log_stderr:
			ts->log_debug = im_sdk_log_stderr_;
			break;
		case im_sdk_log_android:
			ts->log_debug = fc;
			break;
	}
	return ts;
}

void  im_sdk_log_free(log_t t){
	free(t);
}
