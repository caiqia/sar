#include<stdlib.h>
#include<stdio.h>

#include"history.h"

/**
  * new_history - alloue, initialise et retourne un historique.
  *
  * @name: nom de l'historique
  */
struct history *new_history(char *name)
{
	/* TODO : Exercice 3 - Question 2 */
	struct history *h;
	h = malloc(sizeof(struct history));
	h->name= name;
	h->commit_count = 0;
	h->commit_list = new_commit(0,0, "DO NOT PRINT ME !!!");
	return h;
}

/**
  * last_commit - retourne l'adresse du dernier commit de l'historique.
  *
  * @h: pointeur vers l'historique
  */
struct commit *last_commit(struct history *h)
{
	/* TODO : Exercice 3 - Question 2 */

    return h->commit_list->prev;
	
	
}

/**
  * display_history - affiche tout l'historique, i.e. l'ensemble des commits de
  *                   la liste
  *
  * @h: pointeur vers l'historique a afficher
  */
void display_history(struct history *h)
{
	/* TODO : Exercice 3 - Question 2 */
    struct commit *current_commit = h->commit_list->next;

    if(h->commit_count == 0){
	display_commit(h->commit_list);
	printf("\n");
    }
    
    h->commit_count = 1;
    printf("Historique de \'%s\' :\n", h->name);

    while(current_commit != h->commit_list ){
	display_commit(current_commit);
	current_commit = current_commit->next;
    }

    printf("\n");
	
}

/**
  * infos - affiche le commit qui a pour numero de version <major>-<minor> ou
  *         'Not here !!!' s'il n'y a pas de commit correspondant.
  *
  * @major: major du commit affiche
  * @minor: minor du commit affiche
  */
void infos(struct history *h, int major, unsigned long minor)
	/* TODO : Exercice 3 - Question 2 */
    struct commit *current_commit = h->commit_list->next;
    int i = 0, j=0;
    int trouve = 0;

    while(current_commit != h->commit_list && current_commit->version.major < major){
	current_commit = current_commit->next;
    }

    if(current_commit->version.major == major){

	while(current_commit != h->commit_list && current_commit->version.minor < minor){
	    current_commit = current_commit->next;
	}

	if(current_commit->version.minor == minor){
	    display_commit(current_commit);
	    trouve = 1;
	}
    }

   
    
    if(!trouve){
	printf("NOT HERE !! \n");
    }
    
    
   
}
