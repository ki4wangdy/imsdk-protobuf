/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_imrub_shoulder_module_im_server_IMService */

#ifndef _Included_com_imrub_shoulder_module_im_server_IMService
#define _Included_com_imrub_shoulder_module_im_server_IMService
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_imrub_shoulder_module_im_server_IMService
 * Method:    initImserver
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_initImserver
  (JNIEnv *, jobject);

/*
 * Class:     com_imrub_shoulder_module_im_server_IMService
 * Method:    desotryserver
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_desotryserver
  (JNIEnv *, jobject);

/*
 * Class:     com_imrub_shoulder_module_im_server_IMService
 * Method:    isconnected
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_imrub_shoulder_module_im_server_IMService_isconnected
  (JNIEnv *, jobject);

/*
 * Class:     com_imrub_shoulder_module_im_server_IMService
 * Method:    sendMsgNative
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_sendMsgNative
  (JNIEnv *, jobject, jstring, jstring, jstring,jstring,jstring);

/*
 * Class:     com_imrub_shoulder_module_im_server_IMService
 * Method:    login
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_imrub_shoulder_module_im_server_IMService_login
  (JNIEnv *, jobject, jstring, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif