//
// Created by hector on 7/27/20.
//

#include <cstring>
#include "parser.h"



//Struct used when converting unsigned int base 32 to float.
//uint32_t is needed to covert from big endian to little endian.
//union allows us to reinterpret the bits as floats.
static union {
    uint32_t from;
    float to;
}uint32_to_float;

//Initializes the parser class with a pointer to the packet location (void is used, as unknown type of data)
parser::parser(void * packerPointer, size_t packetLength) {
this->packetData = packerPointer;
this->length = packetLength;
this->currentLocationInPacket = ZERO_INITIALIZER;
}

/**
 * This function reset the internal pointer into the packet.
 * @Return: Always returns no ERROR.
 */
ERROR_CODE parser::reset() {
    currentLocationInPacket = ZERO_INITIALIZER;
    return NO_ERROR;
}

/**
 * this function moves the pointer to the packet n times.
 * @param: int: numToRead, number of bytes to read.
 * @return: Returns NO_ERROR on successful skip, READ_OUT_OF_BOUNDS_ATTEMPTED, if skip lands outside of packet.
 */
ERROR_CODE parser::readN(int numToRead) {
    if (numToRead + currentLocationInPacket >= length)
    {
        return READ_OUT_OF_BOUNDS_ATTEMPTED;
    } else
    {
        currentLocationInPacket += numToRead;
        return NO_ERROR;
    }
}
/**
 * This functions reads a string from the packet and moves the internal location in the packet by the length of the string
 * @returns: tuple composed of the ERROR_CODE, and a string, NO_ERROR,resulting_string if success,
 *          otherwise, READ_OUT_OF_BOUNDS_ATTEMPTED, errorString if length of string exceeds length of packet.
 */
string parser::readString() {
    auto [error_code, stringLength] = readUShort();
    if(stringLength + currentLocationInPacket >= length || error_code > NO_ERROR)
    {
        if(error_code > NO_ERROR)
        {
            return string("Error getting size of String");
        } else {
            return string("Tried to read outside the packet. (current location in packet + string size > packet length");
        }
    }
    else {
        char * startPointPtr = ((char *) packetData + currentLocationInPacket);
        string resultString(startPointPtr, (currentLocationInPacket + stringLength));
        currentLocationInPacket += stringLength;
        return resultString;
    }
}
/**
 * This function extracts a short from the packet, it reads from the current location in the packet.
 * Converts the short from Big Endian to Little Endian.
 * @return: tuple<ERROR_CODE, short>, NO_ERROR,short on success, READ_OUT_OF_BOUNDS_ATTEMPTED, ERROR_RETURN_VAL
 * if length of string exceeds length of packet.
 */
short parser::readShort() {
    if(currentLocationInPacket + SIZE_OF_SHORT >= length)
    {
        return ERROR_RETURN_VAL;
    }
    else {
        char * startPointPtr = ((char *)packetData + currentLocationInPacket);
        uint16_t destShort = ZERO_INITIALIZER;
        memcpy(startPointPtr, &destShort, SIZE_OF_SHORT);
        currentLocationInPacket += SIZE_OF_SHORT;
        //This converts short from big endian to little endian. (ntohl)
        short finalShort = ntohs(destShort);

        return finalShort;
    }
}
/**
 * This function extracts an int from the current location in the packet.
 * Converts from Big Endian to Little Endian.
 * @return: Returns a tuple containing Error Code and int.
 */
int parser::readInt() {
    if(currentLocationInPacket + SIZE_OF_INT >= length)
    {
        return ERROR_RETURN_VAL;
    } else{
        char * startPointPtr = ((char * ) packetData) + currentLocationInPacket;
        uint32_t destInt = ZERO_INITIALIZER;
        memcpy(startPointPtr,&destInt,SIZE_OF_INT);
        currentLocationInPacket += SIZE_OF_INT;
        //This converts short from big endian to little endian. (ntohl)
        int finalInt = ntohl(destInt);
        return finalInt;
    }
}

/**
 * This function extracts a byte from the current location in the packet.
 * Converts from Big Endian to Little Endian.
 * @return: Returns a tuple containing Error Code and int.
 */
