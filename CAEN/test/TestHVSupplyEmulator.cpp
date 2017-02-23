#include "HVSupplyEmulator.h"
#include <iostream>
int main(){

    caen::HVSupplyEmulator h;
    std::cout<<"SuppyName : " << h.GetPhysicalSupply().GetName() << std::endl;
    return 0;
}