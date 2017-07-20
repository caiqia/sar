#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/fsnotify.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/hash.h>
#include <linux/cache.h>
#include <linux/export.h>
#include <linux/mount.h>
#include <linux/file.h>
#include <asm/uaccess.h>
#include <linux/security.h>
#include <linux/seqlock.h>
#include <linux/swap.h>
#include <linux/bootmem.h>
#include <linux/fs_struct.h>
#include <linux/hardirq.h>
#include <linux/bit_spinlock.h>
#include <linux/rculist_bl.h>
#include <linux/prefetch.h>
#include <linux/ratelimit.h>
#include <linux/list_lru.h>
#include <linux/kasan.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/linkage.h>
#include <linux/mmzone.h>
#include <linux/list.h>
#include <linux/memcontrol.h>
#include <linux/sched.h>
#include <linux/node.h>
#include <linux/atomic.h>
#include <linux/types.h>
#include <linux/buffer_head.h>
#include <asm/segment.h>
#include <linux/workqueue.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include "mod.h"


MODULE_DESCRIPTION("Module \"mod\" pour noyau linux");
MODULE_AUTHOR("Jaime WahnonAbitbol, Kevin VU et Cai Qiaoshan, UPMC");
MODULE_LICENSE("GPL");

long ioctl_funcs(struct file *filp,unsigned long cmd, unsigned long arg)
{
  int ret=0;
  switch(cmd) {
    
  case MODINFO:
    {
      /*Appel de la fonction modinfo*/
      ret = modinfo((char*)arg);
      break;
    }
  case LIST: 
    pr_info("utilisateur fourni %s",(char*)arg);
    break;
  case FG: 
    pr_info("utilisateur fourni %s",(char*)arg);
    break;
    
  case KILL:
    {
      char *paramkill;
      long int sig, pid;
      char* arg1kill;
      char* arg2kill;
    
      /*On récupère les arguments qui correspondent au pid et au signal*/
      paramkill = kmalloc(sizeof(char)*64,0);
      strcpy(paramkill,(char *)arg);
      arg1kill = strsep(&paramkill, " ");
      arg2kill = strsep(&paramkill, " ");
      
      /*On convertit les string en entier*/
      kstrtol(arg1kill,0,&sig);
      kstrtol(arg2kill,0,&pid);
      
      /*On appelle la fonction killmod*/
      ret = killmod(sig,pid);

      kfree(paramkill);
      break;
    }
    
  case WAIT:
    {
      int taille;
      char* paramwait;
      char* arg1wait;
      
      taille=0;
      paramwait = kmalloc(sizeof(char)*100,0);
      strcpy(paramwait,(char *)arg);
      arg1wait = strsep(&paramwait, " ");

      /*On récupère le nombre de pid passé à la commande*/
      while(arg1wait!=NULL){
	taille++;
	arg1wait = strsep(&paramwait, " ");
      }
      /*On appelle la fonction waitmod*/
      ret = waitmod(taille,(char*)arg);

      kfree(paramwait);
      break;
    }
  case MEMINFO:
    {
      /*On appelle la fonction meminfo avec en arg l'adresse du buffer*/
      ret = meminfo((char*)arg);
      break;
    }
  default :
    ret = -ENOTTY;
  }
  return ret;
}

struct file_operations fops =
  {
    .unlocked_ioctl = ioctl_funcs,
  };

/*Fonction waitmod*/
int waitmod(int taille,char* tab){
  int tableau[taille];
  long int pid;
  int i=0;
  char* arg1;
  struct task_struct *task;
  arg1 = strsep(&tab, " ");

  /*Remplissage du tableau avec les pid*/
  while(arg1!=NULL){
    kstrtol(arg1,0,&pid);
    tableau[i]=pid;
    i++;
    arg1 = strsep(&tab, " ");
  }

  /*Attente active de la tache de pid*/
  task = pid_task(find_get_pid(tableau[0]), PIDTYPE_PID);
  while(task->state==0){
	msleep(100);
  }
  return 0;
}

