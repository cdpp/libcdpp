#ifndef BASE64_H
#define BASE64_H


namespace cdpp {
	namespace base64 {
		int calcDecodeLength(const char* b64input);
		int Base64Decode(char* b64message, char* buffer);
		int Base64Encode(const char* message, char** buffer);
	}
}
#endif // BASE64_H
