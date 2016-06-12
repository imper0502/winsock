// =============================================================================
// 最佳的瀏覽效果，請用Notepad++ 開啟。
// =============================================================================
#include <stdio.h>                                                              // for printf(), fprintf()
#include <stdlib.h>                                                             // for exit()
// Main program of a thread.====================================================
void *ThreadMain(void *arg);
// Structure of arguments to pass to client thread.=============================
struct ThreadArgs
{
  int clntSock;
};
// Main=========================================================================
int main(int argc, char *argv[])
{
  // 宣告=======================================================================
  DWORD                                                                threadID;// Thread ID from CreateThread()
  // Pointer to argument structure for thread
  struct ThreadArgs                                                 *threadArgs;
  threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));

  threadArgs->clntSock = 2;

  // TO DO======================================================================
  printf("(Main) Hello World! \n");

  // Create A NEW Thread.=======================================================
  int nResult = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPDWORD)&threadID);
  if (nResult == NULL)
  {
    printf("CreateThread() failed");
  }

  printf("Thread %ld created.\n", threadID);
  sleep(10);
  return 0;
}
// =============================================================================
void *ThreadMain(void *threadArgs)
{
  int arg = ((struct ThreadArgs *) threadArgs)->clntSock;

  printf("(Thread) Hello World! ");
  printf(" with arg: %d\n",arg);
  // Deallocate memory for argument
  free(threadArgs);
  return(NULL);
}