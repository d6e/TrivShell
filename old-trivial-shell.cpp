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
	vector<char*> args;
	while (!cin.eof())
    {
		// print the prompt
		cout << "|> " ;
		
		// get input
		char cmd[1000];
		cin.getline(cmd, 1000);

		cout << "cmd: " << cmd << endl;
		// have to put each word into vector
		char* program = strtok(cmd, " "); //collect token strings
		
	    while(program)
	    {
	    	char** argv = new char*[args.size() + 2];
		
			for (int k = 0; k < args.size(); k++)
			{
				argv[k] = args[k];
			}
			
			argv[args.size()] = NULL;
			


			if(strcmp(program, ";") == 0) // run a program
		    {
				pid_t childpid = fork();

				if (childpid < 0)
				{
					perror("Error:Failed to fork.");
					return 1;
				}
				else if (childpid == 0) // The child
				{
					cout << "executing \"" << argv[0] << "\"" << endl;
					execvp(argv[0], argv);
					
					perror(argv[0]);
					return 1;
				}
				else
				{
					int status = 0;
					// This is the parent.  Need to wait for the child.
					waitpid(childpid, &status, 0);
					cout << "Child process exit status: " << status << endl;
				}
				//args.clear();
				break;
			}			
			else if (strcmp(program, "exit") == 0) //exit if the command is "exit"
			{
				return 0;
			}
			else if (strcmp(program, "#") == 0) // comment
			{
				break;
			}
			else if (strcmp(program, "xyzzy") == 0)  //built-in "xyzzy" command
		    {
				cout << "Nothing happens." << endl;
				break;
			}
			else
			{
				args.push_back(program);
	        	program = strtok(NULL, " ");
			}
	      
	    }
/*
		char* temp = program;
		
		cout << "program: " << program << endl;
		while (temp != NULL)
		{	
			args.push_back(temp);
			temp = strtok(NULL, " ");
		}
*/     
		
    }
	return 0;
}