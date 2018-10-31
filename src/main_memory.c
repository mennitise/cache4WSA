#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main_memory.h"

/* ---------------- MAIN MEMORY BLOCK DEFINITION ------------------ */

main_memory_block_t* init_main_memory_block(){
    main_memory_block_t* block = malloc(sizeof(main_memory_block_t));
    if (!block){
        printf("ERROR: Can't Initialize blocks from main memory\n");
        abort();
    }

    block->data = malloc(MAIN_MEMORY_BLOCK_SIZE * sizeof(char*));
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

void init_main_memory(){
    printf("Initialize Main Memory...\n");

    MAIN_MEMORY = malloc(sizeof(main_memory_t));
    if (!MAIN_MEMORY){
        printf("ERROR: Can't Initialize Main Memory\n");
        abort();
    }

    MAIN_MEMORY->blocks_amount = MAIN_MEMORY_BLOCKS;
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

