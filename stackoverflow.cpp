#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   
#include <signal.h>    
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>
using namespace std;

int runit(char** argv)
{

	cout << "runit works" << endl;

	if (argv[0] == "cd")  //built-in "cd" command
			chdir(argv[1]);
	else if (argv[0] == "xyzzy")  //built-in "xyzzy" command
    {
		puts("Nothing happens.");
	}
	else // run a program
    {
		

		pid_t childpid = fork();  //create the child process
	
		// BEGIN PARENT
		int status = 0;
		waitpid(childpid, &status, 0); // wait for the child
		cout << "Child process exit status: " << status << endl;
		// END PARENT

		if (childpid < 0)
		{
			puts("Error:Failed to fork.");
			return 1;
		}
		else // The child process
		{
/*			char* arglist[128];
			for(int k = 0; k < argv.size() && k < 127; ++k)
				strcpy(arglist[k], argv[k].c_str());
		    arglist[argv.size()] = NULL;
*/
			cout << "executing \"" << argv[0] << "\"" << endl;
			execvp(argv[0], argv);   // Convert this to a *char
		}
	}
}

int main()
{
	vector<string> args;
	bool quitLoop = false;

	while (quitLoop == false)  //prompt update loop
    {
    	cout << endl;  // This ends things   ~(*o*)~  MaGiC!!

		// print the prompt
		cout << "|> " ;
		
		// get input
		char cmd[128];
		cin.getline(cmd, 128);
		istringstream iss(cmd);
		
		if(cin.eof()) //check if file is done
		{
			//quitLoop = true;
		}

		while (iss)
	    {
	        string substr;
	        iss >> substr; 
	        if (substr == "#") break;
			if (substr == ";") 
			{
				char **argv = new char*[args.size() + 2];
				argv[args.size() + 1] = NULL;
			    for(int c = 0; c < args.size(); c++)
			        argv[c] = (char*)args[c].c_str();

//				if (!args.empty())
				runit(argv);
				args.clear();
			}
	        else if (substr != "")
	        { 
	        	args.push_back(substr);
	        }
	    }
    }
	return 0;
}
