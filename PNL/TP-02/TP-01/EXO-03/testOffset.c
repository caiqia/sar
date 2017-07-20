#include<stdlib.h>
#include<stdio.h>

#include"version.h"


struct commit *commit_of(struct version *version){

/*	struct commit c;
	int decalage = (void*)&(c.version) - (void*)&c;

	return (struct commit*) (version - decalage);
	*/

	return (struct commit*)((void*)version - (void*)&((struct commit*)0)->version);
	
}

int main(int argc, char const *argv[])
{
	struct version v = {.major = 3,
			    .minor = 5,
			    .flags = 0};
	printf("v %d\n", &v);
struct commit c = {
		.id = 10,
		.version = NULL,
		.comment = malloc(sizeof(char*)),
		NULL,
		NULL,
	};
	c.version = v;
	printf("c: %d, id: %d, version: %d,comment: %d,next: %d,prev: %d\n",&c,&c.id,&c.version,&c.comment,&c.next,&c.prev);
	
	printf("adresse du commit de v : %d\n", commit_of(&c.version));
	
	return 0;
}


