#include <iostream>
#include <Eigen/Dense>
int main() {
  // Define a 3x3 matrix and two 3-dimensional vectors
  Eigen::Matrix3d p;
  p << 1, 2, 8,
       4, 5, 6,
       7, 8, 9;
  Eigen::Vector3d r(10, 11, 12);
  Eigen::Vector3d s(13, 14, 15);
  
  // Matrix/matrix multiplication
  std::cout << "p*p:\n" << p*p << std::endl;

  // Matrix/vector multiplication
  std::cout << "p*r:\n" << p*r << std::endl;
  std::cout << "r^T*p:\n" << r.transpose()*p << std::endl;
  
  // Vector/vector multiplication (inner product)
  std::cout << "r^T*s:\n" << r.transpose()*s << std::endl;

  //Determinant
  std::cout << "pDeterminanat: \n" << p.determinant() << std::endl;

  //Inverse : Exist only if matrix is non singular. ie. determinant != 0
  std::cout << "pInv: \n" << p.inverse() << std::endl;

  double rvect[]={1.,2.,3.};
  Eigen::Vector3d rEigen(3);
  for(int i = 0 ; i < 3 ; i++){
   rEigen(i) = rvect[i];
  }

  std::cout<<"REigen : " << std::endl << rEigen << std::endl;
  std::cout<<"10 - REigen : " << std::endl << (rEigen-Eigen::VectorXd::Ones(10)) << std::endl;
  //std::cout

}
