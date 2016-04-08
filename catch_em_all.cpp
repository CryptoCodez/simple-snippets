// Copyright (c) Marc Puttkammer
// Use -std=c++11 To Compile
// Pokemon-Programming

#include <iostream>
#include <string>

using Ash_will = bool;

template <typename Pokemon>
Ash_will catch_em_all( Pokemon Pikachu ) {
  try {
    Pokemon masterball { Pikachu }; // wasting my only masterball for pikachu...
    throw masterball;
    
  } catch ( Pokemon &had_success ) {  // a masterball was used, so it can't fail...
	  std::cout << "You've got: " << had_success << std::endl;
  }
  return true; // Yes Ash will catch 'em all
}

int main() {
  std::string first_pokemon = "pikachu";
  catch_em_all( first_pokemon );
  return 0;
}
