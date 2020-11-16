#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 11

struct symbol {
	int assigned; //if this space is free (1) or not (0)
	char add[20];
	char label[10];
	int forRef[10]; //list of addresses forward referenced
	int count; //count of addresses in the list
}

symTab[11];

int hashKey(char c) {
	int key;
	key = c % 11;
	return key;
}

int AddToSym(int key, char l[], char add[]) { //Linear Probing
	
	int count = 0;
	if (symTab[key].assigned == 0) {
		strcpy(symTab[key].add,add);
		strcpy(symTab[key].label, l);
		symTab[key].assigned=1;
	}
	else {
		int spaceFound = 0;
		for(int i = key+1; count<MAX-1; i = (i+1)%MAX) {
			if(symTab[i].assigned == 0) {
				spaceFound = 1;

				strcpy(symTab[i].add, add);
				strcpy(symTab[i].label, l);

				symTab[i].assigned = 1;
				key = i;

				break;
			}
			count++;
		}
		if(!spaceFound) {
			exit(1);
		}
	}
	return key;
}

int search(char label[]) {

	int key;
	key = hashKey(label[0]);

	int count = 0, found = 0;
	for(int i=key;count<MAX;i=(i+1)%MAX) {
		if(strcmp(symTab[i].label,label)==0) {
			found = 1;
			return i;
		}
		count++;
	}

	if(!found) {
		return -1;
	}
}

int main() {

	FILE *fin, *ftab, *flen, *fsym;
	int i, len, foundSym = 0;
	char add[20], symadd[5], op[5], start[10], label[20];
	char mne[10], operand[10], symtab[10], opmne[10];
	int count;
	int key, newKey, symIndex;
	char str[500];
	char currRec[200] = "";
	char objCode[10000] = "";
	char recStart[50];
	char Pname[50];
	int recLen = 0;
	int locctr = 0;

	for (i = 0; i < MAX; i++) {
		symTab[i].count=-1;
		symTab[i].assigned=0;
	}

	fin = fopen("input.txt", "r");
	ftab = fopen("optab.txt", "r");

	fscanf(fin, "%s %s %s", label, mne, operand);
	if (strcmp(mne, "START") == 0) {
		strcpy(start, operand);
		strcpy(recStart, operand);
		strcpy(Pname, label);
		sprintf(add, "%s", start);
		locctr = atoi(start);
	}

	fscanf(fin, " %s %s %s", label, mne, operand);
	while (strcmp(mne,"END")!= 0 && !feof(fin)) {

		foundSym = 0;
		if(strcmp(label,"-")!=0) {

			symIndex=search(label);
			if(symIndex==-1) {
				key = hashKey(label[0]);
				AddToSym(key,label,add);
			}
			else {
				if(symTab[symIndex].count>=0) {
					
					if(strcmp(currRec,"")!=0) {
						sprintf(str,"\nT^00%s^%d^%s", recStart,recLen,currRec);
						strcat(objCode,str);
					}
					strcpy(currRec,"");
					strcpy(recStart,add);
					recLen=0;
				}
				while(symTab[symIndex].count>=0) {
					i = symTab[symIndex].count;
					sprintf(str,"\nT^00%d^02^%s", symTab[symIndex].forRef[i], add);
					strcat(objCode, str);
					symTab[symIndex].count--;
				}
			}
		}

		fscanf(ftab, "%s %s", opmne, op);
		while (!feof(ftab)) {

			if (strcmp(mne, opmne) == 0) {

				symIndex = search(operand);

				if(symIndex == -1) {

					key = hashKey(operand[0]);
					newKey = AddToSym(key,operand,"*");
					symTab[newKey].count = 0;
					symTab[newKey].forRef[0] = atoi(add) + 1;

					sprintf(str,"%s0000^", op);
					strcat(currRec, str);

					recLen += 3;
					locctr += 3;

					sprintf(add,"%d",locctr);
				}
				else if(strcmp(symTab[symIndex].add,"*")) {

					symTab[symIndex].count++;
					count = symTab[symIndex].count;
					symTab[symIndex].forRef[count] = atoi(add)+1;

					sprintf(str,"%s0000^", op);
					strcat(currRec, str);

					recLen += 3;
					locctr += 3;

					sprintf(add,"%d", locctr);
				}
				else {
					sprintf(str,"%s%s^", op, symTab[symIndex].add);
					strcat(currRec,str);

					recLen+=3;
					locctr+=3;

					sprintf(add, "%d", locctr);
				}
				break;
			}
			fscanf(ftab, "%s %s", opmne, op);
		}
		if ((strcmp(mne, "BYTE") == 0) || (strcmp(mne, "WORD") == 0)) {

			if (strcmp(mne, "WORD") == 0) {

				sprintf(str,"0000%s^", operand);
				strcat(currRec,str);

				recLen += 3;
				locctr += 3;
				sprintf(add, "%d", locctr);
			}
			else {

				len = strlen(operand);
				for (int i = 2; i < len-1; i++) {
					sprintf(str,"%d", operand[i]);
					strcat(currRec,str);
				}
				strcat(currRec,"^");

				recLen+=len-3;
				locctr+=len-3;

				sprintf(add,"%d",locctr);
			}
		}
		if(strcmp(mne, "RESW")==0) {

			locctr += 3 * atoi(operand);
			sprintf(add, "%d", locctr);
		}
		if(strcmp(mne, "RESB") == 0) {
			locctr += atoi(operand);
			sprintf(add, "%d", locctr);
		}

		fscanf(fin, "%s %s %s", label, mne, operand);
		fseek(ftab, SEEK_SET, 0);
	}
	if(strcmp(currRec,"")!=0) {
		sprintf(str,"\nT^00%s^%d^%s", recStart,recLen,currRec);
		strcat(objCode,str);
	}

	len = atoi(add) - atoi(start);
	printf("H^%s^%s^%d", Pname, start, len);
	printf("%s",objCode);
	printf("\nE^00%s\n", start);

	fclose(fin);
	fclose(ftab);
}
