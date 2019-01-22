/*
 * TestUnscatteredTracks.cpp
 *
 *  Created on: 21-Jan-2019
 *      Author: rsehgal
 *
 *  This test read the Hit Points file like TracksExact.txt,
 *  and will check whether the tracks are unscattered or not.
 *
 *  Useful test to just test hit point when scatterer is not
 *  placed
 *
 *  Work flow: Call the function GetTracksVector() from CommonFunc.h  and then
 *  get the angle between incoming and outgoing vector
 */

#undef NDEBUG

#include "base/Vector3D.h"
#include "Track.h"
#include "CommonFunc.h"
#include <cassert>



using Vec_t = Tracking::Vector3D<double>;
using Tomography::Track;

int main(){

	//-0.017334 -0.0756802 1060 -0.0307131 -0.105072 460 -0.0639876 -0.211711 -440 -0.0966253 -0.264223 -1040
	{
		Track t1(Vec_t(-0.017334, -0.0756802, 1060),Vec_t(-0.0307131 ,-0.105072, 460));
			Track t2(Vec_t(0.0639876, -0.211711, -440),Vec_t(-0.0966253, -0.264223 ,-1040));
			std::cout << "Angle : " << CommonFunc::Functions::instance()->GetAngleInRadian(t1,t2) << std::endl;

	}

	{
	Track t1(Vec_t(0.,0.,10),Vec_t(0.,0.,5));
	Track t2(Vec_t(0.,0.,-5),Vec_t(0.,0.,-10));
	std::cout << "Angle : " << CommonFunc::Functions::instance()->GetAngleInRadian(t1,t2) << std::endl;
	}

	{
	Track t1(Vec_t(10.,20.,10),Vec_t(15.,25.,5));
	Track t2(Vec_t(20.,20.,-5),Vec_t(10.,-15.,-10));
	std::cout << "Angle : " << CommonFunc::Functions::instance()->GetAngleInRadian(t1,t2) << std::endl;

	Vec_t dir = t1.GetDirCosine();
	Track t3(t1.GetP2()+dir*4.,t1.GetP2()+dir*48.);
	std::cout << "Angle : " << CommonFunc::Functions::instance()->GetAngleInRadian(t1,t3) << std::endl;
	}


	std::vector<Track> incoming, outgoing;
	std::vector<double> momentum;
	CommonFunc::GetTracksVector("TrackExact.txt",incoming,outgoing,momentum);
	for (std::size_t i = 0; i < incoming.size(); ++i) {
		double angle = CommonFunc::Functions::instance()->GetAngleInRadian(incoming[i], outgoing[i]);
		std::cout << "Iteration : " << i << " :: Angle : " << angle << std::endl;
		assert(	angle < 1e-8 && "Unexpected Scattered Track found with Scattering angle > 1e-8");
	}
	return 0;
}






