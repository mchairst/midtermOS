#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        close(STDOUT_FILENO);  // Close standard output
        // Attempt to print using printf after stdout is closed.
        printf("This message will not appear on the terminal.\n");
        fflush(stdout);  // Even flushing won't write out since stdout is closed.
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        wait(NULL);  // Wait for child to complete
        printf("Parent: This message prints normally.\n");
    }
    
    return 0;
}
