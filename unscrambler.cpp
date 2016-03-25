// =====================================================================================
//
//       Filename:  unscrambler.cpp
//
//    Description:  reads scrambled words from input.txt, 
//                  compares with wordlist.txt and writes to output.txt
//
//        Version:  1.0
//        Created:  03/25/2016 07:33:18 PM
//       Revision:  none
//       Compiler:  g++ ( -std=c++11 )
//
//         Author:  Marc Puttkammer (MP), CryptoCodez@protonmail.com
//   Organization:  
//
// =====================================================================================

#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <fstream>

std::string unscramble( std::string s ) {
  long unsigned int slen { s.length() };
  int c { 0 };
  std::string line;
  std::ifstream myfile( "wordlist.txt" );
  if( myfile.is_open() ) { 
    while( myfile.good() ) {
      std::getline(myfile, line);
      if( slen == line.length() ) {
        std::vector<bool> match( slen );
        std::vector<std::string> used( slen );
        int matchcounter = { 0 };
        for( unsigned int i{ 0 }; i < slen; i++ ) {
          std::string ch { s.substr(i, 1) };
          unsigned int y = { 0 };
          bool found = { false };
          while( y < slen && found == false ) { 
            std::string kh { line.substr(y, 1) };
            if( ch == kh ) {
              if( used[y] == ch ) match[matchcounter] = false;
              else {
                used[y] = ch;
                match[matchcounter] = true;
                matchcounter++;
                found = true;
              }
            }
            y++;
          }
        }
        c = 0;
        for( unsigned int t = 0; t < slen; t++ ) {
          if( match[t] == true ) c++;
        }
        if( c == slen ) {
          std::cout << "Match found: " << line << std::endl;
          return line;
        }
      }
    }
    myfile.close();
  }
}

int main() {
  std::vector<std::string> inKey( 10 );
  std::vector<std::string> outKey( 10 );
  int word = { 0 };

  int counter = { 0 };
  std::string linetemp;
  std::ifstream myfile( "input.txt" );

  if( myfile.is_open() ) {
    while( myfile.good() ) {
      std::getline( myfile, linetemp );
      inKey.at( counter ) = linetemp;
      counter++;
    }
    myfile.close();
  }
  
  for( int i = 0; i < 10; i++ ) {
    word++;
    std::cout << "--- Looking for: [" << inKey[i] << "]\tWord #" << word << std::endl;
    outKey[i] = unscramble(inKey[i]);
  }
  std::ofstream out;
  out.open( "output.txt" );

  for( int z = 0; z < 10; z++ ) {
    if( z == 9 ) out << outKey[z];
    else out << outKey[z] << ",";
  }
  out.close();

  return 0;
}
