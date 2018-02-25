// Author: Amine Benelbar

// Program Name: mysch.c

// Shell creates child process that executes the command you entered and prompt for more until it is finished
// Note: At most an input of 300 characters can be parsed and it can only be parsed for at most 30 words
// Note: It can only pipe at most 30 commands with each at most 30 words in them. They can parse more than that
// if the size they were initially initialized with is set to a higher value. EX:
// line[600] instead of line[300], pipedCommands[90] instead of 30. pipedWords[40] instead of 30.
// The constant in them has to change to a higher value.
#include <memory.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
// parsing input 

int isNumeric (const char * s);
int main()
{
   const int  MAX = 3;
   const int  SIZE = 300;
   pid_t pid;
   int i = 0; // counter for words
   int sentinel = 1; // Used to help exit the program
   char delim[] = " \n";
   int counter = 0; // Used for assigning strings to hist 
   int j = 0;
   int count100 = 0; // Counts strings stored in history
   //char * clearStr = NULL; // String used to help clear the string
   //char filename[256] = "/home/013/a/ax/axb157030/HelloShell.exe";
  //char fileArgs[256] = ""; // contains passed in arguments of file or descriptors or arguments of a commmand
  char line[300] = "/home/013/a/ax/axb157030/HelloShell.exe";
  char line2[300];
  char* args[30] = {NULL, NULL, NULL}; // Contains the individual words. Why does this result in error if it's a constant variable size. It can only parse 30 words 
  char Exit[5] = "exit";
  char History[8] = "history";
  char hist[100][300];
  char * clearStr = "";
  char * Clear = "-c";
  int offset; //
  char * lineOffset = NULL; // Line for history offset
  char * CD = "cd";
  char * pipeChar = "|";
   
   while(sentinel) 
   {
      i = 0;
      while(i < 30)
      {
         args[i] = NULL; // Setting args back to null to avoid unforseen errors
         i++; // i acts like counter in this loop not as the counter for the amount of words
      }
      i = 0;
      
      printf("$");
      if(lineOffset == NULL)
      {
      
         fgets(line, SIZE, stdin); 
         strcpy(line2,line);
         strcpy(hist[counter],line); // program takes 3 or less words from the line regardless how many words it has but the history keyword will bring all the lines the user has typed down
   
         counter++;
         
         if(count100 < 100)
         {
            count100++;
         }
         if(counter == 100)
         {
            counter = 0; // Once more than 100 commands are executed, remove the oldest entry from the list to make room for
                         // the newer commands
         }
         printf(line);
         if(line[0] == '\n')
         {
            continue;
         }
         
         printf("\n");
         args[i] = strtok(line, delim); // Where line is parsed into words
          i++;
         while((i < 30) )
         {        
            args[i] = strtok(NULL, delim);
            if(args[i] == NULL)
            {
               break;
            }
            i++;
         }
       }
       
       else
       {
          if(lineOffset[0] == '\n')
         {
            continue;
         }
          i = 0; // this statement doesn't matter
          strcpy(line2,lineOffset);
          args[i] = strtok(lineOffset, delim); // What if line equals "\n"?
          i++;
         while((i < 30) )
         {        
            args[i] = strtok(NULL, delim);
            if(args[i] == NULL) // String has no tokens
            {
               break;
            }
            i++;
         }
         
           lineOffset = NULL;
         
       }
      
          if( strcmp(args[0],Exit) == 0 ) // This handles the program's shell's exit function
         {
            // As long as the first word = "exit" the system will terminate
            printf("Shell is exiting\n");
            sentinel = 0; // gets out of the loop that keeps prompting users for a command
            exit(0);
         }
         
         //cd [dir] - Short for "change directory", and will be used to 
         // change the current working directory of your shell
         else if( (i == 2) && (strcmp(args[0],CD) == 0) )
         {
         printf("directory: %s \n", args[1]);
            if(chdir(args[1]) < 0)
            {
               printf("ERROR. Current directory remained unchanged\n");
               
            }
         }
         
        else if( strcmp(args[0],History) == 0 )
        {
            if(i == 1) // Display history of commands
            {
               j = 0;
               while(j < count100)
               {
                  printf("%d",j);
                  printf(") ");
                  printf(hist[j]);
                  j++;
               }
               
               j = 0;
            }
            
            else if((strcmp(args[1],Clear) == 0)) // Clears history of commands
            {
               j = 0;
               while(j < count100) // Clears history. As long as the second word equals "-c" History will be cleared
               {
                  strcpy(hist[j],clearStr);
                  j++;
               }
               
               j = 0;
               count100 = 0;
               counter = 0;
            }
            
            // The offset is the index of the command in the list, and valid values are 0 to 99, inclusive. 0 is the oldest command
             else if(isNumeric(args[1])) // Gets an offset and execute a command in the history indexed by that offset
             {
                offset = atoi(args[1]);
                if(0 <= offset && offset < count100)
                  {
                    lineOffset = (hist[offset]); // executes the command in history at the given offset. Make it equal to lineOffset. If lineOffset is not NULL. lineOffset will be executed After it is executed it will equal Null. chdir                    
                  }
                  
                   else
                   {
                        printf("ERROR. Offset is not valid \n");
                   }
               }
            continue;
        }
      
      // fork child process
      // Used to store two ends of pipe. Want to make pipe here before first child because it is in a while loop. Want pipe to be done indefinitely
      
   fflush(stdout); // To take standard output passed into the child
      pid = fork();
 
      if(pid < 0)
      {
         fprintf(stderr, "Fork Failed");
         return 1;
      }
      
      else if (pid == 0)
      {
        // We are in child proccess.
         pid_t cpid;
         char * pipedCommand[30] = {NULL, NULL}; // Contains the commands in between, before and maybe pipes. It contains the pipe commands
         char * pipedWords[30] = {NULL, NULL};// Contains words of an individual piped command
         
    // https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/


         i = 0; // This time I will count the amount of words in line. "|" Can't be identified if not spaced with each other. wc|ls does not work unlike wc | ls
         j = 0; // Count amount of pipes
         counter = 0; // Count number of words before or after "|" to concatenate to know the piped command
         int oldfd[2];
         int newfd[2];
         
         
         while(i < 30)
         {
            pipedCommand[i] = NULL; // Setting pipedCommand back to null to avoid unforseen errors. Strtok may not stop becuase of existing strings in pipe command
            i++; // i acts like counter in this loop not as the counter for the amount of words
         }
         i = 0;
         
        // Getting the piped commands from line
        
        pipedCommand[j] = strtok(line2, pipeChar);
        //printf("pipeComm = %s\n", pipedCommand[j]);
        if(pipedCommand[j] != NULL)
        {
            //printf("pipeComm = %s\n", pipedCommand[j]);
            j++;
        }    
        while(pipedCommand[j-1] != NULL)
        {
            //printf("%i",i);
            pipedCommand[j] = strtok(NULL, pipeChar);
            printf("%i\n", j);
             //printf("pipeComm = %s\n", pipedCommand[j-1]);
            j++;
        }
         j -= 1; // To make j give amount of commands piped.  If command has no pipe j is 1
         
                             
                  
         counter = 0; // Now counter for number of commands
         
         // Executing piped commands
         while( (j > 1)&& (counter < j) )
         {


            if (pipe(newfd) < 0) // Creating pipe
            {
                fprintf(stderr, "Pipe # Failed" );
                return 1;
            }
                               
                //printf("pipeComm = %s\n", pipedCommand[j]);
                cpid = fork(); // Forking. Copying everything from parent to child
               if(cpid < 0)
               {
                  fprintf(stderr, "Fork # Failed"); // Any fork could fail
                  return 1;
               }
                
              
               else if((cpid == 0))
               {
                  // This is the next child that was forked.
                  close(newfd[1]);  // Close writing end of first pipe
                  dup2(newfd[0],0); // Operate as a read. If fd[1] close fd[0] operate as a read
                  close(newfd[0]);  // ) is different from newfd[0]
                  //wait(NULL); // Chil                           
                                 
               }
               
                
                else
                {
                  // The first child that was forked
                    close(newfd[0]);  // Close reading end of first pipe
                    if((counter + 1) < j)
                    {
                        dup2(newfd[1],1); // whenever printf data in printf write through pipe. Last token do not mess with output
                    }   // standard output of exec will go through pipe instead
                             i=0; // Count amount of words in piped command
                             while(i < 30)
                             {
                                 pipedWords[i] = NULL; // strtok seems to break strings up
                                 i++;
                             }
                             i=0;
                             pipedWords[i] = strtok(pipedCommand[counter], delim); // Where line is parsed into words
                            i++;
                           while((i < 30) )
                           {        
                              pipedWords[i] = strtok(NULL, delim);
                              if(pipedWords[i] == NULL)
                              {
                                 break;
                              }
                              i++;
                           }    
                    if(execvp(pipedWords[0],pipedWords) < 0) // next parameter is address of string
                   {
                     printf("%i\n",counter); 
                     printf("ERROR: 22 exec failed. The file or command may not exist or permission of accessing file may be denied\n");
                   }                                               
                  close(newfd[1]); // Close writing end of first pipe
                  //wait(NULL);
                   //oldfd[0] = newfd[0];
                   //oldfd[1] = newfd[1];
                }
            counter++;
         }     
         // Executing unpiped commands
               if(j <= 1)
               {
                   if(execvp(args[0], args) < 0)
                      {
                        printf("ERROR: exec failed. The file or command may not exist or permission of accessing file may be denied\n");
                      }         
               }
         exit(0); // Make child exit
      }
      
       // Parent process
      wait(NULL); // wait for child to terminate first
      printf("Command complete\n");
    }  
}

int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
    // https://rosettacode.org/wiki/Determine_if_a_string_is_numeric#C
}