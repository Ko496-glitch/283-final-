1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?


   > **Answer**:  using fork() always create a child process and then we do all the operation there but if we just call excevp it will replace the shell process with the commands that we input.


2. What happens if the fork() system call fails? How does your implementation handle this scenario?


   > **Answer**:  if fork() system call fails we will never create seperate process that is child p rocess and it will return -1. Now if we look our code from line 132 this is where the implementation starts of fork() and so in that part of code what we are doing is that in line 133 we are creating the process and then in subsequent  line 139 process we are  waiting for the child process to finish if otherwise it will throw an error.


3. How does execvp() find the command to execute? What system environment variable plays a role in this process?


   > **Answer**:  so execvp finds the command by actually iterating through the directories in PATH Env variable. Now if not find it wil throw an error but main idea is that if the given command starts with /  excecvp() will use the given path directly.


4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?


   > **Answer**: the wait() call in the parent process actually helps in preventing the zombie process by cleaning up terminated child processes and freeing the memory and other resourcess used. This is really important becasue if not done we could see a high memory usage and other resource to be used in a wrong way.


5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?


   > **Answer**:  WEXITSTATUS() extracts the exit status of a child process that terminated normally using wait() or waitpid(). It returns the low-order 8 bits of the child's exit code, indicating success or failure. This allows the shell's rc command to display the correct exit status of the last executed command.


6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?


   > **Answer**: My build_cmd_buff() handles quoted arguments by using a boolean variable quotes that is set to true when a double quote (") is encountered. While quotes is true, spaces are treated as part of the argument until the closing quote is found. This allows commands like echo "hello, world" to be correctly parsed as a single argument. Handling quoted arguments is essential for preserving user input as intended and ensuring compatibility with shell commands that expect multi-word strings.


7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?


   > **Answer**:  The main challenges during refactoring were implementing custom parsing to handle spaces within quoted arguments, which strtok_r() could not achieve. Removing pipe support required restructuring the logic while ensuring single-command parsing remained accurate. Additionally, managing dynamic memory allocation using strdup() introduced the need to prevent memory leaks.


8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.


- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?


   > **Answer**:  Signals in a Linux system are asynchronous notifications we use to indicate events like termination, interruption, or errors. Unlike other IPC methods like pipes or shared memory, signals don’t need a communication channel and are delivered instantly. They’re lightweight, transmit only a signal number, and can interrupt a process anytime, making them perfect for time-sensitive tasks.




- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?


   > **Answer**: SIGINT:: a) Signal Number 2
   b) Default Behavior: Terminates the process, but processes can catch and handle this signal.
   c) Typical Use Cases:
    Interrupting a running command or program gracefully.
    Stopping processes interactively from the terminal.

2) SIGTERM:: a) Signal Number 15
b) Default Behavior: Graceful termination; allows the process to perform cleanup (save data, release resources) before exiting. Processes can catch, handle, or ignore it.
c) Typical Use Cases:
Safely requesting a process to terminate itself.
Stopping services and daemons gracefully without abrupt shutdown.

3)  SIGKILL (Kill Signal)
a) Signal Number 9
b) Default Behavior: Immediately and forcefully terminates the process. Cannot be caught, blocked, or ignored.
c) Typical Use Cases:
    Forcefully killing unresponsive, stuck, or runaway processes that do not respond to SIGTERM or other signals.
    Ensuring immediate termination without the possibility of cleanup.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?


   > **Answer**: When a process receives SIGSTOP, it is immediately paused and stops executing until resumed with SIGCONT. Unlike SIGINT, which we can catch or ignore, SIGSTOP cannot be caught, blocked, or ignored because it's designed to unconditionally suspend a process. This ensures system administrators can always pause a process, regardless of its behavior.