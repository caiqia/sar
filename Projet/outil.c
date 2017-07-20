#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "mod.h"

int main (int argc, char* argv[]) {
  char chaine[100];
  char* arg[100];
  int i=0;
  char* mot;
  
  /*Ouverture du fichier du module*/
  int fd = open("/dev/mod", O_RDONLY);
  unsigned int req;
  if (fd < 0) {
    printf("Can't open device file: mod\n");
    exit(-1);
  }
	
  printf("Saisir commande\n");
  scanf("%[^\n]",chaine);
  getchar();
	
  /*L'outil est arrêté lorsque l'utilisateur rentre exit*/
  while(strcmp(chaine,"exit")!=0){
    
    /*Découpage de la chaîne de caractères*/
    int l = strlen(chaine);
    mot = strtok (chaine, " ");
    while(mot!=NULL){
      arg[i]=mot;
      i++;
      mot = strtok (NULL, " ");
    }
    arg[i+1]=NULL;


    /*Si c'est modinfo*/
    if (strcmp(arg[0],"modinfo")==0){
      char *msg1=malloc(sizeof(char)*256);
      strcpy(msg1, arg[1]);
      req = MODINFO;
      ioctl(fd, req, msg1);
      printf("%s",msg1);
      free(msg1);
    }

    /*Si c'est meminfo*/
    if (strcmp(arg[0],"meminfo")==0){
      char* buffer2=malloc(sizeof(char)*150);
      req = MEMINFO;
      ioctl(fd, req, buffer2);
      printf("%s",buffer2);
      free(buffer2);
    }

    /*Si c'est kill*/
    if (strcmp(arg[0],"kill")==0){
      req = KILL;
      char *msg=malloc(sizeof(char)*150);
      strcpy(msg, arg[1]);
      strcat(msg, " ");
      strcat(msg, arg[2]);
      ioctl(fd, req, msg);
      free(msg);
    }

    /*Si c'est wait*/
    if (strcmp(arg[0],"wait")==0){
      req = WAIT;
      char *msg=malloc(sizeof(char)*100);
      int j=1;
      strcpy(msg, "");
      for(j=1;j<i;j++){
	strcat(msg, arg[j]);
	strcat(msg, " ");
      }
      ioctl(fd, req, msg);
      free(msg);
    }

    /*Si c'est list*/
    if (strcmp(arg[0],"list")==0){
      req = LIST;
      ioctl(fd, req, NULL);
    }
	  
    /*Demande d'une autre commande*/
    printf("Saisir commande\n");
    scanf("%[^\n]",chaine);
    getchar();	
  }
  
  close(fd);
  return 0;
}
