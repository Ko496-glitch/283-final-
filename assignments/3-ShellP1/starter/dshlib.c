#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

/*
 * build_cmd_list:
 *   cmd_line: a complete command line entered by the user.
 *   clist:    pointer to a command_list_t structure to be populated.
 *
 * The function splits the cmd_line into commands using the pipe ('|') character
 * as a delimiter. For each command, it trims whitespace, then tokenizes by spaces.
 * The first token is stored as the executable name; subsequent tokens are concatenated
 * (with a single space separating them) into the arguments string.
 *
 * Return codes:
 *   OK                      if parsing was successful.
 *   WARN_NO_CMDS            if no non-blank commands were provided.
 *   ERR_TOO_MANY_COMMANDS   if the number of commands exceeds CMD_MAX.
 *   ERR_CMD_OR_ARGS_TOO_BIG if an executable or its arguments exceed the allowed size.
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    clist->num = 0;
    int cmd_count = 0;


    char *p = cmd_line;
    while (*p) {
        if (!isspace((unsigned char)*p))
            break;
        p++;
    }
    if (*p == '\0')
        return WARN_NO_CMDS;


    char *saveptr1;
    char *commandStr = strtok_r(cmd_line, PIPE_STRING, &saveptr1);
    while (commandStr != NULL)
    {
    
        char *trimmedCmd = trim_whitespace(commandStr);

        if (strlen(trimmedCmd) > 0)
        {
            if (cmd_count >= CMD_MAX)
                return ERR_TOO_MANY_COMMANDS;

        
            char temp[SH_CMD_MAX + 1];
            strncpy(temp, trimmedCmd, SH_CMD_MAX);
            temp[SH_CMD_MAX] = '\0';

            
            char *saveptr2;
            char *token = strtok_r(temp, " ", &saveptr2);
            if (token == NULL)
            {
            
                commandStr = strtok_r(NULL, PIPE_STRING, &saveptr1);
                continue;
            }


            if (strlen(token) >= EXE_MAX)
                return ERR_CMD_OR_ARGS_TOO_BIG;
            strcpy(clist->commands[cmd_count].exe, token);

            clist->commands[cmd_count].args[0] = '\0';

    
            token = strtok_r(NULL, " ", &saveptr2);
            while (token != NULL)
            {
            
                if (strlen(clist->commands[cmd_count].args) > 0)
                {
                    if (strlen(clist->commands[cmd_count].args) + 1 >= ARG_MAX)
                        return ERR_CMD_OR_ARGS_TOO_BIG;
                    strcat(clist->commands[cmd_count].args, " ");
                }
        
                if (strlen(clist->commands[cmd_count].args) + strlen(token) >= ARG_MAX)
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                strcat(clist->commands[cmd_count].args, token);
                token = strtok_r(NULL, " ", &saveptr2);
            }
            cmd_count++;
        }
        commandStr = strtok_r(NULL, PIPE_STRING, &saveptr1);
    }

    if (cmd_count == 0)
        return WARN_NO_CMDS;

    clist->num = cmd_count;
    return OK;
}
void print_dragon()
{
    const char *dragon = 
        "                                                                        @%%%%                       \n"
        "                                                                     %%%%%%                         \n"
        "                                                                    %%%%%%                          \n"
        "                                                                 % %%%%%%%           @              \n"
        "                                                                %%%%%%%%%%        %%%%%%%           \n"
        "                                       %%%%%%%  %%%%@         %%%%%%%%%%%%@    %%%%%%  @%%%%        \n"
        "                                  %%%%%%%%%%%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%%%%%%%%%%%%          \n"
        "                                %%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%% %%%%%%%%%%%%%%%           \n"
        "                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%     %%%            \n"
        "                             %%%%%%%%%%%%%%%%%%%%%%%%%%%%@ @%%%%%%%%%%%%%%%%%%        %%            \n"
        "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%                \n"
        "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              \n"
        "                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@              \n"
        "      %%%%%%%%@           %%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%      %%                \n"
        "    %%%%%%%%%%%%%         %%@%%%%%%%%%%%%           %%%%%%%%%%% %%%%%%%%%%%%      @%                \n"
        "  %%%%%%%%%%   %%%        %%%%%%%%%%%%%%            %%%%%%%%%%%%%%%%%%%%%%%%                        \n"
        " %%%%%%%%%       %         %%%%%%%%%%%%%             %%%%%%%%%%%%@%%%%%%%%%%%                       \n"
        "%%%%%%%%%@                % %%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%                     \n"
        "%%%%%%%%@                 %%@%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  \n"
        "%%%%%%%@                   %%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              \n"
        "%%%%%%%%%%                  %%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%      %%%%  \n"
        "%%%%%%%%%@                   @%%%%%%%%%%%%%%         %%%%%%%%%%%%@ %%%% %%%%%%%%%%%%%%%%%   %%%%%%%%\n"
        "%%%%%%%%%%                  %%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%% %%%%%%%%%\n"
        "%%%%%%%%%@%%@                %%%%%%%%%%%%%%%%@       %%%%%%%%%%%%%%     %%%%%%%%%%%%%%%%%%%%%%%%  %%\n"
        " %%%%%%%%%%                  % %%%%%%%%%%%%%%@        %%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%% %%\n"
        "  %%%%%%%%%%%%  @           %%%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%% \n"
        "   %%%%%%%%%%%%% %%  %  %@ %%%%%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%% \n"
        "    %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%           @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%%%%%% \n"
        "     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              %%%%%%%%%%%%%%%%%%%%%%%%%%%%        %%%   \n"
        "      @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  %%%%%%%%%%%%%%%%%%%%%%%%%               \n"
        "        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%%%%%%  %%%%%%%          \n"
        "           %%%%%%%%%%%%%%%%%%%%%%%%%%                           %%%%%%%%%%%%%%%  @%%%%%%%%%         \n"
        "              %%%%%%%%%%%%%%%%%%%%           @%@%                  @%%%%%%%%%%%%%%%%%%   %%%        \n"
        "                  %%%%%%%%%%%%%%%        %%%%%%%%%%                    %%%%%%%%%%%%%%%    %         \n"
        "                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%            \n"
        "                %%%%%%%%%%%%%%%%%%%%%%%%%%  %%%% %%%                      %%%%%%%%%%  %%%@          \n"
        "                     %%%%%%%%%%%%%%%%%%% %%%%%% %%                          %%%%%%%%%%%%%@          \n"
        "                                                                                 %%%%%%%@       \n";
    printf("%s\n", dragon);
}
