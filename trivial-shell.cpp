#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

int main() {
    vector<char*> args;
    while (!cin.eof()) {
        // get input
        char cmd[128];
        cin.getline(cmd, 128);

        char* cmdStr = strtok(cmd, " "); //collect token strings

        while(cmdStr != NULL) {
            char** argv = new char*[args.size() + 2]; //need an array of c-strings

            for (int k = 0; k < args.size(); ++k) {
                argv[k] = args[k];  // load command and arguments into the array of c-strings
            }

            argv[args.size()] = NULL; //make sure the end is null

             // run a command
            if(strcmp(cmdStr, ";") == 0 && !args.empty()) {
                pid_t childpid = fork();

                if (childpid < 0) {
                    delete[] argv;
                    perror("Error: Fork failed.");
                    return 1;
                }
                // The child
                else if (childpid == 0) {
                    cout << "executing \"" << argv[0] << "\"" << endl;
                    execv(argv[0], argv);
                    delete[] argv;
                    perror(argv[0]);
                    return 1;
                }
                else {
                    int status = 0;
                    // This is the parent.  Need to wait for the child.
                    waitpid(childpid, &status, 0);
                    cout << "Child process exit status: " << status << endl;
                    for(int i = 0; i < args.size(); ++i){
                        delete args[i];
                    }
                    args.clear();
                }
            }
            // exit if the command is 'exit'
            else if (strcmp(cmdStr, "exit") == 0) {
                delete[] argv;
                return 0;
            }
            // Support commenting
            else if (strcmp(cmdStr, "#") == 0 || strcmp(cmdStr, "#!") == 0) {
                delete[] argv;
                break;
            }
            //built-in "xyzzy" command
            else if (strcmp(cmdStr, "xyzzy") == 0) {
                cout << "Nothing happens." << endl;
                delete[] argv;
                break;
            }
            else {
                if (strcmp(cmdStr, ";") != 0) {
                    char *c = new char[128]; // have to create a new char string
                    memcpy(c, cmdStr, 128); // otherwise the value of cmdStr gets re-written
                    args.push_back(c);       // while push_back takes an address (took far too long to figure out v.v)
                    cmdStr = strtok(NULL, " "); // then clear cmdStr so we can move on
                }
                else {
                    cmdStr = strtok(NULL, " "); // clear cmdStr so we can move on
                }
            }
            delete[] argv;
        }
    }
    return 0;
}
