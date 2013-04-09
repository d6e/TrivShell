#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

const unsigned int MAX_ARGS = 128;

using namespace std;

// Execute a parsed command line returning the command's exit code
int doit(const vector<string>& tok)
{
    if (!tok.size() || tok[0] == "") return 0;
    // If user entered cd, change to specified dir. If no dir specified, change to the users home dir
    else if (tok[0] == "cd")
    {
        if (tok.size() > 1) chdir(tok[1].c_str());
        else chdir(getenv("HOME"));
        return 0;
    }

    //Else execute the command specified by the user
    if (pid_t kidpid = fork())
    {
        //Parent
        int status = 0;
        if (tok.back().at(tok.back().size() - 1) != '&')
        {
            waitpid(kidpid, &status, 0);

            #ifdef _INSPECT_EXIT_STATUS
                cout << "exit status=" << status << endl;
            #endif //_INSPECT_EXIT_STATUS
        }
        return status;
    }
    //Child - execute the command
    char* arglist[MAX_ARGS];
    for (unsigned int x = 0; x < tok.size() && x < MAX_ARGS - 1; x++)
        strcpy(arglist[x], tok[x].c_str());
    arglist[tok.size()] = NULL;

    execvp(tok[0].c_str(), arglist);

    //Program will never reach here unless execvp failed
    cerr << "execpv failed: " << strerror(errno) << endl;
    exit(errno);
}

int main(int argc, char* argv[], char* envp[])
{
    while (!cin.eof())
    {
        cout << "? ";
        string temp;
        getline(cin, temp);
        if (temp == "exit") break;

        vector<string> v;
        //Break string into separate strings on whitespace
        {
            stringstream foo(temp);
            string s;
            while (foo >> s)
            {
                if (s[0]=='~') s = getenv("HOME") + s.substr(1);
                v.push_back(s);
            }
        }
        doit(v);
    }
    cout << "exit" << endl;
    return 0;
}
