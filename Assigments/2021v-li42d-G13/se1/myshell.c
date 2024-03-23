#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE 64
#define DELIM " \t"

//Commands function declarations
int cmd_help(char **args);
int cmd_exit(char **args);//extra options
int cmd_ls(char **args);
int cmd_cat_txt(char **args);
int cmd_cat_txt_copy(char **args);
int cmd_clear(char **args);
int cmd_execute(char **args, int j);
/*int cmd_cat_txt_stdin(char **args);
int cmd_cat_txt_stdin_redi(char **args);*/
int cmd_launch(char **args);

//List of commands, followed by their corresponding functions.
char *cmd_str[] = {
  "help",
  "exit",
  "ls",
  "clear",
  "cat",
  ">",
};

int (*cmd_func[]) (char **) = { //it as to be on the same order as cmd_str
  &cmd_help,
  &cmd_exit,
  &cmd_ls,
  &cmd_clear,
  &cmd_cat_txt,
  &cmd_cat_txt_copy,
};

int cmd_num() {
  return sizeof(cmd_str) / sizeof(char *);
}

/**
   Command example: help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int cmd_help(char **args){
  int i;
  printf("This is the existing commands\n");
  for (i = 0; i < cmd_num(); i++) {
    printf("  %s\n", cmd_str[i]);
  }
  return 1;
}

/**
   Command example: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int cmd_exit(char **args){
  return 0;
}

/**
   Command example: ls.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue execution.
 */
int cmd_ls(char **args){
	return cmd_launch(args);
}

/**
   Command example: cat abc.txt, cat abc.txt | grep abc, cat abc.txt | grep abc | sort > copia.txt.
   @param args List of args. args[0] is "cat".  args[1] txt file.
   @param args List of args. args[0] is "cat". args[1] txt file. args[2] "|". args[3] standard input.
   @param args List of args.  args[0] is "cat". args[1] txt file. args[2] "|". args[3] standard input. args[4] "|". args[5] standard input. args[6] ">". args[7] txt file.
   @return Always returns 1, to continue execution.
 */
