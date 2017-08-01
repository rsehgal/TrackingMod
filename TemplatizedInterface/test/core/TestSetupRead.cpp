#include <iostream>
#include <Setup.h>

#undef NDEBUG
#include <cassert>

//Provide path to setup.xml file with filename
int main(int argc, char **argv){
	Tomography::Setup *set = new Tomography::Setup(argv[1]);
	assert(set->GetTriggerTDC()==1);
	assert(set->GetTriggerTDCChannel()==31);

	std::cout<<"--- XML read test passed for Trigger ----" << std::endl;

    assert(set->GetTriggeringPlaneVector().size()==2);
    assert(set->GetDetectorVector().size()==3);
	return 1;
}
