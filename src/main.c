#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cache.h"

#define NONE 0
#define READ 1
#define WRITE 2
#define MISS_RATE 3

const int MAX_LENGHT = 256;
const int MAX_LENGHT_LINES = 256;

cache_t* CACHE;

void read(int register_1){
	printf("reading %d\n", register_1);
	read_byte(register_1);
}

void write(int register_1, int register_2){
	printf("writing %d in %d\n", register_2, register_1);
	write_byte(register_1, register_2);
}

void miss_rate(){
	printf("Calculating miss rate...\n");
	printf("MISS RATE: %d\n", get_miss_rate());
}

void process(char* line){
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
				abort();
			}
			register_1 = atoi(token);
		} else if (strcmp(token, "W") == 0){
			operation = WRITE;
			token = strtok(NULL, ",");
			if (!token){
				printf("ERROR IN COMMAND READ: Inconsitence reading the file\n");
				abort();
			}
			register_1 = atoi(token);
        	token = strtok(NULL, " ");
        	if (!token){
				printf("ERROR IN COMMAND READ: Inconsitence reading the file\n");
				abort();
			}
			register_2 = atoi(token);
		} else {
			printf("ERROR READING THE NEXT LINE\n%s\nINCOSITENCE IN THE FILE\n", token);
		}
    }

    switch(operation){
    	case READ:
    		read(register_1);
    		break;
    	case WRITE:
    		write(register_1, register_2);
    		break;
    	case MISS_RATE:
    		miss_rate();
    		break;
    }
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
			process(chars);
			lenLines++;
			fgets(chars,1024,file);
		}
	}
	lenLines--;
	fclose(file);
}

int main(int argc, char *argv[]) { 
	init();

	read_file(argv[argc-1]);

	destroy();
	return 0;
}
