#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kernel_stat.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>

MODULE_DESCRIPTION("A weasel kernel module");
MODULE_AUTHOR("Cai ");
MODULE_LICENSE("GPL");

extern struct hlist_bl_head *dentry_hashtable;
extern unsigned int d_hash_shift __read_mostly;

static unsigned address; 
static unsigned taille;
static int nbentry;
static struct proc_dir_entry *rep;

unsigned addr_table((void *)table){

	return (& table[0]);
}

unsigned size_table((void *)table){
	
	return sizeof(table);
}

int nb_entry((void *)table){
	return (int)(taille/d_hash_shift);
}

static int weasel_init(void)
{
	rep = proc_mkdir("weasel", NULL);
	proc_mkdir("list", rep);
	proc_create("whoami", 0, rep, NULL);
	address = addr_table(dentry_hashtable);
	taille = size_table(dentry_hashtable);
	nbentry = nb_entry(dentry_hashtable);
	pr_info("address de la table: %u\n",address);
	pr_info("taille de la table: %u\n",taille);
	pr_info("entry de la table: %d\n",nbentry);
	return 0;
}

static void weasel_exit(void)
{
	remove_proc_entry("whoami", rep);
	remove_proc_entry("list", rep);
	remove_proc_entry("weasel", NULL);
	pr_info("weasel module unloaded");
	
}

module_init(weasel_init);
module_exit(weasel_exit);