/*
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
int cfileexists(const char * filename){
    /* try to open file to read */
    FILE *file;
    if (filename!=NULL&&(file = fopen(filename, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
}
int cmd_cat_txt(char **args){
	if(cfileexists(args[1])){// cat abc.txt
		if(args[2]!=NULL){
			if (strcmp(args[2], ">") == 0&&args[4]==NULL) {// cat(0) abc.txt(1) >(2) copia.txt(3) 
				if(cfileexists(args[3])){
					cmd_execute(args,2);
					return 1;
				}
				printf("Second file doesnt exist\n");
				return 1;
				}
			else if(strcmp(args[2],"|")==0&&strcmp(args[3],"grep")==0){// cat(0) abc.txt(1) |(2) grep(3) abcISEL(4)	Need improve because he finish the program when executed
				if(args[5]==NULL){
					int pipefd[2];
					char *cat_args[] = {"cat",args[1], NULL};
					char *grep_args[] = {args[3], args[4], NULL};
					//pipe(pipefd);
					if(pipe(pipefd) == -1) {
						perror("Pipe failed");
						exit(1);
					}
					pid_t pid = fork();
					if (pid == 0) {// Child process
						close(STDIN_FILENO);
						dup2(pipefd[0], STDIN_FILENO);
						close(pipefd[0]);//closing pipe read
						close(pipefd[1]);
						if (execvp(args[3], grep_args) == -1) { //int execvp (const char *filename, char *const argv[])
							perror("ERROR EXECVP");
						}
						perror("execvp failed");
						exit(1);//FAIL
					} 
					else if (pid < 0) {// Error forking
						perror("ERROR FORKING");
					}
					else {
						close(STDOUT_FILENO);
						dup2(pipefd[1], STDOUT_FILENO );
						close(pipefd[0]);
						close(pipefd[1]);
						if (execvp(args[0], cat_args) == -1) { 
							perror("ERROR EXECVP");
						}
					} 
					close(pipefd[0]);
					close(pipefd[1]);
					wait(0);
					wait(0);
					return 1;
				}
				else if(strcmp(args[5],"|")==0&&strcmp(args[6],"sort")==0&&strcmp(args[7],">")==0&&cfileexists(args[8])){// cat(0) abc.txt(1) |(2) grep(3) Gru(4) |(5) sort(6) >(7) saida.txt(8) Need improves because he copy all the content of the file and do not sort
					char buff[1024], childbuff[1024];
					int fd[2], source, bytes, dest;
					pipe(fd);
					pid_t child = fork();
					if(child==0) {//Child process
						read(fd[0], childbuff, sizeof(childbuff));//reads from the pipe
						close(fd[0]);
						bytes = strlen(childbuff);
						dest = open(args[8], O_WRONLY);//writes in the file
						write(dest, childbuff, bytes);
					} 
					else {// parent process
						source = open(args[1], O_RDONLY);//reads the file
						bytes = read(source, buff, sizeof(buff));
						write(fd[1], buff, bytes);//puts data in pipe
						close(fd[1]);
				  }
					return 1;
					}// cat abc.txt | grep Gru | sort > copy.txt
					perror("Wrong command");
				}
			
			}
		else return cmd_launch(args);
		}
	printf("First file doesnt exist\n");
	return 1;
}

int cmd_clear(char **args){
	return cmd_launch(args);
}


int cmd_cat_txt_copy(char **args){
		char buff[256], childbuff[256];
		int fd[2], source, bytes, dest;
		pipe(fd);
		pid_t child = fork();
		if(child==0) {//Child process
			read(fd[0], childbuff, sizeof(childbuff));//reads from the pipe
			close(fd[0]);
			bytes = strlen(childbuff);
			printf("\n bytes child %d",bytes);
			char* file2 =args[3];
			printf("\n file2 %s",file2);
			dest = open(file2, O_WRONLY);//writes in the file
			write(dest, childbuff, bytes);
			}else if (child < 0) {// Error forking
				perror("ERROR FORKING");
			}
		else {// parent process
			char* file1=args[1];
			printf("\n file1 %s",file1);
			source = open(file1, O_RDONLY);//reads the file
			bytes = read(source, buff, sizeof(buff));
			printf("\n bytes %d",bytes);
			write(fd[1], buff, bytes);//puts data in pipe
			close(fd[1]);
			}
		return 1;
}



int cmd_launch(char **args){
	pid_t pid = fork();
	int res;
	if (pid == 0) {// Child process
		if (execvp(args[0], args) == -1) { //int execvp (const char *filename, char *const argv[])
			perror("ERROR EXECVP");
		}
		exit(1);//FAIL
		printf("SHOULD NEVER GET HERE!");
	} 
	else if (pid < 0) {// Error forking
		perror("ERROR FORKING");
	}
	else {
		do {
			waitpid(pid, &res, 0);
		} while (!WIFEXITED(res) && !WIFSIGNALED(res));
	}
  return 1;
}

int cmd_execute(char **args, int j){
  int i;
  if (args[j] == NULL) {// User did not write
    return 1;
  }
  for (i = 0; i < cmd_num(); i++) {
    if (strcmp(args[j], cmd_str[i]) == 0) {
		//printf("Tou execute %s %s %d\n",cmd_str[i],args[j],i);
		return (*cmd_func[i])(args); //call upper functions
    }
  }
  return 1;//cmd_launch(args); //launch the program
}

char *readLine(void){
  int bufsize = BUFSIZE;
  int pos = 0;
  char *buf = malloc(sizeof(char) * bufsize);
  int c;
  while (1) {
    c = getchar();
    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {// Do enter
      buf[pos] = '\0';
      return buf;
    } else {// User is writting
      buf[pos] = c;
    }
    pos++;
  }
}

char **splitLine(char *line)
{
  int bufsize = BUFSIZE, pos = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;
  if (!tokens) {
    printf("$ ERROR MESSAGE: allocation error\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(line, DELIM);// cat abc.txt > text.txt
  while (token != NULL) {
    tokens[pos] = token;
    pos++;
    token = strtok(NULL, DELIM);
  }
  tokens[pos] = NULL;
  return tokens;
}

void mainshell(void)
{
	char *line;
	char **args;
	int status;
	do {
		printf("$ ");	
		line = readLine();
		args = splitLine(line);
		status = cmd_execute(args,0);
		free(line);
		free(args);
	} while (status);
}

int main(int argc, char **argv)
{
  mainshell();
  return EXIT_SUCCESS;
}