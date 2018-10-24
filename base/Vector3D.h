/*
 * Vector3D.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef BASE_VECTOR3D_H_
#define BASE_VECTOR3D_H_

#include <iostream>
#include "base/Global.h"
#include <cmath>

#define BINARY_OP(OPERATOR) TRACKING_INLINE Vector3D<Type> operator OPERATOR (Vector3D<Type> vec2) \
													{ \
													return Vector3D<Type>( vec[0] OPERATOR vec2.x(), \
																  	  	   vec[1] OPERATOR vec2.y(), \
																		   vec[2] OPERATOR vec2.z()); \
													}

#define SCALAR_OP(OPERATOR) TRACKING_INLINE Vector3D<Type> operator OPERATOR (Type val) \
													{ \
													return Vector3D<Type>( vec[0] OPERATOR val, \
																  	  	   vec[1] OPERATOR val, \
																		   vec[2] OPERATOR val); \
													}

#define IN_PLACE_BINARY_OP(OPERATOR) TRACKING_INLINE void operator OPERATOR (Vector3D<Type> vec2) \
													{ \
													 vec[0] OPERATOR vec2.x(); \
													 vec[1] OPERATOR vec2.y(); \
													 vec[2] OPERATOR vec2.z(); \
													}

#define IN_PLACE_SCALAR_OP(OPERATOR) TRACKING_INLINE void operator OPERATOR (Type val) \
													{ \
													vec[0] OPERATOR val; \
													vec[1] OPERATOR val; \
													vec[2] OPERATOR val; \
													}




namespace Tracking{
template <class Type>
class Vector3D{
private:
	Type vec[3];
        double fColor;
        std::vector<Type> fGaussProb;
public:
    void SetProbability(int gaussNum, double probValue){
    	fGaussProb[gaussNum] = probValue;
    }

	Vector3D(){
		vec[0]=0.;
		vec[1]=0.;
		vec[2]=0.;
                fColor = 1;
	}

	Vector3D(Type x, Type y, Type z){
			vec[0]=x;
			vec[1]=y;
			vec[2]=z;
			fColor=1;
	}

	Vector3D(Type x, Type y, Type z, Type w){
			vec[0]=x;
			vec[1]=y;
			vec[2]=z;
			fColor=w;
	}

	~Vector3D(){
		fGaussProb.clear();
	}

	TRACKING_INLINE
	void Set(Type x, Type y, Type z){
		vec[0]=x;
		vec[1]=y;
		vec[2]=z;
		fColor=1;
	}

	TRACKING_INLINE
	void Set(Type x, Type y, Type z, Type w){
		vec[0]=x;
		vec[1]=y;
		vec[2]=z;
		//fColor = int(w);
		fColor = w;
	}

        void SetColor(int color){
		fColor=color;
 	}

 	void SetColor(double color){
		fColor=color;
 	}
 	TRACKING_INLINE
	std::vector<double>  GetProbabilityVector()const {return fGaussProb;}

	TRACKING_INLINE
	double GetColor()const {return fColor;}
	TRACKING_INLINE
	Type* GetArr(){return vec;}
	TRACKING_INLINE
	void SetX(Type x){vec[0]=x;}
	TRACKING_INLINE
	void SetY(Type y){vec[1]=y;}
	TRACKING_INLINE
	void SetZ(Type z){vec[2]=z;}
	TRACKING_INLINE
	Type x(){return vec[0];}
	TRACKING_INLINE
	Type y(){return vec[1];}
	TRACKING_INLINE
	Type z(){return vec[2];}
	TRACKING_INLINE
	int w(){return fColor;}
	TRACKING_INLINE
	Type Dot(Vector3D<Type> vec2){
		return vec[0]*vec2.x() + vec[1]*vec2.y() + vec[2]*vec2.z();
	}
	TRACKING_INLINE
	Type Perp2() const {
	    return vec[0]*vec[0]+vec[1]*vec[1];
	  }
	TRACKING_INLINE
	Type Perp() const {
	    return std::sqrt(Perp2());
	  }
	TRACKING_INLINE
	Type Mag2() {
	    return (*this).Dot(*this);
	  }
	TRACKING_INLINE
	Type Mag()  {
	    return std::sqrt(Mag2());
	  }

	TRACKING_INLINE
	Vector3D<Type> Unit(){
		return Vector3D<Type>(this->x()/this->Mag(), this->y()/this->Mag(), this->z()/this->Mag());
	}

	TRACKING_INLINE
	void Print(){
		std::cout<<"("<<vec[0]<<","<<vec[1]<<","<<vec[2]<<")"<<std::endl;
	}

	TRACKING_INLINE
	void operator = (Vector3D<Type> vec2){
		this->vec[0]=vec2.vec[0];
		this->vec[1]=vec2.vec[1];
		this->vec[2]=vec2.vec[2];
	}

	TRACKING_INLINE
    void Transform(Type dx, Type dy, Type dt) {
          Type x, y;
          x = this->x();
          y = this->y();

          this->vec[0] = dx + (std::cos(dt) * x - std::sin(dt) * y);
          this->vec[1] = dy + (std::sin(dt) * x + std::cos(dt) * y);
//          this->vec[0] += dy;
//          this->vec[1] += dx;


        }

	static Type Distance(Vector3D<Type> vec1, Vector3D<Type> vec2){
		return (vec1-vec2).Mag();
	}

        BINARY_OP(+)
	BINARY_OP(-)
	SCALAR_OP(+)
	SCALAR_OP(-)
	SCALAR_OP(/)
	SCALAR_OP(*)
	IN_PLACE_BINARY_OP(+=)
	IN_PLACE_BINARY_OP(-=)
	IN_PLACE_SCALAR_OP(+=)
	IN_PLACE_SCALAR_OP(-=)
	IN_PLACE_SCALAR_OP(*=)
	IN_PLACE_SCALAR_OP(/=)


}; //end of Vector3D class
}// end of Tracking namespace



#endif /* BASE_VECTOR3D_H_ */
