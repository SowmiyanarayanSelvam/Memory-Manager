// Hittishi Kurlagunda					[Net ID: hk919]
//Sowmiyanarayan Selvam					[Net ID: ss4370]

#ifndef _UMALLOC_H
#define _UMALLOC_H

#define malloc( x ) umalloc( ( x ) , __FILE__, __LINE__ )
#define free( x ) ufree( ( x ) , __FILE__ , __LINE__ )

// #define MEM_SIZE 4096
#define MEM_SIZE 10485760
typedef struct block//The metadata of a memory block is defined by this structure. It uses 4 bytes for block_size and 1 byte for bool so the metadata for a particular block is 5 bytes.
{
    unsigned long block_size; // block size
    bool is_free; // flag to check if the block is free
}block;

//main functions for malloc library
void *umalloc (size_t,char*,int);
void ufree(void*,char*,int);
void traverse_blocks();
//handling memory architecture
char *find_first_block(size_t);
void merge_free_block(void);
void *break_block(char*,size_t);
char* next(char *);

#endif
