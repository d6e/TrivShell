#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   
#include <signal.h>     // signal name constants and kill() *possibly kill
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

int main()
{
	while (true)
    {
		// print the prompt
		cout << "|> " ;
		
		// get input
		char cmd[1000];
		cin.getline(cmd, 1000);
		
		char* program = strtok(cmd, " "); //collect token strings
		char* temp = program;
		
		vector<char*> args;
		while (temp != NULL)
		{	
			args.push_back(temp);
			temp = strtok(NULL, " ");
		}
      
		char** argv = new char*[args.size() + 2];
		
		for (int k = 0; k < args.size(); k++)
		{
			argv[k] = args[k];
		}
		
		argv[args.size()] = NULL;
		
		if (strcmp(program, "exit") == 0) //exit if the command is "exit"
		{
			return 0;
		}
		else if (strcmp(program, "cd") == 0)  //built-in "cd" command
	    {
			if (argv[1] == NULL)
			{
				chdir("/");
			}
			else
			{
				chdir(argv[1]);
			}
		}
		else if (strcmp(program, "xyzzy") == 0)  //built-in "xyzzy" command
	    {
			cout << "Nothing happens." << endl;
		}
		else // run a program
	    {
			pid_t childpid = fork();  //create the child process

			if (childpid < 0)
			{
				perror("Error:Failed to fork.");
				return 1;
			}
			else if (childpid == 0) //process creation is successful
			{
				//the child process
				execvp(argv[0], argv);
				
				perror(argv[0]);
				return 1;
			}
			else
			{
				// This is the parent.  Need to wait for the child.
				if (waitpid(childpid, 0, 0) < 0)
				{
					perror("Internal error: cannot wait for child.");
					return 1;
				}
			}
		}
    }
	return 0;
}
