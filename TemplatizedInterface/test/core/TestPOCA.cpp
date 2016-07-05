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
#include "Imaging.h"
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
    int numOfTracks = 10;
    Tomography::SimulateScatteredTracks s(scatterer,"GLASS");
    std::vector<Track*> incomingTracksVector;
    std::vector<Track*> outgoingTracksVector;

    incomingTracksVector.reserve(numOfTracks);
    outgoingTracksVector.reserve(numOfTracks);

    for(int trkNo = 0 ; trkNo < numOfTracks ; trkNo++){
    s.GenerateIncomingTrack();
    s.GenerateOutgoingTrack();
    std::cout<<"--- Scattering point : " ; s.GetScatteringPoint().Print();
    v.Register(s.GetIncomingTrack());

    incomingTracksVector.push_back(s.GetIncomingTrack());
    outgoingTracksVector.push_back(s.GetOutgoingTrack());


    std::cout<<"Incoming track : ";
    incomingTracksVector[trkNo]->GetP1().Print() ;
    outgoingTracksVector[trkNo]->GetP1().Print();

    v.RegisterLine(s.GetIncomingTrack()->GetP2(),s.GetScatteringPoint());
    v.RegisterLine(s.GetScatteringPoint(),s.GetOutgoingTrack()->GetP1());
    v.Register(s.GetOutgoingTrack());
    //numOfTracks--;
    }


    //v.RegisterLine(hitVector[0], hitVector[hitVector.size()-1]);


    Tracking::ImageReconstruction im;
    Tracking::Vector3D<double> pocaPt;
    double error = 1e-13;
    double minz=0., maxz=0.;

    incomingTracksVector[0]->GetP1().Print();
    incomingTracksVector[1]->GetP1().Print();


    //Printing Coordinates of incoming and outgoing tracks
/*
    int mismatchCount = 0;
    for(int i=0 ; i < numOfTracks ; i++){
    	std::cout<<"P1 for IncomingTrack : "; incomingTracksVector[i]->GetP1().Print();
    	std::cout<<"P2 for IncomingTrack : "; incomingTracksVector[i]->GetP2().Print();
        std::cout << "P1 for OutgoingTrack : "; outgoingTracksVector[i]->GetP1().Print();
        std::cout << "P2 for OutgoingTrack : "; outgoingTracksVector[i]->GetP2().Print();
        std::cout<<"---------------------------------------------" << std::endl;
        Tracking::Vector3D<double> p1(0.,0.,0.), q1(0.,0.,0.);
        pocaPt = im.POCA(incomingTracksVector[i]->GetP1(),
        		         incomingTracksVector[i]->GetDirCosine(),
						 outgoingTracksVector[i]->GetP1(),
						 outgoingTracksVector[i]->GetDirCosine(),p1,q1);

        Tracking::Vector3D<double> diff = pocaPt - s.GetScatteringPoint();

        if(std::fabs(diff.x()) > error || std::fabs(diff.y()) > error || std::fabs(diff.z()) > error ){
        	mismatchCount++;
        }

        std::cout<<"POCA POINT : "; pocaPt.Print();

        if(pocaPt.z() < minz)
        	minz = pocaPt.z();
        if(pocaPt.z() > maxz)
        	maxz = pocaPt.z();

    }

    std::cout<<"Total Num of mismatched Points : " << mismatchCount << std::endl;
    std::cout<<"MinZ : "<< minz <<" : MaxZ : "<< maxz << std::endl;
*/

	v.Show();
	gEve->DoRedraw3D();

	fApp->Run();

}

