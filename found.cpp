void execute_command(string left_command, string right_command)
{
   //char *argv[] = {"ls", "ls", 0};
   //char *left_arg[] = {"ls", 0};
   //char *right_arg[] = {"grep", "main", 0};
   int p[2];
   int pid;
   int len, len2;
   ParsedTokens left_comm_toks;
   ParsedTokens right_comm_toks;
       
   // int pipe(int filedes[2]);
   // filedes[0] is for reading, filedes[1] is for writing
   if(pipe(p) < 0)
   {
      cerr << strerror(errno) << endl;
      exit(EXIT_FAILURE);
   }
 
   pid = fork();
   if(pid < 0)
   {
      cerr << strerror(errno) << endl;
      exit(EXIT_FAILURE);
   }
   if(pid == 0)
   {
      cout << "exec left arg" << endl;
      char * tempstring;
      tempstring = new char[left_command.length() + 1];
      strcpy(tempstring, left_command.c_str());
      left_comm_toks.ParseAndAddTokens(tempstring);
      len = left_comm_toks[0].length();
      const char * arg0 = left_comm_toks[0].c_str();
      // converts left_command into left_argv[] which execvp uses
      /*len = left_command.length();
      char * const myArr = new char[len + 1];
      for(int x = 0; x < len; x++)
         myArr[x] = left_command[x];
      myArr[len] = '\0';
      char * const left_argv[] = {myArr,0};*/
 
      vector<string> left_args;
 
      for(int i = 0; i < left_comm_toks.GetNumTokens(); i++)
      {
         left_args.push_back(left_comm_toks[i]);
      }
 
      vector<char *> left_argv;
 
      for(int i = 0; i < left_args.size(); i ++)
         left_argv.push_back(const_cast<char *>(left_args[i].c_str()));
 
      close(p[0]);
      dup2(p[1], 1);
      close(p[1]);
      execvp(arg0, &left_argv[0]); 
      cerr << "Error on exec of " << arg0 << ": " << strerror(errno) << endl;
      _exit(errno == ENOENT ? 127 : 126);
 
   }
 
   // do the same thing except for the right command
   pid = fork();
   if(pid < 0)
   {
      cerr << strerror(errno) << endl;
      exit(EXIT_FAILURE);
   }
   if(pid == 0)
   {
      cout << "executing right command" << endl;
      // converts left_command into left_argv[] which execvp uses
      len2 = right_command.length();
      char * const myArr2 = new char[len +1];
      for(int x = 0; x < len2; x++)
         myArr2[x] = right_command[x];
       myArr2[len2] = '\0';
 
      char *const right_argv[] = {myArr2, 0};
 
      close(p[1]);
      dup2(p[0], 0);
      close(p[0]);
      execvp(right_argv[0], right_argv);
      cerr << "Error on exec of " << right_argv[0] << ": " << strerror(errno) << endl;
      _exit(errno == ENOENT ? 127 : 126);
   }
   close(p[0]);
   close(p[1]);
 
   wait(0);
   wait(0);
    
}