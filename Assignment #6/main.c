#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define PROGRAM_DURATION 30
#define  SLEEP_DURATION 3

// registration timers
 struct itimerval timer;
 time_t startTime;

 struct timeval start, stop;
 
typedef struct {
    int id;
    int messageCount;
} CHILD;

int main() {
    char buffer[128];
    int result, nread;

    fd_set inputs, inputfds;  // sets of file descriptors
    struct timeval timeout;
    
    // set up rand
    srand(time(NULL));

    // set the timer
    timer.it_value.tv_sec = PROGRAM_DURATION;
    setitimer(ITIMER_REAL, &timer, NULL);
    
    gettimeofday(&start, NULL);
    
    // start the timer
    time(&startTime);
    
    // random sleep time
    sleep(2);
    
    gettimeofday(&stop, NULL);

    long elapsedTime = stop.tv_usec - start.tv_usec;
    printf("%lu", elapsedTime);
    
    FD_ZERO(&inputs);    // initialize inputs to the empty set
    FD_SET(0, &inputs);  // set file descriptor 0 (stdin)

    //  Wait for input on stdin for a maximum of 2.5 seconds.    
    for (;;)  {
        inputfds = inputs;
        
        // 2.5 seconds.
        timeout.tv_sec = 2;
        timeout.tv_usec = 500000;

        // Get select() results.
        result = select(FD_SETSIZE, &inputfds, 
                        (fd_set *) 0, (fd_set *) 0, &timeout);

        // Check the results.
        //   No input:  the program loops again.
        //   Got input: print what was typed, then terminate.
        //   Error:     terminate.
        switch(result) {
            case 0: {
                printf("@");
                fflush(stdout);
                break;
            }
            
            case -1: {
                perror("select");
                exit(1);
            }

            // If, during the wait, we have some action on the file descriptor,
            // we read the input on stdin and echo it whenever an 
            // <end of line> character is received, until that input is Ctrl-D.
            default: {
                if (FD_ISSET(0, &inputfds)) {
                    ioctl(0,FIONREAD,&nread);
                    
                    if (nread == 0) {
                        printf("Keyboard input done.\n");
                        exit(0);
                    }
                    
                    nread = read(0,buffer,nread);
                    buffer[nread] = 0;
                    printf("Read %d characters from the keyboard: %s", 
                           nread, buffer);
                }
                break;
            }
        }
    }
    
    /**
* Print a line for each event:
* elapsed time
* who is registering from what queue
* who is waiting in what queue
* what action they take: Register/drop/gaveup and where
*/
void print(char *event){
    time_t now;
    time(&now);
    
    double elapsed = difftime(now, startTime);
    int min = 0;
    int sec = (int) elapsed;

    while (sec >=60){
        min++;
        sec -=60;
    }

    // Elapsed time.
    printf("%1d:%02d | ", min, sec);

    //What they are doing
    printf(event);
    printf("\n");
}
}
