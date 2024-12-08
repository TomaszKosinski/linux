#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void signal_handler(int signum) {
    /* signal habdler */
    if (signum == SIGUSR1) {
        printf("Proces %d gets SIGUSR1 from %d\n", getpid(), getppid());
    }
}

int main() {
    pid_t pid;

    /* Signal habdler registration */
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        perror("handler registration error");
        exit(1);
    }

    pid = fork(); // Tworzenie procesu potomnego

    if (pid < 0) {
        // Błąd przy tworzeniu procesu
        perror("fork() error");
        exit(1);
    }

    if (pid == 0) {

        printf("Child PID = %d, parent PID = %d\n", getpid(), getppid());
        
        /* Child is waiting for signals*/
        while(1) {
            pause();
        }
    } else {
        /* Parent process */
        printf("Parent PID = %d\n", getpid());

        sleep(1);
        kill(pid, SIGUSR1); /* send signal to child */

        /* wait for child to die */
        wait(NULL);
        printf("Child died\n");
    }

    return 0;
}