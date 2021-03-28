// Isaiah Cabugos

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


#define READ_END 0
#define WRITE_END 1

// char* parse(char* s) {

// }


int main(int argc, const char * argv[]) {  
  char input[BUFSIZ];
  char last_command[BUFSIZ];
  //char *array;
  
  memset(input, 0, BUFSIZ * sizeof(char));
  memset(input, 0, BUFSIZ * sizeof(char));
  bool finished = false;

  pid_t pid, pPid;

  while (!finished) {
    printf("osh> ");
    fflush(stdout);

    if ((fgets(input, BUFSIZ, stdin)) == NULL) {   // or gets(input, BUFSIZ);
      fprintf(stderr, "no command entered\n");
      exit(1);
    }
    input[strlen(input) - 1] = '\0';          // wipe out newline at end of string
    printf("input was: \n'%s'\n", input);

    // check for history (!!) command
    if (strncmp(input, "!!", 2) == 0) {
      if (strlen(last_command) == 0) {
        fprintf(stderr, "no last command to execute\n");
      }
      else {
        printf("last command was: %s\n", last_command);
        strcpy(input, last_command);
        strcpy(last_command, input);
        printf("You entered: %s\n", input);   // you will call fork/exec
        //parse(input);

        // ========================= PARSING CODE =============================
        const char break_chars[] = " \t";
        char* p;
        int i = 0;
        char *array[10];
        bool amp = false;

        p = strtok(input, break_chars);
        while (p != NULL) {
        printf("token was: %s\n", p);
        array[i++] = p;
        p = strtok(NULL, break_chars);
        }
      
        i = 0;
        while (array[i] != NULL) {
          printf("%s\n", array[i]);
          amp = strncmp(array[i], "&", 1) == 0;
          i++;
        }
			  printf("after parse, what is input: %s\n", input);
        printf("\n");

        // ========================= PARSING CODE =============================

        pid = fork();

        if (pid < 0) { /* Error In Forking */
        fprintf(stderr, "Fork Failed");
        return 1;
        }
        else if (pid == 0) { /* Child Process */
          execvp(array[0], array);
        }

        else /* parent process */
        /* parent process waits for child to complete */
        if (amp == true) {
          printf("Parent not waiting.\n");
        } else {
          wait(NULL);
          printf("Child Complete\n");
        }
      }

    } else if (strncmp(input, "exit", 4) == 0) {   // only compare first 4 letters
      finished = true;
    } else {
      strcpy(last_command, input);
      printf("You entered: %s\n", input);   // you will call fork/exec
      //parse(input);

      // ========================= PARSING CODE =============================
      const char break_chars[] = " \t";
      char* p;
      char *array[10];
      char *pipeArray[10];
      int i = 0;
      int writeIndex = 0;
      int readIndex = 0;
      int fd = 0;
      int fdArray[2];
      bool amp = false;
      bool write = false;
      bool read = false;
      bool piped = false;

      // Clears arrays
      for(i = 0; i < 10; i++) {
        array[i] = NULL;
        pipeArray[i] = NULL;
      }
      i = 0;

      p = strtok(input, break_chars);
      while (p != NULL) {
      printf("token was: %s\n", p);
      array[i++] = p;
      p = strtok(NULL, break_chars);
      }
      
      i = 0;
      while (array[i] != NULL) {
        printf("%s\n", array[i]);
        if(strncmp(array[i], "&", 1) == 0) {
          amp = true;
        }

        if(strncmp(array[i], ">", 1) == 0) {
          write = true;
          writeIndex = i + 1;
        }

        if(strncmp(array[i], "<", 1) == 0) {
          read = true;
          readIndex = i + 1;
        }

        if(strncmp(array[i], "|", 1) == 0) {
          int j = i + 1;
          int k = 0;
          piped = true;
          array[i] = NULL;
          do {
            pipeArray[k] = array[j];
            array[j] = NULL;
            j++;
            k++;
          } while(array[j] != NULL);
        }

        i++;
      }
      printf("State of amp: %d\n", amp);
      printf("State of write: %d\n", write);
      printf("State of read: %d\n", read);
      printf("State of pipe: %d\n", piped);
        

			printf("after parse, what is input: %s\n", input);
        printf("after parse, what is array[0]: %s\n", array[0]);
        printf("after parse, what is array[1]: %s\n", array[1]);
        printf("after parse, what is array[2]: %s\n", array[2]);
        printf("after parse, what is array[2]: %s\n", array[3]);
          printf("after parse, what is pipeArray[0]: %s\n", pipeArray[0]);
          printf("after parse, what is pipeArray[1]: %s\n", pipeArray[1]);
          printf("after parse, what is pipeArray[2]: %s\n", pipeArray[2]);
      printf("\n");

      // ========================= PARSING CODE =============================      

      pid = fork();

      if (pid < 0) { /* Error In Forking */
        fprintf(stderr, "Fork Failed");
        return 1;
      }
      else if (pid == 0) { /* Child Process */
      //printf("Second State of write: %d\n", write);

      
        if (piped) {
          if(pipe(fdArray) == -1) { /* Creates Pipe */
            fprintf(stderr,"Pipe Failed.\n");
          }
      /* ===== Second Child Process ===== */
          pPid = fork(); // Second child forked

          if (pPid < 0) { /* Error In Forking */
            fprintf(stderr, "Fork Failed");
            return 1;
          }

          else if (pPid == 0) { /* Child Process */
            close(fdArray[READ_END]);
            dup2(fdArray[WRITE_END], STDOUT_FILENO);
            close(fdArray[WRITE_END]);


            if(execvp(pipeArray[0], pipeArray) == -1){
              printf("Error: Typo\n");
              break;
            }
          }

          else { /* Parent Process */


            close(fdArray[WRITE_END]);
            dup2(fdArray[READ_END], STDIN_FILENO);
            close(fdArray[READ_END]);

            wait(NULL);
            printf("Child's Child Complete\n");
          }  
        }
      /* ===== Second Child Process ===== */

        if (write) {
          //fprintf(stderr, "File Name: %s\n", array[writeIndex]);
          array[writeIndex-1] = NULL;
          fd = open(array[writeIndex], O_CREAT|O_WRONLY, 0666);
          dup2(fd, STDOUT_FILENO);
          write = false;
        }

        if (read) {
          array[readIndex-1] = NULL;
          fd = open(array[readIndex], O_RDONLY);
          read = false;
          if (fd == -1){
            fprintf(stderr, "Error Opening File.\n");
            break;
          } else {
            dup2(fd, STDIN_FILENO);
          }
        }
            
        if(execvp(array[0], array) == -1){
          printf("Error: Typo\n");
          break;
        }
          
      } 
      else /* parent process */
      /* parent process waits for child to complete */
      if (amp == true) {
        printf("Parent not waiting.\n");
      } else {
        wait(NULL);
        printf("Child Complete\n");
      }
    }
  }
  
  printf("osh exited\n");
  printf("program finished\n");
  
  return 0;
}