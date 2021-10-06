#include "execrise.h"

int main(int argc, char **argv)
{
    pid_t c1_pid = 1, c2_pid = 1;
    int status1, status2;

    char *args1[] = {"/bin/ps", "-l", NULL};
    char *args2[] = {"/bin/ls", "-l", NULL};

    while(c1_pid != 0 && c2_pid != 0) {
        c1_pid = fork();
        c2_pid = fork();

        if (c1_pid < 0 || c2_pid < 0) {
            printf("Create process failed.\n");
            return EXIT_FAILURE;
        }

        sleep(3);
    }

    if (c1_pid == 0) {
        printf("Child %d running \n", getpid());
        sleep(1);

        status1 = execve(args1[0], args1, NULL);
    }

    if (c2_pid == 0) {
        printf("Child %d running \n", getpid());
        status2 = execve(args2[0], args2, NULL);
    }
}