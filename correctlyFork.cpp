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

int main() {
    vector<string> args;

    int test = 0;
    int test1 = 0;
    while (true)  // BEGIN prompt update loop
    {
//        cout << endl;  // This ends things   ~(*o*)~  MaGiC!!
        cout << "|> " ;

        // get input
        char cmd[128];
        cin.getline(cmd, 128);
        istringstream iss(cmd);

        cout << "test00: " << test << endl;
        test++;

        while (iss)
        {
            cout << "test01: " << test1 << endl;
            test1++;

            string substr;
            iss >> substr;
            
           // const char* substr = tempString.c_str();
            
 //           char* substr = new char[tempString.size() + 1];
 //           copy(tempString.begin(), tempString.end(), substr);
 //           substr[tempString.size()] = '\0'; // don't forget the terminating 0


            if (substr == "exit") return 0;
            if (substr == "#") break;
            if (substr == ";")  // break out if substr is a ";" or if there is no more string stream
            {
                cout << "forking stuff!" << endl;

  //              vector<char*> argv(args.begin(), args.end()); 

                



 /*               
                // push NULL to the end of the vector (execvp expects NULL as last element)
                args.push_back(NULL);
                // pass the vector's internal array to execvp
                char **argv = &args[0];
*/
                 // Execute commands
                pid_t childpid = fork();
                if (childpid < 0)
                {
                    perror("Error: Failed to fork");
                    return 1;
                }
                else if (childpid == 0) // the child process
                {
                    cout << "executing: " << argv[0] << endl;
                    execvp(argv[0], argv);
                    // execvp only returns on error
                    perror("execvp");
                    return 1;
                }
                else //Parent
                {
                    int status = 0;
                    waitpid(childpid, &status, 0); // wait for the child
                    cout << "Child process exit status: " << status << endl;
                }


            }
            else if (substr != "" && substr != " ")  // substr can't be empty or a space
            { 
                cout << "pushing back: \"" << substr << "\"" << endl;
                args.push_back(substr);
            }
            else
            {
                cout << "empty or space detected!" << endl;
            }
        }
    } // END prompt update loop
    return 0;
}
