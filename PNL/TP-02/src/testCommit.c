#include<stdlib.h>
#include<stdio.h>

#include"history.h"

int main(int argc, char const *argv[])
{

    struct commit *c;
    c = new_commit(0,0, "Je suis un test");
    
    display_commit(c);

    return 0;
}
