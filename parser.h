//
// Created by hector on 7/27/20.
//

#ifndef KIPACKETPARSER_PARSER_H
#define KIPACKETPARSER_PARSER_H

#include <string>
#include <cstddef>
#include <tuple>
#include <arpa/inet.h>
#include <endian.h>

#define ZERO_INITIALIZER 0
#define SIZE_OF_SHORT 2
#define SIZE_OF_INT 4
#define SIZE_OF_BYTE 1
#define SIZE_OF_FLOAT 4
#define SIZE_OF_UNSIGNED_SHORT 2
#define SIZE_OF_GID 8

#define ERROR_RETURN_VAL 0
using namespace std;
enum ERROR_CODE {
    NO_ERROR, ERROR, READ_OUT_OF_BOUNDS_ATTEMPTED
};
class parser {

private:
    void *packetData;
    int length;
    int currentLocationInPacket;

public:
    parser(void *,size_t);
    ERROR_CODE reset();
    ERROR_CODE readN(int);
    string readString();
    short readShort();
    int readInt();
    int8_t readByte();
    uint8_t readUByte();
    float readFloat();
    unsigned short readUShort();
    string readGUID();
    ERROR_CODE moveToNextPacket(void *, int);
    int getLengthRemaining();
};


#endif //KIPACKETPARSER_PARSER_H
