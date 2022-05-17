#pragma once

#include "xxtea.h"
#include "jni.h"
#include "jvmti.h"
#include "jni_md.h"

#ifndef _Included_helloworld_ByteCodeEncryptor
#define _Included_helloworld_ByteCodeEncryptor
#ifdef __cplusplus
extern "C" {
#endif
	/*
	 * Class:     helloworld_ByteCodeEncryptor
	 * Method:    encrypt
	 * Signature: ([B)[B
	 */
	JNIEXPORT jbyteArray JNICALL Java_helloworld_ByteCodeEncryptor_encrypt(JNIEnv*, jclass, jbyteArray);
	/*
	 * Class:     helloworld_ByteCodeEncryptor
	 * Method:    decrypt
	 * Signature: ([B)[B
	 */
	JNIEXPORT jbyteArray JNICALL Java_helloworld_ByteCodeEncryptor_decrypt(JNIEnv*, jclass, jbyteArray);
#ifdef __cplusplus
}
#endif
#endif
#ifndef CONST_HEADER_H_   
#define CONST_HEADER_H_    

#endif // CONST_HEADER_H_  
