#define _GNU_SOURCE

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <dlfcn.h>


ssize_t read(int fd,void *buf,size_t count)
{
		printf("Tchao !!!\n");
		ssize_t (*original_read)(int ,void *,size_t);
		original_read = dlsym(RTLD_NEXT, "read");
		return (*original_read)(fd,buf,count);
		
}
