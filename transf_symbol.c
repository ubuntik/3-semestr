#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/*parent's pid*/
pid_t pid;
/*child's pid*/
int id;
pid_t cid;

sigset_t mask1, mask2, oldmask;

int written_bit = 0;

void handler0(int nsig)
{
	written_bit = 0;
	signal(SIGUSR1, handler0);
}

void handler1(int nsig)
{
	written_bit = 1;
	signal(SIGUSR2, handler1);
}

void phandler(int nsig)
{
	signal(SIGUSR1, phandler);
}

int main(int argc, char* argv[]) 
{
	int nbit_i = 0;
	char buff;
	char out = 0;
	int nbit_o = 0;
	pid = getpid();
	signal(SIGUSR1, phandler);

	if (argc < 2) {
		fprintf(stderr, "Not enough args!\n");
		exit(-1);
	}

	if (strlen(argv[1]) != 1) {
		fprintf(stderr, "Wrong argument!\n");
		exit(-1);
	}

	buff = argv[1][0];

	sigemptyset(&mask1);
	sigaddset(&mask1, SIGUSR1);
	sigaddset(&mask1, SIGUSR2);
	sigemptyset(&mask2);
	sigaddset(&mask2, SIGUSR1);

	/* "block" other signals for receive USR1 */
	sigprocmask(SIG_BLOCK, &mask2, &oldmask);

	if ((id = fork()) < 0) {
		perror("fork failed");
		exit(errno);
	}
	if (id > 0) {	/* parent */
		//fprintf(stdout, ">> parent << pid %d -- id %d\n", getpid(),id);
		fflush(stdout);
		do {
			sigsuspend(&oldmask);
			(buff & 1 << nbit_i) ? \
				kill(id, SIGUSR2) : kill(id, SIGUSR1);
			/* all bits were sent */
			if (nbit_i == sizeof(char) * 8 - 1) {
				kill(id, SIGKILL);
				exit(0);
			}
			nbit_i++;
		} while (1);
	} else {		/* child */
		//fprintf(stdout, ">> child << pid %d -- id %d\n", getpid(), id);
		fflush(stdout);
		signal(SIGUSR1, handler0);	/*setting handlers*/
		signal(SIGUSR2, handler1);
		sigprocmask(SIG_BLOCK, &mask1, NULL);
		cid = getpid();		/*child's pid*/
		kill(pid, SIGUSR1);	/*telling hi to parent*/
		do {			/*waiting for signals*/
			sigsuspend(&oldmask);
			if (written_bit == 1) 
				out = (1 << nbit_o) | out;
			nbit_o++;
			/* all bits were received */
			if (nbit_o == sizeof(char) * 8 - 1) {
				fprintf(stdout, "%c\n", out);
			}
			kill(pid,SIGUSR1);
		} while (1);
	}

	return 0;
}

