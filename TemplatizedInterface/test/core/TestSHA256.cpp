#include "sha256.h" 
#include <iostream>
int main(int, char**) {
SHA256 sha256;
std::cout << sha256("Hello World how are you  hope you are doing good, what going on in PHD, try to understand some and do some good stuff, lets see how this hashing algorithm works , hope this will solves out problem of getting unique combination idd") << std::endl;
return 0;
}
