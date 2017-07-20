#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");

static int howmany;
module_param(howmany, int, 0000);
MODULE_PARM_DESC(howmany, "param howmany");
static char* whom;
module_param(whom, charp, 0660);
MODULE_PARM_DESC(whom, "param whom");


static int __init hello_init(void)
{
	int i;
	for(i=0; i< howmany; i++){
		pr_info("Hello, %s\n",whom);
	}
	
	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	pr_info("Goodbye, %s\n",whom);
}
module_exit(hello_exit);

