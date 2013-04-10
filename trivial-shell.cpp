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
	if (argv[0] == "exit") return 0;
	else if (argv[0] == "cd")  //built-in "cd" command
			chdir(argv[1].c_str());
	else if (argv[0] == "xyzzy")  //built-in "xyzzy" command
    {
		puts("Nothing happens.");
	}
	else // run a program
    {
		pid_t childpid = fork();  //create the child process

		if (childpid < 0)
		{
			puts("Error:Failed to fork.");
			return 1;
		}
		else if (childpid == 0) // the child process
		{
			char* arglist[128];
			for(int k = 0; k < argv.size() && k < 127; ++k)
				strcpy(arglist[k], argv[k].c_str());
		    arglist[argv.size()] = NULL;

			cout << "executing \"" << argv[0] << "\"" << endl;
			execvp(argv[0].c_str(), arglist);
		}
		else  // the parent process
		{
			int status = 0;
			waitpid(childpid, &status, 0); // wait for the child
			cout << "Child process exit status: " << status << endl;
			return status;
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
			if (substr == ";") 
			{
				runit(argv);
				argv.clear();
			}
	        if (substr != "" && substr != ";") argv.push_back(substr);
	    } while (iss);
		
		if (!argv.empty())
		{
			#ifdef DEBUG
			puts("substrings:");
			for(int i = 0; i < argv.size(); ++i)
				puts(argv[i].c_str());
			puts("---------------");
			#endif
			
			runit(argv);
		}
    }
	return 0;
}
