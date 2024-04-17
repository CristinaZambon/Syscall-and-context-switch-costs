//MARIA CRISTINA ZAMBON mz348
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <assert.h>

double GetTime();

int main(int argc, char *argv[])
{    char ch[1];
    
    if (argc != 1) {
	fprintf(stderr, "error in command line>\n");
	exit(1);
    }
    
    int i=0;
   double start=0, end=0;
   int n=20;
   
   start=GetTime();
    printf("\nStart time of syscall: %f \n",start);
   while(i<n){
   read(0, &ch, 0);
 	++i;
    }
    end=GetTime();
    printf("\nEnd time of syscall: %f \n",end);
    printf("\nAverage time of syscall in microseconds: %f \n", (double) 1e6*(end-start)/(double)n);
    
    return 0;
}

double GetTime() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
           	// tv_sec handle the seconds
           	//tv_usec the microseconds
    return (double) t.tv_sec + (double) t.tv_usec/1e6;
}
