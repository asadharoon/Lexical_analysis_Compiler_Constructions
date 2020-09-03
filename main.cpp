#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
using namespace std;
// Asad Haroon
// SP17-BCS-012
// Section:C
/*
I have used small words as keywords and handle errors also.
In CSV File I have added some states in last for error handling and some states which I forgot to mention and I have appended it in Last of Excel.
So please dont get confuse due to my excel file.
*/
enum TOK { No = 999, Keyword = 1000, ID, OP, LIT, PUN,  IO };
void genToken(FILE* tokensGenerations, char* lex, int t) {
	if (t == Keyword) {
		fprintf(tokensGenerations, "<");
		fprintf(tokensGenerations, lex);
		fprintf(tokensGenerations, ",");
		fprintf(tokensGenerations, "keyword >\n");
	}
	if (t == ID) {
		fprintf(tokensGenerations, "<");
		fprintf(tokensGenerations, lex);
		fprintf(tokensGenerations, ",");
		fprintf(tokensGenerations, "ID >\n");
	}
	if (t == LIT) {
		fprintf(tokensGenerations, "<");
		fprintf(tokensGenerations, lex);
		fprintf(tokensGenerations, ",");
		fprintf(tokensGenerations, "literals>\n");
	}
	if (t == OP) {
		fprintf(tokensGenerations, "<");
		fprintf(tokensGenerations, lex);
		fprintf(tokensGenerations, ",");
		fprintf(tokensGenerations, "OP >\n");
	}
	if (t == IO) {
		fprintf(tokensGenerations, "<");
		fprintf(tokensGenerations, lex);
		fprintf(tokensGenerations, ",");
		fprintf(tokensGenerations, "IO >\n");
	}
	if (t == PUN) {
		fprintf(tokensGenerations, "<");
		fprintf(tokensGenerations, lex);
		fprintf(tokensGenerations, ",");
		fprintf(tokensGenerations, "PUN >\n");
	}
}
void generateErrorFile(FILE* errorList, char* lex,int t) {
	if (t == Keyword) {
		//fprintf(errorList, "\n");
		fprintf(errorList, lex);
		fprintf(errorList, " 'Invalid Keyword' \n");
	}
	if (t == ID) {
		fprintf(errorList, lex);
		fprintf(errorList, " 'Invalid Identifier' \n");
	}
	if (t == LIT) {
		fprintf(errorList, lex);
		fprintf(errorList, " 'Invalid Literal' \n");
	}
	if (t == OP) {
		fprintf(errorList, lex);
		fprintf(errorList, " 'Invalid Operator' \n");
	}
	if (t == IO) {
		fprintf(errorList, lex);
		fprintf(errorList, " 'Invalid Input Output' \n");
	}
	if (t == PUN) {
		fprintf(errorList, lex);
		fprintf(errorList, " 'Invalid Punctuation Marks' \n");
	}
	if (t == 1010) {
		fprintf(errorList, lex);
		fprintf(errorList, " 'Invalid Comment ' \n");
	}
}
int* getstate(int state,char c) {
	ifstream input;
	input.open("Assign2Chart(SP17-BCS-012).csv"); // change address
	string str = "";
	int *arr=(int*)malloc(sizeof(int)*5);
	string line, col;
	int lineNumber = 0;
	while (input.good()) {
		while (getline(input, line)) {
			if (lineNumber == state+2) {
				istringstream myline(line);
				int statenumber = -1;
				int actualstate = int(c);
				arr[0] = -1;
				arr[1] = -1;
				arr[2] = -1;
				arr[3] = -1;
			while (getline(myline, col, ',')) {
					if (statenumber == actualstate) {
						//cout << col << endl;
						stringstream item(col);
						int it = 0;
						item >> it;
						arr[0] = it;
					}
					if (statenumber == 128) //A
					{
						stringstream item(col);
						int it = 0;
						item >> it;
						arr[1] =it;
					}
					if (statenumber == 129) { //R

						stringstream item(col);
						int it = 0;
						item >> it;
						arr[2] = it;
					}
					if (statenumber == 130) { //T
						stringstream item(col);
						int it = 0;
						item >> it;
						arr[3] = it;
					}
					statenumber++;
			}
		}
			lineNumber++;
		}

	}
	return arr;
}
int main() {
	// Files
	ifstream obj;
	obj.open("sourceCode.txt"); //code file to read
	FILE* tokensGenerations;
	tokensGenerations = fopen("tokens.txt", "w+");
	FILE* errorList;
	errorList = fopen("errors.txt", "w+");
	//buffer and lexemes
	char b1[4096];
	char b2[4096];
	char lex[100];
	//counters for lexeme and buffer
	int bi = 0;
	int li = 0;

	char ch;
	int state = 0;
	// buffering
	char* curr_buff = b1;
	obj.get(curr_buff, 4096, EOF);
	while (curr_buff[bi] != '\0') {
		ch = (char)curr_buff[bi];
		printf("\n Ch=%c", ch);
		int* arr = getstate(state, ch);
		state = *(arr + 0);
		arr = getstate(state, ch);

		int A = *(arr + 1);
		int R = *(arr + 2);
		int T = *(arr + 3);
		if (A == 0)
		{
		    //printf("state is%d",state);
			if (state == 6 || state==103 || state==104 || state==105 || state==106 || state==107) // this is my error state for generating error file
			{
				lex[li++] = ch;
				bi++;
              //  printf("state is %d at char C=%d\n",state,ch);
				while (true) {
					ch = (char)curr_buff[bi];
					T = *(getstate(state, ch) + 3);
					state = *(getstate(state, ch)+0);
					//printf("state is %d and in error state at char c=%c\n",state,ch);
					if (state!=6 && state!=103 && state!=105 && state!=104 && state!=106 && state!=107) {
						break;
					}
					lex[li] = ch;
					li++;
					bi++;
				}
				lex[li] = ch;
				lex[li++] = '\0';

				generateErrorFile(errorList, lex,T);
				state = 0;
				li = 0;
				continue;
			}
			else if (state != 0) {
				lex[li] = ch;
				li++;
			}
			bi++;
		}
		else if (A)
		{
			if (R == 1)
			{
				lex[li] = '\0';
			}
			else {
				lex[li] = ch;
				li++;
				lex[li] = '\0';
				bi++;
			}
			li = 0;
			state = 0;
			genToken(tokensGenerations, lex, T);
		}
		if (bi > 4095)
		{
			if (curr_buff == b1) {
				curr_buff = b2;
			}
			else
				curr_buff = b1;
			obj.get(curr_buff, 4096, EOF);
			bi = 0;
			continue;
		}

	}
	printf("\nDone");
	fclose(errorList);
	fclose(tokensGenerations);
	//system("pause");
	return 0;
}
