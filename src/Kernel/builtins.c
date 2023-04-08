#include <stdio.h>
#include <unistd.h> 

#include "user.h"

void my_sleep(void *arg) {
    char *ticks = (char *)arg;
    // printf("ticks in my_sleep = %s\n", ticks);

    int t = atoi(ticks);
    if (t == 0) {
        perror("Fail to atoi");
    }
    
    p_sleep(t * 10);
}

void zombie_child() {
    return;
}

void zombify() {
    char *zombie_arg[2] = {"zombie", NULL};
    p_spawn(zombie_child, zombie_arg, 0, STDIN_FILENO, STDOUT_FILENO);
    while (1);
    return;
}

void orphan_child() {
    while (1);
}

void orphanify() {
    char *orphan_arg[2] = {"orphan", NULL};
    p_spawn(orphan_child, orphan_arg, 0, STDIN_FILENO, STDOUT_FILENO);
    return;
}