#include <iostream>
#include <cstdlib> // for system function
#include <cstring> // for strcmp

using namespace std;

int main(int argc, char * argv[]) {
    if (argc < 5) {
        cout << "Choose a sufficient number of options" << endl;
        return 1;
    }

    cout << "You have chosen " << argv[1] << " scheduling algorithm" << endl;

    // Prepare command string
    string command = "";

    if (strcmp(argv[1], "SJF") == 0) {
        command = "./sjf ";
    } else if(strcmp(argv[1], "FCFS")==0) {
        command = "./fcfs ";
    }else if(strcmp(argv[1], "LJF")==0) {
        command = "./ljf ";
    }else if(strcmp(argv[1],"RR")==0) {
        command = "./round_robin ";
    }else if(strcmp(argv[1],"MLFQ")==0) {
        command = "./mlfq ";
    }    

    // Concatenate arguments
    for (int i = 4; i < argc; ++i) {
        command += argv[i];
        command += " ";
    }

cout<<argc<<endl;
    // Call the program using system
    cout << "Calling program: " << command << endl;
    system(command.c_str());

    return 0;
}
