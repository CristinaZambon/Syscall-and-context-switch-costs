
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>
#include <sched.h>


//MARIA CRISTINA ZAMBON
double GetTime ();
int
main (int argc, char *argv[])
{


  int pipe1[2];			//pipe1[0] for reading, while [1] for writing
  int pipe2[2];
  pid_t process_a;
  pid_t process_b;
  char txt[] = "This is the message from A!";
  char buf[50];

  int first_pipe = pipe (pipe1);
  int second_pipe = pipe (pipe2);
  double start = 0, end = 0;
  int REPETITIONS = 20;

  int parentCPU, childCPU;
  cpu_set_t set;

  if (argc != 3)
    {
      fprintf (stderr, "error command line\n");
      exit (EXIT_FAILURE);
    }

  parentCPU = atoi (argv[1]);
  childCPU = atoi (argv[2]);

  if (parentCPU != childCPU)
    {
      printf ("\nthe cpus are different, pick the parentCPU as default\n");
    }
  CPU_ZERO (&set);		//Clears set, so that it contains no CPUs

  if (first_pipe == -1 || second_pipe == -1)
    {
      perror ("pipe");
      exit (1);
    }

  if ((process_a = fork ()) == 0)
    {
      CPU_SET (parentCPU, &set);	//Add CPU cpu to set

      if (sched_setaffinity (getpid (), sizeof (set), &set) == -1)
	{
	  perror ("set cpu for child process");
	  exit (EXIT_FAILURE);
	}


      //Child/process A code
      //the first child/process_a is writing to pipe 1
      close (pipe1[0]);		//close pipe1 for reading as child is writing
      close (pipe2[1]);

      for (int i = 0; i < REPETITIONS; ++i)
	{
	  while (read (pipe2[0], buf, strlen (txt) + 1) <= 0)
	    {
	    }
	  printf ("A reads:  %s \n",
		  buf);

	  write (pipe1[1], txt, strlen (txt) + 1);
	}

    }
  else
    {

      if ((process_b = fork ()) == 0)
	{

	  CPU_SET (parentCPU, &set);

	  if (sched_setaffinity (getpid (), sizeof (set), &set) == -1)
	    {
	      perror ("set cpu for parent process");
	      exit (EXIT_FAILURE);
	    }

	  // Child/process B code 
	  //the second child/process_b is writing to pipe b
	  close (pipe1[1]);
	  close (pipe2[0]);
	  start = GetTime ();

	  for (int i = 0; i < REPETITIONS; ++i)
	    {
	      write (pipe2[1], "hi by B", strlen ("hi by B") + 1);
	      while (read (pipe1[0], buf, strlen (txt) + 1) <= 0)
		{
		}
	      printf
		("B reads:  %s \n",
		 buf);
	    }
	  close (pipe1[0]);
	  close (pipe2[1]);
	  end = GetTime ();

	  printf
	    ("\n The average cost of context switching is: %f seconds \n",
	     (double) ((end - start) / REPETITIONS));
	}
      else
	{

// Parent process waits here for children to terminate.
	  if (waitpid (process_a, NULL, 0) == -1
	      || waitpid (process_b, NULL, 0) == -1)
	    perror ("waitpid");
	  exit (1);
	}

    }

  return 0;
}

double
GetTime ()
{
  struct timeval t;
  int rc = gettimeofday (&t, NULL);
  assert (rc == 0);
  // tv_sec handle the seconds
  //tv_usec the microseconds
  return (double) t.tv_sec + (double) t.tv_usec / 1e6;
}
