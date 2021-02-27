#include<vector>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<cstdlib>

using namespace std;

int main(int argc, char **argv)
{
	vector<pid_t> masterpro;
	opterr = 0;
      	string temp;
    	int snum = 10, nnum = 5, lnum = 5, cnum = 20, c, status;
	
	while ((c = getopt (argc, argv, "s:n:l:c:")) != -1)
    {
          
          
          if(c=='s')
          {
               temp = optarg;
               snum = atoi(temp.c_str());
               if(snum < 2){
                    cerr << "Error: Minimum 2 streets are required for intersection" << endl;
                    exit(0);
                } 
          }
          else if(c=='n')
          {
              temp = optarg;
                nnum = atoi(temp.c_str());
              
                if(nnum < 1){
                    cerr << "Error: A Street should have minimum 1 line(street line)" << endl;
                    exit(0);
                }
          }
          else if (c=='l')
          {
              temp = optarg;
                lnum = atoi(temp.c_str());
               
                if(lnum < 5){
                    cerr << "Error: Seconds should be more than 5." << endl;
                    exit(0);
                }
          }
          else if(c=='c')
          {
                    temp = optarg;
                cnum = atoi(temp.c_str());
               
                if(cnum < 1){
                    cerr << "Error: K less than 1." << endl;
                    exit(0);
                }
          }
          else if(c=='?')
          {
              if (optopt == 's' || optopt == 'n' || optopt == 'l' || optopt == 'c')
                    cerr << "Error: No argument for the option given" << endl;
                else
                    cerr << "Error: Invalid option." << endl;
                return 1;
          }
          else
          {
              return 0;
          }
       
    }
	

    	char* rgenarg[10];
    	rgenarg[0] = (char*)"./rgen";
    	rgenarg[1] = (char*)"-s";
    	rgenarg[2] = (char*)to_string(snum).c_str();
    	rgenarg[3] = (char*)"-n";
    	rgenarg[4] = (char*)to_string(nnum).c_str();
    	rgenarg[5] = (char*)"-l";
    	rgenarg[6] = (char*)to_string(lnum).c_str();
    	rgenarg[7] = (char*)"-c";
    	rgenarg[8] = (char*)to_string(cnum).c_str();
    	rgenarg[9] = nullptr;

	char* pyarg[3];
    	pyarg[0] = (char*)"/usr/bin/python3";
    	pyarg[1] = (char*)"ece650-a1.py";
    	pyarg[2] = nullptr;

	//***1.Parent process creating the pipe
	int cppTopython[2];
	int pythonTocpp[2];

	pid_t cprocess, pyprocess, c2process;

	pipe(cppTopython);
	pipe(pythonTocpp);

	//***2. Creating 1st child process - tat executes c++->couts streets i.e. input for Assign1 python pgm.
	cprocess = fork();
	if (cprocess == 0)
	{
		close(cppTopython[0]);
		dup2(cppTopython[1], 1);
		
		//close(cppTopython[1]);


		execv("./rgen", rgenarg);
		//execlp("./Assign1-inpout","./Assign1-inpout", nullptr);

	
}
	else if (cprocess < 0)
	{
		cout << "Error: involking cprocess fork()";
	}

	masterpro.push_back(cprocess);
	
	//***3. Creating 2nd child process - tat gets the input from 1st child, and executes the Assign1 python pgm.
	pyprocess = fork();

	if (pyprocess == 0)
	{
		close(cppTopython[1]);
		dup2(cppTopython[0], 0);
		//close(cppTopython[0]);
		//cout<<"1";

		close(pythonTocpp[0]);
		dup2(pythonTocpp[1], 1);

		execv("/usr/bin/python3", pyarg);

		//execlp("/usr/bin/python3", "Assign1.py", nullptr);	
		//execlp("./Assign2out", "./Assign2out", nullptr);
	}
	else if (pyprocess < 0)
	{
		cout << "Error: involking pyprocess fork()";
	}

	masterpro.push_back(pyprocess);

	// 4. Creating 3rd child - tat gets output of python as input to its C++ program.
	c2process = fork();

	if (c2process == 0)
	{
		close(pythonTocpp[1]);
		dup2(pythonTocpp[0], 0);
		execlp("./a2ece650", "./a2ece650", nullptr);
	
	}
	else if (c2process < 0)
	{
		cout << "Error: involking c2process fork()";

	}

	masterpro.push_back(c2process);
	dup2(pythonTocpp[1], 1);
	
	while(true){
        string input;
        getline(cin, input);
        if (cin.eof()){
            break;
        }
        if (input.size() > 0)
            cout << input << endl;
    	}	
	//wait for 1st child to finish
	//waitpid(cprocess, nullptr, 0);

	//Close the write end of pipe, coz 2nd child will continuously receive input and will NOT TERMINATE
	close(cppTopython[1]);

	//wait for 2ns child to finish
	//waitpid(pyprocess, nullptr, 0);

	//Close write end of 2nd pipe, coz 3rd child will continuously receive input and will NOT TERMINATE
	close(pythonTocpp[1]);

	//wait for 3rd(last child) to finish
	//waitpid(c2process, nullptr, 0);

	for (pid_t childpro : masterpro)
	{
		kill (childpro, SIGTERM);
		waitpid(childpro, &status, 0);
	}


return 0;
}
