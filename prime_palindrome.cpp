// =====================================================================================
//
//       Filename:  prime_palindrome.cpp
//
//    Description:  nums up to 1000 that are prime and palindrome
//
//        Version:  1.0
//        Created:  03/25/2016 07:23:25 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Marc Puttkammer (MP), CryptoCodez@protonmail.com
//   Organization:  
//
// =====================================================================================

#include <iostream>
#include <string>
#include <sstream>

std::string to_string( int n )
{
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
}

bool is_prime (int n)
{
	int i, count = 0;
	if (n == 1 || n == 2)
		return true;
	if (n % 2 == 0)
		return false;
	for (i = 1; i <= n; i++)
	{
		if (n % i == 0)
			count++;
	}
	if (count == 2)
		return true;
	else 
		return false;
}

bool str_is_palin(int num_str)
{
	std::string str { to_string(num_str) };
	if (str == std::string(str.rbegin(), str.rend()))
		return true;
	else
		return false;
}

int main()
{
	int high_palin = 1;
	int i = 0;
	for (; i < 1000; i++)
	{
		if(is_prime(i) == true)
		{
			if(str_is_palin(i) == true)
				high_palin = i;
		}	
	}

	std::cout << high_palin << std::endl;
	return 0;
}

