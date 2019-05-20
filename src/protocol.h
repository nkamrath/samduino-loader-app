/*
Firmware update message protocol structures
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define MAX_DATA_LEN 1024           //max size of the data payload portion in a flexboot message

//message id defines
#define CODE_BLOCK_MSG_ID			2   //id for code message
#define COMMAND_MSG_ID				3   //id for command message.  command itself will be in the data portion of message
#define IMAGE_METADATA_MSG_ID		6   //id for firmware image meta data packet.  contains info such as size

typedef struct
{
    uint8_t msgId;                  //indicates the message type
    uint16_t msgLen;                //indicates the length of the data portion of the message
    uint16_t sequenceNum;           //standard sequence number
    uint16_t msgChecksum;            //indicates the checksum of the message for error checking on the data block (only for data portion)
    uint8_t data[MAX_DATA_LEN];     //data payload portion of the message
}SamduinoBootMsg;

typedef struct  
{
	uint32_t numBytes;				//number of bytes in the coming firmware image
	uint32_t numMessages;			//number of messages that will be used to transmit the firmware image
}ImageMetaData;

#endif //protocol.h