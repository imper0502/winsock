#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */

void *ThreadMain(void *arg);             /* Main program of a thread */

/* Structure of arguments to pass to client thread */
struct ThreadArgs {
    int clntSock;
};

int main(int argc, char *argv[]) {
    DWORD  threadID;                 /* Thread ID from CreateThread() */
    struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    threadArgs->clntSock = 5;
    printf("(Main) Hello World! \n");
    int nResult = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs, 0, (LPDWORD)&threadID);
    if (nResult == NULL) {
        printf("CreateThread() failed");
    }

    printf("Thread %ld created.\n", threadID);
    sleep(10);
    return 0;
}

void *ThreadMain(void *threadArgs) {
    int arg = ((struct ThreadArgs *) threadArgs)->clntSock;
    printf("(Thread) Hello World! ");
    printf(" with arg: %d\n",arg);
    free(threadArgs);              /* Deallocate memory for argument */

    return (NULL);
}
