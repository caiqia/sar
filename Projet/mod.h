#ifndef MOD_H
#define MOD_H

#include <linux/ioctl.h>



#define MODINFO _IOR('N',2,char*)
#define LIST _IOR('N',3,char*)
#define FG _IOR('N',4,char*)
#define KILL _IOR('N',5,char*)
#define WAIT _IOR('N',6,char*)
#define MEMINFO _IOR('N',7,char*)

int modinfo(char * filename);
int meminfo(char* buffer);
int killmod(int signal, int pid);
int waitmod(int taille,char* tab);
#endif
