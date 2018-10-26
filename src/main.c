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



void process(char* line){
	char *token = strtok(line, " ");
	int operation = NONE;
    while(token) {
        //puts(token);
		if (strcmp(token, "MR\n") == 0){
			operation = MISS_RATE;
			printf("Miss Rate\n");
		} else if (strcmp(token, "R") == 0){
			operation = READ;
			printf("Read\n");
		} else if (strcmp(token, "W") == 0){
			operation = WRITE;
			printf("Write\n");
		} else {

			printf("No reconoce: %s\n", token);
		}
        token = strtok(NULL, " ");
    }

/*
*/
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
