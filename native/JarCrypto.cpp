#include <iostream>
#include "JarCrypto.h"

const char* key = "your secret keys here";
extern"C" JNIEXPORT jbyteArray JNICALL Java_helloworld_ByteCodeEncryptor_encrypt(JNIEnv* env, jclass cla, jbyteArray j_array)
{
	char* dst = (char*)env->GetByteArrayElements(j_array, 0);
	int len_arr = env->GetArrayLength(j_array);
	size_t len;
	unsigned char* encrypt_data = (unsigned char*)xxtea_encrypt(dst, len_arr, key, &len);
	jbyteArray c_result = env->NewByteArray(len);
	env->SetByteArrayRegion(c_result, 0, len, (jbyte*)encrypt_data);
	return c_result;
}
extern"C" JNIEXPORT jbyteArray JNICALL Java_helloworld_ByteCodeEncryptor_decrypt(JNIEnv * env, jclass cla, jbyteArray j_array)
{
	char* dst = (char*)env->GetByteArrayElements(j_array, 0);
	int len_arr = env->GetArrayLength(j_array);
	size_t len;
	unsigned char* encrypt_data = (unsigned char*)xxtea_decrypt(dst, len_arr, key, &len);
	jbyteArray c_result = env->NewByteArray(len);
	env->SetByteArrayRegion(c_result, 0, len, (jbyte*)encrypt_data);
	return c_result;
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
	//cafebabe---*.class
	if (name && class_data[0] != 0xca && class_data[1] != 0xfe && class_data[2] != 0xba && class_data[3] != 0xbe) {
		size_t len;
		unsigned char* decrypt_data = (unsigned char*)xxtea_decrypt(class_data, class_data_len, key, &len);
		*new_class_data_len = len;
		jvmti_env->Allocate(len, new_class_data);
		unsigned char* _data = *new_class_data;
		for (int i = 0; i < class_data_len; i++)
		{
			_data[i] = decrypt_data[i];
		}
	}
	else {
		*new_class_data_len = class_data_len;
		jvmti_env->Allocate(class_data_len, new_class_data);
		unsigned char* _data = *new_class_data;
		for (int i = 0; i < class_data_len; i++)
		{
			_data[i] = class_data[i];
		}
		
	}
}
JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM* vm, char* options, void* reserved)
{
	printf("Agent_OnLoad\n");
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
	capabilities.can_tag_objects = 1;
	capabilities.can_generate_object_free_events = 1;
	capabilities.can_get_source_file_name = 1;
	capabilities.can_get_line_numbers = 1;
	capabilities.can_generate_vm_object_alloc_events = 1;

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


