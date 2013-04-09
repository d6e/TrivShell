#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;

int main()
{
	pid_t pid;
	
	while ( true )
	{
		cout << ":> " << flush;
		char cmd[128];
		cin.getline(cmd,128);
		
		
		
		if((pid = fork()) < 0)
		{
			cout << "an error occurred while forking" << endl;
		}
		else if(pid == 0)
		{
			int status;
			
			status = execv(const char *path, char *const argv[]);
			
			
			cout << "child pid: " << waitpid(status) << endl;
			//cout << "child pid: " << getpid() << endl;
			
			
			execv(cmd,NULL);
			cout << "if this line is printed then execv failed" << endl;
		}
		else
		{
			cout << "success-baby.jpg" << endl;
		}
				
	}
return 0;
}
















/*
      .'``'.      ...
     :o  o `....'`  ;
     `. O         :'
       `':          `.
         `:.          `.
          : `.         `.
         `..'`...       `.
                 `...     `.
                     ``...  `.
                          `````.
                 ghost in the shell
*/
