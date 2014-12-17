#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <spawn.h>
#include <unistd.h>
#include <string.h>


//
// TODO
//   * Save child processes in a data structure, must have the pid, and how it was started
//   * Add function to check integrity of saved child processes
//


//
// Utility Functions
//
unsigned char startsWith(const char* str, const char* cmp)
{
  while(1) {
    char strC;
    char cmpC = *cmp;
    if(cmpC == '\0') return 1;
    strC = *str;
    if(strC == '\0' || strC != cmpC) return 0;
    str++;cmp++;
  }
}

char* skipWhitespace(char* s)
{
  while(1) {
    char c = *s;
    if(c == '\0') return s;
    if(c != ' ' && c != '\t' && c != '\n') return s;
    s++;
  }
}

const char** globalEnvp;

pthread_mutex_t childProcessesMutex;
pthread_cond_t childProcessesCond;

unsigned char debug;
unsigned char globalExit;

void signalManagerThread()
{
  pthread_mutex_lock( &childProcessesMutex );
  pthread_cond_signal( &childProcessesCond );
  pthread_mutex_unlock( &childProcessesMutex );
}

void manager()
{
  pid_t managerPid = getpid();

  int status;

  printf("[manager %d] start\n", managerPid);
  
  while(1) {

    if(globalExit) break;

    //
    // Wait for child processes to be created
    //
    pthread_mutex_lock( &childProcessesMutex );

    // TODO: Make sure there are no child processes (in case they were
    //       created between the waitpid loop and now)

    if(debug) printf("[manager %d] [debug] waiting for child processes...\n", managerPid);
    pthread_cond_wait( &childProcessesCond, &childProcessesMutex );
    pthread_mutex_unlock( &childProcessesMutex );


    while(1) {

      if(debug) printf("[manager %d] [debug] waitpid\n", managerPid);

      pid_t pid = waitpid(WAIT_ANY, &status, 0);

      if(debug) printf("[manager %d] [debug] waitpid returned %d\n", managerPid, pid);

      if(pid == -1) {
	if(errno == ECHILD) {
	  // no child processes
	  break;
	}
	printf("[manager %d] waitpid failed (errno %d)\n", managerPid, errno);
	return;
      }

      if (WIFEXITED(status)) {
	printf("[manager %d] pid %d exited with code %d\n",
	       managerPid, pid, WEXITSTATUS(status));
	/*
	  } else if (WIFSIGNALED(status)) {
	  printf("killed by signal %d\n", WTERMSIG(status));
	  } else if (WIFSTOPPED(status)) {
	  printf("stopped by signal %d\n", WSTOPSIG(status));
	  } else if (WIFCONTINUED(status)) {
	  printf("continued\n");
	  }
	*/
      } else {
	printf("[manager %d] pid %d changed somehow\n", managerPid, pid);
      }
    }

  }

  printf("[manager %d] finish\n", managerPid);
}


#define EXEC_COMMAND "exec"
#define EXEC_COMMAND_LENGTH (sizeof(EXEC_COMMAND) - 1)

#define SYSTEM_COMMAND "system"
#define SYSTEM_COMMAND_LENGTH (sizeof(SYSTEM_COMMAND) - 1)


void* uiThread(void* ptr)
{
  pid_t uiPid = getpid();

  printf("[ui %d] start\n", uiPid);

  char buffer[100];
  while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
    size_t stringLength = strlen(buffer);
    while(stringLength > 0) {
      stringLength--;
      if(buffer[stringLength] == '\n' || buffer[stringLength] == '\r') {
	buffer[stringLength] = '\0';
      } else {
	stringLength++;
	break;
      }
    }

    if(startsWith(buffer, EXEC_COMMAND)) {
      char* args = skipWhitespace(buffer + EXEC_COMMAND_LENGTH);
      if(*args == '\0') {
	printf("[ui %d] the 'exec' command requires arguments\n", uiPid);
      } else {

	printf("[ui %d] the 'exec' command is not implemented\n", uiPid);

      }
    } else if(startsWith(buffer, SYSTEM_COMMAND)) {
      char* args = skipWhitespace(buffer + SYSTEM_COMMAND_LENGTH);
      if(*args == '\0') {
	printf("[ui %d] the 'system' command requires arguments\n", uiPid);
      } else {

	//printf("[ui %d] forking new child\n", uiPid);

	pid_t pid = fork();
	if(pid == 0) {
	  
	  system(args);
	  return;

	}

	if(pid == -1) {
	  printf("[ui %d] fork failed\n", uiPid);
	} else {

	  printf("[ui %d] created child process %d\n", uiPid, pid);
	  signalManagerThread();
	  //printf("[ui %d] signal done\n", uiPid);
	}
      }
    } else {
      printf("[ui %d] unknown command '%s'\n", uiPid, buffer);
    }
  }

  printf("[ui %d] finish\n", uiPid);

  globalExit = 1;
  signalManagerThread();

  return NULL;
}


int main(int argc, const char* argv[], const char* envp[])
{
  debug = 0;
  globalExit = 0;
  globalEnvp = envp;

  //
  // Start the ui thread
  //
  pthread_t uiThreadInfo;
  int status;

  status = pthread_create(&uiThreadInfo, NULL, uiThread, (void*)NULL);
  if(status) {
    printf("[main] error: pthread_create returned %d\n", status);
    return 1;
  }

  manager();

  return 0;
}
