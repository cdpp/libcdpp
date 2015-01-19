#ifndef AES_H_INCLUDED
#define AES_H_INCLUDED

int aes_decrypt_ecb(unsigned char* decrypted, unsigned char* encrypted, int length, unsigned char* key, unsigned char* iv);

#endif // AES_H_INCLUDED
