#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/ioctl.h>

typedef int serial_port_t;

serial_port_t SerialPort_Create(char* port);
int SerialPort_ToggleDTR(serial_port_t port);
int SerialPort_Read(serial_port_t port, char* buffer, int buffer_length);
int SerialPort_Write(serial_port_t port, char* buffer, int buffer_length);
void SerialPort_Destroy(serial_port_t port);

#endif