#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Error: Not enough arguments provided." << std::endl;
        return 1;
    }

    int n1= stoi(argv[1]);
    int n2= stoi(argv[2]);

    cout << "sum of the number is  " << n1+n2<< endl;
    cout << "product of the number is  " << n1*n2 << endl;
    cout << "division of the number is  " << n1/n2 << endl;

    cout << "subtraction of the number is  " << n1-n2 << endl;


    return 0;
}
