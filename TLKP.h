typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct{
    u32 location[6];
}Group;

typedef struct{
    char filemagic[4];
    u32 fileSize;
    u16 constant1;
    u16 constant2;
    u16 constant3;
    u16 constant4;
    u32 groupAmount;
    u8 entriesPerGroup;
    u8 pad[3];
    u32 groupState;
    u32 tableStartLocation;
}TLKPHeader;

void createTLKP(char* filename, char* filetext);
