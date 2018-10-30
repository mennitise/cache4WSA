#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main_memory.h"

#define MAIN_MEMORY_SIZE 	64 * 1024 // 64KB
#define BLOCK_SIZE 		    64 // 64B
#define BLOCKS              ( 64 * 1024 ) / 64


/* ---------------- MAIN MEMORY BLOCK DEFINITION ------------------ */

typedef struct main_memory_block{
    char* data;
}main_memory_block_t;

main_memory_block_t* init_main_memory_block(){
    main_memory_block_t* block = malloc(sizeof(main_memory_block_t));
    if (!block){
        printf("ERROR: Can't Initialize blocks from main memory\n");
        abort();
    }

    block->data = malloc(BLOCK_SIZE * sizeof(char*));
    if (!block->data){
        printf("ERROR: Can't Initialize data blocks from main memory\n");
        abort();
    }

    return block;
}

void destroy_main_memory_block(main_memory_block_t* block){
    if(block) free(block->data);
    free(block);
}

/* ---------------------------------------------------- */

/* ---------------- MAIN MEMORY DEFINITION ------------------ */

typedef struct main_memory{
    int size;
    main_memory_block_t* blocks[BLOCKS];
    size_t blocks_amount;

}main_memory_t;


void init_main_memory(){
    printf("Initialize Main Memory...\n");

    MAIN_MEMORY = malloc(sizeof(main_memory_t));
    if (!MAIN_MEMORY){
        printf("ERROR: Can't Initialize Main Memory\n");
        abort();
    }

    MAIN_MEMORY->blocks_amount = BLOCKS;
    MAIN_MEMORY->size = 64 * 1024;

    for (int i = 0; i < MAIN_MEMORY->blocks_amount; ++i){
        MAIN_MEMORY->blocks[i] = init_main_memory_block();
    }
}

void destroy_main_memory(){
    if (MAIN_MEMORY){
        for (int i = 0; i < MAIN_MEMORY->blocks_amount; ++i){
            destroy_main_memory_block(MAIN_MEMORY->blocks[i]);
        }
    }
    free(MAIN_MEMORY);
    puts("Main Memory destroyed!");
}

/* ---------------------------------------------------- */

