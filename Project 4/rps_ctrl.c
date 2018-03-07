#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void catch_sigrtmin(int sig);
void catch_sigrtmin1(int sig);
void catch_sigrtmin2(int sig);
void catch_sigrtmin3(int sig);
void catch_alarm(int sig);

static int fd;
char buffer[5];

int main(){
	
	fd = open("project4.txt", O_WRONLY | O_CREAT);
	printf("HERE");
	signal(SIGALRM, catch_alarm);
	signal(SIGRTMIN, catch_sigrtmin);
	signal(SIGRTMIN+1, catch_sigrtmin1);
	signal(SIGRTMIN+2, catch_sigrtmin2);
	signal(SIGRTMIN+3, catch_sigrtmin3);
	alarm(1);
	while (1) pause();
	alarm(0);
	return 0;
}

void catch_alarm(int sig){
	printf("CAUGHT");
	alarm(1);
}

void catch_sigrtmin(int sig){
	printf("HERE");
	strcpy(buffer, "0\n");
	write(fd, buffer, strlen(buffer));
	alarm(1);
}

void catch_sigrtmin1(int sig){
	printf("HERE");
	strcpy(buffer, "1\n");
	write(fd, buffer, strlen(buffer));
	alarm(1);
}

void catch_sigrtmin2(int sig){
        printf("HERE");
	strcpy(buffer, "2\n");
	write(fd, buffer, strlen(buffer));
	alarm(1);
}

void catch_sigrtmin3(int sig){
        printf("HERE");
	strcpy(buffer, "3\n");
	write(fd, buffer, strlen(buffer));
	alarm(1);
}
