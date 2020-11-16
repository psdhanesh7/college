// Author : Dhanesh P S
// Date : 10/11/2020
// Problem statement : Program to implement absolute loader.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define memorySize 32768

void splitHeaderRecord(char record[], char programName[], int *startingAddress, int *programLength) {
	
	if(record[0] != 'H') {
		printf("Invalid header format!!!\n");
		printf("Terminating the program loading\n");

		exit(0);
	}
	
	int i = 2;
	while(record[i] != '^') {
		programName[i-2] = record[i];
		i++;
	}
	programName[i] = '\0';

	sscanf(record + 9, "%X", startingAddress);
	sscanf(record + 17, "%X", programLength);
}

void splitTextRecord(char record[], int *startingAddress, char *recordType, int *recordLength, int objectCode[], int *objectCodeCount) {
	
	sscanf(record, "%c", recordType);
	// printf("Printing record type from array, record type: %c", record[0]);
	sscanf(record + 2, "%6X", startingAddress);
	sscanf(record + 10, "%2X", recordLength);

	*objectCodeCount = (*recordLength) / 3;
	
	int count = 0;
	int i = 12;
	while(count < *objectCodeCount) {
		
		sscanf(record + i, "%6X", &objectCode[count]);
		// printf("%06X\n", objectCode[count]);

		count++;
		i += 7;
	}

}

void load(FILE *fp, char filename[]) {
	
	int memory[memorySize];
	char record[100];
	char programName[20];
	int programLength, recordLength;
	int startingAddress;
	int objectCode[10], objectCodeCount;
	char recordType;

	// Read the header record
	fgets(record, sizeof(record), fp);
	splitHeaderRecord(record, programName, &startingAddress, &programLength);

	// Read first text record
	fgets(record, sizeof(record), fp);
	splitTextRecord(record, &startingAddress, &recordType, &recordLength, objectCode, &objectCodeCount);


	// Loading to memory
	while(recordType != 'E') {
		// move the object codes to specified location in the memory
		for(int i = 0; i < objectCodeCount; i++) {
			// printf("%X\n", objectCode[i]);
			int currentAddress = startingAddress + i*3;
			
			int j = 2;
			while(j >= 0) {
				memory[currentAddress + j] = objectCode[i] % (16 * 16);
				objectCode[i] /= (0X10 * 0X10);
				
				j--;
			}
			for(int j = 0; j < 3; j++) {
				printf("%06X  %02X\n", currentAddress + j, memory[currentAddress + j]);
			}

			
		}
		fgets(record, sizeof(record), fp);
		splitTextRecord(record, &startingAddress, &recordType, &recordLength, objectCode, &objectCodeCount);
		
	}

}

void main(int argc, char *argv[]) {
	
	FILE *fp;
	char filename[100];
	
	strcpy(filename, argv[1]);

	fp = fopen(filename, "r");
	if(fp == NULL) {
		printf("File not found!!\n");
		exit(0);
	}

	load(fp, filename);
}
