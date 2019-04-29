#include "I2CAccess.h"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cerrno>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <cstring>
#include <iostream>

// I2C definitions

//#define I2C_SLAVE	0x0703
#define I2C_SMBUS	0x0720	/* SMBus-level access */

#define I2C_SMBUS_READ	1
//#define I2C_SMBUS_WRITE	0

// SMBus transaction types

//#define I2C_SMBUS_QUICK		    0
//#define I2C_SMBUS_BYTE		    1
//#define I2C_SMBUS_BYTE_DATA	    2
//#define I2C_SMBUS_WORD_DATA	    3
//#define I2C_SMBUS_PROC_CALL	    4
//#define I2C_SMBUS_BLOCK_DATA	    5
//#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
//#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */
//#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages

#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */
//#define I2C_SMBUS_I2C_BLOCK_MAX	32	/* Not specified but we use same structure */

// Structures used in the ioctl() calls

union i2c_smbus_data
{
	uint8_t  byte ;
//	uint16_t word ;
	uint8_t  block [I2C_SMBUS_BLOCK_MAX + 2] ;	// block [0] is used for length + one more for PEC
} ;

struct i2c_smbus_ioctl_data
{
	char read_write ;
	uint8_t command ;
	int size ;
	union i2c_smbus_data *data ;
} ;

static inline int i2c_smbus_access (int fd, char rw, uint8_t command, int size, union i2c_smbus_data *data)
{
	i2c_smbus_ioctl_data args {
		.read_write = rw,
		.command = command,
		.size = size,
		.data = data
	};

	return ioctl (fd, I2C_SMBUS, &args) ;
}

int I2CRead(int fd, unsigned char * data, unsigned char size)
{
	if (size > I2C_SMBUS_BLOCK_MAX)
		size = I2C_SMBUS_BLOCK_MAX;

	i2c_smbus_data d {
		.byte = 0
	};

	if (i2c_smbus_access(fd, I2C_SMBUS_READ, 0, size, &d)) {
		std::cout << std::strerror(errno) << std::endl;
		return -1;
	}
	memcpy(data, d.block, size);
	return 0;
}
