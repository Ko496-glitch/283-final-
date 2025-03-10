
/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include "dshlib.h"

static char *trim_whitespace(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == '\0')
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = '\0';
    return str;
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    clist->num = 0;
    int cmd_count = 0;

    char *saveptr1;
    char *commandStr = strtok_r(cmd_line, PIPE_STRING, &saveptr1);
    while (commandStr != NULL)
    {
        if (cmd_count >= CMD_MAX)
            return ERR_TOO_MANY_COMMANDS;

        if (alloc_cmd_buff(&clist->commands[cmd_count]) != OK)
            return ERR_MEMORY;

        char *token;
        char temp[SH_CMD_MAX + 1];
        strncpy(temp, trim_whitespace(commandStr), SH_CMD_MAX);
        temp[SH_CMD_MAX] = '\0';

        int argc = 0;
        token = strtok(temp, " ");
        while (token && argc < CMD_ARGV_MAX - 1) {
            clist->commands[cmd_count].argv[argc] = strdup(token);
            if (!clist->commands[cmd_count].argv[argc])
                return ERR_MEMORY;

            argc++;
            token = strtok(NULL, " ");
        }
        clist->commands[cmd_count].argv[argc] = NULL;
        clist->commands[cmd_count].argc = argc;

        cmd_count++;
        commandStr = strtok_r(NULL, PIPE_STRING, &saveptr1);
    }

    if (cmd_count == 0)
        return WARN_NO_CMDS;

    clist->num = cmd_count;
    return OK;
}

int exec_local_cmd_loop()
{
    char cmd_line[SH_CMD_MAX + 1];
    command_list_t clist;

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        cmd_line[strcspn(cmd_line, "\n")] = '\0';

        int build_result = build_cmd_list(cmd_line, &clist);
        if (build_result == WARN_NO_CMDS)
        {
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        else if (build_result == ERR_TOO_MANY_COMMANDS)
        {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }
        else if (build_result == ERR_MEMORY)
        {
            printf("error: memory allocation failed\n");
            continue;
        }

        execute_pipeline(&clist);

        for (int i = 0; i < clist.num; i++)
            free_cmd_buff(&clist.commands[i]);
    }
    return OK;
}

int execute_pipeline(command_list_t *clist)
{
    int pipefd[CMD_MAX - 1][2];

    for (int i = 0; i < clist->num - 1; i++)
    {
        if (pipe(pipefd[i]) == -1)
        {
            perror("pipe");
            return ERR_EXEC_CMD;
        }
    }

    for (int i = 0; i < clist->num; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            if (i != 0)
                dup2(pipefd[i - 1][0], STDIN_FILENO);

            if (i != clist->num - 1)
                dup2(pipefd[i][1], STDOUT_FILENO);

            for (int j = 0; j < clist->num - 1; j++)
            {
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork failed");
            return ERR_EXEC_CMD;
        }
    }

    for (int i = 0; i < clist->num - 1; i++)
    {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    int status;
    for (int i = 0; i < clist->num; i++)
        wait(&status);

    return OK;
}


int free_cmd_buff(cmd_buff_t *cmd_buff)
{
   for (int i = 0; i < cmd_buff->argc; i++)
   {
       free(cmd_buff->argv[i]);
   }
   return OK;
}
int clear_cmd_buff(cmd_buff_t* cmd_buff)
{
   if (!cmd_buff) {
       return -1;
   }
   for (int i = 0; i < cmd_buff->argc; ++i) {
       if (cmd_buff->argv[i] != NULL) {
           free(cmd_buff->argv[i]);
           cmd_buff->argv[i] = NULL;
       }
   }
   cmd_buff->argc = 0;
   if (cmd_buff->_cmd_buffer != NULL) {
       free(cmd_buff->_cmd_buffer);
       cmd_buff->_cmd_buffer = NULL;
   }


   return OK;
}
int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{
   for (int i = 0; i < CMD_ARGV_MAX; i++)
   {
       cmd_buff->argv[i] = NULL;
   }
   cmd_buff->argc = 0;
   return OK;
}
