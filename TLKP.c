#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TLKP.h"

char** strArray;
Group* groups;
u32 entryAmount = 0;
u32 fileSize = 0;

TLKPHeader getHeader(){
    TLKPHeader header;
    strcpy(header.filemagic, "TLKP");
    header.constant1 = 0x0200;
    header.constant2 = 1;
    header.constant3 = 0x100;
    header.constant4 = 1;
    header.entriesPerGroup = 6;
    memset(header.pad, 0, 3);
    header.groupState = 1;
    header.tableStartLocation = 0x20;
    return header;
}

void appendString(char* str){
    strArray = (char**)realloc(strArray, entryAmount * sizeof(char**));
    strArray[entryAmount - 1] = str;
}

void setGroup(){
    groups = (Group*)malloc(sizeof(Group)*(entryAmount/3));
    fileSize = (entryAmount/3) * 0x18 + 0x20;
    u32 stringCounter = 0;
    for(int i = 0; i < (entryAmount/3); i++){
        memset(groups[i].location, 0, sizeof(groups[i].location));
        groups[i].location[0] = fileSize;
        fileSize += strlen(strArray[stringCounter++])+1;
        groups[i].location[1] = fileSize;
        fileSize += strlen(strArray[stringCounter++])+1;
        groups[i].location[2] = fileSize;
        fileSize += strlen(strArray[stringCounter++])+1;
    }
}

void interpretStrings(char* textfile){
    FILE* file;
    file = fopen(textfile, "r");
    char c;

    while(1){
        char* str = malloc(1);
        memset(str, 0, 1);
        u32 charCounter = 0;
        while(1){
            c = getc(file);
            if(c == '\n' || c == EOF) break;
            charCounter++;
            str = (char*)realloc(str, charCounter+1);
            strncat(str, &c, 1);
        }
        entryAmount++;
        appendString(str);
        if(c == EOF) break;
    }
    printf("Total amount of entries: %d\n", entryAmount);

}

void createTLKP(char* filename, char* filetext){
    FILE* file;
    file = fopen(filename, "wb");

    TLKPHeader header = getHeader();

    interpretStrings(filetext);


    setGroup();
    header.groupAmount = entryAmount/3;
    header.fileSize = fileSize+2;

    fwrite(&header, 0x20, 1, file);

    for(int i = 0; i < header.groupAmount; i++){
        fwrite(groups[i].location, sizeof(groups[i].location), 1, file);
    }

    u8 p = 0;
    for(int i = 0; i < entryAmount; i++){
        fwrite(strArray[i], strlen(strArray[i]), 1, file);
        fwrite(&p, 1, 1, file);
    }
    fwrite(&p, 1, 1, file);
    fwrite(&p, 1, 1, file);

    fclose(file);
}
