#ifndef OSLAB_PCTL_H
#define OSLAB_PCTL_H

#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void sigcat() {
    printf("%d Process continue\n", getpid());
}

#endif //OSLAB_PCTL_H
