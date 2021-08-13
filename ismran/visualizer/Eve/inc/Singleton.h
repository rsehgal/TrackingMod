/*
 * Singleton.h
 *
 *  Created on: Apr 27, 2016
 *      Author: rsehgal
 */

#ifndef EVE_SINGLETON_H_
#define EVE_SINGLETON_H_

#include "TEveElement.h"
#include "TEveGeoShape.h"
#include <TEveStraightLineSet.h>
#include <TEvePointSet.h>
//#include "Eve/EveVisualizer.h"
namespace Tracking{
class Singleton
{
    int m_value;
    TEveElementList *fEveGeomList;
    TEveStraightLineSet *fLineSet;
    TEvePointSetArray *fPointSetArray;
    //TEvePointSet *fPointSet;
    //EveVisualizer *fEve;
    static Singleton *s_instance;

    Singleton(){
      m_value = 0;
      fEveGeomList = new TEveElementList("Geometry");
      fLineSet = new TEveStraightLineSet();
      //fEve = new EveVisualizer();
    }

    Singleton(int v)
    {
        m_value = v;
    }
  public:
    int get_value()
    {
        return m_value;
    }
    void set_value(int v)
    {
        m_value = v;
    }

    static Singleton *instance();

    /*static Singleton *instance()
    {
        if (!s_instance)
          s_instance = new Singleton;
        return s_instance;
    }*/
    void AddElement(TEveGeoShape *eveShape){
      fEveGeomList->AddElement(eveShape);
    }

    void RemoveElement(TEveGeoShape *eveShape) {
      fEveGeomList->RemoveElement(eveShape);
    }

    TEveElementList* GetList(){return fEveGeomList;}

    void AddElement(TEveStraightLineSet *ls){
    	fLineSet = ls;
    	fEveGeomList->AddElement(fLineSet);
    }

    void AddElement(TEvePointSetArray *pointSetArray){
        fPointSetArray = pointSetArray;
        fEveGeomList->AddElement(fPointSetArray);
    }

    void RemoveElement(){ //TEveStraightLineSet *ls){
        	//fLineSet = ls;
        	fEveGeomList->RemoveElement(fLineSet);
        }

    void AddLineSet(TEveStraightLineSet *ls){
        	fLineSet = ls;
        	fEveGeomList->AddElement(fLineSet);
        }

/*    void AddPointSet(TEvePointSet *ptSet){
            	fPointSet = ptSet;
            	//fEveGeomList->AddElement(fPointSet);
            }
*/
/*    void AddElement(TEvePointSet *ptSet){
                	fPointSet = ptSet;
                //	fEveGeomList->AddElement(fPointSet);
    }
*/

    TEveStraightLineSet *GetLineSet(){
    	return fLineSet;
    }

    //EveVisualizer* GetEveVisualizer(){return fEve;}

};




// Allocating and initializing Singleton's
// static data member.  The pointer is being
// allocated - not the object inself.

//Singleton *Singleton::s_instance = 0;

}//end of Tracking namespace

#endif /* VISUALIZER_EVE_SINGLETON_H_ */
