
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>

void win(int);
int vuln(int, char**, char**);

int asdf()
{
    __asm__ __volatile__ (
        ".intel_syntax noprefix;"
		"push rdi;"
		"pop rdi;"
		"ret;"
        ".att_syntax;"
		:
		:
		:
	);
	exit(0);
}

int main(int argc, char **argv, char **envp)
{
	puts("===================================================");
	printf("\tWelcome to %s!\n", argv[0]);
	puts("===================================================");
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 1);

	return vuln(argc, argv, envp);
}

int vuln(int argc, char **argv, char **envp)
{

	struct { char input[344]; uint64_t n; } frame;
	uint64_t *n = &frame.n;
	char *input = (char *)&frame.input;

	short size;
	printf("Payload size? ");
	scanf("%hi", &size);
	assert(size <= 352);
	printf("Send your payload (up to %d bytes)!\n", size);
	read(0, input, (unsigned short)size);

	puts("Goodbye!");
	return 0;
}

void win(int win_token)
{

	puts("You win! Here is your flag:");
	register int flag_fd = open("/flag", 0);
	sendfile(1, flag_fd, 0, 1024);
}
