#include <iostream>
#include <sys/types.h>  // fork, getpid
#include <unistd.h>
#include <sys/wait.h>   // wait
#include <queue>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

queue<int> pids;

void recursive_creation(int leaves)
{
   if (leaves > 1)
   {
      // add current process to stack
      pids.push(getpid());

      // fork left and right children
      int child1, child2, leftLeaves, rightLeaves;
      rightLeaves = leaves / 2;
      leftLeaves = leaves - rightLeaves;

      // FIRST METHOD: FORK BOTH AND LET RUN PARALLEL
      child1 = fork();
      if (child1 < 0) perror("error forking process...");
      else if (child1 == 0)     // left child process
      {
         recursive_creation(leftLeaves);
      }
      else                      // parent process afte first fork
      {
         child2 = fork();
         if (child2 < 0) perror("error forking process...");
         else if (child2 == 0)  // right child process
         {
            recursive_creation(rightLeaves);
         }
         else                   // parent process after second fork
         {
            wait(&child1);
            wait(&child2);
         }
      }
      // END FIRST METHOD

      // SECOND METHOD: FORK FIRST CHILD AND WAIT UNTIL FORKING SECOND
      // this makes no sense because it blocks execution
      // child1 = fork();
      // if (child1 == 0)   // left child process
      // {
      //    recursive_creation(leftLeaves);
      // }
      // wait(&child1);

      // child2 = fork();   // right child process
      // if (child2 == 0)
      // {
      //    recursive_creation(rightLeaves);
      // }
      // wait(&child2);
      // END SECOND METHOD

      exit(0);
   }
   else  // leaf process here
   {
      // print out all processes in the stack
      while (pids.size() > 0)
      {
         cout << pids.front() << " ";
         pids.pop();
      }

      // print out my process id
      cout << getpid() << endl;
      exit(0);
   }
}

int main(int argc, char* argv[])
{
   // validate arguments
   if (argc != 2)
   {
      cerr << "usage: lab1 #leaves" << endl;
      return -1;
   }

   int leaves = atoi(argv[1]);

   // ensure correct usage
   if (leaves < 1)
   {
      cerr << "usage: lab1 #leaves" << endl;
      cerr << "where leaves >= 1"   << endl;
   }

   recursive_creation(leaves);
   return 0;
}