#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(fd);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process writes to the file
        if (write(fd, "Child writes to the file.\n", 26) == -1) {
            perror("child write");
        }
    } else {
        // Parent process writes to the file
        if (write(fd, "Parent writes to the file.\n", 27) == -1) {
            perror("parent write");
        }
        // Wait for the child to finish
        wait(NULL);
    }

    close(fd);
    return 0;
}