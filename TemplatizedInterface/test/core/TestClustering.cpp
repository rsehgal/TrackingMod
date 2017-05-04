#include "Clustering.h"
#include "base/Vector3D.h"
#include <vector>
using Vec_t = Tracking::Vector3D<double>;
int main(){

	Vec_t pt1(1.,0.,0.),pt2(2.,0.,0.),pt3(3.,0.,0.),pt4(4.,0.,0.);
	std::vector<Vec_t> ptVect;
	ptVect.push_back(pt1);
	ptVect.push_back(pt2);
	ptVect.push_back(pt3);
	ptVect.push_back(pt4);


	//ptVect.push_back(Vec_t(5.,0.,0.));
	ptVect.push_back(Vec_t(6.,0.,0.));
	ptVect.push_back(Vec_t(7.,0.,0.));
	ptVect.push_back(Vec_t(8.,0.,0.));

	new Tracking::Clustering(ptVect);

	return 0;

}
