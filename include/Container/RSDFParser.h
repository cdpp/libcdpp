#ifndef RSDFPARSER_H
#define RSDFPARSER_H
#include <string>
#include <vector>

namespace cdpp {
    class RSDFParser
    {
        public:
            RSDFParser();
            virtual ~RSDFParser();
            char* parse(char* encrypted);
            std::vector<std::string> parseFile(std::string filename);
        protected:
        private:
            final unsigned char key_[] = {  0x8C, 0x35, 0x19, 0x2D, 0x96, 0x4D,
                                            0xC3, 0x18, 0x2C, 0x6F, 0x84, 0xF3,
                                            0x25, 0x22, 0x39, 0xEB, 0x4A, 0x32,
                                            0x0D, 0x25, 0x00, 0x00, 0x00, 0x00 };
            final unsigned char iv_[] = {   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                            0xFF, 0xFF, 0xFF, 0xFF };
    };
}
#endif // RSDFPARSER_H
