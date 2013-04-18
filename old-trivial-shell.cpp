#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   
#include <signal.h>     
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

int main()
{
	vector<char*> args;
	bool commented = false;
	bool prompt = true;
	while (!cin.eof())
    {
    	if(prompt == true)
			cout << "|> "; // print the prompt
		else
			prompt = true;

		// get input
		char cmd[128];
		cin.getline(cmd, 128);

		char* program = strtok(cmd, " "); //collect token strings
		
	    while(program != NULL)
	    {
	    	char** argv = new char*[args.size() + 2];

			for (int k = 0; k < args.size(); k++)
			{
				argv[k] = args[k];
			}
			
			argv[args.size()] = NULL; //make sure the end is null
			
			if(strcmp(program, ";") == 0 && !args.empty()) // run a program
		    {
				pid_t childpid = fork();

				if (childpid < 0)
				{
					delete[] argv;
					perror("Error:Failed to fork.");
					return 1;
				}
				else if (childpid == 0) // The child
				{
					cout << "executing \"" << argv[0] << "\"" << endl;
					execvp(argv[0], argv);
					delete[] argv;
					perror(argv[0]);
					return 1;
				}
				else
				{
					int status = 0;
					// This is the parent.  Need to wait for the child.
					waitpid(childpid, &status, 0);
					cout << "Child process exit status: " << status << endl;
					for(int i = 0; i < args.size(); i++)
						delete args[i]; 
					args.clear();
				}
			}			
			else if (strcmp(program, "exit") == 0) //exit if the command is "exit"
			{
				delete[] argv;
				return 0;
			}
			else if (strcmp(program, "#") == 0) // comment
			{
				prompt = false;
				delete[] argv;
				break;
			}
			else if (strcmp(program, "xyzzy") == 0)  //built-in "xyzzy" command
		    {
				cout << "Nothing happens." << endl;
				prompt = false;
				delete[] argv;
				break;
			}
			else
			{
				if (strcmp(program, ";") != 0) 
				{
					char *c = new char[128]; // sizeof "hello" will give 6
					memcpy(c, program, 128);
					args.push_back(c);
		        	program = strtok(NULL, " "); //clear program so we can move on
				}
				else
				{
					program = strtok(NULL, " "); //clear program so we can move on
				}
			}
			delete[] argv;
	    }
    }
	return 0;
}