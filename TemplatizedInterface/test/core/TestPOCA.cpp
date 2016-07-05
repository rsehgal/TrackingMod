/*
 * TestPOCA.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: rsehgal
 */


#include "SimulateScatteredTracks.h"
#include "Properties.h"
#include "GlassRpc.h"
#include "SetupManager.h"
#include "VisualizationHelper.h"
#include <TApplication.h>
#include "Track.h"
#include <TGeoShape.h>
#include <TGeoBBox.h>
#include "Paddle.h"
typedef Tomography::Properties Detector;
using namespace Tomography;
int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	Tomography::VisualizationHelper v;
	Detector *rpc0 = new GlassRpc(4, "FirstGlassRpc", 60, 31);
	Detector *rpc1 = new GlassRpc(2, "SecondGlassRpc", 30, 31);
	Detector *rpc2 = new GlassRpc(4, "ThirdGlassRpc", -30, 31);
	Detector *rpc3 = new GlassRpc(4, "FourthGlassRpc", -60, 31);
	SetupManager *setup = SetupManager::instance();
	setup->Register(rpc0);
	setup->Register(rpc1);
	setup->Register(rpc2);
	setup->Register(rpc3);
	v.Register(rpc0);
	v.Register(rpc1);
	v.Register(rpc2);
	v.Register(rpc3);


/*
	Tomography::SimulateScatteredTracks s("GLASS");
	std::vector<Tracking::Vector3D<double>> hitVector = s.GetHitVector();
	Track t(hitVector[0], hitVector[hitVector.size()-1]);
	v.Register(&t);
*/

   // Creating and Registering Paddle
    Detector *paddle = new Paddle(2, "Paddle", 0, 15, 10., 10., 10.);
    paddle->GetPlane(0)->GetScintVector()[0]->SetPlacedLocation(Tracking::Vector3D<double>(0., 0., 0.));
    v.Register(paddle);
    setup->Register(paddle);

    TGeoShape *scatterer = new TGeoBBox("TestScatterer", 5.,5.,5.);
    int numOfTracks = 1;
    Tomography::SimulateScatteredTracks s(scatterer,"GLASS");
    while(numOfTracks){
    s.GenerateIncomingTrack();
    s.GenerateOutgoingTrack();
    v.Register(s.GetIncomingTrack());

    v.RegisterLine(s.GetIncomingTrack()->GetP2(),s.GetScatteringPoint());
    v.RegisterLine(s.GetScatteringPoint(),s.GetOutgoingTrack()->GetP1());

    v.Register(s.GetOutgoingTrack());
    numOfTracks--;
    }


    //v.RegisterLine(hitVector[0], hitVector[hitVector.size()-1]);
	v.Show();
	gEve->DoRedraw3D();

	fApp->Run();

}

