// C library headers
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "serial_port.h"
#include "protocol.h"
#include "bootCommands.h"
#include "fletcher16.h"

#define MAX_FIRMWARE_SIZE_BYTES (512*1024)
#define NUM_MESSAGE_RETRIES 10

int writeMsg(serial_port_t port, SamduinoBootMsg* msg)
{

    int bytesWritten = 0;
    bytesWritten += SerialPort_Write(port, (char*)&msg->msgId, 1);

    bytesWritten += SerialPort_Write(port, (char*)&msg->msgLen, 2);

    bytesWritten += SerialPort_Write(port, (char*)&msg->sequenceNum, 2);

    bytesWritten += SerialPort_Write(port, (char*)&msg->msgChecksum, 2);

    bytesWritten += SerialPort_Write(port, (char*)&msg->data[0], msg->msgLen);

    return 1;
}

int getResponse(serial_port_t port, uint8_t* buffer, int buffer_length)
{
    int timeoutCounter = 0;
    while(timeoutCounter < NUM_MESSAGE_RETRIES) //sleep while we wait for target to be ready for next block
    {
        int bytes_read = SerialPort_Read(port, buffer, buffer_length);
        if(bytes_read > 0)
        {
            return bytes_read;
        }

        timeoutCounter++;
        usleep(100000);
    }
    printf("get response timeout\n");
    return -1; //timeout no message
}

