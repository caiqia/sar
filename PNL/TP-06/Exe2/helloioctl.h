#ifndef HELLOIOCTL_H
#define HELLOIOCTL_H

#include <linux/ioctl.h>




#define magic_number 'N'
char* string1 = "Hello ioctl!";

#define HELLO _IOR(magic_number, 0, int)
#define WHO _IOW(magic_number, 0, int)

#endif
