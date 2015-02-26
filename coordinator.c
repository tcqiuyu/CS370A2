#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
int main(int argc, char *argv[]){
	int shmid[5];
	int status[5]={0,0,0,0,0};
	pid_t cpid[5];
	int fd[2];
	int *addr[5] = {0,0,0,0,0};	

	for(int i=0; i<4; i++){
		pipe(fd);
		shmid[i] = shmget(IPC_PRIVATE, 4096, S_IRUSR|S_IWUSR);	
		cpid[i] = fork();
		if (cpid[i] > 0){
			close(fd[0]);
			printf("Coordinator: forked process with ID %d.\n", cpid[i]);
			printf("Coordinator: wrote shm ID %d to pipe (%d bytes)\n", shmid[i], 4);
			write( fd[1], &shmid[i], sizeof(shmid[i]));
			close(fd[1]);
		}
		else if(cpid[i] == 0){
			close(fd[1]);
			execlp("Qiu_Yu_Checker.out", "Qiu_Yu_Checker.out", argv[1], argv[i+2], fd, (char*)NULL);
			close(fd[0]);
		}
	
	}
	for(int i=0; i<4; i++){
		printf("Coordinator: waiting on child process ID %d...\n", cpid[i]);
		waitpid(cpid[i], &status[i], 0);
		addr[i] = shmat(shmid[i], NULL, 0);	
		switch(*addr[i]){
			case 1: printf("Coordinator: result %d read from shared memory: %d *IS* divisible by %d.\n", *addr[i], atoi(argv[i+2]), atoi(argv[1])); break;
			case 0: printf("Coordinator: result %d read from shared memory: %d *IS NOT* divisible by %d.\n", *addr[i], atoi(argv[i+2]), atoi(argv[1])); break;
		}		
		shmctl(shmid[i], IPC_RMID, 0);
	}
	printf("Coordinator: exiting.\n");
	exit(1);
}

