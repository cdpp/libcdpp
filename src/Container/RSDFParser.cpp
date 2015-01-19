#include "Container/RSDFParser.h"
using namespace cdpp;

RSDFParser::RSDFParser()
{
    //ctor
}

RSDFParser::~RSDFParser()
{
    //dtor
}

char* RSDFParser::parse(char* encrypted)
{
//     key = binascii.unhexlify("8C35192D964DC3182C6F84F3252239EB4A320D2500000000")
//
//    iv = binascii.unhexlify("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF")
//    IV_Cipher = AES.new(key, AES.MODE_ECB)
//    iv = IV_Cipher.encrypt(iv)
//
//    aes = AES.new(key, AES.MODE_CFB, iv)
//    data = binascii.unhexlify("".join(rsdf.split())).splitlines()
//
//    urls = []
//    for link in data:
//        link = base64.b64decode(link)
//        link = aes.decrypt(link)
//        link = link.replace("CCF: ","")
//        urls.append(link)

//    return urls
    return NULL;
}
