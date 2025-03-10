1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My shell calls the wait function in a loop after creating each child process. As we can see from line 199  the loop uses wait() to esnure each child process finish execution. If we forgot to call waitpid() or wait(), terminated child process would be remain as zombie process. This would result in system wasted resources and could eventually exhaust the available space and other resources.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

When we use dup2() to redirect input or output, it is critical to close the orignal unused pipe ends immediately afterward. if we leaves these unused pipe ends open, the process will have hard time to get the EOF and may throw invalid output.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is implemented as a built in  command and it should change the current directory depends on what actions we are performing with cd. External commands run in separate child processes; thus, calling cd externally wouldn't affect the shell’s directory.Built-in commands run within the shell process, allowing them to alter the shell's internal state directly.


4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I currently limit piped commands using a fixed-size array (commands[CMD_MAX]). To handle arbitrary command counts, I’d dynamically allocate memory (malloc()) for command arrays, resizing as needed.If I leave pipes open without proper closure, it could cause file descriptor leaks and resource exhaustion, eventually leading to system errors.