int8_t parser::readByte() {
    if(currentLocationInPacket + SIZE_OF_BYTE >= length)
    {
        return ERROR_RETURN_VAL;
    } else{
        char* startPointPtr = ((char *) packetData) + currentLocationInPacket;
        uint8_t resultByte = 0;
        memcpy(startPointPtr,&resultByte,SIZE_OF_BYTE);
        //This converts short from big endian to little endian. (ntohl)
        int8_t finalByte = (ntohs(resultByte));
        currentLocationInPacket += SIZE_OF_BYTE;
        return finalByte;
    }
}
/**
 * This function extracts an unsigned byte from the current location in the packet.
 * Converts from Big Endian to Little Endian.
 * @return: Returns a tuple containing Error Code and int.
 */
uint8_t parser::readUByte() {
    if(currentLocationInPacket + SIZE_OF_BYTE >= length)
    {
        return ERROR_RETURN_VAL;
    } else{
        char* startPointPtr = ((char *) packetData) + currentLocationInPacket;
        uint8_t resultByte = 0;
        memcpy(startPointPtr,&resultByte,SIZE_OF_BYTE);
        //This converts short from big endian to little endian. (ntohl)
        uint8_t finalByte = (ntohs(resultByte));
        currentLocationInPacket += SIZE_OF_BYTE;
        return finalByte;
}

/**
 * This function extracts a float from our current location in the packet.
 * Converts the float from Big Endian to Little Endian.
 * @Return: Returns tuple with Error code if eny and the float, ERROR_RETURN_VAL on error.
 */
float parser::readFloat() {
    if(currentLocationInPacket + SIZE_OF_FLOAT >= length)
    {
        return float(ERROR_RETURN_VAL);
    } else{
        char * startPointPtr = ((char *) packetData) + currentLocationInPacket;
        uint32_to_float.from = ZERO_INITIALIZER;
        memcpy(&startPointPtr, &uint32_to_float,SIZE_OF_FLOAT);
        currentLocationInPacket += SIZE_OF_FLOAT;
        //This converts short from big endian to little endian. (ntohl)
        uint32_to_float.from = ntohl(uint32_to_float.from);
        return uint32_to_float.to;
    }
}
/**
 * This function extracts a short from the packet, it reads from the current location in the packet.
 * Converts the short from Big Endian to Little Endian.
 * @return: tuple<ERROR_CODE, short>, NO_ERROR,short on success, READ_OUT_OF_BOUNDS_ATTEMPTED, 0
 * if length of string exceeds length of packet.
 */
unsigned short parser::readUShort() {
    if(currentLocationInPacket + SIZE_OF_UNSIGNED_SHORT >= length)
    {
        return ERROR_RETURN_VAL;
    } else{
        char * startPointPtr = ((char *)packetData + currentLocationInPacket);
        uint16_t destShort = ZERO_INITIALIZER;
        memcpy(startPointPtr, &destShort, SIZE_OF_UNSIGNED_SHORT);
        currentLocationInPacket += SIZE_OF_UNSIGNED_SHORT;
        //This converts short from big endian to little endian. (ntohl)
        unsigned short finalShort = ntohs(destShort);

        return finalShort;
    }
}

/**
 *This function returns the GUID extracted from the current location in the packet.
 * @return NO_ERROR,GUID on success, READ_OUT_OF_BOUNDS_ATTEMPTED,error_string on failure.
 */
string parser::readGUID() {
    if(currentLocationInPacket + SIZE_OF_GID > length)
    {
        return string("Tried to read passed the size of the packet!");
    } else{
        char * startPointPtr = ((char *) packetData + currentLocationInPacket);
        string resultString(startPointPtr, (currentLocationInPacket + SIZE_OF_GID));
        currentLocationInPacket += SIZE_OF_GID;
        return resultString);
    }
}
/**
 * Function to start working on next packet without creating another instance of the class.
 * Function also reset the location of the internal location indicator.
 * @param nextPacket : pointer to next packet.
 * @param packetSize: Size of the new packet.
 * @return : Always returns NO_ERROR.
 */
ERROR_CODE parser::moveToNextPacket(void * nextPacket, int packetSize) {
    packetData = nextPacket;
    length = packetSize;
    reset();
    return NO_ERROR;
}

/**
 * Function returns remaining space in packet from the current pointer to the end of the packet
 * @return int remaining space in packet.
 */
int parser::getLengthRemaining() {
    return (length - currentLocationInPacket);
}






