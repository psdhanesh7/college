#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define capacity 20

typedef struct {
	char key[20];
	int value;
} pair;

typedef struct node {
	pair data;
	struct node* next;
} node;

// Declaring OPTAB and SYMTAB
node* symtab[capacity];
node* optab[capacity];

int startingAddress;
int programLength;
int errorFlag = 0;

// ----------------------------------------------------------
// Implementation of hash table using chaining

int hash(char key[]) {
	
	int h = 0;
	for(int i = 0; key[i] != '\0'; i++) {
		h += (i+1) * key[i];
	}

	return h % capacity;
}

void chaining(node* hashArray[], pair data, int hashCode) {
	node *ptr;
	ptr = (node*) malloc(sizeof(node));

	ptr->data = data;
	ptr->next = hashArray[hashCode];
	hashArray[hashCode] = ptr;
}

void insert(node* hashArray[], char key[], int value) {
	
	int hashCode = hash(key);

	pair data;
	strcpy(data.key, key);
	data.value = value;

	chaining(hashArray, data, hashCode);
}

int find(node* hashArray[], char key[]) {

	int hashCode = hash(key);

	node* curr = hashArray[hashCode];
	while(curr != NULL) {
		if(strcmp(curr->data.key, key) == 0) {
			return curr->data.value;
		}
		curr = curr->next;
	}
	return -1;
}

void displayChain(node *header) {

	if(header == NULL) {
		return;
	}

	node *ptr = header;
	while(ptr != NULL) {
		printf("%s\t%02X\n", ptr->data.key, ptr->data.value);
		ptr = ptr->next;
	}	
}

void printTable(node *hashArray[]) {
	for(int i = 0; i < capacity; i++) {
		displayChain(hashArray[i]);  //Display each chain
	}
}

