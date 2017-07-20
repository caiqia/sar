#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>


MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");

char* foo1="Hello";
char* foo2;

static ssize_t foo_show(struct foo_obj *foo_obj, struct foo_attribute *attr,
			char *buf)
{
	pr_info("lecture de fichier\n");
	char*tmp =strcat(foo1,foo2);
	char* foo1="Hello";
	return sprintf(buf, "%s\n", tmp);
}

static ssize_t foo_store(struct foo_obj *foo_obj, struct foo_attribute *attr,
			 const char *buf, size_t count)
{
	pr_info("ecriture de fichier \n");
	
	sscanf(buf, "%u", foo2);
	
	return count;
}

static struct kobj_attribute foo_attribute =__ATTR_RW(foo);


static int __init hello_init(void)
{
	
	sysfs_create_file(kernel_kobj, &foo_attribute.attr);
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	
	sysfs_remove_file(kernel_kobj, &foo_attribute.attr);
	
}
module_exit(hello_exit);
