#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
void fx(int *num);
void fy(int *num);
void fxy(int *num);
void generateFx(int x);
void generateFy(int y);
void sigcat();
int pipe11[2];
int pipe12[2];
int pipe21[2];
int pipe22[2];
int fxRes[1000], numx = 0;
int fyRes[1000], numy = 0;
pthread_t thrd1, thrd2, thrd3;

int main(int argc, char **argv) {
    int ret;
    int num1, num2, num3;

    generateFx(1);
    generateFy(2);

    signal(SIGINT, sigcat);

    if (pipe(pipe11) < 0) {
        perror("pipe not estate");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe12) < 0) {
        perror("pipe not estate");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe21) < 0) {
        perror("pipe not estate");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe22) < 0) {
        perror("pipe not estate");
        exit(EXIT_FAILURE);
    }

    num1 = 1;
    ret = pthread_create(&thrd1, NULL, fx, (void *)&num1);
    if (ret) {
        perror("pthead_create: fx");
        exit(EXIT_FAILURE);
    }
    num2 = 2;
    ret = pthread_create(&thrd2, NULL, fy, (void *)&num2);
    if (ret) {
        perror("pthead_create: fy");
        exit(EXIT_FAILURE);
    }
    num3 = 3;
    ret = pthread_create(&thrd3, NULL, fxy, (void *)&num3);
    if (ret) {
        perror("pthead_create: fxy");
        exit(EXIT_FAILURE);
    }

    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd3, NULL);
    exit(EXIT_SUCCESS);
}

void generateFx(int x) {
    if (x == 1) fxRes[0] = 1;
    else for (int i = numx + 1; i <= x; i++) {
        fxRes[i-1] = fxRes[i-2] * i;
    }

    numx = x;
}

void generateFy(int y) {
    if (y == 1 || y == 2) {
        fyRes[0] = 1;
        fyRes[1] = 1;
    }
    else for (int i = numy; i < y; i++) {
        fyRes[i] = fyRes[i-1] + fyRes[i-2];
    }

    numy = y;
}

void fx(int *num) {
    int x;
    while(1) {
        read(pipe11[0], &x, sizeof(int));
        printf("thread%d read: %d\n", *num, x);

        if (x < numx) {
            write(pipe12[1], &fxRes[x-1], sizeof(int));
        }
        else {
            generateFx(x);
            write(pipe12[1], &fxRes[x-1], sizeof(int));
        }
    }
}

void fy(int *num) {
    int y;
    while(1) {
        read(pipe21[0], &y, sizeof(int));
        printf("thread%d read: %d\n", *num, y);

        if (y < numy) {
            write(pipe22[1], &fyRes[y-1], sizeof(int));
        }
        else {
            generateFy(y);
            write(pipe22[1], &fyRes[y-1], sizeof(int));
        }
    }
}

void fxy(int *num) {
    int x, y;
    int res;
    int opt;
    while(1) {
        printf("Usage: input 1 and x    to get f(x)\n");
        printf("       input 2 and y    to get f(y)\n");
        printf("       input 3 and x, y to get f(x, y)\n");

        scanf("%d", &opt);
        if (opt == 1) {
            scanf("%d", &x);
            write(pipe11[1], &x, sizeof(int));
            read(pipe12[0], &res, sizeof(int));
            printf("thread %d get fx(%d): %d\n", *num, x, res);
        }
        if (opt == 2) {
            scanf("%d", &y);
            write(pipe21[1], &y, sizeof(int));
            read(pipe22[0], &res, sizeof(int));
            printf("thread %d get fx(%d): %d\n", *num, y, res);
        }
        if (opt == 3) {
            scanf("%d", &x);
            scanf("%d", &y);
            int tmp;
            write(pipe11[1], &x, sizeof(int));
            read(pipe12[0], &res, sizeof(int));

            write(pipe21[1], &y, sizeof(int));
            read(pipe22[0], &tmp, sizeof(int));

            res += tmp;
            printf("thread %d get f(%d, %d): %d\n", *num, x, y, res);
        }

        printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    }
}

void sigcat() {
    close(pipe11[0]);
    close(pipe11[1]);
    close(pipe12[0]);
    close(pipe12[1]);
    close(pipe21[0]);
    close(pipe21[1]);
    close(pipe22[0]);
    close(pipe22[1]);
    exit(EXIT_SUCCESS);
}