void clearChain(node *head) {

	node *curr = head;
	node *next = NULL;

	while(curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
}

void clearHashTable(node* hashArray[]) {
	for(int i = 0; i < capacity; i++) {
		clearChain(hashArray[i]);
	}
}

// Hash table implementation ends here
// ------------------------------------------------------

// Function to separate label, opcode and operand fields from the read line
void splitLine(char line[], char address[], char label[], char opcode[], char operand[]) {
	
	int i = 0;
	int j = 0;

	// Copying the address field
	while(line[i] != ' ') {
		address[j] = line[i];
		j++;
		i++;
	}

	i++;
	j = 0;
	
	// Copying the label field
	while(line[i] != ' ') {
		label[j] = line[i];
		i++;
		j++;
	}
	label[j] = '\0';

	i++;
	j = 0;
	
	// Copying the opcode field
	while(line[i] != ' ' && line[i] != '\n') {
		opcode[j] = line[i];
		i++;
		j++;
	}
	opcode[j] = '\0';

	while(line[i] == ' ') {
		i++;
	}
	j = 0;
	
	// Copying the operand field
	while(line[i] != '\n' && line[i] != ' ') {
		operand[j] = line[i];
		i++;
		j++;
	}
	operand[j] = '\0';
}

int getMachineCode(char opcode[]) {
	
	int machineCode = find(optab, opcode);
	return machineCode;
}

int getOperandAddress(char operand[]) {

	int operandAddress = find(symtab, operand);
	return operandAddress;
}


void createOperationTable() {

	insert(optab, "LDA", 0x00);
	insert(optab, "STA", 0x0C);
	insert(optab, "ADD", 0x18);
}

void assemblerPass1(char* filename) {
	
	FILE *fp1;
	char line[150];
	char address[50], label[50], opcode[50], operand[50];
	
	fp1 = fopen(filename, "r");

	if(fp1 == NULL) {
		printf("Failed to open %s. File not found\n", filename);
		return;
	}
	
	startingAddress = 0x0;
	int locctr = 0x0;
	
	// Processing the first line
	fgets(line, sizeof(line), fp1);
	splitLine(line, address, label, opcode, operand);
	
	if(strcmp(opcode, "START") == 0) {
		sscanf(operand, "%X", &locctr);
		startingAddress = locctr;
	}

	while(fgets(line, sizeof(line), fp1) != NULL) {
		
		splitLine(line, address, label, opcode, operand);
		
		if(strcmp(opcode, "END") == 0) {
			break;
		}

		if(strcmp(label, "ERROR") == 0) {
			errorFlag = 1;
			return;
		}

		if(label[0] != '-') {
			if(find(symtab, label) != -1) {
				errorFlag = 1;
			}
			else {
				insert(symtab, label, locctr);
			}
		}

		// Get the machine code equivalent of opcode
		int machineCode = getMachineCode(opcode);

		if(machineCode != -1) {
			locctr += 0x3;
		}
		else if(strcmp(opcode, "WORD") == 0) {
			locctr += 0x3;
		}
		else if(strcmp(opcode, "RESW") == 0) {
			int memorySize;
			sscanf(operand, "%d", &memorySize);
			
			locctr += (3 * memorySize);
		}
		else if(strcmp(opcode, "RESB") == 0) {
			int memorySize;
			sscanf(operand, "%d", &memorySize);

			locctr += memorySize;
		}
		else if(strcmp(opcode, "BYTE") == 0) {
			int length = strlen(operand) - 3;
			
			locctr += length;
		}
		else {
			// Set error flag, invalid operation
			errorFlag = 1;
		}
	}

	programLength = locctr - startingAddress;
	
	fclose(fp1);
}


void assemblerPass2(char* filename) {
	
	FILE *fp1, *fp2;
	char line[150];
	char address[50], label[50], opcode[50], operand[50];
	char currentTextRecord[100];

	fp1 = fopen(filename, "r");
	fp2 = fopen("output.obj", "w");

	if(fp1 == NULL) {
		printf("Failed to open %s. File not found\n", filename);
		return;
	}
	if(fp2 == NULL) {
		printf("Failed to create an intermediate file\n");
		return;
	}

	// Processing the first line
	fgets(line, sizeof(line), fp1);
	splitLine(line, address, label, opcode, operand);

	if(strcmp(opcode, "START") == 0) {
		// Write the header record
		fprintf(fp2, "H%-6s%06X%06X\n", label, startingAddress, programLength);
	}

	int currentRecordStartAddress = startingAddress;
	int currentRecordLength = 0;
	currentTextRecord[0] = 'T';

	while(fgets(line, sizeof(line), fp1) != NULL) {
		splitLine(line, address, label, opcode, operand);

		if(strcmp(opcode, "END") == 0) {
			break;
		}

		// Search optab for opcode
		int machineCode = getMachineCode(opcode);
		int operandAddress;

		if(machineCode != -1) {
			if(strcmp(operand, "") != 0) {
				if(find(symtab, operand) != -1) {
					// Convert to object code
					operandAddress = getOperandAddress(operand);
				}
				else {
					operandAddress = 0;
					errorFlag = 1;
				}
			}
			else {
				operandAddress = 0;
			}

			if(currentRecordLength + 6 < 60) {
				sprintf(currentTextRecord + 9 + currentRecordLength, "%02X%04X", machineCode, operandAddress);
				currentRecordLength += 6;
			}
			else {
				sprintf(currentTextRecord + 1, "%06X%02X", currentRecordStartAddress, currentRecordLength);
				fprintf(fp2, "%s\n", currentTextRecord);
				
				currentRecordLength = 0;
				sscanf(address, "%X", &currentRecordStartAddress);
				// currentRecordStartAddress = address;
			
				sprintf(currentTextRecord + 9 + currentRecordLength, "%02X%04X", machineCode, operandAddress);
				currentRecordLength += 6;
			}

		}
		else if(strcmp(opcode, "WORD") == 0) {
			
			int objectCode;
			sscanf(operand, "%d", &objectCode);

			if(currentRecordLength + 6 < 60) {
				sprintf(currentTextRecord + 9 + currentRecordLength, "%06X", objectCode);
				currentRecordLength += 6;
			}
			else {
				sprintf(currentTextRecord + 1, "%06X%02X%s", currentRecordStartAddress, currentRecordLength, currentTextRecord + 9);
				fprintf(fp2, "%s\n", currentTextRecord);
				
				currentRecordLength = 0;
				// currentRecordStartAddress = address;
				sscanf(address, "%X", &currentRecordStartAddress);

			
				sprintf(currentTextRecord + 9 + currentRecordLength, "%06X", objectCode);
				currentRecordLength += 6;
			}

		}
	}

	sprintf(currentTextRecord + 1, "%06X%02X%s", currentRecordStartAddress, currentRecordLength, currentTextRecord + 9);
	fprintf(fp2, "%s\n", currentTextRecord);

	fprintf(fp2, "E%06X\n", startingAddress);

	fclose(fp2);
}


void main(int argc, char* argv[]) {

	FILE *fp;
	char *filename = argv[1];
	char line[100];

	// Initialising optab and symtab to NULL
	for(int i = 0; i < capacity; i++) {
		optab[i] = NULL;
		symtab[i] = NULL;
	}

	createOperationTable();

	assemblerPass1(filename);

	// if there are some errors in the intermediate file, then program is aborted
	if(errorFlag) {
		printf("You have an error in your program\nAborting!!!\n");
		printf("Check %s for error details\n", filename);

		clearHashTable(symtab);
		clearHashTable(optab);

		return;
	}

	assemblerPass2(filename);

	clearHashTable(symtab);
	clearHashTable(optab);

	fp = fopen("output.obj", "r");

	printf("Object Code\n");
	printf("-----------\n");
	while(fgets(line, sizeof(line), fp) != NULL) {
		printf("%s", line);
	}

	fclose(fp);

}
