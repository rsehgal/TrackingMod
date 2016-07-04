/*
 * TestImaging.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: rsehgal
 */

#include "Imaging.h"
#include "base/Vector3D.h"
#include "base/Global.h"
using namespace Tracking;

int main() {

  Tracking::Vector3D<Precision> p(0., 0., 0.), p1(1., -1., 0);
  Tracking::Vector3D<Precision> q(1., -7., 0.5), q1(0., -8., 0.5);

  Tracking::Vector3D<Precision> u = (p1 - p) / (p1 - p).Mag();
  Tracking::Vector3D<Precision> v = (q1 - q) / (q1 - q).Mag();

  Tracking::ImageReconstruction im;
  Tracking::Vector3D<Precision> t1(0., 0., 0.), t2(0., 0., 0.);
  Tracking::Vector3D<Precision> pocaPt = im.POCA(p, u, q, v, t1, t2);

  pocaPt.Print();
  return 0;
}
