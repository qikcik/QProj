#include <iostream>
#include <fstream>


int main(int argc, char* argv[])
{
    std::ofstream outfile;
    outfile.open(argv[1], std::ios::trunc);
    outfile << "int add(int a,int b) {return a+b;}";
    return 0;
}
