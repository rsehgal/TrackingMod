#include "Clustering.h"
#include "base/Vector3D.h"
#include <vector>
using Vec_t = Tracking::Vector3D<double>;
int main(){

	Vec_t pt1(1.,0.,0.),pt2(0.,1.,0.),pt3(0.,0.,1.),pt4(1.,2.,3.);
	std::vector<Vec_t> ptVect;
	ptVect.push_back(pt1);
	ptVect.push_back(pt2);
	ptVect.push_back(pt3);
	ptVect.push_back(pt4);

	new Tracking::Clustering(ptVect);

	return 0;

}
