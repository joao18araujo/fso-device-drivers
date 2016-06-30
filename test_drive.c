#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char * argv[]){
	int fd;
	char ans[10000];

	if(argc < 2){
		printf("Passe o número do CPF a ser validado\n");
		return -1;
	}

	fd = open("/dev/chardev", O_RDWR);
	if(fd == -1){
		printf("Failed to open chardev\n");
		return -1;
	}

	write(fd, argv[1], strlen(argv[1]));
	
	while(read(fd, ans, 256));

	printf("Retorno: %s", ans);

	return 0;
}
