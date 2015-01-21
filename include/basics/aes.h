#ifndef AES_H_INCLUDED
#define AES_H_INCLUDED

#define CDPP_AES_ECB 1
#define CDPP_AES_CFB 2
int aes_decrypt(unsigned char* decrypted, unsigned char* encrypted, int length, unsigned char* key, unsigned char* iv, uint8_t type);

#endif // AES_H_INCLUDED
