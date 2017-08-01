// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Track_rflx

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Track.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *TrackingcLcLVector3DlEdoublegR_Dictionary();
   static void TrackingcLcLVector3DlEdoublegR_TClassManip(TClass*);
   static void *new_TrackingcLcLVector3DlEdoublegR(void *p = 0);
   static void *newArray_TrackingcLcLVector3DlEdoublegR(Long_t size, void *p);
   static void delete_TrackingcLcLVector3DlEdoublegR(void *p);
   static void deleteArray_TrackingcLcLVector3DlEdoublegR(void *p);
   static void destruct_TrackingcLcLVector3DlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tracking::Vector3D<double>*)
   {
      ::Tracking::Vector3D<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Tracking::Vector3D<double>));
      static ::ROOT::TGenericClassInfo 
         instance("Tracking::Vector3D<double>", "base/Vector3D.h", 48,
                  typeid(::Tracking::Vector3D<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TrackingcLcLVector3DlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(::Tracking::Vector3D<double>) );
      instance.SetNew(&new_TrackingcLcLVector3DlEdoublegR);
      instance.SetNewArray(&newArray_TrackingcLcLVector3DlEdoublegR);
      instance.SetDelete(&delete_TrackingcLcLVector3DlEdoublegR);
      instance.SetDeleteArray(&deleteArray_TrackingcLcLVector3DlEdoublegR);
      instance.SetDestructor(&destruct_TrackingcLcLVector3DlEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tracking::Vector3D<double>*)
   {
      return GenerateInitInstanceLocal((::Tracking::Vector3D<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Tracking::Vector3D<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TrackingcLcLVector3DlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Tracking::Vector3D<double>*)0x0)->GetClass();
      TrackingcLcLVector3DlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TrackingcLcLVector3DlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TomographycLcLTrack_Dictionary();
   static void TomographycLcLTrack_TClassManip(TClass*);
   static void *new_TomographycLcLTrack(void *p = 0);
   static void *newArray_TomographycLcLTrack(Long_t size, void *p);
   static void delete_TomographycLcLTrack(void *p);
   static void deleteArray_TomographycLcLTrack(void *p);
   static void destruct_TomographycLcLTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tomography::Track*)
   {
      ::Tomography::Track *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Tomography::Track));
      static ::ROOT::TGenericClassInfo 
         instance("Tomography::Track", "", 21,
                  typeid(::Tomography::Track), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TomographycLcLTrack_Dictionary, isa_proxy, 0,
                  sizeof(::Tomography::Track) );
      instance.SetNew(&new_TomographycLcLTrack);
      instance.SetNewArray(&newArray_TomographycLcLTrack);
      instance.SetDelete(&delete_TomographycLcLTrack);
      instance.SetDeleteArray(&deleteArray_TomographycLcLTrack);
      instance.SetDestructor(&destruct_TomographycLcLTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tomography::Track*)
   {
      return GenerateInitInstanceLocal((::Tomography::Track*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Tomography::Track*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TomographycLcLTrack_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Tomography::Track*)0x0)->GetClass();
      TomographycLcLTrack_TClassManip(theClass);
   return theClass;
   }

   static void TomographycLcLTrack_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_TrackingcLcLVector3DlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Tracking::Vector3D<double> : new ::Tracking::Vector3D<double>;
   }
   static void *newArray_TrackingcLcLVector3DlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Tracking::Vector3D<double>[nElements] : new ::Tracking::Vector3D<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrackingcLcLVector3DlEdoublegR(void *p) {
      delete ((::Tracking::Vector3D<double>*)p);
   }
   static void deleteArray_TrackingcLcLVector3DlEdoublegR(void *p) {
      delete [] ((::Tracking::Vector3D<double>*)p);
   }
   static void destruct_TrackingcLcLVector3DlEdoublegR(void *p) {
      typedef ::Tracking::Vector3D<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Tracking::Vector3D<double>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TomographycLcLTrack(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Tomography::Track : new ::Tomography::Track;
   }
   static void *newArray_TomographycLcLTrack(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Tomography::Track[nElements] : new ::Tomography::Track[nElements];
   }
   // Wrapper around operator delete
   static void delete_TomographycLcLTrack(void *p) {
      delete ((::Tomography::Track*)p);
   }
   static void deleteArray_TomographycLcLTrack(void *p) {
      delete [] ((::Tomography::Track*)p);
   }
   static void destruct_TomographycLcLTrack(void *p) {
      typedef ::Tomography::Track current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Tomography::Track

namespace {
  void TriggerDictionaryInitialization_Track_rflx_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/home/rsehgal/Tomo/TrackingMod/",
"/home/rsehgal/BackBoneSoftwares/root6-master/install/include",
"/home/rsehgal/Tomo/TrackingMod/Helpers/inc/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Track_rflx dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace Tracking{template <class Type> class __attribute__((annotate("$clingAutoload$base/Vector3D.h")))  Vector3D;
}
namespace Tomography{class Track;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Track_rflx dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
/*
 * Track.h
 *
 *  Created on: Jul 5, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_TRACK_H_
#define TEMPLATIZEDINTERFACE_INC_TRACK_H_

#include "base/Vector3D.h"
#include "base/Global.h"

namespace Tomography {

class Track {
	Tracking::Vector3D<double> fP1;
	Tracking::Vector3D<double> fP2;
	Tracking::Vector3D<double> fDirCosine;
public:
  Track();
  Track(Tracking::Vector3D<double> p1,Tracking::Vector3D<double> p2);
  void CalculateDirCosine();
  Tracking::Vector3D<double> GetP1(){return fP1;}
  Tracking::Vector3D<double> GetP2(){return fP2;}
  void SetP1(Tracking::Vector3D<double> val){fP1 = val; CalculateDirCosine();}
  void SetP2(Tracking::Vector3D<double> val){fP2 = val; CalculateDirCosine();}
  Tracking::Vector3D<double> GetDirCosine(){return fDirCosine;}
  void Print(){fP1.Print(); std::cout<<" : " ; fP2.Print();}

  ~Track();
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_TRACK_H_ */

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Tomography::Track", payloadCode, "@",
"Tracking::Vector3D<double>", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Track_rflx",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Track_rflx_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Track_rflx_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Track_rflx() {
  TriggerDictionaryInitialization_Track_rflx_Impl();
}
