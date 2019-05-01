#include "I2CAccess.h"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Not using ioctl cause I couldn't figure out how and this is way easier
// Also I'm short on time... otherwise I would figure it out.
int32_t I2CReadReg24(int fd, uint8_t reg)
{
	if (write(fd, &reg, 1) != 1) {
		return -1;
	}

	int32_t data = 0;
	if (read(fd, &data, 3) != 3) {
		return -1;
	}
	return data;
}
