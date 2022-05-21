
#ifndef DES_INCLUDED
#define DES_INCLUDED

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
	int  des_init(int mode);
	void des_set_key(char* key);
	void des_encrypt(char* block);
	void des_decrypt(char* block);
	void des_done(void);
#ifdef __cplusplus
}
#endif
#endif

