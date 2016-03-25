// =====================================================================================
//
//       Filename:  dec2bin.cpp
//
//    Description:  get a file line by line and convert decimal numbers to binary
//
//        Version:  1.0
//        Created:  03/25/2016 07:26:09 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Marc Puttkammer (MP), CryptoCodez@protonmail.com
//   Organization:  
//
// =====================================================================================

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void dec2bin(int n) {
    if(n != 0) 
        dec2bin (n / 2);
    if(n != 0) 
        cout << n % 2;
}

int main(int argc, char *argv[]) 
{
    ifstream stream(argv[1]);
    string line;
    int n, i;
    
    while (getline(stream, line)) 
    {
        i = stoi (line);
        if(i >= 0)
            dec2bin ( i );
        cout << endl;
    }
    return 0;
}
