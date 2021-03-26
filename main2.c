// Isaiah Cabugos

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


// char* parse(char* s) {

// }


int main(int argc, const char * argv[]) {  
  char input[BUFSIZ];
  char last_command[BUFSIZ];
  //char *array;
  
  memset(input, 0, BUFSIZ * sizeof(char));
  memset(input, 0, BUFSIZ * sizeof(char));
  bool finished = false;

  pid_t pid;

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
      int i = 0;
      int writeIndex = 0;
      int readIndex = 0;
      bool amp = false;
      bool write = false;
      bool read = false;

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
        write = strncmp(array[i], "<", 1) == 0;
        if (write) {
          writeIndex = i + 1;
        }
        read = strncmp(array[i], ">", 1) == 0;
        if (read) {
          readIndex = i + 1;
        }
        i++;
      }
      //printf("State of amp: %d\n", amp);
        

			printf("after parse, what is input: %s\n", input);
            // printf("after parse, what is array[0]: %s\n", array[0]);
            // printf("after parse, what is array[1]: %s\n", array[1]);
            // printf("after parse, what is array[2]: %s\n", array[2]);
      printf("\n");

      // ========================= PARSING CODE =============================

      pid = fork();

      if (pid < 0) { /* Error In Forking */
      fprintf(stderr, "Fork Failed");
      return 1;
      }
      else if (pid == 0) { /* Child Process */
          if(write == false && read == false) {
            execvp(array[0], array);
          }
      } else if (write == true) {
            dup2(1, STDOUT_FILENO);
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