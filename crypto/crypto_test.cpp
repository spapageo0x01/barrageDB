#include <iostream>
#include "sha256.hpp"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    string input1 = "grape";
    string input2 = "grapa";
    string output1 = sha256(input1);
    string output2 = sha256(input2);
 
    cout << "sha256('"<< input1 << "'):" << output1 << endl;
    cout << "sha256('"<< input2 << "'):" << output2 << endl;

    return 0;
}