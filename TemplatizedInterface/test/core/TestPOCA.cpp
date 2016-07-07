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
#include <TApplication.h>
#include "Track.h"
#include <TGeoShape.h>
#include <TGeoBBox.h>
#include <TGeoSphere.h>
#include "Paddle.h"
#include "Imaging.h"
#ifdef USE_EVE
#include "VisualizationHelper.h"
#endif
typedef Tomography::Properties Detector;
using namespace Tomography;
int main(int argc, char *argv[]){
	int numTracks = atoi(argv[1]);
	//double penetration = (double)atoi(argv[2]);
	//std::cout<<"Penetration Level is set to : " << penetration << std::endl;
	TApplication *fApp = new TApplication("Test", NULL, NULL);
#ifdef USE_EVE
	Tomography::VisualizationHelper v;
#endif
	Detector *rpc0 = new GlassRpc(4, "FirstGlassRpc", 60, 31);
	Detector *rpc1 = new GlassRpc(2, "SecondGlassRpc", 30, 31);
	Detector *rpc2 = new GlassRpc(4, "ThirdGlassRpc", -30, 31);
	Detector *rpc3 = new GlassRpc(4, "FourthGlassRpc", -60, 31);
	SetupManager *setup = SetupManager::instance();
	setup->Register(rpc0);
	setup->Register(rpc1);
	setup->Register(rpc2);
	setup->Register(rpc3);

   // Creating and Registering Paddle
    Detector *paddle = new Paddle(2, "Paddle", 0, 15, 10., 10., 10.);
    paddle->GetPlane(0)->GetScintVector()[0]->SetPlacedLocation(Tracking::Vector3D<double>(0., 0., 0.));
    setup->Register(paddle);

    TGeoShape *scatterer = new TGeoBBox("TestScatterer", 5.,5.,5.);
    //TGeoShape *scatterer = new TGeoSphere("TestScatterer", 0.,5.,0.,180.,0.,360.);
    int numOfTracks = numTracks;
    Tomography::SimulateScatteredTracks s(scatterer,"GLASS");
    //Default penetration depth is set to 0.02
    s.SetPenetrationDepth(0.05);
    //s.SetPenetrationDepth(penetration);
    //std::cout<<"Penetration Level is set to : " << penetration << std::endl;
    std::vector<Track> incomingTracksVector;
    std::vector<Track> outgoingTracksVector;
    std::vector<Tracking::Vector3D<double>> poiVect;

    incomingTracksVector.reserve(numOfTracks);
    outgoingTracksVector.reserve(numOfTracks);

#ifdef USE_EVE
	v.Register(rpc0);
	v.Register(rpc1);
	v.Register(rpc2);
	v.Register(rpc3);
	v.Register(paddle);

#endif

    for(int trkNo = 0 ; trkNo < numOfTracks ; trkNo++){
    s.GenerateIncomingTrack();
    s.GenerateOutgoingTrack();
    std::cout<<"--- Scattering point : " ; s.GetScatteringPoint().Print();


    Track incoming = *s.GetIncomingTrack();
    Track outgoing = *s.GetOutgoingTrack();
    incomingTracksVector.push_back(incoming);
    outgoingTracksVector.push_back(outgoing);
    poiVect.push_back(s.GetScatteringPoint());


#ifdef USE_EVE
    /*v.Register(s.GetIncomingTrack());
    v.RegisterLine(s.GetIncomingTrack()->GetP2(), s.GetScatteringPoint());
    v.RegisterLine(s.GetScatteringPoint(), s.GetOutgoingTrack()->GetP1());
    v.Register(s.GetOutgoingTrack());*/

#endif
    }


    //v.RegisterLine(hitVector[0], hitVector[hitVector.size()-1]);


    Tracking::ImageReconstruction im;
    Tracking::Vector3D<double> pocaPt;
    double error = 1e-11;
    double minz=0., maxz=0.;

    incomingTracksVector[0].GetP1().Print();
    incomingTracksVector[1].GetP1().Print();


    //Printing Coordinates of incoming and outgoing tracks

    int mismatchCount = 0;
    for(int i=0 ; i < numOfTracks ; i++){
    	std::cout<<"P1 for IncomingTrack : "; incomingTracksVector[i].GetP1().Print();
    	std::cout<<"P2 for IncomingTrack : "; incomingTracksVector[i].GetP2().Print();
        std::cout << "P1 for OutgoingTrack : "; outgoingTracksVector[i].GetP1().Print();
        std::cout << "P2 for OutgoingTrack : "; outgoingTracksVector[i].GetP2().Print();
        std::cout<< "Scattering Point : "; poiVect[i].Print();//s.GetScatteringPoint().Print();
        
        std::cout<<"---------------------------------------------" << std::endl;
        Tracking::Vector3D<double> p1(0.,0.,0.), q1(0.,0.,0.);
        pocaPt = im.POCA(incomingTracksVector[i].GetP1(),
        		         incomingTracksVector[i].GetDirCosine(),
						 outgoingTracksVector[i].GetP1(),
						 outgoingTracksVector[i].GetDirCosine(),p1,q1);

        std::cout<< "POCA point : "; pocaPt.Print();
#ifdef USE_EVE
        v.RegisterLine(pocaPt,pocaPt);
        v.Register(pocaPt);
#endif

        Tracking::Vector3D<double> diff = pocaPt - poiVect[i];

        if(std::fabs(diff.x()) > error || std::fabs(diff.y()) > error || std::fabs(diff.z()) > error ){
        	mismatchCount++;
        }

              if(pocaPt.z() < minz)
        	minz = pocaPt.z();
        if(pocaPt.z() > maxz)
        	maxz = pocaPt.z();

    }

    std::cout<<"Total Num of mismatched Points : " << mismatchCount << std::endl;
    std::cout<<"MinZ : "<< minz <<" : MaxZ : "<< maxz << std::endl;
    std::cout<<"Estimated Target Thickness : " << (maxz - minz) << std::endl;

#ifdef USE_EVE
	v.Show();
	gEve->DoRedraw3D();
#endif

	fApp->Run();

}