/*Fonction killmod*/
int killmod(int signal, int pid){
  return kill_pid(find_get_pid(pid),signal, 1);
}

/*Fonction meminfo*/
int meminfo(char* buffer){
	struct sysinfo val1;
	/*On remplit les champs de la structure*/
	si_meminfo(&val1);
	si_swapinfo(&val1);
	/*On remplit le buffer*/
	sprintf(buffer,"MenTotal:  %lu\nMemFree: %lu\nBuffers: %lu\nHighTotal: %lu\nHighFree: %lu\nSharedMemory: %lu\nMemUnit: %d\nSwapTotal: = %lu\nSwapFree: %lu\n",val1.totalram*val1.mem_unit/1000,val1.freeram*val1.mem_unit/1000,val1.bufferram*val1.mem_unit/1000,val1.totalhigh*val1.mem_unit/1000,val1.freehigh*val1.mem_unit/1000,(val1.sharedram*val1.mem_unit)/1000,val1.mem_unit,val1.totalswap*val1.mem_unit/1000,val1.freeswap*val1.mem_unit/1000);
	/*On envoie à l'outil*/
	copy_to_user(buffer,buffer,strlen(buffer));
	return 0;
}

/*Fonction modinfo*/
int modinfo(char * filename)
{
  int i;
  mm_segment_t old_fs;
  loff_t offset;
  struct file* fic;
  struct module *m;
  char* adresse;
  unsigned char* data;
  char* ligne;
  char* param;

  /*Sauvegarde du segment*/
  old_fs = get_fs();
  set_fs(get_ds());
  
  /*Allocation des buffers*/
  param = kmalloc(sizeof(char)*256,0);
  data = kmalloc(sizeof(char),0);
  ligne = kmalloc(sizeof(char)*256,0);
  adresse = kmalloc(sizeof(char)*256,0);

  /*Initialisation*/
  fic = filp_open("/proc/modules",O_RDONLY,0644);
  m = find_module(filename);
  offset = 0;
  
  /*Récupération de l'adresse de chargement du module dans le fichier*/
  do{
    strcpy(data,"");
    strcpy(ligne,"");
    do{
      vfs_read(fic, data, 1, &offset);
      sprintf(data,"%c",data[0]);
      strcat(ligne,data);
    }while(strcmp(data,"\n")!=0);
    strcpy(data,strsep(&ligne, " "));
  }while(strcmp(data,filename)!=0);

  for(i=0;i<4;i++){
    strsep(&ligne, " ");
  }
  strcpy(adresse,strsep(&ligne, " "));

  /*Si le module a des paramètres*/
  if(m->num_kp!=0){
	/*On récupère le nom des paramètres*/
  	for (i = m->num_kp-1 ; i >=0 ; i--) {
    		strcat(param,m->kp[i].name);
    		strcat(param," ");
  	}
    sprintf(ligne,"Nom du module : %s\nVersion du module : %s\nAdresse de chargement du module : %s\nParamètre(s) : %s\n",filename,m->version,adresse,param);
  }
  /*S'il n'en a pas*/
  else{
    sprintf(ligne,"Nom du module : %s\nVersion du module : %s\nAdresse de chargement du module : %s\nPas de paramètres\n",filename,m->version,adresse);
  }
  
  /*On envoie à l'outil*/
  copy_to_user(filename,ligne,strlen(ligne));
  
  /*Fermeture du fichier et libération*/
  filp_close(fic, NULL);
  set_fs(old_fs);
  kfree(data);
  kfree(adresse);
  kfree(param);
  sprintf(ligne," ");
  kfree(ligne);
  return 0;
}

/*Initialisation du module*/
static int __init mod_init(void)
{
  pr_info("Chargement");
  register_chrdev(0,"mod", &fops);
  return 0;
}

module_init(mod_init);

/*Terminaison du module*/
static void __exit mod_exit(void)
{
  pr_info("Déchargement\n");
  unregister_chrdev(0, "mod");
}
module_exit(mod_exit);
