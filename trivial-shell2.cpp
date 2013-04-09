#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   
#include <signal.h>     // signal name constants and kill() *possibly kill
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>

#include <sstream>
using namespace std;

int runit(const vector<string>& argv)
{
	if (argv[0] == "cd")  //built-in "cd" command
    {
		if (argv[1].c_str() == NULL)
		{
			chdir("/");
		}
		else
		{
			chdir(argv[1].c_str());
		}
	}
	else if (argv[0] == "xyzzy")  //built-in "xyzzy" command
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
			char* arglist[128];
			for(int k = 0; k < argv.size() && k < 127; ++k)
				strcpy(arglist[k], argv[k].c_str());
		    arglist[argv.size()] = NULL;

			
			//the child process
			execvp(argv[0].c_str(), arglist);
			
			perror(argv[0].c_str());
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

int main()
{
	while (!cin.eof())
    {
		// print the prompt
		cout << "|> " ;
		
		// get input
		char cmd[128];
		cin.getline(cmd, 128);
		istringstream iss(cmd);
		
		vector<string> argv;
		do
	    {
	        string substr;
	        iss >> substr;
	        if (substr == "#") break;
	        if (substr != "") argv.push_back(substr);
	    } while (iss);
		
		if (!argv.empty())
		{
		    if (argv[0] == "exit") return 0;
			
			for(int i = 0; i < argv.size(); ++i)
			{
				puts("substrings:");
				puts(argv[i].c_str());
				puts("---------------");
			}
			
			runit(argv);
		}
    }
	return 0;
}
