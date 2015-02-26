#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
int main(int argc, char *argv[]){
	int arg1 = atoi(argv[1]);
	int arg2 = atoi(argv[2]);
	int shmid;
	read(argv[3][0], &shmid, sizeof(shmid));
	close(argv[3][0]);
	int *addr = shmat(shmid, 0, 0),out;
	
//	printf("addr_before = %d\n", *addr);
	printf("Checker Process [%d]: starting.\n", getpid());
	printf("Checker Process [%d]: read %d bytes containing shm ID %d\n", getpid(), 4, shmid);
	if (arg2 % arg1 !=0){
		printf("Checker Process [%d]: %d *IS NOT* divisible by %d.\n", getpid(), arg2, arg1);
		out = 0;
		printf("Checker Process [%d]: wrote result (%d) to shared memory.\n", getpid(), out);
		memcpy(addr, &out, 1);
		//printf("addr(C) = %d\n", *addr);
		exit(0);
	}else if(arg2 % arg1==0){
		printf("Checker Process [%d]: %d *IS* divisible by %d.\n", getpid(), arg2, arg1);
		out = 1;
		printf("Checker Process [%d]: wrote result (%d) to shared memory.\n", getpid(), out);
		memcpy(addr, &out, 1);
		exit(1);
	}
}
 
