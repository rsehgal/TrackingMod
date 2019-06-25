#include "sha256.h" 
#include <iostream>
int main(int, char**) {
SHA256 sha256;
std::cout << sha256("Hello World how are you  hope you are doing good, what going on in PHD, try to understand some and do some good stuff, lets see how this hashing algorithm works , hope this will solves out problem of getting unique combination idd") << std::endl;
/*
const char* buffer = "How are you";
std::cout << sha256(buffer, 11) << std::endl;

SHA256 sha256stream;

const char* url = "create.stephan-brumme.com";
int step = 5;
for (int i = 0; i < 25; i += step)   
  sha256stream.add(url + i, step); // add five bytes at a time   std::cout << sha256stream.getHash() << std::endl;
*/  
return 0;
}
