#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    const char *pipe_path = "/tmp/my_named_pipe";


    int pipe_fd = open(pipe_path, O_RDONLY);
    if (pipe_fd == -1) {
        perror("open");
        return 1;
    }

    char buffer[100];
    read(pipe_fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    close(pipe_fd);
    printf("Receiver close\n");
    return 0;
}

