#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   
#include <signal.h>   
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

int runit(char ** argv)
{
 // Execute commands
    switch (fork()) 
    {
        case -1:
            perror("fork");
            return 1;

        case 0:
            execvp(argv[0], argv);
            // execvp only returns on error
            perror("execvp");
            return 1;

        default:
            wait(0);
    }
}

int main() {
    cout << "here" << endl;
    while (std::cin.eof())  // BEGIN prompt update loop
    {
//        cout << endl;  // This ends things   ~(*o*)~  MaGiC!!
        std::cout << "|> " ;
        
        // get input
        char cmd[128];
        cin.getline(cmd, 128);
        istringstream iss(cmd);

        vector<string> args;



        while (true)
        {
            string substr;
            iss >> substr; 
            if (substr == "#") break;
            if (substr == ";" || !iss)  // break out if substr is a ";" or if there is no more string stream
            {
                break;
            }
            else if (substr != "" && substr != " ")  // substr can't be empty or a space
            { 
                args.push_back(substr);
            }
        }


        // collecting command + arguments together
        char **argv = new char*[args.size() + 2];
        argv[args.size() + 1] = NULL;
        for(unsigned int c=0; c<args.size(); c++)
            argv[c+1] = (char*)args[c].c_str();

        runit(argv);

    } // END prompt update loop
    return 0;
}
