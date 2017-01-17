#include "base/Vector3D.h"
#include <iostream>
using Tracking::Vector3D;
int main(){
Vector3D<double> pt(0.832701, -0.472254, -0.261020);
pt.Unit().Print();

return 0;
}
