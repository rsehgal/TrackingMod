/*
 * TestVoxel.cpp
 *
 *  Created on: Aug 11, 2017
 *      Author: rsehgal
 */

#undef NDEBUG

#include "VoxelV2.h"
#include "Voxelator_Evolution.h"
#include "cassert"
#include "base/Vector3D.h"
#include "Track.h"
#include "EventHelper.h"
#include <G4SystemOfUnits.hh>

using Tracking::Vector3D;
using Tomography::evolution::Voxelator;
using Tomography::Track;
using Tomography::Voxel_V2;

int main(){

	//Tomography::
	Voxelator::Create(50*cm,50*cm,45*cm,20*cm,20*cm,18*cm);
	Voxelator *voxelator = Voxelator::instance();
    assert(voxelator->GetTotalNumberOfVoxels() == 125);
    Vector3D<double> pt(0.,0.,0.);
    std::cout<<"VoxelNumber : " << voxelator->GetVoxelNumber(pt) << std::endl;

    for(int i = 0 ; i < voxelator->GetTotalNumberOfVoxels(); i++){
    	Vector3D<double> voxelCenter = voxelator->GetVoxelCenters()[i];
    	std::cout << "Center of Voxel no : " << i << " : ";voxelCenter.Print();
    }

    /*Track incoming(Vector3D<double>(-10.,-10., 60), Vector3D<double>(0.,0.,0.));
    Track outgoing(Vector3D<double>(0.,0.,0.),Vector3D<double>(-10.,-10., -60));

    Tomography::EventHelper v(incoming,outgoing);
    std::cout<<"Deviation : " << v.GetScatteringAngle() << std::endl;
    Vector3D<double> pocaPt = v.GetPocaPt();
    assert(pocaPt.x()==0. && pocaPt.y()==0. && pocaPt.z()==0.);
    std::cout<<"PocaPt : " ; pocaPt.Print();

    std::cout <<"Size of Voxel Vector : " << Voxel::GetVoxelVector().size() << std::endl;
    assert(Voxel::GetVoxelVector().size()==1);
    std::cout<<"Checking Voxel Number from VoxelVector : " << Voxel::GetVoxelVector()[0]->GetVoxelNum() << std::endl;
    assert(Voxel::GetVoxelVector()[0]->GetVoxelNum()==62);



    std::cout<<"---  Inserting New incoming and outgoing tracks ---------" << std::endl;
    incoming.SetP1(Vector3D<double>(-20, -30, 80));
    incoming.SetP2(Vector3D<double>(-22, -37, 40));
    outgoing.SetP1(Vector3D<double>(-21, -39, 89));
    outgoing.SetP2(Vector3D<double>(-29, -30, -67));
    Tomography::EventHelper t(incoming,outgoing);
    assert(Voxel::GetVoxelVector().size()==2);


    for(int i = 0 ; i < Voxel::GetVoxelVector().size() ; i++){
    	//std::cout << "Voxel Number : " << Voxel::GetVoxelVector()[i]->GetVoxelNum() << std::endl;
    	std::cout<<"Address : " << Voxel::GetVoxelVector()[i] << std::endl;
    }

    std::cout<<"---  Inserting Again the Previous incoming and outgoing tracks ---------" << std::endl;
    Tomography::EventHelper u(incoming,outgoing);
    assert(Voxel::GetVoxelVector().size()==2);


    for(int i = 0 ; i < Voxel::GetVoxelVector().size() ; i++){

    	std::vector<Vector3D<double>> pocaPtVector = Voxel::GetVoxelVector()[i]->GetPocaPointsVector();
    	std::cout<<"---- Printing Poca Pts in Voxel : " << Voxel::GetVoxelVector()[i]->GetVoxelNum() << std::endl;
    	for (int j = 0 ; j < pocaPtVector.size() ; j++)
    		pocaPtVector[j].Print();

    }

    std::cout<<"---  Inserting Again the Previous incoming and outgoing tracks four times ---------" << std::endl;
    for(int i = 0 ; i < 4 ; i++){
    	Tomography::EventHelper u(incoming,outgoing);
    }

    for(int i = 0 ; i < Voxel::GetVoxelVector().size() ; i++){

        	std::vector<Vector3D<double>> pocaPtVector = Voxel::GetVoxelVector()[i]->GetPocaPointsVector();
        	std::cout<<"---- Printing Poca Pts in Voxel : " << Voxel::GetVoxelVector()[i]->GetVoxelNum() << std::endl;
        	for (int j = 0 ; j < pocaPtVector.size() ; j++)
        		pocaPtVector[j].Print();

        }

*/
    return 0;
}

