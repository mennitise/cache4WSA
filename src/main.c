#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "main_memory.h"
#include "cache.h"

#define NONE 0
#define READ 1
#define WRITE 2
#define MISS_RATE 3

const int MAX_LENGHT = 256;
const int MAX_LENGHT_LINES = 256;
const int MAX_ADDRESS = 64 * 1024;
const int SIZE_OF_BYTE = 256;

main_memory_t* MAIN_MEMORY;
cache_t* CACHE;

bool validate_adress(int address){
	if (address >= MAX_ADDRESS){
		printf("ERROR: The specified address is very large. The address limit is: %d\n", MAX_ADDRESS);
		return false;
	}
	return true;
}

bool validate_value(int value){
	if (value >= SIZE_OF_BYTE){
		printf("ERROR: The specified value is more bigger that a byte\n");
		return false;
	}
	return true;
}

bool process(char* line){
	char *token = strtok(line, " ");
	int operation = NONE;
	int register_1, register_2;
    if(token) {
		if (strcmp(token, "MR\n") == 0){
			operation = MISS_RATE;
		} else if (strcmp(token, "R") == 0){
			operation = READ;
			token = strtok(NULL, " ");
			if (!token){
				printf("ERROR IN COMMAND WRITE: Inconsitence reading the file\n");
				return false;
			}
			register_1 = atoi(token);
		} else if (strcmp(token, "W") == 0){
			operation = WRITE;
			token = strtok(NULL, ",");
			if (!token){
				printf("ERROR IN COMMAND READ: Inconsitence reading the file\n");
				return false;
			}
			register_1 = atoi(token);
        	token = strtok(NULL, " ");
        	if (!token){
				printf("ERROR IN COMMAND READ: Inconsitence reading the file\n");
				return false;
			}
			register_2 = atoi(token);
		} else {
			printf("ERROR READING THE NEXT LINE\n%s\nINCOSITENCE IN THE FILE\n", token);
			return false;
		}
    }

    switch(operation){
    	case READ:
    		if (!validate_adress(register_1)) return false;
    		printf("reading %d\n", register_1);
			printf("%d\n", read_byte(register_1));
    		break;
    	case WRITE:
    		if (!validate_adress(register_1))return false;
    		if (!validate_value(register_2))return false;
    		printf("writing %d in %d\n", register_2, register_1);
    		printf("%d\n", write_byte(register_1, register_2));
    		break;
    	case MISS_RATE:
    		printf("Calculating miss rate...\n");
			printf("%d\n", get_miss_rate());
    		break;
    }
    return true;
}

void read_file(char* filename) {
	char chars[MAX_LENGHT];
	FILE *file = fopen(filename,"r");

	int lenLines = 0;
	if (file == NULL){
		printf("No such file\n");
		exit(1);
	} else {
		fgets(chars,1024,file);
		while (feof(file) == 0) {			
			if (!process(chars)) break;
			lenLines++;
			fgets(chars,1024,file);
		}
	}
	lenLines--;
	fclose(file);
}

int main(int argc, char *argv[]) {
	init_main_memory();
	init();

	read_file(argv[argc-1]);

	destroy_main_memory();
	destroy();
	return 0;
}
