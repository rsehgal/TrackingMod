/*
 * TestVoxelNavigator.cpp
 *
 *  Created on: 01-Jan-2019
 *      Author: rsehgal
 */

#include "VoxelNavigator.h"
#include "Voxelator_Evolution.h"
#include <G4SystemOfUnits.hh>


int main(){
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,75*cm,10*cm,10*cm,15*cm);
	Tomography::VoxelNavigator v;
	v.Debugger();
}




