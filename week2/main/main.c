#include <stdio.h>
#include <stdlib.h>
#include "../lib/week2.h"

int main(int argc, char *argv[]){
	if (atoi(argv[1]) == 1) {
        get_ip(argv[2]);
    } else if (atoi(argv[1]) == 2){
        get_domain(argv[2]);
    } else {
        printf("Wrong parameter\n");
    }
}