/*
Flexboot command definitions
*/

#define LOOP							0x00
#define FLEXBOOT_REQUEST_ACK			0x01
#define FLEXBOOT_ACK					0x02
#define BOOT							0x04				//command to boot the application code
#define RESTART							0x05				//command to restart the target

#define FLEXBOOT_REQUEST_TARGET_READY	0x09


#define TARGET_READY					0x10				//command to send to host letting it know target is ready to accept data
#define TARGET_BUSY						0x11				//command to send to host letting it know target is busy
#define TARGET_ERROR					0x12                //command to send to host letting it kow target has encountered an error
#define TARGET_REQUEST_PREVIOUS			0x13				//command to send to host letting it know the target wants the prevoius block again
