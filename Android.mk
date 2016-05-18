
LOCAL_PATH := $(call my-dir)

LOCAL_MODULE := libimcore
LOCAL_MODULE_FILENAME := libimcore

local_c_includes :=	\
	$(LOCAL_PATH)/include	\
	$(LOCAL_PATH)/../imProtocal-protobuf/include

local_cflags := -DDEBUG

LOCAL_SRC_FILES :=	\
	src/logproxy.c	\
	src/imutil.c	\
	src/client.c	\
	src/callback.c	\
	src/jni.c	\
	src/jnicontext.c
	

LOCAL_C_INCLUDES += $(local_c_includes)
LOCAL_EXPORT_C_INCLUDES += $(local_c_includes)

LOCAL_CFLAGS := $(local_cflags)
LOCAL_EXPORT_CFLAGS := $(local_cflags)

LOCAL_SHARED_LIBRARIES := libimprotocal

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-module,imProtocal-protobuf)

