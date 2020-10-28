#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define capacity 20

typedef struct node {
	char key[20];
	int value;
	struct node* next;
} node;

// Declaring SYMTAB
node* symtab[capacity];

// ----------------------------------------------------------
// Implementation of hash table using chaining

int hash(char key[]) {
	
	int h = 0;
	for(int i = 0; key[i] != '\0'; i++) {
		h += (i+1) * key[i];
	}

	return h % capacity;
}

void chaining(node* hashArray[], char key[], int value, int hashCode) {
	node *ptr;
	ptr = (node*) malloc(sizeof(node));

//	ptr->key = key;
	strcpy(ptr->key, key);
	ptr->value = value;
	ptr->next = hashArray[hashCode];
	hashArray[hashCode] = ptr;

	printf("Symbol inserted successfully\n");
}

void insert(node* hashArray[], char key[], int value) {
	
	int hashCode = hash(key);
	chaining(hashArray, key, value, hashCode);
}

void deleteFromChain(node* hashtable[], char key[], int hashCode) {
 
	if(hashtable[hashCode] == NULL ) {
		printf("Symbol not found!!\n");
		return;
	}

	node *ptr = hashtable[hashCode];
	node *pre = NULL;

	while(strcmp(ptr->key, key) != 0 && ptr->next != NULL) {
		pre = ptr;
		ptr = ptr->next;
	}

	if(ptr == hashtable[hashCode]) {
		hashtable[hashCode] = ptr->next;
		free(ptr);
		printf("%s deleted succesfully\n", key);
	}
	else if(strcmp(ptr->key, key) == 0) {
		pre->next = ptr->next;
		free(ptr);
		printf("%s deleted succesfully\n", key);
	}
	else {
		printf("Symbol name not found!!\n");
	}
}

void delete(node* hashtable[], char key[]) {
	int hashCode;

	hashCode = hash(key);
	deleteFromChain(hashtable, key, hashCode);
}

int find(node* hashArray[], char key[]) {

	int hashCode = hash(key);

	node* curr = hashArray[hashCode];
	while(curr != NULL) {
		if(strcmp(curr->key, key) == 0) {
			return curr->value;
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
		printf("%s\t%X\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}	
}

void display(node *hashArray[]) {
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

void printMenu() {
	
	printf("****MENU****\n");
	printf("1. Insert\n2. Delete\n3. Search\n4. Display\n");
	printf("5. Exit\n");
}

void main() {
	
	for(int i = 0; i < capacity; i++) {
		symtab[i] = NULL;
	}

	char symbol[50];
	int data;
	int choice;

	int a = 1;
	while(a) {
		
		printMenu();

		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch(choice) {
			case 1:
				printf("Enter the symbol to be inserted: ");
				scanf("%s", symbol);
	
				printf("Enter the value: ");
				scanf("%X", &data);

				insert(symtab, symbol, data);
				break;
			case 2:
				printf("Enter the symbol to be deleted: ");
				scanf("%s", symbol);

				delete(symtab, symbol);
				break;
			case 3:
				printf("Enter the symbol to be searched: ");
				scanf("%s", symbol);

				int value = find(symtab, symbol);
				if(value == -1) {
					printf("Symbol not found\n");
				}
				else {
					printf("Value of %s: %X\n", symbol, value);
				}
				break;
			case 4:
				printf("--------------\n");
				printf("   SYMTAB\n");
				printf("--------------\n");
				display(symtab);
				printf("--------------\n");
				
				break;	
			case 5:
				a = 0;
				break;
			default:
				printf("Invalid Entry!!!\n");
		}
	}
	
	clearHashTable(symtab);
}