int main(int argc, char **argv)
{
	printf("Attempting to connect on port %s...\n", argv[1]);
	serial_port_t serial_port;
	for(int i = 0; i < 50; i++)
	{
		printf(".\n");
		serial_port = SerialPort_Create(argv[1]);
		if(serial_port > 0)
		{
			SerialPort_ToggleDTR(serial_port);
			SerialPort_Destroy(serial_port);
			usleep(1000000);
			break;
		}
		else
		{
			usleep(100000);
		}
	}

	for(int i = 0; i < 50; i++)
	{
		printf(".\n");
		serial_port = SerialPort_Create(argv[1]);
		if(serial_port > 0)
		{
			break;
		}
		else
		{
			usleep(100000);
		}
	}
	printf("\n");

	printf("loading firmware file %s\n", argv[2]);
	FILE *f = NULL;
	char firmware_image[MAX_FIRMWARE_SIZE_BYTES];
	f=fopen(argv[2],"rb");
	if(f==NULL)
	{
		printf("Error opening fimrware image file\n");
		return -1;
	}

	int image_size_bytes = 0;
	while(!feof(f))
	{
		firmware_image[image_size_bytes] = fgetc(f);
	    //fread(&firmware_image[image_size_bytes],sizeof(int),1,f);
	    image_size_bytes++;
	}
	fclose(f);


	//create ack request msg for retries
    SamduinoBootMsg ackRequestMsg;
    ackRequestMsg.msgId = COMMAND_MSG_ID;
    ackRequestMsg.msgLen = 1;
    ackRequestMsg.data[0] = FLEXBOOT_REQUEST_TARGET_READY;

    //create flexboot message for sending firmware
    SamduinoBootMsg msg;
    msg.msgId = CODE_BLOCK_MSG_ID;
    msg.msgLen = 512;
    msg.msgChecksum = 0; //0 unimplemented
    uint8_t* dataPtr;

    int blocksNeeded = image_size_bytes / msg.msgLen;
    int blockRemainder = image_size_bytes % msg.msgLen;//msg.msgLen % bytes.size();
    //state = UPLOADING_FIRMWARE;
    printf("uploading firmware...\n");
    printf("Firmware Size in bytes: %d\n", image_size_bytes);
    printf("Blocks: %d\n", blocksNeeded);
    printf("Remainder: %d\n", blockRemainder);

    //need to create the meta data message
    ImageMetaData imageMetaData;
    imageMetaData.numBytes = image_size_bytes;

    uint8_t responseBuffer[256];

    SamduinoBootMsg imageMetaDataMsg;
    imageMetaDataMsg.msgId = IMAGE_METADATA_MSG_ID;
    imageMetaDataMsg.msgLen = 4;
    memcpy(imageMetaDataMsg.data, &imageMetaData.numBytes, sizeof(imageMetaData.numBytes));

    int responseLength;
    //state = WAITING_FOR_READY;
    //retries for image meta data send
    for(int i = 0; i < 20; i++)
    {
        writeMsg(serial_port, &imageMetaDataMsg);
        responseLength = getResponse(serial_port, responseBuffer, sizeof(responseBuffer));
        if(responseLength == -1 && i == 19)
        {
            printf("Timeout occurred during firmware upload: no response to image metadata.\n");
            return -1;
        }
        else if(responseBuffer[0] == TARGET_READY)
        {
            //target got meta data
            printf("Image metadata ack'ed\n");
            break;
        }
        else if(responseLength != -1)
        {
            printf("Got unexpected response!\n");
        }
        printf("no meta data response\n");
        usleep(100000);
    }


    int corruptedBlocks = 0;

    //split message up into blocks and send
    int consecutiveTimeouts = 0;
    int consecutiveTimeoutLimit = 10;
    for(int block = 0; block < blocksNeeded; block++)
    {
        //cout << "Sending block : " << block << endl;

        // progressBar->setValue((block/float(blocksNeeded)) * 100);
        // progressBar->update();
        // progressBar->repaint();

        dataPtr = &firmware_image[(block*msg.msgLen)];
        memcpy(msg.data, dataPtr, msg.msgLen);
        msg.msgChecksum = fletcher16(msg.data, msg.msgLen);
        //msg.msgChecksum = fletcher16(dataPtr, msg.msgLen);
        writeMsg(serial_port, &msg);
        //state = WAITING_FOR_READY;
        responseLength = getResponse(serial_port, responseBuffer, sizeof(responseBuffer));
        if(responseLength == -1)
        {
            if(consecutiveTimeouts == consecutiveTimeoutLimit)
            {
                //timeout, no response
                printf("Timeout occurred during firmware update\n");
                return -1;
            }
            consecutiveTimeouts++;
            printf("Consecutive timeouts: %d\n", consecutiveTimeouts);
            block--;
            continue;
        }
        else
        {
            consecutiveTimeouts = 0;
            if(responseLength > 0)
            {
                if(responseBuffer[0] == TARGET_READY)
                {
                    //state = UPLOADING_FIRMWARE;
                }
                else if(responseBuffer[0] == TARGET_REQUEST_PREVIOUS)
                {
                    //if the target wants the previous block, back up an index so it resends the same code again
                    printf("TARGET REQUESTED PREVIOUS\n");
                    corruptedBlocks++;
                    block--;
                }
                else if(responseBuffer[0] == TARGET_ERROR)
                {
                    printf("An error occurred on the target while uploading.  Please retry.\n");
                    return -1;
                }
                else
                {
                    printf("Unknown ack response received from target.  Please Retry\n");
                    return 0;
                }
            }
        }
    }

    //handle remaining bytes if code doesn't fit into even block size
    if(blockRemainder)
    {
        printf("Sending Remainder Block of size: %d\n", blockRemainder);
        dataPtr = &firmware_image[(blocksNeeded*msg.msgLen)];
        memcpy(msg.data, dataPtr, blockRemainder);
        msg.msgLen = blockRemainder;
        msg.msgChecksum = fletcher16(dataPtr, blockRemainder);
        writeMsg(serial_port, &msg);
        usleep(200000);
    }

    printf("Total corrupted blocks: %d\n", corruptedBlocks);

    printf("Finished firmware update!\n");

    msg.msgId = COMMAND_MSG_ID;
    msg.msgLen = 1;
    msg.data[0] = BOOT;

    writeMsg(serial_port, &msg);
    //wait for ack
    //serialPort.waitForReadyRead(200);
    
    int response_length = SerialPort_Read(serial_port, responseBuffer, sizeof(responseBuffer));
    if(response_length > 0)
    {
        //QByteArray bytes1 = serialPort.readAll();
        //cout << "Bytes[0]: " << (int)bytes1.data()[0] << endl;
        bool finalAck = false;
        for(int i = 0; i < response_length; i++)
        {
            if(responseBuffer[i] == FLEXBOOT_ACK)
            {
                finalAck = true;
                break;
            }
        }

        if(finalAck)
        {
            printf("Got final ack from target, target is now updated and booting\n");
        }
    }
    else
    {
        printf("Did not find final ack from target after boot command!\n");
    }
	

	SerialPort_Destroy(serial_port);

	return 0;
}