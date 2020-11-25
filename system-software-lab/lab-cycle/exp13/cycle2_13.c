// Author : Dhanesh P S
// Date : 10/11/2020
// Problem statement : Program to implement relocating loader.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define memorySize 1048576

int memory[memorySize];

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

void splitTextRecord(char record[], int *startingAddress, char *recordType, int *recordLength, char objectCode[][100], int *objectCodeCount) {
	
	sscanf(record, "%c", recordType);

	if(*recordType != 'T') {
		return;
	}

	sscanf(record + 2, "%6X", startingAddress);
	sscanf(record + 10, "%2X", recordLength);

	*objectCodeCount = 0;

	int j = 0;
	int length = strlen(record);
	for(int i = 12; i < length; i++) {
		if(record[i] == '^' || record[i] == '\0') {
			objectCode[*objectCodeCount][j] = '\0';
			j = 0;
			(*objectCodeCount)++;

			continue;
		}

		objectCode[*objectCodeCount][j++] = record[i]; 
	}
}

void splitModificationRecord(char record[], char *recordType, int *modificationAddress, int *halfbytes) {

	sscanf(record, "%c", recordType);

	sscanf(record + 2, "%6X", modificationAddress);
	sscanf(record + 9, "%2X", halfbytes);

}

void loadProgram(FILE *fp, int startingAddress) {

	char record[100];
	char programName[20];
	int programLength, recordLength;
	int recordStartingAddress;
	char objectCode[10][100];
	int objectCodeCount;
	char recordType, currentObjectCode[10];
	int lastAddress;

	// Read the header record
	fgets(record, sizeof(record), fp);
	splitHeaderRecord(record, programName, &recordStartingAddress, &programLength);

	// Read first text record
	fgets(record, sizeof(record), fp);
	splitTextRecord(record, &recordStartingAddress, &recordType, &recordLength, objectCode, &objectCodeCount);

	int currentAddress = startingAddress;

	while(recordType == 'T') {

		currentAddress = startingAddress + recordStartingAddress;

		for(int i = 0; i < objectCodeCount; i++) {

			strcpy(currentObjectCode, objectCode[i]);

			int objectCodeLength = strlen(currentObjectCode);
			for(int j = 0; j < objectCodeLength/2; j++) {
				sscanf(currentObjectCode + j*2, "%2X", &memory[currentAddress]);
				currentAddress++;
			}
		}

		fgets(record, sizeof(record), fp);
		splitTextRecord(record, &recordStartingAddress, &recordType, &recordLength, objectCode, &objectCodeCount);
	}

	lastAddress = currentAddress;

	int modificationAddress;
	int halfbytes;

	splitModificationRecord(record, &recordType, &modificationAddress, &halfbytes);

	while(recordType != 'E') {
		modificationAddress += startingAddress;
		
		int address, modifiedAddress;
		int i = 0;
		if(halfbytes % 2 != 0) {
			address = memory[modificationAddress] % 16;
			i++;

			while(i <= halfbytes/2) {
				address = address * (16*16) + memory[modificationAddress + i];
				i++;
			}

			modifiedAddress = address + startingAddress;
		}

		memory[modificationAddress] = (memory[modificationAddress]/16) * 16 + (modifiedAddress / (1 << ((halfbytes - 1) * 4)));
		for(int i = modificationAddress + (halfbytes/2); i > modificationAddress; i--) {
			memory[i] = modifiedAddress % (16*16);
			modifiedAddress /= (16 * 16);
		}

		fgets(record, sizeof(record), fp);
		splitModificationRecord(record, &recordType, &modificationAddress, &halfbytes);

	}

	for(int i = startingAddress; i < lastAddress; i++) {
		if(memory[i] != -1) {
			printf("%06X\t%02X\n", i, memory[i]);
		}
	}

}

void main(int argc, char *argv[]) {

	FILE *fp;
	int startingAddress;

	fp = fopen(argv[1], "r");
	if(fp == NULL) {
		printf("Invalid Filename!!!\n");
		exit(0);
	}

	for(int i = 0; i < memorySize; i++) {
		memory[i] = -1;
	}

	printf("Enter the starting addring to which program is to be loaded: \n");
	scanf("%X", &startingAddress);

	loadProgram(fp, startingAddress);

}