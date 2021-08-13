/*
 * Singleton.cpp
 *
 *  Created on: Apr 27, 2016
 *      Author: rsehgal
 */

#include "Singleton.h"
namespace Tracking{

Singleton *Singleton::s_instance = 0;

Singleton* Singleton::instance() {
        if (!s_instance)
          s_instance = new Singleton;
        return s_instance;
    }
}//end of Tracking namespace


