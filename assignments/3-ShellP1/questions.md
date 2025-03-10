1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice because it safely reads input line by line, preventing buffer overflows by limiting the number of characters read. Additionally, it handles EOF and newline termination, making it ideal for interactive shell input processing.


2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

      > **Answer**:  Using malloc() for cmd_buff allows dynamic memory allocation, ensuring flexibility in handling varying input sizes and avoiding stack overflows for large inputs. A fixed-size array could limit input handling and waste memory if the buffer is too large.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing spaces ensures that commands and arguments are stored cleanly, preventing issues like empty or malformed command names. Without trimming, extra spaces could cause execution errors, misinterpretation of commands, or failed lookups for executable paths.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Output Redirection (> and >>):  Redirects command output to a file instead of displaying it in the terminal. For example, ls > output.txt → Writes the output of ls to output.txt (overwrites if exists). echo "Hello" >> log.txt → Appends "Hello" to log.txt instead of overwriting.


    Challenges:
a) Handling file opening/closing correctly (O_CREAT | O_WRONLY | O_TRUNC for > and O_APPEND for >>).
b) Ensuring permissions allow writing to the file.
c) Managing errors if the file cannot be created or accessed.

2) Input Redirection (<): Feeds input to a command from a file instead of the keyboard.For example, sort < data.txt → Reads input from data.txt and sorts it. 

Challenges: 
a)Opening the file in read mode (O_RDONLY).
b)Redirecting STDIN properly so the command reads from the file.
c)Handling errors if the file does not exist or lacks read permissions. 

3) Error Redirection (2> and 2>&1): Redirects error messages (STDERR) to a file or merges them with standard output (STDOUT).For example, grep "text" file.txt 2> errors.log → Saves error messages (if any) to errors.log.

Challenges
a) Detecting and Parsing 2> in User Input
b) Handling File Operations
c) Implementing 2>&1 (Merging STDERR into STDOUT) 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is used to transfer input or output between a command and a file, while piping is used to pass the output of one command directly as input to another command.
                    Redirection modifies where a command reads or writes data, typically using files, whereas piping connects commands without involving files.
                    Redirection is implemented using file operations like opening, closing, and modifying file descriptors, while piping requires creating a pipe, forking processes, and redirecting input/output using system calls.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: Separation of concerns: Keeping STDERR (error messages) separate from STDOUT (regular output) allows programs to distinguish between  normal output and errors, making it easier to handle failures without interfering with expected results.
    When writing shell scripts, having separate STDOUT and STDERR allows scripts to capture and log errors separately while continuing to process normal output correctly.


- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  The shell should capture and display STDERR separately from STDOUT to clearly indicate failures. This ensures that error messages do not mix with valid output, making debugging easier
.                 The shell should not crash when a command fails but instead display an informative error message and allow the user to continue.