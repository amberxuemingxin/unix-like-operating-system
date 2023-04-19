#include <unistd.h>

#include "execute.h"
#include "user.h"
#include "builtins.h"
#include "scheduler.h"
#include "stress.h"
#include "../pennFAT/FAT.h"
#include "../pennFAT/macro.h"
#include "../pennFAT/pennfatlib.h"

extern job_list *list;
extern bool file_system;

/* execute the commands
* return = 0: success
* return = -1: failure
*/
int execute(struct parsed_command *cmd, job *j, int priority) {
    char *buf;
    int size;
    pid_t child = 0;
    // place holder

    if (strcmp(cmd->commands[0][0], "sleep") == 0) {
        if (!cmd->commands[0][1]) {
            printf("Please input an argument for sleep!\n");
            remove_job(j, list, false);
            free_job(j);
            return FAILURE;
        }
        size = strlen(cmd->commands[0][1])+1;
        buf = malloc(size);
        memcpy(buf, cmd->commands[0][1], size);

        char *sleep_arg[3] = {"sleep", buf, NULL};
        child = p_spawn(my_sleep, sleep_arg, 1, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "zombify") == 0) {
        char *zombie_arg[2] = {"zombify", NULL};
        child = p_spawn(zombify, zombie_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "orphanify") == 0) {
        char *orphan_arg[2] = {"orphanify", NULL};
        child = p_spawn(orphanify, orphan_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "ps") == 0) {
        char *ps_arg[2] = {"ps", NULL};
        child = p_spawn(ps, ps_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "hang") == 0) {
        char *hang_arg[2] = {"hang", NULL};
        child = p_spawn(hang, hang_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "nohang") == 0) {
        char *nh_arg[2] = {"nohang", NULL};
        child = p_spawn(nohang, nh_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "recur") == 0) {
        char *recur_arg[2] = {"recur", NULL};
        child = p_spawn(recur, recur_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "busy") == 0) {
        char *busy_arg[2] = {"busy", NULL};
        child = p_spawn(busy, busy_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (strcmp(cmd->commands[0][0], "kill") == 0) {
        if (!cmd->commands[0][1] || !cmd->commands[0][2]) {
            printf("Please input an argument for kill [signal] [pid]!\n");
            remove_job(j, list, false);
            free_job(j);
            return FAILURE;
        }

        char *kill_arg[4] = {"kill", cmd->commands[0][1], cmd->commands[0][2], NULL};
        int pid_num = atoi(cmd->commands[0][2]);
        if (pid_num == 0)
        {
            printf("Fail to atoi\n");
            remove_job(j, list, false);
            free_job(j);
            return FAILURE;
        } else if (pid_num == 1 ) {
            remove_job(j, list, false);
            free_job(j);
            return FAILURE;
        }

        if (strcmp(cmd->commands[0][1], "term") == 0) {
            /* kill the job */
            job *job_to_remove = find_by_pid(pid_num, list);
            remove_job(job_to_remove, list, true);
            remove_job(job_to_remove, list, false);
            free_job(job_to_remove);
        }

        /* kill the process */
        child = p_spawn(my_kill, kill_arg, 2, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (file_system && strcmp(cmd->commands[0][0], "cat") == 0) {
        char **cat_arg = cmd->commands[0];
        child = p_spawn((void *)pennfat_cat, cat_arg, 1, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (file_system && strcmp(cmd->commands[0][0], "echo") == 0) {
        // TODO

    } else if (file_system && strcmp(cmd->commands[0][0], "ls") == 0) {
        char *ls_arg[2] = {"ls", NULL};
        child = p_spawn((void *)pennfat_ls, ls_arg, 0, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (file_system && strcmp(cmd->commands[0][0], "touch") == 0) {
        char **touch_arg = cmd->commands[0];
        child = p_spawn((void *)pennfat_touch, touch_arg, 1, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (file_system && strcmp(cmd->commands[0][0], "mv") == 0) {
        if (!cmd->commands[0][1] && !cmd->commands[0][2]) {
            printf("Please input an argument for mv!\n");
            remove_job(j, list, false);
            free_job(j);
            return FAILURE;
        }
        char *mv_arg[4] = {"mv", cmd->commands[0][1], cmd->commands[0][2], NULL};
        child = p_spawn((void *)pennfat_mv, mv_arg, 2, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (file_system && strcmp(cmd->commands[0][0], "cp") == 0) {
        char **cp_arg = cmd->commands[0];
        child = p_spawn((void *)pennfat_cp, cp_arg, 1, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (file_system && strcmp(cmd->commands[0][0], "rm") == 0) {
        char **rm_arg = cmd->commands[0];
        child = p_spawn((void *)pennfat_remove, rm_arg, 1, j->fd0, j->fd1, priority, cmd->is_background);
    } else if (file_system && strcmp(cmd->commands[0][0], "chmod") == 0) {
        char **ch_arg = cmd->commands[0];
        child = p_spawn((void *)pennfat_chmod, ch_arg, 1, j->fd0, j->fd1, priority, cmd->is_background);
    } else {
        /* invalid input */
        remove_job(j, list, false);
        free_job(j);
        return FAILURE;
    }

    j->pid = child;
    j->pgid = child;
    return 0;
}