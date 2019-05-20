#include "serial_port.h"
#include <stdio.h>
#include <string.h>

serial_port_t SerialPort_Create(char* port)
{
	int serial_port = open(port, O_RDWR);

	//Check for errors
	if (serial_port < 0) {
	    //printf("Error %i from open: %s\n", errno, strerror(errno));
	    return -1;
	}

	struct termios tty;
	memset(&tty, 0, sizeof tty);

	// Read in existing settings, and handle any error
	if(tcgetattr(serial_port, &tty) != 0) {
	    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	    return -1;
	}

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tty.c_lflag &= ~ICANON; //turn off canonical mode
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)

	tty.c_cc[VTIME] = 1;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 9600
	cfsetispeed(&tty, B115200);
	cfsetospeed(&tty, B115200);

	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
	    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	    return -1;
	}

	return serial_port;
}

int SerialPort_ToggleDTR(serial_port_t port)
{
	int cts_flag = TIOCM_DTR;
	ioctl(port, TIOCMBIC, &cts_flag);
	ioctl(port, TIOCMBIS, &cts_flag);
}

int SerialPort_Read(serial_port_t port, char* buffer, int buffer_length)
{
	// Read bytes. The behaviour of read() (e.g. does it block?,
	// how long does it block for?) depends on the configuration
	// settings above, specifically VMIN and VTIME
	int n = read(port, buffer, sizeof(buffer_length));
	// if(n > 0)
	// {
	// 	printf("read: %s\n", buffer);
	// }
	// else
	// {
	// 	printf("no data!\n");
	// }
	return n;
}

int SerialPort_Write(serial_port_t port, char* buffer, int buffer_length)
{
	write(port, buffer, buffer_length);
	return buffer_length;
}

void SerialPort_Destroy(serial_port_t port)
{
	close(port);
}