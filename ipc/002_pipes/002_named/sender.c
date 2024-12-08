#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    const char *pipe_path = "/tmp/my_named_pipe";

    /* Create named pipe if do not exist */
    if (mkfifo(pipe_path, 0666) == -1) {
        perror("mkfifo");
        return 1;
    }

    
    int pipe_fd = open(pipe_path, O_WRONLY);
    if (pipe_fd == -1) {
        perror("open");
        return 1;
    }

    const char *msg = "Hello from process 1";
    write(pipe_fd, msg, strlen(msg) + 1);
    printf("Data written to pipe\n");

    close(pipe_fd);
    printf("Sender close\n");
    return 0;
}

