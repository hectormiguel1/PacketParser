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
    tuple<ERROR_CODE,string> readString();
    tuple<ERROR_CODE,short> readShort();
    tuple<ERROR_CODE,int> readInt();
    tuple<ERROR_CODE, int8_t> readByte();
    tuple<ERROR_CODE, uint8_t > readUByte();
    tuple<ERROR_CODE,float> readFloat();
    tuple<ERROR_CODE,unsigned short> readUShort();
    tuple<ERROR_CODE,string> readGUID();
    ERROR_CODE moveToNextPacket(void *, int);
    int getLengthRemaining();
};


#endif //KIPACKETPARSER_PARSER_H
