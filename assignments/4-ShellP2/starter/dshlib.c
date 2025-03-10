#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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


int exec_local_cmd_loop()
{
   char cmd_buff[SH_CMD_MAX + 1];
   cmd_buff_t cmd;
   memset(&cmd, 0,sizeof(cmd));
   int  last_flag = 0;


   while (1)
   {
       memset(&cmd,0,sizeof(cmd));
       printf("%s", SH_PROMPT);


       if (fgets(cmd_buff, sizeof(cmd_buff), stdin) == NULL)
       {
           printf("\n");
           break;
       }


       cmd_buff[strcspn(cmd_buff, "\n")] = '\0';


       int result = build_cmd_buff(cmd_buff, &cmd);
       if (result == WARN_NO_CMDS)
       {
           printf("%s", CMD_WARN_NO_CMD);
           continue;
       }
       else if (result == ERR_MEMORY)
       {
           printf("error: memory allocation failed\n");
           continue;
       }
       else if (result == ERR_CMD_OR_ARGS_TOO_BIG)
       {
           printf("error: command or arguments too big\n");
           continue;
       }


       Built_In_Cmds cmd_type = match_command(cmd.argv[0]);


       if (cmd_type != BI_NOT_BI)
       {
           switch (cmd_type)
           {
           case BI_CMD_EXIT:
               free_cmd_buff(&cmd);
               exit(0);


           case BI_CMD_CD:
               if (cmd.argc > 1)
               {
                   if (chdir(cmd.argv[1]) != 0)
                   {
                       perror("cd");
                   }
               }
               break;


           case BI_RC:
               printf("%d\n", last_flag);
               break;


           default:
               break;
           }


           free_cmd_buff(&cmd);
           continue;
       }
       pid_t pid = fork();
       if (pid == 0)
       {
           execvp(cmd.argv[0], cmd.argv);
           perror("execvp failed");
           exit(EXIT_FAILURE);
       }
       else if (pid > 0)
       {
           int status;
           if (waitpid(pid, &status, 0) == -1)
           {
               perror("waitpid failed");
               last_flag = -1;
           }
           else
           {
               last_flag = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
           }
       }
       else
       {
           perror("fork failed");
           continue;
       }


       free_cmd_buff(&cmd);
   }


   return OK;
}




Built_In_Cmds match_command(const char *input) {
   typedef struct {
       const char *cmd;
       Built_In_Cmds code;
   } cmd_map_t;


   const cmd_map_t command_map[] = {
       {EXIT_CMD, BI_CMD_EXIT},
       {"cd",   BI_CMD_CD},
       {"rc",   BI_RC}
   };


   size_t num_commands = sizeof(command_map) / sizeof(command_map[0]);
   for (size_t i = 0; i < num_commands; i++) {
       if (strcmp(input, command_map[i].cmd) == 0) {
           return command_map[i].code;
       }
   }
   return BI_NOT_BI;
}


int last_flag = 0;
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
   Built_In_Cmds cmd_type = match_command(cmd->argv[0]);


   switch (cmd_type) {
       case BI_CMD_EXIT:
           free_cmd_buff(cmd);
           exit(0);
           break;
       case BI_CMD_CD:
           if (cmd->argc > 1) {
               if (chdir(cmd->argv[1]) != 0) {
                   perror("cd");
               }
           }
           break;


       case BI_RC:
           printf("%d\n", last_flag);
           break;


       default:
           break;
   }


   return cmd_type;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
   clear_cmd_buff(cmd_buff);


   if (alloc_cmd_buff(cmd_buff) != OK)
   {
       return ERR_MEMORY;
   }


   char *str_ptr = cmd_line;
   char *token = NULL;
   bool quotes = false;
   int argc = 0;


   while (*str_ptr != '\0')
   {
       while (*str_ptr == SPACE_CHAR && !quotes)
       {
           str_ptr++;
       }


       if (*str_ptr == '\0')
       {
           break;
       }


       if (*str_ptr == '"')
       {
           quotes = true;
           str_ptr++;
       }


       token = str_ptr;


       while (*str_ptr && ((quotes && *str_ptr != '"') || (!quotes && *str_ptr != SPACE_CHAR)))
       {
           str_ptr++;
       }


       if (*str_ptr != '\0')
       {
           *str_ptr = '\0';
           str_ptr++;
       }


       if (quotes)
       {
           quotes = false;
       }


       if (argc < CMD_ARGV_MAX - 1)
       {
           cmd_buff->argv[argc] = strdup(token);


           if (cmd_buff->argv[argc] == NULL)
           {
               return ERR_MEMORY;
           }


           argc++;
       }
       else
       {
           return ERR_CMD_OR_ARGS_TOO_BIG;
       }
   }


   cmd_buff->argv[argc] = NULL;
   cmd_buff->argc = argc;


   return (argc > 0) ? OK : WARN_NO_CMDS;
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
