#include "TestClass.h"

TestClass::TestClass(){
std::cout<<"Constructor called" << std::endl;
}

TestClass::~TestClass(){
std::cout<<"Destructor Called" << std::endl;
}

void TestClass::Print(){
	std::cout<< "A" << std::endl;
}