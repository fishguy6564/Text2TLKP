#include <stdio.h>
#include "TLKP.h"

int main(int argc, char **argv){
    if(argc == 3){
        createTLKP(argv[2], argv[1]);
        printf("Done!\n");
    }else{
        printf("Please provide input and output file names\n");
    }
    return 0;
}
