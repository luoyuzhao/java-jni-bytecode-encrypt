#include <iostream>
#include "JarCrypto.h"
#include "des.h"

const char* key = "your key here";

extern"C" JNIEXPORT jbyteArray JNICALL Java_helloworld_ByteCodeEncryptor_encrypt(JNIEnv* env, jclass cla, jbyteArray j_array)
{
	char* dst = (char*)env->GetByteArrayElements(j_array, 0);
	des_init(0);
	des_set_key((char*)key);
	des_encrypt(dst);
	des_done();
	env->SetByteArrayRegion(j_array, 0, strlen(dst), (jbyte*)dst);
	return j_array;
}

void JNICALL ClassDecryptCallback(
	jvmtiEnv* jvmti_env,
	JNIEnv* jni_env,
	jclass class_being_redefined,
	jobject loader,
	const char* name,
	jobject protection_domain,
	jint class_data_len,
	const unsigned char* class_data,
	jint* new_class_data_len,
	unsigned char** new_class_data)
{
	//printf("%x", static_cast<unsigned>(class_data[0]));//ca
	//printf("%x", static_cast<unsigned>(class_data[1]));//fe
	//printf("%x", static_cast<unsigned>(class_data[2]));//ba
	//printf("%x", static_cast<unsigned>(class_data[3]));//be 
	*new_class_data_len = class_data_len;
	jvmti_env->Allocate(class_data_len, new_class_data);
	unsigned char* _data = *new_class_data;
	if (name && class_data[0] != 0xca && class_data[1] != 0xfe && class_data[2] != 0xba && class_data[3] != 0xbe) {
		for (int i = 0; i < class_data_len; i++)
		{
			_data[i] = class_data[i];
		}
		des_init(0);
		des_set_key((char*)key);
		des_decrypt((char*)_data);
		des_done();
	}
	else {
		for (int i = 0; i < class_data_len; i++)
		{
			_data[i] = class_data[i];
		}
	}
}
jint setAgent(JavaVM* vm, char* options, void* reserved) {
	jvmtiEnv* jvmti;
	//Create the JVM TI environment(jvmti)
	jint ret = vm->GetEnv((void**)&jvmti, JVMTI_VERSION);
	if (JNI_OK != ret)
	{
		printf("ERROR: Unable to access JVMTI!\n");
		return ret;
	}
	jvmtiCapabilities capabilities;
	(void)memset(&capabilities, 0, sizeof(capabilities));

	capabilities.can_generate_all_class_hook_events = 1;
	capabilities.can_retransform_classes = 1;
	capabilities.can_retransform_any_class = 1;

	jvmtiError error = jvmti->AddCapabilities(&capabilities);
	if (JVMTI_ERROR_NONE != error)
	{
		printf("ERROR: Unable to AddCapabilities JVMTI!\n");
		return error;
	}

	jvmtiEventCallbacks callbacks;
	(void)memset(&callbacks, 0, sizeof(callbacks));

	callbacks.ClassFileLoadHook = &ClassDecryptCallback;
	error = jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
	if (JVMTI_ERROR_NONE != error) {
		printf("ERROR: Unable to SetEventCallbacks JVMTI!\n");
		return error;
	}

	error = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
	if (JVMTI_ERROR_NONE != error) {
		printf("ERROR: Unable to SetEventNotificationMode JVMTI!\n");
		return error;
	}
	return JNI_OK;
}
JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM* vm, char* options, void* reserved)
{
	printf("Agent_OnAttach\n");
	return setAgent(vm, options, reserved);
}
JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM* vm, char* options, void* reserved)
{
	printf("Agent_OnLoad\n");
	return setAgent(vm, options, reserved);
}


