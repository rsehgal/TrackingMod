/*
 * VoxelNavigator.cpp
 *
 *  Created on: 01-Jan-2019
 *      Author: rsehgal
 */

#include "VoxelNavigator.h"
#include "Voxelator_Evolution.h"
#include <G4ThreeVector.hh>
#include <G4SystemOfUnits.hh>
#include <iostream>

namespace Tomography {

VoxelNavigator::VoxelNavigator() {
	// TODO Auto-generated constructor stub
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
	double xhalf = voxelator->GetEachVoxelDim().x()/2.;
	double yhalf = voxelator->GetEachVoxelDim().y()/2.;
	double zhalf = voxelator->GetEachVoxelDim().z()/2.;
	fBox = new G4Box("TestVoxel",xhalf,yhalf,zhalf);
	fVoxelizedVolume = new G4Box("TestVoxel", voxelator->GetVoxelizedVolumeDim().x()/2.,
								voxelator->GetVoxelizedVolumeDim().y()/2.,
								voxelator->GetVoxelizedVolumeDim().z()/2.);

}

VoxelNavigator::~VoxelNavigator() {
	// TODO Auto-generated destructor stub
}

double VoxelNavigator::ComputeStep(Tracking::Vector3D<double> point, Tracking::Vector3D<double> dir, Tracking::Vector3D<double> placement){
	//LOGIC to find the step within the voxel, making use of G4Box functions
	//Converting Vector3D point, dir and placement to G4ThreeVector
	G4ThreeVector g4placement(placement.x(),placement.y(),placement.z());
	G4ThreeVector g4point(point.x(),point.y(),point.z());
	G4ThreeVector g4dir(dir.x(),dir.y(),dir.z());
	//Applying transformation to the point
	g4point -= g4placement;
	//Calculating step by calling DistanceToOut of G4Box
	double step=fBox->DistanceToOut(g4point,g4dir);
	return step;
}

EInside VoxelNavigator::IsPointOnSurfaceOfVoxelizedVolume(Tracking::Vector3D<double> point){
	G4ThreeVector g4point(point.x(),point.y(),point.z());
	return fVoxelizedVolume->Inside(g4point);
}

void VoxelNavigator::Debugger(){

	std::cout << "Hello from Voxel Navigator...." << std::endl;
	G4ThreeVector pt(12.5*cm,0.,0.);
	G4ThreeVector dir(1.,0.,0.);
	G4ThreeVector placement(10*cm,0.,0.);

//	pt.setX(pt.x()-placement.x());
//	pt.setY(pt.y()-placement.y());
//	pt.setZ(pt.z()-placement.y());
//
	pt -= placement;

	double step=fBox->DistanceToOut(pt,dir);
	std::cout << "Step : " << step << std::endl;

}

} /* namespace Tomography */
