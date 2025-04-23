#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int pipefd[2];
    pipe(pipefd); // Create a pipe for synchronization

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close read end
        printf("hello\n");
        fflush(stdout);   // Force the output to appear immediately
        write(pipefd[1], "1", 1); // Send a signal to the parent
        close(pipefd[1]); // Close write end
    } else {
        // Parent process
        char buf;
        close(pipefd[1]); // Close write end
        read(pipefd[0], &buf, 1); // Wait for child to signal
        close(pipefd[0]); // Close read end
        printf("goodbye\n");
    }

    return 0;
}