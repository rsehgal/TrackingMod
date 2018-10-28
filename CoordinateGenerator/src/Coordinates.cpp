#include "Coordinates.h"
#include "Randomize.hh"
#include <math.h>
#include "DetectorMapping.h"
typedef Tomography::Properties Detector;
namespace Tomography {

// Coordinates::Coordinates(){}ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

Coordinates::Coordinates(){
	fP1.Set(0.,0.,0.);
	fP2.Set(0.,0.,0.);
}

Coordinates::Coordinates(std::vector<Detector *> det) {
  detectors = det;
  totalDetector = detectors.size();
}



Coordinates::Coordinates(std::vector<Detector *> det, Vector3D<double> init) {
  detectors = det;
  totalDetector = detectors.size();
  InitialPoint.SetX(init.x());
  InitialPoint.SetY(init.y());
  InitialPoint.SetZ(init.z());
  signal = 1;
}

Coordinates::Coordinates(Vector3D<double> p1, Vector3D<double> p2 ) {
  fP1 = p1;
  fP2 = p2;
}

void Coordinates::InitializeVectors() {

  coordinate.resize(totalDetector);

  stripcoord.resize(totalDetector);

  strip.resize(totalDetector);

}

/*
 * default value of topToDown is set to true.
 *
 * if topToDown is true then it will try to find the point of intersection
 * with detector planes in downward direction
 * otherwise in upward direction
 */
Vector3D<double> Coordinates::GetPOI(Detector *det, bool topToDown){

	Vector3D<double> poi;
	Vector3D<double> dir = fP2-fP1;

	if(!topToDown)
		dir = dir*(-1);

	double zdiff = (det->GetZPos() - fP1.z())/dir.z();
	//std::cout<<"Zdiff : "<< zdiff << std::endl;

	poi.Set(fP1.x()+dir.x()*zdiff,
			fP1.y()+dir.y()*zdiff,
			det->GetZPos());

	return poi;
}



void Coordinates::CoGenerator() {


  double minRandom, maxRandom; // added in new version

  InitializeVectors();


  for (int i = 0; i < detectors.size(); i++) {
    // coordinate[i][2] = (double)((N-1-i)*M);
    coordinate[i].SetZ(detectors[i]->GetZPos());
  }

  srand(time(NULL));
  if (signal == 0) {
    InitialPoint.SetX(GenRandom(-50, 50));
    InitialPoint.SetY(GenRandom(-50, 50));
    InitialPoint.SetZ(detectors[0]->GetZPos() + 30);

    // initial points are still hard coded
  }
  coordinate[totalDetector - 1].SetX(GenRandom(detectors[totalDetector - 1]->GetPlane(0)->GetBreadth() / (-2),
                                               detectors[totalDetector - 1]->GetPlane(0)->GetBreadth() / 2));
  coordinate[totalDetector - 1].SetX(GenRandom(detectors[totalDetector - 1]->GetPlane(1)->GetLength() / (-2),
                                               detectors[totalDetector - 1]->GetPlane(1)->GetLength() / 2));

   for (int i = 0; i < totalDetector - 1; i++) {
    // temp = ( (i+1) * (double)M - coordinate[0][2] ) / ( coordinate[0][2] - coordinate[N-1][2]);
    // coordinate[N-i-2][0] = (coordinate[0][0] - coordinate[N-1][0]) * temp + coordinate[0][0];
    // coordinate[N-i-2][1] = (coordinate[0][1] - coordinate[N-1][1]) * temp + coordinate[0][1];
    temp = (coordinate[totalDetector - i - 2].z() - InitialPoint.z()) /
           (InitialPoint.z() - coordinate[totalDetector - 1].z());
    coordinate[totalDetector - i - 2].SetX((InitialPoint.x() - coordinate[totalDetector - 1].x()) * temp +
                                           InitialPoint.x());
    coordinate[totalDetector - i - 2].SetY((InitialPoint.y() - coordinate[totalDetector - 1].y()) * temp +
                                           InitialPoint.y());
  }
}

double Coordinates::GenRandom(double min, double max) {
  return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
}

Vector3D<double> Coordinates::GetSpecificCoordinate(double zpos) {
  Vector3D<double> vec;
  temp = (zpos - InitialPoint.z()) / (InitialPoint.z() - coordinate[totalDetector - 1].z());
  vec.SetX((InitialPoint.x() - coordinate[totalDetector - 1].x()) * temp + InitialPoint.x());
  vec.SetY((InitialPoint.y() - coordinate[totalDetector - 1].y()) * temp + InitialPoint.y());
  vec.SetZ(zpos);
  return vec;
}

double Coordinates::GenRandomGauss(double mu, double sigma){

	return G4RandGauss::shoot(mu,sigma);
}

Vector3D<double> Coordinates::GetInitialPoint() { return InitialPoint; }

Vector3D<double> Coordinates::GetCoordinate(int detector) {
  Vector3D<double> temp;
  // if(detector > N || detector < 1)     ooooooooooooooooooooooooooooooooo
  if (detector > totalDetector || detector < 1) {
    std::cout << "Inappropriate Request" << std::endl;
    //	return NULL;
  }

  // for(int i = 0 ; i < 3 ; i++)
  //{
  // temp[i] = coordinate[detector - 1][i];
  temp = coordinate[detector - 1];
  //}

  return temp;
}

void Coordinates::SetStripCoordinates() {
 for (int i = 0; i < totalDetector; i++) {
    stripcoord[i].SetX(
        detectors[i]->GetPlane(0)->GetBreadth() / (-2) +
        (strip[i].x()) *
            (detectors[i]->GetPlane(0)->GetBreadth() / detectors[i]->GetPlane(0)->GetNumOfScintillators()) +
        detectors[i]->GetPlane(0)->GetBreadth() / detectors[i]->GetPlane(0)->GetNumOfScintillators() / 2);
    stripcoord[i].SetY(
        detectors[i]->GetPlane(1)->GetLength() / (-2) +
        (strip[i].x()) * (detectors[i]->GetPlane(1)->GetLength() / detectors[i]->GetPlane(1)->GetNumOfScintillators()) +
        detectors[i]->GetPlane(1)->GetLength() / detectors[i]->GetPlane(1)->GetNumOfScintillators() / 2);
    stripcoord[i].SetZ(coordinate[i].z());
  }
}

Vector3D<double> Coordinates::GetStripCoordinate(int detector) {
  Vector3D<double> temp;
 if (detector > totalDetector || detector < 1) {
    std::cout << "Inappropriate Request" << std::endl;
   }

  // for(int i = 0 ; i < 3 ; i++)
  temp = stripcoord[detector - 1];

  return temp;
}

Vector3D<double> Coordinates::GetStripCoordinate(double x, double y, double z) {
  int tmp = 0;
  Vector3D<double> temp;

  // temp.SetX(floor((x + (double)500) / 31.25));
  // temp.SetY(floor((y + (double)500) / 31.25));
  //double stripWidth = 31.25;
	double stripWidth =
			Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength
					/ Tomography::DetectorMapping::instance()->GetNumberOfStripsInEachPlane();
  double halfStripWidth = stripWidth/2.;
//  temp.SetX(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + x*stripWidth + halfStripWidth);
//  temp.SetY(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + y*stripWidth + halfStripWidth);
  temp.SetX(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + (x+0.5)*stripWidth);
  temp.SetY(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + (y+0.5)*stripWidth);

//  temp.SetX(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + (y+1)*halfStripWidth);
//  temp.SetY(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + (x+1)*halfStripWidth);
  temp.SetZ(z);

  return temp;
}

/*
Vector3D<double> Coordinates::GetStripCoordinate(double x, double y) {
  int tmp = 0;
  Vector3D<double> temp;

  // temp.SetX(floor((x + (double)500) / 31.25));
  // temp.SetY(floor((y + (double)500) / 31.25));
  //double stripWidth = 31.25;
	double stripWidth =
			Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength
					/ Tomography::DetectorMapping::instance()->GetNumberOfStripsInEachPlane();
  double halfStripWidth = stripWidth/2.;
  temp.SetX(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + (y+1)*halfStripWidth);
  temp.SetY(-Tomography::DetectorMapping::instance()->GetDetectorVector()[3]->sDetectorLength/2. + (x+1)*halfStripWidth);
  temp.SetZ(0.);

  return temp;
}
*/


#if(0)
Vector3D<double> Coordinates::GetStripCoordinate(Detector *det, double x, double y, double z) {
  Vector3D<double> temp;
  double stripLength = det->GetPlane(0)->GetScintVector()[0]->GetLength()/det->GetPlane(0)->GetNumOfScintillators();
  double stripBreadth = det->GetPlane(1)->GetScintVector()[0]->GetBreadth()/det->GetPlane(0)->GetNumOfScintillators();
  //std::cout<<"StripLenght : " << stripLength << " :  StripBredth : " << stripBreadth << std::endl;
  temp.SetX(-det->GetLength()/2. + (31-x) * stripLength + stripLength/2.);
  temp.SetY(-det->GetBreadth()/2. + y * stripBreadth + stripBreadth/2.);
  temp.SetZ(z);

  return temp;
}
#endif

Vector3D<double> Coordinates::GetStripCoordinate(Detector *det, double x, double y, double z) {
  Vector3D<double> temp;
  double stripLength = det->GetPlane(0)->GetScintVector()[0]->GetLength()/det->GetPlane(0)->GetNumOfScintillators();
  double stripBreadth = det->GetPlane(1)->GetScintVector()[0]->GetBreadth()/det->GetPlane(0)->GetNumOfScintillators();
  //std::cout<<"StripLenght : " << stripLength << " :  StripBredth : " << stripBreadth << std::endl;
  temp.SetX(-det->GetLength()/2. +  x * stripLength + stripLength/2.);
  temp.SetY(-det->GetBreadth()/2. + y * stripBreadth + stripBreadth/2.);
  temp.SetZ(z);

  return temp;
}

void Coordinates::SetStrips() {

  for (int i = 0; i < totalDetector; i++) {

    strip[i].SetX(
        floor((coordinate[i].x() + detectors[i]->GetPlane(0)->GetBreadth() / 2) /
              (detectors[i]->GetPlane(0)->GetBreadth() / detectors[i]->GetPlane(0)->GetNumOfScintillators())));
    strip[i].SetY(floor((coordinate[i].y() + detectors[i]->GetPlane(1)->GetLength() / 2) /
                        (detectors[i]->GetPlane(1)->GetLength() / detectors[i]->GetPlane(1)->GetNumOfScintillators())));
    strip[i].SetZ(coordinate[i].z());
  }
}

void Coordinates::SetStrips(double x, double y, double z) {
  int tmp = 0;
  // while(tmp != N && coordinate[tmp].z()!=z)              ooooooooooooooooooooooooooooooooooooooo
  while (tmp != totalDetector && coordinate[tmp].z() != z)
    tmp++;
  strip[tmp].Set(x, y, z);
}

Vector3D<double> Coordinates::GetStrip(int detector) {
  Vector3D<double> temp;
  if (detector > totalDetector || detector < 1) {
    std::cout << "Inappropriate Request" << std::endl;
    // return NULL;
  }

  temp = strip[detector - 1];

  return temp;
}

void Coordinates::Print() {
  std::cout << "Coordinates" << std::endl;
  std::cout << "" << std::endl;

  for (int i = 0; i < totalDetector; i++) {
    coordinate[i].Print();
  }

  std::cout << "" << std::endl;
  std::cout << "Strips:" << std::endl;
  std::cout << "" << std::endl;

   for (int i = 0; i < totalDetector; i++) {
    strip[i].Print();
  }
}

} // end of Tomography namespace
