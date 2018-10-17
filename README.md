# Shell.c
Shell in C that can execute executables, commands, pipelined commands, and specific built-in functions like history,
changing directories, exiting the shell, executing a past commands.
The built-ins it can follow include:
exit: It exits the shell
history: It lists up to the first 100 commands that were typed and entered by the user. After the first hundred commands, the shell will
replace the oldest command with the newest one.
history -c: It clears the commands listed by history. If history is called after this command. "history" will be the only reported command
history [offset]: The user can exececute a command that the history built in lists. If the user types hundred commands and in the first command
they typed execute the pwd command, and they type the built in history 0. The shell will print the working directory. the offset specfies
the index the command is located in the array that stores the history of the users typed commands and executables.
cd [directory]: It changes directories like the Linux cd command does. Be aware of the current directory the shell currently is in.

The shell has some built in error detection like when the user types in invalid executables and commands or the shell fails to change
to another different directory

# Prerequisites
This program can only run on Linux. I ran program in Linux that was installed into a virtual machine.

# Running
Type: "gcc mysh.c -o main" to compile the file. Do not include the quotatation marks. "main" in the command could be any name you want but it is the name of the file you will execute. Type "./main without the quotation marks to execute. After this command you can then list the files or Linux commands to execute. 

