#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	
#include <linux/ioctl.h>
#include "helloioctl.h"

#define DEVICE_NAME "hello"	



MODULE_DESCRIPTION("helloioctl module");
MODULE_AUTHOR("cai qiaoshan, upmc");
MODULE_LICENSE("GPL");

static int Major;		/* Major number assigned to our device driver */

long ioctl_funcs(struct file *filp,unsigned long cmd, unsigned long arg)
{

	int ret=0;

	switch(cmd) {

	case HELLO: 
		pr_info("Hello %s",(char*)arg);
	 break;
	case WHO: 
		pr_info("utilisateur fourni %s",(char*)arg);
	 break;
	default :
		ret = ENOTTY;
	 } 
	 
	return ret;
 
}


struct file_operations fops = {
	.unlocked_ioctl: ioctl_funcs
};



static int __init hello_init(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	pr_info("Major %d\n",Major);
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
	
}
module_exit(hello_exit);


