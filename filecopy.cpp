// =====================================================================================
//
//       Filename:  filecopy.cpp
//
//    Description:  copy a file
//
//        Version:  1.0
//        Created:  03/25/2016 07:49:05 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Marc Puttkammer (MP), CryptoCodez@protonmail.com
//   Organization:  
//
// =====================================================================================

#include <iostream>
#include <fstream>
#include <memory>
#include <new>

int main( int argc, char *argv[] ) {
	if( argc != 3 ) {
		std::cout << "Usage: " << argv[ 0 ] << " <infile> <outfile>" << std::endl;
	}
	std::ifstream in( argv[ 1 ], std::ios::binary ) ;
	std::ofstream out( argv[ 2 ], std::ios::binary ) ;

	in.seekg(0, in.end);
	std::size_t size { in.tellg() }; 
	in.seekg(0);

	std::unique_ptr<char> buffer { new char[ size ] };

	in.read( buffer.get(), size );
	out.write( buffer.get(), size );

	in.close();
	out.close();

	return 0;
}
