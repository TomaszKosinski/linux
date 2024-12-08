#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    char buffer[100];
    pid_t pid;

    /* Pipe creation */
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Child process - close write fd */
        close(pipefd[1]);
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(pipefd[0]);
    } else {
        /* Parent proces - close read fd */
        close(pipefd[0]);
        char *message = "Message from parent to child!";
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
    }

    return 0;
}