/*******************************************************************************************
 * Extra Credit Project: A Shell                                                           *
 * *****************************************************************************************
 * Author: Matthew Viego                                                                   *
 *                                                                                         *
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char token1[100]; char token2[100]; char token3[100]; char token4[100]; char token5[100]; char token6[100]; char token7[100]; char token8[100];
char token9[100]; char token10[100]; char token11[100]; char token12[100];
char *tokens[12] = {token1, token2, token3, token4, token5, token6, token7, token8, token9, token10, token11, token12};
int command = 0;
char *file;
int i = 0;
FILE *f;
char userInput[100];
char* token;
char s[] = " \t\n";
char *holder[1200];

void changeDirectory(char* directory);
void commandTime();

void commandTime(){
	if (fork() == 0){
		(holder[i])=NULL;
		if (command == 1){
			f = freopen(file, "a", stdout);
		}
		else if (command == 2){
			f = freopen(file, "r", stdin);
		}
		else if(command == 3){
			f = freopen(file, "w", stdout);
		}
		else{
			if(f!=NULL){
				fclose(f);
			}
		}
		execvp(holder[0], holder);
	}
	
	wait(NULL);
}

int main(){
    while(1){
		i = 0;
		command = 0;
		printf("myshell $ ");
		fgets(userInput, 100, stdin);
		userInput[strlen(userInput) - 1] = 0x00;
		if (strcmp(userInput, "exit") == 0){
			exit(0);
		}
		else{
			token = strtok(userInput, s);
			while (token != NULL){
				if (strcmp(token, ">>") == 0){
					command = 1;
				}
				else if(strcmp(token, "<") == 0){
					command = 2;
				}
				else if(strcmp(token, ">") == 0){
					command = 3;
				}
				 
				strncpy(tokens[i++], token, 100);
				token = strtok(NULL, s);				
			}
			
			if (command != 0){
				file = tokens[i - 1]; i = i - 2;
			}
			
			if (strcmp(token1, "cd") == 0){
				changeDirectory(token2);
			}
			else{
				memcpy(holder, tokens, sizeof(tokens));
				commandTime();
			}
		}
    }
}

void changeDirectory(char* directory){
	char currentDirectory[100];
	int result;
	
	if (i == 1){
		printf("Please enter a valid directory\n");
		return;
	}
	
	if (directory == "/"){
		result = chdir((const char*)directory);
		if (result == -1){
			printf("This directory does not exist.\n");
			return;
		}
		else{
			getcwd(currentDirectory, 100);
			printf("Directory has been changed to %s\n", currentDirectory);
			return;
		}
	}
	
	getcwd(currentDirectory, 100);
	strcat(currentDirectory, "/");
	strcat(currentDirectory, directory);
	
	result = chdir((const char *)currentDirectory);
	
	if (result == -1){
		printf("This directory does not exist.\n");
		return;
	}
	else{
		getcwd(currentDirectory, 100);
		printf("Directory has been changed to %s\n", currentDirectory);
	}
}
